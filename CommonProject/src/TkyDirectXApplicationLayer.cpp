#include "TkyDirectXApplicationLayer.h"

//ダブルクリックの速さ
const static DWORD WindowDoubleClick = 100;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXテスト用
// 基本的なアプリケーションレイヤー　ダイレクトXに特化
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXApplication要素
	//コンストラクタ・デストラクタ
	IDirectXBaseApplicationLayer::IDirectXBaseApplicationLayer()
	{
		m_info.Initialize();
		m_vec = Vector(0,0,1);
		m_pos = Vector(  );
	}
	IDirectXBaseApplicationLayer::IDirectXBaseApplicationLayer(const IDirectXBaseApplicationLayer& x) :IWindowsApplicationLayer(x)
	{
		m_info = x.GetDirectXInfo();
	}
	IDirectXBaseApplicationLayer::~IDirectXBaseApplicationLayer()
	{
		IF(m_info.m_pDtata!=NULL)m_info.m_pDtata->DestroyOwnObject();
		IF(m_info.m_pFont!=NULL)m_info.m_pFont->DestroyOwnObject();	
		IF(m_info.m_device!=NULL)m_info.m_device->DestroyOwnObject();
		IF(m_info.m_envi!=NULL)m_info.m_envi->DestroyOwnObject();
	}
	//キーの取得
	VOID				IDirectXBaseApplicationLayer::pKeyBoard()
	{
//		assert(m_keybord);

		IF(m_info.m_device != NULL)
		{
			//キーイベント
			CHAR key[256];	
			IF(m_info.m_device->GetKeyBoard(key)==true)
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
//WindowsApplication要素
	BOOL			IDirectXBaseApplicationLayer::pOnCreate()
	{
		//データの取得
		m_info.m_pDtata = Init3D::CreateInit3D();
		IF_FAILED( m_info.m_pDtata->Create3DX(GetHWnd()) )
		{
			MessageBoxA( NULL ,"エラー" , "INIT3D" ,NULL );
			return FALSE;
		}
		//フォントデータ取得
		m_info.m_pFont = CFONT::CreateFontW(m_info.m_pDtata);
		IF_FAILED( m_info.m_pFont->GetState() )
		{
			MessageBoxA( NULL , "sdadf" , "csdaf" , MB_OK);
			return FALSE;
		}
		//キーボード情報
		m_info.m_device = CTkyInput::CreateInput();
		HRESULT h = m_info.m_device->SetInputinfo(GetHInstance(),GetHWnd());
		IF(FAILED(h))
		{
			MessageBoxW( NULL , TEXT("キーボード失敗") , TEXT("err") , NULL );
			return false;
		}

		//環境の設定
		m_info.m_envi = CEnvironment::CreateEnvironment();
		m_info.m_envi->SetDevice(m_info.m_pDtata);

		//セットタイマー(キー用)
		SetTimer( GetHWnd() , 10 , 10 , NULL );

		//カメラ情報
		m_info.m_pos = &m_pos;
		m_info.m_vec = &m_vec;

		//継承先の動作
		IF(!pOnCreateDirectX())
		{
			MessageBoxW( NULL , TEXT("DirectX失敗") , TEXT("err") , NULL );
			return false;
		}

	   return true;
	}
	BOOL			IDirectXBaseApplicationLayer::pOnUpdate()
	{
		//キーボード
		pKeyBoard();

		//環境更新
		m_info.m_envi->SetLightParameterAt( &Vector(0,10,0),&Vector( 0,0,0 ),&D3DXVECTOR4( 0.1f,0.1f,0.1f ,1.0 ),&D3DXVECTOR4( 0.9f,0.9f,0.9f , 1.0f ),&D3DXVECTOR4( 0,0,0,0 ) );
		m_info.m_envi->SetCameraParameter( &Vector(0,0,0) , &Vector(0,0,1) ,60.0f , GetWindowsSize().x/GetWindowsSize().y , 0.1f , 500.0f );

		//継承先の更新
		IF(!pOnUpdateDirectX( 1.0f ))
		{
			MessageBoxW( NULL , TEXT("DirectXUpdate失敗") , TEXT("err") , NULL );
			return false;
		}

		//カメラの向きの設定
		GetDirectXInfo().m_envi->SetCameraParameter( &m_pos , &m_vec );

		//環境設定
		m_info.m_envi->onExecute();

		//描画
		IF( m_info.m_pDtata->StartDraw() )
		{
			//画面の初期化
			m_info.m_pDtata->Clear( D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(0,0,0,0));

			//フォントの描画
			//文字の描画==============================
//			m_info.m_pFont->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"Directx　Hello World!! FPS%d 　X:%d Y:%d Z:%d ",GetFps(),m_info.m_mouse.lX,m_info.m_mouse.lY,m_info.m_mouse.lZ	);
//			m_info.m_pFont->DrawFontA(100,150,str);

			//========================================

			//継承先の描画
			pOnRenderDirectX();

			m_info.m_pDtata->EndDraw();
		}

		return true;
	}
//イベント要素
	VOID			IDirectXBaseApplicationLayer::pOnEvent( TEventMessage msg )
	{
		//継承先のイベント
		pOnEventDirectX( msg );
		//普通イベント
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			IF( message == WM_MOUSEMOVE )
			{
				m_info.m_absmouse.x = (float)LOWORD(lParam);
				m_info.m_absmouse.y = (float)HIWORD(lParam);
			}
			IF( message == WM_TIMER )
			{
				IF( wParam == 10 )
				{
					//マウスイベント
					m_info.m_mouse = m_info.m_device->GetDeviceEvent();
					CEventManager::StaticAddEventMessage( "MOUSE_INFO" , (DWORD)&m_info.m_mouse );
				}
			}
			IF( message == WM_ACTIVATE )
			{
				//デバイス
				IF(m_info.m_device!=NULL)m_info.m_device->SetDeviceAcquire(true);
			}
		}
	}

