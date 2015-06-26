#pragma once

#include<d3dx9.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include "TKYHLSL.h"
#include "TKY3D.h"
#include "TKY2DTEXTURE.h"


//�V���h�E�p�X
enum ShadowPass{
	D3DTKYSHADOW_NORMAL ,
	D3DTKYSHADOW_SPOT
};

//-------------------------------------------------------------
// �e�쐬����XFILE�̃��[�J�����W���p�̍\����
//-------------------------------------------------------------
typedef struct {
	D3DTKYXANIMETION	*anime;
    FLOAT       m_x,m_y,m_z,m_r;
} XandPOS;

//--------------------------------------------------//
//�e�̍쐬
//--------------------------------------------------//
class ShadowProc
{

private:
	
	//�����_�����O�e�N�X�`��
	LPDIRECT3DTEXTURE9			m_pRenderingTexture;	//�ŏI�I�ȃ����_�����O�e�N�X�`��
	LPDIRECT3DSURFACE9			m_pRenderingSurf;		//���̃T�[�t�F�X�̃C���^�[�t�F�C�X
	LPDIRECT3DSURFACE9			m_pRenderingZ;			//�[�x�X�e���V�� �T�[�t�F�C�X��ݒ肷��B

	//�ꎞ�p�̃e�N�X�`��
	LPDIRECT3DTEXTURE9			m_pTempTexture;			//�ŏI�I�ȃ����_�����O�e�N�X�`��
	LPDIRECT3DSURFACE9			m_pTempSurf;			//���̃T�[�t�F�X�̃C���^�[�t�F�C�X
	LPDIRECT3DSURFACE9			m_pTempZ;				//�[�x�X�e���V�� �T�[�t�F�C�X��ݒ肷��B

	//�e�̃e�N�X�`���i�e�̕������z������j
	LPDIRECT3DTEXTURE9			m_pDephTex; 			//�e�̃e�N�X�`��
	LPDIRECT3DSURFACE9			m_pDephSurf;			//���̃T�[�t�F�X�̃C���^�[�t�F�C�X
	LPDIRECT3DSURFACE9			m_pDephTexZ;			//�[�x�X�e���V�� �T�[�t�F�C�X��ݒ肷��B

	//�O���̕ϐ����g��
	LPDIRECT3DDEVICE9			m_pD3DDevice;			//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X
	HLSLShader					*m_pHLSL;				//HLSL�̐ݒ�

	vector<XandPOS>				m_tvXfileInfo;			//�e�쐬����XFILE���i�ϔz��j

	//�V���h�E�쐬�ȑO�̃����_�����O�^�[�Q�b�g�̕ۑ������o�ϐ�
	LPDIRECT3DSURFACE9			m_pOldBackBuffer;
	LPDIRECT3DSURFACE9			m_pOldZBuffer;
	D3DVIEWPORT9				m_oldViewport;

	//�����_�����O�̕ۑ�
	VOID pStartReadShadow();

	//�����_�����O�̍ĊJ
	VOID pEndReadShadow();

	//���f���̕`��
	VOID pRenderModel(int pass);

public:

	//�R���X�g���N�^
	ShadowProc(LPTKY3D a_pinit3d,HLSLShader *a_pHLSL);

	//�e�̍쐬
	HRESULT InitShadow();

	//�e��X�t�@�C���̕`����n�߂�
	VOID StartDrawShadow();

	//�e��X�t�@�C���̕`����I������
	VOID EndDrawShadow(int pass);

	//�e�쐬�p��XFIE�ݒ�
	VOID SetShadowXfile(D3DTKYXANIMETION  *anime,float x,float y,float z,float rad);

	//�V���h�E�e�N�X�`���̃f�o�b�N�\��
	VOID DebugShadow(int pass);

	//�J������
	VOID Close();

};