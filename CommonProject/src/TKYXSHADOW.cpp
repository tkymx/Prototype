
#include "TKYXSHADOW.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		�e�֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//�R���g���X�^
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
//�e�̍쐬
//---------------------------------------------//
HRESULT ShadowProc::InitShadow()
{
	//�T���v�����O�̈�
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//��ʕ��̎擾
	UINT SCREEN_W = (UINT)rect.right;
	UINT SCREEN_H = (UINT)rect.bottom;

	//-----------------------------------------
	//�����_�����O�e�N�X�`���̐ݒ�
	//-----------------------------------------

	//�V���h�E�e�N�X�`���̐ݒ�
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

	//���̃T�[�t�F�X�̃C���^�[�t�F�C�X�̍쐬
	if( FAILED( m_pRenderingTexture->GetSurfaceLevel(0,&m_pRenderingSurf) )  )
		return E_FAIL;

	//�[�x�X�e���V�� �T�[�t�F�C�X��ݒ肷��B
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
	//�ꎞ�e�N�X�`���̐ݒ�
	//-----------------------------------------
	//�V���h�E�e�N�X�`���̐ݒ�
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

	//���̃T�[�t�F�X�̃C���^�[�t�F�C�X�̍쐬
	if( FAILED( m_pTempTexture->GetSurfaceLevel(0,&m_pTempSurf) )  )
		return E_FAIL;

	//�[�x�X�e���V�� �T�[�t�F�C�X��ݒ肷��B
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
	//�e�e�N�X�`���̐ݒ�
	//-----------------------------------------

	//�V���h�E�e�N�X�`���̐ݒ�
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

	//���̃T�[�t�F�X�̃C���^�[�t�F�C�X�̍쐬
	if( FAILED( m_pDephTex->GetSurfaceLevel(0,&m_pDephSurf) )  )
		return E_FAIL;

	//�[�x�X�e���V�� �T�[�t�F�C�X��ݒ肷��B
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
//�e�̓ǂݍ���
//---------------------------------------------//
VOID	ShadowProc::pStartReadShadow()
{
	//�T���v�����O�̈�
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//��ʕ��̎擾
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	D3DVIEWPORT9 l_RenderViewport = { 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	//-------------------------------------------------
	// �����_�����O�^�[�Q�b�g�̕ۑ�
	//-------------------------------------------------
	m_pD3DDevice->GetRenderTarget(0, &m_pOldBackBuffer);
	m_pD3DDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	m_pD3DDevice->GetViewport(&m_oldViewport);

	//-------------------------------------------------
	// �V���h�E�����_�����O�o�b�t�@�̏�����
	//-------------------------------------------------
	//�`�����ݐ�̎w��
	m_pD3DDevice->SetRenderTarget(0,m_pRenderingSurf);
	//�`�����ݐ�̐[�x�T�[�t�F�X
	m_pD3DDevice->SetDepthStencilSurface(m_pRenderingZ);
	//�r���[�|�[�g�̕ύX
	m_pD3DDevice->SetViewport(&l_RenderViewport);
	// �o�b�t�@�̃N���A(��)
	m_pD3DDevice->Clear( 0L, NULL
							, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
							, 0x00000000, 1.0f, 0L );

	//-------------------------------------------------
	// �ꎞ�o�b�t�@�̏�����
	//-------------------------------------------------
	//�`�����ݐ�̎w��
	m_pD3DDevice->SetRenderTarget(0,m_pTempSurf);
	//�`�����ݐ�̐[�x�T�[�t�F�X
	m_pD3DDevice->SetDepthStencilSurface(m_pTempZ);
	//�r���[�|�[�g�̕ύX
	m_pD3DDevice->SetViewport(&l_RenderViewport);
	// �o�b�t�@�̃N���A(��)
	m_pD3DDevice->Clear( 0L, NULL
							, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
							, 0x00000000, 1.0f, 0L );

	//-------------------------------------------------
	// 
	//-------------------------------------------------

}
//---------------------------------------------//
//�e�̓ǂݍ��݂̏I��
//---------------------------------------------//
VOID	ShadowProc::pEndReadShadow()
{

	//-------------------------------------------------
	// �����_�����O�^�[�Q�b�g�����ɖ߂�
	//-------------------------------------------------
	m_pD3DDevice->SetRenderTarget(0, m_pOldBackBuffer);
	m_pD3DDevice->SetDepthStencilSurface(m_pOldZBuffer);
	m_pD3DDevice->SetViewport(&m_oldViewport);
	m_pOldBackBuffer->Release();
	m_pOldZBuffer->Release();

}
//---------------------------------------------//
//�e�pXFILE�쐬�i�����j
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
//�e�`��J�n
//---------------------------------------------//
VOID	ShadowProc::StartDrawShadow()
{

	//X�t�@�C���pvector�̏�����
	m_tvXfileInfo.clear();

}
//---------------------------------------------//
//�e�`��I��
//---------------------------------------------//
VOID	ShadowProc::EndDrawShadow(int pass)
{
	//�T���v�����O�̈�
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//��ʕ��̎擾
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	//�f�v�X�o�b�t�@�p
	D3DVIEWPORT9 l_viewport = { 0,0,MAP_SIZE,MAP_SIZE,0.0f,1.0f };
	//�e�����_�����O�p
	D3DVIEWPORT9 l_RenderViewport = { 0,0,SCREEN_W,SCREEN_H,0.0f,1.0f };

	//�e���[�h�ł̕`��
	switch(pass)
	{

		case D3DTKYSHADOW_NORMAL:

			//---------�e�e�N�X�`���̐ݒ�J�n---------
			ShadowProc::pStartReadShadow();
			/*
			for(int j=0;j<m_pHLSL->GetLightNum();j++)
			{

				//�����̕ύX
				m_pHLSL->SetNowLight(j);
				*/
				m_pHLSL->SetNowLight(0);									//�Վ�
				//-------------------------------------------------
				// �����_�����O�^�[�Q�b�g�̕ύX�E�E�f�v�X�e�N�X�`��
				//-------------------------------------------------
				//�`�����ݐ�̎w��
				m_pD3DDevice->SetRenderTarget(0,m_pDephSurf);
				//�`�����ݐ�̐[�x�T�[�t�F�X
				m_pD3DDevice->SetDepthStencilSurface(m_pDephTexZ);
				//�r���[�|�[�g�̕ύX
				m_pD3DDevice->SetViewport(&l_viewport);


				//�V���h�E�e�N�X�`���̃N���A
				m_pD3DDevice->Clear(
									0L,NULL,
									D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
									0xFFFFFFFF,1.0f,0L);

				//��������X�t�@�C���`����n�߂�(�[�x�o�b�t�@���[�h)
				pRenderModel(D3DTKY_DEPTH);
/*
				//�f�v�X�e�N�X�`����ݒ肷��
				m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hShadowTex,m_pDephTex );	
				//�e�݂̂̐ݒ�
				m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hRenderTex,m_pTempTexture);

				//-------------------------------------------------
				// �����_�����O�^�[�Q�b�g�̕ύX�E�E�����_�����O�e�N�X�`��
				//-------------------------------------------------
				//�`�����ݐ�̎w��
				m_pD3DDevice->SetRenderTarget(0,m_pRenderingSurf);
				//�`�����ݐ�̐[�x�T�[�t�F�X
				m_pD3DDevice->SetDepthStencilSurface(m_pRenderingZ);
				//�r���[�|�[�g�̕ύX
				m_pD3DDevice->SetViewport(&l_RenderViewport);

				// �o�b�t�@�̃N���A
				m_pD3DDevice->Clear( 0L, NULL
									, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
									, 0x00000000, 1.0f, 0L );


				//��������X�t�@�C���`����n�߂�(�e����) 
				pRenderModel(D3DTKY_SHADOW_PLUS);

				//�����_�����O�̕ۑ�
				m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hRenderTex,m_pRenderingTexture);

				//-------------------------------------------------
				// �����_�����O�^�[�Q�b�g�̕ύX�E�E�ꎞ�I�e��p
				//-------------------------------------------------
				//�`�����ݐ�̎w��
				m_pD3DDevice->SetRenderTarget(0,m_pTempSurf);
				//�`�����ݐ�̐[�x�T�[�t�F�X
				m_pD3DDevice->SetDepthStencilSurface(m_pTempZ);
				//�r���[�|�[�g�̕ύX
				m_pD3DDevice->SetViewport(&l_RenderViewport);

				// �o�b�t�@�̃N���A
				m_pD3DDevice->Clear( 0L, NULL
									, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
									, 0x00000000, 1.0f, 0L );

				//��������X�t�@�C���`����n�߂�(�e����) 
				pRenderModel(D3DTKY_SAVE_SHADOW);

			}
*/
			//---------�e�e�N�X�`���̐ݒ�I��---------
			ShadowProc::pEndReadShadow();

			// �o�b�t�@�̃N���A
			m_pD3DDevice->Clear( 0L, NULL
								, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
								, 0x00000000, 1.0f, 0L );

			//�f�v�X�e�N�X�`����ݒ肷��
			m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hShadowTex,m_pDephTex );					//�Վ�
			//�e�e�N�X�`����ݒ肷��
			m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hRenderTex,m_pRenderingTexture );

			//��������X�t�@�C���`����n�߂�(�V���h�E���[�h) 
			pRenderModel(D3DTKY_SHADOW);

		break;

		case D3DTKYSHADOW_SPOT:
			//
			//����̓P���Ȃ��ōs��
			//

/*			//---------�e�e�N�X�`���̐ݒ�J�n---------
			ShadowProc::pStartReadShadow();
			
			//�����̕ύX
			m_pHLSL->SetNowLight(0);

			//-------------------------------------------------
			// �����_�����O�^�[�Q�b�g�̕ύX�E�E�f�v�X�e�N�X�`��
			//-------------------------------------------------
			//�`�����ݐ�̎w��
			m_pD3DDevice->SetRenderTarget(0,m_pDephSurf);
			//�`�����ݐ�̐[�x�T�[�t�F�X
			m_pD3DDevice->SetDepthStencilSurface(m_pDephTexZ);
			//�r���[�|�[�g�̕ύX
			m_pD3DDevice->SetViewport(&l_viewport);

			//�V���h�E�e�N�X�`���̃N���A
			m_pD3DDevice->Clear(
								0L,NULL,
								D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
								0xFFFFFFFF,1.0f,0L);

			//��������X�t�@�C���`����n�߂�(�[�x�o�b�t�@���[�h)
			pRenderModel(D3DTKY_DEPTH);

			//---------�e�e�N�X�`���̐ݒ�I��---------
			ShadowProc::pEndReadShadow();
/*
			// �o�b�t�@�̃N���A
			m_pD3DDevice->Clear( 0L, NULL
								, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER
								, 0x00000000, 1.0f, 0L );
*/

			//�e�e�N�X�`����ݒ肷��
			m_pHLSL->m_pEffect->SetTexture( m_pHLSL->m_hShadowTex,m_pDephTex );

			//��������X�t�@�C���`����n�߂�(�V���h�E�X�|�b�g���[�h) 
			pRenderModel(D3DTKY_SHADOW_SPOT);

		break;

	}

	//X�t�@�C���pvector�̏�����
	m_tvXfileInfo.clear();

}
//---------------------------------------------//
//�e�pXFILE�쐬
//---------------------------------------------//
VOID	ShadowProc::SetShadowXfile(D3DTKYXANIMETION *anime,float x,float y,float z,float rad)
{

	//�v�f�̐ݒ�
	XandPOS l_xpos = { anime,x,y,z,rad };

	//�v�f�̒ǉ�
	m_tvXfileInfo.push_back(l_xpos);

}
//---------------------------------------------//
//�f�o�b�N�\������
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
//�J������
//---------------------------------------------//
VOID	ShadowProc::Close()
{

	SAFE_RELEASE(m_pDephSurf);
	SAFE_RELEASE(m_pDephTex);
	SAFE_RELEASE(m_pDephTexZ);

}