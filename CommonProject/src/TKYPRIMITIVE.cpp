#include "TKYPRIMITIVE.h"

LPTkyPrimitive CPrimitive::m_single = NULL;

	//色
	BOOL CPrimitive::SetColor(Vector c)
	{
		m_Color = D3DCOLOR_RGBA( (UINT)(0xff*c.x) , (UINT)(0xff*c.y) , (UINT)(0xff*c.z) , 0xff);
		return TRUE;
	}

	//線描画
	HRESULT CPrimitive::DrawLine(Vector start,Vector end , float thick)
	{
		//横幅の設定
		Vector Normal = ((end-start)*Vector(0,1,0)).Normal()*thick;

		CUSTOMVERTEX Vertex[3] = {
			// x							y								z								rhw	color
			{	start.x + Normal.x	,	start.y + Normal.y	,	start.z + Normal.z	,1.0f	,m_Color,},
			{	end.x						,	end.y						,	end.z						,1.0f	,m_Color,},
			{	start.x - Normal.x	,	start.y - Normal.y	,	start.z - Normal.z	,1.0f	,m_Color,},
		};

		//頂点バッファの値を格納する配列
		CUSTOMVERTEX pVertices[3];
		//頂点バッファの変更を可能にする
		if(FAILED( m_pVertexBuffer->Lock(
			0,
			0,
			(void**)&pVertices,
			0) ))return  E_FAIL;
		//値の格納
		memcpy_s( pVertices , sizeof(CUSTOMVERTEX) * 3 , Vertex , sizeof(CUSTOMVERTEX) * 3  );
		//頂点バッファの変更を不可能にする
		m_pVertexBuffer->Unlock();

		//描画する頂点バッファの指定
		this->m_device->SetStreamSource(0,m_pVertexBuffer,0,sizeof(CUSTOMVERTEX));
		//フレキシブル頂点フォーマットを指定
		this->m_device->SetFVF( D3DFVF_XYZW | D3DFVF_DIFFUSE);

		//実際に描画する
		this->m_device->DrawPrimitive(D3DPT_TRIANGLEFAN,0,1);

//		m_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, pVertices, sizeof( CUSTOMVERTEX ) );

		return S_OK;
	}

	//頂点データの初期化
	HRESULT CPrimitive::InitializeVertexBuffer()
	{
		//頂点配列の作成
		if(FAILED( this->m_device->CreateVertexBuffer(
			VERTEX_SIZE * sizeof(CUSTOMVERTEX),
			D3DUSAGE_WRITEONLY,
			D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT,
			&m_pVertexBuffer,
			NULL) ))return E_FAIL;

		return S_OK;
	}