
#include "TKYXFILE.h"

//========== 新しい内容 ==========
	//コンストラクタ
	CXFile::CXFile(LPTKY3D d)
	{
		m_device = d;

		//メッシュ情報だけ
		m_pMeshMaterials = NULL;		//マテリアルを格納する
		m_pMeshTextures = NULL;		//テクスチャを格納する
		m_dwNumMaterials = NULL;		//マテリアルの数を格納する
		m_pMesh = NULL;

		//アニメ情報の設定
		anime.g_bShowHelp = true;					
		anime.g_pFrameRoot = NULL;
		anime.g_pAnimController = NULL;
		anime.g_SkinningMethod = METHOD::D3DNONINDEXED;			//今は何もなし//インデックス情報のついている頂点情報の描画(HLSL)
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
		//メッシュ情報を消す
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

		//アニメーション情報を消す
		CAllocateHierarchy Alloc;
		D3DXFrameDestroy( anime.g_pFrameRoot, &Alloc );
		SAFE_RELEASE( anime.g_pAnimController );
	}
	//Xファイルの設定
	HRESULT CXFile::SetXFileW(wstring name)
	{

		LPD3DXBUFFER pD3DMtrlBuffer;							//一時的なマテリアルデータを記録する
		HRESULT h;

		if(FAILED(h = D3DXLoadMeshFromXW(name.c_str(),					//Xファイルの名前		
									D3DXMESH_SYSTEMMEM,			//メッシュのメモリ上での格納方法を指定
									m_device->GetD3DDevice(),		//Direct3Dデバイスのポインタ
									NULL,						//隣接性データを記録するポインタ
									NULL,//&pD3DMtrlBuffer,			//マテリアルデータを記録するポインタ
									NULL,						//エフェクトインスタンスを記録するポインタ
									&(m_dwNumMaterials),	//マテリアル数を記録するポインタ
									&(m_pMesh))))			//メッシュデータを記録するポインタ
		{

			WCHAR strMesh[MAX_PATH];							//階層を一つ戻るための文字列(完成版)
			StringCchCopyW( strMesh , MAX_PATH , TEXT("..\\"));	//階層も一つ戻るための文字列の代入
			StringCchCatW(strMesh , MAX_PATH , name.c_str());		//メッシュファイル名を連結する

			if(FAILED(h = D3DXLoadMeshFromXW(strMesh,
										D3DXMESH_SYSTEMMEM,
										this->m_device->GetD3DDevice(),
										NULL,
										&pD3DMtrlBuffer,
										NULL,
										&(m_dwNumMaterials),
										&(m_pMesh))))
			{
		
				MessageBox(NULL,TEXT("X　ファイルが見つかりません"),TEXT("Meshes.exe"),MB_OK);
				return E_FAIL;

			}

		}

		//=======================================================================================
		//Xファイルのあるフォルダパスの取得
		//=======================================================================================

		//スラッシュ
		wstring LastSlash;
		//”/”のインデックス値を探す
		UINT l_LastSlashIndex = name.find_last_of(TEXT("/"),name.npos);
		IF(l_LastSlashIndex == wstring::npos)
		{
			//”\\”のインデックス値を探す
			l_LastSlashIndex = name.find_last_of(TEXT("\\"),name.npos);
		}

		//もしスラッシュがなかったら
		IF(l_LastSlashIndex == wstring::npos)
		{
			LastSlash = TEXT("");
		}
		else	//スラッシュがあったら
		{
			//スラッシュ部の文字を追加
			_FOR_I(l_LastSlashIndex+1)
			{
				LastSlash += name.at(i);
			}
		}	

/*
		//=======================================================================================
		//=======================================================================================

		//=======================================================================================
		//動かないXFILEの取得
		//=======================================================================================

		D3DXMATERIAL *d3dxMaterials = (D3DXMATERIAL*)pD3DMtrlBuffer->GetBufferPointer();	//データ編集用のアドレスを取り出す(データが入ってるやつ)

		m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];								//Xファイルのマテリアルの動的確保
		if(m_pMeshMaterials == NULL) return E_OUTOFMEMORY;									//エラー処理

		m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];							//Xファイルのテクスチャの動的確保
		if(m_pMeshTextures == NULL) return E_OUTOFMEMORY;									//エラー処理

		//マテリアルの数だけループ
		for( DWORD i=0; i<m_dwNumMaterials ; i++)
		{

			m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;									//マテリアルを格納用変数にコピーする

			m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;						//Xファイルに環境光が存在しないので反射光を代入する

			m_pMeshTextures[i] = NULL;														//テクスチャの初期化

			//テクスチャ文字があったら
			if(d3dxMaterials[i].pTextureFilename != NULL && strlen(d3dxMaterials[i].pTextureFilename) > 0)
			{

				//テクスチャの名前
				WCHAR wtex[MAX_PATH];
				MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,d3dxMaterials[i].pTextureFilename,MAX_PATH,wtex,MAX_PATH);
				wstring tmpFilename = wtex;
				//スラッシュの追加
				tmpFilename = LastSlash + tmpFilename;


				if(FAILED(D3DXCreateTextureFromFileW(m_device->GetD3DDevice(),								//Direct3Dデバイス
													tmpFilename.c_str(),										//テクスチャファイル名
													&(m_pMeshTextures[i]))))						//読み込んだテクスチャを返すポインタ
				{

					WCHAR strTexture[MAX_PATH];															//階層を一つ戻るための文字列(完成版)
					StringCchCopyW( strTexture , MAX_PATH , TEXT("..\\"));								//階層も一つ戻るための文字列の代入
					StringCchCatW(strTexture , MAX_PATH , tmpFilename.c_str());		//テクスチャファイル名を連結する

					if(FAILED(D3DXCreateTextureFromFileW(m_device->GetD3DDevice(),							//Direct3Dデバイス
														strTexture,										//テクスチャファイル名
														&(m_pMeshTextures[i]))))					//読み込んだテクスチャを返すポインタ
					{

						MessageBox(NULL,TEXT("テクスチャファイルが見つかりません"),TEXT("ERR"),MB_OK);

					}
				}
			}
		}


		pD3DMtrlBuffer->Release();					//一時ファイルなので開放する

		//=======================================================================================
		//=======================================================================================
*/
		//=======================================================================================
		//アニメーションXファイルの取得
		//=======================================================================================

		HRESULT hr;
		CAllocateHierarchy Alloc;
		Alloc.anime = &(this->anime);
		Alloc.anime->m_LastSlash.clear();

		Alloc.anime->m_LastSlash = LastSlash;

		//Xファイルからメッシュとボーン階層フレームを取得する
		IF_FAILED(hr = D3DXLoadMeshHierarchyFromXW( 
			name.c_str(),									//ファイル名
			D3DXMESH_MANAGED,						//メッシュの作成オプション
			m_device->GetD3DDevice(),				//IDirect3DDevice9 インターフェイスへのポインタ                     
			&(Alloc),										//ID3DXAllocateHierarchy インターフェイスへのポインタ
			NULL,											//ユーザー データをロードするためにアプリケーションが提供するインターフェイス
			&(anime.g_pFrameRoot),					//ロードされたフレーム階層へのポインタ
			&(anime.g_pAnimController)				//.x ファイルのアニメーションに対応するアニメーション コントローラーへのポインタ
			) )
		{
			MessageBoxA( NULL,"アニメ読み込めません","err",NULL );
			return hr;
		}

		//ボーンのセット
		V_RETURN( anime.SetupBoneMatrixPointers( anime.g_pFrameRoot ) );
		//フレームのセット
		V_RETURN( D3DXFrameCalculateBoundingSphere( anime.g_pFrameRoot, &anime.g_vObjectCenter, &anime.g_fObjectRadius ) );

		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		UINT freem=0;
		//アニメがセットされていたら
		if(anime.g_pAnimController!=NULL)
		{

			UINT x = anime.g_pAnimController->GetMaxNumAnimationSets();
			LPD3DXANIMATIONSET l_anime[1];

			anime.g_pAnimController->GetTrackAnimationSet(0,l_anime);
			freem = 58;

			//デバッグ用
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

		anime.NumFreem = (FLOAT)freem;		//アニメーションのフレームの数
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		//=======================================================================================
		//=======================================================================================


		return S_OK;
	}
	//表示
	HRESULT CXFile::DrawMesh( Vector pos , Vector rotate )
	{

		//ワールド変換
		D3DXMATRIXA16 matWorld = GetWorkdMatrix( pos , rotate );

		//=============================================================================
		//動かないやつの描画
		//=============================================================================
/*
		device->SetTransform( D3DTS_WORLD , &matWorld );	//座標行列セット

		//マテリアルだけループ
		for( DWORD i=0; i<m_dwNumMaterials ;i++)
		{
			m_device->GetD3DDevice()->SetMaterial( &m_pMeshMaterials[i] );			//Direc3Dデバイスにマテリアル設定する
			m_device->GetD3DDevice()->SetTexture(0,m_pMeshTextures[i]);				//Direc3Dデバイスにテクスチャ設定する
			m_pMesh->DrawSubset(i);												//Direc3Dデバイスに登録したメッシュを描画する
		}
*/
		//=============================================================================
		//アニメーションの描画
		//=============================================================================

		//フレームの更新
		anime.UpdateFrameMatrices( anime.g_pFrameRoot, &matWorld );		
		//フレームの描画
		anime.DrawFrame( m_device->GetD3DDevice(), anime.g_pFrameRoot );

		return S_OK;
	}
	//フレームのセット
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
	//生成
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
	//たたり判定
	BOOL CXFile::CollisionLay( Vector laypos, Vector layvec , FLOAT* length , Vector xp , Vector xr  )
	{

		BOOL hit = false;

		FLOAT Length = 0.0f;

		Vector pos = laypos.Rotate( xp , xr.x , xr.y , xr.z) - xp;
		Vector vec = layvec.Rotate( Vector() , -xr.x , -xr.y , -xr.z );

		D3DXIntersect(      
			this->GetMesh(),			//メッシュの指定
			&pos,
			&vec,		//レイの向き
			&hit,					//あたったかどうか
			NULL,				//あたった三角形のインデックス
			NULL,				//三角形の座標U
			NULL,				//三角形の座標V
			&Length,			//距離
			NULL,				//D3DXINTERSECTINFO 構造体の配列
			NULL				//↑配列のエントリー数
		);

		//あたっていたら
		IF(hit)
		{
			*length = Length;
			return TRUE;
		}

		return FALSE;
	}

//========== 新しい内容 ==========

//////////////////////////////////////////////////////////////////////////////////////////
//
//		Xファイル読み込み関係
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
XfileLoad::XfileLoad(LPTKY3D pinit3D,HLSLShader *a_pHLSL)
{

	XfileLoad::m_pD3D = pinit3D->GetD3D();
	XfileLoad::m_pD3DDevice = pinit3D->GetD3DDevice();

	XfileLoad::m_pMesh = NULL;
	XfileLoad::m_pHLSL = NULL;

	XfileLoad::m_pHLSL = a_pHLSL;

	//アニメーションの初期化
	XfileLoad::anime.g_pHLSL = a_pHLSL;					
	XfileLoad::anime.g_bShowHelp = true;					
	XfileLoad::anime.g_pFrameRoot = NULL;
	XfileLoad::anime.g_pAnimController = NULL;

	XfileLoad::anime.g_SkinningMethod = METHOD::D3DINDEXED;			//今は何もなし//インデックス情報のついている頂点情報の描画(HLSL)

	XfileLoad::anime.g_pBoneMatrices = NULL;
	XfileLoad::anime.g_NumBoneMatricesMax = 0;

}
//---------------------------------------------//
//デストラクタ
//---------------------------------------------//
XfileLoad::~XfileLoad()
{
	XfileLoad::CleanXFile();
}
//---------------------------------------------//
//Xファイルのよみこみ
//---------------------------------------------//
HRESULT XfileLoad::ReadXFile(LPWSTR filename){
	
	if(FAILED(D3DXLoadMeshFromXW(filename,					//Xファイルの名前		
								D3DXMESH_SYSTEMMEM,			//メッシュのメモリ上での格納方法を指定
								XfileLoad::m_pD3DDevice,		//Direct3Dデバイスのポインタ
								NULL,						//隣接性データを記録するポインタ
								NULL,			//マテリアルデータを記録するポインタ
								NULL,						//エフェクトインスタンスを記録するポインタ
								NULL,	//マテリアル数を記録するポインタ
								&(XfileLoad::m_pMesh))))			//メッシュデータを記録するポインタ
								return E_FAIL;

	//アニメーションの読み込み-----------------------------------------------------------------------------------------------------------------------------------------------------
	HRESULT hr;
    CAllocateHierarchy Alloc;
	Alloc.anime = &(XfileLoad::anime);

    // Load the mesh
    WCHAR* pLastSlash = filename;
	wstring wstr = pLastSlash;
	Alloc.anime->m_LastSlash.clear();

	//”/”のインデックス値を探す
	UINT l_LastSlashIndex = wstr.find_last_of(TEXT("/"),wstr.npos);
	IF(l_LastSlashIndex == wstring::npos)
	{
		//”\\”のインデックス値を探す
		l_LastSlashIndex = wstr.find_last_of(TEXT("\\"),wstr.npos);
	}

	//もしスラッシュがなかったら
	IF(l_LastSlashIndex == wstring::npos)
	{
		Alloc.anime->m_LastSlash = TEXT("");
	}
	else	//スラッシュがあったら
	{
		//スラッシュ部の文字を追加
		_FOR_I(l_LastSlashIndex+1)
		{
			Alloc.anime->m_LastSlash += wstr.at(i);
		}
	}	

	//Xファイルからメッシュとボーン階層フレームを取得する
    V_RETURN( D3DXLoadMeshHierarchyFromXW( 
		pLastSlash,										//ファイル名
		D3DXMESH_MANAGED,						//メッシュの作成オプション
		XfileLoad::m_pD3DDevice,				//IDirect3DDevice9 インターフェイスへのポインタ                     
		&Alloc,											//ID3DXAllocateHierarchy インターフェイスへのポインタ
		NULL,											//ユーザー データをロードするためにアプリケーションが提供するインターフェイス
		&XfileLoad::anime.g_pFrameRoot,		//ロードされたフレーム階層へのポインタ
		&XfileLoad::anime.g_pAnimController	//.x ファイルのアニメーションに対応するアニメーション コントローラーへのポインタ
		) );

	//ボーンのセット
    V_RETURN( XfileLoad::anime.SetupBoneMatrixPointers( XfileLoad::anime.g_pFrameRoot ) );
	//フレームのセット
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
	XfileLoad::anime.NumFreem = (FLOAT)freem;		//アニメーションのフレームの数
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	return S_OK;

}
//---------------------------------------------//
//描画の準備
//---------------------------------------------//
void XfileLoad::pPrepara(float a_x,float a_y,float a_z,float a_rad,int a_pass)
{
	//サンプリング領域
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//画面幅の取得
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	D3DXMATRIXA16 l_WorldMat,l_TransMat,l_RotateMat;						//座標変換行列
	D3DXMATRIXA16 l_mat,l_matCamera,l_matlight,l_matWorldInverse;	//行列変換,カメラの位置行列
	
	D3DXVECTOR4 l_vLightVec = m_pHLSL->GetNowLightVec();				//ライトのいち

	D3DXVECTOR4	l_localcamera,l_locallight,l_locallightvec;				//ローカル座標系でのカメラの位置		

	// 射影空間から、テクスチャーの空間に変換する
	float fOffsetX = 0.5f + (0.5f / (float)MAP_SIZE);
	float fOffsetY = 0.5f + (0.5f / (float)MAP_SIZE);
	D3DXMATRIX l_ScaleBias(	0.5f,     0.0f,     0.0f,   0.0f,
							0.0f,    -0.5f,     0.0f,   0.0f,
							0.0f,     0.0f,     0.0f,	0.0f,
							fOffsetX, fOffsetY, 0.0f,   1.0f );

	//画面の大きさのバイアス
	fOffsetX = 0.5f + (0.5f / (float)SCREEN_W);
	fOffsetY = 0.5f + (0.5f / (float)SCREEN_H);
	D3DXMATRIX l_ScreenScaleBias(	0.5f,     0.0f,     0.0f,   0.0f,
									0.0f,    -0.5f,     0.0f,   0.0f,
									0.0f,     0.0f,     0.0f,	0.0f,
									fOffsetX, fOffsetY, 0.0f,   1.0f );

	//減衰率値の設定-----------------------------------------------
	m_pHLSL->m_pEffect->SetFloat(m_pHLSL->m_hDecay_c0,m_pHLSL->GetNowDecayC0());
	m_pHLSL->m_pEffect->SetFloat(m_pHLSL->m_hDecay_c1,m_pHLSL->GetNowDecayC1());
	m_pHLSL->m_pEffect->SetFloat(m_pHLSL->m_hDecay_c2,m_pHLSL->GetNowDecayC2());
	//--------------------------------------------------------------

	//座標セット---------------------------------------------------
	D3DXMatrixIdentity(&l_WorldMat);								//行列の初期化
	D3DXMatrixIdentity(&l_TransMat);								//行列の初期化
	D3DXMatrixIdentity(&l_RotateMat);								//行列の初期化

	D3DXMatrixTranslation(&l_TransMat,a_x,a_y,a_z);					//座標行列移動			
	D3DXMatrixRotationY(&l_RotateMat,a_rad);//timeGetTime()/500.0f);//y軸回転の行列にする

	l_WorldMat = l_RotateMat * l_TransMat;							//行列の合体
	//XfileLoad::m_pD3DDevice->SetTransform( D3DTS_WORLD , &l_WorldMat);	//座標行列セット
	m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWorld,&l_WorldMat);
	//-------------------------------------------------------------
	

	//ローカル座標系のカメラの位置-------------------------------------------------
	l_matCamera = l_WorldMat * m_pHLSL->GetCameraView();					//カメラからの変換
	D3DXMatrixInverse( &l_matCamera,NULL,&l_matCamera );					//逆行列を取り
	l_localcamera = D3DXVECTOR4(0,0,0,1);									//原点から
	D3DXVec4Transform( &l_localcamera,&l_localcamera,&l_matCamera );		//カメラの位置に移動する
	l_localcamera.w = 0;
	m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvEyePos , &l_localcamera );	//カメラ座標ベクトルの設定
	//-------------------------------------------------------------

	//光源の設定---------------------------------------------------
	D3DXMatrixInverse( &l_matWorldInverse , NULL , &l_WorldMat);			//l_WorldMatの逆行列の作成
	D3DXVec4Transform( &l_vLightVec , &l_vLightVec , &l_matWorldInverse );		//光源ベクトルのローカル移動
	D3DXVec3Normalize( (D3DXVECTOR3*)&l_vLightVec , (D3DXVECTOR3*)&l_vLightVec );	//正規化
	m_pHLSL->m_pEffect->SetVector(m_pHLSL->m_hvLight,&l_vLightVec);			//光源（ローカル）設定
	//-------------------------------------------------------------

	//視点変換行列の作成(ビューと射影のみ)-------------------------------------------
	l_mat =  m_pHLSL->GetCameraView() * m_pHLSL->GetCameraProj();
	m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmVP,&l_mat);			//行列の設定
	//-------------------------------------------------------------

	switch(a_pass)
	{

		case D3DTKY_TEXTURE:
		case D3DTKY_WHITE:

			//視点変換行列の作成-------------------------------------------
			l_mat = l_WorldMat * m_pHLSL->GetCameraView() * m_pHLSL->GetCameraProj();
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWVP,&l_mat);			//行列の設定
			//-------------------------------------------------------------

		break;

		case D3DTKY_DEPTH:

			//ライトからみた行列の作成-------------------------------------
			l_mat = m_pHLSL->GetNowLightView() * m_pHLSL->GetNowLightProj();		//ライトの位置変換行列
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmLP,&l_mat);			//行列の設定
			//-------------------------------------------------------------	

			//ライトからみたワールド行列の作成-------------------------------------
			l_mat = l_WorldMat * l_mat;		//ライトの位置変換行列
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWLP,&l_mat);			//行列の設定
			//-------------------------------------------------------------


		break;

		case D3DTKY_SHADOW_SPOT:

			//ローカル座標系のライトの位置-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealLightPos , &m_pHLSL->GetNowLightPos() );	//カメラの座標の設定
			//-------------------------------------------------------------

			//ローカル座標系のライトの注意点の位置-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealLightVec , &m_pHLSL->GetNowLightVec() );//カメラの注意点の座標の設定
			//-------------------------------------------------------------

			//ローカル座標系のカメラの位置-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealCameraPos , &m_pHLSL->GetCameraPos() );	//カメラの座標の設定
			//-------------------------------------------------------------

			//ローカル座標系のカメラの注意点の位置-------------------------------------------------
			m_pHLSL->m_pEffect->SetVector( m_pHLSL->m_hvRealCameraVec , &m_pHLSL->GetCameraVec() );//カメラの注意点の座標の設定
			//-------------------------------------------------------------

		case D3DTKY_SHADOW:
		case D3DTKY_SAVE_SHADOW:
		case D3DTKY_SHADOW_PLUS:

			//視点変換行列の作成-------------------------------------------
			l_mat = l_WorldMat * m_pHLSL->GetCameraView() * m_pHLSL->GetCameraProj();
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWVP,&l_mat);			//行列の設定
			//-------------------------------------------------------------

			//視点変換行列の作成テクスチャ空間への射影行列-----------------
			l_mat = l_mat * l_ScreenScaleBias ;
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWVPB,&l_mat);			//行列の設定
			//-------------------------------------------------------------

			//ライトからみた行列の作成-------------------------------------
			l_mat = l_WorldMat * m_pHLSL->GetNowLightView() * m_pHLSL->GetNowLightProj();		//ライトの位置変換行列
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWLP,&l_mat);			//行列の設定
			//-------------------------------------------------------------

			//ライトからみたテクスチャ空間への射影行列---------------------
			l_mat = l_mat * l_ScaleBias;
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmWLPB,&l_mat);
			//-------------------------------------------------------------

			//バイアス行列-----------------------------------------------
			m_pHLSL->m_pEffect->SetMatrix(m_pHLSL->m_hmB,&l_ScaleBias);
			//-------------------------------------------------------------


		break;

	}

}
//---------------------------------------------//
//実際の描画
//---------------------------------------------//
void XfileLoad::pDrawMesh(D3DMATERIAL9* material,LPDIRECT3DTEXTURE9 texture,LPD3DXMESH mesh,INT pass,INT subset)		//環境光、反射光、テクスチャ、メッシュ , パス , サブセット
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

		//HLSLにテクスチャを設定する	
		m_pHLSL->m_pEffect->SetTexture(m_pHLSL->m_hTex,texture);
		m_pHLSL->m_pEffect->SetVector(m_pHLSL->m_hvLightAmb,&l_vLightAmb);
		m_pHLSL->m_pEffect->SetVector(m_pHLSL->m_hvLightDif,&l_vLightDif);

		//シェーダを開始する
		m_pHLSL->m_pEffect->BeginPass(pass);

		mesh->DrawSubset(subset);												//Direc3Dデバイスに登録したメッシュを描画する

		//終了する
		m_pHLSL->m_pEffect->EndPass();

}
//---------------------------------------------//
//今のフレームのセット
//---------------------------------------------//
void  XfileLoad::SetAnimeFreem(INT freem)
{
	XfileLoad::anime.g_pAnimController->SetTrackPosition(0,freem);
	XfileLoad::anime.g_pAnimController->AdvanceTime( 0, NULL );
}
//---------------------------------------------//
//Xファイル(アニメ)の書き込み
//---------------------------------------------//
bool	XfileLoad::DrawAnimeXFile(float a_x,float a_y,float a_z,float a_rad,int a_pass)
{
	//loop	0		:無限ループ
	//		1～∞	:その回数だけループ	

	bool flag=false;
	int time=1;
/*
	XfileLoad::m_pD3DDevice->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL);
	XfileLoad::m_pD3DDevice->SetTextureStageState(0,	D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
	XfileLoad::m_pD3DDevice->SetTextureStageState(0,	D3DTSS_COLORARG1,	D3DTA_TEXTURE);
*/	
	

	//座標初期化
	D3DXMATRIXA16 TransMat,RotateMat,matWorld;						//座標変換行列	
	D3DXMatrixIdentity(&TransMat);									//行列の初期化
	D3DXMatrixIdentity(&RotateMat);									//行列の初期化
	D3DXMatrixIdentity(&matWorld);									//合計行列の初期化
	//座標セット
	D3DXMatrixTranslation(&TransMat,a_x,a_y,a_z);							//座標行列移動			
	D3DXMatrixRotationY(&RotateMat,a_rad);							//y軸回転の行列にする

	matWorld = RotateMat * TransMat;								//行列の合体				 
//	XfileLoad::m_pD3DDevice->SetTransform( D3DTS_WORLD , &matWorld );	//座標行列セット

	//描画時のパスの設定
	anime.m_pass = a_pass;			

	//描画の準備
	pPrepara(a_x,a_y,a_z,a_rad,a_pass);

	//フレームの更新
	XfileLoad::anime.UpdateFrameMatrices( XfileLoad::anime.g_pFrameRoot, &matWorld );		

	//フレームの描画
	XfileLoad::anime.DrawFrame( XfileLoad::m_pD3DDevice, XfileLoad::anime.g_pFrameRoot );



	return flag;

}
//---------------------------------------------//
//開放処理
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
//		Xファイルアニメーション読み込み関係
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
	D3DTKYXANIMETION::D3DTKYXANIMETION(XfileLoad* model,INT time)//基本情報,移動時間
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
	//描画に必要なもの
	bool D3DTKYXANIMETION::NextModel()
	{	
		bool flag = false;		
		if(D3DTKYXANIMETION::m_model->GetAnimetionClass().g_pAnimController != NULL )
		{
			//経過時間の取得
			if( D3DTKYXANIMETION::m_nowtime<=0) D3DTKYXANIMETION::m_nowtime = timeGetTime();
			DWORD sub = timeGetTime() - D3DTKYXANIMETION::m_nowtime;
			if( sub > (D3DTKYXANIMETION::m_time/D3DTKYXANIMETION::m_maxloop) &&  D3DTKYXANIMETION::m_time!=0)
			{
				//今回の移動回数を決める
				INT loop = (sub / (D3DTKYXANIMETION::m_time/D3DTKYXANIMETION::m_maxloop)  );

				//時間の初期化
				D3DTKYXANIMETION::m_nowtime = timeGetTime();
				//モデルの遷移
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
	//データ取得系
	LPD3DXMESH D3DTKYXANIMETION::GetModelMesh()
	{
		return D3DTKYXANIMETION::m_model->GetMesh();
	}