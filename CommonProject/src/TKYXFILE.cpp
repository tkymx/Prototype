
#include "TKYXFILE.h"

//========== �V�������e ==========
	//�R���X�g���N�^
	CXFile::CXFile(LPTKY3D d)
	{
		m_device = d;

		//���b�V����񂾂�
		m_pMeshMaterials = NULL;		//�}�e���A�����i�[����
		m_pMeshTextures = NULL;		//�e�N�X�`�����i�[����
		m_dwNumMaterials = NULL;		//�}�e���A���̐����i�[����
		m_pMesh = NULL;

		//�A�j�����̐ݒ�
		anime.g_bShowHelp = true;					
		anime.g_pFrameRoot = NULL;
		anime.g_pAnimController = NULL;
		anime.g_SkinningMethod = METHOD::D3DNONINDEXED;			//���͉����Ȃ�//�C���f�b�N�X���̂��Ă��钸�_���̕`��(HLSL)
		anime.g_pBoneMatrices = NULL;
		anime.g_NumBoneMatricesMax = 0;

	}
	CXFile::CXFile(const CXFile &file)
	{
		this->anime = file.anime;
		this->m_pMesh = file.GetMesh();
		this->m_pMeshMaterials = file.GetMaterial();
		this->m_pMeshTextures = file.GetTexture();
		this->m_dwNumMaterials = file.GetMaterialNum();
	}
	CXFile::~CXFile()
	{
		//���b�V����������
		SAFE_DELETE_ARRAY( m_pMeshMaterials );
		IF(m_pMeshTextures)
		{
			FOR(DWORD i=0;i<m_dwNumMaterials;i++)
			{
				SAFE_RELEASE(m_pMeshTextures[i]);
			}
			SAFE_DELETE_ARRAY(m_pMeshTextures);
		}
		SAFE_RELEASE(m_pMesh);

		//�A�j���[�V������������
		CAllocateHierarchy Alloc;
		D3DXFrameDestroy( anime.g_pFrameRoot, &Alloc );
		SAFE_RELEASE( anime.g_pAnimController );
	}
	//X�t�@�C���̐ݒ�
	HRESULT CXFile::SetXFileW(wstring name)
	{

		LPD3DXBUFFER pD3DMtrlBuffer;							//�ꎞ�I�ȃ}�e���A���f�[�^���L�^����
		HRESULT h;

		if(FAILED(h = D3DXLoadMeshFromXW(name.c_str(),					//X�t�@�C���̖��O		
									D3DXMESH_SYSTEMMEM,			//���b�V���̃�������ł̊i�[���@���w��
									m_device->GetD3DDevice(),		//Direct3D�f�o�C�X�̃|�C���^
									NULL,						//�אڐ��f�[�^���L�^����|�C���^
									NULL,//&pD3DMtrlBuffer,			//�}�e���A���f�[�^���L�^����|�C���^
									NULL,						//�G�t�F�N�g�C���X�^���X���L�^����|�C���^
									&(m_dwNumMaterials),	//�}�e���A�������L�^����|�C���^
									&(m_pMesh))))			//���b�V���f�[�^���L�^����|�C���^
		{

			WCHAR strMesh[MAX_PATH];							//�K�w����߂邽�߂̕�����(������)
			StringCchCopyW( strMesh , MAX_PATH , TEXT("..\\"));	//�K�w����߂邽�߂̕�����̑��
			StringCchCatW(strMesh , MAX_PATH , name.c_str());		//���b�V���t�@�C������A������

			if(FAILED(h = D3DXLoadMeshFromXW(strMesh,
										D3DXMESH_SYSTEMMEM,
										this->m_device->GetD3DDevice(),
										NULL,
										&pD3DMtrlBuffer,
										NULL,
										&(m_dwNumMaterials),
										&(m_pMesh))))
			{
		
				MessageBox(NULL,TEXT("X�@�t�@�C����������܂���"),TEXT("Meshes.exe"),MB_OK);
				return E_FAIL;

			}

		}

		//=======================================================================================
		//X�t�@�C���̂���t�H���_�p�X�̎擾
		//=======================================================================================

		//�X���b�V��
		wstring LastSlash;
		//�h/�h�̃C���f�b�N�X�l��T��
		UINT l_LastSlashIndex = name.find_last_of(TEXT("/"),name.npos);
		IF(l_LastSlashIndex == wstring::npos)
		{
			//�h\\�h�̃C���f�b�N�X�l��T��
			l_LastSlashIndex = name.find_last_of(TEXT("\\"),name.npos);
		}

		//�����X���b�V�����Ȃ�������
		IF(l_LastSlashIndex == wstring::npos)
		{
			LastSlash = TEXT("");
		}
		else	//�X���b�V������������
		{
			//�X���b�V�����̕�����ǉ�
			_FOR_I(l_LastSlashIndex+1)
			{
				LastSlash += name.at(i);
			}
		}	

/*
		//=======================================================================================
		//=======================================================================================

		//=======================================================================================
		//�����Ȃ�XFILE�̎擾
		//=======================================================================================

		D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DMtrlBuffer->GetBufferPointer();	//�f�[�^�ҏW�p�̃A�h���X�����o��(�f�[�^�������Ă���)

		m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];								//X�t�@�C���̃}�e���A���̓��I�m��
		if(m_pMeshMaterials == NULL) return E_OUTOFMEMORY;									//�G���[����

		m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];							//X�t�@�C���̃e�N�X�`���̓��I�m��
		if(m_pMeshTextures == NULL) return E_OUTOFMEMORY;									//�G���[����

		//�}�e���A���̐��������[�v
		for( DWORD i=0; i<m_dwNumMaterials ; i++)
		{

			m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;									//�}�e���A�����i�[�p�ϐ��ɃR�s�[����

			m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;						//X�t�@�C���Ɋ��������݂��Ȃ��̂Ŕ��ˌ���������

			m_pMeshTextures[i] = NULL;														//�e�N�X�`���̏�����

			//�e�N�X�`����������������
			if(d3dxMaterials[i].pTextureFilename != NULL && strlen(d3dxMaterials[i].pTextureFilename) > 0)
			{

				//�e�N�X�`���̖��O
				WCHAR wtex[MAX_PATH];
				MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,d3dxMaterials[i].pTextureFilename,MAX_PATH,wtex,MAX_PATH);
				wstring tmpFilename = wtex;
				//�X���b�V���̒ǉ�
				tmpFilename = LastSlash + tmpFilename;


				if(FAILED(D3DXCreateTextureFromFileW(m_device->GetD3DDevice(),								//Direct3D�f�o�C�X
													tmpFilename.c_str(),										//�e�N�X�`���t�@�C����
													&(m_pMeshTextures[i]))))						//�ǂݍ��񂾃e�N�X�`����Ԃ��|�C���^
				{

					WCHAR strTexture[MAX_PATH];															//�K�w����߂邽�߂̕�����(������)
					StringCchCopyW( strTexture , MAX_PATH , TEXT("..\\"));								//�K�w����߂邽�߂̕�����̑��
					StringCchCatW(strTexture , MAX_PATH , tmpFilename.c_str());		//�e�N�X�`���t�@�C������A������

					if(FAILED(D3DXCreateTextureFromFileW(m_device->GetD3DDevice(),							//Direct3D�f�o�C�X
														strTexture,										//�e�N�X�`���t�@�C����
														&(m_pMeshTextures[i]))))					//�ǂݍ��񂾃e�N�X�`����Ԃ��|�C���^
					{

						MessageBox(NULL,TEXT("�e�N�X�`���t�@�C����������܂���"),TEXT("ERR"),MB_OK);

					}
				}
			}
		}


		pD3DMtrlBuffer->Release();					//�ꎞ�t�@�C���Ȃ̂ŊJ������

		//=======================================================================================
		//=======================================================================================
*/
		//=======================================================================================
		//�A�j���[�V����X�t�@�C���̎擾
		//=======================================================================================

		HRESULT hr;
		CAllocateHierarchy Alloc;
		Alloc.anime = &(this->anime);
		Alloc.anime->m_LastSlash.clear();

		Alloc.anime->m_LastSlash = LastSlash;

		//X�t�@�C�����烁�b�V���ƃ{�[���K�w�t���[�����擾����
		IF_FAILED(hr = D3DXLoadMeshHierarchyFromXW( 
			name.c_str(),									//�t�@�C����
			D3DXMESH_MANAGED,						//���b�V���̍쐬�I�v�V����
			m_device->GetD3DDevice(),				//IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^                     
			&(Alloc),										//ID3DXAllocateHierarchy �C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,											//���[�U�[ �f�[�^�����[�h���邽�߂ɃA�v���P�[�V�������񋟂���C���^�[�t�F�C�X
			&(anime.g_pFrameRoot),					//���[�h���ꂽ�t���[���K�w�ւ̃|�C���^
			&(anime.g_pAnimController)				//.x �t�@�C���̃A�j���[�V�����ɑΉ�����A�j���[�V���� �R���g���[���[�ւ̃|�C���^
			) )
		{
			MessageBoxA( NULL,"�A�j���ǂݍ��߂܂���","err",NULL );
			return hr;
		}

		//�{�[���̃Z�b�g
		V_RETURN( anime.SetupBoneMatrixPointers( anime.g_pFrameRoot ) );
		//�t���[���̃Z�b�g
		V_RETURN( D3DXFrameCalculateBoundingSphere( anime.g_pFrameRoot, &anime.g_vObjectCenter, &anime.g_fObjectRadius ) );

		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		UINT freem=0;
		//�A�j�����Z�b�g����Ă�����
		if(anime.g_pAnimController!=NULL)
		{

			UINT x = anime.g_pAnimController->GetMaxNumAnimationSets();
			LPD3DXANIMATIONSET l_anime[1];

			anime.g_pAnimController->GetTrackAnimationSet(0,l_anime);
			freem = 58;

			//�f�o�b�O�p
			char str[260];
			sprintf_s(str,"%d,%d,%d,%d,%d,%d,%lf,     ,%d",
				anime.g_pAnimController->GetMaxNumAnimationSets(),
				anime.g_pAnimController->GetMaxNumAnimationOutputs(),
				anime.g_pAnimController->GetMaxNumEvents(),
				anime.g_pAnimController->GetMaxNumTracks(),
				anime.g_pAnimController->GetTime(),
				anime.g_pAnimController->GetNumAnimationSets(),
				anime.g_pAnimController->GetPriorityBlend(),
				l_anime[0]->GetNumAnimations()
				);
		}

		anime.NumFreem = (FLOAT)freem;		//�A�j���[�V�����̃t���[���̐�
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		//=======================================================================================
		//=======================================================================================


		return S_OK;
	}
	//�\��
	HRESULT CXFile::DrawMesh( Vector pos , Vector rotate )
	{

		//���[���h�ϊ�
		D3DXMATRIXA16 matWorld = GetWorkdMatrix( pos , rotate );

		//=============================================================================
		//�����Ȃ���̕`��
		//=============================================================================
/*
		device->SetTransform( D3DTS_WORLD , &matWorld );	//���W�s��Z�b�g

		//�}�e���A���������[�v
		for( DWORD i=0; i<m_dwNumMaterials ;i++)
		{
			m_device->GetD3DDevice()->SetMaterial( &m_pMeshMaterials[i] );			//Direc3D�f�o�C�X�Ƀ}�e���A���ݒ肷��
			m_device->GetD3DDevice()->SetTexture(0,m_pMeshTextures[i]);				//Direc3D�f�o�C�X�Ƀe�N�X�`���ݒ肷��
			m_pMesh->DrawSubset(i);												//Direc3D�f�o�C�X�ɓo�^�������b�V����`�悷��
		}
*/
		//=============================================================================
		//�A�j���[�V�����̕`��
		//=============================================================================

		//�t���[���̍X�V
		anime.UpdateFrameMatrices( anime.g_pFrameRoot, &matWorld );		
		//�t���[���̕`��
		anime.DrawFrame( m_device->GetD3DDevice(), anime.g_pFrameRoot );

		return S_OK;
	}
	//�t���[���̃Z�b�g
	VOID  CXFile::SetAnimeFreem(FLOAT freem)
	{
		IF(anime.g_pAnimController!=NULL)
		{
			IF(freem >= this->GetAnime()->NumFreem)
			{
				freem = this->GetAnime()->NumFreem;
			}
			anime.g_pAnimController->SetTrackPosition(0,(DOUBLE)freem);
			anime.g_pAnimController->AdvanceTime( 0, NULL );
		}
	}
	//����
	LPTkyXFile CXFile::CreateXFile(LPTKY3D d){return new CXFile(d);}
	LPTkyXFile CXFile::CreateXFile(LPTKY3D d,wstring name)
	{
		LPTkyXFile x = CreateXFile(d);
		IF_FAILED( x->SetXFileW(name) )
		{
			x->DestroyOwnObject();
			return NULL;
		}
		return x;
	}
	//�����蔻��
	BOOL CXFile::CollisionLay( Vector laypos, Vector layvec , FLOAT* length , Vector xp , Vector xr  )
	{

		BOOL hit = false;

		FLOAT Length = 0.0f;

		Vector pos = laypos.Rotate( xp , xr.x , xr.y , xr.z) - xp;
		Vector vec = layvec.Rotate( Vector() , -xr.x , -xr.y , -xr.z );

		D3DXIntersect(      
			this->GetMesh(),			//���b�V���̎w��
			&pos,
			&vec,		//���C�̌���
			&hit,					//�����������ǂ���
			NULL,				//���������O�p�`�̃C���f�b�N�X
			NULL,				//�O�p�`�̍��WU
			NULL,				//�O�p�`�̍��WV
			&Length,			//����
			NULL,				//D3DXINTERSECTINFO �\���̂̔z��
			NULL				//���z��̃G���g���[��
		);

		//�������Ă�����
		IF(hit)
		{
			*length = Length;
			return TRUE;
		}

		return FALSE;
	}

//========== �V�������e ==========

//////////////////////////////////////////////////////////////////////////////////////////
//
//		X�t�@�C���ǂݍ��݊֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//�R���g���X�^
//---------------------------------------------//
XfileLoad::XfileLoad(LPTKY3D pinit3D,HLSLShader *a_pHLSL)
{

	XfileLoad::m_pD3D = pinit3D->GetD3D();
	XfileLoad::m_pD3DDevice = pinit3D->GetD3DDevice();

	XfileLoad::m_pMesh = NULL;
	XfileLoad::m_pHLSL = NULL;

	XfileLoad::m_pHLSL = a_pHLSL;

	//�A�j���[�V�����̏�����
	XfileLoad::anime.g_pHLSL = a_pHLSL;					
	XfileLoad::anime.g_bShowHelp = true;					
	XfileLoad::anime.g_pFrameRoot = NULL;
	XfileLoad::anime.g_pAnimController = NULL;

	XfileLoad::anime.g_SkinningMethod = METHOD::D3DINDEXED;			//���͉����Ȃ�//�C���f�b�N�X���̂��Ă��钸�_���̕`��(HLSL)

	XfileLoad::anime.g_pBoneMatrices = NULL;
	XfileLoad::anime.g_NumBoneMatricesMax = 0;

}
//---------------------------------------------//
//�f�X�g���N�^
//---------------------------------------------//
XfileLoad::~XfileLoad()
{
	XfileLoad::CleanXFile();
}
//---------------------------------------------//
//X�t�@�C���̂�݂���
//---------------------------------------------//
HRESULT XfileLoad::ReadXFile(LPWSTR filename){
	
	if(FAILED(D3DXLoadMeshFromXW(filename,					//X�t�@�C���̖��O		
								D3DXMESH_SYSTEMMEM,			//���b�V���̃�������ł̊i�[���@���w��
								XfileLoad::m_pD3DDevice,		//Direct3D�f�o�C�X�̃|�C���^
								NULL,						//�אڐ��f�[�^���L�^����|�C���^
								NULL,			//�}�e���A���f�[�^���L�^����|�C���^
								NULL,						//�G�t�F�N�g�C���X�^���X���L�^����|�C���^
								NULL,	//�}�e���A�������L�^����|�C���^
								&(XfileLoad::m_pMesh))))			//���b�V���f�[�^���L�^����|�C���^
								return E_FAIL;

	//�A�j���[�V�����̓ǂݍ���-----------------------------------------------------------------------------------------------------------------------------------------------------
	HRESULT hr;
    CAllocateHierarchy Alloc;
	Alloc.anime = &(XfileLoad::anime);

    // Load the mesh
    WCHAR* pLastSlash = filename;
	wstring wstr = pLastSlash;
	Alloc.anime->m_LastSlash.clear();

	//�h/�h�̃C���f�b�N�X�l��T��
	UINT l_LastSlashIndex = wstr.find_last_of(TEXT("/"),wstr.npos);
	IF(l_LastSlashIndex == wstring::npos)
	{
		//�h\\�h�̃C���f�b�N�X�l��T��
		l_LastSlashIndex = wstr.find_last_of(TEXT("\\"),wstr.npos);
	}

	//�����X���b�V�����Ȃ�������
	IF(l_LastSlashIndex == wstring::npos)
	{
		Alloc.anime->m_LastSlash = TEXT("");
	}
	else	//�X���b�V������������
	{
		//�X���b�V�����̕�����ǉ�
		_FOR_I(l_LastSlashIndex+1)
		{
			Alloc.anime->m_LastSlash += wstr.at(i);
		}
	}	

	//X�t�@�C�����烁�b�V���ƃ{�[���K�w�t���[�����擾����
    V_RETURN( D3DXLoadMeshHierarchyFromXW( 
		pLastSlash,										//�t�@�C����
		D3DXMESH_MANAGED,						//���b�V���̍쐬�I�v�V����
		XfileLoad::m_pD3DDevice,				//IDirect3DDevice9 �C���^�[�t�F�C�X�ւ̃|�C���^                     
		&Alloc,											//ID3DXAllocateHierarchy �C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,											//���[�U�[ �f�[�^�����[�h���邽�߂ɃA�v���P�[�V�������񋟂���C���^�[�t�F�C�X
		&XfileLoad::anime.g_pFrameRoot,		//���[�h���ꂽ�t���[���K�w�ւ̃|�C���^
		&XfileLoad::anime.g_pAnimController	//.x �t�@�C���̃A�j���[�V�����ɑΉ�����A�j���[�V���� �R���g���[���[�ւ̃|�C���^
		) );

	//�{�[���̃Z�b�g
    V_RETURN( XfileLoad::anime.SetupBoneMatrixPointers( XfileLoad::anime.g_pFrameRoot ) );
	//�t���[���̃Z�b�g
    V_RETURN( D3DXFrameCalculateBoundingSphere( XfileLoad::anime.g_pFrameRoot, &XfileLoad::anime.g_vObjectCenter, &XfileLoad::anime.g_fObjectRadius ) );

	UINT freem=0;

	if(XfileLoad::anime.g_pAnimController!=NULL)
	{
		UINT x = XfileLoad::anime.g_pAnimController->GetMaxNumAnimationSets();
		
		LPD3DXANIMATIONSET l_anime[1];
		XfileLoad::anime.g_pAnimController->GetTrackAnimationSet(0,l_anime);

		char str[260];
		sprintf_s(str,"%d,%d,%d,%d,%d,%d,%lf,     ,%d",
			XfileLoad::anime.g_pAnimController->GetMaxNumAnimationSets(),
			XfileLoad::anime.g_pAnimController->GetMaxNumAnimationOutputs(),
			XfileLoad::anime.g_pAnimController->GetMaxNumEvents(),
			XfileLoad::anime.g_pAnimController->GetMaxNumTracks(),
			XfileLoad::anime.g_pAnimController->GetTime(),
			XfileLoad::anime.g_pAnimController->GetNumAnimationSets(),
			XfileLoad::anime.g_pAnimController->GetPriorityBlend(),
			l_anime[0]->GetNumAnimations()
			);
		freem = XfileLoad::anime.g_pAnimController->GetMaxNumEvents()*2;
	}
	XfileLoad::anime.NumFreem = (FLOAT)freem;		//�A�j���[�V�����̃t���[���̐�
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	return S_OK;

}
//---------------------------------------------//
//�`��̏���
//---------------------------------------------//
void XfileLoad::pPrepara(float a_x,float a_y,float a_z,float a_rad,int a_pass)
{
	//�T���v�����O�̈�
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//��ʕ��̎擾
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	D3DXMATRIXA16 l_WorldMat,l_TransMat,l_RotateMat;						//���W�ϊ��s��
	D3DXMATRIXA16 l_mat,l_matCamera,l_matlight,l_matWorldInverse;	//�s��ϊ�,�J�����̈ʒu�s��
	
	D3DXVECTOR4 l_vLightVec = m_pHLSL->GetNowLightVec();				//���C�g�̂���

	D3DXVECTOR4	l_localcamera,l_locallight,l_locallightvec;				//���[�J�����W�n�ł̃J�����̈ʒu		

	// �ˉe��Ԃ���A�e�N�X�`���[�̋�Ԃɕϊ�����
	float fOffsetX = 0.5f + (0.5f / (float)MAP_SIZE);
	float fOffsetY = 0.5f + (0.5f / (float)MAP_SIZE);
	D3DXMATRIX l_ScaleBias(	0.5f,     0.0f,     0.0f,   0.0f,
							0.0f,    -0.5f,     0.0f,   0.0f,
							0.0f,     0.0f,     0.0f,	0.0f,
							fOffsetX, fOffsetY, 0.0f,   1.0f );

	//��ʂ̑傫���̃o�C�A�X
	fOffsetX = 0.5f + (0.5f / (float)SCREEN_W);
	fOffsetY = 0.5f + (0.5f / (float)SCREEN_H);
	D3DXMATRIX l_ScreenScaleBias(	0.5f,     0.0f,     0.0f,   0.0f,
									0.0f,    -0.5f,     0.0f,   0.0f,
									0.0f,     0.0f,     0.0f,	0.0f,
									fOffsetX, fOffsetY, 0.0f,   1.0f );

	//�������l�̐ݒ�-----------------------------------------------
	m_pHLSL->m_pEffect->SetFloat(m_pHLSL->m_hDecay_c0,m_pHLSL->GetNowDecayC0());
	m_pHLSL->m_pEffect->SetFloat(m_pHLSL->m_hDecay_c1,m_pHLSL->GetNowDecayC1());
	m_pHLSL->m_pEffect->SetFloat(m_pHLSL->m_hDecay_c2,m_pHLSL->GetNowDecayC2());
	//--------------------------------------------------------------

	//���W�Z�b�g---------------------------------------------------
	D3DXMatrixIdentity(&l_WorldMat);								//�s��̏�����
	D3DXMatrixIdentity(&l_TransMat);								//�s��̏�����
	D3DXMatrixIdentity(&l_RotateMat);								//�s��̏�����

	D3DXMatrixTranslation(&l_TransMat,a_x,a_y,a_z);					//���W�s��ړ�			
	D3DXMatrixRotationY(&l_RotateMat,a_rad);//timeGetTime()/500.0f);//y����]�̍s��ɂ���

	l_WorldMat = l_RotateMat * l_TransMat;							//�s��̍���
	//XfileLoad::m_pD3DDevice->SetTransform( D3DTS_WORLD , &l_WorldMat);	//���W�s��Z�b�g
	m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWorld,&l_WorldMat);
	//-------------------------------------------------------------
	

	//���[�J�����W�n�̃J�����̈ʒu-------------------------------------------------
	l_matCamera = l_WorldMat * m_pHLSL->GetCameraView();					//�J��������̕ϊ�
	D3DXMatrixInverse( &l_matCamera,NULL,&l_matCamera );					//�t�s������
	l_localcamera = D3DXVECTOR4(0,0,0,1);									//���_����
	D3DXVec4Transform( &l_localcamera,&l_localcamera,&l_matCamera );		//�J�����̈ʒu�Ɉړ�����
	l_localcamera.w = 0;
	m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvEyePos , &l_localcamera );	//�J�������W�x�N�g���̐ݒ�
	//-------------------------------------------------------------

	//�����̐ݒ�---------------------------------------------------
	D3DXMatrixInverse( &l_matWorldInverse , NULL , &l_WorldMat);			//l_WorldMat�̋t�s��̍쐬
	D3DXVec4Transform( &l_vLightVec , &l_vLightVec , &l_matWorldInverse );		//�����x�N�g���̃��[�J���ړ�
	D3DXVec3Normalize( (D3DXVECTOR3*)&l_vLightVec , (D3DXVECTOR3*)&l_vLightVec );	//���K��
	m_pHLSL->m_pEffect->SetVector(m_pHLSL->m_hvLight,&l_vLightVec);			//�����i���[�J���j�ݒ�
	//-------------------------------------------------------------

	//���_�ϊ��s��̍쐬(�r���[�Ǝˉe�̂�)-------------------------------------------
	l_mat =  m_pHLSL->GetCameraView() * m_pHLSL->GetCameraProj();
	m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmVP,&l_mat);			//�s��̐ݒ�
	//-------------------------------------------------------------

	switch(a_pass)
	{

		case D3DTKY_TEXTURE:
		case D3DTKY_WHITE:

			//���_�ϊ��s��̍쐬-------------------------------------------
			l_mat = l_WorldMat * m_pHLSL->GetCameraView() * m_pHLSL->GetCameraProj();
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWVP,&l_mat);			//�s��̐ݒ�
			//-------------------------------------------------------------

		break;

		case D3DTKY_DEPTH:

			//���C�g����݂��s��̍쐬-------------------------------------
			l_mat = m_pHLSL->GetNowLightView() * m_pHLSL->GetNowLightProj();		//���C�g�̈ʒu�ϊ��s��
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmLP,&l_mat);			//�s��̐ݒ�
			//-------------------------------------------------------------	

			//���C�g����݂����[���h�s��̍쐬-------------------------------------
			l_mat = l_WorldMat * l_mat;		//���C�g�̈ʒu�ϊ��s��
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWLP,&l_mat);			//�s��̐ݒ�
			//-------------------------------------------------------------


		break;

		case D3DTKY_SHADOW_SPOT:

			//���[�J�����W�n�̃��C�g�̈ʒu-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealLightPos , &m_pHLSL->GetNowLightPos() );	//�J�����̍��W�̐ݒ�
			//-------------------------------------------------------------

			//���[�J�����W�n�̃��C�g�̒��ӓ_�̈ʒu-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealLightVec , &m_pHLSL->GetNowLightVec() );//�J�����̒��ӓ_�̍��W�̐ݒ�
			//-------------------------------------------------------------

			//���[�J�����W�n�̃J�����̈ʒu-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealCameraPos , &m_pHLSL->GetCameraPos() );	//�J�����̍��W�̐ݒ�
			//-------------------------------------------------------------

			//���[�J�����W�n�̃J�����̒��ӓ_�̈ʒu-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealCameraVec , &m_pHLSL->GetCameraVec() );//�J�����̒��ӓ_�̍��W�̐ݒ�
			//-------------------------------------------------------------

		case D3DTKY_SHADOW:
		case D3DTKY_SAVE_SHADOW:
		case D3DTKY_SHADOW_PLUS:

			//���_�ϊ��s��̍쐬-------------------------------------------
			l_mat = l_WorldMat * m_pHLSL->GetCameraView() * m_pHLSL->GetCameraProj();
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWVP,&l_mat);			//�s��̐ݒ�
			//-------------------------------------------------------------

			//���_�ϊ��s��̍쐬�e�N�X�`����Ԃւ̎ˉe�s��-----------------
			l_mat = l_mat * l_ScreenScaleBias ;
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWVPB,&l_mat);			//�s��̐ݒ�
			//-------------------------------------------------------------

			//���C�g����݂��s��̍쐬-------------------------------------
			l_mat = l_WorldMat * m_pHLSL->GetNowLightView() * m_pHLSL->GetNowLightProj();		//���C�g�̈ʒu�ϊ��s��
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWLP,&l_mat);			//�s��̐ݒ�
			//-------------------------------------------------------------

			//���C�g����݂��e�N�X�`����Ԃւ̎ˉe�s��---------------------
			l_mat = l_mat * l_ScaleBias;
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWLPB,&l_mat);
			//-------------------------------------------------------------

			//�o�C�A�X�s��-----------------------------------------------
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmB,&l_ScaleBias);
			//-------------------------------------------------------------


		break;

	}

}
//---------------------------------------------//
//���ۂ̕`��
//---------------------------------------------//
void XfileLoad::pDrawMesh(D3DMATERIAL9* material,LPDIRECT3DTEXTURE9 texture,LPD3DXMESH mesh,INT pass,INT subset)		//�����A���ˌ��A�e�N�X�`���A���b�V�� , �p�X , �T�u�Z�b�g
{

		D3DXVECTOR4 l_vLightAmb = D3DXVECTOR4( 
			material[subset].Ambient.r ,
			material[subset].Ambient.g ,
			material[subset].Ambient.b ,
			material[subset].Ambient.a );

		D3DXVECTOR4 l_vLightDif = D3DXVECTOR4( 
			material[subset].Diffuse.r ,
			material[subset].Diffuse.g ,
			material[subset].Diffuse.b ,
			material[subset].Diffuse.a );

		//HLSL�Ƀe�N�X�`����ݒ肷��	
		m_pHLSL->m_pEffect->SetTexture(m_pHLSL->m_hTex,texture);
		m_pHLSL->m_pEffect->SetVector(m_pHLSL->m_hvLightAmb,&l_vLightAmb);
		m_pHLSL->m_pEffect->SetVector(m_pHLSL->m_hvLightDif,&l_vLightDif);

		//�V�F�[�_���J�n����
		m_pHLSL->m_pEffect->BeginPass(pass);

		mesh->DrawSubset(subset);												//Direc3D�f�o�C�X�ɓo�^�������b�V����`�悷��

		//�I������
		m_pHLSL->m_pEffect->EndPass();

}
//---------------------------------------------//
//���̃t���[���̃Z�b�g
//---------------------------------------------//
void  XfileLoad::SetAnimeFreem(INT freem)
{
	XfileLoad::anime.g_pAnimController->SetTrackPosition(0,freem);
	XfileLoad::anime.g_pAnimController->AdvanceTime( 0, NULL );
}
//---------------------------------------------//
//X�t�@�C��(�A�j��)�̏�������
//---------------------------------------------//
bool	XfileLoad::DrawAnimeXFile(float a_x,float a_y,float a_z,float a_rad,int a_pass)
{
	//loop	0		:�������[�v
	//		1�`��	:���̉񐔂������[�v	

	bool flag=false;
	int time=1;
/*
	XfileLoad::m_pD3DDevice->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL);
	XfileLoad::m_pD3DDevice->SetTextureStageState(0,	D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
	XfileLoad::m_pD3DDevice->SetTextureStageState(0,	D3DTSS_COLORARG1,	D3DTA_TEXTURE);
*/	
	

	//���W������
	D3DXMATRIXA16 TransMat,RotateMat,matWorld;						//���W�ϊ��s��	
	D3DXMatrixIdentity(&TransMat);									//�s��̏�����
	D3DXMatrixIdentity(&RotateMat);									//�s��̏�����
	D3DXMatrixIdentity(&matWorld);									//���v�s��̏�����
	//���W�Z�b�g
	D3DXMatrixTranslation(&TransMat,a_x,a_y,a_z);							//���W�s��ړ�			
	D3DXMatrixRotationY(&RotateMat,a_rad);							//y����]�̍s��ɂ���

	matWorld = RotateMat * TransMat;								//�s��̍���				 
//	XfileLoad::m_pD3DDevice->SetTransform( D3DTS_WORLD , &matWorld );	//���W�s��Z�b�g

	//�`�掞�̃p�X�̐ݒ�
	anime.m_pass = a_pass;			

	//�`��̏���
	pPrepara(a_x,a_y,a_z,a_rad,a_pass);

	//�t���[���̍X�V
	XfileLoad::anime.UpdateFrameMatrices( XfileLoad::anime.g_pFrameRoot, &matWorld );		

	//�t���[���̕`��
	XfileLoad::anime.DrawFrame( XfileLoad::m_pD3DDevice, XfileLoad::anime.g_pFrameRoot );



	return flag;

}
//---------------------------------------------//
//�J������
//---------------------------------------------//
void	XfileLoad::CleanXFile(){

	assert(XfileLoad::m_pMesh);
	if(XfileLoad::m_pMesh != NULL) SAFE_RELEASE(XfileLoad::m_pMesh);

    CAllocateHierarchy Alloc;
	D3DXFrameDestroy( XfileLoad::anime.g_pFrameRoot, &Alloc );
    SAFE_RELEASE( XfileLoad::anime.g_pAnimController );

}

//////////////////////////////////////////////////////////////////////////////////////////
//
//		X�t�@�C���A�j���[�V�����ǂݍ��݊֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
	D3DTKYXANIMETION::D3DTKYXANIMETION()
	{
		D3DTKYXANIMETION::m_maxloop=0;
		D3DTKYXANIMETION::m_nowloop=0;
		D3DTKYXANIMETION::m_time=0;
		D3DTKYXANIMETION::m_nowtime=0;
	}
	D3DTKYXANIMETION::D3DTKYXANIMETION(const D3DTKYXANIMETION& anime)
	{
		*this = anime;
		this->m_nowloop = 0;
	}
	D3DTKYXANIMETION::D3DTKYXANIMETION(XfileLoad* model,INT time)//��{���,�ړ�����
	{
		D3DTKYXANIMETION::m_model = model;
		D3DTKYXANIMETION::m_maxloop=(int)model->GetMaxFreem();
		D3DTKYXANIMETION::m_nowloop=0;
		D3DTKYXANIMETION::m_time=time;
		D3DTKYXANIMETION::m_nowtime=0;
	}
	D3DTKYXANIMETION::~D3DTKYXANIMETION()
	{
	}
	//�`��ɕK�v�Ȃ���
	bool D3DTKYXANIMETION::NextModel()
	{	
		bool flag = false;		
		if(D3DTKYXANIMETION::m_model->GetAnimetionClass().g_pAnimController != NULL )
		{
			//�o�ߎ��Ԃ̎擾
			if( D3DTKYXANIMETION::m_nowtime<=0) D3DTKYXANIMETION::m_nowtime = timeGetTime();
			DWORD sub = timeGetTime() - D3DTKYXANIMETION::m_nowtime;
			if( sub > (D3DTKYXANIMETION::m_time/D3DTKYXANIMETION::m_maxloop) &&  D3DTKYXANIMETION::m_time!=0)
			{
				//����̈ړ��񐔂����߂�
				INT loop = (sub / (D3DTKYXANIMETION::m_time/D3DTKYXANIMETION::m_maxloop)  );

				//���Ԃ̏�����
				D3DTKYXANIMETION::m_nowtime = timeGetTime();
				//���f���̑J��
				if( ( D3DTKYXANIMETION::m_nowloop+=loop  ) >= D3DTKYXANIMETION::m_maxloop )
				{
					D3DTKYXANIMETION::m_nowloop = 0;
					D3DTKYXANIMETION::m_nowtime = 0;
					flag = true;
				}
			}
		}
		return flag;
	}
	void D3DTKYXANIMETION::DrawModel(D3DXVECTOR3 pos,FLOAT rad,INT pass)
	{
		if(D3DTKYXANIMETION::m_model->GetAnimetionClass().g_pAnimController != NULL )D3DTKYXANIMETION::m_model->SetAnimeFreem(D3DTKYXANIMETION::m_nowloop);
		D3DTKYXANIMETION::m_model->DrawAnimeXFile(
			pos.x,pos.y,pos.z,rad,pass);
	}
	//�f�[�^�擾�n
	LPD3DXMESH D3DTKYXANIMETION::GetModelMesh()
	{
		return D3DTKYXANIMETION::m_model->GetMesh();
	}