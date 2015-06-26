#include "TKYPRIMITIVE.h"

LPTkyPrimitive CPrimitive::m_single = NULL;

	//�F
	BOOL CPrimitive::SetColor(Vector c)
	{
		m_Color = D3DCOLOR_RGBA( (UINT)(0xff*c.x) , (UINT)(0xff*c.y) , (UINT)(0xff*c.z) , 0xff);
		return TRUE;
	}

	//���`��
	HRESULT CPrimitive::DrawLine(Vector start,Vector end , float thick)
	{
		//�����̐ݒ�
		Vector Normal = ((end-start)*Vector(0,1,0)).Normal()*thick;

		CUSTOMVERTEX Vertex[3] = {
			// x							y								z								rhw	color
			{	start.x + Normal.x	,	start.y + Normal.y	,	start.z + Normal.z	,1.0f	,m_Color,},
			{	end.x						,	end.y						,	end.z						,1.0f	,m_Color,},
			{	start.x - Normal.x	,	start.y - Normal.y	,	start.z - Normal.z	,1.0f	,m_Color,},
		};

		//���_�o�b�t�@�̒l���i�[����z��
		CUSTOMVERTEX pVertices[3];
		//���_�o�b�t�@�̕ύX���\�ɂ���
		if(FAILED( m_pVertexBuffer->Lock(
			0,
			0,
			(void**)&pVertices,
			0) ))return  E_FAIL;
		//�l�̊i�[
		memcpy_s( pVertices , sizeof(CUSTOMVERTEX) * 3 , Vertex , sizeof(CUSTOMVERTEX) * 3  );
		//���_�o�b�t�@�̕ύX��s�\�ɂ���
		m_pVertexBuffer->Unlock();

		//�`�悷�钸�_�o�b�t�@�̎w��
		this->m_device->SetStreamSource(0,m_pVertexBuffer,0,sizeof(CUSTOMVERTEX));
		//�t���L�V�u�����_�t�H�[�}�b�g���w��
		this->m_device->SetFVF( D3DFVF_XYZW | D3DFVF_DIFFUSE);

		//���ۂɕ`�悷��
		this->m_device->DrawPrimitive(D3DPT_TRIANGLEFAN,0,1);

//		m_device->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 1, pVertices, sizeof( CUSTOMVERTEX ) );

		return S_OK;
	}

	//���_�f�[�^�̏�����
	HRESULT CPrimitive::InitializeVertexBuffer()
	{
		//���_�z��̍쐬
		if(FAILED( this->m_device->CreateVertexBuffer(
			VERTEX_SIZE * sizeof(CUSTOMVERTEX),
			D3DUSAGE_WRITEONLY,
			D3DFVF_CUSTOMVERTEX,
			D3DPOOL_DEFAULT,
			&m_pVertexBuffer,
			NULL) ))return E_FAIL;

		return S_OK;
	}