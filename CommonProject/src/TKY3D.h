#pragma once

#include<d3dx9.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TKYPRIMITIVE.h"

using namespace std;
/*
//--------------------------------------------------//
//定義領域
//--------------------------------------------------//
//頂点バッファー関係
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZW|D3DFVF_DIFFUSE)
typedef struct{

	FLOAT x,y,z,rhw;	//スクリーン座標での位置
	DWORD color;		//頂点色

}CUSTOMVERTEX;
*/
//--------------------------------------------------//
//DirectXの初期化を行う
//--------------------------------------------------//
CLASS_POINTA_OBJECT(Init3D,LPTKY3D){

private:

	LPDIRECT3D9 g_pD3D;										//DirectX9　インターフェイス
	LPDIRECT3DDEVICE9 g_pD3DDevice;							//DirectX9　インターフェイスのデバイス

	static vector<Init3D*>  m_vIndex;

	Init3D(){ g_pD3D = NULL ; g_pD3DDevice = NULL ; m_primitive = NULL; }
	VIRTUAL ~Init3D()
	{
		clean3D();
	};

	//色
	DWORD m_Color;
	Vector m_vColor;

	//プリミティブデータ
	LPTkyPrimitive m_primitive;

	//シングルトン
	STATIC LPTKY3D m_single3D;

public:

	//DirectX基幹関数
	HRESULT Create3DX(HWND hWnd);

	void clean3D();

	//描画処理関数
	HRESULT StartDraw();
	void EndDraw();
	void Clear(int type,UINT color); //D3DCLEAR_TARGET , D3DCOLOR_ARGB(255,255,255,255)

	//情報閲覧関数
	LPDIRECT3D9 GetD3D(){return g_pD3D;};					//DirectX9　インターフェイス
	LPDIRECT3DDEVICE9 GetD3DDevice(){return g_pD3DDevice;};	//DirectX9　インターフェイスのデバイス

	//画面の大きさの取得
	Vector GetScreenSize()
	{
		RECT r;
		g_pD3DDevice->GetScissorRect(&r);
		return Vector( (float)r.right , (float)r.bottom , 0 );
	}

	//通常描画
	BOOL SetColor(Vector);
	BOOL DrawLine(Vector,Vector,float);
	BOOL DrawLine3D(Vector,Vector,float);
	BOOL DrawSquare(Vector,Vector,Vector,Vector);
	BOOL DrawCircle(Vector p1,float r,int k);

	//3Dの取得
//	static Init3D *CreateInit3D(){Init3D *i = new Init3D();m_vIndex.push_back(i); return i; }
//	static VOID Destroy(){FOR_I(m_vIndex.size())SAFE_DELETE(m_vIndex[i]);}
	static Init3D *CreateInit3D()
	{
		IF(m_single3D==NULL)m_single3D = new Init3D();
		return m_single3D;
	}

};
