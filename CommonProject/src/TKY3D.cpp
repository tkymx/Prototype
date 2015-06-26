
#include "TKY3D.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		init3D�֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
vector<Init3D*>  Init3D::m_vIndex;
LPTKY3D  Init3D::m_single3D;
//---------------------------------------------//
//DirectX 9.0�̏�����
//---------------------------------------------//
HRESULT Init3D::Create3DX(HWND hWnd){

	HRESULT h ;


	if(NULL == (Init3D::g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))					//			
		return E_FAIL;

	D3DDISPLAYMODE dispmode;
	Init3D::g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dispmode);

	D3DPRESENT_PARAMETERS d3dpp;				//�`��Ώۂ̑����ݒ�\����

	ZeroMemory( &d3dpp ,sizeof(d3dpp));

	switch(true){//Init3D::Window){

		case true:

			d3dpp.Windowed = true;													//�E�B���h�E���[�htrue,�t���E�B���h�E���[�hfalse
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//�X���b�v��̃o�b�N�o�b�t�@�̏����̎w��
			d3dpp.BackBufferFormat = dispmode.Format;								//�o�b�N�o�b�t�@�̐F�[�x(�ő�\���F��)���w��
			d3dpp.EnableAutoDepthStencil = TRUE;									//Z�o�b�t�@�̗L��
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;								//Z�o�b�t�@�̌`��

			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		break;

		case false:

			d3dpp.Windowed = false;													//�E�B���h�E���[�htrue,�t���E�B���h�E���[�hfalse
			d3dpp.BackBufferWidth = dispmode.Width;									//�E�B���h�E�̕�
			d3dpp.BackBufferHeight = dispmode.Height;								//�E�B���h�E�̍���
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//�X���b�v��̃o�b�N�o�b�t�@�̏����̎w��
			d3dpp.BackBufferFormat = dispmode.Format;								//�o�b�N�o�b�t�@�̐F�[�x(�ő�\���F��)���w��
			d3dpp.FullScreen_RefreshRateInHz = dispmode.RefreshRate;				//�t���X�N���[�����̃��t���b�V�����[�g
			d3dpp.EnableAutoDepthStencil = TRUE;									//Z�o�b�t�@�̗L��
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;								//Z�o�b�t�@�̌`��

		break;

	}

	//�����������ƌ����Ă���̂ł�������P����@2011/2/20/13/32<�������>


	HRESULT r;
	if( FAILED( r=Init3D::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,				//�g�p����f�B�X�v���C�A�_�v�^(3D�r�f�I�`�b�v)
									D3DDEVTYPE_HAL,								//�f�o�C�X�̃^�C�v
									hWnd,										//�`��ΏۂƂȂ�E�B���h�E�̃n���h��
									D3DCREATE_HARDWARE_VERTEXPROCESSING,		//�f�o�C�X�̃I�v�V�����ݒ�(�n�[�h�E�F�AT&L)
									&d3dpp,										//�p�����[�^�\���̂̃|�C���^
									&(Init3D::g_pD3DDevice))))					//�f�o�C�X��Ԃ��|�C���^
	{
		
		if( FAILED( r=Init3D::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,			//�g�p����f�B�X�v���C�A�_�v�^(3D�r�f�I�`�b�v)
										D3DDEVTYPE_HAL,							//�f�o�C�X�̃^�C�v
										hWnd,									//�`��ΏۂƂȂ�E�B���h�E�̃n���h��
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,	//�f�o�C�X�̃I�v�V�����ݒ�(�ʏ탂�[�h)
										&d3dpp,									//�p�����[�^�\���̂̃|�C���^
										&(Init3D::g_pD3DDevice))))				//�f�o�C�X��Ԃ��|�C���^
		{
			return E_FAIL;

		}

	}

	Init3D::g_pD3DDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_NONE );			//�J�����O�̗L���̐ݒ�(���͖�)
	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ZENABLE , TRUE );					//Z�o�b�t�@�̗L���̐ݒ�(���͗L)

	//�v���~�e�B�u�f�[�^�̐ݒ�
	m_primitive = CPrimitive::CreatePrimitive( g_pD3DDevice );
	IF( FAILED( h = m_primitive->InitializeVertexBuffer() ) )
	{
		return h;
	}

	m_Color = NULL;

	return S_OK;


}
//---------------------------------------------//
//DirectX 9.0�̏���
//---------------------------------------------//
void	Init3D::clean3D()
{

	SAFE_RELEASE(Init3D::g_pD3D);
	SAFE_RELEASE(Init3D::g_pD3DDevice);

}
//---------------------------------------------//
//�`����n�߂�
//---------------------------------------------//
HRESULT Init3D::StartDraw()
{

	//�A���t�@�u�����h�̐ݒ�
	Init3D::g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	Init3D::g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
//	Init3D::g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_SRCALPHA);
	Init3D::g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
//	Init3D::g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�̗L����
	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	// �s�����ɂ���l�̐ݒ�
//	Init3D::g_pD3DDevice->SetRenderState( D3DRS_ALPHAREF, 0x66 );
	
	//�T���v���̓ǂݍ���
	Init3D::GetD3DDevice()->SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );
	Init3D::GetD3DDevice()->SetSamplerState( 0 , D3DSAMP_MIPFILTER , D3DTEXF_LINEAR );
	Init3D::GetD3DDevice()->SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );

	//�e�N�X�`���ƒ��_�F�̗����𔽉f�����
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1 , D3DTA_TEXTURE  );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP , D3DTOP_MODULATE );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2 , D3DTA_DIFFUSE  );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1 , D3DTA_TEXTURE  );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP , D3DTOP_MODULATE );
	Init3D::g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE  );


/*
	//�o�b�N��ʂ̏�����()
	Init3D::g_pD3DDevice->Clear(0,								//���̗v�f���B�S�̂���������ꍇ�O
						NULL,									//���������`�̔z��B�S�̂���������ꍇNULL
						D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,		//�����Ώۂ��w�肷��t���O
						D3DCOLOR_ARGB(255,0,0,0),				//�����_�����O�^�[�Q�b�g����������Ƃ��Ɏg���l
						1.0f,									//�[�x�o�b�t�@����������Ƃ��Ɏg���l
						0);										//�X�e���V���o�b�t�@����������Ƃ��Ɏg���l
*/

	return Init3D::g_pD3DDevice->BeginScene();

}
//---------------------------------------------//
//�`��I��
//---------------------------------------------//
void	Init3D::EndDraw()
{

	//�`��̏I��
	Init3D::g_pD3DDevice->EndScene();

	//��ʂ̕\������
	Init3D::g_pD3DDevice->Present(NULL,NULL,NULL,NULL);

}
//---------------------------------------------//
//��ʂ̃N���A�i�ݒ�j
//---------------------------------------------//
void Init3D::Clear(int type,UINT color)
{
	//�o�b�N��ʂ̏�����()
	Init3D::g_pD3DDevice->Clear(0,								//���̗v�f���B�S�̂���������ꍇ�O
						NULL,									//���������`�̔z��B�S�̂���������ꍇNULL
						type,											//�����Ώۂ��w�肷��t���O
						color,				//�����_�����O�^�[�Q�b�g����������Ƃ��Ɏg���l
						1.0f,									//�[�x�o�b�t�@����������Ƃ��Ɏg���l
						0);										//�X�e���V���o�b�t�@����������Ƃ��Ɏg���l

}
//---------------------------------------------//
//�ʏ�`��
//---------------------------------------------//
BOOL Init3D::SetColor(Vector c)
{
	m_Color = D3DCOLOR_RGBA( (UINT)(0xff*c.x) , (UINT)(0xff*c.y) , (UINT)(0xff*c.z) , 0xff);
	m_vColor = c;
	return TRUE;
}
BOOL Init3D::DrawLine(Vector start,Vector end , float thick)
{

	//�����̐ݒ�
	Vector Normal = (end-start)*Vector(1,1,0);
	Normal = ((end-start)*Normal).Normal() * thick;

	CUSTOMVERTEX Vertex[3] = {
		// x							y								z								rhw	color
		{	start.x + Normal.x	,	start.y + Normal.y	,	start.z + Normal.z	,1.0f	,m_Color,},
		{	end.x						,	end.y						,	end.z						,1.0f	,m_Color,},
		{	start.x - Normal.x	,	start.y - Normal.y	,	start.z - Normal.z	,1.0f	,m_Color,},
	};

	//�}�b�v������
	D3DXMATRIXA16 TransMat;						//���W�ϊ��s��	
	D3DXMatrixIdentity(&TransMat);									//�s��̏�����
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//���W�s��Z�b�g

	g_pD3DDevice->SetTexture( 0, NULL);
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_pD3DDevice->SetPixelShader(0);
	g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, Vertex, sizeof( CUSTOMVERTEX ) );

	return TRUE;

}
BOOL Init3D::DrawLine3D(Vector start,Vector end , float thick)
{
	//�ꎞ�`�悵�Ȃ�
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	return true;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	//�����̐ݒ�
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

	//�}�b�v������
	D3DXMATRIXA16 TransMat;						//���W�ϊ��s��	
	D3DXMatrixIdentity(&TransMat);									//�s��̏�����
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//���W�s��Z�b�g


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

	//�}�b�v������
	D3DXMATRIXA16 TransMat;						//���W�ϊ��s��	
	D3DXMatrixIdentity(&TransMat);									//�s��̏�����
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//���W�s��Z�b�g


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

	//�}�b�v������
	D3DXMATRIXA16 TransMat;						//���W�ϊ��s��	
	D3DXMatrixIdentity(&TransMat);									//�s��̏�����
	m_single3D->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//���W�s��Z�b�g


		g_pD3DDevice->SetTexture( 0, NULL);
		g_pD3DDevice->SetVertexShader(NULL);
		g_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		g_pD3DDevice->SetPixelShader(0);
		g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, k, Vertex, sizeof( CUSTOMVERTEX ) );

		SAFE_DELETE_ARRAY(Vertex);

		return TRUE;
}


/*
	//�����̐ݒ�
	Vector Normal = (end-start)*Vector(0,0,1);
	Normal = ((end-start)*Normal).Normal() * thick;
	//��̕��̌���
	CUSTOMVERTEX vertices[3] = {
		{	start.x + Normal.x ,	start.y ,	start.z + Normal.z	,1.0f,D3DCOLOR_RGBA(m_rgb[0],m_rgb[1],m_rgb[2],0xff),},
		{	end.x	 ,	end.y ,	end.z										,1.0f,D3DCOLOR_RGBA(m_rgb[0],m_rgb[1],m_rgb[2],0xff),},
		{	start.x - Normal.x ,	start.y ,	start.z - Normal.z	,1.0f,D3DCOLOR_RGBA(m_rgb[0],m_rgb[1],m_rgb[2],0xff),},
	};
	//�`��̓o�^
	m_vertics.push_back(vertices[0]);
	m_vertics.push_back(vertices[1]);
	m_vertics.push_back(vertices[2]);

	return S_OK;
*/

/*
	//�`��
	HRESULT priDRAW(vector<CUSTOMVERTEX> );
	int m_rgb[3];
*/

/*
	LPDIRECT3DVERTEXBUFFER9 g_pVB;

	//�o�^���ꂽ�ϔz�񂩂�\���̂̔z��𓮓I�ɍ쐬����
	CUSTOMVERTEX *vertices;
	VectorToScalar<CUSTOMVERTEX>(vertices,vertex);

	//���_�̐�
	int i_num = vertex.size();

	//���_�o�b�t�@�̍쐬
	if(FAILED( this->g_pD3DDevice->CreateVertexBuffer(
		i_num * sizeof(CUSTOMVERTEX),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&g_pVB,
		NULL) ))return E_FAIL;

	//���_�o�b�t�@�ɒl���i�[����
	VOID* pVertices;
	//���_�o�b�t�@�̕ύX���\�ɂ���
	if(FAILED( g_pVB->Lock(
		0,
		_msize(vertices),
		(void**)&pVertices,
		0) ))return  E_FAIL;
	//���g������
	memcpy(pVertices,vertices,_msize(vertices));
	//���_�o�b�t�@�̕ύX��s�\�ɂ���
	g_pVB->Unlock();

	//�`�悷�钸�_�o�b�t�@�̎w��
	this->g_pD3DDevice->SetStreamSource(0,g_pVB,0,sizeof(CUSTOMVERTEX));
	//�t���L�V�u�����_�t�H�[�}�b�g���w��
	this->g_pD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	//���ۂɕ`�悷��
	this->g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,i_num);

	//�擾�����z��̏�����
	SAFE_DELETE_ARRAY( vertices );
	return S_OK;
*/