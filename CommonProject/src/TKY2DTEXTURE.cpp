
#include "TKY2DTEXTURE.h"

//===========================================================
//�V�e�N�X�`���N���X
// �ǂݍ��݁A�\���A�A�j���[�V�����ŕ\���A�A�j���[�V�������̍X�V�A�d���p�e�N�X�`���}�b�v
//===========================================================

	queue<CTexture::TWaitTexture> CTexture::m_DrawWaitTexture;
	//�R���X�g���N�^�A�f�X�g���N�^�A�R�s�[�R���X�g���N�^
	CTexture::CTexture()
	{
		//��{���
		m_device = Init3D::CreateInit3D();
		//�e�N�X�`�����
		m_texture.Initialize();

	}
	CTexture::CTexture(const CTexture& tex )
	{
		//�e�N�X�`�����
		m_device = tex.GetDevice();
		m_texture = tex.GetTextureInfo();
	}
	CTexture::~CTexture()
	{
		//�e�N�X�`�����
		m_texture.Initialize();
	}
	//�ǂݍ���
	HRESULT CTexture::SetTexture( string name ,INT width,INT height )
	{
		HRESULT r;
		if(FAILED(r =	D3DXCreateTextureFromFileExA(
			m_device->GetD3DDevice(),
			name.c_str(),              // �t�@�C����
			0,
			0,
			0,
			0,
			D3DFMT_A8B8G8R8,         
			D3DPOOL_MANAGED,
			D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR,
			D3DCOLOR_ARGB(0, 0, 0, 0),  // ���F�𓧉ߐF�Ƃ���
			NULL,
			NULL,
			&(m_texture.m_pTextures))						//�ǂݍ��񂾃e�N�X�`����Ԃ��|�C���^
			))
		{
			m_texture.m_pTextures = NULL;
			MessageBoxA(NULL,name.c_str(),"�摜�ǂݍ��߂܂���	",MB_OK);
			return r;
		}

		//�摜�T�C�Y���擾
		m_texture.m_width = width;    // ���i�s�N�Z���j
		m_texture.m_height = height;   // �����i�s�N�Z���j
		m_texture.m_MaxFrame = 1;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = 1;
		m_texture.m_HorizontalDivision = 1;		//��������
		m_texture.m_VerticalDivision = 1;		//�c������

		return r;
	}
	HRESULT CTexture::SetTextureW( wstring name ,INT width,INT height )
	{
		HRESULT r;
		if(FAILED(r =	D3DXCreateTextureFromFileExW(
			m_device->GetD3DDevice(),
			name.c_str(),              // �t�@�C����
			0,
			0,
			0,
			0,
			D3DFMT_A8B8G8R8,         
			D3DPOOL_MANAGED,
			D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR,
			D3DCOLOR_ARGB(0, 0, 0, 0),  // ���F�𓧉ߐF�Ƃ���
			NULL,
			NULL,
			&(m_texture.m_pTextures))						//�ǂݍ��񂾃e�N�X�`����Ԃ��|�C���^
			))
		{
			m_texture.m_pTextures = NULL;
			MessageBoxW(NULL,name.c_str(),TEXT("�摜�ǂݍ��߂܂���"),MB_OK);
			return r;
		}

		//�摜�T�C�Y���擾
		m_texture.m_width = width;    // ���i�s�N�Z���j
		m_texture.m_height = height;   // �����i�s�N�Z���j
		m_texture.m_MaxFrame = 1;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = 1;
		m_texture.m_HorizontalDivision = 1;		//��������
		m_texture.m_VerticalDivision = 1;		//�c������

		return r;
	}
	//�t�@�C�����A���摜���A���摜���A�c�摜��,�摜�̉����A�摜�̏c��,�A�j���[�V�����X�s�[�h
	HRESULT CTexture::SetTexture( string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		HRESULT r;
		//�A�j���[�V�����̃Z�b�g
		IF_FAILED(r = SetTexture( name ,width,height ))return r;
		//�e�A�j���[�V�������
		m_texture.m_MaxFrame = num;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = speed;
		m_texture.m_HorizontalDivision = Horizontal;		//��������
		m_texture.m_VerticalDivision = Vertical;		//�c������
		return r;
	}
	HRESULT CTexture::SetTextureW( wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		HRESULT r;
		//�A�j���[�V�����̃Z�b�g
		IF_FAILED(r = SetTextureW( name ,width,height ))return r;
		//�e�A�j���[�V�������
		m_texture.m_MaxFrame = num;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = speed;
		m_texture.m_HorizontalDivision = Horizontal;		//��������
		m_texture.m_VerticalDivision = Vertical;		//�c������
		return r;
	}
	//�`��@:�@�ʒu�A�p�x�A���[�v���鎞�ԁ@:�@����������ǂ���
	BOOL CTexture::DrawTexture(Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type , bool wait)
	{

		DrawTextureIndex(pos,rotate,expand,type,this->m_texture.m_NowFrame,wait);

		return FALSE;
		
	}
	BOOL CTexture::DrawTextureIndex(Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type,BYTE index , bool wait)
	{
		IF( m_texture.m_height > 2024U || m_texture.m_width > 2024U )return false;

		//�҂��ɒǉ�
		IF( wait == true )
		{
			TWaitTexture t;
			t.m_tex = this;
			t.pos = pos;
			t.rotate = rotate;
			t.expand = expand;
			t.type = type;
			t.Index = index;
			m_DrawWaitTexture.push(t);
			return false;
		}

		
		float width = (float)m_texture.m_width;
		float height = (float)m_texture.m_height;

		BYTE NowFrame = index;

		//���̉摜�ʒu
		float Upos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_texture.m_MaxFrame>1)Upos = ((float)NowFrame-(NowFrame/this->m_texture.m_HorizontalDivision)*this->m_texture.m_HorizontalDivision)/this->m_texture.m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_texture.m_HorizontalDivision);

		//�c�̉摜�ʒu
		float Vpos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_texture.m_MaxFrame>1)Vpos = (NowFrame/this->m_texture.m_HorizontalDivision)/(float)this->m_texture.m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_texture.m_VerticalDivision);

		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		//�T���v��
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

		IF(m_texture.m_pTextures==NULL)return false;

		//�}�g���b�N�X�̏�����
		D3DXMATRIXA16 TransMat;						//���W�ϊ��s��	
		D3DXMatrixIdentity(&TransMat);									//�s��̏�����
		m_device->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//���W�s��Z�b�g

		IF( type == TKYTEXTURE_TYPE_2D_MODE)
		{
			Vector vec[4] = {
				Vector( -width/2 , -height/2 , 0 ),
				Vector( width/2 , -height/2 , 0 ),
				Vector( -width/2 , height/2 , 0 ),
				Vector( width/2 , height/2 , 0 ),
			};

			FOR_I(4)
			{
				vec[i] = vec[i].Expand( Vector() , expand );	//�g��
				vec[i] = vec[i].Rotate( Vector() , rotate.x , rotate.y , rotate.z );
				vec[i] += pos;
			}

			TVERTEX Vertex[4] = {
				// x				y				z				rhw	color															tu							tv
				{ vec[0].x,	vec[0].y,	vec[0].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos					,	Vpos ,},
				{ vec[1].x,	vec[1].y,	vec[1].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	,	Vpos ,},
				{ vec[2].x,	vec[2].y,	vec[2].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos					,	Vpos + Vvec,},
				{ vec[3].x,	vec[3].y,	vec[3].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	,	Vpos + Vvec,},
			};

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1 );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( TVERTEX ) );
		}
		//3d��xy���ō\������(0,0,1)�̌����������Ă���
		IF( type == TKYTEXTURE_TYPE_3D_MODE)
		{
/*
			TVERTEX Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ 1.0f,			1.0f,				0,	1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos ,},
				{ -1.0f,			1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos ,},
				{ -1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos + Vvec,},
				{ 1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos + Vvec,},
			};
*/
			CUSTOMVERTEX3DTEXTURE Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ Vector(-10.0f,-1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos ,},
				{ Vector(10.0f,	-1.0f,	0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos ,},
				{ Vector(-10.0f,	1.0f,0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos + Vvec,},
				{ Vector(10.0f,1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos + Vvec,},
			};


			//�}�b�v������
			SetDeviceMatrix( m_device->GetD3DDevice() , pos, rotate , expand);

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_CUSTOMVERTEX3DTEXTURE );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( CUSTOMVERTEX3DTEXTURE ) );
		}

		return FALSE;
	}
	BOOL CTexture::DrawTextureAlpha(float alpha,Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type , bool wait)
	{

		DrawTextureIndexAlpha(alpha,pos,rotate,expand,type,this->m_texture.m_NowFrame,wait);

		return FALSE;
		
	}
	BOOL CTexture::DrawTextureIndexAlpha(float alpha,Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type,BYTE index , bool wait)
	{
		//�҂��ɒǉ�
		IF( wait == true )
		{
			TWaitTexture t;
			t.m_tex = this;
			t.pos = pos;
			t.rotate = rotate;
			t.expand = expand;
			t.type = type;
			t.Index = index;
			m_DrawWaitTexture.push(t);
			return false;
		}

		
		float width = (float)m_texture.m_width;
		float height = (float)m_texture.m_height;

		BYTE NowFrame = index;

		//���̉摜�ʒu
		float Upos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_texture.m_MaxFrame>1)Upos = ((float)NowFrame-(NowFrame/this->m_texture.m_HorizontalDivision)*this->m_texture.m_HorizontalDivision)/this->m_texture.m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_texture.m_HorizontalDivision);

		//�c�̉摜�ʒu
		float Vpos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_texture.m_MaxFrame>1)Vpos = (NowFrame/this->m_texture.m_HorizontalDivision)/(float)this->m_texture.m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_texture.m_VerticalDivision);

		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		//�T���v��
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

		IF(m_texture.m_pTextures==NULL)return false;

		//�}�g���b�N�X�̏�����
		D3DXMATRIXA16 TransMat;						//���W�ϊ��s��	
		D3DXMatrixIdentity(&TransMat);									//�s��̏�����
		m_device->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//���W�s��Z�b�g

		IF( type == TKYTEXTURE_TYPE_2D_MODE)
		{
			Vector vec[4] = {
				Vector( -width/2 , -height/2 , 0 ),
				Vector( width/2 , -height/2 , 0 ),
				Vector( -width/2 , height/2 , 0 ),
				Vector( width/2 , height/2 , 0 ),
			};

			FOR_I(4)
			{
				vec[i] = vec[i].Expand( Vector() , expand );	//�g��
				vec[i] = vec[i].Rotate( Vector() , rotate.x , rotate.y , rotate.z );
				vec[i] += pos;
			}

			INT al = 0xff * alpha;

			TVERTEX Vertex[4] = {
				// x				y				z				rhw	color															tu							tv
				{ vec[0].x,	vec[0].y,	vec[0].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos					,	Vpos ,},
				{ vec[1].x,	vec[1].y,	vec[1].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos + Uvec	,	Vpos ,},
				{ vec[2].x,	vec[2].y,	vec[2].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos					,	Vpos + Vvec,},
				{ vec[3].x,	vec[3].y,	vec[3].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos + Uvec	,	Vpos + Vvec,},
			};

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1 );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( TVERTEX ) );
		}
		//3d��xy���ō\������(0,0,1)�̌����������Ă���
		IF( type == TKYTEXTURE_TYPE_3D_MODE)
		{
/*
			TVERTEX Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ 1.0f,			1.0f,				0,	1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos ,},
				{ -1.0f,			1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos ,},
				{ -1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos + Vvec,},
				{ 1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos + Vvec,},
			};
*/
			CUSTOMVERTEX3DTEXTURE Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ Vector(-10.0f,-1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos ,},
				{ Vector(10.0f,	-1.0f,	0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos ,},
				{ Vector(-10.0f,	1.0f,0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos + Vvec,},
				{ Vector(10.0f,1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos + Vvec,},
			};


			//�}�b�v������
			SetDeviceMatrix( m_device->GetD3DDevice() , pos, rotate , expand);

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_CUSTOMVERTEX3DTEXTURE );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( CUSTOMVERTEX3DTEXTURE ) );
		}

		return FALSE;
	}
	//�X�V
	BOOL CTexture::Update(float step)
	{
/*
		//���Ԃɂ���đJ��
		IF( timeGetTime() - m_texture.m_NowTime > m_texture.m_AnimeSpeed*step )
		{
			//���Ԃ̐ݒ�
			m_texture.m_NowTime = timeGetTime();
			//�A�j���[�V�����̑J��
			IF( ++m_texture.m_NowFrame >= m_texture.m_MaxFrame )
			{
				//������
				m_texture.m_NowFrame=0;
				//�^��Ԃ�
				return true;
			}
		}
		//�ʏ�J�ڂ�����U��Ԃ�
		*/
		UpdateIndex( step , &m_texture.m_NowTime , &m_texture.m_NowFrame );

		return false;
	}
	BOOL CTexture::UpdateIndex(float step,DWORD *time,BYTE *index)
	{
		IF(time == NULL || index == NULL)MessageBoxW(NULL,TEXT("CTexture::Update(float step,DWORD *time,float *index)"),TEXT("����"),NULL);

		//���Ԃɂ���đJ��
		IF( timeGetTime() - *time > m_texture.m_AnimeSpeed*step )
		{
			//���Ԃ̐ݒ�
			*time = timeGetTime();
			//�A�j���[�V�����̑J��
			IF( ++*index >= m_texture.m_MaxFrame )
			{
				//������
				*index=0;
				//�^��Ԃ�
				return true;
			}
		}
		//�ʏ�J�ڂ�����U��Ԃ�
		return false;
	}
	//����(�ł��Ȃ�������NULL��Ԃ�)
	LPTkyTexture CTexture::CreateTexture( LPTKY3D device , string name ,INT width,INT height)
	{
		LPTkyTexture tex = new CTexture();
		tex->SetDevice(device);
		IF_FAILED( tex->SetTexture( name , width , height ) )
		{
			//����
			tex->DestroyOwnObject();
			return NULL;
		}
		return tex;
	}
	//�����@:�@�t�@�C�����A���摜���A���摜���A�c�摜��,�摜�̉����A�摜�̏c��,�A�j���[�V�����X�s�[�h;
	LPTkyTexture CTexture::CreateTexture( LPTKY3D device , string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		LPTkyTexture tex = new CTexture();
		tex->SetDevice(device);
		IF_FAILED( tex->SetTexture( name , num,Horizontal,Vertical,width,height,speed ) )
		{
			//����
			tex->DestroyOwnObject();
			return NULL;
		}
		return tex;
	}
	LPTkyTexture CTexture::CreateTextureW( LPTKY3D device , wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		LPTkyTexture tex = new CTexture();
		tex->SetDevice(device);
		IF_FAILED( tex->SetTextureW( name , num,Horizontal,Vertical,width,height,speed ) )
		{
			//����
			tex->DestroyOwnObject();
			return NULL;
		}
		return tex;
	}

//===========================================================
//�I���
//===========================================================

//////////////////////////////////////////////////////////////////////////////////////////
//
//		2D�`��
//
//////////////////////////////////////////////////////////////////////////////////////////
vector<D3DTKY2DTEXTURE*> D3DTKY2DTEXTURE::m_vTex;
//---------------------------------------------//
//�R���g���X�^
//---------------------------------------------//
D3DTKY2DTEXTURE::D3DTKY2DTEXTURE(LPTKY3D init,HLSLShader* phlsl)
{
	IF(init!=NULL)	D3DTKY2DTEXTURE::m_pD3D = init->GetD3D();
	IF(init!=NULL)D3DTKY2DTEXTURE::m_pD3DDevice = init->GetD3DDevice();
	D3DTKY2DTEXTURE::m_pTextures = NULL;
	D3DTKY2DTEXTURE::m_pHLSL = phlsl;
	D3DTKY2DTEXTURE::m_width = 0;
	D3DTKY2DTEXTURE::m_height = 0;
}
D3DTKY2DTEXTURE::D3DTKY2DTEXTURE(const D3DTKY2DTEXTURE& tky2d)
{
	D3DTKY2DTEXTURE::m_pTextures = NULL;
	*this = tky2d;
	//���݂̃t���[���̏�����
	this->m_NowFrame = 0;
}
D3DTKY2DTEXTURE::~D3DTKY2DTEXTURE()
{

	assert(D3DTKY2DTEXTURE::m_pTextures);
	SAFE_RELEASE(D3DTKY2DTEXTURE::m_pTextures);

}
HRESULT D3DTKY2DTEXTURE::ReadTexture(LPCTSTR filename,INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
{
	HRESULT r;
	if(FAILED(r =	D3DXCreateTextureFromFileEx(
									D3DTKY2DTEXTURE::m_pD3DDevice,
									filename,              // �t�@�C����
									0,
									0,
									0,
									0,
									D3DFMT_A16B16G16R16,         
									D3DPOOL_MANAGED,
									D3DX_FILTER_LINEAR,
									D3DX_FILTER_LINEAR,
									D3DCOLOR_ARGB(0, 0, 0, 0),  // ���F�𓧉ߐF�Ƃ���
									NULL,
									NULL,
									&(D3DTKY2DTEXTURE::m_pTextures)						//�ǂݍ��񂾃e�N�X�`����Ԃ��|�C���^
								)))
	{
		D3DTKY2DTEXTURE::m_pTextures = NULL;
		MessageBox(NULL,filename,TEXT("�摜�ǂݍ��߂܂���	"),MB_OK);
		return r;
	}

	// �T�[�t�F�C�X���擾
	IDirect3DSurface9 *pSurface;
	D3DTKY2DTEXTURE::m_pTextures->GetSurfaceLevel( 0, &pSurface ); 
	// �T�[�t�F�C�X��񂩂�摜�T�C�Y���擾
	D3DSURFACE_DESC SurfaceInfo;
	pSurface->GetDesc( &SurfaceInfo );
	D3DTKY2DTEXTURE::m_width = width;    // ���i�s�N�Z���j
	D3DTKY2DTEXTURE::m_height = height;   // �����i�s�N�Z���j

	//�e�A�j���[�V�������
	D3DTKY2DTEXTURE::m_MaxFrame = num;
	D3DTKY2DTEXTURE::m_NowFrame = 0;
	D3DTKY2DTEXTURE::m_NowTime = timeGetTime();
	D3DTKY2DTEXTURE::m_AnimeSpeed = speed;
	D3DTKY2DTEXTURE::m_HorizontalDivision = Horizontal;		//��������
	D3DTKY2DTEXTURE::m_VerticalDivision = Vertical;		//�c������

	return r;

}
BOOL D3DTKY2DTEXTURE::DrawTexture(float x,float y,INT loop)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//���̉摜�ʒu
		float Upos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//�c�̉摜�ʒu
		float Vpos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		//�T���v��
		D3DTKY2DTEXTURE::m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				, Vpos + Vvec,},
		};

		IF(D3DTKY2DTEXTURE::m_pTextures==NULL)return false;

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTexture( 0, D3DTKY2DTEXTURE::m_pTextures);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetVertexShader(NULL);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1 );
		D3DTKY2DTEXTURE::m_pD3DDevice->SetPixelShader(0);
		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		return TRUE;
}
BOOL D3DTKY2DTEXTURE::DrawTexture(float x,float y,float falpha,INT loop)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//���̉摜�ʒu
		float Upos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//�c�̉摜�ʒu
		float Vpos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		UINT alpha = (UINT)(0xff * falpha);


		TVERTEX Vertex[4] = {
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTexture( 0, D3DTKY2DTEXTURE::m_pTextures);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetVertexShader(NULL);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW  | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetPixelShader(0);
		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		return TRUE;
}
BOOL D3DTKY2DTEXTURE::DrawTexture3D(float x,float y,float z,INT loop)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//���̉摜�ʒu
		float Upos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//�c�̉摜�ʒu
		float Vpos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);

		char str[MAX_PATH];
		sprintf_s(str,"NOW:%d,MAX:%d,VERTICAL:%d,UPOS:%lf UVEC:%lf VPOS:%lf VVEC:%lf\n",this->m_NowFrame,this->m_MaxFrame,this->m_VerticalDivision,Upos,Uvec,Vpos,Vvec);

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);

		//���W������
		D3DXMATRIXA16 TransMat,RotateMat,matWorld;						//���W�ϊ��s��	
		D3DXMatrixIdentity(&TransMat);									//�s��̏�����
		D3DXMatrixIdentity(&RotateMat);									//�s��̏�����
		D3DXMatrixIdentity(&matWorld);									//���v�s��̏�����
		//���W�Z�b�g
		D3DXMatrixTranslation(&TransMat,x,y,z);							//���W�s��ړ�			
//		D3DXMatrixRotationY(&RotateMat,a_rad);							//y����]�̍s��ɂ���

		matWorld = RotateMat * TransMat;								//�s��̍���				 
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTransform( D3DTS_WORLD , &matWorld );	//���W�s��Z�b�g


		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTexture( 0, D3DTKY2DTEXTURE::m_pTextures);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetVertexShader(NULL);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );
		D3DTKY2DTEXTURE::m_pD3DDevice->SetPixelShader(0);
		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		return TRUE;

}
BOOL D3DTKY2DTEXTURE::DrawTextureHLSL(float x,float y,float z,INT loop,HLSLShader* hlsl)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//���̉摜�ʒu
		float Upos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//�c�̉摜�ʒu
		float Vpos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);


		//���W������
		D3DXMATRIXA16 TransMat,RotateMat,matWorld,l_mat;						//���W�ϊ��s��	
		D3DXMatrixIdentity(&TransMat);									//�s��̏�����
		D3DXMatrixIdentity(&RotateMat);									//�s��̏�����
		D3DXMatrixIdentity(&matWorld);									//���v�s��̏�����
		//���W�Z�b�g
		D3DXMatrixTranslation(&TransMat,x,y,z);							//���W�s��ړ�			
//		D3DXMatrixRotationY(&RotateMat,a_rad);							//y����]�̍s��ɂ���

		matWorld = RotateMat * TransMat;								//�s��̍���				 


		//���_�ϊ��s��̍쐬-------------------------------------------
		D3DXMatrixIdentity(&l_mat);
		hlsl->m_pEffect->SetMatrix(hlsl->m_hmWVP,&l_mat);			//�s��̐ݒ�
		//-------------------------------------------------------------

		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );

		hlsl->m_pEffect->SetTexture( hlsl->m_hTex , D3DTKY2DTEXTURE::m_pTextures );


		hlsl->BeginShader();

		hlsl->m_pEffect->BeginPass(D3DTKY_ONLY_TEXTURE);

		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		hlsl->m_pEffect->EndPass();

		hlsl->EndShader();

		return TRUE;

}
BOOL D3DTKY2DTEXTURE::DrawTextureHLSL(float x,float y,float z,float falpha,INT loop,HLSLShader* hlsl)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//���̉摜�ʒu
		float Upos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//�c�̉摜�ʒu
		float Vpos = 0;
		//�摜�����P�ȏゾ������
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);


		//���W������
		D3DXMATRIXA16 TransMat,RotateMat,matWorld,l_mat;						//���W�ϊ��s��	
		D3DXMatrixIdentity(&TransMat);									//�s��̏�����
		D3DXMatrixIdentity(&RotateMat);									//�s��̏�����
		D3DXMatrixIdentity(&matWorld);									//���v�s��̏�����
		//���W�Z�b�g
		D3DXMatrixTranslation(&TransMat,x,y,z);							//���W�s��ړ�			
//		D3DXMatrixRotationY(&RotateMat,a_rad);							//y����]�̍s��ɂ���

		matWorld = RotateMat * TransMat;								//�s��̍���				 


		//���_�ϊ��s��̍쐬-------------------------------------------
		D3DXMatrixIdentity(&l_mat);
		hlsl->m_pEffect->SetMatrix(hlsl->m_hmWVP,&l_mat);			//�s��̐ݒ�
		//-------------------------------------------------------------

		UINT alpha = (UINT)(0xff * falpha);

		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );

		hlsl->m_pEffect->SetTexture( hlsl->m_hTex , D3DTKY2DTEXTURE::m_pTextures );


		hlsl->BeginShader();

		hlsl->m_pEffect->BeginPass(D3DTKY_ONLY_TEXTURE);

		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		hlsl->m_pEffect->EndPass();

		hlsl->EndShader();

		return TRUE;

}
BOOL D3DTKY2DTEXTURE::Update()
{
	//���Ԃɂ���đJ��
	if( timeGetTime() - this->m_NowTime > this->m_AnimeSpeed )
	{
		//���Ԃ̐ݒ�
		this->m_NowTime = timeGetTime();
		//�A�j���[�V�����̑J��
		if( ++this->m_NowFrame >= this->m_MaxFrame )
		{
			//������
			this->m_NowFrame=0;
			//�^��Ԃ�
			return true;
		}
	}
	//�ʏ�J�ڂ�����U��Ԃ�
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//		2D�A�j���[�V�����`��
//
//////////////////////////////////////////////////////////////////////////////////////////
	vector<D3DTKY2DTEXTUREANIMATION*> D3DTKY2DTEXTUREANIMATION::m_vTexAnime;
	D3DTKY2DTEXTUREANIMATION::D3DTKY2DTEXTUREANIMATION()
	{
		D3DTKY2DTEXTUREANIMATION::m_texture = NULL;
		D3DTKY2DTEXTUREANIMATION::m_nowindex = 0;
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}
	D3DTKY2DTEXTUREANIMATION::D3DTKY2DTEXTUREANIMATION(D3DTKY2DTEXTURE* tex)
	{
		D3DTKY2DTEXTUREANIMATION::m_texture = tex;
		D3DTKY2DTEXTUREANIMATION::m_nowindex = 0;
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}
	D3DTKY2DTEXTUREANIMATION::D3DTKY2DTEXTUREANIMATION(const D3DTKY2DTEXTUREANIMATION& texanime)
	{
		D3DTKY2DTEXTUREANIMATION::m_texture = texanime.m_texture;
		D3DTKY2DTEXTUREANIMATION::m_nowindex = 0;
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}
	D3DTKY2DTEXTUREANIMATION::~D3DTKY2DTEXTUREANIMATION()
	{
		for(UINT i=0;i<m_Element.size();i++)
		{
			for(UINT j=0;j<m_Element[i].size();j++)
			{
				m_Element[i].clear();
			}
		}
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}

	void	D3DTKY2DTEXTUREANIMATION::AddElement(vector<ANIMATION> anime)	//�v�f�̒ǉ� �����@�A�j���[�V�����f�[�^,������@�O�F���ꂽ�㎟�ɐi�ށ@�A�@�P�F���ꂽ�㎟�ɐi�܂Ȃ�
	{
		//�f�[�^�����p
		D3DTKY2DTEXTUREANIMATION::m_Element.push_back(anime);

	}
	bool	D3DTKY2DTEXTUREANIMATION::Update()								//�X�V
	{
		bool flag=false;
		//���̓��������
		for(UINT i=0;i<this->m_Element[this->m_nowindex].size();i++)
		{
			//�I����������
			if( ++this->m_Element[this->m_nowindex][i].m_nowloop >= this->m_Element[this->m_nowindex][i].m_loopnum )
			{
				//�I����Œ�
				this->m_Element[this->m_nowindex][i].m_nowloop = this->m_Element[this->m_nowindex][i].m_loopnum;
				//�I���t���O��������
				if(this->m_Element[this->m_nowindex][i].m_endflag)flag = true;
			}
			else	//�I������Ȃ�������
			{
				//�ړ����@
				switch(this->m_Element[this->m_nowindex][i].m_type)
				{
					case MOVE_ANIME:
						this->m_pos += this->m_Element[this->m_nowindex][i].m_value;
					break;
					case MAGNIFY_ANIME:
					break;
					case ROTATE_ANIME:
					break;
					case ALPHA_ANIME:
					break;
				}
			}
		}
		//�G���h�t���O��T��
		if(flag)
		{
			if( ++this->m_nowindex >= (int)this->m_Element.size() )
			{
				//���W������
				this->m_pos = Vector();
				this->m_nowindex = 0;
				//���[�v�󋵂̏�����
				for(UINT d=0;d<this->m_Element.size();d++)
				{
						for(UINT i=0;i<this->m_Element[d].size();i++)
						{
							this->m_Element[d][i].m_nowloop=0;
						}
				}
				//������x�������s��
				Update();
				return true;
			}
		}
		return false;
	}
	void	D3DTKY2DTEXTUREANIMATION::Darw()									//�`�悷��
	{
		D3DTKY2DTEXTUREANIMATION::m_texture->DrawTexture(
			D3DTKY2DTEXTUREANIMATION::m_pos.x,
			D3DTKY2DTEXTUREANIMATION::m_pos.y,0);
	}
	void	D3DTKY2DTEXTUREANIMATION::Darw(HLSLShader *hlsl)									//�`�悷��
	{
		D3DTKY2DTEXTUREANIMATION::m_texture->DrawTextureHLSL(
			D3DTKY2DTEXTUREANIMATION::m_pos.x,
			D3DTKY2DTEXTUREANIMATION::m_pos.y,0,0,hlsl);
	}

//////////////////////////////////////////////////////////////////////////////////////////
//
//		2D�A�j���[�V�����G�t�F�N�g���p
//
//////////////////////////////////////////////////////////////////////////////////////////
//�V�X�e��
vector<CTKYEFFECT*> CTKYEFFECT::m_vIndex;

CTKYEFFECT::CTKYEFFECT()
{
	m_effect.clear();
}
CTKYEFFECT::~CTKYEFFECT()
{
	m_effect.clear();
}

VOID CTKYEFFECT::SetEffect(LPTKYTEXTURE tex,Vector v)
{
	t_effect effect;
	effect.m_anime = D3DTKY2DTEXTURE::CreateTexture(NULL,NULL);
	*effect.m_anime = *tex;
	effect.m_pos = v;
	m_effect.push_back( effect);
}
VOID CTKYEFFECT::SetEffect(LPTKYTEXTUREANIME texanime,Vector v)
{
	t_effect effect;
	effect.m_anime = D3DTKY2DTEXTURE::CreateTexture(NULL,NULL);
	*effect.m_anime = *texanime->GetTexture();
	effect.m_pos = v;
	m_effect.push_back( effect);
}

VOID CTKYEFFECT::Update()
{
	vector<t_effect>::iterator effect = m_effect.begin();
	FOR( ;effect != m_effect.end();effect++ )
	{
		IF( effect->m_anime->Update() )
		{
			effect = m_effect.erase(effect);
			IF(m_effect.empty())break;
		}
	}

}
VOID CTKYEFFECT::Draw()
{

	vector<t_effect>::iterator effect = m_effect.begin();
	FOR( ;effect != m_effect.end();effect++ )
	{
		effect->m_anime->DrawTexture(
			effect->m_pos.x,
			effect->m_pos.y,
			0
			);
	}

}


CTKYEFFECT* CTKYEFFECT::CreateEffect()
{
	CTKYEFFECT * e = new CTKYEFFECT();
	m_vIndex.push_back(e);
	return e;
}
VOID CTKYEFFECT::Destroy()
{
	FOR_I(m_vIndex.size())SAFE_DELETE(m_vIndex[i]);
	m_vIndex.clear();
}