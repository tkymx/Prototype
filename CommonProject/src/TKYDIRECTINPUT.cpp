#include "TKYDIRECTINPUT.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		入力関係
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
CTkyInput::CTkyInput()
{
	CTkyInput::m_IsAcquire=false;
	CTkyInput::g_pDI = NULL;
	CTkyInput::g_pKeyDevice = NULL;
	CTkyInput::g_pMouseDevice = NULL;
}

CTkyInput::~CTkyInput()
{
	this->Release();
}
//---------------------------------------------//
//詳細設定を行う
//---------------------------------------------//
HRESULT CTkyInput::SetInputinfo(HINSTANCE a_hInst,HWND a_hWnd)
{
	HRESULT             hr; 
	HANDLE g_hMouseEvent = NULL;

	//DirectInput オブジェクトの作成
	hr = DirectInput8Create(a_hInst, DIRECTINPUT_VERSION, 
			IID_IDirectInput8, (void**)&g_pDI, NULL); 
	IF(FAILED(hr) )
	{ 
		return FALSE;
		// DirectInput not available; take appropriate action 
	} 

	//マウス情報

		//マウスデバイスの作成
		hr = g_pDI->CreateDevice(GUID_SysMouse, &g_pMouseDevice, NULL);
		IF(FAILED(hr)) {Release();return FALSE;}

		//マウスデータ形式の設定
		hr = g_pMouseDevice->SetDataFormat(&c_dfDIMouse);
		IF(FAILED(hr)) {Release();return FALSE;}

		//マウス動作の設定
		hr = g_pMouseDevice->SetCooperativeLevel(a_hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		IF(FAILED(hr)) {Release();return FALSE;}
/*	
		//バッファの指定らしい
		#define SAMPLE_BUFFER_SIZE  16

		//イベントを作成する
		g_hMouseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (g_hMouseEvent == NULL) {return FALSE;}

		//マウスにイベントを関連づける
		hr = g_pDIDevice->SetEventNotification(g_hMouseEvent);
		if (FAILED(hr)) {return FALSE;}
*/
		//パラメーターの設定
		DIPROPDWORD dipdw;
		ZeroMemory(&dipdw,sizeof(DIPROPDWORD));
		//定型文
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		//アクセスするプロパティが属するオブジェク
		dipdw.diph.dwObj = 0;
		//使用方法？
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL; // 相対座標モード
//		dipdw.dwData = DIPROPAXISMODE_ABS; // 絶対座標モード
//		dipdw.dwData = SAMPLE_BUFFER_SIZE;
		//情報の設定
		hr = g_pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		IF(FAILED(hr)) {return FALSE;}

	//キーボード時

		//キーボードデバイスの作成
		hr = g_pDI->CreateDevice(GUID_SysKeyboard, &g_pKeyDevice, NULL); 
		if(FAILED(hr)) {Release();return FALSE;} 

		//キーボードデータフォーマットの作成(定義済みの変数にデータフォーマットを入れる)
		hr = g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard); 
		if(FAILED(hr)) {Release();return FALSE;} 

		//キーボードの動作の設定
		hr = g_pKeyDevice->SetCooperativeLevel(a_hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
		if(FAILED(hr)){Release();return FALSE;} 

		//マウスキーボードへのアクセス権の取得
		this->SetDeviceAcquire(true);


	return TRUE;
}
//---------------------------------------------//
//マウスの値の取得
//---------------------------------------------//
DIMOUSESTATE CTkyInput::GetDeviceEvent()
{
	DIMOUSESTATE dims = {0,0,0,0};

	//アクセス権があったら
	if(m_IsAcquire)		
	{
		g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&dims);
	}

	return dims;

}

//---------------------------------------------//
//キーボードの値の取得
//---------------------------------------------//
bool CTkyInput::GetKeyBoard(char Abuffer[256])				//キーの状態を取得する
{
	
	CHAR buffer[256];
	ZeroMemory(buffer,sizeof(buffer));
    HRESULT  hr; 
	hr = g_pKeyDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
    if FAILED(hr) 
    { 
        return false; 
    }
	memcpy(Abuffer,buffer,sizeof(buffer));
	
	return true;
}
//---------------------------------------------//
//アクセス権の管理
//---------------------------------------------//
void CTkyInput::SetDeviceAcquire(bool a_Acquire)
{
	IF(g_pKeyDevice!=NULL)
	{
		if(a_Acquire)	{
			g_pKeyDevice->Acquire();
			m_IsAcquire = a_Acquire;
		}else{
			m_IsAcquire = a_Acquire;
			g_pKeyDevice->Unacquire();
		}
	}
	IF(g_pMouseDevice!=NULL)
	{
		if(a_Acquire)	{
			g_pMouseDevice->Acquire();
			m_IsAcquire = a_Acquire;
		}else{
			m_IsAcquire = a_Acquire;
			g_pMouseDevice->Unacquire();
		}
	}
}

//---------------------------------------------//
//開放処理
//---------------------------------------------//
VOID CTkyInput::Release()
{
    if (g_pDI) 
    { 
		if (g_pKeyDevice) 
        { 
			//デバイスの解放処理
			g_pKeyDevice->Unacquire(); 
			g_pKeyDevice->Release();
			g_pKeyDevice = NULL; 
        } 
		if (g_pMouseDevice) 
        { 
			//デバイスの解放処理
			g_pMouseDevice->Unacquire(); 
			g_pMouseDevice->Release();
			g_pMouseDevice = NULL; 
        } 
		//DirectInput　の　ハンドル　の　解放処理
        g_pDI->Release();
        g_pDI = NULL; 
    } 
}

