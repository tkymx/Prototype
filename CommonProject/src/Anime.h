#pragma once
#ifndef ANIME_H
#define ANIME_H

// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>
#include "TKYBASEHEADER.h"
#include"TKYHLSL.h"


using namespace std;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_METHOD              5

class CAllocateHierarchy;
class AnimetionClass;

enum METHOD
{
    D3DNONINDEXED,
    D3DINDEXED,
    SOFTWARE,
    D3DINDEXEDVS,
    D3DINDEXEDHLSLVS,
    NONE
};


//--------------------------------------------------------------------------------------
// Name: struct D3DXFRAME_DERIVED
// Desc: Structure derived from D3DXFRAME so we can add some app-specific
//       info that will be stored with each frame
////�t���[���\����
//--------------------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
    D3DXMATRIXA16        CombinedTransformationMatrix;
};


//--------------------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//���b�V���R���e�i�\����
//--------------------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	//��X�����������Ŏ�������(�e�N�X�`�����d���쐬����Ȃ��悤�ɂ���)
    LPDIRECT3DTEXTURE9*  ppTextures;//�e�N�X�`���[�̔z��A�e�N�X�`���[���Ȃ�������NULL   
                               
    //�X�L�����b�V�����           
    LPD3DXMESH           pOrigMesh;						//���b�V��
    LPD3DXATTRIBUTERANGE pAttributeTable;			//����
    DWORD                NumAttributeGroups;			//�����̃O���[�v
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;		//�{�[���̑g�ݍ��킹�o�b�t�@
    D3DXMATRIX**         ppBoneMatrixPtrs;				//�{�[���}�g���N�X�|�C���^
    D3DXMATRIX*          pBoneOffsetMatrices;			//�{�[���I�t�Z�b�g�}�g���N�X
    DWORD                NumPaletteEntries;				//�p���b�g�G���g���[
    bool                 UseSoftwareVP;						//�\�t�g�E�F�AVP���g��
    DWORD                iAttributeSW;						//�m���C���f�b�N�X�X�L�j���O�Ȃ�\�t�g�E�F�A�ƃn�[�h�E�F�A�ŕ����Ďw�肷��K�v������
};



//--------------------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//�K�w���蓖�ăN���X
//--------------------------------------------------------------------------------------
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	//�쐬
    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);		//�֐��F�t���[���쐬
 	STDMETHOD(CreateMeshContainer)(THIS_															//�֐��F���b�V���R���e�i�쐬
        LPCSTR Name,																							//���O
        CONST D3DXMESHDATA *pMeshData,																//���b�V���f�[�^
        CONST D3DXMATERIAL *pMaterials,																//�}�e���A��
        CONST D3DXEFFECTINSTANCE *pEffectInstances,												//�G�t�F�N�g�̎���
        DWORD NumMaterials,																					//�}�e���A����
        CONST DWORD *pAdjacency,																		//�אڊ֌W
        LPD3DXSKININFO pSkinInfo,																			//�X�L�����
        LPD3DXMESHCONTAINER *ppNewMeshContainer);											//���b�V���R���e�i�̃|�C���^
	//����
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);							//�֐��F�t���[���폜
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);//�֐��F���b�V���R���e�i�폜

	//�����ŌĂԂ���
	AnimetionClass *anime;

	CAllocateHierarchy() {}
};

class AnimetionClass{

public:
	//--------------------------------------------------------------------------------------
	// Global variables
	//--------------------------------------------------------------------------------------
//	ID3DXEffect*						g_pEffect;					// D3DX effect interface
	HLSLShader*						g_pHLSL;						//�V�F�[�_�̃Z�b�g
	bool									g_bShowHelp;					// If true, it renders the UI control text
	LPD3DXFRAME					g_pFrameRoot;
	ID3DXAnimationController*	g_pAnimController;
	D3DXVECTOR3					g_vObjectCenter;					// Center of bounding sphere of object
	FLOAT								g_fObjectRadius;					// Radius of bounding sphere of object
	METHOD								g_SkinningMethod;	// Current skinning method
	D3DXMATRIXA16*				g_pBoneMatrices;
	UINT									g_NumBoneMatricesMax;
	IDirect3DVertexShader9*		g_pIndexedVertexShader[4];
	D3DXMATRIXA16					g_matView;							// View matrix
	D3DXMATRIXA16					g_matProj;							// Projection matrix
	D3DXMATRIXA16					g_matProjT;							// Transpose of projection matrix (for asm shader)
	DWORD								g_dwBehaviorFlags;					// Behavior flags of the 3D device
	bool									g_bUseSoftwareVP;					// Flag to indicate whether software vp is
	CAllocateHierarchy				alloc;
	INT									m_pass;									//�`�悷�鎞�̃p�X

	//�t�@�C���̃X���b�V���܂ł̃p�X
	wstring								m_LastSlash;

	FLOAT						NumFreem;							//X�t�@�C���̃A�j���[�V�����t���[���̐�	
	LPD3DXMESH				m_pNowMesh;									//���̃��b�V��

	//�A�j���[�V�����X�s�[�h
	FLOAT m_AnimeSpeed;

	//--------------------------------------------------------------------------------------
	// Forward declarations 
	//--------------------------------------------------------------------------------------
	void    DrawMeshContainer( IDirect3DDevice9 *pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase );
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainer );
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
	void    UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
	void    UpdateSkinningMethod( LPD3DXFRAME pFrameBase );
	void    ReleaseAttributeTable( LPD3DXFRAME pFrameBase );
	void    DrawFrame( IDirect3DDevice9 *pd3dDevice, LPD3DXFRAME pFrame );

	//�R���X�g���N�^
	AnimetionClass();
	AnimetionClass(const AnimetionClass& a);
	~AnimetionClass();



	//�t���[������
	FLOAT UpdateFrame( FLOAT frame , float step )
	{
		frame += step * m_AnimeSpeed;
		IF( NumFreem < frame )frame = 0;
		return frame;
	}

	//�X�s�[�h�̃Z�b�g
	VOID SetAnimeSpeed(float s){  m_AnimeSpeed = s; }

};



#endif // !defined(ANIME_H)
