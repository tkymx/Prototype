
#include "TKYHLSL.h"


//////////////////////////////////////////////////////////////////////////////////////////
//
//		HLSL�V�F�[�_�֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//�R���g���X�^
//---------------------------------------------//
HLSLShader::HLSLShader()
{

	HLSLShader::m_pEffect = NULL;

	//�n���h���֌W
	m_hmWorld		= NULL;		//���[���h�s��
	m_hmVP			= NULL;		//�r���[�ˉe�s��
	m_hmWVP			= NULL;		//���[���h�ˉe�s��
	m_hmWLP			= NULL;		//���C�g����݂��ˉe�s��
	m_hmWLPB		= NULL;		//�o�C�A�X�K�p��(�e�N�X�`�����W)�ˉe�s��

	m_hTex			= NULL;		//�f�B�e�B�[���e�N�X�`���̃n���h��
	m_hShadowTex	= NULL;		//�V���h�E�e�N�X�`���̃n���h��

	m_hvLight		= NULL;		//�����x�N�g���̃n���h��
	m_hvLightAmb	= NULL;		//�����̃n���h��
	m_hvLightDif	= NULL;		//�g�U���ˌ��̃n���h��
	m_hvEyePos		= NULL;		//�J�����̈ʒu�x�N�g��

	m_hsBaias		= NULL;		//�o�C�A�X�␳�l

	m_pEffect		= NULL;
}
//---------------------------------------------//
//HLSL�ǂݍ���
//---------------------------------------------//
HRESULT HLSLShader::InitHLSL(LPCTSTR filename,LPDIRECT3DDEVICE9 pD3DDev)
{

	m_pD3DDevice = pD3DDev;

	//�V�F�[�_�̓ǂݍ���
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

		//�A�X�L�[���烆�j�R�[�h
		MultiByteToWideChar(
			CP_ACP,
			MB_PRECOMPOSED,
			s,
			MAX_PATH*2,
			(LPTSTR)t,
			MAX_PATH*2);

		MessageBox(NULL,(LPCTSTR)t,TEXT("���s"),MB_OK);	



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
//���C�g�̐��ʂ̐ݒ�
//---------------------------------------------//
VOID	HLSLShader::SetLightCount(int num)
{
	//���C�g�̌�
	m_LightNum = num;

	//���C�g�̐ݒ�
	m_matLightProj	= new D3DXMATRIX[num];
	m_matLightView	= new D3DXMATRIX[num];
	m_vLightPos		= new D3DXVECTOR4[num];
	m_vLightVec		= new D3DXVECTOR4[num];

	//���݂̃��C�g
	m_NowLight = 0;

}
//---------------------------------------------//
//�V�F�[�_�������n�߂�(���̐ݒ������)
//---------------------------------------------//
VOID	HLSLShader::BeginShader()
{
	//�T���v�����O�̈�
	RECT rect;
	this->m_pD3DDevice->GetScissorRect(&rect);
	//��ʕ��̎擾
	UINT SCREEN_W = rect.right;
	UINT SCREEN_H = rect.bottom;

	if(m_pEffect!=NULL)
	{

		//�V�F�[�_�̐ݒ�
		m_pEffect->SetTechnique(m_hTechnique);
		m_pEffect->Begin(NULL,0);

		//�r���[�s��̐ݒ�---------------------------------------------
		D3DXVECTOR3 l_vEye	= (D3DXVECTOR3)m_vCameraPos;
		D3DXVECTOR3 l_vAt	= (D3DXVECTOR3)(m_vCameraPos + m_vCameraVec);
		D3DXVECTOR3 l_vUp	= D3DXVECTOR3( 0.0f , 1.0f , 0.0f );	
		D3DXMatrixLookAtLH(&m_matCameraView,&l_vEye,&l_vAt,&l_vUp);
		//-------------------------------------------------------------

		//�ˉe�s��̍쐬�i�������Ă����ʂ̑傫���ݒ�j---------------
		D3DXMatrixPerspectiveFovLH(&m_matCameraProj,D3DXToRadian(WORLD_DEGREE),(float)SCREEN_W/(float)SCREEN_H,WORLD_NEAR_Z,WORLD_FAR_Z);
		//-------------------------------------------------------------		

		//���C�g�̈ʒu�ƍs��̐ݒ�
		for(int i=0;i<m_LightNum;i++)
		{
			//���C�g�̒��ӓ_�̈ʒu
			D3DXVECTOR4 l_vLightAt = m_vLightPos[i] + m_vLightVec[i];

			//���C�g����݂��r���[�s��̐ݒ�---------------------------------------------
			D3DXMatrixLookAtLH(&m_matLightView[i],&(D3DXVECTOR3)m_vLightPos[i],&(D3DXVECTOR3)l_vLightAt,&l_vUp);
			//-------------------------------------------------------------

			//���C�g����݂��ˉe�s��̍쐬�i�������Ă����ʂ̑傫���ݒ�j---------------
			D3DXMatrixPerspectiveFovLH(&m_matLightProj[i],D3DXToRadian(WORLD_DEGREE*4),1.0f,SHADOW_NEAR_Z,SHADOW_FAR_Z);
			//-------------------------------------------------------------

		}

	}

}
//---------------------------------------------//
//���[���h����X�N���[�����W�ւ̕Ԋ�
//---------------------------------------------//
// �X�N���[�����W�����[���h���W�ɕϊ�
D3DXVECTOR3* HLSLShader::CalcScreenToWorld(
   D3DXVECTOR3* pout,
   float Sx,  // �X�N���[��X���W
   float Sy,  // �X�N���[��Y���W
   float fZ)  // �ˉe��Ԃł�Z�l�i0�`1�j
{

	RECT rect;
	m_pD3DDevice->GetScissorRect(&rect);

	//�ˉe�A�r���[�̎擾
   const D3DXMATRIX matV = HLSLShader::m_matCameraView;
   const D3DXMATRIX matP = HLSLShader::m_matCameraProj;
   // �e�s��̋t�s����Z�o
   D3DXMATRIX InvView, InvPrj, VP, InvViewport;
   D3DXMatrixInverse( &InvView, NULL, &matV);
   D3DXMatrixInverse( &InvPrj, NULL,  &matP );
   D3DXMatrixIdentity( &VP );
   VP._11 = rect.right/2.0f; VP._22 = -rect.bottom/2.0f;
   VP._41 = rect.right/2.0f; VP._42 = rect.bottom/2.0f;
   //�r���[�|�[�g
   D3DXMatrixInverse( &InvViewport, NULL, &VP );

   // �t�ϊ�
   D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
   D3DXVec3TransformCoord( pout, &D3DXVECTOR3(Sx,Sy,fZ), &tmp );

   return pout;
}

//---------------------------------------------//
//�V�F�[�_�������I������
//---------------------------------------------//
VOID	HLSLShader::EndShader()
{
	m_pEffect->End();
}
//---------------------------------------------//
//�J������
//---------------------------------------------//
VOID	HLSLShader::Close()
{
	SAFE_RELEASE(m_pEffect);
	SAFE_DELETE_ARRAY(m_vLightPos);
	SAFE_DELETE_ARRAY(m_matLightProj);
	SAFE_DELETE_ARRAY(m_matLightView);
}