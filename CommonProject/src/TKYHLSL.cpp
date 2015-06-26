
#include "TKYHLSL.h"


//////////////////////////////////////////////////////////////////////////////////////////
//
//		HLSLシェーダ関係
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
HLSLShader::HLSLShader()
{

	HLSLShader::m_pEffect = NULL;

	//ハンドル関係
	m_hmWorld		= NULL;		//ワールド行列
	m_hmVP			= NULL;		//ビュー射影行列
	m_hmWVP			= NULL;		//ワールド射影行列
	m_hmWLP			= NULL;		//ライトからみた射影行列
	m_hmWLPB		= NULL;		//バイアス適用後(テクスチャ座標)射影行列

	m_hTex			= NULL;		//ディティールテクスチャのハンドル
	m_hShadowTex	= NULL;		//シャドウテクスチャのハンドル

	m_hvLight		= NULL;		//光源ベクトルのハンドル
	m_hvLightAmb	= NULL;		//環境光のハンドル
	m_hvLightDif	= NULL;		//拡散反射光のハンドル
	m_hvEyePos		= NULL;		//カメラの位置ベクトル

	m_hsBaias		= NULL;		//バイアス補正値

	m_pEffect		= NULL;
}
//---------------------------------------------//
//HLSL読み込み
//---------------------------------------------//
HRESULT HLSLShader::InitHLSL(LPCTSTR filename,LPDIRECT3DDEVICE9 pD3DDev)
{

	m_pD3DDevice = pD3DDev;

	//シェーダの読み込み
	LPD3DXBUFFER pErr = NULL;
	HRESULT hr = S_OK;

	if( FAILED( hr = D3DXCreateEffectFromFile(
		pD3DDev,
		filename,
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErr ) ) )
	{

		HRESULTCheck(hr);

		LPSTR s = (LPSTR)pErr->GetBufferPointer();

		LPTCH t[MAX_PATH*10];

		//アスキーからユニコード
		MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			s,
			MAX_PATH*2,
			(LPTSTR)t,
			MAX_PATH*2);

		MessageBox(NULL,(LPCTSTR)t,TEXT("失敗"),MB_OK);	



	}else{

		m_hTechnique		=	m_pEffect->GetTechniqueByName("TShader");

		m_hmWorld			=	m_pEffect->GetParameterByName(NULL,"g_mWorld");
		m_hmVP				=	m_pEffect->GetParameterByName(NULL,"g_mVP");
		m_hmWVP				=	m_pEffect->GetParameterByName(NULL,"g_mWVP");
		m_hmWVPB			=	m_pEffect->GetParameterByName(NULL,"g_mWVPB");
		m_hmWLP				=	m_pEffect->GetParameterByName(NULL,"g_mWLP");
		m_hmLP				=	m_pEffect->GetParameterByName(NULL,"g_mLP");
		m_hmWLPB			=	m_pEffect->GetParameterByName(NULL,"g_mWLPB");
		m_hmB			=	m_pEffect->GetParameterByName(NULL,"g_mB");

		m_hmWorldMatrixArray = m_pEffect->GetParameterByName(NULL,"g_mWorldMatrixArray");
		m_hiNumBones = m_pEffect->GetParameterByName(NULL,"g_NumBones");	
		
		m_hTex				=	m_pEffect->GetParameterByName(NULL,"g_Tex");
		m_hShadowTex		=	m_pEffect->GetParameterByName(NULL,"g_ShadowTex");
		m_hRenderTex		=	m_pEffect->GetParameterByName(NULL,"g_RenderTex");

		m_hvLight			=	m_pEffect->GetParameterByName(NULL,"g_vLight");
		m_hvRealLightPos	=	m_pEffect->GetParameterByName(NULL,"g_vRealLightPos");
		m_hvRealLightVec	=	m_pEffect->GetParameterByName(NULL,"g_vRealLightVec");
		m_hvRealCameraPos	=	m_pEffect->GetParameterByName(NULL,"g_vRealCameraPos");
		m_hvRealCameraVec	=	m_pEffect->GetParameterByName(NULL,"g_vRealCameraVec");

		m_hvLightAmb		=	m_pEffect->GetParameterByName(NULL,"g_vLightAmb");
		m_hvLightDif		=	m_pEffect->GetParameterByName(NULL,"g_vLightDif");
		m_hvEyePos			=	m_pEffect->GetParameterByName(NULL,"g_vEyePos");

		m_hDecay_c0		=	m_pEffect->GetParameterByName(NULL,"decay_c0");
		m_hDecay_c1		=	m_pEffect->GetParameterByName(NULL,"decay_c1");
		m_hDecay_c2		=	m_pEffect->GetParameterByName(NULL,"decay_c2");

		m_hsBaias			=	m_pEffect->GetParameterByName(NULL,"g_sBaias");
		m_hShadowLange =	m_pEffect->GetParameterByName(NULL,"g_ShadowLange");
	}
	if(pErr!=NULL)pErr->Release();

	return hr;
}
//---------------------------------------------//
//ライトの数量の設定
//---------------------------------------------//
VOID	HLSLShader::SetLightCount(int num)
{
	//ライトの個数
	m_LightNum = num;

	//ライトの設定
	m_matLightProj	= new D3DXMATRIX[num];
	m_matLightView	= new D3DXMATRIX[num];
	m_vLightPos		= new D3DXVECTOR4[num];
	m_vLightVec		= new D3DXVECTOR4[num];

	//現在のライト
	m_NowLight = 0;

}
//---------------------------------------------//
//シェーダ処理を始める(環境の設定をする)
//---------------------------------------------//
VOID	HLSLShader::BeginShader()
{
	//サンプリング領域
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//画面幅の取得
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	if(m_pEffect!=NULL)
	{

		//シェーダの設定
		m_pEffect->SetTechnique(m_hTechnique);
		m_pEffect->Begin(NULL,0);

		//ビュー行列の設定---------------------------------------------
		D3DXVECTOR3 l_vEye	= (D3DXVECTOR3)m_vCameraPos;
		D3DXVECTOR3 l_vAt	= (D3DXVECTOR3)(m_vCameraPos + m_vCameraVec);
		D3DXVECTOR3 l_vUp	= D3DXVECTOR3( 0.0f , 1.0f , 0.0f );	
		D3DXMatrixLookAtLH(&m_matCameraView,&l_vEye,&l_vAt,&l_vUp);
		//-------------------------------------------------------------

		//射影行列の作成（今見えている画面の大きさ設定）---------------
		D3DXMatrixPerspectiveFovLH(&m_matCameraProj,D3DXToRadian(WORLD_DEGREE),(float)SCREEN_W/(float)SCREEN_H,WORLD_NEAR_Z,WORLD_FAR_Z);
		//-------------------------------------------------------------		

		//ライトの位置と行列の設定
		for(int i=0;i<m_LightNum;i++)
		{
			//ライトの注意点の位置
			D3DXVECTOR4 l_vLightAt = m_vLightPos[i] + m_vLightVec[i];

			//ライトからみたビュー行列の設定---------------------------------------------
			D3DXMatrixLookAtLH(&m_matLightView[i],&(D3DXVECTOR3)m_vLightPos[i],&(D3DXVECTOR3)l_vLightAt,&l_vUp);
			//-------------------------------------------------------------

			//ライトからみた射影行列の作成（今見えている画面の大きさ設定）---------------
			D3DXMatrixPerspectiveFovLH(&m_matLightProj[i],D3DXToRadian(WORLD_DEGREE*4),1.0f,SHADOW_NEAR_Z,SHADOW_FAR_Z);
			//-------------------------------------------------------------

		}

	}

}
//---------------------------------------------//
//ワールドからスクリーン座標への返還
//---------------------------------------------//
// スクリーン座標をワールド座標に変換
D3DXVECTOR3* HLSLShader::CalcScreenToWorld(
   D3DXVECTOR3* pout,
   float Sx,  // スクリーンX座標
   float Sy,  // スクリーンY座標
   float fZ)  // 射影空間でのZ値（0～1）
{

	RECT rect;
	m_pD3DDevice->GetScissorRect(&rect);

	//射影、ビューの取得
   const D3DXMATRIX matV = HLSLShader::m_matCameraView;
   const D3DXMATRIX matP = HLSLShader::m_matCameraProj;
   // 各行列の逆行列を算出
   D3DXMATRIX InvView, InvPrj, VP, InvViewport;
   D3DXMatrixInverse( &InvView, NULL, &matV);
   D3DXMatrixInverse( &InvPrj, NULL,  &matP );
   D3DXMatrixIdentity( &VP );
   VP._11 = rect.right/2.0f; VP._22 = -rect.bottom/2.0f;
   VP._41 = rect.right/2.0f; VP._42 = rect.bottom/2.0f;
   //ビューポート
   D3DXMatrixInverse( &InvViewport, NULL, &VP );

   // 逆変換
   D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
   D3DXVec3TransformCoord( pout, &D3DXVECTOR3(Sx,Sy,fZ), &tmp );

   return pout;
}

//---------------------------------------------//
//シェーダ処理を終了する
//---------------------------------------------//
VOID	HLSLShader::EndShader()
{
	m_pEffect->End();
}
//---------------------------------------------//
//開放処理
//---------------------------------------------//
VOID	HLSLShader::Close()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_DELETE_ARRAY(m_vLightPos);
	SAFE_DELETE_ARRAY(m_matLightProj);
	SAFE_DELETE_ARRAY(m_matLightView);
}