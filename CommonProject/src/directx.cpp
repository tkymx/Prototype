
#include "directx.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		�֗��֐�
//
//////////////////////////////////////////////////////////////////////////////////////////
VOID AllDestroy()
{
	ITkyObject::DestroyObject();
}
//////////////////////////////////////////////////////////////////////////////////////////
//
//		���_�o�b�t�@�֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//�R���g���X�^
//---------------------------------------------//
VertexBuffer::VertexBuffer()
{

	VertexBuffer::g_pVB = NULL;

}
//---------------------------------------------//
//����������
//---------------------------------------------//
HRESULT VertexBuffer::Initialize(LPDIRECT3DDEVICE9 pD3DDev)
{

	//���_�̐�
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

	//���_�o�b�t�@�̍쐬
	if(FAILED( pD3DDev->CreateVertexBuffer(
		i_num * sizeof(CUSTOMVERTEX),
		0,
		D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT,
		&g_pVB,
		NULL) ))return E_FAIL;

	//���_�o�b�t�@�ɒl���i�[����
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
//�`�揈��
//---------------------------------------------//
VOID	VertexBuffer::Update(LPDIRECT3DDEVICE9 pD3DDev)
{

	//�`�悷�钸�_�o�b�t�@�̎w��
	pD3DDev->SetStreamSource(0,g_pVB,0,sizeof(CUSTOMVERTEX));
	//�t���L�V�u�����_�t�H�[�}�b�g���w��
	pD3DDev->SetFVF(D3DFVF_CUSTOMVERTEX);
	//���ۂɕ`�悷��
	pD3DDev->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

}
//---------------------------------------------//
//�J������
//---------------------------------------------//
VOID	VertexBuffer::Close()
{

	//�J������
	SAFE_RELEASE(g_pVB);

}
//////////////////////////////////////////////////////////////////////////////////////////
//
//		���_�V�F�[�_�֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//�R���g���X�^
//---------------------------------------------//
VertexShader::VertexShader(LPDIRECT3DDEVICE9 pD3DDev)
{
	VertexShader::m_pVS = NULL;
	VertexShader::m_pVD = NULL;

	VertexShader::m_pD3DDev = pD3DDev;

}
//---------------------------------------------//
//���_�V�F�[�_�̍쐬
//---------------------------------------------//
HRESULT VertexShader::InitShader(LPCTSTR Filename)
{
	HRESULT hr;
	LPD3DXBUFFER pCode;

	//���_�錾�I�u�W�F�N�g�̍쐬
	D3DVERTEXELEMENT9 decl[] = {
		{
			0,						//�X�g���[���ԍ�(���ʔԍ�)
			0,						//�擪����̃I�t�Z�b�g�o�C�g��
			D3DDECLTYPE_FLOAT3,		//�f�[�^�^�C�v
			D3DDECLMETHOD_DEFAULT,	//�e�b�Z���[�V�����̕��@
			D3DDECLUSAGE_POSITION,	//�g�p�ړI
			0						//�����������̂ňႤ�f�[�^����ʂ������
		},
		D3DDECL_END()
	};

	//���_���̓ǂݍ���
	if( FAILED( hr = m_pD3DDev->CreateVertexDeclaration(decl,&m_pVD) ) )
	{
		return hr;
	}

	//�V�F�[�_�̓ǂݍ���
	if( FAILED( hr = D3DXAssembleShaderFromFile(
		Filename,		//�t�@�C�������w�肷�镶����ւ̃|�C���^
		NULL,			//���v���Z�b�T��`�ւ̃|�C���^
		NULL,			//�I�v�V�����̃C���^�[�t�F�C�X �|�C���^
		0,				//D3DXSHADER �ɂ���Ď��ʂ����R���p�C�� �I�v�V����
		&pCode,			//�쐬���ꂽ�V�F�[�_���܂ރo�b�t�@��Ԃ�
		NULL) ) )		//�R���p�C�����ɔ��������G���[����ьx���̃��X�g���܂ރo�b�t�@��Ԃ�
	{
		return hr;
	}
	
	//���_�V�F�[�_�̍쐬
	hr = m_pD3DDev->CreateVertexShader(
		(DWORD*)pCode->GetBufferPointer(),
		&m_pVS
		);

	pCode->Release();

	if(FAILED(hr))return hr;

	return S_OK;

}
//---------------------------------------------//
//���_�V�F�[�_�̒萔�ݒ�
//---------------------------------------------//
VOID	VertexShader::SetShader()
{
	//�T���v�����O�̈�
	RECT rect;
	this->m_pD3DDev->GetScissorRect(&rect);
	//��ʕ��̎擾
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	D3DXMATRIXA16 l_mat,l_matView,l_matProj;

	//�r���[�s��̐ݒ�
	D3DXVECTOR3 l_vEye	= D3DXVECTOR3( 0.0f , 0.0f , 1.0f );
	D3DXVECTOR3 l_vAt	= D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	D3DXVECTOR3 l_vUp	= D3DXVECTOR3( 0.0f , 1.0f , 0.0f );	
	D3DXMatrixLookAtRH(&l_matView,&l_vEye,&l_vAt,&l_vUp);

	//�ˉe�s��̍쐬�i�������Ă����ʂ̑傫���ݒ�j
	D3DXMatrixPerspectiveFovRH(&l_matProj,D3DXToRadian(60.0f),(float)SCREEN_W/(float)SCREEN_H,0.1f,100.0f);
	
	//���W�ϊ��̒萔���W�X�^�̐ݒ�
	D3DXMatrixMultiply(&l_mat,&l_matView,&l_matProj);	//h1=h2*h3;
	D3DXMatrixTranspose(&l_mat,&l_mat);					//�]�u�s��̍쐬
	m_pD3DDev->SetVertexShaderConstantF(0,(float*)&l_mat,4);

	//���_�F�̒萔���W�X�^�̐ݒ�
	float color[] = {1.0f,0.0f,0.0f,1.0f};	//�ԐF
	m_pD3DDev->SetVertexShaderConstantF(4,(float*)&color,1);

	//���_�V�F�[�_�̐ݒ�
	m_pD3DDev->SetVertexDeclaration(m_pVD);
	m_pD3DDev->SetVertexShader(m_pVS);

}
//---------------------------------------------//
//�J���錾
//---------------------------------------------//
VOID	VertexShader::Close()
{

	SAFE_RELEASE(m_pVD);
	SAFE_RELEASE(m_pVS);

}
