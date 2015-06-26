#include "FPSGameProject.h"

//空気抵抗
const STATIC float WorldAir3D = 0.0009f;
//重力
const STATIC float WorldGravity3D = -0.03f;

VOID	WriteScore( DWORD NowScore)
{
			DWORD score[3];

			//読み込み
			CTkyFileRead read;
			//読み込み
			IF(!read.Open(TEXT("text/score.txt")))
			{
				MessageBoxW(NULL,TEXT("スコアが読み込めません"),TEXT(""),NULL);
				return;
			}
			//キャラ情報読み込み
			WHILE(!read.IsEof())
			{
				wstring key = read.GetToWhiteSpace();
				swscanf_s( key.c_str() , TEXT("%u"),&score[0] );
				key = read.GetToWhiteSpace();
				swscanf_s( key.c_str() , TEXT("%u"),&score[1] );
				key = read.GetToWhiteSpace();
				swscanf_s( key.c_str() , TEXT("%u"),&score[2] );
			}
			//開放
			read.Close();

			//順番入れ替え
			IF( score[0] < NowScore )
			{
				score[2] = score[1];
				score[1] = score[0];
				score[0] = NowScore;
			}
			EIF( score[1] < NowScore )
			{
				score[2] = score[1];
				score[1] = NowScore;
			}
			EIF( score[2] < NowScore )
			{
				score[2] = NowScore;
			}


			//書き込み開始
			FILE* file;
			file = fopen("text/score.txt","w");
			fprintf( file , "%u %u %u",score[0],score[1],score[2] );
			fclose(file);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ロジック用用
// 基本的なFPSロジック
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//コンストラクタ・デストラクタ
	CFPSGameLogic::CFPSGameLogic()
	{
		//世界の設定
		m_world3d = NULL;

		//世界の生成
		m_world3d =	CPhysicsWorld3D::CreatePhysicsWorld3D();
		m_world3d->SetAirGap( WorldAir3D );
		m_world3d->SetGravity( WorldGravity3D );

		m_AdvectureWinodwQueue = NULL;
		m_TextWinodwQueue = NULL;

		m_NowScore = 0;
		//状態
		m_state = TKYFPSLOGIC_READ;
	}
	CFPSGameLogic::CFPSGameLogic(const CFPSGameLogic& f) : IGameLogic(f)
	{
		//世界の設定
		*m_world3d = *f.m_world3d;
	}
	CFPSGameLogic::~CFPSGameLogic()
	{
		IF(m_read!=NULL)m_read->DestroyOwnObject();
		IF(m_loading!=NULL)m_loading->DestroyOwnObject();

		IF(m_world3d!=NULL)m_world3d->DestroyOwnObject();
	}
//GameLogic情報
	//初期化
	BOOL CFPSGameLogic::onCreate() 
	{
		SetWindowTextW( NULL , TEXT("Prototype") );

		//ロード画像
		m_loading = CTexture::CreateTexture( Init3D::CreateInit3D() , "texture/loading.png" , 800,600 );
		//読み込みタスク
		m_read = CTkyReadThread::CreateReadThread( &ResourceManage , &CharaManage , &TextManage  );
		m_read->Run( TKYTHREAD_ONCE_LOOP );

		//フォグ情報の設定
		D3DCAPS9 caps;
		ZeroMemory(&caps, sizeof(D3DCAPS9));    //初期化
		Init3D::CreateInit3D()->GetD3DDevice()->GetDeviceCaps(&caps);
    
		//ピクセルフォグが使えるかどうか確認
		if((caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) == 0)
		{
			MSGBOX( "フォグできません","失敗" );
		}
		else
		{
			//フォグの設定
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0x00, 0x00, 0x00, 0x00)); //白色で不透明
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);      //頂点モード
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);      //頂点モード
			float s = 1 , e = 10;
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGSTART, *(DWORD*) (&s)); //開始位置
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGEND, *(DWORD*) (&e));     //終了位置

		}

		return true;
	}
	//更新( View更新よりも速い )
	VOID CFPSGameLogic::onUpdate(float step) 
	{
		IF( m_state == TKYFPSLOGIC_READ )
		{
			m_loading->Update(step);
			//終了していたら
			IF( m_read->GetParam() == TKYTHREAD_NONE_LOOP )
			{
				//開放
				m_read->DestroyOwnObject();
				m_read = NULL;
				m_loading->DestroyOwnObject();
				m_loading = NULL;

				//ゲームフロー情報取得
				GameFlowManage.LoadText( TEXT("text/stage.txt") , &ResourceManage , &CharaManage , &TextManage , m_info->m_pos , m_info->m_vec );
				GameFlowManage.Run( this );

				m_state = TKYFPSLOGIC_RUN;
			}
		}
		EIF( m_state == TKYFPSLOGIC_RUN )
		{
			//世界の更新
			m_world3d->Update(step);
		}
	}
	VOID CFPSGameLogic::onRender()
	{
		IF( m_state == TKYFPSLOGIC_READ )
		{
			m_loading->DrawTexture(Vector(400,300,0),Vector(),Vector(1,1,1),TKYTEXTURE_TYPE_2D_MODE);
		}
		EIF( m_state == TKYFPSLOGIC_RUN )
		{
		}
	};

	//ビューが消されるときに呼ばれる
	VOID CFPSGameLogic::onDeleteView( LPTkyGameView view )
	{

		//障害物の消去
		IF( view->GetName() == "FPS_Map" )
		{
			m_world3d->DeleteObtaclePhysicsModel( ((LPTkyFPSMapView)view)->GetModel() );
		}
		//モデルの消去
		EIF( view->GetName() == "FPS_Charactor_Hero" || 
			view->GetName() == "FPS_Charactor_Enemy" )
		{
			m_world3d->DeletePhysicsModel( ((LPTkyFPSCharactorView)view)->GetCharactor() );
		}
		//弾の消去
		EIF( view->GetName() == "FPS_Bullet" )
		{
			m_world3d->DeletePhysicsModel( ((LPTkyBulletView)view)->GetBullet() );
		}	
		//テキストの追加
		EIF( view->GetName() == "FPS_TextWinodwView" )
		{
			IF( m_TextWinodwQueue == view )
			{
				m_TextWinodwQueue = NULL;
			}
		}
		//アドヴぇベンチャーの追加
		EIF( view->GetName() == "FPS_AdventureWinodwView" )
		{
			IF( m_AdvectureWinodwQueue == view )
			{
				m_AdvectureWinodwQueue = NULL;
			}
		}

	}
	//View設定時(View情報は自動的にタスクに追加されるのでその他の設定)
	 VOID CFPSGameLogic::onAddView( LPTkyGameView view )
	{

		//障害物の追加
		IF( view->GetName() == "FPS_Map" )
		{
			m_world3d->AddObtaclePhysicsModel( ((LPTkyFPSMapView)view)->GetModel() );
		}

		//モデルの追加
		EIF( view->GetName() == "FPS_Charactor_Hero" || 
			view->GetName() == "FPS_Charactor_Enemy" )
		{
			m_world3d->AddPhysicsModel( ((LPTkyFPSCharactorView)view)->GetCharactor() );

			//AIを入れる
			wstring ai = ((LPTkyFPSCharactorView)view)->GetCharactor()->GetCharaInfo().m_ai;
			wstring move = ((LPTkyFPSCharactorView)view)->GetCharactor()->GetCharaInfo().m_automove;
			LPTkyFPSCharactorView cview = dynamic_cast<LPTkyFPSCharactorView>(view);
			assert( cview != 0 );

			IF(  ai == TEXT("HERO_AI")  )
			{
/*				//書き込みするんだったら
				IF( MessageBoxW(NULL,TEXT("書き込みますか？"),TEXT("選択") , MB_OKCANCEL) == IDOK )
			{
//記録用
					//動作記録ビュー
				LPTkyMovementPathView m = CMovementPathView::CreateMovementPath( move );
				m->SetBase( cview->GetCharactor()->GetCharaInfo().m_first );
				AddGameView( m );
					//キーで制御ビュー
				LPTkyFpsMoveAIView l =  CFpsMoveAIView::CreateFpsMoveAI( cview , m_info->m_pos , m_info->m_vec ) ;
				l->SetBaseVector(cview->GetCharactor()->GetCharaInfo().m_first);
				AddGameView( l );
			}
				else
*/				{
//実行用
					//自動制御ビュー
					AddGameView( CFpsAutoAIView::CreateFpsAutoAI( cview , m_info->m_pos , m_info->m_vec , move.c_str() ) );
				}

			}
			EIF( ai == TEXT("NORMAL_AI") )
			{
				AddGameView( CFpsEnemyAIView::CreateFpsEnemyAI( cview ) );
			}
			EIF( ai == TEXT("ATTACK_LEAVE_AI") )
			{
				AddGameView( CFpsEnemyAttackLeaveAIView::CreateFpsEnemyAttackLeaveAI(cview) );
			}
			EIF( ai == TEXT("STOP_ATTACK_AI") )
			{
				AddGameView( CFpsEnemyStopAttackAIView::CreateFpsEnemyStopAttackAI( cview ) );
			}
			EIF( ai == TEXT("THROW_AI") )
			{
				AddGameView( CFpsEnemyThrowAIView::CreateFpsEnemyThrowAI( cview ) );
			}

		}

		//テキストの追加
		EIF( view->GetName() == "FPS_TextWinodwView" )
		{
			//殻じゃなかったら
			IF(m_TextWinodwQueue!=NULL)
			{
				LPTkyTextWindowiew im = m_TextWinodwQueue;
				im->SetNextTextWinodw( dynamic_cast<LPTkyTextWindowiew>(view) );
				this->DeleteGameViewOnlyIndex(view);
			}
			else
			{
				//要素の追加(殻だったら)
				m_TextWinodwQueue = dynamic_cast<LPTkyTextWindowiew>(view);
			}
		}

		//アドヴぇん茶の追加
		EIF( view->GetName() == "FPS_AdventureWinodwView" )
		{
			//殻じゃなかったら
			IF(m_AdvectureWinodwQueue!=NULL)
			{
				LPTkyAdventureWindowiew im = m_AdvectureWinodwQueue;
				(im)->SetNextAdventureWinodw( dynamic_cast<LPTkyAdventureWindowiew>(view) );
				//開放なしでプロジェクトから消す
				this->DeleteGameViewOnlyIndex(view);
			}
			else
			{
				//要素の追加(殻だったら)
				m_AdvectureWinodwQueue = dynamic_cast<LPTkyAdventureWindowiew>(view);
			}
		}

		//弾の追加
		EIF( view->GetName() == "FPS_Bullet" )
		{

			//弾追加時に寿命と速度を設定する
			//マウス情報構造体から今のスクリーン座標を取得し、ワールド座標を取得する。
			//↑を実行するには、環境クラスの返還関数を利用する。

			//向き
			Vector vec = Vector();

			IF(  GetModelMessageP( ((LPTkyBulletView)view)->GetWeapon()->GetChara() ) == "HERO_CHARACTOR" )
			{
				//主人公の場合向き取得
//				m_info->m_envi->GetCamera(NULL,&vec,NULL,NULL,NULL,NULL);
				//ワールド変換したベクトル
				vec = m_info->m_envi->GetWorldPosition( m_info->m_absmouse );
			}
			else
			{
				//それ以外は速度を向きととる
				vec = ((LPTkyBulletView)view)->GetBullet()->GetVel();
			}

			//位置取得
			Vector pos = ((LPTkyBulletView)view)->GetBullet()->GetPos();
			vec = vec.Normal();

			//取得したワールド座標より、物理エンジンクラスから、障害物までの距離関数によって
			//寿命となる、距離を取得する。

			float distance = m_world3d->GetDistanceObstacle( pos , vec );

			//寿命を設定する

			TBulletInfo bullet = ((LPTkyBulletView)view)->GetBullet()->GetBulletInfo();

			//武器によってここの設定は変わる
			//弾の場合は距離の短い方が採用となる
			IF(distance > 0)bullet.m_Life = min(distance , bullet.m_Life);

			((LPTkyBulletView)view)->GetBullet()->SetBulletInfo( bullet );

			//速度はワールド座標に攻撃速度を掛け合わせて求めることが出来る。

			((LPTkyBulletView)view)->GetBullet()->SetVel( vec*bullet.m_AttackSpeed );

			//ビューの追加

			m_world3d->AddPhysicsModel( ((LPTkyBulletView)view)->GetBullet() );
		}

	 
	 }
//イベント情報
	//イベントをもらう
	VOID CFPSGameLogic::onEvent( TEventMessage msg )
	{
		IGameLogic::onEvent(msg);
		//ビューの追加、ビューの消去のメッセージをもらう

		//エフェクトビューの追加
		IF( msg.m_message == "FPSLOGIC_ADDVIEW_EFFECT"  )
		{
			this->AddGameView( CEffectView::CreateEffect( 
				(CGameEffect*)(msg.m_value[0]),
				*(Vector*)(msg.m_value[1]),
				*(Vector*)(msg.m_value[2]),
				*(Vector*)(msg.m_value[3]),
				(TKYTEXTURE_TYPE)(msg.m_value[4])
				) );
		}

		EIF( msg.m_message == "FPSLOGIC_ADDVIEW_BULLET"  )
		{
			LPTkyBulletView b = CBulletView::CreateBullet( (LPTkyWeapon)(msg.m_value[0]) );
			//座標
			b->GetBullet()->SetPos( *(Vector*)(msg.m_value[1]) );
			//速度
			b->GetBullet()->SetVel( *(Vector*)(msg.m_value[2])  );
			this->AddGameView(b);
		}

		EIF( msg.m_message == "SEND_SCORE" )
		{
			m_NowScore += msg.m_value[0];
		}

		EIF( msg.m_message == "END_STAGE" )
		{
			//木曽情報取得
			GameFlowManage.GetBaseData();

			//とめる
			GameFlowManage.Stop();

			//文章の終了
			IF( m_TextWinodwQueue!=NULL )
			{
				//連鎖すべて消去
				m_TextWinodwQueue->DeleteNext();
				DELETE_VIEW_MESSAGE( m_TextWinodwQueue );
				m_TextWinodwQueue = NULL;

			}

			//次に進める
			IF(!GameFlowManage.Next())
			{
				//スコアの書き込み
				WriteScore(m_NowScore);
				//スコア
				m_NowScore = 0;

				map< LPTkyGameView , LPTKYMULTITASK > m = m_TaskMap;
				map< LPTkyGameView , LPTKYMULTITASK >::iterator im = m.begin();
				WHILE( im != m.end() )
				{
					DELETE_VIEW_MESSAGE( im->second );
					im++;
				}

				//最初見込み
				GameFlowManage.LoadText( TEXT("text/stage.txt") , &ResourceManage , &CharaManage , &TextManage , m_info->m_pos , m_info->m_vec );
			}
			//しぇ良い度スタート
			CEventManager::StaticAddEventMessage( "START_SHADE" , 100 );

		}

		EIF( msg.m_message == "END_SHADE_IN" )
		{
			//シェイドが終わったら
			IF( msg.m_value[0] == 100 )
			{
				//開始する
				GameFlowManage.Run(this);
			}
		}

		EIF( msg.m_message == "END_TEXTVIEW" )
		{
			//文章の終了
			IF( m_TextWinodwQueue!=NULL )
			{
				//連鎖すべて消去
				m_TextWinodwQueue->DeleteNext();
				DELETE_VIEW_MESSAGE( m_TextWinodwQueue );
				m_TextWinodwQueue = NULL;
			}
		}

		EIF( msg.m_message == "RESET_STAGE" )
		{
			//すべてとめる
			do
			{
				//とめる
				GameFlowManage.Stop();
			}
			while( GameFlowManage.Next() );

			m_NowScore = 0;

			//文章の終了
			IF( m_TextWinodwQueue!=NULL )
			{
				//連鎖すべて消去
				m_TextWinodwQueue->DeleteNext();
				DELETE_VIEW_MESSAGE( m_TextWinodwQueue );
				m_TextWinodwQueue = NULL;
			}

			map< LPTkyGameView , LPTKYMULTITASK > m = m_TaskMap;
			map< LPTkyGameView , LPTKYMULTITASK >::iterator im = m.begin();
			WHILE( im != m.end() )
			{
				DELETE_VIEW_MESSAGE( im->second );
				im++;
			}

			//最初見込み
			GameFlowManage.LoadText( TEXT("text/stage.txt") , &ResourceManage , &CharaManage , &TextManage , m_info->m_pos , m_info->m_vec );
			
			//開始する
			GameFlowManage.Run(this);
		}

		//普通イベント
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//右ボタンで次へ
			IF( message == WM_RBUTTONDOWN )
			{
//				CEventManager::StaticAddEventMessage( "END_STAGE" , 0 );
			}
			EIF( message == WM_CHAR )
			{
//				IF( wParam == 'A' )	CEventManager::StaticAddEventMessage( "END_STAGE" , 0 );
			}
		}

	}
//生成
	LPTkyFPSGameLogic CFPSGameLogic::CreateFPSGameLogic()
	{
		return new CFPSGameLogic();
	}
	LPTkyFPSGameLogic CFPSGameLogic::CreateFPSGameLogic(TDirectXInfo* info)
	{
		LPTkyFPSGameLogic f = CFPSGameLogic::CreateFPSGameLogic();
		f->SetDirectXInfo( info );
		return f;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//アプリケーション用用
// 基本的なアプリケーション
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFPSApplicationLayer::CFPSApplicationLayer()
	{
		m_Logic = CFPSGameLogic::CreateFPSGameLogic( GetDirectXInfoPointa() );

	}
	CFPSApplicationLayer::CFPSApplicationLayer(const CFPSApplicationLayer& f) :IDirectXBaseApplicationLayer(f)
	{
		*this = f;
	}
	CFPSApplicationLayer::~CFPSApplicationLayer()
	{
	}

	//DirectX用仮想関数
	BOOL CFPSApplicationLayer::pOnCreateDirectX()
	{
		//文字の大きさ
//		GetDirectXInfo().m_pFont->SetFontInfo(15,30,3, "メイリオ" );		

		//社で
		m_shade.Read(TEXT("texture/shade.png") );

		//ロジックの初期化
		m_Logic->Create();

		return true;

	}
	BOOL CFPSApplicationLayer::pOnUpdateDirectX(float step)
	{
		m_shade.UpdateShade(step);
		IF( m_shade.IsUpdate() )
		{

			//ロジックの更新
			m_Logic->Update(step);	

		}

		return true;

	}
	VOID CFPSApplicationLayer::pOnRenderDirectX()
	{

		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE); //フォグ：ON

		m_Logic->Render();

		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE); //フォグ：ON

		//テクスチャの描画（アルファのため）
		CTexture::DrawWaitTexture();
		//文字の描画
		CFONT::DrawWaitFont();

		m_shade.DrawShade();

	}
	VOID CFPSApplicationLayer::pOnEventDirectX(TEventMessage msg )
	{
		//終了イベントをもらうことによって次のデータの読み込みが始まる
		IF( msg.m_message == "START_SHADE" )
		{
			m_shade.Run(msg.m_value[0]);
		}

	}