
#include "TKY3D.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		init3D関係
//
//////////////////////////////////////////////////////////////////////////////////////////
vector<Init3D*>  Init3D::m_vIndex;
LPTKY3D  Init3D::m_single3D;
//---------------------------------------------//
//DirectX 9.0の初期化
//---------------------------------------------//
HRESULT Init3D::Create3DX(HWND hWnd){

	HRESULT h ;


	if(NULL == (Init3D::g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))					//			
		return E_FAIL;

	D3DDISPLAYMODE dispmode;
	Init3D::g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dispmode);

	D3DPRESENT_PARAMETERS d3dpp;				//描画対象の属性設定構造体

	ZeroMemory( &d3dpp ,sizeof(d3dpp));

	switch(true){//Init3D::Window){

		case true:

			d3dpp.Windowed = true;													//ウィンドウモードtrue,フルウィンドウモードfalse
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//スワップ後のバックバッファの処理の指定
			d3dpp.BackBufferFormat = dispmode.Format;								//バックバッファの色深度(最大表示色数)を指定
			d3dpp.EnableAutoDepthStencil = TRUE;									//Zバッファの有無
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;								//Zバッファの形式

			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		break;

		case false:

			d3dpp.Windowed = false;													//ウィンドウモードtrue,フルウィンドウモードfalse
			d3dpp.BackBufferWidth = dispmode.Width;									//ウィンドウの幅
			d3dpp.BackBufferHeight = dispmode.Height;								//ウィンドウの高さ
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//スワップ後のバックバッファの処理の指定
			d3dpp.BackBufferFormat = dispmode.Format;								//バックバッファの色深度(最大表示色数)を指定
			d3dpp.FullScreen_RefreshRateInHz = dispmode.RefreshRate;				//フルスクリーン時のリフレッシュレート
			d3dpp.EnableAutoDepthStencil = TRUE;									//Zバッファの有無
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;								//Zバッファの形式

		break;

	}

	//引数が無効と言われているのでそれを改善する　2011/2/20/13/32<もやもや>


	HRESULT r;
	if( FAILED( r=Init3D::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,				//使用するディスプレイアダプタ(3Dビデオチップ)
									D3DDEVTYPE_HAL,								//デバイスのタイプ
									hWnd,										//描画対象となるウィンドウのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,		//デバイスのオプション設定(ハードウェアT&L)
									&d3dpp,										//パラメータ構造体のポインタ
									&(Init3D::g_pD3DDevice))))					//デバイスを返すポインタ
	{
		
		if( FAILED( r=Init3D::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			//使用するディスプレイアダプタ(3Dビデオチップ)
										D3DDEVTYPE_HAL,							//デバイスのタイプ
										hWnd,									//描画対象となるウィンドウのハンドル
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//デバイスのオプション設定(通常モード)
										&d3dpp,									//パラメータ構造体のポインタ
										&(Init3D::g_pD3DDevice))))				//デバイスを返すポインタ
		{
			return E_FAIL;

		}

	}

	Init3D::g_pD3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_NONE );			//カリングの有無の設定(今は無)
	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ZENABLE , TRUE );					//Zバッファの有無の設定(今は有)

	//プリミティブデータの設定
	m_primitive = CPrimitive::CreatePrimitive( g_pD3DDevice );
	IF( FAILED( h = m_primitive->InitializeVertexBuffer() ) )
	{
		return h;
	}

	m_Color = NULL;

	return S_OK;


}
//---------------------------------------------//
//DirectX 9.0の消去
//---------------------------------------------//
void	Init3D::clean3D()
{

	SAFE_RELEASE(Init3D::g_pD3D);
	SAFE_RELEASE(Init3D::g_pD3DDevice);

}
//---------------------------------------------//
//描画を始める
//---------------------------------------------//
HRESULT Init3D::StartDraw()
{

	//アルファブレンドの設定
	Init3D::g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	Init3D::g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
//	Init3D::g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHA);
	Init3D::g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
//	Init3D::g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);

	// アルファテストの有効化
	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	// 不透明にする値の設定
//	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ALPHAREF, 0x66 );
	
	//サンプラの読み込み
	Init3D::GetD3DDevice()->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
	Init3D::GetD3DDevice()->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
	Init3D::GetD3DDevice()->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );

	//テクスチャと頂点色の両方を反映される
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 , D3DTA_TEXTURE  );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP , D3DTOP_MODULATE );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2 , D3DTA_DIFFUSE  );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1 , D3DTA_TEXTURE  );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP , D3DTOP_MODULATE );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE  );


/*
	//バック画面の初期化()
	Init3D::g_pD3DDevice->Clear(0,								//↓の要素数。全体を消去する場合０
						NULL,									//消去する矩形の配列。全体を消去する場合NULL
						D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,		//消去対象を指定するフラグ
						D3DCOLOR_ARGB(255,0,0,0),				//レンダリングターゲットを消去するときに使う値
						1.0f,									//深度バッファを消去するときに使う値
						0);										//ステンシルバッファを消去するときに使う値
*/

	return Init3D::g_pD3DDevice->BeginScene();

}
//---------------------------------------------//
//描画終了
//---------------------------------------------//
void	Init3D::EndDraw()
{

	//描画の終了
	Init3D::g_pD3DDevice->EndScene();

	//画面の表示する
	Init3D::g_pD3DDevice->Present(NULL,NULL,NULL,NULL);

}
//---------------------------------------------//
//画面のクリア（設定可）
//---------------------------------------------//
void Init3D::Clear(int type,UINT color)
{
	//バック画面の初期化()
	Init3D::g_pD3DDevice->Clear(0,								//↓の要素数。全体を消去する場合０
						NULL,									//消去する矩形の配列。全体を消去する場合NULL
						type,											//消去対象を指定するフラグ
						color,				//レンダリングターゲットを消去するときに使う値
						1.0f,									//深度バッファを消去するときに使う値
						0);										//ステンシルバッファを消去するときに使う値

}
//---------------------------------------------//
//通常描画
//---------------------------------------------//
BOOL Init3D::SetColor(Vector c)
{
	m_Color = D3DCOLOR_RGBA( (UINT)(0xff*c.x) , (UINT)(0xff*c.y) , (UINT)(0xff*c.z) , 0xff);
	m_vColor = c;
	return TRUE;
}
BOOL Init3D::DrawLine(Vector start,Vector end , float thick)
{

	//横幅の設定
	Vector Normal = (end-start)*Vector(1,1,0);
	Normal = ((end-start)*Normal).Normal() * thick;

	CUSTOMVERTEX Vertex[3] = {
		// x							y								z								rhw	color
		{	start.x + Normal.x	,	start.y + Normal.y	,	start.z + Normal.z	,1.0f	,m_Color,},
		{	end.x						,	end.y						,	end.z						,1.0f	,m_Color,},
		{	start.x - Normal.x	,	start.y - Normal.y	,	start.z - Normal.z	,1.0f	,m_Color,},
	};

	//マップ初期化
	D3DXMATRIXA16 TransMat;						//座標変換行列	
	D3DXMatrixIdentity(&TransMat);									//行列の初期化
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//座標行列セット

	g_pD3DDevice->SetTexture( 0, NULL);
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_pD3DDevice->SetPixelShader(0);
	g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, Vertex, sizeof( CUSTOMVERTEX ) );

	return TRUE;

}
BOOL Init3D::DrawLine3D(Vector start,Vector end , float thick)
{
	//一時描画しない
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return true;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	//横幅の設定
	Vector Normal = ((end-start)*Vector(0,1,0)).Normal() *thick;
	Vector n = Vector(0,1,0);
/*
	CUSTOMVERTEX Vertex[3] = {
		// x							y								z								rhw	color
		{	start.x + Normal.x	,	start.y + Normal.y	,	start.z + Normal.z	,1.0f	,m_Color,},
		{	end.x						,	end.y						,	end.z						,1.0f	,m_Color,},
		{	start.x - Normal.x	,	start.y - Normal.y	,	start.z - Normal.z	,1.0f	,m_Color,},
	};
*/
	CUSTOMVERTEX3D Vertex[3] = {
		//pos  normal
		{ start + Normal , n  },
		{ end , n},
		{ start - Normal , n},
	};

	D3DMATERIAL9 mate;
	ZeroMemory(&mate,sizeof(D3DMATERIAL9));
	mate.Diffuse.r = m_vColor.x;
	mate.Diffuse.g = m_vColor.y;
	mate.Diffuse.b = m_vColor.z;
	mate.Diffuse.a = 1.0f;
	g_pD3DDevice->SetMaterial( &mate );

	//マップ初期化
	D3DXMATRIXA16 TransMat;						//座標変換行列	
	D3DXMatrixIdentity(&TransMat);									//行列の初期化
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//座標行列セット


	g_pD3DDevice->SetTexture( 0, NULL);
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX3D);
	g_pD3DDevice->SetPixelShader(0);
	g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, Vertex, sizeof( CUSTOMVERTEX3D ) );

	return TRUE;
}
BOOL Init3D::DrawSquare(Vector p1,Vector p2,Vector p3,Vector p4)
{
		Vector c = m_vColor;
		DWORD Color = D3DCOLOR_RGBA( (UINT)(0xff*c.x) , (UINT)(0xff*c.y) , (UINT)(0xff*c.z) , 0x55);

		CUSTOMVERTEX Vertex[4] = {
			// x				y					z		rhw		color
			{ p1.x,			p1.y,				0,		1,			Color	,	},
			{ p2.x,			p2.y,				0,		1,			Color	,	},
			{ p3.x,			p3.y,				0,		1,			Color	,	},
			{ p4.x,			p4.y,				0,		1,			Color	,	},
		};

	//マップ初期化
	D3DXMATRIXA16 TransMat;						//座標変換行列	
	D3DXMatrixIdentity(&TransMat);									//行列の初期化
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//座標行列セット


		g_pD3DDevice->SetTexture( 0, NULL);
		g_pD3DDevice->SetVertexShader(NULL);
		g_pD3DDevice->SetFVF( D3DFVF_CUSTOMVERTEX);
		g_pD3DDevice->SetPixelShader(0);
		g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( CUSTOMVERTEX ) );

	return TRUE;
}
BOOL Init3D::DrawCircle(Vector p1,float r,int k)
{
		if(k<5)k=5;

		CUSTOMVERTEX *Vertex = new CUSTOMVERTEX[k * 3];
		FOR_I(k)
		{
			int in = i*3;
			Vector v = Vector(r,0,0);
			Vertex[in+2].x = p1.x;
			Vertex[in+2].y = p1.y;
			Vertex[in+2].z = p1.z;
			v = v.Rotate(Vector(),0,0, 360.0f/k * i );
			Vertex[in+1].x = p1.x + v.x;
			Vertex[in+1].y = p1.y + v.y;
			Vertex[in+1].z = p1.z + v.z;
			v = v.Rotate(Vector(),0,0, 360.0f/k );
			Vertex[in].x = p1.x + v.x;
			Vertex[in].y = p1.y + v.y;
			Vertex[in].z = p1.z + v.z;
			Vertex[in].color = Vertex[in+1].color = Vertex[in+2].color = m_Color;
			Vertex[in].rhw = Vertex[in+1].rhw = Vertex[in+2].rhw = 1;
		}

	//マップ初期化
	D3DXMATRIXA16 TransMat;						//座標変換行列	
	D3DXMatrixIdentity(&TransMat);									//行列の初期化
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//座標行列セット


		g_pD3DDevice->SetTexture( 0, NULL);
		g_pD3DDevice->SetVertexShader(NULL);
		g_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		g_pD3DDevice->SetPixelShader(0);
		g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, k, Vertex, sizeof( CUSTOMVERTEX ) );

		SAFE_DELETE_ARRAY(Vertex);

		return TRUE;
}


/*
	//横幅の設定
	Vector Normal = (end-start)*Vector(0,0,1);
	Normal = ((end-start)*Normal).Normal() * thick;
	//一つの幅の決定
	CUSTOMVERTEX vertices[3] = {
		{	start.x + Normal.x ,	start.y ,	start.z + Normal.z	,1.0f,D3DCOLOR_RGBA(m_rgb[0],m_rgb[1],m_rgb[2],0xff),},
		{	end.x	 ,	end.y ,	end.z										,1.0f,D3DCOLOR_RGBA(m_rgb[0],m_rgb[1],m_rgb[2],0xff),},
		{	start.x - Normal.x ,	start.y ,	start.z - Normal.z	,1.0f,D3DCOLOR_RGBA(m_rgb[0],m_rgb[1],m_rgb[2],0xff),},
	};
	//描画の登録
	m_vertics.push_back(vertices[0]);
	m_vertics.push_back(vertices[1]);
	m_vertics.push_back(vertices[2]);

	return S_OK;
*/

/*
	//描画
	HRESULT priDRAW(vector<CUSTOMVERTEX> );
	int m_rgb[3];
*/

/*
	LPDIRECT3DVERTEXBUFFER9 g_pVB;

	//登録された可変配列から構造体の配列を動的に作成する
	CUSTOMVERTEX *vertices;
	VectorToScalar<CUSTOMVERTEX>(vertices,vertex);

	//頂点の数
	int i_num = vertex.size();

	//頂点バッファの作成
	if(FAILED( this->g_pD3DDevice->CreateVertexBuffer(
		i_num * sizeof(CUSTOMVERTEX),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&g_pVB,
		NULL) ))return E_FAIL;

	//頂点バッファに値を格納する
	VOID* pVertices;
	//頂点バッファの変更を可能にする
	if(FAILED( g_pVB->Lock(
		0,
		_msize(vertices),
		(void**)&pVertices,
		0) ))return  E_FAIL;
	//中身を入れる
	memcpy(pVertices,vertices,_msize(vertices));
	//頂点バッファの変更を不可能にする
	g_pVB->Unlock();

	//描画する頂点バッファの指定
	this->g_pD3DDevice->SetStreamSource(0,g_pVB,0,sizeof(CUSTOMVERTEX));
	//フレキシブル頂点フォーマットを指定
	this->g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	//実際に描画する
	this->g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,i_num);

	//取得した配列の初期化
	SAFE_DELETE_ARRAY( vertices );
	return S_OK;
*/