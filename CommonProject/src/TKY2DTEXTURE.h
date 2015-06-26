#pragma once

#include<d3dx9.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include "TKYXFILE.h"
#include "TKYHLSL.h"
#include "TKY3D.h"
#include "TKYLIBRARY.h"

using namespace std;

//===========================================================
//�V�e�N�X�`���N���X
// �ǂݍ��݁A�\���A�A�j���[�V�����ŕ\���A�A�j���[�V�������̍X�V�A�d���p�e�N�X�`���}�b�v
//===========================================================

STRUCT( TTexutureInfo )  
{
public:
	//�摜���
	LPDIRECT3DTEXTURE9		m_pTextures;			//�e�N�X�`�����i�[����
	//���I���
	UINT m_width,m_height;								//�摜�̑傫��
	DWORD							m_AnimeSpeed;			//�A�j���̑J�ڎ���(ms)
	BYTE								m_NowFrame;			//���̃A�j���̃t���[��
	BYTE								m_MaxFrame;				//�t���[���̐�
	BYTE								m_HorizontalDivision;	//��������
	BYTE								m_VerticalDivision;		//�c������
	//�e���|�������
	DWORD							m_NowTime;				//���̎���

	//������
	VOID Initialize()
	{
		m_pTextures = NULL;			//�e�N�X�`�����i�[����
		//���I���
		m_width = NULL;
		m_height = NULL;								//�摜�̑傫��
		m_AnimeSpeed = NULL;			//�A�j���̑J�ڎ���(ms)
		m_MaxFrame = NULL;;				//�t���[���̐�
		m_HorizontalDivision = NULL;;	//��������
		m_VerticalDivision = NULL;;		//�c������
		//�e���|�������
		m_NowFrame = NULL;			//���̃A�j���̃t���[��
		m_NowTime = NULL;				//���̎���
	}

};

//�\��������
enum TKYTEXTURE_TYPE{ TKYTEXTURE_TYPE_3D_MODE , TKYTEXTURE_TYPE_2D_MODE };
//�e�N�X�`���N���X
CLASS_POINTA_OBJECT( CTexture , LPTkyTexture )
{
private:
	//�R���X�g���N�^�A�f�X�g���N�^�A�R�s�[�R���X�g���N�^
	CTexture();
	CTexture(const CTexture&);
	VIRTUAL ~CTexture();

	//��{���
	LPTKY3D m_device;

	//�e�N�X�`�����
	TTexutureInfo m_texture;

	//�A���t�@�����̂��߂�
	STRUCT( TWaitTexture )
	{
		LPTkyTexture m_tex;
		Vector pos;
		Vector rotate;
		Vector expand; 
		TKYTEXTURE_TYPE type; 
		BYTE Index;
	};
	STATIC queue<TWaitTexture> m_DrawWaitTexture;

public:

	//�ǂݍ���
	HRESULT SetTexture( string name ,INT width,INT height);
	HRESULT SetTextureW( wstring name ,INT width,INT height);
	HRESULT SetTexture( string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//�t�@�C�����A���摜���A���摜���A�c�摜��,�摜�̉����A�摜�̏c��,�A�j���[�V�����X�s�[�h
	HRESULT SetTextureW( wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//�t�@�C�����A���摜���A���摜���A�c�摜��,�摜�̉����A�摜�̏c��,�A�j���[�V�����X�s�[�h
	
	//�`��@:�@�ʒu�A�p�x�A���[�v���鎞�ԁ@(wait �E�F�C�g�ɑ����Č�ŕ`�悷��) :�@����������ǂ���
	BOOL DrawTextureIndex(Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , BYTE Index , bool wait = true); 
	BOOL DrawTexture(Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , bool wait = true ); 
	BOOL DrawTextureIndexAlpha(float alpha,Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , BYTE Index , bool wait = true); 
	BOOL DrawTextureAlpha(float alpha,Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , bool wait = true ); 

	//�X�V
	BOOL Update(float step);
	BOOL UpdateIndex(float step,DWORD *time,BYTE *index);

	//�v���p�e�B
	LPTKY3D		GetDevice()const{return m_device;}
	VOID				SetDevice(LPTKY3D d){m_device = d;}
	TTexutureInfo	GetTextureInfo()const{return m_texture;}
	VOID				SetTextureInfo(TTexutureInfo t){m_texture = t;}

	//����(�ł��Ȃ�������NULL��Ԃ�)
	STATIC LPTkyTexture CreateTexture( LPTKY3D device , string name ,INT width,INT height);
	STATIC LPTkyTexture CreateTexture( LPTKY3D device , string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//�t�@�C�����A���摜���A���摜���A�c�摜��,�摜�̉����A�摜�̏c��,�A�j���[�V�����X�s�[�h;
	STATIC LPTkyTexture CreateTextureW( LPTKY3D device , wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//�t�@�C�����A���摜���A���摜���A�c�摜��,�摜�̉����A�摜�̏c��,�A�j���[�V�����X�s�[�h;

	//�`��
	STATIC VOID DrawWaitTexture()
	{
		//���ׂĕ`��
		WHILE( !m_DrawWaitTexture.empty() )
		{
			TWaitTexture t = m_DrawWaitTexture.front();
			t.m_tex->DrawTextureIndex(t.pos,t.rotate,t.expand,t.type,t.Index,false);
			m_DrawWaitTexture.pop();
		}
	}

};

//===========================================================
//�I���
//===========================================================

//-------------------------------------------------------------
// �f�o�b�O�p�ɕ\������e�N�X�`���p�̍\����
//-------------------------------------------------------------
typedef struct {
    FLOAT       p[4];
	D3DCOLOR	diffuse;
    FLOAT       tu, tv;
} TVERTEX;

//-------------------------------------------------------------
// �A�j���[�V�����̍\����
//-------------------------------------------------------------
enum AnimeAction{ MOVE_ANIME , MAGNIFY_ANIME , ROTATE_ANIME , ALPHA_ANIME };
typedef struct
{
	//�ÓI���
	INT				m_type;		//�A�j���[�V�����t���O
	Vector			m_value;	//�l
	DWORD			m_loopnum;   //�ړ���
	//���I���
	DWORD			m_nowloop;	//���̉�
	BOOL				m_endflag;//�I���t���O
}ANIMATION;

//--------------------------------------------------//
//2D�`��̍쐬
//--------------------------------------------------//
class D3DTKY2DTEXTURE
{
private:
	LPDIRECT3D9					m_pD3D;				//DirectX9�@�C���^�[�t�F�C�X
	LPDIRECT3DDEVICE9		m_pD3DDevice;		//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X
	HLSLShader					*m_pHLSL;			//HLSL���
	LPDIRECT3DTEXTURE9		m_pTextures;			//�e�N�X�`�����i�[����
	UINT m_width,m_height;								//�摜�̑傫��
	//���I���
	DWORD							m_AnimeSpeed;			//�A�j���̑J�ڎ���(ms)
	BYTE								m_NowFrame;			//���̃A�j���̃t���[��
	BYTE								m_MaxFrame;				//�t���[���̐�
	BYTE								m_HorizontalDivision;	//��������
	BYTE								m_VerticalDivision;		//�c������
	//�e���|�������
	DWORD							m_NowTime;				//���̎���

	//�V�X�e�����
	static vector<D3DTKY2DTEXTURE*> m_vTex;

	D3DTKY2DTEXTURE(LPTKY3D,LPTKYHLSL);
	VIRTUAL ~D3DTKY2DTEXTURE();

public:
	D3DTKY2DTEXTURE(const D3DTKY2DTEXTURE&);
	HRESULT ReadTexture(LPCTSTR filename,INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//�t�@�C�����A���摜���A���摜���A�c�摜��,�摜�̉����A�摜�̏c��,�A�j���[�V�����X�s�[�h
	BOOL DrawTexture(float x,float y,INT loop);
	BOOL DrawTexture(float x,float y,float alpha,INT loop);
	BOOL DrawTexture3D(float x,float y,float z,INT loop);
	BOOL DrawTextureHLSL(float x,float y,float z,INT loop,LPTKYHLSL);
	BOOL DrawTextureHLSL(float x,float y,float z,float falpha,INT loop,LPTKYHLSL);
	BOOL Update();
	VOID Initialize(){m_NowFrame=0;};
	//�T�C�Y��Ԃ�
	Vector GetSize(){return Vector( (float)m_width , (float)m_height , 0);}
	//���ڃZ�b�g
	VOID SetTexture(LPDIRECT3DTEXTURE9 tex,INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{	
		SAFE_RELEASE(m_pTextures);
		m_pTextures = tex;	
		D3DTKY2DTEXTURE::m_width = width;    // ���i�s�N�Z���j
		D3DTKY2DTEXTURE::m_height = height;   // �����i�s�N�Z���j
		D3DTKY2DTEXTURE::m_MaxFrame = num;
		D3DTKY2DTEXTURE::m_NowFrame = 0;
		D3DTKY2DTEXTURE::m_NowTime = timeGetTime();
		D3DTKY2DTEXTURE::m_AnimeSpeed = speed;
		D3DTKY2DTEXTURE::m_HorizontalDivision = Horizontal;		//��������
		D3DTKY2DTEXTURE::m_VerticalDivision = Vertical;		//�c������
	};
	//���̏���
	VOID Release(){D3DTKY2DTEXTURE::~D3DTKY2DTEXTURE();};

	//�e�N�X�`���̎擾
	static D3DTKY2DTEXTURE *CreateTexture(LPTKY3D d,LPTKYHLSL h){D3DTKY2DTEXTURE *i = new D3DTKY2DTEXTURE(d,h);m_vTex.push_back(i); return i; }
	static VOID Destroy(){FOR_I(m_vTex.size())SAFE_DELETE(m_vTex[i]);}

};
typedef D3DTKY2DTEXTURE* LPTKYTEXTURE;

//--------------------------------------------------//
//2D�`��A�j���[�V�����̍쐬
//���s�̓O���[�o���ƃ��[�J������Ȃ�
//--------------------------------------------------//
class D3DTKY2DTEXTUREANIMATION
{
private:
	LPTKYTEXTURE					   m_texture;		//���C���̉摜
	vector<vector<ANIMATION>>	m_Element;		//�s���̗v�f
	INT										m_nowindex;		//���̍s��

	Vector									m_pos;				//���̕`��ʒu

	//�V�X�e�����
	static vector<D3DTKY2DTEXTUREANIMATION*> m_vTexAnime;

	D3DTKY2DTEXTUREANIMATION();
	D3DTKY2DTEXTUREANIMATION(LPTKYTEXTURE);
	VIRTUAL ~D3DTKY2DTEXTUREANIMATION();

public:

	D3DTKY2DTEXTUREANIMATION(const D3DTKY2DTEXTUREANIMATION&);

	void	AddElement(vector<ANIMATION>);	//�v�f�̒ǉ� �����@�A�j���[�V�����f�[�^,������@�O�F���ꂽ�㎟�ɐi�ށ@�A�@�P�F���ꂽ�㎟�ɐi�܂Ȃ�
	bool	Update();								//�X�V
	void	Darw();									//�`�悷��
	void	Darw(HLSLShader *hlsl);									//�`�悷��

	//�擾
	LPTKYTEXTURE GetTexture(){return m_texture;}

	//�e�N�X�`���̎擾
	static D3DTKY2DTEXTUREANIMATION *CreateTextureAnime(){D3DTKY2DTEXTUREANIMATION *i = new D3DTKY2DTEXTUREANIMATION();m_vTexAnime.push_back(i); return i; }
	static D3DTKY2DTEXTUREANIMATION *CreateTextureAnime(LPTKYTEXTURE t){D3DTKY2DTEXTUREANIMATION *i = new D3DTKY2DTEXTUREANIMATION(t);m_vTexAnime.push_back(i); return i; }
	static VOID Destroy(){FOR_I(m_vTexAnime.size())SAFE_DELETE(m_vTexAnime[i]);}

};
typedef D3DTKY2DTEXTUREANIMATION* LPTKYTEXTUREANIME;

//--------------------------------------------------//
//2D�`��A�j���[�V�����̃G�t�F�N�g���p�N���X
//--------------------------------------------------//
typedef struct 
{
	LPTKYTEXTURE m_anime;
	Vector m_pos;
}t_effect;
class CTKYEFFECT
{
private:

	//�l�ێ�
	vector<t_effect> m_effect;

	//�V�X�e��
	static vector<CTKYEFFECT*> m_vIndex;

	CTKYEFFECT();
	VIRTUAL ~CTKYEFFECT();

public:

	VOID SetEffect(LPTKYTEXTURE,Vector);
	VOID SetEffect(LPTKYTEXTUREANIME,Vector);

	VOID Update();
	VOID Draw();

	static CTKYEFFECT* CreateEffect();
	static VOID Destroy();
};
typedef CTKYEFFECT* LPTKYEFFECT;

