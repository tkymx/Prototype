#include "DirectXProject.h"

//ウィンドウの大きさ
const static DWORD WindowWidth = 1200;
const static DWORD WindowHeight = 800;
//ダブルクリックの速さ
const static DWORD WindowDoubleClick = 100;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ロジック用用
// 基本的なFPSロジック
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//コンストラクタ・デストラクタ
	CDirectXGameLogic::CDirectXGameLogic()
	{
		//世界の設定
		m_world3d = NULL;
	}
	CDirectXGameLogic::~CDirectXGameLogic()
	{

	}
//GameLogic情報
	//初期化
	BOOL CDirectXGameLogic::onCreate() 
	{
		//世界の生成
		m_world3d =	CPhysicsWorld3D::CreatePhysicsWorld3D();
		m_world3d->SetAirGap( WorldAir3D );
		m_world3d->SetGravity( WorldGravity3D );

		return true;
	}
	//更新( View更新よりも速い )
	VOID CDirectXGameLogic::onUpdate(float step) 
	{

		//世界の更新
		m_world3d->Update(step);

	}
	//View設定時(View情報は自動的にタスクに追加されるのでその他の設定)
	 VOID CDirectXGameLogic::onAddView( LPTkyGameView view )
	{
		//障害物の追加
		IF( view->GetName() == "3DObstaclePhysicsModel" )
		{
			m_world3d->AddObtaclePhysicsModel( ((LPTkyObstacleModelView)view)->GetXFile() );
		}
		//モデルの追加
		IF( view->GetName() == "3DNormalPhysicsModel" )
		{
			m_world3d->AddPhysicsModel( ((LPTkyNormalModelView)view)->GetPhysicsModel() );
		}
	}
//イベント情報
	//イベントをもらう
	VOID CDirectXGameLogic::onEvent( TEventMessage msg )
	{
	}
//独自情報
	//生成
	LPTkyDirectXGameLogic CDirectXGameLogic::CreateDirectXGameLogic()
	{
		return new CDirectXGameLogic();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXテスト用
// 基本的なアプリケーションレイヤー　ダイレクトXに特化
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXApplication要素
	//コンストラクタ・デストラクタ
	CDirectXApplicationLayer::CDirectXApplicationLayer()
	{
		m_pDtata = NULL;
		m_pFont = NULL;	
		m_device = NULL;

		m_Info.pos = &m_pos;
		m_Info.vec = &m_vec;

	}
	CDirectXApplicationLayer::~CDirectXApplicationLayer()
	{

	}
	//キーの取得
	VOID				CDirectXApplicationLayer::pKeyBoard()
	{
//		assert(m_keybord);

		IF(m_device != NULL)
		{
			//キーイベント
			CHAR key[256];	
			IF(m_device->GetKeyBoard(key)==true)
			{
				//ダブルクリック用
				static DWORD lefttime = 0;
				static DWORD righttime = 0;

				TEventMessage msg = { "" , vector<DWORD>() };

				//キーを送る====================
				CEventManager::StaticAddEventMessage( "KEY_INFO" , (DWORD)&key );
				//========================================
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
	}
	//生成
	LPTkyDirectXApplicationLayer CDirectXApplicationLayer::CreateDirectXApplicationLayer()
	{
		return new CDirectXApplicationLayer();
	}
//WindowsApplication要素
	BOOL			CDirectXApplicationLayer::pOnCreate()
	{
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
		m_device = CTkyInput::CreateInput();
		HRESULT h = m_device->SetInputinfo(GetHInstance(),GetHWnd());
		IF(FAILED(h))
		{
			return false;
		}

		//環境の設定
		m_envi = CEnvironment::CreateEnvironment();
		m_envi->SetDevice(m_pDtata);

		//ロジック
		m_logic = CDirectXGameLogic::CreateDirectXGameLogic();
		m_logic->Create();

		//情報取得ビュー

		//初期化ビュー
		IF(!m_logic->AddGameView(CLoadGameView::CreateLoadGameView(m_logic,&m_Info)))return false;

		//セットタイマー(キー用)
		SetTimer( GetHWnd() , 10 , 10 , NULL );

	   return true;
	}
	BOOL			CDirectXApplicationLayer::pOnUpdate()
	{
		//キーボード
		pKeyBoard();

		//環境更新
		m_envi->SetLightParameterAt( &Vector(0,10,0),&Vector( 0,0,0 ),&D3DXVECTOR4( 0.1f,0.1f,0.1f ,1.0 ),&D3DXVECTOR4( 0.9f,0.9f,0.9f , 1.0f ),&D3DXVECTOR4( 0,0,0,0 ) );
		m_envi->SetCameraParameter( &m_pos , &m_vec ,60.0f , (float)WindowWidth/(float)WindowHeight , 0.1f , 100.0f );

		//ロジック
		m_logic->Update(1.0f);

		//環境設定
		m_envi->onExecute();

		//描画
		IF( m_pDtata->StartDraw() )
		{
			m_pDtata->Clear( D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(255,255,255,255));

			m_pDtata->SetColor(Vector(1,1,0));
			m_pDtata->DrawLine3D( Vector(3,0,3),Vector(-3,0,-3),10 );

			m_pDtata->SetColor(Vector(1,0,0));
			m_pDtata->DrawCircle( Vector(500,800,0),100,20 );
			m_pDtata->DrawLine( Vector(30,600,0),Vector(600,600,0),10 );

			//ロジックの描画
			m_logic->Render();

			//フォントの描画
			//文字の描画==============================
			m_pFont->SetColor(0,0,0);
			char str[200];
			sprintf_s(str,"Directx　Hello World!! FPS%d 　X:%d Y:%d Z:%d ",GetFps(),m_mouse.lX,m_mouse.lY,m_mouse.lZ	);
			m_pFont->DrawFontA(100,150,str);

			//========================================

			m_pDtata->EndDraw();
		}

		return true;
	}
//イベント要素
	VOID			CDirectXApplicationLayer::pOnEvent( TEventMessage msg )
	{
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			IF( message == WM_TIMER )
			{
				IF( wParam == 10 )
				{
					//マウスイベント
					m_mouse = m_device->GetDeviceEvent();
					CEventManager::StaticAddEventMessage( "MOUSE_INFO" , (DWORD)&m_mouse );
				}
			}
			IF( message == WM_ACTIVATE )
			{
				//デバイス
				IF(m_device!=NULL)m_device->SetDeviceAcquire(true);
			}
		}
	}

