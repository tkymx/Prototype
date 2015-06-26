#pragma once

#include<d3dx9.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include "TKYHLSL.h"
#include "TKY3D.h"
#include "TKY2DTEXTURE.h"


//シャドウパス
enum ShadowPass{
	D3DTKYSHADOW_NORMAL ,
	D3DTKYSHADOW_SPOT
};

//-------------------------------------------------------------
// 影作成時のXFILEのローカル座標情報用の構造体
//-------------------------------------------------------------
typedef struct {
	D3DTKYXANIMETION	*anime;
    FLOAT       m_x,m_y,m_z,m_r;
} XandPOS;

//--------------------------------------------------//
//影の作成
//--------------------------------------------------//
class ShadowProc
{

private:
	
	//レンダリングテクスチャ
	LPDIRECT3DTEXTURE9			m_pRenderingTexture;	//最終的なレンダリングテクスチャ
	LPDIRECT3DSURFACE9			m_pRenderingSurf;		//↑のサーフェスのインターフェイス
	LPDIRECT3DSURFACE9			m_pRenderingZ;			//深度ステンシル サーフェイスを設定する。

	//一時用のテクスチャ
	LPDIRECT3DTEXTURE9			m_pTempTexture;			//最終的なレンダリングテクスチャ
	LPDIRECT3DSURFACE9			m_pTempSurf;			//↑のサーフェスのインターフェイス
	LPDIRECT3DSURFACE9			m_pTempZ;				//深度ステンシル サーフェイスを設定する。

	//影のテクスチャ（影の分だけ配列を作る）
	LPDIRECT3DTEXTURE9			m_pDephTex; 			//影のテクスチャ
	LPDIRECT3DSURFACE9			m_pDephSurf;			//↑のサーフェスのインターフェイス
	LPDIRECT3DSURFACE9			m_pDephTexZ;			//深度ステンシル サーフェイスを設定する。

	//外部の変数を使う
	LPDIRECT3DDEVICE9			m_pD3DDevice;			//DirectX9　インターフェイスのデバイス
	HLSLShader					*m_pHLSL;				//HLSLの設定

	vector<XandPOS>				m_tvXfileInfo;			//影作成時のXFILE情報（可変配列）

	//シャドウ作成以前のレンダリングターゲットの保存メンバ変数
	LPDIRECT3DSURFACE9			m_pOldBackBuffer;
	LPDIRECT3DSURFACE9			m_pOldZBuffer;
	D3DVIEWPORT9				m_oldViewport;

	//レンダリングの保存
	VOID pStartReadShadow();

	//レンダリングの再開
	VOID pEndReadShadow();

	//モデルの描画
	VOID pRenderModel(int pass);

public:

	//コンストラクタ
	ShadowProc(LPTKY3D a_pinit3d,HLSLShader *a_pHLSL);

	//影の作成
	HRESULT InitShadow();

	//影つきXファイルの描画を始める
	VOID StartDrawShadow();

	//影つきXファイルの描画を終了する
	VOID EndDrawShadow(int pass);

	//影作成用のXFIE設定
	VOID SetShadowXfile(D3DTKYXANIMETION  *anime,float x,float y,float z,float rad);

	//シャドウテクスチャのデバック表示
	VOID DebugShadow(int pass);

	//開放する
	VOID Close();

};