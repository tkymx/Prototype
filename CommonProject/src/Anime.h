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
////フレーム構造体
//--------------------------------------------------------------------------------------
struct D3DXFRAME_DERIVED: public D3DXFRAME
{
    D3DXMATRIXA16        CombinedTransformationMatrix;
};


//--------------------------------------------------------------------------------------
// Name: struct D3DXMESHCONTAINER_DERIVED
// Desc: Structure derived from D3DXMESHCONTAINER so we can add some app-specific
//       info that will be stored with each mesh
//メッシュコンテナ構造体
//--------------------------------------------------------------------------------------
struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	//後々ここを自分で実装して(テクスチャが重複作成されないようにする)
    LPDIRECT3DTEXTURE9*  ppTextures;//テクスチャーの配列、テクスチャーがなかったらNULL   
                               
    //スキンメッシュ情報           
    LPD3DXMESH           pOrigMesh;						//メッシュ
    LPD3DXATTRIBUTERANGE pAttributeTable;			//属性
    DWORD                NumAttributeGroups;			//属性のグループ
    DWORD                NumInfl;
    LPD3DXBUFFER         pBoneCombinationBuf;		//ボーンの組み合わせバッファ
    D3DXMATRIX**         ppBoneMatrixPtrs;				//ボーンマトリクスポインタ
    D3DXMATRIX*          pBoneOffsetMatrices;			//ボーンオフセットマトリクス
    DWORD                NumPaletteEntries;				//パレットエントリー
    bool                 UseSoftwareVP;						//ソフトウェアVPを使う
    DWORD                iAttributeSW;						//ノンインデックススキニングならソフトウェアとハードウェアで分けて指定する必要がある
};



//--------------------------------------------------------------------------------------
// Name: class CAllocateHierarchy
// Desc: Custom version of ID3DXAllocateHierarchy with custom methods to create
//       frames and meshcontainers.
//階層割り当てクラス
//--------------------------------------------------------------------------------------
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	//作成
    STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);		//関数：フレーム作成
 	STDMETHOD(CreateMeshContainer)(THIS_															//関数：メッシュコンテナ作成
        LPCSTR Name,																							//名前
        CONST D3DXMESHDATA *pMeshData,																//メッシュデータ
        CONST D3DXMATERIAL *pMaterials,																//マテリアル
        CONST D3DXEFFECTINSTANCE *pEffectInstances,												//エフェクトの実体
        DWORD NumMaterials,																					//マテリアル数
        CONST DWORD *pAdjacency,																		//隣接関係
        LPD3DXSKININFO pSkinInfo,																			//スキン情報
        LPD3DXMESHCONTAINER *ppNewMeshContainer);											//メッシュコンテナのポインタ
	//消去
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);							//関数：フレーム削除
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);//関数：メッシュコンテナ削除

	//内部で呼ぶため
	AnimetionClass *anime;

	CAllocateHierarchy() {}
};

class AnimetionClass{

public:
	//--------------------------------------------------------------------------------------
	// Global variables
	//--------------------------------------------------------------------------------------
//	ID3DXEffect*						g_pEffect;					// D3DX effect interface
	HLSLShader*						g_pHLSL;						//シェーダのセット
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
	INT									m_pass;									//描画する時のパス

	//ファイルのスラッシュまでのパス
	wstring								m_LastSlash;

	FLOAT						NumFreem;							//Xファイルのアニメーションフレームの数	
	LPD3DXMESH				m_pNowMesh;									//今のメッシュ

	//アニメーションスピード
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

	//コンストラクタ
	AnimetionClass();
	AnimetionClass(const AnimetionClass& a);
	~AnimetionClass();



	//フレーム制御
	FLOAT UpdateFrame( FLOAT frame , float step )
	{
		frame += step * m_AnimeSpeed;
		IF( NumFreem < frame )frame = 0;
		return frame;
	}

	//スピードのセット
	VOID SetAnimeSpeed(float s){  m_AnimeSpeed = s; }

};



#endif // !defined(ANIME_H)
