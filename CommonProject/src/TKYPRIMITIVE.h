#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"


//頂点配列数
#define VERTEX_SIZE 100

//--------------------------------------------------//
//定義領域
//--------------------------------------------------//
//頂点バッファー関係
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
typedef struct{

	FLOAT x,y,z,rhw;	//スクリーン座標での位置
	DWORD color;		//頂点色

}CUSTOMVERTEX;

#define D3DFVF_CUSTOMVERTEX3DTEXTURE (D3DFVF_XYZ|D3DFVF_NORMAL | D3DFVF_TEX1)
typedef struct{

	D3DXVECTOR3 pos;	//スクリーン座標での位置
	D3DXVECTOR3 normal;//法線
	FLOAT Uvec,Vvec;

}CUSTOMVERTEX3DTEXTURE;

#define D3DFVF_CUSTOMVERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL )
typedef struct{

	D3DXVECTOR3 pos;	//スクリーン座標での位置
	D3DXVECTOR3 normal;//法線


}CUSTOMVERTEX3D;


//------------------------------------------------
//頂点情報の描画に使う(シングルトン)
//------------------------------------------------


CLASS_POINTA_OBJECT( CPrimitive , LPTkyPrimitive )
{
private:

	//シングルトン用
	STATIC LPTkyPrimitive m_single;

	//頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	//デバイス情報
	LPDIRECT3DDEVICE9			 m_device;

	CPrimitive(LPDIRECT3DDEVICE9 device)
	{
		m_device = device;
		m_pVertexBuffer = NULL;
	}
	CPrimitive(const CPrimitive&)
	{
	}
	~CPrimitive()
	{
		SAFE_RELEASE( m_pVertexBuffer );
	}

	//色データ
	DWORD m_Color;

public:

	//色
	BOOL SetColor(Vector c);
	BOOL SetColor(DWORD c){m_Color=c;return TRUE;};
	//線描画
	HRESULT DrawLine(Vector start,Vector end , float thick);
	//頂点データの初期化
	HRESULT InitializeVertexBuffer();

	//生成
	STATIC LPTkyPrimitive CreatePrimitive( LPDIRECT3DDEVICE9 device )
	{
		IF(m_single == NULL)m_single = new CPrimitive(device);
		return m_single;
	}

};
