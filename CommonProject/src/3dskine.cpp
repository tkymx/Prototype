
#include"Anime.h"

//-------------------------------------------------------------------------------------
//詳しい情報はこちらから
//http://msdn.microsoft.com/ja-jp/library/bb147387(v=vs.85).aspx
//-------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Animetionクラス　コピーコンストラクタ
//--------------------------------------------------------------------------------------
AnimetionClass::AnimetionClass()
{
	//アニメ情報の設定
	g_bShowHelp = true;					
	g_pFrameRoot = NULL;
	g_pAnimController = NULL;
	g_SkinningMethod = METHOD::D3DNONINDEXED;			//今は何もなし//インデックス情報のついている頂点情報の描画(HLSL)
	g_pBoneMatrices = NULL;
	g_NumBoneMatricesMax = 0;
	m_AnimeSpeed = 1.0f;
}
AnimetionClass::AnimetionClass(const AnimetionClass& a)
{
//	g_pEffect = a.g_pEffect;					// D3DX effect interface
	g_pHLSL = a.g_pHLSL;						//シェーダのセット
	g_bShowHelp = a.g_bShowHelp;					// If true, it renders the UI control text
	g_pFrameRoot = a.g_pFrameRoot;
	g_pAnimController = a.g_pAnimController;
	g_vObjectCenter = a.g_vObjectCenter;					// Center of bounding sphere of object
	g_fObjectRadius = a.g_fObjectRadius;					// Radius of bounding sphere of object
	g_SkinningMethod = a.g_SkinningMethod;	// Current skinning method
	g_pBoneMatrices = a.g_pBoneMatrices;
	g_NumBoneMatricesMax = a.g_NumBoneMatricesMax;
	FOR_I(4)g_pIndexedVertexShader[i] = a.g_pIndexedVertexShader[i];
	g_matView = a.g_matView;							// View matrix
	g_matProj = a.g_matProj;							// Projection matrix
	g_matProjT = a.g_matProjT;							// Transpose of projection matrix (for asm shader)
	g_dwBehaviorFlags = g_dwBehaviorFlags;					// Behavior flags of the 3D device
	g_bUseSoftwareVP = g_bUseSoftwareVP;					// Flag to indicate whether software vp is
	alloc = a.alloc;
	m_pass = a.m_pass;									//描画する時のパス
	//ファイルのスラッシュまでのパス
	m_LastSlash = a.m_LastSlash;
	NumFreem = a.NumFreem;							//Xファイルのアニメーションフレームの数	
	m_pNowMesh = a.m_pNowMesh;									//今のメッシュ
	m_AnimeSpeed  =a.m_AnimeSpeed;
}
AnimetionClass::~AnimetionClass()
{

}
//--------------------------------------------------------------------------------------
// Called either by CreateMeshContainer when loading a skin mesh, or when 
// changing methods.  This function uses the pSkinInfo of the mesh 
// container to generate the desired drawable mesh and bone combination 
// table.
//--------------------------------------------------------------------------------------
HRESULT GenerateSkinnedMesh( IDirect3DDevice9 *pd3dDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer,AnimetionClass *anime )
{
    HRESULT hr = S_OK;
    D3DCAPS9 d3dCaps;
    pd3dDevice->GetDeviceCaps( &d3dCaps );

    IF( pMeshContainer->pSkinInfo == NULL )
        return hr;

	anime->g_bUseSoftwareVP = false;

    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );

    // IF non-indexed skinning mode selected, use ConvertToBlendedMesh to generate drawable mesh
    IF( anime->g_SkinningMethod == D3DNONINDEXED )
    {

        hr = pMeshContainer->pSkinInfo->ConvertToBlendedMesh
                                   (
                                       pMeshContainer->pOrigMesh,
                                       D3DXMESH_MANAGED|D3DXMESHOPT_VERTEXCACHE, 
                                       pMeshContainer->pAdjacency, 
                                       NULL, NULL, NULL, 
                                       &pMeshContainer->NumInfl,
                                       &pMeshContainer->NumAttributeGroups, 
                                       &pMeshContainer->pBoneCombinationBuf, 
                                       &pMeshContainer->MeshData.pMesh
                                   );
        IF (FAILED(hr))
            goto e_Exit;


        // IF the device can only do 2 matrix blends, ConvertToBlendedMesh cannot approximate all meshes to it
        // Thus we split the mesh in two parts: The part that uses at most 2 matrices and the rest. The first is
        // drawn using the device's HW vertex processing and the rest is drawn using SW vertex processing.
        LPD3DXBONECOMBINATION rgBoneCombinations  = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

        // look FOR any set of bone combinations that do not fit the caps
        FOR (pMeshContainer->iAttributeSW = 0; pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups; pMeshContainer->iAttributeSW++)
        {
            DWORD cInfl   = 0;

            FOR (DWORD iInfl = 0; iInfl < pMeshContainer->NumInfl; iInfl++)
            {
                IF (rgBoneCombinations[pMeshContainer->iAttributeSW].BoneId[iInfl] != UINT_MAX)
                {
                    ++cInfl;
                }
            }

            IF (cInfl > d3dCaps.MaxVertexBlendMatrices)
            {
                break;
            }
        }

        // IF there is both HW and SW, add the Software Processing flag
        IF (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
        {
            LPD3DXMESH pMeshTmp;

            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_SOFTWAREPROCESSING|pMeshContainer->MeshData.pMesh->GetOptions(), 
                                                pMeshContainer->MeshData.pMesh->GetFVF(),
                                                pd3dDevice, &pMeshTmp);
            IF (FAILED(hr))
            {
                goto e_Exit;
            }

            pMeshContainer->MeshData.pMesh->Release();
            pMeshContainer->MeshData.pMesh = pMeshTmp;
            pMeshTmp = NULL;
        }
    }
    // IF indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else IF( anime->g_SkinningMethod == D3DINDEXED )
    {
        DWORD NumMaxFaceInfl;
        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

        LPDIRECT3DINDEXBUFFER9 pIB;
        hr = pMeshContainer->pOrigMesh->GetIndexBuffer(&pIB);
        IF (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences(pIB, pMeshContainer->pOrigMesh->GetNumFaces(), &NumMaxFaceInfl);
        pIB->Release();
        IF (FAILED(hr))
            goto e_Exit;

        // 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
        // can be handled
        NumMaxFaceInfl = min(NumMaxFaceInfl, 12);

        IF( d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl )
        {
            // HW does not support indexed vertex blending. Use SW instead
            pMeshContainer->NumPaletteEntries = min(256, pMeshContainer->pSkinInfo->GetNumBones());
            pMeshContainer->UseSoftwareVP = true;
            anime->g_bUseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }
        else
        {
            // using hardware - determine palette size from caps and number of bones
            // IF normals are present in the vertex data that needs to be blended FOR lighting, then 
            // the number of matrices is half the number specIFied by MaxVertexBlendMatrixIndex.
            pMeshContainer->NumPaletteEntries = min( ( d3dCaps.MaxVertexBlendMatrixIndex + 1 ) / 2, 
                                                     pMeshContainer->pSkinInfo->GetNumBones() );
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pMeshContainer->pOrigMesh,
                                                Flags, 
                                                pMeshContainer->NumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL, 
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh);
        IF (FAILED(hr))
            goto e_Exit;
    }
    // IF vertex shader indexed skinning mode selected, use ConvertToIndexedsBlendedMesh to generate drawable mesh
    else IF( ( anime->g_SkinningMethod == D3DINDEXEDVS ) || ( anime->g_SkinningMethod == D3DINDEXEDHLSLVS ) )
    {
        // Get palette size
        // First 9 constants are used FOR other data.  Each 4x3 matrix takes up 3 constants.
        // (96 - 9) /3 i.e. Maximum constant count - used constants 
        UINT MaxMatrices = 26; 
        pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

        DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
        IF (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
        {
            pMeshContainer->UseSoftwareVP = false;
            Flags |= D3DXMESH_MANAGED;
        }
        else
        {
            pMeshContainer->UseSoftwareVP = true;
            anime->g_bUseSoftwareVP = true;
            Flags |= D3DXMESH_SYSTEMMEM;
        }

        SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

        hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
                                                (
                                                pMeshContainer->pOrigMesh,
                                                Flags, 
                                                pMeshContainer->NumPaletteEntries, 
                                                pMeshContainer->pAdjacency, 
                                                NULL, NULL, NULL,             
                                                &pMeshContainer->NumInfl,
                                                &pMeshContainer->NumAttributeGroups, 
                                                &pMeshContainer->pBoneCombinationBuf, 
                                                &pMeshContainer->MeshData.pMesh);
        IF (FAILED(hr))
            goto e_Exit;


        // FVF has to match our declarator. Vertex shaders are not as FORgiving as FF pipeline
        DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
        IF (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
        {
            LPD3DXMESH pMesh;
            hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, pd3dDevice, &pMesh);
            IF (!FAILED(hr))
            {
                pMeshContainer->MeshData.pMesh->Release();
                pMeshContainer->MeshData.pMesh = pMesh;
                pMesh = NULL;
            }
        }

        D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
        LPD3DVERTEXELEMENT9 pDeclCur;
        hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
        IF (FAILED(hr))
            goto e_Exit;

        // the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
        //   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
        //          this is more of a "cast" operation
        pDeclCur = pDecl;
        WHILE (pDeclCur->Stream != 0xff)
        {
            IF ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
                pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
            pDeclCur++;
        }

        hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
        IF (FAILED(hr))
            goto e_Exit;

        // allocate a buffer FOR bone matrices, but only IF another mesh has not allocated one of the same size or larger
        IF( anime->g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
        {
            anime->g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space FOR blend matrices
            delete[] anime->g_pBoneMatrices; 
            anime->g_pBoneMatrices  = new D3DXMATRIXA16[anime->g_NumBoneMatricesMax];
            IF( anime->g_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }

    }
    // IF software skinning selected, use GenerateSkinnedMesh to create a mesh that can be used with UpdateSkinnedMesh
    else IF( anime->g_SkinningMethod == SOFTWARE )
    {
        hr = pMeshContainer->pOrigMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
                                              pd3dDevice, &pMeshContainer->MeshData.pMesh);
        IF (FAILED(hr))
            goto e_Exit;

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumAttributeGroups);
        IF (FAILED(hr))
            goto e_Exit;

        delete[] pMeshContainer->pAttributeTable;
        pMeshContainer->pAttributeTable  = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
        IF (pMeshContainer->pAttributeTable == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        hr = pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);
        IF (FAILED(hr))
            goto e_Exit;

        // allocate a buffer FOR bone matrices, but only IF another mesh has not allocated one of the same size or larger
        IF (anime->g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
        {
            anime->g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

            // Allocate space FOR blend matrices
            delete[] anime->g_pBoneMatrices; 
            anime->g_pBoneMatrices  = new D3DXMATRIXA16[anime->g_NumBoneMatricesMax];
            IF( anime->g_pBoneMatrices == NULL )
            {
                hr = E_OUTOFMEMORY;
                goto e_Exit;
            }
        }
    }
    else  // invalid g_SkinningMethod value
    {        
        // return failure due to invalid skinning method value
        hr = E_INVALIDARG;
        goto e_Exit;
    }

e_Exit:
    return hr;
}
//--------------------------------------------------------------------------------------
// Name: AllocateName()
// Desc: Allocates memory FOR a string to hold the name of a frame or mesh
//--------------------------------------------------------------------------------------
HRESULT AllocateName( LPCSTR Name, LPSTR *pNewName )
{
    UINT cbLength;

    IF( Name != NULL )
    {
        cbLength = (UINT)strlen(Name) + 1;
        *pNewName = new CHAR[cbLength];
        IF (*pNewName == NULL)
            return E_OUTOFMEMORY;
        memcpy( *pNewName, Name, cbLength*sizeof(CHAR) );
    }
    else
    {
        *pNewName = NULL;
    }

    return S_OK;
}
//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateFrame()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame( LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{
    HRESULT hr = S_OK;
    D3DXFRAME_DERIVED *pFrame;

    *ppNewFrame = NULL;

    pFrame = new D3DXFRAME_DERIVED;
    IF (pFrame == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    hr = AllocateName(Name, &pFrame->Name);
    IF (FAILED(hr))
        goto e_Exit;

    // initialize other data members of the frame
    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;

    *ppNewFrame = pFrame;
    pFrame = NULL;

e_Exit:
    delete pFrame;
    return hr;
}
//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateMeshContainer()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateMeshContainer(
    LPCSTR Name, 
    CONST D3DXMESHDATA *pMeshData,
    CONST D3DXMATERIAL *pMaterials, 
    CONST D3DXEFFECTINSTANCE *pEffectInstances, 
    DWORD NumMaterials, 
    CONST DWORD *pAdjacency, 
    LPD3DXSKININFO pSkinInfo, 
    LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
    UINT NumFaces;
    UINT iMaterial;
    UINT iBone, cBones;
    LPDIRECT3DDEVICE9 pd3dDevice = NULL;

    LPD3DXMESH pMesh = NULL;

    *ppNewMeshContainer = NULL;

    // this sample does not handle patch meshes, so fail when one is found
    IF (pMeshData->Type != D3DXMESHTYPE_MESH)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // get the pMesh interface pointer out of the mesh data structure
    pMesh = pMeshData->pMesh;

    // this sample does not FVF compatible meshes, so fail when one is found
    IF (pMesh->GetFVF() == 0)
    {
        hr = E_FAIL;
        goto e_Exit;
    }

    // allocate the overloaded structure to return as a D3DXMESHCONTAINER
    pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
    IF (pMeshContainer == NULL)
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }
    memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

    // make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
    hr = AllocateName(Name, &pMeshContainer->Name);
    IF (FAILED(hr))
        goto e_Exit;        

    pMesh->GetDevice(&pd3dDevice);
    NumFaces = pMesh->GetNumFaces();

    // IF no normals are in the mesh, add them
    IF (!(pMesh->GetFVF() & D3DFVF_NORMAL))
    {
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        // clone the mesh to make room FOR the normals
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                    pMesh->GetFVF() | D3DFVF_NORMAL, 
                                    pd3dDevice, &pMeshContainer->MeshData.pMesh );
        IF (FAILED(hr))
            goto e_Exit;

        // get the new pMesh pointer back out of the mesh container to use
        // NOTE: we do not release pMesh because we do not have a reference to it yet
        pMesh = pMeshContainer->MeshData.pMesh;

        // now generate the normals FOR the pmesh
        D3DXComputeNormals( pMesh, NULL );
    }
    else  // IF no normals, just add a reference to the mesh FOR the mesh container
    {
        pMeshContainer->MeshData.pMesh = pMesh;
        pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

        pMesh->AddRef();
    }
        
    // allocate memory to contain the material information.  This sample uses
    //   the D3D9 materials and texture names instead of the EffectInstance style materials
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
    IF ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        hr = E_OUTOFMEMORY;
        goto e_Exit;
    }

    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    // IF materials provided, copy them
    IF (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        FOR (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
        {
            IF (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {

				//DirectXには環境光が無いので拡散光を代入
				pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient = pMaterials[iMaterial].MatD3D.Diffuse  ;

				//テクスチャーファイルの読み込み//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				WCHAR path[MAX_PATH];
				MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pMeshContainer->pMaterials[iMaterial].pTextureFilename,MAX_PATH,path,MAX_PATH);

				//スラッシュ部の追加
				wstring wstr = this->anime->m_LastSlash + path;

                IF( FAILED( /*D3DXCreateTextureFromFile(

												pd3dDevice, 
												strTexturePath,
                                                &pMeshContainer->ppTextures[iMaterial] )
*/

							D3DXCreateTextureFromFileExW(
									pd3dDevice,
									wstr.c_str(),              // ファイル名
									0,
									0,
									0,
									0,
									D3DFMT_A16B16G16R16,         
									D3DPOOL_MANAGED,
									D3DX_FILTER_LINEAR,
									D3DX_FILTER_LINEAR,
									D3DCOLOR_ARGB(0, 0, 0, 0),  // 黒色を透過色とする
									NULL,
									NULL,
									&pMeshContainer->ppTextures[iMaterial]  // テクスチャ名
								)
								
					) )
					{
						MessageBoxA( NULL,"読み込み失敗","texture",NULL );
						pMeshContainer->ppTextures[iMaterial] = NULL;
					}
                // don't remember a pointer into the dynamic memory, just FORget the name after loading
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;

				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }
        }
    }
    else // IF no materials provided, use a default one
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }

    // IF there is skinning information, save off the required data and then setup FOR HW skinning
    IF (pSkinInfo != NULL)
    {
        // first save off the SkinInfo and original mesh data
        pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();

        pMeshContainer->pOrigMesh = pMesh;
        pMesh->AddRef();

        // Will need an array of offset matrices to move the vertices from the figure space to the bone's space
        cBones = pSkinInfo->GetNumBones();
        pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
        IF (pMeshContainer->pBoneOffsetMatrices == NULL)
        {
            hr = E_OUTOFMEMORY;
            goto e_Exit;
        }

        // get each of the bone offset matrices so that we don't need to get them later
        FOR (iBone = 0; iBone < cBones; iBone++)
        {
            pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
        }

        // GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
		hr = GenerateSkinnedMesh( pd3dDevice, pMeshContainer,CAllocateHierarchy::anime );
        IF (FAILED(hr))
            goto e_Exit;
    }

    *ppNewMeshContainer = pMeshContainer;
    pMeshContainer = NULL;

e_Exit:
    SAFE_RELEASE(pd3dDevice);

    // call Destroy function to properly clean up the memory allocated 
    IF (pMeshContainer != NULL)
    {
        DestroyMeshContainer(pMeshContainer);
    }

    return hr;
}
//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
    SAFE_DELETE_ARRAY( pFrameToFree->Name );
    SAFE_DELETE( pFrameToFree );
    return S_OK; 
}
//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    UINT iMaterial;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    SAFE_DELETE_ARRAY( pMeshContainer->Name );
    SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
    SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );

    // release all the allocated textures
    IF (pMeshContainer->ppTextures != NULL)
    {
        FOR (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
        }
    }

    SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
    SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
    SAFE_RELEASE( pMeshContainer->pBoneCombinationBuf );
    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
    SAFE_RELEASE( pMeshContainer->pSkinInfo );
    SAFE_RELEASE( pMeshContainer->pOrigMesh );
    SAFE_DELETE( pMeshContainer );
    return S_OK;
}
//--------------------------------------------------------------------------------------
// Called to render a mesh in the hierarchy(実際描画する)
//--------------------------------------------------------------------------------------
void AnimetionClass::DrawMeshContainer( IDirect3DDevice9 *pd3dDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase )
{
    HRESULT hr;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    UINT iMaterial;
    UINT NumBlend;
    UINT iAttrib;
    DWORD AttribIdPrev;
    LPD3DXBONECOMBINATION pBoneComb;

    UINT iMatrixIndex;
    UINT iPaletteEntry;
    D3DXMATRIXA16 matTemp;
    D3DCAPS9 d3dCaps;
    pd3dDevice->GetDeviceCaps( &d3dCaps );

	//スキンメッシュか判断する
    // first check FOR skinning
    IF (pMeshContainer->pSkinInfo != NULL )
    {
		//============================================
		//固定機能インデックスなしスキニングによるレンダリング
		//============================================
        IF( g_SkinningMethod == D3DNONINDEXED )
        {
            AttribIdPrev = UNUSED32; 
            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

			//ハードウェアレンダリングだったら
            // Draw using default vtx processing of the device (typically HW)
            FOR (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            {
				//ブレンド数の取得
                NumBlend = 0;
                FOR (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                {
                    IF (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
                    {
                        NumBlend = i;
                    }
                }


                IF( d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1 )
                {
					////まず最初に、現在のセットのブレンドウェイトのためのワールドマトリクスを計算してください、そして、ブレンドの数の正確な計算を得てください。
                    // first calculate the world matrices FOR the current set of blend weights and get the accurate count of the number of blends
                    FOR (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                    {
                        iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                        IF (iMatrixIndex != UINT_MAX)
                        {
							//ワールド座標を出した
                            D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
							//デバイスのセット
                            V( pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), &matTemp ) );
                        }
                    }

					//ブレンディングのための行列の個数を指定
                    V( pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend) );

					//このサブセットで使うマテリアルの検索
                    // lookup the material used FOR this subset of faces
                    IF ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
                    {
						//マテリアルのセット
                        V( pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D ) );
						//テクスチャのセット
                        V( pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] ) );

                        AttribIdPrev = pBoneComb[iAttrib].AttribId;
                    }

					//サブセットの描画
                    // draw the subset now that the correct material and matrices are loaded
                    V( pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib) );
                }
            }

			//もし必要だったらソフトウェアレンダリングで
            // IF necessary, draw parts that HW could not handle using SW
            IF (pMeshContainer->iAttributeSW < pMeshContainer->NumAttributeGroups)
            {
                AttribIdPrev = UNUSED32; 
                V( pd3dDevice->SetSoftwareVertexProcessing(TRUE) );
                FOR (iAttrib = pMeshContainer->iAttributeSW; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
                {
					//ブレンド数の取得
                    NumBlend = 0;
                    FOR (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                    {
                        IF (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
                        {
                            NumBlend = i;
                        }
                    }

                    IF (d3dCaps.MaxVertexBlendMatrices < NumBlend + 1)
                    {
                        // first calculate the world matrices FOR the current set of blend weights and get the accurate count of the number of blends
                        FOR (DWORD i = 0; i < pMeshContainer->NumInfl; ++i)
                        {
                            iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
                            IF (iMatrixIndex != UINT_MAX)
                            {
                                D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );
                                V( pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), &matTemp ) );
                            }
                        }

                        V( pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend) );

                        // lookup the material used FOR this subset of faces
                        IF ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
                        {
                            V( pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D ) );
                            V( pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] ) );
                            AttribIdPrev = pBoneComb[iAttrib].AttribId;
                        }

                        // draw the subset now that the correct material and matrices are loaded
                        V( pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib) );
                    }
                }
                V( pd3dDevice->SetSoftwareVertexProcessing( FALSE ) );
            }

			///ブレンディングのための行列の個数を0に設定する
            V( pd3dDevice->SetRenderState( D3DRS_VERTEXBLEND, 0) );
        }
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//インデックス番号が付いているメッシュをHLSLで描画
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        IF (g_SkinningMethod == D3DINDEXEDHLSLVS)																																						
        {
            IF (pMeshContainer->UseSoftwareVP)
            {
                // IF hw or pure hw vertex processing is FORced, we can't render the
                // mesh, so just exit out.  Typical applications should create
                // a device with appropriate vertex processing capability FOR this
                // skinning method.
				//もしハードウェアでレンダリングできな場合は抜ける
                IF( g_dwBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
                    return;

				//
                V( pd3dDevice->SetSoftwareVertexProcessing(TRUE) );
            }


            pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
            FOR (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
            { 
				
                // まずはじめにワールドマトリックスの計算
                FOR (iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
                {
                    iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
                    IF (iMatrixIndex != UINT_MAX)
                    {
                        D3DXMatrixMultiply(&g_pBoneMatrices[iPaletteEntry], &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex], pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
                    }
                }
				//ボーンによるマトリックス変換の設定
				V( g_pHLSL->m_pEffect->SetMatrixArray( g_pHLSL->m_hmWorldMatrixArray, g_pBoneMatrices, pMeshContainer->NumPaletteEntries) );

                // マテリアルの情報をセットする
				D3DXVECTOR4 l_vLightAmb = D3DXVECTOR4( 
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient.r ,
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient.g ,
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient.b ,
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Ambient.a );

				D3DXVECTOR4 l_vLightDif = D3DXVECTOR4( 
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse.r ,
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse.g ,
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse.b ,
					pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse.a );

				V( g_pHLSL->m_pEffect->SetVector(g_pHLSL->m_hvLightDif, &l_vLightDif));
				V( g_pHLSL->m_pEffect->SetVector(g_pHLSL->m_hvLightAmb, &l_vLightAmb ));

                //オリジナルのプレスキニング属性IDを使用してテクスチャをセットする
				V( g_pHLSL->m_pEffect->SetTexture(g_pHLSL->m_hTex,pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]));

                // CurNumBonesにボーンの数のための正しい頂点シェーダを選択するように設定してください。
				V( g_pHLSL->m_pEffect->SetInt( g_pHLSL->m_hiNumBones, pMeshContainer->NumInfl-1) );

				V( g_pHLSL->m_pEffect->BeginPass( AnimetionClass::m_pass ) );

                // draw the subset with the current world matrix palette and material state
                V( pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib ) );
				m_pNowMesh = pMeshContainer->MeshData.pMesh;

                V( g_pHLSL->m_pEffect->EndPass() );

                V( pd3dDevice->SetVertexShader(NULL) );
            }

            // remember to reset back to hw vertex processing IF software was required
            IF (pMeshContainer->UseSoftwareVP)
            {
                V( pd3dDevice->SetSoftwareVertexProcessing(FALSE) );
            }
        }
    }
    else  //もしスキンメッシュじゃなく通常メッシュだったら
    {
        V( pd3dDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix) );

        for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
            V( pd3dDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D ) );
            V( pd3dDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] ) );
            V( pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial) );
        }
/*
        // CurNumBonesにボーンの数のための正しい頂点シェーダを選択するように設定してください。
		V( g_pHLSL->m_pEffect->SetInt( g_pHLSL->m_hiNumBones, 0) );

		//マテリアルの数だけ繰り返し、描画する
        FOR (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
        {
			D3DXVECTOR4 l_vLightAmb = D3DXVECTOR4( 
				pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient.r ,
				pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient.g ,
				pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient.b ,
				pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient.a );

			D3DXVECTOR4 l_vLightDif = D3DXVECTOR4( 
				pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.r ,
				pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.g ,
				pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.b ,
				pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.a );

			//HLSLにテクスチャを設定する	
			g_pHLSL->m_pEffect->SetTexture(g_pHLSL->m_hTex,pMeshContainer->ppTextures[iMaterial] );
			g_pHLSL->m_pEffect->SetVector(g_pHLSL->m_hvLightAmb,&l_vLightAmb);
			g_pHLSL->m_pEffect->SetVector(g_pHLSL->m_hvLightDif,&l_vLightDif);

			//シェーダを開始する
			g_pHLSL->m_pEffect->BeginPass(AnimetionClass::m_pass);

			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);												//Direc3Dデバイスに登録したメッシュを描画する

			//終了する
			g_pHLSL->m_pEffect->EndPass();

		}
*/
    }
}
//--------------------------------------------------------------------------------------
// Called to render a frame in the hierarchy(フレームを読み込んで描画する)
//--------------------------------------------------------------------------------------
void AnimetionClass::DrawFrame( IDirect3DDevice9 *pd3dDevice, LPD3DXFRAME pFrame )
{
    LPD3DXMESHCONTAINER pMeshContainer;

    pMeshContainer = pFrame->pMeshContainer;
    WHILE (pMeshContainer != NULL)
    {
        DrawMeshContainer( pd3dDevice, pMeshContainer, pFrame );

        pMeshContainer = pMeshContainer->pNextMeshContainer;
    }

    IF (pFrame->pFrameSibling != NULL)
    {
        DrawFrame( pd3dDevice, pFrame->pFrameSibling);
    }

    IF (pFrame->pFrameFirstChild != NULL)
    {
        DrawFrame( pd3dDevice, pFrame->pFrameFirstChild );
    }
}
//--------------------------------------------------------------------------------------
// Called to setup the pointers FOR a given bone to its transformation matrix (ボーン情報を入れる)
//--------------------------------------------------------------------------------------
HRESULT AnimetionClass::SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase )
{
    UINT iBone, cBones;
    D3DXFRAME_DERIVED *pFrame;

    D3DXMESHCONTAINER_DERIVED *pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

    // IF there is a skinmesh, then setup the bone matrices
    IF (pMeshContainer->pSkinInfo != NULL)
    {
        cBones = pMeshContainer->pSkinInfo->GetNumBones();

        pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
        IF (pMeshContainer->ppBoneMatrixPtrs == NULL)
            return E_OUTOFMEMORY;

        FOR (iBone = 0; iBone < cBones; iBone++)
        {
            pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind( g_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone) );
            IF (pFrame == NULL)
                return E_FAIL;

            pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;

        }
    }

    return S_OK;
}
//--------------------------------------------------------------------------------------
// Called to setup the pointers FOR a given bone to its transformation matrix
//--------------------------------------------------------------------------------------
HRESULT AnimetionClass::SetupBoneMatrixPointers( LPD3DXFRAME pFrame )
{
    HRESULT hr;

    IF (pFrame->pMeshContainer != NULL)
    {
        hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
        IF (FAILED(hr))
            return hr;
    }

    IF (pFrame->pFrameSibling != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
        IF (FAILED(hr))
            return hr;
    }

    IF (pFrame->pFrameFirstChild != NULL)
    {
        hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
        IF (FAILED(hr))
            return hr;
    }

    return S_OK;
}
//--------------------------------------------------------------------------------------
// update the frame matrices
//--------------------------------------------------------------------------------------
void AnimetionClass::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

    IF (pParentMatrix != NULL)
        D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
    else
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

    IF (pFrame->pFrameSibling != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    }

    IF (pFrame->pFrameFirstChild != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}
//--------------------------------------------------------------------------------------
// update the skinning method
//--------------------------------------------------------------------------------------
void AnimetionClass::UpdateSkinningMethod( LPD3DXFRAME pFrameBase )
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer;

    pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pFrame->pMeshContainer;

    WHILE( pMeshContainer != NULL )
    {
//        GenerateSkinnedMesh( DXUTGetD3DDevice(), pMeshContainer,this );

        pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pMeshContainer->pNextMeshContainer;
    }

    IF (pFrame->pFrameSibling != NULL)
    {
        UpdateSkinningMethod(pFrame->pFrameSibling);
    }

    IF (pFrame->pFrameFirstChild != NULL)
    {
        UpdateSkinningMethod(pFrame->pFrameFirstChild);
    }
}
void AnimetionClass::ReleaseAttributeTable( LPD3DXFRAME pFrameBase )
{
    D3DXFRAME_DERIVED *pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
    D3DXMESHCONTAINER_DERIVED *pMeshContainer;

    pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pFrame->pMeshContainer;

    WHILE( pMeshContainer != NULL )
    {
        delete[] pMeshContainer->pAttributeTable;

        pMeshContainer = (D3DXMESHCONTAINER_DERIVED *)pMeshContainer->pNextMeshContainer;
    }

    IF (pFrame->pFrameSibling != NULL)
    {
        ReleaseAttributeTable(pFrame->pFrameSibling);
    }

    IF (pFrame->pFrameFirstChild != NULL)
    {
        ReleaseAttributeTable(pFrame->pFrameFirstChild);
    }
}
