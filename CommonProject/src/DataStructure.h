#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "vector"

#include "TkyPhysics.h"
#include "TKY2DTEXTURE.h"
#include "TKYFONT.h"


using namespace std;

#define GetModelMessageP( pmodel ) pmodel->GetCollisionMessage().m_message

//================
//�@�f�[�^�݌v���Q�l
//================

static void all()
{
}

//============================================================
//�Q�[���̃f�[�^�\��
//============================================================

//�ǂݍ��݃f�[�^
STRUCT( TGameText )
{
	wstring		m_key;
	wstring		m_text;
	DWORD m_element;

	VOID Initialize()
	{
		m_key.clear();
		m_text.clear();
		m_element = NULL;
	}

	TGameText(){Initialize();}
	TGameText( wstring k ,wstring t , DWORD e )
	{
		m_key = k;
		m_text = t;
		m_element = e;
	}
};
typedef vector<TGameText> TGameTextInfo;

//�Q�[�����[�h�C���^�[�t�F�C�X
interface IGameLoadText
{
public:
	VIRTUAL BOOL onLoadGameDate(TGameTextInfo info) = 0;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�G�t�F�N�g���
//	�G�t�F�N�g���͂��ꂩ��A���Ԃɂ��A��]�ⓧ�߂ȂǁA�ǂ�ǂ�i�����Ă����\��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//�G�t�F�N�g�f�[�^
STRUCT( TGameEffect )
{
	LPTkyTexture	m_texture;	//�摜���
	DWORD			m_life;				//�A�j���[�V�����̎���(���[�v��)
	
	BYTE					m_NowTexFrame;
	DWORD				m_NowTexTime;

	VOID Initialize()
	{
		m_texture = NULL;
		m_life = NULL;
		m_NowTexFrame = 0;
		m_NowTexTime = 0;
	}
	TGameEffect(){Initialize();}
	TGameEffect(LPTkyTexture texture , DWORD life)
	{
		Initialize();
		m_texture = texture;
		m_life = life;
	}
};
//�G�t�F�N�g�s���N���X
enum eTky_State{TKYSTATE_RUN,TKYSTATE_STOP};
CLASS_POINTA( CGameEffect , LPTkyGameEffect )
{
private:
	TGameEffect m_effect;
	//���
	UINT m_state;
	//���[�v��񂵂����t���O
	bool m_oneLoop;
public:
	//�R���X�g���N�^�A�f�X�g���N�^�A
	CGameEffect();
	CGameEffect( const CGameEffect& );
	VIRTUAL ~CGameEffect();

	BOOL CreateEffect(TGameEffect);
	BOOL UpdateEffect(float step);
	BOOL DrawEffect(Vector pos,Vector rotate , Vector expand);
	BOOL DrawEffect2D(Vector pos);

	VOID RUN(){m_state = TKYSTATE_RUN;};
	VOID STOP(){m_state = TKYSTATE_STOP;};

	bool					IsOneLoopEnd(){return m_oneLoop;}

	//�ݒ�擾
	VOID				SetEffectInfo(TGameEffect t){m_effect = t;}	
	TGameEffect	GetEffectInfo()const{return m_effect;}

};
//�G�t�F�N�g�Ǘ��N���X
CLASS_POINTA( CManageEffect , LPTkyManageEffect )
{
private:
	//�A�j���[�V�����̃}�b�`���
	CStringMatchW< CGameEffect > m_EffectMatch;
public:

	//�R���X�g���N�^�A�f�X�g���N�^
	CManageEffect();
	CManageEffect(const CManageEffect&);
	VIRTUAL ~CManageEffect();

	//�ǉ�
	BOOL AddEffectData( wstring,LPTkyTexture , UINT life );
	BOOL AddEffectData( wstring, TGameEffect* );
	//����
	BOOL DeleteEffectData( wstring );
	//�X�V(���ۂ͎g�p����K�v���Ȃ�)
	BOOL UpdateEffectData( wstring , float step );
	//�Đ�
	BOOL DrawEffectData( wstring , Vector vec , Vector rotate , Vector expand );
	BOOL DrawEffectData2D( wstring , Vector vec );
	//�ݒ�
	BOOL RunEffectData( wstring );
	BOOL StopEffectData( wstring );
	//�擾
	LPTkyGameEffect GetEffect(wstring name );

	//���[�v���I����Ă��邩����
	BOOL IsFinish( wstring );

	//�擾�ݒ�
	CStringMatchW< CGameEffect >	GetEffectMatch()const{return m_EffectMatch;}
	VOID										SetEffectMatch(CStringMatchW< CGameEffect > am){ m_EffectMatch = am; }

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�L�����N�^���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//-----------------------------
//�A�j���[�V�������
//-----------------------------
//�A�j���[�V�����\����
STRUCT( TGameAnimation )
{
	LPTkyXFile m_model;
	FLOAT m_NowFrame;
	VOID Initialize()
	{
		m_model = NULL;
		m_NowFrame = NULL;
	}

	TGameAnimation(){Initialize();}
	TGameAnimation( LPTkyXFile model , FLOAT NowFrame )
	{
		m_model = model;
		m_NowFrame = NowFrame;
	}
};
//�A�j���[�V�����s���N���X
CLASS_POINTA( CGameAnimation , LPTkyGameAnimation )
{
private:
	TGameAnimation m_Animation;
	bool m_oneLoop;
public:
	//�R���X�g���N�^�A�f�X�g���N�^�A
	CGameAnimation();
	CGameAnimation( const CGameAnimation& );
	VIRTUAL ~CGameAnimation();

	BOOL CreateAnimation(TGameAnimation);
	BOOL UpdateAnimation(float step);
	BOOL DrawAnimation(Vector pos,Vector rotate , Vector expand);

	//�ݒ�擾
	VOID				SetAnimationInfo(TGameAnimation t){m_Animation = t;}	
	TGameAnimation	GetAnimationInfo()const{return m_Animation;}

	//������
	VOID				InitializeFrame(){m_Animation.m_NowFrame = 0;};

	bool					IsOneLoopEnd(){return m_oneLoop;}

};
//�A�j���[�V�����Ǘ��N���X
//TKYMODEL_STAND�@���@�����Ă���
CLASS_POINTA( CManageAnimation , LPTkyManageAnimation )
{
private:
	//�A�j���[�V�����̃}�b�`���
	CStringMatchW< CGameAnimation > m_XAnimeMatch;
public:

	//�R���X�g���N�^�A�f�X�g���N�^
	CManageAnimation();
	CManageAnimation(const CManageAnimation&);
	VIRTUAL ~CManageAnimation();

	//������
	BOOL InitializeAnimationData( wstring);
	//�ǉ�
	BOOL AddAnimationData( wstring,LPTkyXFile );
	//����
	BOOL DeleteAnimationData( wstring );
	//�X�V
	BOOL UpdateAnimationData( wstring ,float step);
	//�Đ�
	BOOL DrawAnimationData( wstring , Vector vec , Vector rotate , Vector expand );

	//���[�v���I����Ă��邩����
	BOOL IsFinish( wstring );

	//�擾�ݒ�
	CStringMatchW<CGameAnimation >	GetAnimeMatch()const{return m_XAnimeMatch;}
	VOID										SetAnimeMatch(CStringMatchW< CGameAnimation > am){ m_XAnimeMatch = am; }

};

//-----------------------------
//�L�������
//-----------------------------
STRUCT( TCharaInfo )
{
	wstring m_name;	//���O

	INT m_HitPoint;	//�̗�
	INT m_Velocity;	//���x
	FLOAT m_Length;	//�傫��
	FLOAT m_CollisonLength;	//�傫��

	wstring m_ai;					//AI
	wstring m_automove;		//�ړ�������

	//�\������Ƃ��̉�]
	Vector m_rotate;
	//�U������
	Vector m_AttackDirection;

	//�����p�x
	Vector m_first;

	VOID Initialize()
	{
		m_name = TEXT("");
		m_Length = NULL;
		m_HitPoint = m_Velocity = NULL;
		m_CollisonLength = NULL;
		m_ai.clear();
		m_automove.clear();
		m_rotate = Vector(0,0,1);
		m_AttackDirection = Vector(0,0,1);
		m_first = Vector(0,0,1);
	}
	TCharaInfo(){Initialize();}
	TCharaInfo( wstring name,INT h , INT v, FLOAT l ,wstring ai   )
	{
		m_name = name;
		m_HitPoint = h;
		m_Velocity = v;
		m_Length = l;
		m_ai = ai;
	}
};
//���C���L�����N�^�[�N���X
CLASS_POINTA( CCharactor , LPTkyCharactor ) :public PhysicsCircleModel , IGameLoadText
{
private:

//�Ǝ��v�f

	//�R���X�g���N�^�A�f�X�g���N�^
	CCharactor();
	CCharactor(const CCharactor&);
	VIRTUAL ~CCharactor();

	//�A�j���[�V�������
	CManageAnimation m_AnimationData;
	//�G�t�F�N�g�Ǘ�
	CManageEffect m_EffectData;

	//�L�������
	TCharaInfo m_chara;

	//���̏�� ( CHARACTOR_STAND ) 
	string m_state;	

public:

//PhysicsCircleModel�v�f

	//�ڐG���
	VIRTUAL bool onCollisionEvent(PhysicsModel *model);

//�Ǝ��v�f

	//�X�V
	BOOL UpdateCharactor( float step );
	//�`��(�ʒu����PhysicsCircleModel����Ƃ�)
	VOID DrawCharactor();

	//�C�x���g(�����Ă����C�x���g�ɂ��G�t�F�N�g�C�x���g�ł��쐬����)
	VOID EventCharactor( string name, DWORD message );

	//�_���[�W
	VOID Damage( float damage);

	//�擾�ݒ�
	CManageAnimation GetManageAnime()const{return m_AnimationData;}
	CManageEffect GetManageEffect()const{return m_EffectData;}
	VOID SetManageAnime(CManageAnimation a){m_AnimationData = a;}
	VOID SetManageEffect(CManageEffect e){m_EffectData = e;}
	string		GetState()const{return m_state;}
	VOID		SetState(string s){m_state = s;}
	TCharaInfo	GetCharaInfo()const{return m_chara;}
	VOID			SetCharaInfo(TCharaInfo c)
	{
		m_chara = c;
		SetCircle( c.m_Length );
	}

	CManageAnimation GetAnime(){return m_AnimationData;}

	//�m�F
	BOOL					IsDead(){return ( m_chara.m_HitPoint <= 0 && m_state == "CHARACTOR_NULL" );}

	//�ˑ��擾
	Vector GetNowRotate()
	{
		Vector v = Vector();
		v.y = m_vel.GetRadVec().y;
		return v;
	}

//����
	STATIC LPTkyCharactor CreateCharactor()
	{
		return new CCharactor();
	}

//IgameLoadText�v�f

	VIRTUAL BOOL onLoadGameDate(TGameTextInfo info);

};

//-----------------------------
//�U���i�e�j
//�@�r���[�Ƃ��ēo�^���ꂠ���������ǂ����Ȃ�(�ŏ��ɐ������ꂸ�@����ɂ���č����)
//-----------------------------
//�U�����
STRUCT( TBulletInfo ) 
{
	INT m_AttackPower;	//�U����
	FLOAT m_AttackSpeed;	//�U�����x(�����܂ł̔��Έړ������݂����Ȃ�)
	FLOAT m_AttackRange;	//�U���͈�
	FLOAT m_Life;				//�����i�e�̏ꍇ�ړ����������ɂȂ�,�����Ɖi���j

	VOID Initialize()
	{
		m_AttackPower  = 0;
		m_Life = m_AttackRange = m_AttackSpeed = 0;
	}
	TBulletInfo(){Initialize();}
	TBulletInfo(INT ap,FLOAT as,FLOAT ar , FLOAT l)
	{
		m_AttackPower = ap;
		m_AttackRange = ar;
		m_AttackSpeed = as;
		m_Life = l;
	}
};
//�U���N���X
CLASS_POINTA( CBullet , LPTkyBullet ) :public PhysicsCircleModel
{
private:

//�Ǝ��v�f

	//�R���X�g���N�^�A�f�X�g���N�^
	CBullet();
	CBullet(const CBullet&);
	VIRTUAL ~CBullet();
	
	//�U�����
	TBulletInfo m_Bullet;

	//�����ʒu
	Vector m_OriginPos;


	//���˃L����
	LPTkyCharactor m_BatteryChara;

	//�I���t���O
	bool m_end;

	//�o���ʒu
	Vector m_BatteryPosition;

public:

//PhysicsCircleModel�v�f

	//�ڐG���
	VIRTUAL bool onCollisionEvent(PhysicsModel *model);

	//���ˈ�ݒ�
	VOID SetBatteryPosition(Vector v){m_BatteryPosition = v;}

	//�I�����Ă��邩�擾
	bool IsFinish();

	//�擾�ݒ�
	TBulletInfo	GetBulletInfo()const{return m_Bullet;}
	VOID			SetBulletInfo(TBulletInfo bi)
	{ 
		m_Bullet = bi; 
		SetCircle( (float)m_Bullet.m_AttackRange );
	}

	LPTkyCharactor GetChara(){return m_BatteryChara;}
	VOID SetChara(LPTkyCharactor chara){m_BatteryChara = chara;}

	//����
	STATIC LPTkyBullet CreateBullet()
	{
		return new CBullet();
	}
	STATIC LPTkyBullet CreateBullet( TBulletInfo b , LPTkyCharactor chara )
	{
		LPTkyBullet c = CreateBullet();
		c->SetBulletInfo(b);
		c->SetChara(chara);
		return c;
	}

};


//-----------------------------
//����
//�@�r���[�Ƃ��ď풓���A�U�����x���g�ɂ��U������
//	 *�I�����Ƀt���[�����~�܂�悤�ɐ݌v����Ă��Ȃ�
//-----------------------------
//������
//�@�̂��Ƀ^�C�~���O�ȊO���ǉ����A���悢���ƂȂ��Ă���
STRUCT( TWeaponInfo ) 
{

	//�U�����e�x��(�A�j���[�V�������n�܂��Ă�����ۂɔ��˂���܂ł̎���)
	FLOAT m_FiringBulletsLate;		

	VOID Initialize()
	{
		m_FiringBulletsLate = NULL;
	}
	TWeaponInfo(){Initialize();}
	TWeaponInfo( FLOAT f)
	{
		m_FiringBulletsLate = f;
	}
};
//����N���X
//
CLASS_POINTA_OBJECT( CWeapon , LPTkyWeapon ) , IGameLoadText
{
private:

//�Ǝ��v�f

	//�R���X�g���N�^�A�f�X�g���N�^
	CWeapon();
	CWeapon(const CWeapon&);
	VIRTUAL ~CWeapon();
	
	//�U�����(��������Ƃ��ɃN���X�̕Ԋ҂���)
	TBulletInfo m_Bullet;
	//������
	TWeaponInfo m_Weapon;

	//�A�j���[�V�������
	CManageAnimation m_AnimationData;
	//�G�t�F�N�g�Ǘ�
	CManageEffect m_EffectData;

	//�������  (�������Ă��Ȃ��@�A�@���ˏ����@�A�@���ˁ@�A�@�e���܂������Ă���)�݂�����
	//WEAPON_SHOT , WEAPON_PRELAUNCH , WEAPON_SHOT
	string m_NowState;		

	//jikann 
	float m_time;

	//������g�p����L�����N�^�̏��
	LPTkyCharactor m_chara;

	//�U�����̃}�E�X�̈ʒu
	Vector m_pos;

	//�U�����
	VOID pAttack();

public:

	//�U��
	BOOL Attack(Vector);
	//�U�����Ȃ�e���ǂ����Ő�������
	BOOL UpdateAttack(float step);
	//�U�����Ȃ�`��
	VOID DrawAttack();
	//�U�������I������C�x���g
	BOOL EndAttack();

	//�擾�ݒ�
	TBulletInfo			GetBulletInfo()const{return m_Bullet;}
	VOID					SetBulletInfo(TBulletInfo bi){ m_Bullet = bi; }
	TWeaponInfo		GetWeaponInfo()const{return m_Weapon;}
	VOID					SetWeaponInfo(TWeaponInfo bi){ m_Weapon = bi; }
	string					GetNowState()const{return m_NowState;}
	VOID					SetNowState(string s){m_NowState = s;}
	LPTkyCharactor	GetChara()const{return m_chara;}
	VOID					SetChara(LPTkyCharactor c){m_chara=c;}


	//����
	STATIC LPTkyWeapon CreateWeapon()
	{
		return new CWeapon();
	}
	STATIC LPTkyWeapon CreateWeapon( LPTkyCharactor chara )
	{
		LPTkyWeapon w =  new CWeapon();
		w->SetChara( chara );
		return w;
	}

//IgameLoadText�v�f

	VIRTUAL BOOL onLoadGameDate(TGameTextInfo info);

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�}�b�v���(��Q��)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�܂����؂������͂��Ă��Ȃ�
CLASS_POINTA( CGameMap , LPTkyGameMap )
{
private:

	CGameMap();
	CGameMap(const CGameMap&);
	VIRTUAL ~CGameMap();

	//�}�b�v���X�t�@�C��
	LPTkyXFile m_model;

public:



};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�C�x���g���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//-----------------------------
//�e�L�X�g���
//		�e�L�X�g�摜�ƁA���͂Ȃǂ�ݒ肷��Əo�Ăė���
//-----------------------------
//�����G���
STRUCT( TTextWindowCharaInfo )
{
	LPTkyTexture m_tex;
	Vector m_pos;

	VOID Initialize()
	{
		m_tex = NULL;
		m_pos = Vector();
	}
	TTextWindowCharaInfo()
	{
		Initialize();
	}
	TTextWindowCharaInfo(LPTkyTexture tex,Vector p)
	{
		m_tex = tex;
		m_pos = p;
	}
};
//�ړ����͍��̂Ƃ���g��̂�
STRUCT( TTextWindowInfo )
{
	LPTkyTexture		m_WindowTexture;					//�E�B���h�E�̉摜	
	Vector					m_textpos;								//�E�B���h�E�̍��ォ��̕\�����鑊�΋���

	//�����G���
	vector<TTextWindowCharaInfo> m_BackChara;

	CVector				m_WindowPosEndStart;			//�ړ����
	CVector				m_WindowRotateEndStart;		//��]���
	CVector				m_WindowExpandEndStart;	//�g����
	CVector				m_WindowAlphaEndStart;		//���ߏ��

	TTextWindowInfo()
	{
		m_WindowTexture = NULL;
		m_textpos = Vector();
		m_WindowPosEndStart = CVector();
		m_WindowRotateEndStart = CVector();
		m_WindowExpandEndStart = CVector();
		m_WindowAlphaEndStart = CVector();
		m_BackChara.clear();
	}
	TTextWindowInfo( LPTkyTexture tex,Vector tp,CVector p,CVector r,CVector e,CVector a , vector<TTextWindowCharaInfo> c = vector<TTextWindowCharaInfo>() )
	{
		m_WindowTexture = tex;
		m_textpos = tp;
		m_WindowPosEndStart = p;
		m_WindowRotateEndStart = r;
		m_WindowExpandEndStart = e;
		m_WindowAlphaEndStart = a;
		m_BackChara = c;
	}
};
//�e�L�X�g���
//�@�ꕶ���̑傫���Ȃǂ́A�t�H���g�N���X����
STRUCT( TTextInfo )
{
	RECT m_Render;					//�E�B���h�E���ŕ�����������͈�
	wstring		m_string;				//������
	wstring		m_name;				//�w�b�h������
	FLOAT		m_ShowSpeed;	//�\���X�s�[�h
	TTextInfo()
	{
		m_Render = RECT();
		m_string = TEXT("");
		m_ShowSpeed = NULL;
	}
	TTextInfo( RECT r,wstring w , wstring name ,FLOAT ss )
	{
		m_Render = r;
		m_string = w;
		m_name = name;
		m_ShowSpeed = ss;
	}
};
//���̃Z�b�g
STRUCT( TTextSet )
{
	TTextInfo		m_text;
	TTextWindowInfo *m_window;

	VOID Initialize()
	{
		m_text = TTextInfo();
		m_window = NULL;
	}
	TTextSet()	{Initialize();}
	TTextSet(TTextInfo text,TTextWindowInfo *window)
	{
		m_text = text;
		m_window = window;
	}
};


//�����\���N���X
// �����̕\�����󂯎��A
CLASS( CShowText )
{
private:
	//�e�L�X�g���
	TTextInfo m_text;
	//���̕\������Ă��镶����(��ŕ�����)
	vector<wstring> m_ShowString;
	//���̕\��������
	FLOAT	m_NowCharacterNum;
public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CShowText();
	CShowText(const CShowText&);
	VIRTUAL ~CShowText();

	//�����̏�񏉊���
	VOID	InitializeText();
	//�X�V(�I�����Ă�����^��Ԃ�)
	bool		UpdateText(float step);
	//�`��
	VOID	DrawShowText(Vector pos);

	//���ׂČ�����
	VOID AllShow(){ WHILE(!UpdateText(1.0f)); }

	//�ݒ�擾
	TTextInfo	GetTextInfo(){return m_text;}
	VOID	SetTextInfo(TTextInfo t){m_text = t;}
};
//�E�B���h�E�\���N���X
enum eTKYSHOWWINODW{ TKYSHOWWINODW_NULL , TKYSHOWWINODW_ENTER , TKYSHOWWINODW_LEAVE , TKYSHOWWINODW_SHOW };
CLASS( CShowWindow )
{
private:
	//�E�B���h�E���
	TTextWindowInfo *m_window;
	//���̎���
	FLOAT m_time;
	//�E�B���h�E�̏��
	eTKYSHOWWINODW m_state;
public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CShowWindow();
	CShowWindow( const CShowWindow& );
	VIRTUAL ~CShowWindow();

	//������
	VOID	InitializeTextWindow( eTKYSHOWWINODW state );
	//�X�V(�I�����ɐ^)
	bool		UpdateTextWindow(float step);
	//�`��
	VOID	DrawTextWindow();
	//�e�L�X�g�̈ʒu�̎擾
	Vector GetTextPos();

	//�ݒ�擾
	TTextWindowInfo GeTTextWindowInfoInfo(){return *m_window;}
	VOID	 SeTTextWindowInfo(TTextWindowInfo *t){m_window = t;}
};

//�����ƃE�B���h�E�̓��@�N���X
enum eManageTextWindow{ TKYMTW_NULL , TKYMTW_WINODW_ENTER , TKYMTW_TEXT , TKYMTW_WINODW_LEAVE};
CLASS_POINTA_OBJECT( CManageTextWindow , LpTkyManageTextWindow ) , IGameLoadText
{
private:
	//�R���X�g���N�^�A�f�X�g���N�^
	CManageTextWindow();
	CManageTextWindow(const CManageTextWindow&);
	VIRTUAL ~CManageTextWindow();

	//�e�L�X�g���
	CShowText			m_Textinfo;
	//�E�B���h�E���
	CShowWindow		m_Winodwinfo;
	//���̏��
	eManageTextWindow m_state;

public:

	//�J�n
	BOOL StartTextWinodw();
	BOOL StartTextWinodw(eManageTextWindow);
	//�X�V(�I�����ɐ^)
	BOOL UpdateTextWinodw( float step );
	//�`��
	VOID DrawTextWindow();

	//��C�ɏI��������
	VOID EndTextWinodw(  );


	//�ݒ�
	VOID SetTextInfo( CShowText text , CShowWindow window )
	{
		m_Textinfo = text;
		m_Winodwinfo = window;
	}
	VOID SetTextInfo( TTextInfo text , TTextWindowInfo *window )
	{
		m_Textinfo.SetTextInfo( text );
		m_Winodwinfo.SeTTextWindowInfo( window );
	}

	//�擾
	eManageTextWindow GetState(){return m_state;}

	//����
	STATIC LpTkyManageTextWindow CreateManageTextWinodw(){return new CManageTextWindow;}

	//���̃Z�b�g
	VIRTUAL BOOL onLoadGameDate(TGameTextInfo text);
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�͈̓N���X
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�͈͏��\����
STRUCT( TRange )
{
	Vector pos;
	FLOAT r;
	TRange()
	{
		pos = Vector();
		r = 0;
	}
	TRange( Vector v, FLOAT r1 )
	{
		pos = v;
		r = r1;
	}
};

//�͈͏��
template < class T >
STRUCT( TElementRange ) :public TRange
{
	//�e�L�X�g���queue	
	queue<T> m_Element;

	TElementRange()
	{
		//������
		WHILE(!m_Element.empty())m_Element.pop();
	}
	TElementRange( Vector v, FLOAT r1 , queue<T> t )
	{
		pos = v;
		r = r1;
		m_Element = t;
	}
};
//�͈͊Ǘ��N���X
template < class T >
CLASS( CElementRange )
{
private:
	//�C�x���g���
	list<TElementRange<T>> m_Element;
public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CElementRange()
	{
		m_Element.clear();
	}
	CElementRange(const CElementRange& t)
	{
		m_Element = t.m_Element;
	}
	VIRTUAL ~CElementRange()
	{
		m_Element.clear();
	}

	//�͈͏��̒ǉ�
	VOID AddRange(TElementRange<T> t)
	{
		m_Element.push_back(t);
	}

	//����
	TElementRange<T> Decision(Vector p)
	{
		list<TElementRange<T>>::iterator im = m_Element.begin();
		WHILE( im != m_Element.end() )
		{
			float l = ( Vector( im->pos.x , 0 , im->pos.z ) - Vector( p.x , 0 , p.z ) ).GetLarge();
			//�͈͓���������
			IF( l < im->r )
			{
				return *im;
			}
			im++;
		}
		return TElementRange<T>();
	}

	//�͈͂̏���
	VOID DeleteRange( Vector p )
	{
		list<TElementRange<T>>::iterator im = m_Element.begin();
		WHILE( im != m_Element.end() )
		{
			float l = ( Vector( im->pos.x , 0 , im->pos.z ) - Vector( p.x , 0 , p.z ) ).GetLarge();
			//�͈͓���������
			IF( l < im->r )
			{
				im = m_Element.erase( im );
				return;
			}
			im++;
		}
	}

	//�擾
	list<TElementRange<T>> GetElement(){ return m_Element;}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�����ړ��N���X
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�L�[���
enum eTkyAutoKey{ TKYKEY_UP = 0x01 , TKYKEY_DOWN = 0x02 , TKYKEY_LEFT = 0x04 , TKYKEY_RIGHT = 0x08 };
STRUCT( TAutoKeyInput )
{
	//�L�[���
	UINT m_eTkyAutoKey;
	//�t���[����
	DWORD m_FrameNum;
	//
	TAutoKeyInput()
	{
		m_eTkyAutoKey = NULL;
		m_FrameNum = NULL;
	}
	TAutoKeyInput(UINT e,DWORD f)
	{
		m_eTkyAutoKey = e;
		m_FrameNum = f;
	}
};
//�}�E�X���
STRUCT( TAutoMouseInput )
{
	//�L�[���
	Vector m_MousePos;
	//�t���[����
	DWORD m_FrameNum;
	//
	TAutoMouseInput()
	{
		m_MousePos = Vector();
		m_FrameNum = NULL;
	}
	TAutoMouseInput(Vector cv,DWORD f)
	{
		m_MousePos = cv;
		m_FrameNum = f;
	}
};
//�����ړ�����N���X
CLASS_POINTA_OBJECT( CAutoKeyInput , LPTkyAutoKeyInput )
{
private:
	CAutoKeyInput();
	CAutoKeyInput(const CAutoKeyInput&);
	VIRTUAL ~CAutoKeyInput();

	//�L�[���
	queue<TAutoKeyInput> m_KeyInfo;
	//�t���[�����
	DWORD m_NowFrame;

public:

	//�L�[���̒ǉ�
	VOID AddKeyInput( TAutoKeyInput key );
	//���̃L�[���̎擾
	TAutoKeyInput GetNowKeyInput();
	//�t���[����i�߂�(�����i�܂Ȃ�������FALSE��Ԃ�)
	BOOL NextFrame();

	//����
	STATIC LPTkyAutoKeyInput CreateAutoKeyInput()
	{
		return new CAutoKeyInput();
	}

	//�擾
	queue<TAutoKeyInput> GetKeyInfo(){return m_KeyInfo;}

	//���̃t���[�����
	DWORD GetNowFrame(){return m_NowFrame;}

};
//�������_����N���X
CLASS_POINTA_OBJECT( CAutoMouseInput , LPTkyAutoMouseInput )
{
private:
	CAutoMouseInput();
	CAutoMouseInput(const CAutoMouseInput&);
	VIRTUAL ~CAutoMouseInput();

	//�L�[���
	queue<TAutoMouseInput> m_MouseInfo;
	//�t���[�����
	DWORD m_NowFrame;

public:

	//�}�E�X���̒ǉ�
	VOID AddMouseInput( TAutoMouseInput Mouse );
	//�}�E�X���̒ǉ�
	VOID AddMouseInput( Vector , DWORD );
	//���̃}�E�X�̈ʒu�擾
	Vector GetNowMouseInput();
	//�t���[����i�߂�(�����i�܂Ȃ�������FALSE��Ԃ�)
	BOOL NextFrame();

	//����
	STATIC LPTkyAutoMouseInput CreateAutoMouseInput()
	{
		return new CAutoMouseInput();
	}

	//�擾
	queue<TAutoMouseInput> getNouseInof(){return m_MouseInfo;}

	//���̃t���[�����
	DWORD GetNowFrame(){return m_NowFrame;}

};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�^�C�g���N���X
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TTitle )
{
	LPTkyTexture m_logo;
	LPTkyTexture m_back;
	LPTkyTexture m_start;
	LPTkyTexture m_score;
	LPTkyTexture m_ranking;
	TTitle()
	{
		m_logo = NULL;
		m_back= NULL;
		m_start= NULL;
		m_score= NULL;
		m_ranking = NULL;
	}
};
CLASS_POINTA_OBJECT( CTitle , LPTkyTitle )
{
private:

	TTitle m_title;

	//TITLE_LOGO,TITLE_BACK,TITLE_END,TITLE_SCORE
	wstring m_state;

	Vector pos;

	DWORD m_Score[3];

	CTitle();
	CTitle(const CTitle&);
	VIRTUAL ~CTitle();
public:

	VOID UpdateTitle(float step);
	VOID DrawTitle();

	VOID Next();
	BOOL IsEnd();

	TTitle GetTitle(){return m_title;}
	VOID SetTitle(TTitle t){m_title  = t;}

	VOID SetPos(Vector t){pos = t;}

	wstring GetState(){return m_state;}

	STATIC LPTkyTitle CreateTitle( TTitle t )
	{
		LPTkyTitle ti = new CTitle();
		ti->SetTitle(t);
		return ti;
	}
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�X�R�A�N���X
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eScore{ TKYSCORE_START,TKYSCORE_PLAY,TKYSCORE_RESULT,TKYSCORE_END,TKYSCORE_GAMEOVER,TKYSCORE_RESET };
STRUCT( TScore )
{
	DWORD m_score;				//�X�R�A
	DWORD m_BatteryNum;		//���ː�
	DWORD m_HitNum;			//�q�b�g��

	DWORD m_time;					//����
	DWORD m_starttime;					//�J�n����

	LPTkyCharactor m_chara;	//�L�������
	
	//���ʉ�ʂ̔w�i
	LPTkyTexture	m_ScoreBack;

	//�V���[�e�B���O���̃X�R�A
	LPTkyTexture m_ShootingScore;

	//���ʉ�ʂ̔w�i
	LPTkyTexture	m_Gameover;

	//���̏��
	eScore m_state;

	VOID initialize()
	{
		m_score = NULL;
		m_BatteryNum = NULL;
		m_HitNum = NULL;
		m_time = NULL;
		m_chara = NULL;
		m_state = TKYSCORE_START;
		m_starttime = NULL;
		m_Gameover = NULL;
	}
};

CLASS_POINTA_OBJECT( CScore , LPTkyScore )
{
private:

	TScore m_score;

	CScore();
	CScore(const CScore&);
	VIRTUAL ~CScore();
public:

	VOID UpdateScore(float step);
	VOID DrawScore();

	TScore GetScore(){return m_score;}
	VOID SetScore(TScore t){m_score = t;}

	STATIC LPTkyScore CreateScore(TScore t)
	{
		LPTkyScore s = new CScore();
		s->SetScore(t);
		return s;
	}

};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�V�F�C�h�N���X
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eShade{ TKYSHADE_OUT , TKYSHADE_MIDDLE , TKYSHADE_IN , TKYSHADE_NULL };
CLASS( CShade )
{
private:
	LPTkyTexture m_shade;
	eShade m_State;
	float m_alpha;

	//������������ID
	DWORD m_run_id;

public:

	CShade();
	CShade(const CShade&);
	VIRTUAL ~CShade();

	VOID Read(wstring name);

	VOID UpdateShade(float step);
	VOID DrawShade();

	//�A�b�v�f�[�g���Ă�������
	bool IsUpdate();

	//�͂��߂�
	VOID Run(DWORD id);

};