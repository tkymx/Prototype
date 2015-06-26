#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"


//���_�z��
#define VERTEX_SIZE 100

//--------------------------------------------------//
//��`�̈�
//--------------------------------------------------//
//���_�o�b�t�@�[�֌W
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
typedef struct{

	FLOAT x,y,z,rhw;	//�X�N���[�����W�ł̈ʒu
	DWORD color;		//���_�F

}CUSTOMVERTEX;

#define D3DFVF_CUSTOMVERTEX3DTEXTURE (D3DFVF_XYZ|D3DFVF_NORMAL | D3DFVF_TEX1)
typedef struct{

	D3DXVECTOR3 pos;	//�X�N���[�����W�ł̈ʒu
	D3DXVECTOR3 normal;//�@��
	FLOAT Uvec,Vvec;

}CUSTOMVERTEX3DTEXTURE;

#define D3DFVF_CUSTOMVERTEX3D (D3DFVF_XYZ|D3DFVF_NORMAL )
typedef struct{

	D3DXVECTOR3 pos;	//�X�N���[�����W�ł̈ʒu
	D3DXVECTOR3 normal;//�@��


}CUSTOMVERTEX3D;


//------------------------------------------------
//���_���̕`��Ɏg��(�V���O���g��)
//------------------------------------------------


CLASS_POINTA_OBJECT( CPrimitive , LPTkyPrimitive )
{
private:

	//�V���O���g���p
	STATIC LPTkyPrimitive m_single;

	//���_�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	//�f�o�C�X���
	LPDIRECT3DDEVICE9			 m_device;

	CPrimitive(LPDIRECT3DDEVICE9 device)
	{
		m_device = device;
		m_pVertexBuffer = NULL;
	}
	CPrimitive(const CPrimitive&)
	{
	}
	~CPrimitive()
	{
		SAFE_RELEASE( m_pVertexBuffer );
	}

	//�F�f�[�^
	DWORD m_Color;

public:

	//�F
	BOOL SetColor(Vector c);
	BOOL SetColor(DWORD c){m_Color=c;return TRUE;};
	//���`��
	HRESULT DrawLine(Vector start,Vector end , float thick);
	//���_�f�[�^�̏�����
	HRESULT InitializeVertexBuffer();

	//����
	STATIC LPTkyPrimitive CreatePrimitive( LPDIRECT3DDEVICE9 device )
	{
		IF(m_single == NULL)m_single = new CPrimitive(device);
		return m_single;
	}

};
