#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyWindowsApplication.h"
#include "directx.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectX構造体
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TDirectXInfo )
{
	//基本クラス
	LPTKY3D m_pDtata;
	//文字表示クラス
	LPTKYFONT m_pFont;
	//デバイスの取得
	LPTKYINPUT m_device;
	//環境
	LPTkyEnvironment m_envi;
	//マウス情報
	DIMOUSESTATE m_mouse;
	//マウス絶対情報
	Vector m_absmouse;
	//カメラの位置構造体
	Vector *m_pos;
	//カメラの向き構造体
	Vector *m_vec;

	//初期化
	VOID Initialize()
	{
		m_pDtata = NULL;
		m_pFont = NULL;
		m_device = NULL;
		m_envi = NULL;
		m_mouse = DIMOUSESTATE();
		m_absmouse = Vector();
		m_pos = NULL;
		m_vec = NULL;
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXテスト用
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IDirectXBaseApplicationLayer , LPTkyDirectXBaseApplicationLayer ) :public IWindowsApplicationLayer
{
private:
//DirectX情報

	//DirectX構造体
	TDirectXInfo m_info;

	//高さ
	Vector m_pos;
	//視点
	Vector m_vec;

private:

//キーの取得
	VOID							pKeyBoard();
//WindowsApplication要素	
	VIRTUAL BOOL			pOnCreate();
	VIRTUAL BOOL			pOnUpdate();
//イベント要素
	VIRTUAL	VOID			pOnEvent( TEventMessage msg );

protected:

//DirectXApplication要素
	//コンストラクタ・デストラクタ
	IDirectXBaseApplicationLayer();
	IDirectXBaseApplicationLayer(const IDirectXBaseApplicationLayer&) ;
	VIRTUAL ~IDirectXBaseApplicationLayer();

	//DirectX用仮想関数
	VIRTUAL BOOL pOnCreateDirectX()=0;
	VIRTUAL BOOL pOnUpdateDirectX(float step)=0;
	VIRTUAL VOID pOnRenderDirectX()=0;
	VIRTUAL VOID pOnEventDirectX(TEventMessage msg )=0;

public:

	//高さ
	Vector GetCameraPosition()const{return m_pos;}
	//視点
	Vector GetCameraDirection()const{return m_vec;}

	//取得
	//Directx
	TDirectXInfo GetDirectXInfo()const{return m_info;}
	TDirectXInfo* GetDirectXInfoPointa()const{return (TDirectXInfo*)(&m_info);}
	
};