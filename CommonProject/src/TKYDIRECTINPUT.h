#pragma once

#include<d3dx9.h>
#include <dinput.h>
#include "TKYBASEHEADER.h"

#define KEYDOWN(name,key) (name[key] & 0x80)

CLASS(CTkyInput);
TYPEDEF(CTkyInput,*LPTKYINPUT);

/*
ここではデバイス関係の取得を行う

SetInputinfo(int type,HINSTANCE a_hIns,HWND a_hWnd);	
でデバイスごとに初期化をする

DIMOUSESTATE2	GetDeviceEvent();					//マウスの状態を取得する
bool					GetKeyBoard(char*);				//キーの状態を取得する
でキーを取得する
取得する際はタイマーなど常に通過する地点に配置すること

キーボード時はKEYDOWN(name,key)
マクロを使用する
keyには
http://msdn.microsoft.com/ja-jp/library/cc351922.aspx
を参照する

*/

//--------------------------------------------------//
//デバイスの操作の作成
//--------------------------------------------------//
enum tinput{KEYBORD,MOUSE};	//デバイスの種類
CLASS_OBJECT(CTkyInput)
{

private:

	bool	m_IsAcquire;				//アクセス権の有無
	LPDIRECTINPUT8 g_pDI;					//DirectInput　の　ハンドル

	LPDIRECTINPUTDEVICE8  g_pKeyDevice;		//キーデバイスの準備
	LPDIRECTINPUTDEVICE8  g_pMouseDevice;		//マウスデバイスの準備

	CTkyInput();						//初期化を行う
	VIRTUAL ~CTkyInput();					//デストラクタ

public:



	HRESULT SetInputinfo(HINSTANCE a_hIns,HWND a_hWnd);				//インプットの設定を行う

	DIMOUSESTATE	GetDeviceEvent();					//マウスの状態を取得する
	bool					GetKeyBoard(char buffer[256]);				//キーの状態を取得する

	void	SetDeviceAcquire(bool a_Acquire);				//デバイスのアクセス権の管理

	//解放処理
	VOID Release();		
	static LPTKYINPUT CreateInput(){return new CTkyInput();};

};
