#include "TkyWindowsApplication.h"

//�A�v���P�[�V����

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

//WindowView�v�f

	//�ݒ�
	VOID IWindowsApplicationLayer::SetWindowInfo(HINSTANCE hInstance , UINT w, UINT h)
	{
		m_hInst = hInstance;								
		m_WindowHeight = h;
		m_WindowWidth= w;
	}

//GameAppllication�v�f

	//�X�V����
	BOOL IWindowsApplicationLayer::onLoopStep()
	{

 		// TODO: �����ɃR�[�h��}�����Ă��������B
		MSG msg;
		HACCEL hAccelTable;

		hAccelTable = LoadAccelerators(m_hInst, MAKEINTRESOURCE(IDC_COMMONPROJECT));

		// ���C�� ���b�Z�[�W ���[�v:
		WHILE(1)
		{
			//���b�Z�[�W�擾
			IF(PeekMessage (&msg,NULL,0,0,PM_NOREMOVE)) {
				//���b�Z�[�W���܂ő҂iWM_QUIT��������O��Ԃ��j
				IF(GetMessage(&msg, NULL, 0, 0))
				{
					if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
					{
						TranslateMessage(&msg);	//�L�[�{�[�h�֘A�̃C�x���g
						DispatchMessage(&msg);	//Windows�V�X�e���ɓ]������֐�
					}		
				}
				else
				{
					break;
				}
			}
			else
			{
				//�J�n����
				DWORD time  = timeGetTime();

				pOnUpdate();
				m_looptime++;

				//FPS����^�C���̐ݒ�
				IF( ( timeGetTime() - time ) < (1.0f/LOOK_FPS)*1000 )
				{
					time = (DWORD)((1.0f/LOOK_FPS)*1000 - ( timeGetTime() - time ));
				}
				else
				{
					time = 0;
				}
				//FPS�Œ蓮��
				Sleep(time);
			}
		}

		return (int)msg.wParam;
	}
	//������
	BOOL IWindowsApplicationLayer::onCreate()
	{

		// �O���[�o������������������Ă��܂��B
		LoadString(m_hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(m_hInst, IDC_COMMONPROJECT, szWindowClass, MAX_LOADSTRING);
		MyRegisterClass(m_hInst);

		// �A�v���P�[�V�����̏����������s���܂�:
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


//�C�x���g�v�f

	//�C�x���g�����炤
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
	//�C�x���g�l�[�����O�����߂�
	VOID IWindowsApplicationLayer::onNameSet( string *str )
	{
		*str = "WindowsApplication";
	}

//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL IWindowsApplicationLayer::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   m_hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(
		szWindowClass,					//  LPCTSTR lpClassName,  // �o�^����Ă���N���X��
		szTitle,								//  LPCTSTR lpWindowName, // �E�B���h�E��
		WS_OVERLAPPEDWINDOW	//  DWORD dwStyle,        // �E�B���h�E�X�^�C��
		^ WS_MAXIMIZEBOX ^ WS_SIZEBOX,
		CW_USEDEFAULT,				//  int x,                // �E�B���h�E�̉������̈ʒu
		0,										//  int y,                // �E�B���h�E�̏c�����̈ʒu
		m_WindowWidth + 6,					//  int nWidth,           // �E�B���h�E�̕�
		m_WindowHeight + 49,					//  int nHeight,          // �E�B���h�E�̍���
		NULL,								//  HWND hWndParent,      // �e�E�B���h�E�܂��̓I�[�i�[�E�B���h�E�̃n���h��
		NULL,								//  HMENU hMenu,          // ���j���[�n���h���܂��͎q�E�B���h�E ID
		hInstance,							//  HINSTANCE hInstance,  // �A�v���P�[�V�����C���X�^���X�̃n���h��
		NULL									//  LPVOID lpParam        // �E�B���h�E�쐬�f�[�^
		);

   if (!hWnd)
   {
	  MessageBoxA( NULL ,"�G���[" , "HWND" ,NULL );
      return FALSE;
   }

   m_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd,TIMER_FPS,1000,NULL);

   return TRUE;
}
//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK IWindowsApplicationLayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	Vector vec;
	float by = 0;

	//�C�x���g���
	TEventMessage msg;

	//Winodws�C�x���g���M
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
		// �I�����ꂽ���j���[�̉��:
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
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
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
// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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
