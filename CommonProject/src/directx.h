#pragma once

#include<d3dx9.h>
#include <dinput.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include"Anime.h"
#include"TKYHLSL.h"
#include"TKYXFILE.h"
#include"TKYXSHADOW.h"
#include"TKY3D.h"
#include "TKYDIRECTINPUT.h"
#include "TKY2DTEXTURE.h"
#include "TKYFONT.h"
#include "TKYLIBRARY.h"
#include "TKYVECTOR.h"

using namespace std;


class VertexBuffer;
class VertexShader;
//--------------------------------------------------//
//消去関数
//--------------------------------------------------//
VOID AllDestroy();

//--------------------------------------------------//
//頂点バッファをつかう
//--------------------------------------------------//
class VertexBuffer
{

private:

	LPDIRECT3DVERTEXBUFFER9 g_pVB;

public:

	//コントラスタ
	VertexBuffer();

	//頂点バッファの初期化
	HRESULT Initialize(LPDIRECT3DDEVICE9 pD3DDev);

	//描画関数
	VOID Update(LPDIRECT3DDEVICE9 pD3DDev);

	//開放処理
	VOID Close();

};
//--------------------------------------------------//
//頂点バッファをつかう
//--------------------------------------------------//
class VertexShader
{

private:

	//頂点シェーダ
	LPDIRECT3DVERTEXSHADER9 m_pVS;
	//頂点宣言
	LPDIRECT3DVERTEXDECLARATION9 m_pVD;
	//DirectX9　インターフェイスのデバイス
	LPDIRECT3DDEVICE9 m_pD3DDev;							

public:

	//コントラスタ
	VertexShader(LPDIRECT3DDEVICE9 a_pD3DDev);

	//頂点シェーダの初期化
	HRESULT InitShader(LPCTSTR Filename);

	//頂点シェーダの定数設定
	VOID SetShader();

	//開放処理
	VOID Close();

};


