// CommonProject.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "CommonProject.h"

//CommonWindowsApplicationLayer
//LPTkyCommonWindowsApplicationLayer g_windows;
//LPTkyDirectXApplicationLayer g_windows;
LPTkyFPSApplicationLayer g_windows;

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR    lpCmdLine,int       nCmdShow)
{

	// メモリリーク検出
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );

	ITkyObject::CreateObject();

//	g_windows = CCommonWindowsApplicationLayer::CreateCommonWindowsApplicationLayer();
//	g_windows = CDirectXApplicationLayer::CreateDirectXApplicationLayer();
	g_windows = CFPSApplicationLayer::CreateFPSApplicationLayer();

	g_windows->SetWindowInfo(hInstance , WindowWidth ,WindowHeight );

	IF(!g_windows->Create())
	{
		ITkyObject::DestroyObject();
		return FALSE;
	}

	BOOL h = g_windows->LoopStart();

#ifdef _DEBUG
	//スレッド数の確認
	CHAR str[MAX_PATH];
	_snprintf_s( str , MAX_PATH ,  "Thread:%d" , ITkyThread::GetThreadNum()  );
//	MessageBoxA( NULL , str , "ThreadSTART" ,NULL );
#endif

	ITkyObject::DestroyObject();
	
#ifdef _DEBUG
	//スレッド数の確認
	CHAR str1[MAX_PATH];
	_snprintf_s( str1 , MAX_PATH ,  "Thread:%d" , ITkyThread::GetThreadNum()  );
//	MessageBoxA( NULL , str1 , "ThreadEnd" ,NULL );
#endif


	return 0;
	
}

