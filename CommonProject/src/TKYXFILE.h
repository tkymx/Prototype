#pragma once

#include<d3dx9.h>
#include<vector>
#include <mmsystem.h>
#include <assert.h>
#include "TKYBASEHEADER.h"
#include "TKYHLSL.h"
#include "TKY3D.h"
#include "Anime.h"

static const UINT MAP_SIZE = 2048;
#define _FOR_I(x) for( UINT i=0;i<x;i++ )


//�����_�����O�p�X�i�e�N�j�b�N�j
enum Rendering{
	D3DTKY_TEXTURE		,
	D3DTKY_WHITE		,
	D3DTKY_DEPTH		,
	D3DTKY_SHADOW		,
	D3DTKY_SAVE_SHADOW	,
	D3DTKY_SHADOW_PLUS	,
	D3DTKY_SHADOW_SPOT	,
	D3DTKY_ONLY_TEXTURE	
};


//========== �V�������e ==========

CLASS_POINTA_OBJECT( CXFile , LPTkyXFile )
{
private:
	//�f�o�C�X���
	LPTKY3D m_device;


	//X�t�@�C���ǂݍ��݃C���^�[�t�F�C�X
//	LPD3DXMESH				m_pMesh;				//���b�V���f�[�^���i�[����
	D3DMATERIAL9			*m_pMeshMaterials;		//�}�e���A�����i�[����
	LPDIRECT3DTEXTURE9		*m_pMeshTextures;		//�e�N�X�`�����i�[����
	DWORD					m_dwNumMaterials;		//�}�e���A���̐����i�[����

	//X�t�@�C���ǂݍ��݃C���^�[�t�F�C�X
	LPD3DXMESH				m_pMesh;				//���b�V���f�[�^���i�[����
	AnimetionClass			anime;					//�A�j���[�V�����N���X

	//�R���X�g���N�^
	CXFile(LPTKY3D d);
	CXFile(const CXFile&);
	VIRTUAL ~CXFile();
public:

	//X�t�@�C���̐ݒ�
	HRESULT SetXFileW(wstring name);
	//�\��
	HRESULT DrawMesh( Vector pos , Vector rotate );
	
	//�t���[���̐ݒ�
	VOID SetAnimeFreem(FLOAT freem);

	//���C�̂����蔻��
	BOOL CollisionLay( Vector laypos, Vector layvec , FLOAT* length , Vector xp = Vector() , Vector xr = Vector()  );

	//�ݒ�
	VOID SetDevice(LPTKY3D d){m_device=d;}
	VOID SetMesh(LPD3DXMESH x){m_pMesh=x;}
	//�擾
	LPTKY3D						GetDevice()const{return m_device;}
	LPD3DXMESH					GetMesh()const{return m_pMesh;}
	D3DMATERIAL9*				GetMaterial()const{return m_pMeshMaterials;}				//�}�e���A�����i�[����
	LPDIRECT3DTEXTURE9*	GetTexture()const{return m_pMeshTextures;}				//�e�N�X�`�����i�[����
	DWORD							GetMaterialNum()const{return m_dwNumMaterials;}	//�}�e���A���̐����i�[����
	AnimetionClass*				GetAnime(){return &anime;}								//�A�j���[�V�����N���X

	//����
	STATIC LPTkyXFile CreateXFile(LPTKY3D d);
	STATIC LPTkyXFile CreateXFile(LPTKY3D d,wstring name);

};


//========== �Â����e ==========

//--------------------------------------------------//
//X�t�@�C����ǂݍ���
//--------------------------------------------------//
class XfileLoad{

private:

	LPDIRECT3D9				m_pD3D;					//DirectX9�@�C���^�[�t�F�C�X
	LPDIRECT3DDEVICE9	m_pD3DDevice;		//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X
	HLSLShader				*m_pHLSL;				//HLSL���

	//X�t�@�C���ǂݍ��݃C���^�[�t�F�C�X
	LPD3DXMESH				m_pMesh;				//���b�V���f�[�^���i�[����
	AnimetionClass			anime;					//�A�j���[�V�����N���X

	//�v���C�x�[�g�֐�
	void pPrepara(float a_x,float a_y,float a_z,float a_rad,int a_pass);
	void pDrawMesh(D3DMATERIAL9* ,LPDIRECT3DTEXTURE9,LPD3DXMESH,INT,INT);		//�����A���ˌ��A�e�N�X�`���A���b�V�� , �p�X , �T�u�Z�b�g


public:

	//�R���X�g���N�^
	XfileLoad();
	XfileLoad(LPTKY3D pinit3d,HLSLShader *a_pHLSL);
	VIRTUAL ~XfileLoad();

	//X�t�@�C���ǂݍ���
	HRESULT ReadXFile(LPWSTR filename);

	//�����ɕ`������
	bool	DrawAnimeXFile(float x,float y,float z,float rad,int pass);

	//�ݒ�n
	void  SetAnimeFreem(INT);

	//�J������
	void	CleanXFile();

	//�擾�n
	LPD3DXMESH GetMesh()const{
		return m_pMesh;
	};
	FLOAT GetMaxFreem()const{return anime.NumFreem;};
	AnimetionClass GetAnimetionClass()const{return anime;};

};
//--------------------------------------------------//
//X�t�@�C���A�j���[�V������ǂݍ���
//--------------------------------------------------//
class D3DTKYXANIMETION
{
private:
	//�擾������
	XfileLoad *m_model;	//���f���̃C���f�b�N�X�ԍ�
	DWORD m_time;			//�t���[���̑J�ڑ��x
	//�擾���Ȃ����
	int m_maxloop;
	int m_nowloop;		//���̃t���[��
	DWORD m_nowtime;	//���̎���
public:
	D3DTKYXANIMETION();
	D3DTKYXANIMETION(const D3DTKYXANIMETION&);
	D3DTKYXANIMETION(XfileLoad*,INT);//��{���,�ړ�����
	VIRTUAL ~D3DTKYXANIMETION();
	//�`��ɕK�v�Ȃ���
	bool NextModel();
	void DrawModel(D3DXVECTOR3 pos,FLOAT rad,INT pass);
	//�f�[�^�ݒ�n
	void SetNowLoop(INT loop){m_nowloop = loop;}
	//�f�[�^�擾�n
	LPD3DXMESH GetModelMesh();
};