
#include "FPSGameViews.h"

//歩き
const STATIC float WorldWolk3D = 0.001f;
//ジャンプ力
const STATIC float WorldJump3D = 0.1f;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//キャラクタービュー
//　キャラクタ情報の基本となるよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	IFPSCharactorView::IFPSCharactorView()
	{
		//キャラクタ情報
		m_chara = CCharactor::CreateCharactor();
		m_weapon = CWeapon::CreateWeapon(m_chara);
	}
	IFPSCharactorView::IFPSCharactorView(const IFPSCharactorView& f) :IGameView(f)
	{
		assert(m_chara);
		assert(m_weapon);
		*m_chara = *f.GetCharactor();
		*m_weapon = *f.GetWeapon();
	}
	IFPSCharactorView::~IFPSCharactorView()
	{
		m_chara->DestroyOwnObject();
		m_weapon->DestroyOwnObject();
	}
	//設定(データを入れるだけなのでポインタとしてみない)
	BOOL IFPSCharactorView::SetCharactorWeaponData( CCharactor* c, CWeapon* w)
	{
		IF(c!=NULL && m_chara!=NULL)*m_chara = *c;
		IF(w!=NULL && m_weapon!=NULL)*m_weapon = *w;
		m_weapon->SetChara(m_chara);
		return true;
	}
//GameView 要素
	BOOL IFPSCharactorView::onCreate() 	//初期化
	{
		return true;
	}
	BOOL IFPSCharactorView::onUpdate(float step)	//更新
	{
		m_chara->UpdateCharactor(step);
		m_weapon->UpdateAttack(step);

		//もし死んでたら消す
		IF( m_chara->IsDead() )
		{
			DELETE_VIEW_MESSAGE(this);
			return true;
		}

		return true;
	}
	VOID IFPSCharactorView::onRender()	//描画
	{
		m_chara->DrawCharactor();
		m_weapon->DrawAttack();
	}
//イベント要素
	//イベントをもらう
	VOID IFPSCharactorView::onEvent( TEventMessage msg )
	{
		//攻撃をもらって、それによって攻撃を開始する

		//終了をもらって終了する( 終了はポインタによって行い、イベントは来ないかもしれない )


		//死んだ場合は死んだイベント
		//死んだことを伝える

	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSHeroView :: キャラクタービュー
//　主人公用　今のところは何にもすることはない
//　　　　　　主人公が死んだ場合は特殊なことが起きる
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFPSHeroView::CFPSHeroView()
	{
		//接触時のメッセージ設定
		m_chara->SetCollisionMessage( TEventMessage::Initializel( "HERO_CHARACTOR" , (DWORD)m_chara ) );
	}
	CFPSHeroView::CFPSHeroView(const CFPSHeroView& f) :IFPSCharactorView(f)
	{
	}
	CFPSHeroView::~CFPSHeroView()
	{
	}
//GameView 要素
	BOOL CFPSHeroView::onCreate()  	//初期化
	{
		//親呼び出し
		IFPSCharactorView::onCreate();

		m_chara->SetCollisionMessage( TEventMessage::Initializel( "HERO_CHARACTOR" , (DWORD)m_chara ) );

		return true;
	}
	BOOL CFPSHeroView::onUpdate(float step)	//更新
	{
		//親呼び出し
		return IFPSCharactorView::onUpdate(step);


	}
	VOID CFPSHeroView::onRender()	//描画
	{
		//親呼び出し
		IFPSCharactorView::onRender();
	}
//イベント要素
	//イベントをもらう
	VOID CFPSHeroView::onEvent( TEventMessage msg )
	{
		//親呼び出し
		IFPSCharactorView::onEvent(msg);

		//普通イベント
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//左ボタンが押されたら弾発射
			IF( message == WM_LBUTTONDOWN )
			{
				m_weapon->Attack( Vector( (float)LOWORD(lParam) , (float)HIWORD(lParam) , 0  ) );


//				LPTkyBulletView b = CBulletView::CreateBullet( m_weapon );
//				b->GetBullet()->SetPos( m_chara->GetPos() );
//				CEventManager::StaticAddEventMessage( "LOGIC_ADDVIEW" , (DWORD)b );
			}
		}

	}
//独自要素
	LPTkyFPSHeroView CFPSHeroView::CreateFPSHeroView()
	{
		return new CFPSHeroView();
	}
	LPTkyFPSHeroView CFPSHeroView::CreateFPSHeroView( CCharactor* c, CWeapon* w)
	{
		LPTkyFPSHeroView h = CreateFPSHeroView();
		h->SetCharactorWeaponData(c,w);
		return h;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSEnemyView :: キャラクタービュー
//　適用　今のところは何にもすることはない
//　　　　　　敵が死んだら消さなければならない
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFPSEnemyView::CFPSEnemyView()
	{
		//接触時のメッセージ設定
		m_chara->SetCollisionMessage( TEventMessage::Initializel( "ENEMY_CHARACTOR" , (DWORD)m_chara ) );
	}
	CFPSEnemyView::CFPSEnemyView(const CFPSEnemyView& f) :IFPSCharactorView(f)
	{
	}
	CFPSEnemyView::~CFPSEnemyView()
	{
	}
//GameView 要素
	BOOL CFPSEnemyView::onCreate()  	//初期化
	{
		//親呼び出し
		IFPSCharactorView::onCreate();

		m_chara->SetCollisionMessage( TEventMessage::Initializel( "ENEMY_CHARACTOR" , (DWORD)m_chara ) );

		return true;
	}
	BOOL CFPSEnemyView::onUpdate(float step)	//更新
	{
		//親呼び出し
		return IFPSCharactorView::onUpdate(step);
	}
	VOID CFPSEnemyView::onRender()	//描画
	{
		//親呼び出し
		IFPSCharactorView::onRender();
	}
//イベント要素
	//イベントをもらう
	VOID CFPSEnemyView::onEvent( TEventMessage msg )
	{
		//親呼び出し
		IFPSCharactorView::onEvent(msg);

		//敵チェック
		IF( msg.m_message == "ENEMY_CHECK" )
		{
			(*((INT*)msg.m_value[0]))++;
		}

		//終了
		EIF( msg.m_message == "END_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}

		//チェックで終わり
		EIF( msg.m_message == "RESET_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}

	}
//独自要素
	LPTkyFPSEnemyView CFPSEnemyView::CreateFPSEnemyView()
	{
		return new CFPSEnemyView();
	}
	LPTkyFPSEnemyView CFPSEnemyView::CreateFPSEnemyView( CCharactor* c, CWeapon* w)
	{
		LPTkyFPSEnemyView h = CreateFPSEnemyView();
		h->SetCharactorWeaponData(c,w);
		return h;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CBulletView 
//　弾用、追加された後に障害物と比較され寿命が決まる
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CBulletView::CBulletView()
	{
		//物質情報
		m_Bullet = CBullet::CreateBullet();
		//発射位置
		m_BatteryPosition = Vector();
	}
	CBulletView::CBulletView(const CBulletView& b) : IGameView(b)
	{
		assert(m_Bullet);
		*m_Bullet = *b.GetBullet();
	}
	CBulletView::~CBulletView()
	{
		IF( m_Bullet != NULL )m_Bullet->DestroyOwnObject();
	}
//GameView 要素
	BOOL CBulletView::onCreate()		//初期化
	{
		m_Bullet->SetCollisionMessage( TEventMessage::Initializel( "BULLET_MODEL" , (DWORD)m_Bullet ) );

		//初期位置の設定
		m_Bullet->SetBatteryPosition( m_Bullet->GetPos() ) ;
		return true;
	}
	BOOL CBulletView::onUpdate(float step)	//更新
	{
		IF( m_Bullet->IsFinish() )
		{
			CEventManager::StaticAddEventMessage( "LOGIC_DELETEVIEW" , (DWORD)(this) );
		}

		return true;
	}
	VOID CBulletView::onRender()	//描画
	{
		//いちよう描画
		LPTKY3D d = Init3D::CreateInit3D();
		d->SetColor(Vector(0,0,0));
		d->DrawLine3D( m_Bullet->GetPos()-Vector(0,0.1f,0) , m_Bullet->GetPos() + m_Bullet->GetVel() , m_Bullet->GetCircle() );

		//文字の描画==============================

		Vector m_pos  = m_Bullet->GetPos();

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"弾の距離情報:%lf X:%lf Y:%lf Z:%lf ",1.0f,m_pos.x,m_pos.y,m_pos.z );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,350,str);

		//========================================

		//	moderunobyouga 

		IF( m_Bullet->GetChara()!=NULL )
		{
			IF( m_Bullet->GetChara()->GetCharaInfo().m_ai == TEXT("THROW_AI")  )
			{
				Vector rotate = Vector();		//表示回転
				Vector attackdirection = m_Bullet->GetVel();
				rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;

				m_Bullet->GetChara()->GetAnime().DrawAnimationData( TEXT("CHARACTOR_THROW") , m_pos , rotate , Vector(1,1,1) );

			}
		}

	}
//イベント要素
	//イベントをもらう
	VOID CBulletView::onEvent( TEventMessage msg )
	{

		//チェックで終わり
		IF( msg.m_message == "END_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}
		//チェックで終わり
		IF( msg.m_message == "RESET_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}

		EIF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			IF( m_Bullet->GetChara() == ((LPTkyFPSCharactorView)msg.m_value[0])->GetCharactor() )
			{
				m_Bullet->SetChara(NULL);
			}
		}


	}
//独自要素
	//生成
	LPTkyBulletView CBulletView::CreateBullet( )
	{
		return new CBulletView();
	}
	LPTkyBulletView CBulletView::CreateBullet( LPTkyWeapon w )
	{
		LPTkyBulletView b = CreateBullet( );
		b->GetBullet()->SetBulletInfo( w->GetBulletInfo() );
		b->GetBullet()->SetChara(w->GetChara());
		b->SetWeapon(w);
		return b;
	}
	LPTkyBulletView CBulletView::CreateBullet( CBullet* c, LPTkyWeapon w )
	{
		LPTkyBulletView b = CreateBullet( );
		b->SetBullet(c);
		b->SetWeapon(w);
		b->GetBullet()->SetChara( w->GetChara() );
		return b;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMapView 
//　マップはただそこにいるだけでいいんだよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFPSMapView::CFPSMapView()
	{
		m_map = NULL;
	}
	CFPSMapView::CFPSMapView(const CFPSMapView& f)
	{
		m_map = f.GetModel();
	}
	CFPSMapView::~CFPSMapView()
	{
	}
//GameView 要素
	BOOL CFPSMapView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CFPSMapView::onUpdate(float step)	//更新
	{
		return true;
	}
	VOID CFPSMapView::onRender()	//描画
	{
		m_map->DrawMesh( Vector() , Vector() );
	}
//イベント要素
	//イベントをもらう
	VOID CFPSMapView::onEvent( TEventMessage msg )
	{
	}
//独自要素
	//生成
	LPTkyFPSMapView CFPSMapView::CreateMap( )
	{
		return new CFPSMapView();
	}
	LPTkyFPSMapView CFPSMapView::CreateMap( LPTkyXFile x)
	{
		LPTkyFPSMapView f = CreateMap( );
		f->SetModel(x);
		return f;
	}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEffectView 
//　終わったら自分で自動的に終わるよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CEffectView::CEffectView()
	{
		m_Effect = CGameEffect();
		m_pos = Vector();
		m_rotate = Vector();
		m_expand = Vector();
		m_type = TKYTEXTURE_TYPE_3D_MODE;
	}
	CEffectView::CEffectView(const CEffectView& e)
	{
		m_Effect = *e.GetEffect();
		m_pos = e.GetPos();
		m_rotate = e.GetRotate();
		m_expand = e.GetExpand();
	}
	CEffectView::~CEffectView()
	{
	}
//GameView 要素
	BOOL CEffectView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CEffectView::onUpdate(float step)	//更新
	{
		//更新
		m_Effect.UpdateEffect(step);
		//終了チェック
		IF(m_Effect.IsOneLoopEnd())
		{
			DELETE_VIEW_MESSAGE(this);
		}
		return true;
	}
	VOID CEffectView::onRender()	//描画
	{
		IF( m_type == TKYTEXTURE_TYPE_2D_MODE )
		{
			m_Effect.DrawEffect2D(m_pos);
		}
		EIF( m_type == TKYTEXTURE_TYPE_3D_MODE )
		{
			m_Effect.DrawEffect(m_pos,m_rotate,m_expand);
		}
	}
//イベント要素
	//イベントをもらう
	VOID CEffectView::onEvent( TEventMessage msg )
	{
	}
//独自要素
	//生成
	LPTkyEffectView CEffectView::CreateEffect( )
	{
		return new CEffectView();
	}
	LPTkyEffectView CEffectView::CreateEffect( CGameEffect *Effect ,	Vector pos , Vector m_rotate , Vector expand , TKYTEXTURE_TYPE type )
	{
		LPTkyEffectView e = CreateEffect( );
		e->SetEffect( Effect );
		e->SetPos(pos);
		e->SetRotate(m_rotate);
		e->SetExpand(expand);
		e->SetTextureType(type);
		return e;
	}



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//IFpsAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	IFpsAIView::IFpsAIView()
	{
		m_charaview = NULL;
	}
	IFpsAIView::IFpsAIView(const IFpsAIView & f)
	{
		m_charaview = f.GetCharactorView();
	}
	IFpsAIView::~IFpsAIView ()
	{
	}
	VOID IFpsAIView::onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			//担当の敵だったら自分も消す
			IF( (DWORD)m_charaview == msg.m_value[0] )
			{
				DELETE_VIEW_MESSAGE(this);
			}
		}

	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsMoveAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFpsMoveAIView::CFpsMoveAIView()
	{
		m_rotate = Vector();
		
		m_Direction = NULL;
		m_pos = NULL;

		m_mouse = DIMOUSESTATE();
		
		m_BaseVector = Vector(0,0,1);
	}
	CFpsMoveAIView::CFpsMoveAIView(const CFpsMoveAIView& f) :IFpsAIView(f)
	{
		m_rotate = f.m_rotate;
		
		m_Direction = f.m_Direction;
		m_pos = f.m_pos;

		m_mouse = f.m_mouse;

		m_BaseVector = f.m_BaseVector;
	}
	CFpsMoveAIView::~CFpsMoveAIView()
	{
	}
//GameView 要素
	BOOL CFpsMoveAIView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CFpsMoveAIView::onUpdate(float step)	//更新
	{

		*m_pos = m_charaview->GetCharactor()->GetPos();

		Vector v = (m_BaseVector.GetRadVec() - Vector(0,0,1).GetRadVec())*(-1);

		*m_Direction = Vector(0,0,1).Rotate( Vector() , m_rotate.x, m_rotate.y + D3DXToDegree(v.y) , m_rotate.z);

		//今の座標を送信する
		CEventManager::StaticAddEventMessage( "HERO_POSITION" , (DWORD)( m_pos ) );

		return true;
	}
	VOID CFpsMoveAIView::onRender()	//描画
	{

		//文字の描画==============================

		CFONT::CreateFontW(NULL)->SetColor(255,255,255);
		char str[200];
		sprintf_s(str,"距離情報:%lf X:%lf Y:%lf Z:%lf ",1.0f,m_pos->x,m_pos->y,m_pos->z );
		CFONT::CreateFontW(NULL)->DrawFontA(10,130,str);

		//========================================

		//文字の描画==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		sprintf_s(str,"マウス定数 X:%d Y:%d Z:%d ",m_mouse.lX,m_mouse.lY,m_mouse.lZ );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

		//========================================
	}
//イベント要素
	//イベントをもらう
	VOID CFpsMoveAIView::onEvent( TEventMessage msg )
	{

		IFpsAIView::onEvent(msg);

		//モデル情報
		LPPhysicsCircleModel model = this->GetCharactorView()->GetCharactor();

		//移動座標
		Vector move = m_Direction->Normal();
		move.y = 0;

		//キー、マウス操作
		string EventName = msg.m_message;
		IF(EventName == "KEY_INFO")
		{
			//キー情報
			CHAR *key = (CHAR*)msg.m_value[0];
			IF( KEYDOWN( key , DIK_Z ) )
			{
				model->Impact( Vector( 0,WorldWolk3D,0 ) );
			}
			IF( KEYDOWN( key , DIK_X ) )
			{
				model->Impact( Vector( 0,-WorldWolk3D,0 ) );
			}
			IF( KEYDOWN( key , DIK_C ) )
			{
//				m_height += -0.01f;

			}
			IF( KEYDOWN( key , DIK_V ) )
			{
//				m_height -= -0.01f;

			}
			IF( KEYDOWN( key , DIK_SPACE ) )
			{
				model->Impact( Vector( 0,WorldJump3D,0 ) );
			}
		}
		EIF(EventName == "KEY_RIGHT")	//右移動
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*-WorldWolk3D );
		}
		EIF(EventName == "KEY_LEFT")	//左移動
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*WorldWolk3D );
		}
		EIF(EventName == "KEY_UP")		//前進
		{
			model->Impact( move*WorldWolk3D );
		}
		EIF(EventName == "KEY_DOWN")	//後退
		{
			model->Impact( move*-WorldWolk3D );
		}
		EIF(EventName == "MOUSE_INFO")
		{

			m_mouse = *( (DIMOUSESTATE*)msg.m_value[0] );

			m_rotate += Vector( (float)m_mouse.lY / 10.0f , (float)m_mouse.lX / 10.0f , 0 ); 

		}

		IF( msg.m_message == "MOVE_STOP"  )
		{
			//ストップ
			m_charaview->GetCharactor()->SetAcc(Vector());
			m_charaview->GetCharactor()->SetVel(Vector());
		}

	}
//独自要素
	//生成
	LPTkyFpsMoveAIView CFpsMoveAIView::CreateFpsMoveAI( )
	{
		return new CFpsMoveAIView();
	}
	LPTkyFpsMoveAIView CFpsMoveAIView::CreateFpsMoveAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction )
	{
		LPTkyFpsMoveAIView m = CreateFpsMoveAI( );
		m->SetCharactorView( chara );
		m->SetCmameraVector( pos , Direction );
		return m;
	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsAutoAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//カメラの座標、向き
	Vector *m_pos,*m_Direction;

	//オート情報
	LPTkyAutoKeyInput			m_keyinfo;
	LPTkyAutoMouseInput		m_mouseinfo;
*/
	//コンストラクタ・デストラクタ
	CFpsAutoAIView::CFpsAutoAIView(wstring name)
	{
		m_pos = NULL;
		m_Direction = NULL;
		m_keyinfo = CAutoKeyInput::CreateAutoKeyInput();
		m_mouseinfo = CAutoMouseInput::CreateAutoMouseInput();
		m_run = true;
		m_file = name;
	}
	CFpsAutoAIView::CFpsAutoAIView(const CFpsAutoAIView& t)
	{
		m_pos = t.m_pos;
		m_Direction = t.m_Direction;
		*m_keyinfo = *(t.m_keyinfo);
		*m_mouseinfo = *(t.m_mouseinfo);
		m_run = t.m_run;
	}
	CFpsAutoAIView::~CFpsAutoAIView()
	{
		m_keyinfo->DestroyOwnObject();
		m_mouseinfo->DestroyOwnObject();
	}
//GameView 要素
	BOOL GetKeyInfo( wifstream *read, TAutoKeyInput *t)
	{
		wstring wstr;

		WHILE(!read->eof())
		{
			WCHAR word;
			read->get(word);
			IF( word == TEXT('[') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT(']') )
					{
						//数値の取得
						swscanf_s(wstr.c_str(),TEXT("%u"),&t->m_eTkyAutoKey);
						break;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('<') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT('>') )
					{
						//数値の取得
						swscanf_s(wstr.c_str(),TEXT("%u"),&t->m_FrameNum);						
						return false;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('E') )
			{
				WCHAR w[10];
				read->getline(w,10);
				//ループ終了
				return true;
			}
		}
		//ループ終了
		return false;
	}
	BOOL GetMouseInfo( wifstream *read, TAutoMouseInput *t)
	{
		wstring wstr;

		//位置格納
		Vector v = Vector();

		WHILE(!read->eof())
		{
			WCHAR word;
			read->get(word);

			//座標情報
			IF( word == TEXT('[') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT(']') )
					{
						//数値の取得
						swscanf_s(wstr.c_str(),TEXT("%f,%f,%f"),&v.x,&v.y,&v.z);
						break;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('<') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT('>') )
					{
						//数値の取得
						swscanf_s(wstr.c_str(),TEXT("%u"),&t->m_FrameNum);						
						t->m_MousePos = v;
						return false;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('E') )
			{
				WCHAR w[10];
				read->getline(w,10);
				//ループ終了
				return true;
			}
		}
		//ループ終了
		return false;
	}
	BOOL CFpsAutoAIView::onCreate()	//初期化
	{
		//取得をする
		wifstream read;
		read.open(m_file.c_str());

		//基本文字格納用
		WCHAR wstr[MAX_PATH];
		//最後まで
		WHILE(!read.eof())
		{
			//データ取得
			read.getline(wstr,MAX_PATH);

			IF( wstring(wstr) == TEXT("KEYINFO") )
			{
				WHILE(!read.eof())
				{
					TAutoKeyInput t;
					IF( GetKeyInfo(&read,&t) )
					{
						//終了処理をする
						break;
					}
					m_keyinfo->AddKeyInput(t);
				}
			}
			EIF( wstring(wstr) == TEXT("MOUSEINFO") )
			{
				WHILE(!read.eof())
				{
					TAutoMouseInput t;
					IF( GetMouseInfo(&read,&t) )
					{
						//終了処理をする
						break;
					}
					m_mouseinfo->AddMouseInput(t);
				}
			}

		}
		return true;
	}
	BOOL CFpsAutoAIView::onUpdate(float step)	//更新
	{

		//モデル情報
		LPPhysicsCircleModel model = m_charaview->GetCharactor();

		IF(m_run)
		{
			//キー情報
			DWORD key = m_keyinfo->GetNowKeyInput().m_eTkyAutoKey;

			//マウス情報
			Vector move = m_mouseinfo->GetNowMouseInput().Normal();
			move.y = 0;

						IF( key & TKYKEY_RIGHT )	//右移動
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*-WorldWolk3D );
		}
						IF( key & TKYKEY_LEFT )	//左移動
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*WorldWolk3D );
		}
						IF( key & TKYKEY_UP )		//前進
		{
			model->Impact( move*WorldWolk3D );
		}
						IF( key & TKYKEY_DOWN )	//後退
		{
			model->Impact( move*-WorldWolk3D );
		}
		}
		else
		{
			model->SetVel(Vector());
			model->SetAcc(Vector());
		}

		//座標代入
		*m_pos = model->GetPos();
		//マウス情報
		*m_Direction = m_mouseinfo->GetNowMouseInput();

		IF(m_run)
		{

			//情報を進める
			bool end;
			end = m_keyinfo->NextFrame();
			end = m_mouseinfo->NextFrame();

			//シューティングの終了
			IF(!end){
				CEventManager::StaticAddEventMessage("END_SHOOTING_STAGE",NULL);
			}

		}

		//今の座標を送信する
		CEventManager::StaticAddEventMessage( "HERO_POSITION" , (DWORD)( m_pos ) );

		return true;
	}
	VOID CFpsAutoAIView::onRender()	//描画
	{
		//文字の描画==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"今のフレーム Mouse:%d, Key:%d", m_mouseinfo->GetNowFrame() ,  m_keyinfo->GetNowFrame() );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,450,str);

		//========================================
	}
//イベント要素
	//イベントをもらう
	VOID CFpsAutoAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		IF( msg.m_message == "MOVE_STOP"  )
		{
			m_charaview->GetCharactor()->SetAcc(Vector());
			m_charaview->GetCharactor()->SetVel(Vector());
			m_run = false;
		}
		EIF( msg.m_message == "MOVE_START"  )
		{
			m_run = true;
		}
	}
	//生成
	LPTkyFpsAutoAIView CFpsAutoAIView::CreateFpsAutoAI( wstring name )
	{
		return new CFpsAutoAIView(name);
	}
	LPTkyFpsAutoAIView CFpsAutoAIView::CreateFpsAutoAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction , wstring name )
	{
		LPTkyFpsAutoAIView f = CreateFpsAutoAI(name);
		f->SetCharactorView( chara );
		f->SetCmameraVector( pos , Direction );
		return f;
	}

	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAIView 
//　敵ようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFpsEnemyAIView::CFpsEnemyAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyAIView::CFpsEnemyAIView(const CFpsEnemyAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyAIView::~CFpsEnemyAIView()
	{
	}
//GameView 要素
	BOOL CFpsEnemyAIView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CFpsEnemyAIView::onUpdate(float step)	//更新
	{
		//キャラ情報の更新
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//表示回転
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//キャラ情報取得
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//回転の設定
			chara.m_rotate = rotate;
			//攻撃向きの設定
			chara.m_AttackDirection = attackdirection;
			//縦横法を入れる
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		const float LENGTH = 2;
		IF( m_heropos != NULL )
		{
			Vector size = *m_heropos - m_charaview->GetCharactor()->GetPos();
			
			//近づく処理
			IF(  size.GetLarge() > LENGTH  )
			{
				//キャラの移動
				m_charaview->GetCharactor()->Impact(  size.Normal() *WorldWolk3D );
			}
			else
			{
				//速度固定
				m_charaview->GetCharactor()->SetVel(Vector());
				//攻撃が終わっていたら次攻撃
				IF( m_charaview->GetWeapon()->EndAttack() )
				{

					m_charaview->GetWeapon()->Attack(Vector());

				}
			}

		}

		return true;
	}
	VOID CFpsEnemyAIView::onRender()	//描画
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//文字の描画==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"回転情報  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//イベント要素
	//イベントをもらう
	VOID CFpsEnemyAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//主人公の座標
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//独自要素
	//生成
	LPTkyFpsEnemyAIView CFpsEnemyAIView::CreateFpsEnemyAI( )
	{
		return new CFpsEnemyAIView();
	}
	LPTkyFpsEnemyAIView CFpsEnemyAIView::CreateFpsEnemyAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyAIView e = CreateFpsEnemyAI( );
		e->SetCharactorView( chara );
		return e;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAttackLeaveAIView 
//　敵ようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFpsEnemyAttackLeaveAIView::CFpsEnemyAttackLeaveAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyAttackLeaveAIView::CFpsEnemyAttackLeaveAIView(const CFpsEnemyAttackLeaveAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyAttackLeaveAIView::~CFpsEnemyAttackLeaveAIView()
	{
	}
//GameView 要素
	BOOL CFpsEnemyAttackLeaveAIView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CFpsEnemyAttackLeaveAIView::onUpdate(float step)	//更新
	{
		//キャラ情報の更新
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//表示回転
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//キャラ情報取得
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//回転の設定
			chara.m_rotate = rotate;
			//攻撃向きの設定
			chara.m_AttackDirection = attackdirection;
			//縦横法を入れる
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		const float LENGTH = 2;
		IF( m_heropos != NULL )
		{
			Vector size = *m_heropos - m_charaview->GetCharactor()->GetPos();
			
			//近づく処理
			IF(  size.GetLarge() > LENGTH  )
			{
				//キャラの移動
				m_charaview->GetCharactor()->Impact(  size.Normal() *WorldWolk3D );
			}
			else
			{
				//速度固定
				m_charaview->GetCharactor()->SetVel(Vector());
				//攻撃が終わっていたら次攻撃
				IF( m_charaview->GetWeapon()->EndAttack() )
				{

					m_charaview->GetWeapon()->Attack(Vector());

				}
			}

		}

		return true;
	}
	VOID CFpsEnemyAttackLeaveAIView::onRender()	//描画
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//文字の描画==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"回転情報  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//イベント要素
	//イベントをもらう
	VOID CFpsEnemyAttackLeaveAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//主人公の座標
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//独自要素
	//生成
	LPTkyFpsEnemyAttackLeaveAIView CFpsEnemyAttackLeaveAIView::CreateFpsEnemyAttackLeaveAI( )
	{
		return new CFpsEnemyAttackLeaveAIView();
	}
	LPTkyFpsEnemyAttackLeaveAIView CFpsEnemyAttackLeaveAIView::CreateFpsEnemyAttackLeaveAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyAttackLeaveAIView e = CreateFpsEnemyAttackLeaveAI( );
		e->SetCharactorView( chara );
		return e;
	}





	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyThrowAIView 
//　敵ようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFpsEnemyThrowAIView::CFpsEnemyThrowAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyThrowAIView::CFpsEnemyThrowAIView(const CFpsEnemyThrowAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyThrowAIView::~CFpsEnemyThrowAIView()
	{
	}
//GameView 要素
	BOOL CFpsEnemyThrowAIView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CFpsEnemyThrowAIView::onUpdate(float step)	//更新
	{
		//キャラ情報の更新
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//表示回転
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//キャラ情報取得
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//回転の設定
			chara.m_rotate = rotate;
			//攻撃向きの設定
			chara.m_AttackDirection = attackdirection;
			//縦横法を入れる
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		static DWORD time = timeGetTime();

		//二秒おきに攻撃
		IF( timeGetTime() - time  >= 3000 )
		{
			time  = timeGetTime();
			IF( m_charaview->GetWeapon()->EndAttack() )
			{
				m_charaview->GetWeapon()->Attack(Vector());
			}
		}

		static string state = m_charaview->GetWeapon()->GetNowState();

		state = m_charaview->GetWeapon()->GetNowState();

		return true;
	}
	VOID CFpsEnemyThrowAIView::onRender()	//描画
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//文字の描画==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"回転情報  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//イベント要素
	//イベントをもらう
	VOID CFpsEnemyThrowAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//主人公の座標
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//独自要素
	//生成
	LPTkyFpsEnemyThrowAIView CFpsEnemyThrowAIView::CreateFpsEnemyThrowAI( )
	{
		return new CFpsEnemyThrowAIView();
	}
	LPTkyFpsEnemyThrowAIView CFpsEnemyThrowAIView::CreateFpsEnemyThrowAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyThrowAIView e = CreateFpsEnemyThrowAI( );
		e->SetCharactorView( chara );
		return e;
	}







	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyStopAttackAIView 
//　敵ようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFpsEnemyStopAttackAIView::CFpsEnemyStopAttackAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyStopAttackAIView::CFpsEnemyStopAttackAIView(const CFpsEnemyStopAttackAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyStopAttackAIView::~CFpsEnemyStopAttackAIView()
	{
	}
//GameView 要素
	BOOL CFpsEnemyStopAttackAIView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CFpsEnemyStopAttackAIView::onUpdate(float step)	//更新
	{
		//キャラ情報の更新
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//表示回転
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//キャラ情報取得
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//回転の設定
			chara.m_rotate = rotate;
			//攻撃向きの設定
			chara.m_AttackDirection = attackdirection;
			//縦横法を入れる
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		static DWORD time = timeGetTime();

		//二秒おきに攻撃
		IF( timeGetTime() - time  >= 3000 )
		{
			time  = timeGetTime();
			IF( m_charaview->GetWeapon()->EndAttack() )
			{
				m_charaview->GetWeapon()->Attack(Vector());
			}
		}


		return true;
	}
	VOID CFpsEnemyStopAttackAIView::onRender()	//描画
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//文字の描画==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"回転情報  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//イベント要素
	//イベントをもらう
	VOID CFpsEnemyStopAttackAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//主人公の座標
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//独自要素
	//生成
	LPTkyFpsEnemyStopAttackAIView CFpsEnemyStopAttackAIView::CreateFpsEnemyStopAttackAI( )
	{
		return new CFpsEnemyStopAttackAIView();
	}
	LPTkyFpsEnemyStopAttackAIView CFpsEnemyStopAttackAIView::CreateFpsEnemyStopAttackAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyStopAttackAIView e = CreateFpsEnemyStopAttackAI( );
		e->SetCharactorView( chara );
		return e;
	}





















//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindowiew 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CTextWindowiew::CTextWindowiew()
	{
		m_ManageTextWindow = CManageTextWindow::CreateManageTextWinodw();
		m_ManageTextWindow->StartTextWinodw();
		m_NextTextWIndow = NULL;
	}
	CTextWindowiew::CTextWindowiew(const CTextWindowiew &m)
	{
		assert(this->m_ManageTextWindow);
		//値を渡す
		*this->m_ManageTextWindow = *m.m_ManageTextWindow;
		//次のやつは渡さない
		m_NextTextWIndow = NULL;
	}
	CTextWindowiew::~CTextWindowiew ()
	{
		m_ManageTextWindow->DestroyOwnObject();
	}
//GameView 要素
	BOOL CTextWindowiew::onCreate()	//初期化
	{

		return true;
	}
	BOOL CTextWindowiew::onUpdate(float step)	//更新
	{
		//もしテキストが終了した際に次があったら次へ進む
		IF( m_ManageTextWindow->GetState() == TKYMTW_WINODW_LEAVE )
		{
			IF( m_NextTextWIndow !=NULL )
			{
				//終了
				DELETE_VIEW_MESSAGE(this);
				//テキストモードではじめる
				m_NextTextWIndow->GetManageTextWindow()->StartTextWinodw(TKYMTW_TEXT);
				//ビューの追加
				ADD_VIEW_MESSAGE( m_NextTextWIndow );
			}
		}

		//通常動作
		IF( m_ManageTextWindow->UpdateTextWinodw(step) )
		{
			//終了
			DELETE_VIEW_MESSAGE(this);
			//次のビューの作成
			IF( m_NextTextWIndow !=NULL )
			{
				//普通モードではじめる
				m_NextTextWIndow->GetManageTextWindow()->StartTextWinodw();
				//ビューの追加
				ADD_VIEW_MESSAGE( m_NextTextWIndow );
			}
		}
		return true;
	}
	VOID CTextWindowiew::onRender()	//描画
	{
		m_ManageTextWindow->DrawTextWindow();
	}
//イベント要素
	//イベントをもらう
	VOID CTextWindowiew::onEvent( TEventMessage msg )
	{
	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindoRangeDecisionView  
//　テキストの範囲か確認
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//範囲情報管理
	CElementRange<LPTkyTextWindowiew> *m_range;
	//主人公の座標ベクトル
	Vector *m_pos;
*/
	//コンストラクタ・デストラクタ
	CTextWindoRangeDecisionView::CTextWindoRangeDecisionView ()
	{
		m_range = new CElementRange<LPTkyTextWindowiew>();
		m_pos = NULL;
	}
	CTextWindoRangeDecisionView::CTextWindoRangeDecisionView (const CTextWindoRangeDecisionView  &t)
	{
		this->m_pos = t.m_pos;
		*this->m_range = *t.m_range;
	}
	CTextWindoRangeDecisionView::~CTextWindoRangeDecisionView  ()
	{
		list<TElementRange<LPTkyTextWindowiew>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyTextWindowiew>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//要素の開放
			WHILE( !it->m_Element.empty() )
			{
//				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.front()->DestroyOwnObject();
				it->m_Element.pop();
			}
			it++;
		}

		SAFE_DELETE( m_range );
	} 
//GameView 要素
	BOOL CTextWindoRangeDecisionView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CTextWindoRangeDecisionView::onUpdate(float step)	//更新
	{
		IF(m_range == NULL)return false;
		//判定
		TElementRange<LPTkyTextWindowiew> t = m_range->Decision( *m_pos );
		//殻じゃなかったら次に進む
		IF(!t.m_Element.empty())
		{
			//その分だけ生成
			WHILE( !t.m_Element.empty() )
			{
				//生成
				ADD_VIEW_MESSAGE( t.m_Element.front() );
				//はき
				t.m_Element.pop();
			}
			//消去
			m_range->DeleteRange( *m_pos );
		}

		return false;
	}
	VOID CTextWindoRangeDecisionView::onRender()	//描画
	{
		IF(m_range == NULL)return;
		list<TElementRange<LPTkyTextWindowiew>> mm = m_range->GetElement();

		list<TElementRange<LPTkyTextWindowiew>>::iterator im = mm.begin();
		WHILE( im != mm.end() )
		{
			Vector v = Vector( im->pos.x , m_pos->y+0.1f  ,  im->pos.z  );

			//道筋
			LPTKY3D d = Init3D::CreateInit3D();
			d->SetColor(Vector(0.5,0,0));
			d->DrawLine3D( *m_pos - Vector(0,0.1f,0) ,  v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r )  , 0.1f );

			//範囲
			d->SetColor(Vector(1.0f,0,0));
			d->DrawLine3D( v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r) ,  v  , 0.1f );

			im++;
		}
	}

	//開放処理追加
	VOID CTextWindoRangeDecisionView::Delete()
	{
		list<TElementRange<LPTkyTextWindowiew>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyTextWindowiew>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//要素の開放
			WHILE( !it->m_Element.empty() )
			{
				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.pop();
			}
			it++;
		}
	}
//イベント要素
	//イベントをもらう
	VOID CTextWindoRangeDecisionView::onEvent( TEventMessage msg )
	{
	}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyAppearRangeDecisionView  
//　テキストの範囲か確認
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//ファイルからの読み込み

	//コンストラクタ・デストラクタ
	CEnemyAppearRangeDecisionView::CEnemyAppearRangeDecisionView ()
	{
		m_range = new CElementRange<LPTkyFPSEnemyView>();
		m_pos = NULL;
	}
	CEnemyAppearRangeDecisionView::CEnemyAppearRangeDecisionView (const CEnemyAppearRangeDecisionView  &t)
	{
		this->m_pos = t.m_pos;
		*this->m_range = *t.m_range;		
	}
	CEnemyAppearRangeDecisionView::~CEnemyAppearRangeDecisionView  ()
	{
		list<TElementRange<LPTkyFPSEnemyView>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyFPSEnemyView>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//要素の開放
			WHILE( !it->m_Element.empty() )
			{
//				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.front()->DestroyOwnObject();
				it->m_Element.pop();
			}
			it++;
		}
		SAFE_DELETE( m_range );
	}

//GameView 要素
	BOOL CEnemyAppearRangeDecisionView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CEnemyAppearRangeDecisionView::onUpdate(float step)	//更新
	{
		IF(m_range == NULL)return false;
		//判定
		TElementRange<LPTkyFPSEnemyView> t = m_range->Decision( *m_pos );
		//殻じゃなかったら
		IF( !t.m_Element.empty() )
		{
			//その分だけ生成
			WHILE( !t.m_Element.empty() )
			{
				//生成
				ADD_VIEW_MESSAGE( t.m_Element.front() );
				//はき
				t.m_Element.pop();
			}
			//消去
			m_range->DeleteRange( *m_pos );
		}
		return false;
	}
	VOID CEnemyAppearRangeDecisionView::onRender()	//描画
	{
		IF(m_range == NULL)return;
		list<TElementRange<LPTkyFPSEnemyView>> mm = m_range->GetElement();

		list<TElementRange<LPTkyFPSEnemyView>>::iterator im = mm.begin();
		WHILE( im != mm.end() )
		{
			Vector v = Vector( im->pos.x , m_pos->y+0.1f  ,  im->pos.z  );

			//道筋
			LPTKY3D d = Init3D::CreateInit3D();
			d->SetColor(Vector(0,0,0.5f));
			d->DrawLine3D( *m_pos - Vector(0,0.1f,0) ,  v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r )  , 0.1f );

			//範囲
			d->SetColor(Vector(0,0,1.0f));
			d->DrawLine3D( v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r) ,  v  , 0.1f );

			im++;
		}
	}

	VOID CEnemyAppearRangeDecisionView::Delete()
	{
		list<TElementRange<LPTkyFPSEnemyView>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyFPSEnemyView>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//要素の開放
			WHILE( !it->m_Element.empty() )
			{
				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.pop();
			}
			it++;
		}
	}
//イベント要素
	//イベントをもらう
	VOID CEnemyAppearRangeDecisionView::onEvent( TEventMessage msg )
	{
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyKillRangeDecisionView   //シングルトンなやつ
//　敵を倒すためにとまるところ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//範囲情報管理
	CElementRange<INT> *m_range;

	//主人公の座標ベクトル
	Vector *m_pos;

	//殺したい敵の数
	UINT m_KillEnemy;

*/
	//コンストラクタ・デストラクタ
	CEnemyKillRangeDecisionView::CEnemyKillRangeDecisionView ()
	{
		m_range = new CElementRange<INT>();
		m_pos = NULL;
		m_KillEnemy = 0;
	}
	CEnemyKillRangeDecisionView::CEnemyKillRangeDecisionView (const CEnemyKillRangeDecisionView  &t)
	{
		this->m_pos = t.m_pos;
		*this->m_range = *t.m_range;		
		this->m_KillEnemy = t.m_KillEnemy;
	}
	CEnemyKillRangeDecisionView::~CEnemyKillRangeDecisionView  ()
	{
		SAFE_DELETE( m_range );
	}

	//GameView 要素
	BOOL CEnemyKillRangeDecisionView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CEnemyKillRangeDecisionView::onUpdate(float step)	//更新
	{
		//敵がいなくなったら再開する
		IF( m_KillEnemy == 0  )
		{
			CEventManager::StaticAddEventMessage( "MOVE_START" , 0 );
			m_KillEnemy = -1;
		}

		//判定
		TElementRange<int> t = m_range->Decision( *m_pos );
		//殻じゃなかったら
		IF( t.r > EPSILON )
		{

			//敵の数を数える
			m_KillEnemy = 0;
			CEventManager::StaticAddEventMessage( "ENEMY_CHECK" , (DWORD)&m_KillEnemy );
			//ストップを送る
			IF(m_KillEnemy<=0)m_KillEnemy = 0;
			CEventManager::StaticAddEventMessage( "MOVE_STOP" , 0 );

			//消去
			m_range->DeleteRange( *m_pos );
		}
		return false;
	}
	VOID CEnemyKillRangeDecisionView::onRender()	//描画
	{
		list<TElementRange<int>> mm = m_range->GetElement();
		list<TElementRange<int>>::iterator im = mm.begin();
		WHILE( im != mm.end() )
		{
			Vector v = Vector( im->pos.x , m_pos->y+0.1f  ,  im->pos.z  );

			//道筋
			LPTKY3D d = Init3D::CreateInit3D();
			d->SetColor(Vector(0,0.5f,0));
			d->DrawLine3D( *m_pos - Vector(0,0.1f,0) ,  v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r )  , 0.1f );

			//範囲
			d->SetColor(Vector(0,0,1.0f));
			d->DrawLine3D( v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r) ,  v  , 0.1f );

			im++;
		}

		//文字の描画==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"今の敵の数,%d",  m_KillEnemy );
//		CFONT::CreateFontW(NULL)->DrawFontA(100,100,str);

		//========================================
	}
//イベント要素
	//イベントをもらう
	VOID CEnemyKillRangeDecisionView::onEvent( TEventMessage msg )
	{
		//敵が死んだらキルを減らして0になったらスタートを送る
		IF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			IF(((LPTkyGameView)msg.m_value[0] )->GetName() ==  "FPS_Charactor_Enemy" )
			{
				m_KillEnemy--;
				IF( m_KillEnemy <= 0 )m_KillEnemy = 0;
			}
		}

	}




	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMovementPathView  
//　移動の軌跡を保存
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//情報
	LPTkyAutoKeyInput			m_KeyPath;
	LPTkyAutoMouseInput		m_MousePath;
	//今のフレーム数
	DWORD							m_NowFrame;
	//ファイル操作(書き込み)
	wofstream						m_Write;							
*/
	//コンストラクタ・デストラクタ
	CMovementPathView::CMovementPathView ()
	{
		//要素とる
		m_KeyPath = CAutoKeyInput::CreateAutoKeyInput();
		m_MousePath = CAutoMouseInput::CreateAutoMouseInput();
		m_NowKeyFrame = NULL;
		m_NowMouseFrame = NULL;
		m_mouse = DIMOUSESTATE();
		m_rotate = Vector();
		m_key = NULL;
		m_move.clear();
	}
	CMovementPathView::CMovementPathView (const CMovementPathView  & a)
	{
		*m_KeyPath = *a.m_KeyPath;
		*m_MousePath = *a.m_MousePath;
		m_NowKeyFrame = a.m_NowKeyFrame;
		m_NowMouseFrame = a.m_NowMouseFrame;
		m_mouse = a.m_mouse;
		m_rotate = a.m_rotate;
		m_key = a.m_key;
	}
	CMovementPathView::~CMovementPathView  ()
	{
		//書き込みするんだったら
		IF( MessageBoxW(NULL,TEXT("今回の結果を書き込みますか？"),TEXT("選択") , MB_OKCANCEL) == IDOK )
		{

			//最後の情報
			//キー情報追加
			m_KeyPath->AddKeyInput( TAutoKeyInput( m_key , m_NowKeyFrame )  );
			//マウス情報の追加
			m_MousePath->AddMouseInput( 
				Vector(0,0,1).Rotate( Vector() , m_rotate.x , m_rotate.y , m_rotate.z ) ,
				m_NowMouseFrame  );

			//書き込み開始
			wofstream write;
			write.open( m_move.c_str() );

			//見出し
			write << TEXT("KEYINFO") << endl;

			queue<TAutoKeyInput> key = m_KeyPath->GetKeyInfo();
			WHILE( !key.empty() )
			{
				write << TEXT("[") << key.front().m_eTkyAutoKey << TEXT("]<") << key.front().m_FrameNum << TEXT(">") << endl; 
				key.pop();
			}

			write << TEXT("END") << endl;
			write << TEXT("MOUSEINFO") << endl;
		
			queue<TAutoMouseInput> mouse = m_MousePath->getNouseInof();
			WHILE( !mouse.empty() )
			{
				write	<< TEXT("[") << mouse.front().m_MousePos.x << TEXT(",") << mouse.front().m_MousePos.y << TEXT(",") << mouse.front().m_MousePos.z << TEXT("]<")
							<< mouse.front().m_FrameNum << TEXT(">") << endl;
				mouse.pop();
			}

			write << TEXT("END") << endl;

			write.close();
		}
		//終了
		m_KeyPath->DestroyOwnObject();
		m_MousePath->DestroyOwnObject();
	}
//GameView 要素
	BOOL CMovementPathView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CMovementPathView::onUpdate(float step)	//更新
	{
		//進める
		m_NowKeyFrame++;
		m_NowMouseFrame++;

		//前の成分
		static BYTE before_key = m_key;
		static DIMOUSESTATE before_mouse = m_mouse;

		//キーマウスの情報を見る
//		IF( m_NowKeyFrame >= 1 )
		IF( before_key != m_key )
		{
			//キー情報追加
			m_KeyPath->AddKeyInput( TAutoKeyInput( before_key , m_NowKeyFrame )  );
			//フレームの初期化
			m_NowKeyFrame = NULL;
		}
//		IF( m_NowMouseFrame >= 1 )
		IF( before_mouse.lX != m_mouse.lX || 
			before_mouse.lY != m_mouse.lY || 
			before_mouse.lZ != m_mouse.lZ)
		{

			Vector v = (m_base.GetRadVec() - Vector(0,0,1).GetRadVec())*(-1);

			//マウス情報の追加
			m_MousePath->AddMouseInput( 
				Vector(0,0,1).Rotate( Vector() , m_rotate.x , m_rotate.y + D3DXToDegree(v.y) , m_rotate.z ) ,
				m_NowMouseFrame  );

			//フレーム初期化
			m_NowMouseFrame = 0;
		}

		//キー情報
		before_key = m_key;
		before_mouse = m_mouse;

		m_key = 0;

		return true;
	}
	VOID CMovementPathView::onRender()	//描画
	{

		//文字の描画==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"今のフレーム,%d,%d",  m_NowKeyFrame , m_NowMouseFrame );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,250,str);

		//========================================

	}
//イベント要素
	//イベントをもらう
	VOID CMovementPathView::onEvent( TEventMessage msg )
	{
		//キーイベントでフレームを変えたり何とかしたりする
		//マウスイベントでマウスについてやる



		//キー、マウス操作
		string EventName = msg.m_message;

		IF(EventName == "KEY_RIGHT")	//右移動
		{m_key = m_key | TKYKEY_RIGHT;}
		
		IF(EventName == "KEY_LEFT")	//左移動
		{m_key = m_key | TKYKEY_LEFT;}
		
		IF(EventName == "KEY_UP")		//前進
		{m_key = m_key | TKYKEY_UP;}
		
		IF(EventName == "KEY_DOWN")	//後退
		{m_key = m_key | TKYKEY_DOWN;}

		IF(EventName == "MOUSE_INFO")
		{
			m_mouse = *( (DIMOUSESTATE*)msg.m_value[0] );
			m_rotate += Vector( (float)m_mouse.lY / 10.0f , (float)m_mouse.lX / 10.0f , 0 ); 
		}

		EIF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			LPTkyGameView view = (LPTkyGameView)msg.m_value[0];

			IF( view->GetName() == "FPS_Charactor_Hero" )
			{
				DELETE_VIEW_MESSAGE(this);
			}
		}

	}









//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CAdventureWindowiew 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CAdventureWindowiew::CAdventureWindowiew()
	{
		m_ManageAdventureWindow = CManageTextWindow::CreateManageTextWinodw();
		m_ManageAdventureWindow->StartTextWinodw();
		m_NextAdventureWIndow = NULL;

		//実行中
		m_adventure = TKYADVENTURE_SHOW;

	}
	CAdventureWindowiew::CAdventureWindowiew(const CAdventureWindowiew &m)
	{
		assert(this->m_ManageAdventureWindow);
		//値を渡す
		*this->m_ManageAdventureWindow = *m.m_ManageAdventureWindow;
		//次のやつは渡さない
		m_NextAdventureWIndow = NULL;
		//モード
		m_adventure = m.m_adventure;
	}
	CAdventureWindowiew::~CAdventureWindowiew ()
	{
		m_ManageAdventureWindow->DestroyOwnObject();
	}
//GameView 要素
	BOOL CAdventureWindowiew::onCreate()	//初期化
	{

		return true;
	}
	BOOL CAdventureWindowiew::onUpdate(float step)	//更新
	{
		SWITCH( m_adventure )
		{

			case TKYADVENTURE_SHOW:

				//通常更新
				m_ManageAdventureWindow->UpdateTextWinodw(step);

				//終わったら
				IF( m_ManageAdventureWindow->GetState() == TKYMTW_WINODW_LEAVE )
				{
					//全部表示にして
					m_ManageAdventureWindow->EndTextWinodw();
					//終わったら終わりにする
					m_adventure = TKYADVENTURE_END;
				}

			break;
			case TKYADVENTURE_OPEN:

				//オープンしてENDにいく
				m_ManageAdventureWindow->EndTextWinodw();
				m_adventure = TKYADVENTURE_END;

			break;
			case TKYADVENTURE_END:
			break;

			case TKYADVENTURE_ENDPROCESS:

				//終了プロセス
				IF( m_ManageAdventureWindow->UpdateTextWinodw( step ) )
				{
					DELETE_VIEW_MESSAGE(this);

					CEventManager::StaticAddEventMessage( "END_STAGE" , NULL  );

				}

			break;

			case TKYADVENTURE_ESCAPE:

				//終了
				DELETE_VIEW_MESSAGE(this);

				//テキストモードではじめる
				m_NextAdventureWIndow->GetManageAdventureWindow()->StartTextWinodw(TKYMTW_TEXT);

				//ビューの追加
				ADD_VIEW_MESSAGE( m_NextAdventureWIndow );

			break;

		}

		return true;
	}
	VOID CAdventureWindowiew::onRender()	//描画
	{
		m_ManageAdventureWindow->DrawTextWindow();
	}
//イベント要素
	//イベントをもらう
	VOID CAdventureWindowiew::onEvent( TEventMessage msg )
	{

		//普通イベント
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//左ボタンが押されたら弾発射
			IF( message == WM_LBUTTONDOWN || message == WM_MOUSEWHEEL)
			{

				IF( m_adventure == TKYADVENTURE_SHOW )
				{
					m_adventure = TKYADVENTURE_OPEN;
				}
				EIF( m_adventure == TKYADVENTURE_END )
				{
					//次へ行く処理
					m_adventure = TKYADVENTURE_ESCAPE;

					IF( m_NextAdventureWIndow == NULL )
					{
						//終了する
						m_adventure = TKYADVENTURE_ENDPROCESS;
					}
				}

			}

		}

	}






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTitleView 
//　タイトル情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*

	LPTkyTitle m_Title;
*/

	CTitleView::CTitleView(TTitle t)
	{
		m_Title = CTitle::CreateTitle(t);
	}
	CTitleView::CTitleView( const CTitleView& t)
	{
		*m_Title = *t.m_Title;
	}
	CTitleView::~CTitleView()
	{
		m_Title->DestroyOwnObject();
	}

//GameView 要素
	BOOL CTitleView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CTitleView::onUpdate(float step)	//更新
	{
		m_Title->UpdateTitle(step);

		//次がなかったら終わる
		IF( m_Title->IsEnd() )
		{
			CEventManager::StaticAddEventMessage( "END_STAGE" , NULL );
		}

		return true;
	}
	VOID CTitleView::onRender()	//描画
	{
		m_Title->DrawTitle();
	}
//イベント要素
	//イベントをもらう
	VOID CTitleView::onEvent( TEventMessage msg )
	{
		//普通イベント
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//左ボタンが押されたら弾発射
			IF( message == WM_LBUTTONDOWN )
			{
				//しぇ良い度スタート
				CEventManager::StaticAddEventMessage( "START_SHADE" , 10 );
			}

			//マウスが動いたら
			IF( message == WM_MOUSEMOVE )
			{
				m_Title->SetPos( Vector( (float)LOWORD(lParam) ,  (float)HIWORD(lParam) ,0 ) ) ;
			}

		}
		EIF( msg.m_message == "END_SHADE_IN")
		{
			IF( msg.m_value[0] == 10 )
			{
				m_Title->Next();
			}
		}

	}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CScoreView 
//　タイトル情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	LPTkyScore m_Score;
*/

	CScoreView::CScoreView(TScore t)
	{
		m_Score = CScore::CreateScore(t);
	}
	CScoreView::CScoreView( const CScoreView& s)
	{
		*m_Score = *s.m_Score;
	}
	CScoreView::~CScoreView()
	{
		m_Score->DestroyOwnObject();
	}

//GameView 要素
	BOOL CScoreView::onCreate()	//初期化
	{
		return true;
	}
	BOOL CScoreView::onUpdate(float step)	//更新
	{
		m_Score->UpdateScore(step);

		//リセット
		IF( m_Score->GetScore().m_state == TKYSCORE_END )
		{
			CEventManager::StaticAddEventMessage( "END_STAGE" , NULL );
		}
		IF( m_Score->GetScore().m_state == TKYSCORE_RESET )
		{
			CEventManager::StaticAddEventMessage( "RESET_STAGE" , NULL );
		}

		return true;
	}
	VOID CScoreView::onRender()	//描画
	{
		m_Score->DrawScore();
	}
//イベント要素
	//イベントをもらう
	VOID CScoreView::onEvent( TEventMessage msg )
	{
		//スコア情報が送られてくる

		//普通イベント
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//左ボタンが押されたら弾発射
			IF( message == WM_LBUTTONDOWN )
			{
				//結果状態だったら
				IF( m_Score->GetScore().m_state == TKYSCORE_RESULT  )
				{
					//スコアを送る
//					CEventManager::StaticAddEventMessage( "SEND_SCORE" , m_Score->GetScore().m_score );

					//sヒューティん具が終了したら(終了にする)
					TScore t = m_Score->GetScore();
					t.m_state = TKYSCORE_END;
					m_Score->SetScore(t);


				}
				EIF( m_Score->GetScore().m_state == TKYSCORE_GAMEOVER   )
				{
					//sヒューティん具が終了したら(終了にする)
					TScore t = m_Score->GetScore();
					t.m_state = TKYSCORE_RESET;
					m_Score->SetScore(t);					
				}

			}
		}
		EIF( msg.m_message == "END_SHOOTING_STAGE" )
		{
			CEventManager::StaticAddEventMessage( "END_TEXTVIEW" , 0 );

			//終了していなかったら
			IF( m_Score->GetScore().m_state != TKYSCORE_END && 
				m_Score->GetScore().m_state != TKYSCORE_GAMEOVER  && 
				m_Score->GetScore().m_state != TKYSCORE_RESET)
			{

				//sヒューティん具が終了したら(結果にする)
				TScore t = m_Score->GetScore();
				t.m_state = TKYSCORE_RESULT;
				m_Score->SetScore(t);
			}			

		}
		EIF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			//キャラが同じだったら
			TScore t = m_Score->GetScore();
			IF( t.m_chara == ((LPTkyFPSCharactorView)msg.m_value[0])->GetCharactor() )
			{
				IF( ((LPTkyFPSCharactorView)msg.m_value[0])->GetCharactor()->IsDead()  )
				{

					//これが原因で終了後にゲームオーバーが表示される
					CEventManager::StaticAddEventMessage( "END_TEXTVIEW" , 0 );

					t.m_chara = NULL;
					t.m_state = TKYSCORE_GAMEOVER;
					m_Score->SetScore(t);
				}
			}
		}


		//スコアなど
		EIF( msg.m_message ==  "SCORE_BATTERY")
		{
			TScore t = m_Score->GetScore();
			t.m_BatteryNum++;
			m_Score->SetScore(t);
		}
		EIF( msg.m_message ==  "SCORE_HIT")
		{
			TScore t = m_Score->GetScore();
			t.m_HitNum++;
			m_Score->SetScore(t);
		}
		EIF( msg.m_message ==  "SCORE_SCORE")
		{
			TScore t = m_Score->GetScore();
			t.m_score+=msg.m_value[0];
			m_Score->SetScore(t);
		}


	}
