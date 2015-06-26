
#include "directx.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		便利関数
//
//////////////////////////////////////////////////////////////////////////////////////////
VOID AllDestroy()
{
	ITkyObject::DestroyObject();
}
//////////////////////////////////////////////////////////////////////////////////////////
//
//		頂点バッファ関係
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
VertexBuffer::VertexBuffer()
{

	VertexBuffer::g_pVB = NULL;

}
//---------------------------------------------//
//初期化処理
//---------------------------------------------//
HRESULT VertexBuffer::Initialize(LPDIRECT3DDEVICE9 pD3DDev)
{

	//頂点の数
	int i_num = 4;

	CUSTOMVERTEX vertices[] = {

		{	-1.0f,	-1.0f,	-1.0f,	1.0f,D3DCOLOR_RGBA(0xff,0x00,0x00,0xff),},
		{	1.0f,	-1.0f,	-1.0f,	1.0f,D3DCOLOR_RGBA(0x00,0xff,0x00,0xff),},
		{	-1.0f,	1.0f,	-1.0f,	1.0f,D3DCOLOR_RGBA(0x00,0x00,0xff,0xff),},
		{	1.0f,	1.0f,	-1.0f,	1.0f,D3DCOLOR_RGBA(0xff,0xff,0x00,0xff),},
/*
		{	100.0f,	100.0f,	1.0f,	1.0f,D3DCOLOR_RGBA(0xff,0x00,0x00,0xff),},
		{	200.0f,	100.0f,	1.0f,	1.0f,D3DCOLOR_RGBA(0x00,0xff,0x00,0xff),},
		{	100.0f,	200.0f,	1.0f,	1.0f,D3DCOLOR_RGBA(0x00,0x00,0xff,0xff),},
		{	200.0f,	200.0f,	1.0f,	1.0f,D3DCOLOR_RGBA(0xff,0xff,0x00,0xff),},
*/
	};

	//頂点バッファの作成
	if(FAILED( pD3DDev->CreateVertexBuffer(
		i_num * sizeof(CUSTOMVERTEX),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&g_pVB,
		NULL) ))return E_FAIL;

	//頂点バッファに値を格納する
	VOID* pVertices;
	if(FAILED( g_pVB->Lock(
		0,
		sizeof(vertices),
		(void**)&pVertices,
		0) ))return  E_FAIL;
	memcpy(pVertices,vertices,sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;

}
//---------------------------------------------//
//描画処理
//---------------------------------------------//
VOID	VertexBuffer::Update(LPDIRECT3DDEVICE9 pD3DDev)
{

	//描画する頂点バッファの指定
	pD3DDev->SetStreamSource(0,g_pVB,0,sizeof(CUSTOMVERTEX));
	//フレキシブル頂点フォーマットを指定
	pD3DDev->SetFVF(D3DFVF_CUSTOMVERTEX);
	//実際に描画する
	pD3DDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}
//---------------------------------------------//
//開放処理
//---------------------------------------------//
VOID	VertexBuffer::Close()
{

	//開放処理
	SAFE_RELEASE(g_pVB);

}
//////////////////////////////////////////////////////////////////////////////////////////
//
//		頂点シェーダ関係
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
VertexShader::VertexShader(LPDIRECT3DDEVICE9 pD3DDev)
{
	VertexShader::m_pVS = NULL;
	VertexShader::m_pVD = NULL;

	VertexShader::m_pD3DDev = pD3DDev;

}
//---------------------------------------------//
//頂点シェーダの作成
//---------------------------------------------//
HRESULT VertexShader::InitShader(LPCTSTR Filename)
{
	HRESULT hr;
	LPD3DXBUFFER pCode;

	//頂点宣言オブジェクトの作成
	D3DVERTEXELEMENT9 decl[] = {
		{
			0,						//ストリーム番号(識別番号)
			0,						//先頭からのオフセットバイト数
			D3DDECLTYPE_FLOAT3,		//データタイプ
			D3DDECLMETHOD_DEFAULT,	//テッセレーションの方法
			D3DDECLUSAGE_POSITION,	//使用目的
			0						//↑が同じもので違うデータを区別するもの
		},
		D3DDECL_END()
	};

	//頂点情報の読み込み
	if( FAILED( hr = m_pD3DDev->CreateVertexDeclaration(decl,&m_pVD) ) )
	{
		return hr;
	}

	//シェーダの読み込み
	if( FAILED( hr = D3DXAssembleShaderFromFile(
		Filename,		//ファイル名を指定する文字列へのポインタ
		NULL,			//リプロセッサ定義へのポインタ
		NULL,			//オプションのインターフェイス ポインタ
		0,				//D3DXSHADER によって識別されるコンパイル オプション
		&pCode,			//作成されたシェーダを含むバッファを返す
		NULL) ) )		//コンパイル中に発生したエラーおよび警告のリストを含むバッファを返す
	{
		return hr;
	}
	
	//頂点シェーダの作成
	hr = m_pD3DDev->CreateVertexShader(
		(DWORD*)pCode->GetBufferPointer(),
		&m_pVS
		);

	pCode->Release();

	if(FAILED(hr))return hr;

	return S_OK;

}
//---------------------------------------------//
//頂点シェーダの定数設定
//---------------------------------------------//
VOID	VertexShader::SetShader()
{
	//サンプリング領域
	RECT rect;
	this->m_pD3DDev->GetScissorRect(&rect);
	//画面幅の取得
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	D3DXMATRIXA16 l_mat,l_matView,l_matProj;

	//ビュー行列の設定
	D3DXVECTOR3 l_vEye	= D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	D3DXVECTOR3 l_vAt	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 l_vUp	= D3DXVECTOR3( 0.0f , 1.0f , 0.0f );	
	D3DXMatrixLookAtRH(&l_matView,&l_vEye,&l_vAt,&l_vUp);

	//射影行列の作成（今見えている画面の大きさ設定）
	D3DXMatrixPerspectiveFovRH(&l_matProj,D3DXToRadian(60.0f),(float)SCREEN_W/(float)SCREEN_H,0.1f,100.0f);
	
	//座標変換の定数レジスタの設定
	D3DXMatrixMultiply(&l_mat,&l_matView,&l_matProj);	//h1=h2*h3;
	D3DXMatrixTranspose(&l_mat,&l_mat);					//転置行列の作成
	m_pD3DDev->SetVertexShaderConstantF(0,(float*)&l_mat,4);

	//頂点色の定数レジスタの設定
	float color[] = {1.0f,0.0f,0.0f,1.0f};	//赤色
	m_pD3DDev->SetVertexShaderConstantF(4,(float*)&color,1);

	//頂点シェーダの設定
	m_pD3DDev->SetVertexDeclaration(m_pVD);
	m_pD3DDev->SetVertexShader(m_pVS);

}
//---------------------------------------------//
//開放宣言
//---------------------------------------------//
VOID	VertexShader::Close()
{

	SAFE_RELEASE(m_pVD);
	SAFE_RELEASE(m_pVS);

}
