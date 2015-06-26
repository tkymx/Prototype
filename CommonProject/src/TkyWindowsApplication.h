
#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "directx.h"

//FPSタイマーの番号
#define TIMER_FPS 100

//FPS固定
#define LOOK_FPS (80.0f)



#define MAX_LOADSTRING 100
//アプリケーション
CLASS_POINTA( IWindowsApplicationLayer , LPTkyWindowsApplicationLayer ) :public IApplicationLayer
{
//	friend class IApplicationLayer;

private:

	//変数
	UINT			m_looptime	;	//ループタイム
	UINT			m_fps			;	//FPS
	TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
	TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

	//ウィンドウのサイズ
	UINT m_WindowWidth;
	UINT m_WindowHeight;

	// このコード モジュールに含まれる関数の宣言を転送します:
	ATOM									MyRegisterClass(HINSTANCE hInstance);
	BOOL									InitInstance(HINSTANCE, int);
	STATIC LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	STATIC INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

	//Window情報
	HINSTANCE	m_hInst;								
	HWND		m_hWnd;

protected:

	IWindowsApplicationLayer();
	IWindowsApplicationLayer(const IWindowsApplicationLayer& );
	VIRTUAL ~IWindowsApplicationLayer();

	//仮想化
	VIRTUAL	BOOL			pOnCreate()=0;
	VIRTUAL	BOOL			pOnUpdate()=0;
	VIRTUAL	VOID			pOnEvent(TEventMessage msg)=0;

public:

//WindowView要素

	//設定
	VOID SetWindowInfo(HINSTANCE hInstance , UINT w, UINT h);

	//取得
	HWND GetHWnd()const{return m_hWnd;}
	HINSTANCE GetHInstance()const{return m_hInst;}
	UINT GetFps()const{return m_fps;}
	Vector GetWindowsSize()const{return Vector( (float)m_WindowWidth , (float)m_WindowHeight , 0 );}

//GameAppllication要素

	//更新処理
	VIRTUAL BOOL onLoopStep();

	//初期化
	VIRTUAL BOOL onCreate();

//イベント要素

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );

	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str );

};



