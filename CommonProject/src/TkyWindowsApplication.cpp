#include "TkyWindowsApplication.h"

//アプリケーション

	IWindowsApplicationLayer::IWindowsApplicationLayer()
	{
		m_looptime	=0;
		m_fps			=0;
		m_WindowHeight = 0;
		m_WindowWidth = 0;
	}
	IWindowsApplicationLayer::IWindowsApplicationLayer(const IWindowsApplicationLayer& x)
	{
		m_looptime	=x.m_looptime;
		m_fps			=x.m_fps;
		m_WindowHeight = x.m_WindowHeight;
		m_WindowWidth = x.m_WindowWidth;
		m_hInst = x.m_hInst;								
		m_hWnd = x.m_hWnd;
		StringCchCopy( this->szTitle , MAX_LOADSTRING , x.szTitle );
		StringCchCopy( this->szWindowClass , MAX_LOADSTRING , x.szWindowClass );
	}
	IWindowsApplicationLayer::~IWindowsApplicationLayer()
	{
	}

//WindowView要素

	//設定
	VOID IWindowsApplicationLayer::SetWindowInfo(HINSTANCE hInstance , UINT w, UINT h)
	{
		m_hInst = hInstance;								
		m_WindowHeight = h;
		m_WindowWidth= w;
	}

//GameAppllication要素

	//更新処理
	BOOL IWindowsApplicationLayer::onLoopStep()
	{

 		// TODO: ここにコードを挿入してください。
		MSG msg;
		HACCEL hAccelTable;

		hAccelTable = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDC_COMMONPROJECT));

		// メイン メッセージ ループ:
		WHILE(1)
		{
			//メッセージ取得
			IF(PeekMessage (&msg,NULL,0,0,PM_NOREMOVE)) {
				//メッセージがまで待つ（WM_QUITだったら０を返す）
				IF(GetMessage(&msg, NULL, 0, 0))
				{
					if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
					{
						TranslateMessage(&msg);	//キーボード関連のイベント
						DispatchMessage(&msg);	//Windowsシステムに転送する関数
					}		
				}
				else
				{
					break;
				}
			}
			else
			{
				//開始時間
				DWORD time  = timeGetTime();

				pOnUpdate();
				m_looptime++;

				//FPS制御タイムの設定
				IF( ( timeGetTime() - time ) < (1.0f/LOOK_FPS)*1000 )
				{
					time = (DWORD)((1.0f/LOOK_FPS)*1000 - ( timeGetTime() - time ));
				}
				else
				{
					time = 0;
				}
				//FPS固定動作
				Sleep(time);
			}
		}

		return (int)msg.wParam;
	}
	//初期化
	BOOL IWindowsApplicationLayer::onCreate()
	{

		// グローバル文字列を初期化しています。
		LoadString(m_hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(m_hInst, IDC_COMMONPROJECT, szWindowClass, MAX_LOADSTRING);
		MyRegisterClass(m_hInst);

		// アプリケーションの初期化を実行します:
		if (!InitInstance (m_hInst, SW_SHOW))
		{
			return FALSE;
		}
		IF(!pOnCreate())
		{
			return FALSE;
		}

		return TRUE;

	}


//イベント要素

	//イベントをもらう
	VOID IWindowsApplicationLayer::onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			IF( message == WM_TIMER )
			{
				IF( wParam == TIMER_FPS )
				{
					m_fps = m_looptime;
					m_looptime=0;
				}
			}
		}
		pOnEvent( msg );
	}
	//イベントネーム名前を決める
	VOID IWindowsApplicationLayer::onNameSet( string *str )
	{
		*str = "WindowsApplication";
	}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM IWindowsApplicationLayer::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMMONPROJECT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_COMMONPROJECT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}
//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL IWindowsApplicationLayer::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   m_hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(
		szWindowClass,					//  LPCTSTR lpClassName,  // 登録されているクラス名
		szTitle,								//  LPCTSTR lpWindowName, // ウィンドウ名
		WS_OVERLAPPEDWINDOW	//  DWORD dwStyle,        // ウィンドウスタイル
		^ WS_MAXIMIZEBOX ^ WS_SIZEBOX,
		CW_USEDEFAULT,				//  int x,                // ウィンドウの横方向の位置
		0,										//  int y,                // ウィンドウの縦方向の位置
		m_WindowWidth + 6,					//  int nWidth,           // ウィンドウの幅
		m_WindowHeight + 49,					//  int nHeight,          // ウィンドウの高さ
		NULL,								//  HWND hWndParent,      // 親ウィンドウまたはオーナーウィンドウのハンドル
		NULL,								//  HMENU hMenu,          // メニューハンドルまたは子ウィンドウ ID
		hInstance,							//  HINSTANCE hInstance,  // アプリケーションインスタンスのハンドル
		NULL									//  LPVOID lpParam        // ウィンドウ作成データ
		);

   if (!hWnd)
   {
	  MessageBoxA( NULL ,"エラー" , "HWND" ,NULL );
      return FALSE;
   }

   m_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd,TIMER_FPS,1000,NULL);

   return TRUE;
}
//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK IWindowsApplicationLayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	Vector vec;
	float by = 0;

	//イベント情報
	TEventMessage msg;

	//Winodwsイベント送信
	msg.m_message = "WINDOWS_EVENT";
	msg.m_value.resize(3);
	msg.m_value[0] = message;
	msg.m_value[1] = wParam;
	msg.m_value[2] = lParam;
	CEventManager::StaticAddEventMessage(msg);

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox( (HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE) , MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CHAR:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_TIMER:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATE:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK IWindowsApplicationLayer::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	}
	return (INT_PTR)FALSE;
}
