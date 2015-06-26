#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyWindowsApplication.h"

HINSTANCE	g_hInst;								// 現在のインターフェイス
HWND		g_hWnd;


//ウィンドウの大きさ
const static DWORD WindowWidth = 1200;
const static DWORD WindowHeight = 800;
//ダブルクリックの速さ
const static DWORD WindowDoubleClick = 100;

//ウィンドウの差
const static Vector WindowGap = Vector(100,0,0);

//摩擦力
const STATIC float WorldFrictional = 0.1f;
//反発力
const STATIC float WorldElastic = 0.0f;


//遅延スクロール速度
const STATIC float	WorldScrollVelocity = 5.0f;

//空気抵抗
const STATIC float WorldAir = 0.2f;
//重力
const STATIC float WorldGravity = 4.0f;

//重さ
const STATIC float ModelWeight = 1.0f;
//ジャンプ力
const STATIC float ModelJunp = 12.0f;
//動力
const STATIC float ModelForce = 2.0f;
//摩擦力
const STATIC float ModelFrictional = 1.0f;
//反発力
const STATIC float ModelElastic = 0.0f;
//ダッシュ
const STATIC float ModelDash = 30.0f;

//スクロール情報
CLASS_POINTA( CScroll , LPTkyScroll )
{
private:
	Vector m_scroll;
	Vector m_realscroll;
	Vector m_window;
public:
	//座標をセットする
	VOID SetPosision(Vector pos)
	{
		//スクロールのセット
		m_scroll =  pos + Vector( -(float)WindowWidth/2 , -(float)WindowHeight/2 , 0  ) + WindowGap;
	}
	//更新動作
	VOID Update(Vector move)
	{
		m_realscroll.Move( m_scroll , move );
		Vector vec = m_realscroll - m_scroll;
		DWORD lange = min(WindowHeight,WindowHeight)*2/6;
		IF(vec.GetLarge() >= (float)lange )m_realscroll = m_scroll + vec.Normal() * (float)lange; 
	}

	//設定
	VOID SetScroll(Vector s){ m_scroll = s; }
	VOID SetRealScroll(Vector s){ m_realscroll = s; }
	VOID SetWindowSize(Vector v){m_window = v;}
	//取得
	Vector GetScroll(){return m_scroll;}
//	Vector GetRealScroll(){return m_realscroll;}
	Vector GetRealScroll(){return m_scroll;}
	Vector GetWindowSize(){return m_window;}
};

//物体情報ビュー(継承先で物体情報を決めさせる)
CLASS_POINTA( IPhysicsView , LPTkyPhysicsView ) :public IGameView
{
protected:

	//物質情報
	LPPhysicsModel m_model;

public:

	//初期化
	VIRTUAL BOOL onCreate()=0;
	//更新
	VIRTUAL BOOL onUpdate(float step)=0;
	//破棄
	VIRTUAL VOID onRelease() =0;

	//イベントネーム名前を決める
	VOID onNameSet( string *str )
	{
		*str = "PhysicsView";
	}
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg )=0;

	//ビュー要素
	//描画
	VIRTUAL VOID onRender() = 0;

	//モデル情報の取得
	LPPhysicsModel GetModel(){return m_model;}

};

//主人公ビュー
CLASS_POINTA( IHeroView , LPTkyHeroView ) :public IPhysicsView
{
private:

	//移動速度
	float m_MoveForce;
	//ジャンプ定数
	float m_JumpContant;

	//スクロール
	CScroll m_scroll;

	//接触座標
	vector<Vector> m_pos;
	vector<CVector> m_vec;

protected:

	//コンストラクタ・デストラクタ
	IHeroView(Vector pos)
	{
		m_model = PhysicsCircleModel::CreatePhysicsCircleModel( 50 );
		m_JumpContant = ModelJunp;
		m_MoveForce = ModelForce;
		m_model->SetPos(pos);
		m_model->SetFrictional(ModelFrictional);
		m_model->SetElastic(ModelElastic);
		m_model->SetWeight(ModelWeight);
	}
	VIRTUAL ~IHeroView()
	{
		m_pos.clear();
		m_vec.clear();
	}

public:

	//初期化
	VIRTUAL BOOL onCreate()
	{
	}
	//更新
	VIRTUAL BOOL onUpdate(float step)
	{

		//スクロールの設定
		m_scroll.SetPosision( m_model->GetPos() );
		m_scroll.Update( Vector(1,0.1f,1)*WorldScrollVelocity );
	}
	//破棄
	VIRTUAL VOID onRelease()
	{
	}

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		string EventName = msg.m_message;
		IF(EventName == "KEY_UP")
		{
			m_model->Impact( Vector(0,-m_JumpContant,0) );
		}
		IF(EventName == "KEY_DOWN")
		{
			m_model->Impact( Vector(0,20,0) );
		}
		EIF(EventName == "KEY_LEFT")
		{
			m_model->Impact( Vector(-m_MoveForce,0,0) );
		}
		EIF(EventName == "KEY_LEFT_DOUBLE")
		{
			DWORD Dash = (DWORD)ModelDash;
			m_model->Impact( Vector( -m_MoveForce*ModelDash,-m_MoveForce*Dash,0 ) );
		}
		EIF(EventName == "KEY_RIGHT")
		{
			m_model->Impact( Vector(m_MoveForce,0,0) );
		}
		EIF(EventName == "KEY_RIGHT_DOUBLE")
		{
			DWORD Dash = (DWORD)ModelDash;
			m_model->Impact( Vector( m_MoveForce*ModelDash,-m_MoveForce*Dash,0 ) );
		}
		EIF(EventName == "GET_HERO_SCROLL")
		{
			//スクロール情報一斉送信
			vector<DWORD> ele;
			ele.push_back( (DWORD)(&m_scroll) );
			CEventManager::StaticAddEventMessage( "HERO_SCROLL" , ele );
		}

		EIF(msg.m_message == "COLLISION_POS" )
		{
			m_pos.push_back( *((Vector*)msg.m_value[0]) );
		}
		EIF(msg.m_message == "COLLISION_VEC" )
		{
			m_vec.push_back( *((CVector*)msg.m_value[0]) );
		}

	}

	//ビュー要素
	//描画
	VIRTUAL VOID onRender()
	{
		//主人公の動画
		LPTKY3D tky = Init3D::CreateInit3D();
		tky->SetColor(Vector(1,0,0));			//色セット
		tky->DrawCircle( m_model->GetPos() - m_scroll.GetRealScroll() , m_model->GetShape().m_element[0].x , 20 ); //描画

		//軌跡
		tky->SetColor(Vector(0,0,0));			//色セット
		tky->DrawLine( 
			m_model->GetPos() - m_scroll.GetRealScroll() , 
			m_model->GetPos()+m_model->GetVel()  - m_scroll.GetRealScroll(), 
			10 );		

		vector<Vector>::iterator ip = m_pos.begin();
		int i=0;
		WHILE( ip != m_pos.end() )
		{
			i++;

			Vector pos = *ip - m_scroll.GetRealScroll();

			char str[200];
			sprintf_s(str,"%d",i);
			TD3DFONT::CreateFontW(NULL)->DrawFontA( (INT)pos.x , (INT)pos.y - 20 ,str );

			LPTKY3D tky = Init3D::CreateInit3D();
			tky->SetColor(Vector(0,1,1));			//色セット
			tky->DrawCircle( pos , 5, 20 ); //描画
			ip++;
		}

		vector<CVector>::iterator iv = m_vec.begin();
		i=0;
		WHILE( iv != m_vec.end() )
		{
			i++;

			Vector pos = iv->GetVectorStart() - m_scroll.GetRealScroll();

			char str[200];
			sprintf_s(str,"%d",i);
			TD3DFONT::CreateFontW(NULL)->DrawFontA( (INT)pos.x , (INT)pos.y - 20 ,str );

			tky->SetColor(Vector(0.5f,0.5f,0.5f));			//色セット
			tky->DrawLine( 
				pos , 
				pos+iv->GetVector(), 
				5 );		

			iv++;
		}



		//文字
		char str[200];
		sprintf_s(str,"POS (%.3lf,%.3lf,%.3lf) VEL (%.3lf,%.3lf,%.3lf) ACC (%.3lf,%.3lf,%.3lf)"
			,m_model->GetPos().x,m_model->GetPos().y,m_model->GetPos().z
			,m_model->GetVel().x,m_model->GetVel().y,m_model->GetVel().z
			,m_model->GetAcc().x,m_model->GetAcc().y,m_model->GetAcc().z
			);
		TD3DFONT::CreateFontW(NULL)->DrawFontA( 100,120,str );

	}

	//生成
	STATIC LPTkyHeroView CreateHereView(Vector pos)
	{
		return new IHeroView(pos);			
	}

};

//武器ビュー
CLASS_POINTA( IWeaponView , LPTkyWeaponView ) :public IPhysicsView
{
private:

	//スクロール
	LPTkyScroll m_scroll;

protected:

	//コンストラクタ・デストラクタ
	IWeaponView(Vector pos)
	{
		m_model = PhysicsCircleModel::CreatePhysicsCircleModel( 50 );
		m_model->SetPos(pos);
		m_model->SetFrictional(ModelFrictional);
		m_model->SetElastic(ModelElastic);
		m_model->SetWeight(ModelWeight);
	}
	VIRTUAL ~IWeaponView()
	{
	}

public:

	//初期化
	VIRTUAL BOOL onCreate()
	{

	}
	//更新
	VIRTUAL BOOL onUpdate(float step)
	{

	}
	//破棄
	VIRTUAL VOID onRelease()
	{
	}

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}
	}

	//ビュー要素
	//描画
	VIRTUAL VOID onRender()
	{
		//主人公の動画
		LPTKY3D tky = Init3D::CreateInit3D();
		tky->SetColor(Vector(0,0,1));			//色セット
		tky->DrawCircle( m_model->GetPos() - m_scroll->GetRealScroll() , m_model->GetShape().m_element[0].x , 20 ); //描画

	}

	//生成(基点座標)ここを起点とする
	STATIC LPTkyWeaponView CreateWeaponView(Vector *pos)
	{
		return new IWeaponView(*pos);			
	}

	//取得
	LPPhysicsCircleModel GetModel(){return (LPPhysicsCircleModel)m_model;}

};


//障害物ビュー
CLASS_POINTA( IObstaleView , LPTkyObstaleView ) :public IPhysicsView
{
private:

	//スクロール
	LPTkyScroll m_scroll;

protected:

	//コンストラクタ・デストラクタ
	IObstaleView(Vector pos)
	{
		m_model = PhysicsCircleModel::CreatePhysicsCircleModel( 50 );
		m_model->SetPos(pos);
		m_model->SetFrictional(ModelFrictional);
		m_model->SetElastic(ModelElastic);
		m_model->SetWeight(ModelWeight);

		m_scroll=NULL;

	}
	VIRTUAL ~IObstaleView()
	{
	}

public:

	//初期化
	VIRTUAL BOOL onCreate()
	{

	}
	//更新
	VIRTUAL BOOL onUpdate(float step)
	{
		//スクロールがなかったら要求
		IF(m_scroll == NULL)
		{
			vector<DWORD> ele;
			CEventManager::StaticAddEventMessage( "GET_HERO_SCROLL" , ele );
		}
	}
	//破棄
	VIRTUAL VOID onRelease()
	{
	}

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}
	}

	//ビュー要素
	//描画
	VIRTUAL VOID onRender()
	{
		//障害物描画
		LPTKY3D tky = Init3D::CreateInit3D();
		tky->SetColor(Vector(0,0,1));			//色セット
		tky->DrawCircle( m_model->GetPos() - m_scroll->GetRealScroll() , m_model->GetShape().m_element[0].x , 20 ); //描画

		//軌跡
		tky->SetColor(Vector(0,1,1));			//色セット
		tky->DrawLine( 
			m_model->GetPos() - m_scroll->GetRealScroll() , 
			m_model->GetPos()+m_model->GetVel()  - m_scroll->GetRealScroll(), 
			10 );	

	}

	//生成
	STATIC LPTkyObstaleView CreateObstaleView(Vector pos)
	{
		return new IObstaleView(pos);			
	}

	//取得
	LPPhysicsCircleModel GetModel(){return (LPPhysicsCircleModel)m_model;}

};

//マップビュー
CLASS_POINTA( IMapView , LPTkyMapView ) :public IPhysicsView
{
private:

	//スクロール情報
	LPTkyScroll m_scroll;

protected:

	IMapView( CVector cv )
	{
		Vector pos = (cv.GetVectorEnd()+cv.GetVectorStart())/2;
		m_model = PhysicsLineModel::CreatePhysicsLineModel( cv.GetVectorStart()-pos,cv.GetVectorEnd()-pos );
		m_model->SetFrictional(WorldFrictional);
		m_model->SetElastic(WorldElastic);
		m_model->SetPos(pos);
		m_model->SetWeight(0);
	}
	VIRTUAL ~IMapView()
	{
	}

public:

	//初期化
	VIRTUAL BOOL onCreate()
	{
		m_scroll=NULL;
	}
	//更新
	VIRTUAL BOOL onUpdate(float step)
	{
		//スクロールがなかったら要求
		IF(m_scroll == NULL)
		{
			CEventManager::StaticAddEventMessage( "GET_HERO_SCROLL" , vector<DWORD>() );
		}
	}
	//破棄
	VIRTUAL VOID onRelease()
	{
	}

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}
	}

	//ビュー要素
	//描画
	VIRTUAL VOID onRender()
	{
		//要素の確認（スクロール確認）
		assert(m_scroll);

		//マップ描画
		LPTKY3D tky = Init3D::CreateInit3D();
		CVector cv = ( (LPPhysicsLineModel)m_model )->GetLine();

		assert(m_scroll);

		cv.SetVectorEnd( cv.GetVectorEnd() - m_scroll->GetRealScroll() );
		cv.SetVectorStart( cv.GetVectorStart() - m_scroll->GetRealScroll() );
		cv.DrawLineSet( tky , Vector(0,1,0) , 10 );
	}

	//生成
	STATIC LPTkyMapView CreateMapView(CVector cv)
	{
		return new IMapView(cv);			
	}

};

//コモンゲームロジック
CLASS_POINTA( CCommonGameLogic , LPTkyCommonGameLogic ) : public IGameLogic
{
private:

	//画面のスクロール具合
	LPTkyScroll m_scroll;

	LPTkyPhysicsWorld m_World;

	CCommonGameLogic()
	{
	}
	VIRTUAL ~CCommonGameLogic()
	{
		m_World->Release();

	}

public:

	//初期化
	VIRTUAL BOOL onCreate() 
	{
		//世界の情報
		m_World = CPhysicsWorld::CreatePhysicsWorld();
		m_World->SetAirGap( WorldAir );	   
		m_World->SetGravity( WorldGravity );
		RECT rect = { 10 , -1000 , 7000-10 , 1000 };
		m_World->SetRange(rect);

		return true;
	}
	//更新( View更新よりも速い )
	VIRTUAL VOID onUpdate(float step) 
	{
		//更新
		m_World->Update(step);

	}
	//破棄
	VIRTUAL VOID onRelease()
	{
	}

	//View設定時(View情報は自動的にタスクに追加されるのでその他の設定)
	VIRTUAL VOID onAddView( LPTkyGameView view )
	{
		IF( view->GetName() == "PhysicsView" )
		{
			m_World->AddPhysicsModel( ( (LPTkyPhysicsView)view )->GetModel() );
		}
	}

	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "CommonGame";
	}
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "ADD_VIEW" )
		{
			LPTkyObstaleView view = (LPTkyObstaleView)(msg.m_value[0]);
			view->GetModel()->SetPos( view->GetModel()->GetPos() + m_scroll->GetRealScroll() );
			this->AddGameView( view );
		}
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}

	}

	//生成
	STATIC LPTkyCommonGameLogic CreateCommonGameLogic()
	{
		return new CCommonGameLogic();
	}

};

//コモンウィンドウ
CLASS_POINTA( CCommonWindowsApplicationLayer , LPTkyCommonWindowsApplicationLayer ) :public IWindowsApplicationLayer
{
private:

	//ゲームロジック
	LPTkyCommonGameLogic m_CommonGameLogic;
	
	//デバッグ用
	//飛んできた文字表示用
	vector<string> m_str;

	//基本クラス
	LPTKY3D m_pDtata;
	//文字表示クラス
	LPTKYFONT m_pFont;

	//デバイスの取得（キーボード）
	LPTKYINPUT m_keybord;

	//新しいの
	VOID				pKeyBoard()
	{
		assert(m_keybord);

		CHAR key[256];
		IF(m_keybord->GetKeyBoard(key)==true)
		{
			//ダブルクリック用
			static DWORD lefttime = 0;
			static DWORD righttime = 0;

			TEventMessage msg = { "" , vector<DWORD>() };

			//上が押されていたら====================
			IF( KEYDOWN( key , DIK_UP ) )
		{
			msg.m_message = "KEY_UP";
			CEventManager::StaticAddEventMessage( msg );
		}
			//========================================
			//下が押されていたら====================
			IF( KEYDOWN( key , DIK_DOWN ) )
		{
			msg.m_message = "KEY_DOWN";
			CEventManager::StaticAddEventMessage( msg );
		}
			//========================================
			//左が押されていたら====================
			IF( KEYDOWN( key , DIK_LEFT ) )
		{
			msg.m_message = "KEY_LEFT";
			CEventManager::StaticAddEventMessage( msg );
			IF( timeGetTime() - lefttime <= WindowDoubleClick )
			{
				msg.m_message = "KEY_LEFT_DOUBLE";
				CEventManager::StaticAddEventMessage( msg );
				lefttime = 0;
			}
			else
			{
				lefttime = 1;
			}
		}
			else
			{
				IF(lefttime == 1)
			{
				lefttime = timeGetTime();
			}
			}
			//========================================
			//右が押されていたら====================
			IF( KEYDOWN( key , DIK_RIGHT ) )
		{
			msg.m_message = "KEY_RIGHT";
			CEventManager::StaticAddEventMessage( msg );
			IF( timeGetTime() - righttime <= WindowDoubleClick )
			{
				msg.m_message = "KEY_RIGHT_DOUBLE";
				CEventManager::StaticAddEventMessage( msg );
				righttime = 0;
			}
			else
			{
				righttime = 1;
			}
		}
			else
			{
				IF(righttime == 1)
			{
				righttime = timeGetTime();
			}
			}
			//========================================

		}
	}

	VIRTUAL BOOL			pOnCreate()
	{


	   //================新物理初期化=======================================

	   //物理情報
	   m_CommonGameLogic = CCommonGameLogic::CreateCommonGameLogic();
	   m_CommonGameLogic->onCreate();
	   //マップの登録
		//初期座標
		const int NUM = 9;
		Vector vecs[NUM+1] = 
		{
			Vector( 0,0,0 ),			
			Vector( 100,750,0 ),
			Vector( 400,650,0 ),
			Vector( 700,750,0 ),
			Vector( 1200,800,0 ),
			Vector( 2000,100,0 ),	
			Vector( 3000,800,0 ),
			Vector( 3100,200,0 ),
			Vector( 5000,400,0 ),
			Vector( 7000,100,0 ),

//			Vector( 100,650,0 ),
//			Vector( 1000,750,0 ),

		};

		FOR_I(NUM)
		{
	   //================新物理更新=======================================

			m_CommonGameLogic->AddGameView( IMapView::CreateMapView(CVector( vecs[i],vecs[i+1] )) );

	   //==============================================================
		}
	   //キャラクタ登録
	   m_CommonGameLogic->AddGameView( IHeroView::CreateHereView( Vector( WindowWidth/2 , 0 , 0 ) ) );

	   //==============================================================


		//データの取得
		m_pDtata = Init3D::CreateInit3D();
		IF_FAILED( m_pDtata->Create3DX(GetHWnd()) )
		{
			MessageBoxA( NULL ,"エラー" , "INIT3D" ,NULL );
			return FALSE;
		}
		//フォントデータ取得
		m_pFont = TD3DFONT::CreateFontW(m_pDtata);
		IF_FAILED( m_pFont->GetState() )
		{
			MessageBoxA( NULL , "sdadf" , "csdaf" , MB_OK);
			return FALSE;
		}
		//キーボード情報
		m_keybord = CTkyInput::CreateInput();
		m_keybord->SetInputinfo(GetHInstance(),GetHWnd());
	
	   return true;
	}
	VIRTUAL BOOL			pOnUpdate()
	{
		//キーボードの処理
		pKeyBoard();
/*
		static DWORD time;
		static vector<DWORD> time1;
		static vector<DWORD> time2;
		static vector<DWORD> time3;
		static vector<DWORD> time4;
*/
		static vector<DWORD> fps_b;

		IF(fps_b.empty())
		{
			fps_b.push_back(GetFps());
/*			time1.push_back(0);
			time2.push_back(0);
			time3.push_back(0);
			time4.push_back(0);
*/		}
		EIF( fps_b.back() != GetFps())
		{
			fps_b.push_back(GetFps());
/*			time1.push_back(0);
			time2.push_back(0);
			time3.push_back(0);
			time4.push_back(0);
*/		}
/*
		time4.back() += timeGetTime() - time;


		//通常更新

		time  = timeGetTime();
*/
//		m_CommonGameLogic->Update( 0.5f  * (60.0f/ ( (GetFps()>=10) ? GetFps() : 60.0f ) ) );
		m_CommonGameLogic->Update( 0.25f );
/*
		time1.back() += (timeGetTime() - time);

		//イベント更新

		time  = timeGetTime();
*/
//		CEventManager::CreateEventManager()->Update();
/*
		time2.back() += timeGetTime() - time;


		//描画処理

		time  = timeGetTime();
*/
		IF( m_pDtata->StartDraw() )
		{
			m_pDtata->Clear( D3DCLEAR_TARGET , D3DCOLOR_ARGB(255,255,255,255));

		   //================新物理更新=======================================

			m_CommonGameLogic->Render();

		   //==============================================================

			//フォントの描画
			//文字の描画==============================
			m_pFont->SetColor(0,0,0);
			char str[200];
			FOR_I(fps_b.size())
			{
//				sprintf_s(str,"Hello World!! FPS is %d update:%d event:%d render:%d other:%d all:%d ",fps_b[i],time1[i],time2[i],time3[i]	,time4[i] , time1[i]+time2[i]+time3[i]+time4[i]	);
				sprintf_s(str,"物理演算　Hello World!! FPS is %d ",fps_b[i]	);
				m_pFont->DrawFontA(100,150+20*(fps_b.size()-i),str);
			}

			//飛んできた文字
			FOR_I(m_str.size())
			{
				m_pFont->DrawFontA(500,150+20*(m_str.size()-i),m_str[i].c_str());
			}

			//========================================

			m_pDtata->EndDraw();
		}
/*
		int tmp = timeGetTime() - time;

		time3.back() += tmp;

		time  = timeGetTime();
*/
		return true;
	}
	VIRTUAL	VOID			pOnRelease()
	{
	}
	VIRTUAL	VOID			pOnEvent( TEventMessage msg )
	{

		//マウスのライン	
		STATIC CVector	MouseLine;

		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			IF( message == WM_ACTIVATE )
			{
				IF(m_keybord!=NULL)m_keybord->SetDeviceAcquire(true);
			}
			IF( message == WM_MOUSEMOVE)
			{
				//終点ベクトルのセット
				MouseLine.SetVectorEnd( Vector( LOWORD(lParam) , HIWORD(lParam) , 0  ));
			}
			IF( message == WM_LBUTTONDOWN)
			{
				//始点ベクトルのセット
				MouseLine.SetVectorStart( Vector( LOWORD(lParam)  , HIWORD(lParam)  , 0  ) );
				MouseLine.SetVectorEnd( Vector( LOWORD(lParam), HIWORD(lParam)  , 0 ) );
			}
			IF( message == WM_LBUTTONUP)
			{
				//追加
				Vector vec = MouseLine.GetVectorEnd()-MouseLine.GetVectorStart();
				LPTkyObstaleView view = IObstaleView::CreateObstaleView(MouseLine.GetVectorStart() + vec/2);
				LPPhysicsCircleModel model = view->GetModel();
				IF(vec.GetLarge()/2 > 1)model->SetCircle(vec.GetLarge()/2);	 
				else model->SetCircle(10);
				model->SetFrictional(ModelFrictional);	   
				model->SetElastic(ModelElastic);
				FLOAT by =  (vec.GetLarge()/2) / 50.0f;
				IF(by <= 0.01)by = 0.01f;
				model->SetWeight(ModelWeight*by);

				//ベクトル分割
				MouseLine = CVector();

				//VIEWの追加メッセージの送信
			    TEventMessage send;
				send.m_message = "ADD_VIEW";
				send.m_value.resize(1);
				send.m_value[0] = (DWORD)view;
				CEventManager::StaticAddEventMessage(send);
			}

		}
		EIF(msg.m_message == "COMMON_LOGIC_STRING" )
		{
			m_str.push_back(  string(((LPSTR)msg.m_value[0])) );
		}

	}

	//コンストラクタ・デストラクタ
	CCommonWindowsApplicationLayer()
	{
		m_CommonGameLogic = NULL;
		m_pDtata = NULL;
		m_pFont = NULL;	
		m_keybord = NULL;
	}
	VIRTUAL ~CCommonWindowsApplicationLayer()
	{
	}

public:

	STATIC LPTkyCommonWindowsApplicationLayer CreateCommonWindowsApplicationLayer()
	{
		return new CCommonWindowsApplicationLayer();
	}

};

