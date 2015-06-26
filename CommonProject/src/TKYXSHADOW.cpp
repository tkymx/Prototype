
#include "TKYXSHADOW.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		影関係
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
ShadowProc::ShadowProc(LPTKY3D a_pinit3d,HLSLShader *a_pHLSL)
{

	m_pD3DDevice = a_pinit3d->GetD3DDevice();
	m_pDephSurf = NULL;
	m_pDephTex  = NULL;
	m_pDephTexZ = NULL;

	m_pHLSL = a_pHLSL;

}
//---------------------------------------------//
//影の作成
//---------------------------------------------//
HRESULT ShadowProc::InitShadow()
{
	//サンプリング領域
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//画面幅の取得
	UINT SCREEN_W = (UINT)rect.right;
	UINT SCREEN_H = (UINT)rect.bottom;

	//-----------------------------------------
	//レンダリングテクスチャの設定
	//-----------------------------------------

	//シャドウテクスチャの設定
	if( FAILED( m_pD3DDevice->CreateTexture(
		SCREEN_W,
		SCREEN_H,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pRenderingTexture,
		NULL) ) )
		return E_FAIL;

	//↑のサーフェスのインターフェイスの作成
	if( FAILED( m_pRenderingTexture->GetSurfaceLevel(0,&m_pRenderingSurf) )  )
		return E_FAIL;

	//深度ステンシル サーフェイスを設定する。
	if( FAILED( m_pD3DDevice->CreateDepthStencilSurface(
		SCREEN_W,
		SCREEN_H,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pRenderingZ,
		NULL) ) )
		return E_FAIL;

	//-----------------------------------------
	//一時テクスチャの設定
	//-----------------------------------------
	//シャドウテクスチャの設定
	if( FAILED( m_pD3DDevice->CreateTexture(
		SCREEN_W,
		SCREEN_H,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTempTexture,
		NULL) ) )
		return E_FAIL;

	//↑のサーフェスのインターフェイスの作成
	if( FAILED( m_pTempTexture->GetSurfaceLevel(0,&m_pTempSurf) )  )
		return E_FAIL;

	//深度ステンシル サーフェイスを設定する。
	if( FAILED( m_pD3DDevice->CreateDepthStencilSurface(
		SCREEN_W,
		SCREEN_H,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pTempZ,
		NULL) ) )
		return E_FAIL;

	//-----------------------------------------
	//影テクスチャの設定
	//-----------------------------------------

	//シャドウテクスチャの設定
	if( FAILED( m_pD3DDevice->CreateTexture(
		MAP_SIZE,
		MAP_SIZE,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pDephTex,
		NULL) ) )
		return E_FAIL;

	//↑のサーフェスのインターフェイスの作成
	if( FAILED( m_pDephTex->GetSurfaceLevel(0,&m_pDephSurf) )  )
		return E_FAIL;

	//深度ステンシル サーフェイスを設定する。
	if( FAILED( m_pD3DDevice->CreateDepthStencilSurface(
		MAP_SIZE,
		MAP_SIZE,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pDephTexZ,
		NULL) ) )
		return E_FAIL;

	return S_OK;

}
//---------------------------------------------//
//影の読み込み
//---------------------------------------------//
VOID	ShadowProc::pStartReadShadow()
{
	//サンプリング領域
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//画面幅の取得
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	D3DVIEWPORT9 l_RenderViewport = { 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	//-------------------------------------------------
	// レンダリングターゲットの保存
	//-------------------------------------------------
	m_pD3DDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	m_pD3DDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	m_pD3DDevice->GetViewport(&m_oldViewport);

	//-------------------------------------------------
	// シャドウレンダリングバッファの初期化
	//-------------------------------------------------
	//描き込み先の指定
	m_pD3DDevice->SetRenderTarget(0,m_pRenderingSurf);
	//描き込み先の深度サーフェス
	m_pD3DDevice->SetDepthStencilSurface(m_pRenderingZ);
	//ビューポートの変更
	m_pD3DDevice->SetViewport(&l_RenderViewport);
	// バッファのクリア(黒)
	m_pD3DDevice->Clear( 0L, NULL
							, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
							, 0x00000000, 1.0f, 0L );

	//-------------------------------------------------
	// 一時バッファの初期化
	//-------------------------------------------------
	//描き込み先の指定
	m_pD3DDevice->SetRenderTarget(0,m_pTempSurf);
	//描き込み先の深度サーフェス
	m_pD3DDevice->SetDepthStencilSurface(m_pTempZ);
	//ビューポートの変更
	m_pD3DDevice->SetViewport(&l_RenderViewport);
	// バッファのクリア(黒)
	m_pD3DDevice->Clear( 0L, NULL
							, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
							, 0x00000000, 1.0f, 0L );

	//-------------------------------------------------
	// 
	//-------------------------------------------------

}
//---------------------------------------------//
//影の読み込みの終了
//---------------------------------------------//
VOID	ShadowProc::pEndReadShadow()
{

	//-------------------------------------------------
	// レンダリングターゲットを元に戻す
	//-------------------------------------------------
	m_pD3DDevice->SetRenderTarget(0, m_pOldBackBuffer);
	m_pD3DDevice->SetDepthStencilSurface(m_pOldZBuffer);
	m_pD3DDevice->SetViewport(&m_oldViewport);
	m_pOldBackBuffer->Release();
	m_pOldZBuffer->Release();

}
//---------------------------------------------//
//影用XFILE作成（複数）
//---------------------------------------------//
VOID ShadowProc::pRenderModel(int pass)
{
	for( int i= 0 ; i<(int)m_tvXfileInfo.size() ; i++ )
	{
		m_tvXfileInfo[i].anime->DrawModel( 
			D3DXVECTOR3( 
				m_tvXfileInfo[i].m_x,
				m_tvXfileInfo[i].m_y,
				m_tvXfileInfo[i].m_z),
			m_tvXfileInfo[i].m_r,pass);
	}

}
//---------------------------------------------//
//影描画開始
//---------------------------------------------//
VOID	ShadowProc::StartDrawShadow()
{

	//Xファイル用vectorの初期化
	m_tvXfileInfo.clear();

}
//---------------------------------------------//
//影描画終了
//---------------------------------------------//
VOID	ShadowProc::EndDrawShadow(int pass)
{
	//サンプリング領域
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//画面幅の取得
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	//デプスバッファ用
	D3DVIEWPORT9 l_viewport = { 0,0,MAP_SIZE,MAP_SIZE,0.0f,1.0f };
	//影レンダリング用
	D3DVIEWPORT9 l_RenderViewport = { 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	//各モードでの描画
	switch(pass)
	{

		case D3DTKYSHADOW_NORMAL:

			//---------影テクスチャの設定開始---------
			ShadowProc::pStartReadShadow();
			/*
			for(int j=0;j<m_pHLSL->GetLightNum();j++)
			{

				//光源の変更
				m_pHLSL->SetNowLight(j);
				*/
				m_pHLSL->SetNowLight(0);									//臨時
				//-------------------------------------------------
				// レンダリングターゲットの変更・・デプステクスチャ
				//-------------------------------------------------
				//描き込み先の指定
				m_pD3DDevice->SetRenderTarget(0,m_pDephSurf);
				//描き込み先の深度サーフェス
				m_pD3DDevice->SetDepthStencilSurface(m_pDephTexZ);
				//ビューポートの変更
				m_pD3DDevice->SetViewport(&l_viewport);


				//シャドウテクスチャのクリア
				m_pD3DDevice->Clear(
									0L,NULL,
									D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
									0xFFFFFFFF,1.0f,0L);

				//ここからXファイル描画を始める(深度バッファモード)
				pRenderModel(D3DTKY_DEPTH);
/*
				//デプステクスチャを設定する
				m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hShadowTex,m_pDephTex );	
				//影のみの設定
				m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hRenderTex,m_pTempTexture);

				//-------------------------------------------------
				// レンダリングターゲットの変更・・レンダリングテクスチャ
				//-------------------------------------------------
				//描き込み先の指定
				m_pD3DDevice->SetRenderTarget(0,m_pRenderingSurf);
				//描き込み先の深度サーフェス
				m_pD3DDevice->SetDepthStencilSurface(m_pRenderingZ);
				//ビューポートの変更
				m_pD3DDevice->SetViewport(&l_RenderViewport);

				// バッファのクリア
				m_pD3DDevice->Clear( 0L, NULL
									, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
									, 0x00000000, 1.0f, 0L );


				//ここからXファイル描画を始める(影だけ) 
				pRenderModel(D3DTKY_SHADOW_PLUS);

				//レンダリングの保存
				m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hRenderTex,m_pRenderingTexture);

				//-------------------------------------------------
				// レンダリングターゲットの変更・・一時的影専用
				//-------------------------------------------------
				//描き込み先の指定
				m_pD3DDevice->SetRenderTarget(0,m_pTempSurf);
				//描き込み先の深度サーフェス
				m_pD3DDevice->SetDepthStencilSurface(m_pTempZ);
				//ビューポートの変更
				m_pD3DDevice->SetViewport(&l_RenderViewport);

				// バッファのクリア
				m_pD3DDevice->Clear( 0L, NULL
									, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
									, 0x00000000, 1.0f, 0L );

				//ここからXファイル描画を始める(影だけ) 
				pRenderModel(D3DTKY_SAVE_SHADOW);

			}
*/
			//---------影テクスチャの設定終了---------
			ShadowProc::pEndReadShadow();

			// バッファのクリア
			m_pD3DDevice->Clear( 0L, NULL
								, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
								, 0x00000000, 1.0f, 0L );

			//デプステクスチャを設定する
			m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hShadowTex,m_pDephTex );					//臨時
			//影テクスチャを設定する
			m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hRenderTex,m_pRenderingTexture );

			//ここからXファイル描画を始める(シャドウモード) 
			pRenderModel(D3DTKY_SHADOW);

		break;

		case D3DTKYSHADOW_SPOT:
			//
			//今回はケげなしで行く
			//

/*			//---------影テクスチャの設定開始---------
			ShadowProc::pStartReadShadow();
			
			//光源の変更
			m_pHLSL->SetNowLight(0);

			//-------------------------------------------------
			// レンダリングターゲットの変更・・デプステクスチャ
			//-------------------------------------------------
			//描き込み先の指定
			m_pD3DDevice->SetRenderTarget(0,m_pDephSurf);
			//描き込み先の深度サーフェス
			m_pD3DDevice->SetDepthStencilSurface(m_pDephTexZ);
			//ビューポートの変更
			m_pD3DDevice->SetViewport(&l_viewport);

			//シャドウテクスチャのクリア
			m_pD3DDevice->Clear(
								0L,NULL,
								D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
								0xFFFFFFFF,1.0f,0L);

			//ここからXファイル描画を始める(深度バッファモード)
			pRenderModel(D3DTKY_DEPTH);

			//---------影テクスチャの設定終了---------
			ShadowProc::pEndReadShadow();
/*
			// バッファのクリア
			m_pD3DDevice->Clear( 0L, NULL
								, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
								, 0x00000000, 1.0f, 0L );
*/

			//影テクスチャを設定する
			m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hShadowTex,m_pDephTex );

			//ここからXファイル描画を始める(シャドウスポットモード) 
			pRenderModel(D3DTKY_SHADOW_SPOT);

		break;

	}

	//Xファイル用vectorの初期化
	m_tvXfileInfo.clear();

}
//---------------------------------------------//
//影用XFILE作成
//---------------------------------------------//
VOID	ShadowProc::SetShadowXfile(D3DTKYXANIMETION *anime,float x,float y,float z,float rad)
{

	//要素の設定
	XandPOS l_xpos = { anime,x,y,z,rad };

	//要素の追加
	m_tvXfileInfo.push_back(l_xpos);

}
//---------------------------------------------//
//デバック表示処理
//---------------------------------------------//
VOID	ShadowProc::DebugShadow(int pass)
{

		m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
		m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);
		float scale = 128.0f;
		TVERTEX Vertex[4] = {
			// x  y  z rhw tu tv 
			{    0,    0,0, 1, 0, 0,},
			{scale,    0,0, 1, 1, 0,},
			{scale,scale,0, 1, 1, 1,},
			{    0,scale,0, 1, 0, 1,},
		};
		switch(pass)
		{
		case D3DTKYSHADOW_NORMAL:
			m_pD3DDevice->SetTexture( 0, m_pDephTex);
			break;
		case D3DTKYSHADOW_SPOT:
			m_pD3DDevice->SetTexture( 0, m_pDephTex);
			break;
		}
		m_pD3DDevice->SetVertexShader(NULL);
		m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
		m_pD3DDevice->SetPixelShader(0);
		m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

}
//---------------------------------------------//
//開放処理
//---------------------------------------------//
VOID	ShadowProc::Close()
{

	SAFE_RELEASE(m_pDephSurf);
	SAFE_RELEASE(m_pDephTex);
	SAFE_RELEASE(m_pDephTexZ);

}