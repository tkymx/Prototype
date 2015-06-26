#pragma once

#include<d3dx9.h>
#include<vector>
#include<map>
#include "TKYBASEHEADER.h"
#include "TKY3D.h"

class HLSLShader;


//���E�̌�����傫��
static const FLOAT WORLD_NEAR_Z = 0.1f;
static const FLOAT WORLD_FAR_Z = 10000.0f;

//�����Ă���p�x
static const FLOAT WORLD_DEGREE = 30.0f;

//�e�̌�����傫��
static const FLOAT SHADOW_NEAR_Z = 0.5f;
static const FLOAT SHADOW_FAR_Z = 1000.0f;

//�G���[�`�F�b�N
static void HRESULTCheck(HRESULT hr)
{

		switch(hr)
		{
		case D3DERR_INVALIDCALL:
				   MessageBox(NULL,TEXT("D3DERR_INVALIDCALL"),TEXT("���s"),MB_OK);	
			break;
		case D3DXERR_INVALIDDATA:
				   MessageBox(NULL,TEXT("D3DXERR_INVALIDDATA"),TEXT("���s"),MB_OK);	
			break;
		case E_OUTOFMEMORY:
				   MessageBox(NULL,TEXT("E_OUTOFMEMORY"),TEXT("���s"),MB_OK);	
			break;
		default:
				   MessageBox(NULL,TEXT("���̑�"),TEXT("���s"),MB_OK);	
			break;
		}

}


//========== �V�������e ==========


//--------------------------------------------------//
//�@�N���X�@CMatrixMaterialSet
//�@�����@�@���}�g���b�N�X�̐ݒ�
//--------------------------------------------------//
CLASS(CMatrixMaterialSet)
{
protected:
	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up;
	FLOAT m_Deg,m_aspect,m_Near,m_Far;
	D3DXMATRIX m_lookup;
	D3DXMATRIX m_Perspective;
	LPTKY3D m_device;
public:
	CMatrixMaterialSet()
	{
		m_Eye = m_At = m_Up = D3DXVECTOR3();
		m_Deg = m_aspect = m_Near = m_Far = NULL;
		m_lookup = m_Perspective = D3DXMATRIX();
		m_device = NULL;
	}
	//�ݒ�
	bool SetLookAtMatrix( D3DXVECTOR3* Eye, D3DXVECTOR3* At, D3DXVECTOR3*  Up)
	{
		IF(m_Eye==NULL)m_Eye = D3DXVECTOR3();
		else m_Eye = *Eye;
		IF(m_At==NULL)m_At = m_Eye;
		else m_At = *At;
		IF(m_Up==NULL)m_Up = D3DXVECTOR3(0,1,0);
		else m_Up = *Up;

		D3DXMatrixLookAtLH(&m_lookup,&m_Eye,&m_At,&m_Up);
		return true;
	}
	bool  SetVec(D3DXVECTOR3 *pos,D3DXVECTOR3 *vec)
	{
		D3DXVECTOR3 p = *pos,v = *vec;
		IF(pos==NULL)p = m_Eye;
		else m_Eye = *pos;
		IF(vec==NULL)v = m_Eye;
		else m_At = m_Eye+*vec;

		SetLookAtMatrix( &m_Eye , &m_At , &D3DXVECTOR3(0,1,0) );
	}

	bool SetPerspectiveMatrix( FLOAT _deg,FLOAT _aspect,FLOAT _near,FLOAT _far)
	{
		m_Deg = _deg;m_aspect = _aspect;m_Near = _near;m_Far = _far;
		D3DXMatrixPerspectiveFovLH(&m_Perspective,D3DXToRadian(m_Deg),m_aspect,m_Near,m_Far);
		return true;
	}
	bool SetDevice(LPTKY3D device)
	{
		IF(device==NULL)return false;
		m_device = device;
		return true;
	}
	//�擾
	//EYE,AT,UP
	D3DXVECTOR3 GetLookAtValue( string str )
	{
		IF( str == "EYE" )return m_Eye;
		EIF( str == "AT" )return m_At;
		EIF( str == "UP" )return m_Up;	
		return D3DXVECTOR3();
	}
	//DEGREE,ASPECT,NEAR,FAR
	FLOAT GetPerspectiveValue( string str )
	{
		IF( str == "DEGREE" )return m_Deg;
		EIF( str == "ASPECT" )return m_aspect;
		EIF( str == "NEAR" )return m_Near;	
		EIF( str == "FAR" )return m_Far;	
		return NULL;
	}
	//LOOKUP,PERSPECT
	D3DXMATRIX GetMatrix( string str )
	{
		IF( str == "LOOKUP" )return m_lookup;
		EIF( str == "PERSPECT" )return m_Perspective;
		return D3DMATRIX();
	}
	//�f�o�C�X���
	LPTKY3D	GetDevice(){return m_device;};
	//�g�p
	VIRTUAL bool onExecute()=0;
};
//--------------------------------------------------//
//�@�N���X�@CLigth : public CMatrixMaterialSet
//�@�����@�@���C�g�}�g���b�N�X�̐ݒ�
//--------------------------------------------------//
CLASS(CLight) : public CMatrixMaterialSet
{
private:
	D3DMATERIAL9 m_color;
	D3DLIGHT9 m_light;
public:
	//�ݒ�
	VOID SetMaterial(D3DXVECTOR4 *m_amb ,D3DXVECTOR4 *m_dif,D3DXVECTOR4 *m_spe)
	{
		IF(m_amb!=NULL)
		{
			m_color.Ambient.r = m_amb->x;
			m_color.Ambient.g = m_amb->y;
			m_color.Ambient.b = m_amb->z;
		}
		IF(m_dif!=NULL)
		{
			m_color.Diffuse.r = m_dif->x;
			m_color.Diffuse.g = m_dif->y;
			m_color.Diffuse.b = m_dif->z;
		}
		IF(m_spe!=NULL)
		{
			m_color.Specular.r = m_spe->x;
			m_color.Specular.g = m_spe->y;
			m_color.Specular.b = m_spe->z;
		}
	}
	//�擾
	D3DMATERIAL9 GetMaterial(){return m_color;}
	//�g�p(���z)
	VIRTUAL bool onExecute()
	{
		//������
		ZeroMemory(&m_light,sizeof(D3DLIGHT9));
		//�����̎��
		m_light.Type = D3DLIGHT_DIRECTIONAL;	//���s����
		//�}�e���A�����
		m_light.Ambient = m_color.Ambient;
		m_light.Diffuse = m_color.Diffuse;
		m_light.Specular = m_color.Specular;
		//�n�_
		m_light.Position = m_Eye;
		//�����̐ݒ�
		D3DXVec3Normalize( (D3DXVECTOR3*)&m_light.Direction,&(m_At-m_Eye) );
		//���C�g�̓o�^
		m_device->GetD3DDevice()->SetLight(0,&m_light);
		m_device->GetD3DDevice()->LightEnable(0,TRUE);
		m_device->GetD3DDevice()->SetRenderState(D3DRS_LIGHTING,TRUE);
		m_device->GetD3DDevice()->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_ARGB(0,96,96,96));

		return true;
	}

};
//--------------------------------------------------//
//�@�N���X�@CCamera : public CMatrixMaterialSet
//�@�����@�@�J�����}�g���b�N�X�̐ݒ�
//--------------------------------------------------//
CLASS(CCamera) : public CMatrixMaterialSet
{
private:
public:
		//�g�p(���z)
	VIRTUAL bool onExecute()
	{
		m_device->GetD3DDevice()->SetTransform( D3DTS_VIEW , &m_lookup );
		m_device->GetD3DDevice()->SetTransform( D3DTS_PROJECTION , &m_Perspective );
		return true;
	}
};
//--------------------------------------------------//
//�@�N���X�@CEnvironment:CObject
//�@�����@�@���̐ݒ�
//--------------------------------------------------//
CLASS_POINTA_OBJECT( CEnvironment , LPTkyEnvironment )
{
protected:
	CCamera	m_camera;	
	CLight		m_light;
	LPTKY3D m_device;

	CEnvironment()
	{
	}
	VIRTUAL ~CEnvironment()
	{
	}

public:
	//�ݒ�
	VOID SetLightParameter( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , D3DXVECTOR4 *m_amb ,D3DXVECTOR4 *m_dif,D3DXVECTOR4 *m_spe )
	{
		m_light.SetLookAtMatrix( pos , &(*pos+*vec) , &D3DXVECTOR3(0,1,0) );
		m_light.SetMaterial( m_amb,m_dif,m_spe );
	}
	VOID SetLightParameterAt( D3DXVECTOR3 *pos, D3DXVECTOR3 *at , D3DXVECTOR4 *m_amb ,D3DXVECTOR4 *m_dif,D3DXVECTOR4 *m_spe )
	{
		m_light.SetLookAtMatrix( pos , at , &D3DXVECTOR3(0,1,0) );
		m_light.SetMaterial( m_amb,m_dif,m_spe );
	}
	VOID SetLightParameter( CLight *light )
	{
		m_light = *light;
	}
	VOID SetCameraParameter( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , FLOAT _deg,FLOAT _aspect,FLOAT _near,FLOAT _far  )
	{
		m_camera.SetLookAtMatrix( pos , &(*pos+*vec) , &D3DXVECTOR3(0,1,0) );
		m_camera.SetPerspectiveMatrix(_deg,_aspect,_near,_far);
	}
	VOID SetCameraParameter( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec )
	{
		m_camera.SetLookAtMatrix( pos , &(*pos+*vec) , &D3DXVECTOR3(0,1,0) );
	}
	VOID SetCameraParameterAt( D3DXVECTOR3 *pos, D3DXVECTOR3 *at , FLOAT _deg,FLOAT _aspect,FLOAT _near,FLOAT _far  )
	{
		m_camera.SetLookAtMatrix( pos , at , &D3DXVECTOR3(0,1,0) );
		m_camera.SetPerspectiveMatrix(_deg,_aspect,_near,_far);
	}
	VOID SetCameraParameter( CCamera *camera )
	{
		m_camera = *camera;
	}
	VOID SetDevice(LPTKY3D name){m_device = name;m_camera.SetDevice(name);m_light.SetDevice(name);};
	//�擾
	LPTKY3D	GetDevice(){return m_device;}
	VOID			GetLight( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , D3DMATERIAL9 *material )
	{
		IF( pos!=NULL )*pos = m_light.GetLookAtValue("EYE");
		IF( vec!=NULL )*vec = m_light.GetLookAtValue("AT") - m_light.GetLookAtValue("EYE");
		IF( material!=NULL )*material = m_light.GetMaterial(); 
	}
	VOID			GetCamera( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , FLOAT *_deg,FLOAT *_aspect,FLOAT *_near,FLOAT *_far  )
	{
		IF( pos!=NULL )*pos = m_camera.GetLookAtValue("EYE");
		IF( vec!=NULL )*vec = m_camera.GetLookAtValue("AT") - m_camera.GetLookAtValue("EYE");
		IF( _deg!=NULL )*_deg = m_camera.GetPerspectiveValue("DEGREE");
		IF( _aspect!=NULL )*_aspect = m_camera.GetPerspectiveValue("ASPECT");
		IF( _near!=NULL )*_near = m_camera.GetPerspectiveValue("NEAR");
		IF( _far!=NULL )*_far = m_camera.GetPerspectiveValue("FAR");
	}

	//�X�N���[�����W���烏�[���h���W���擾����
	Vector			GetWorldPosition( Vector screen )
	{
		RECT rect;
		this->m_device->GetD3DDevice()->GetScissorRect(&rect);

		//�J�����̍s��
		D3DXMATRIX matView = this->m_camera.GetMatrix("LOOKUP");
		D3DXMATRIX matProj = 	this->m_camera.GetMatrix("PERSPECT");

		//�ˉe�A�r���[�̎擾
	   const D3DXMATRIX matV = matView;
	   const D3DXMATRIX matP = matProj;
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

		//�o�͍��W
		D3DXVECTOR3 Near = Vector();
		D3DXVECTOR3 Far = Vector();
	   D3DXVec3TransformCoord( &Near, &D3DXVECTOR3(screen.x,screen.y,0), &tmp );
	   D3DXVec3TransformCoord( &Far, &D3DXVECTOR3(screen.x,screen.y,1), &tmp );

	   return (Far - Near);
	}


	//���s(���z)(HLSL���͈Ⴄ�󋵂ɂȂ�\�肾����)
	VIRTUAL VOID onExecute()
	{
		m_light.onExecute();
		m_camera.onExecute();
	}
	//�J��
	VIRTUAL VOID Release(){};
	//����
	STATIC LPTkyEnvironment CreateEnvironment(){return new CEnvironment();};
};
//--------------------------------------------------//
//�@�N���X�@CHLSL:CEnvironment
//�@�����@�@HLSL�̐ݒ�
//--------------------------------------------------//
CLASS_POINTA(CHLSL,LPTkyHLSL):public CEnvironment
{
private:
	//�e�N�j�b�N���
	D3DXHANDLE m_hTechnique;
	//�n���h�����(HLSL �ϐ�)
	CStringMatch<D3DXHANDLE> m_handle;
	//�R���X�g���N�^
	CHLSL();
protected:
	//�V�F�[�_���
	LPD3DXEFFECT	m_pEffect;	
public:
	//HLSL�̐���
	HRESULT SetHLSL( string str)
	{
	//�V�F�[�_�̓ǂݍ���
	LPD3DXBUFFER pErr = NULL;
	HRESULT hr = S_OK;

	IF( FAILED( hr = D3DXCreateEffectFromFileA(
		m_device->GetD3DDevice(),
		str.c_str(),
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErr ) ) )
	{
		//�G���[�`�F�b�N
		HRESULTCheck(hr);
		//�����T�C�Y�擾
		LPSTR s = (LPSTR)pErr->GetBufferPointer();
		//���b�Z�[�W�̎擾
		MessageBoxA(NULL,s,"���s",MB_OK);	
	}else{
		//����������n���h���̒ǉ�
		onSetHandle( &m_hTechnique,&m_handle );
	}

	SAFE_RELEASE(pErr);

	return hr;
	}
	//������(���z�����Čp����Őݒ�ł���悤�ɂ���)
	VIRTUAL VOID onSetHandle( D3DXHANDLE *m_hTechnique,CStringMatch<D3DXHANDLE> *m_handle ) PURE;
	//HLSL�̓K�p
	VIRTUAL VOID onExecuteHLSL( string event ) PURE;

	//�J��
	VIRTUAL VOID Release(){};
	//����
//	STATIC LPTkyHLSL CreateHLSL(){return new CHLSL();};
};
//--------------------------------------------------//
//�@�N���X�@CXFileHLSL:CHLSL
//�@�����@�@���W�ό`�^HLSL�̐ݒ�
//--------------------------------------------------//
CLASS_POINTA(CXFileHLSL,LPTkyXFileHLSL):public CHLSL
{
private:
	//�R���X�g���N�^
	CXFileHLSL();
	//�o�^���ꂽXFile���
public:
	//������(���z�����Čp����Őݒ�ł���悤�ɂ���)
	VIRTUAL VOID onSetHandle( D3DXHANDLE *m_hTechnique,CStringMatch<D3DXHANDLE> *m_handle )
	{
		*m_hTechnique		=	m_pEffect->GetTechniqueByName("TShader");

		//�r���[�Ǝˉe��Ԃւ̍��W�ϊ�
		m_handle->AddString(	 "m_hmVP" , m_pEffect->GetParameterByName(NULL,"g_mVP") );
		//���[�J�����W����ˉe��Ԃւ̍��W�ϊ�
		m_handle->AddString(	 "m_hmWVP" , m_pEffect->GetParameterByName(NULL,"g_mWVP") );
		
		//�{�[���p�ϊ��s��̔z��
		m_handle->AddString( "m_hmWorldMatrixArray" , m_pEffect->GetParameterByName(NULL,"g_mWorldMatrixArray") );
		//�{�[����(0�̂Ƃ��͒ʏ탌���_�����O)
		m_handle->AddString( "m_hiNumBones" , m_pEffect->GetParameterByName(NULL,"g_NumBones") );

		//�e�N�X�`���̐ݒ�
		m_handle->AddString( "m_hTex" ,	m_pEffect->GetParameterByName(NULL,"g_Tex") );

		//�����̐ݒ�(�@���l���̉A�e�v�Z�p�����x�N�g��)
		m_handle->AddString( "m_hvLight" , m_pEffect->GetParameterByName(NULL,"g_vLight") );

		//�J�����̈ʒu(�ʏ�`��p(���z�Ƃ���))
		m_handle->AddString( "m_hvEyePos" , m_pEffect->GetParameterByName(NULL,"g_vEyePos") );

		//���̋���(���[�U����)
		m_handle->AddString( "m_hvLightAmb" , m_pEffect->GetParameterByName(NULL,"g_vLightAmb") );
		m_handle->AddString( "m_hvLightDif" , m_pEffect->GetParameterByName(NULL,"g_vLightDif") );

	}
	//HLSL�̓K�p
	VIRTUAL VOID onExecuteHLSL( string event )
	{
	}
	//����
	STATIC LPTkyXFileHLSL CreateXFileHLSL(){return new CXFileHLSL();}
};

//========== �Â����e ==========

//--------------------------------------------------//
//HLSL������
//--------------------------------------------------//
class HLSLShader
{

private:

	D3DXHANDLE		m_hTechnique;		//�e�N�j�b�N
	LPDIRECT3DDEVICE9	m_pD3DDevice;		//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X

	//----------�J�����̐ݒ�----------
	D3DXMATRIX		m_matCameraView;	//�r���[�s��
	D3DXMATRIX		m_matCameraProj;	//�ˉe�s��
	D3DXVECTOR4		m_vCameraPos;		//�J�����̈ʒu
	D3DXVECTOR4		m_vCameraVec;		//�J�����̃x�N�g��
	//--------------------------------

	//----------���C�g�̐ݒ�----------
	D3DXMATRIX		*m_matLightView;	//���C�g����݂��r���[�s��
	D3DXMATRIX		*m_matLightProj;	//���C�g����݂��ˉe�s��
	D3DXVECTOR4		*m_vLightPos;		//���C�g�̈ʒu
	D3DXVECTOR4		*m_vLightVec;		//���C�g�̃x�N�g��
	int				m_NowLight;		    //���݂̃��C�g
	int				m_LightNum;			//���C�g�̌�
	float			m_dacay_c0;			//�������l�O
	float			m_dacay_c1;			//�������l�P
	float			m_dacay_c2;			//�������l�Q
	//--------------------------------

public:

	D3DXHANDLE		m_hmWorld;			//���[���h�s��
	D3DXHANDLE		m_hmVP;				//�r���[�ˉe�s��
	D3DXHANDLE		m_hmWVP;			//���[���h�ˉe�s��
	D3DXHANDLE		m_hmWVPB;			//���[���h�ˉe�s��o�C�A�X�K�p��(�e�N�X�`�����W)�ˉe�s��
	D3DXHANDLE		m_hmWLP;			//���C�g����݂����[���h�ˉe�s��
	D3DXHANDLE		m_hmLP;				//���C�g����݂��ˉe�s��
	D3DXHANDLE		m_hmWLPB;			//���C�g����݂��o�C�A�X�K�p��(�e�N�X�`�����W)�ˉe�s��
	D3DXHANDLE		m_hmB;			//�o�C�A�X�s��

	D3DXHANDLE		m_hmWorldMatrixArray;			//�{�[���̏�ԋL���̂��߂̍s��z��
	D3DXHANDLE		m_hiNumBones;						//�u�����f�B���O���̃{�[����

	D3DXHANDLE		m_hTex;				//�f�B�e�B�[���e�N�X�`���̃n���h��
	D3DXHANDLE		m_hShadowTex;		//�V���h�E�e�N�X�`���̃n���h��
	D3DXHANDLE		m_hRenderTex;		//�����_�����O�e�N�X�`���̃n���h��

	D3DXHANDLE		m_hvLight;			//�����̉A�e�����p�x�N�g���̃n���h��
	D3DXHANDLE		m_hvRealLightPos;	//���ۂ̌������W�̃n���h��
	D3DXHANDLE		m_hvRealLightVec;	//���ۂ̌����x�N�g���̃n���h��
	D3DXHANDLE		m_hvRealCameraPos;	//���ۂ̃J�������W�̃n���h��
	D3DXHANDLE		m_hvRealCameraVec;	//���ۂ̃J�����x�N�g���̃n���h��

	D3DXHANDLE		m_hvLightAmb;		//�����̃n���h��
	D3DXHANDLE		m_hvLightDif;		//�g�U���ˌ��̃n���h��
	D3DXHANDLE		m_hvEyePos;			//�J�����̈ʒu�x�N�g��

	//�������̌v�Z�p�ϐ�
	D3DXHANDLE		m_hDecay_c0;
	D3DXHANDLE		m_hDecay_c1;
	D3DXHANDLE		m_hDecay_c2;

	D3DXHANDLE		m_hsBaias;			//�o�C�A�X�␳�l
	D3DXHANDLE		m_hShadowLange;			//�o�C�A�X�␳�l

	LPD3DXEFFECT	m_pEffect;			//�V�F�[�_

	//�R���g���X�^
	HLSLShader();

	//HLSL�ǂݍ���
	HRESULT InitHLSL(LPCTSTR filename,LPDIRECT3DDEVICE9 pD3DDev);

	//----------�ݒ�֌W------------
	//���C�g
	VOID SetLightCount(int num);															//���C�g�̌����w�肷��
	VOID SetLightPos(int num,D3DXVECTOR4 *a_vLightPos){m_vLightPos[num] = *a_vLightPos;}    //���C�g�̈ʒu���w�肷��(�萔�w��)
	VOID SetLightVec(int num,D3DXVECTOR4 *a_vLightVec){m_vLightVec[num] = *a_vLightVec;}    //���C�g�̈ʒu���x�N�g������(�萔�w��)
	VOID SetLightDecay(int num,float c0,float c1,float c2){m_dacay_c0=c0;m_dacay_c1=c1;m_dacay_c2=c2;}    //���C�g�̌������l(�萔�w��)
	VOID SetNowLight(int num){m_NowLight = num;}											//���܂̃��C�g�����肷��B
	//�J����
	VOID SetCameraPos(D3DXVECTOR4 *a_vCameraPos){m_vCameraPos = *a_vCameraPos;}		//���C�g�̈ʒu���w�肷��(�萔�w��)
	VOID SetCameraVec(D3DXVECTOR4 *a_vCameraVec){m_vCameraVec = *a_vCameraVec;}		//���C�g�̈ʒu���x�N�g������(�萔�w��)
	//------------------------------


	//�V�F�[�_�������n�߂�
	VOID BeginShader();

	//�V�F�[�_�������I������
	VOID EndShader();

	//----------�ǂݍ��݌n----------
	//���C�g
	int			GetLightNum(){return m_LightNum;}						//���C�g�̐�
	D3DXVECTOR4 GetNowLightPos(){return m_vLightPos[m_NowLight];}			//���̃��C�g�̈ʒu
	D3DXVECTOR4 GetNowLightVec(){return m_vLightVec[m_NowLight];}		//���̃��C�g�̃x�N�g��
	D3DXMATRIX  GetNowLightProj(){return m_matLightProj[m_NowLight];}	//���̃��C�g�̎ˉe�s��
	D3DXMATRIX  GetNowLightView(){return m_matLightView[m_NowLight];}	//���̃��C�g�̃r���[�s��
	float				GetNowDecayC0(){return m_dacay_c0;}							//���̌������lc0��Ԃ�
	float				GetNowDecayC1(){return m_dacay_c1;}							//���̌������lc1��Ԃ�
	float				GetNowDecayC2(){return m_dacay_c2;}							//���̌������lc2��Ԃ�
	//�J����
	D3DXVECTOR4 GetCameraPos(){return m_vCameraPos;}					//�J�����̈ʒu
	D3DXVECTOR4 GetCameraVec(){return m_vCameraVec;}					//�J�����̃x�N�g��
	D3DXMATRIX   GetCameraProj(){return m_matCameraProj;}				//�J�����̎ˉe�s��
	D3DXMATRIX   GetCameraView(){return m_matCameraView;}				//�J�����̃r���[�s��
	//------------------------------

	// �X�N���[�����W�����[���h���W�ɕϊ�
	D3DXVECTOR3* CalcScreenToWorld(
	   D3DXVECTOR3* pout,
	   float Sx,  // �X�N���[��X���W
	   float Sy,  // �X�N���[��Y���W
	   float fZ  // �ˉe��Ԃł�Z�l�i0�`1�j
	); 

	//�J������
	VOID Close();

};
typedef HLSLShader* LPTKYHLSL;


static D3DXMATRIXA16 MakeWorldMatrix(float a_x,float a_y,float a_z,float a_rad)
{

}