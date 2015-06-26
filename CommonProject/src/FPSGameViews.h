#pragma once

#include "TKYBASEHEADER.h"
#include "DataStructure.h"
#include "TkyLogicView.h"
#include "TKYDIRECTINPUT.h"
#include "TKYFONT.h"

#include <fstream>

using namespace std;

//�r���[������
#define ADD_VIEW_MESSAGE(view) CEventManager::StaticAddEventMessage( "LOGIC_ADDVIEW" , (DWORD)(view) );
#define DELETE_VIEW_MESSAGE(view) CEventManager::StaticAddEventMessage( "LOGIC_DELETEVIEW" , (DWORD)(view) );

static BOOL _FPSGameViews(){return true;};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�L�����N�^�[�r���[
//�@�L�����N�^���̊�{�ƂȂ��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IFPSCharactorView , LPTkyFPSCharactorView ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	IFPSCharactorView();
	IFPSCharactorView(const IFPSCharactorView&);
	VIRTUAL ~IFPSCharactorView();

	//�L�����N�^�[�f�[�^
	LPTkyCharactor				m_chara;
	//����f�[�^
	LPTkyWeapon				m_weapon;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate() ;	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Charactor";}
//�Ǝ��v�f
	//�ݒ�(�f�[�^�����邾���Ȃ̂Ń|�C���^�Ƃ��Ă݂Ȃ�)
	BOOL SetCharactorWeaponData( CCharactor* , CWeapon* );
	//�擾
	LPTkyCharactor		GetCharactor()const{return m_chara;}	//�L�����N�^�[�f�[�^
	LPTkyWeapon				GetWeapon()const{return m_weapon;}	//����f�[�^
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSHeroView :: �L�����N�^�[�r���[  //�V���O���g���Ȃ��
//�@��l���p�@���̂Ƃ���͉��ɂ����邱�Ƃ͂Ȃ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSHeroView , LPTkyFPSHeroView ) :public IFPSCharactorView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSHeroView();
	CFPSHeroView(const CFPSHeroView&);
	VIRTUAL ~CFPSHeroView();
public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Charactor_Hero";}
//�Ǝ��v�f
	STATIC LPTkyFPSHeroView CreateFPSHeroView();
	STATIC LPTkyFPSHeroView CreateFPSHeroView( CCharactor* , CWeapon* );
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSEnemyView :: �L�����N�^�[�r���[
//�@�G�p�@���̂Ƃ���͉��ɂ����邱�Ƃ͂Ȃ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSEnemyView , LPTkyFPSEnemyView ) :public IFPSCharactorView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSEnemyView();
	CFPSEnemyView(const CFPSEnemyView&);
	VIRTUAL ~CFPSEnemyView();

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Charactor_Enemy";}
//�Ǝ��v�f
	STATIC LPTkyFPSEnemyView CreateFPSEnemyView();
	STATIC LPTkyFPSEnemyView CreateFPSEnemyView(CCharactor* , CWeapon*);
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CBulletView 
//�@�e�p�A�ǉ����ꂽ��ɏ�Q���Ɣ�r������������܂�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CBulletView , LPTkyBulletView )  :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CBulletView();
	CBulletView(const CBulletView&);
	VIRTUAL ~CBulletView();

	//�������
	LPTkyBullet m_Bullet;
	//�e������������
	LPTkyWeapon	m_ShotWeapon;

	//���ˈʒu
	Vector m_BatteryPosition;



public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Bullet";}



//�Ǝ��v�f
	//����
	STATIC LPTkyBulletView CreateBullet( );
	STATIC LPTkyBulletView CreateBullet( LPTkyWeapon );
	STATIC LPTkyBulletView CreateBullet( CBullet* , LPTkyWeapon );
	//�ݒ�擾
	LPTkyBullet GetBullet()const{return m_Bullet;}
	VOID SetBullet(CBullet *t){*m_Bullet=*t;}
	LPTkyWeapon	GetWeapon()const{return m_ShotWeapon;}
	VOID SetWeapon(LPTkyWeapon w){m_ShotWeapon = w;}
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMapView  //�V���O���g���Ȃ��
//�@�}�b�v�͂��������ɂ��邾���ł����񂾂�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSMapView , LPTkyFPSMapView ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSMapView();
	CFPSMapView(const CFPSMapView&);
	VIRTUAL ~CFPSMapView();

	//X�t�@�C��
	LPTkyXFile	m_map;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Map";}
//�Ǝ��v�f
	//����
	STATIC LPTkyFPSMapView CreateMap( );
	STATIC LPTkyFPSMapView CreateMap( LPTkyXFile );
	//�擾�ݒ�
	LPTkyXFile	GetModel()const{return m_map;};	//X�t�@�C��
	VOID	SetModel(LPTkyXFile x){m_map = x;};	//X�t�@�C��
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEffectView 
//�@�I������玩���Ŏ����I�ɏI����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CEffectView , LPTkyEffectView ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CEffectView();
	CEffectView(const CEffectView&);
	VIRTUAL ~CEffectView();

	//�e���
	CGameEffect m_Effect;
	//�\���ʒu
	Vector m_pos;
	Vector m_rotate;
	Vector m_expand;

	//�^�C�v���
	TKYTEXTURE_TYPE m_type;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Effect";}
//�Ǝ��v�f
	//����
	STATIC LPTkyEffectView CreateEffect( );
	STATIC LPTkyEffectView CreateEffect( CGameEffect *Effect ,	Vector pos , Vector m_rotate , Vector expand , TKYTEXTURE_TYPE type);
	//�ݒ�擾
	LPTkyGameEffect GetEffect()const{return (const LPTkyGameEffect)(&m_Effect);}
	Vector GetPos()const{return m_pos;}
	Vector GetRotate()const{return m_rotate;}
	Vector GetExpand()const{return m_expand;}
	VOID SetEffect(CGameEffect* t){m_Effect=*t;}
	VOID SetPos(Vector v){m_pos=v;}
	VOID SetRotate(Vector v){m_rotate=v;}
	VOID SetExpand(Vector v){m_expand=v;}
	VOID SetTextureType(TKYTEXTURE_TYPE t){m_type = t;}
};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//IFpsAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IFpsAIView , LPTkyFpsAIView ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	IFpsAIView();
	IFpsAIView(const IFpsAIView &);
	VIRTUAL ~IFpsAIView ();

	//�L�����N�^�r���[( �������ɏ������b�Z�[�W���o�� )
	LPTkyFPSCharactorView m_charaview;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate()=0;	//������
	VIRTUAL BOOL onUpdate(float step)=0;	//�X�V
	VIRTUAL VOID onRender()=0;	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsAI";}
//�Ǝ��v�f
	//�擾�ݒ�
	LPTkyFPSCharactorView GetCharactorView()const{return m_charaview;}
	VOID SetCharactorView(LPTkyFPSCharactorView c){m_charaview = c;}

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsMoveAIView  //�V���O���g���Ȃ��
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsMoveAIView , LPTkyFpsMoveAIView ) :public IFpsAIView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsMoveAIView();
	CFpsMoveAIView(const CFpsMoveAIView&);
	VIRTUAL ~CFpsMoveAIView();

	//��]����(�}�E�X��)
	Vector m_rotate;
	//�}�E�X���
	DIMOUSESTATE m_mouse;

	//
	Vector m_BaseVector;

	//�J�����̍��W�A����
	Vector *m_pos,*m_Direction;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsMoveAI";}

	VOID SetBaseVector(Vector v){m_BaseVector = v;}

//�Ǝ��v�f
	//����
	STATIC LPTkyFpsMoveAIView CreateFpsMoveAI( );
	STATIC LPTkyFpsMoveAIView CreateFpsMoveAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction );
	//�ݒ�
	VOID SetCmameraVector(Vector *pos,Vector *Direction){ m_pos = pos,m_Direction = Direction; }
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsAutoAIView  //�V���O���g���Ȃ��
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsAutoAIView , LPTkyFpsAutoAIView ) :public IFpsAIView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsAutoAIView(wstring name);
	CFpsAutoAIView(const CFpsAutoAIView&);
	VIRTUAL ~CFpsAutoAIView();

	//�J�����̍��W�A����
	Vector *m_pos,*m_Direction;



	//�I�[�g���
	LPTkyAutoKeyInput			m_keyinfo;
	LPTkyAutoMouseInput		m_mouseinfo;

	//���̏��
	bool m_run;

	wstring m_file;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsAutoAI";}
//�Ǝ��v�f
	//����
	STATIC LPTkyFpsAutoAIView CreateFpsAutoAI( wstring name );
	STATIC LPTkyFpsAutoAIView CreateFpsAutoAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction , wstring name );
	//�ݒ�
	VOID SetCmameraVector(Vector *pos,Vector *Direction){ m_pos = pos,m_Direction = Direction; }



	//�擾
	LPTkyAutoKeyInput			GetAutoKeyInfo(){return m_keyinfo;}
	LPTkyAutoMouseInput		GetAutoMouseInfo(){return m_mouseinfo;}
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyAIView , LPTkyFpsEnemyAIView ) :public IFpsAIView
{
protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyAIView();
	CFpsEnemyAIView(const CFpsEnemyAIView&);
	VIRTUAL ~CFpsEnemyAIView();

	Vector *m_heropos;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyAI";}
//�Ǝ��v�f
	//����
	STATIC LPTkyFpsEnemyAIView CreateFpsEnemyAI( );
	STATIC LPTkyFpsEnemyAIView CreateFpsEnemyAI( LPTkyFPSCharactorView chara );
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAttackLeaveAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyAttackLeaveAIView , LPTkyFpsEnemyAttackLeaveAIView ) :public IFpsAIView
{
protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyAttackLeaveAIView();
	CFpsEnemyAttackLeaveAIView(const CFpsEnemyAttackLeaveAIView&);
	VIRTUAL ~CFpsEnemyAttackLeaveAIView();

	Vector *m_heropos;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyAttackLeaveAI";}
//�Ǝ��v�f
	//����
	STATIC LPTkyFpsEnemyAttackLeaveAIView CreateFpsEnemyAttackLeaveAI( );
	STATIC LPTkyFpsEnemyAttackLeaveAIView CreateFpsEnemyAttackLeaveAI( LPTkyFPSCharactorView chara );
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyStopAttackAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyStopAttackAIView , LPTkyFpsEnemyStopAttackAIView ) :public IFpsAIView
{
protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyStopAttackAIView();
	CFpsEnemyStopAttackAIView(const CFpsEnemyStopAttackAIView&);
	VIRTUAL ~CFpsEnemyStopAttackAIView();

	Vector *m_heropos;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyStopAttackAI";}
//�Ǝ��v�f
	//����
	STATIC LPTkyFpsEnemyStopAttackAIView CreateFpsEnemyStopAttackAI( );
	STATIC LPTkyFpsEnemyStopAttackAIView CreateFpsEnemyStopAttackAI( LPTkyFPSCharactorView chara );
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyThrowAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyThrowAIView , LPTkyFpsEnemyThrowAIView ) :public IFpsAIView
{
protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyThrowAIView();
	CFpsEnemyThrowAIView(const CFpsEnemyThrowAIView&);
	VIRTUAL ~CFpsEnemyThrowAIView();

	Vector *m_heropos;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyThrowAI";}
//�Ǝ��v�f
	//����
	STATIC LPTkyFpsEnemyThrowAIView CreateFpsEnemyThrowAI( );
	STATIC LPTkyFpsEnemyThrowAIView CreateFpsEnemyThrowAI( LPTkyFPSCharactorView chara );
};







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindowiew 
//�@��Ƀe�L�X�g�ɁA�E�B���h�E��񂾂������Ă����A���̂��ƂɃe�L�X�g���𑗂�܂���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTextWindowiew , LPTkyTextWindowiew ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTextWindowiew();
	CTextWindowiew(const CTextWindowiew &);
	VIRTUAL ~CTextWindowiew ();

	//�e�L�X�g�E�B���h�E�̏��
	LpTkyManageTextWindow m_ManageTextWindow;

	//���̃E�B���h�E
	LPTkyTextWindowiew m_NextTextWIndow;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_TextWinodwView";}
//�ݒ�擾
	LpTkyManageTextWindow GetManageTextWindow(){return  m_ManageTextWindow;};
	VOID SetManageTextWindow(LpTkyManageTextWindow t){*m_ManageTextWindow = *t;};

	LPTkyTextWindowiew GetNextTextWinodw(){return m_NextTextWIndow;}
	VOID SetNextTextWinodw(LPTkyTextWindowiew t)
	{
		//���������Ɠ����������甲����
		IF( m_NextTextWIndow == t )return;

		IF(m_NextTextWIndow==NULL)
		{
			//�v�f���Ȃ�������ǉ�
			m_NextTextWIndow = t;
		}
		else
		{
			//�������玟��
			m_NextTextWIndow->SetNextTextWinodw(t);
		}
	}
	//�v�f�̊J��
	VOID DeleteNext()
	{
		IF(m_NextTextWIndow==NULL)
		{
			return;
		}
		else
		{
			//�������玟��
			m_NextTextWIndow->DeleteNext();
			m_NextTextWIndow->DestroyOwnObject();
			m_NextTextWIndow = NULL;
		}
	}

//�擾
	STATIC LPTkyTextWindowiew CreateTextWinodwView(){return new CTextWindowiew();}

	STATIC LPTkyTextWindowiew CreateTextWinodwView(TTextSet set)
	{
		LPTkyTextWindowiew r =  new CTextWindowiew();
		r->GetManageTextWindow()->SetTextInfo( set.m_text , set.m_window );
		return r;
	}

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindoRangeDecisionView  //�V���O���g���Ȃ��
//�@�e�L�X�g�͈̔͂��m�F		
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTextWindoRangeDecisionView  , LPTkyTextWindoRangeDecisionView  ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CTextWindoRangeDecisionView ();
	CTextWindoRangeDecisionView (const CTextWindoRangeDecisionView  &);
	VIRTUAL ~CTextWindoRangeDecisionView  ();

	//�͈͏��Ǘ�
	CElementRange<LPTkyTextWindowiew> *m_range;

	//��l���̍��W�x�N�g��
	Vector *m_pos;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_TextWinodwRangeDecisionView";}
//�ݒ�擾
	CElementRange<LPTkyTextWindowiew>* GetTextWinodwViewRange(){return m_range;}
	VOID GetTextWinodwViewRange(CElementRange<LPTkyTextWindowiew>* t){m_range = t;}

	//�J�������ǉ�
	VOID Delete();

	VOID SetVector(Vector *v){m_pos = v;}
	Vector*  GetVector(){return m_pos;}

//����
	STATIC LPTkyTextWindoRangeDecisionView CreateTextWinodwRangeDecision(Vector *p)
	{
		LPTkyTextWindoRangeDecisionView  v =  new CTextWindoRangeDecisionView();
		v->SetVector(p);
		return v;
	}
};






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyAppearRangeDecisionView   //�V���O���g���Ȃ��
//�@�e�L�X�g�͈̔͂��m�F
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CEnemyAppearRangeDecisionView  , LPTkyEnemyAppearRangeDecisionView  ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CEnemyAppearRangeDecisionView ();
	CEnemyAppearRangeDecisionView (const CEnemyAppearRangeDecisionView  &);
	VIRTUAL ~CEnemyAppearRangeDecisionView  ();

	//�͈͏��Ǘ�
	CElementRange<LPTkyFPSEnemyView> *m_range;

	//��l���̍��W�x�N�g��
	Vector *m_pos;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_EnemyAppearRangeDecisionView";}
//�ݒ�擾
	CElementRange<LPTkyFPSEnemyView>* GetEnemyApperViewRange(){return m_range;}
	VOID GetEnemyApperViewRange(CElementRange<LPTkyFPSEnemyView>* t){m_range = t;}

	//�J�������ǉ�
	VOID Delete();

	VOID SetVector(Vector *v){m_pos = v;}
	Vector*  GetVector(){return m_pos;}

//����
	STATIC LPTkyEnemyAppearRangeDecisionView CreateEnemyApperRangeDecision(Vector *p)
	{
		LPTkyEnemyAppearRangeDecisionView  v =  new CEnemyAppearRangeDecisionView();
		v->SetVector(p);
		return v;
	}
};






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyKillRangeDecisionView   //�V���O���g���Ȃ��
//�@�G��|�����߂ɂƂ܂�Ƃ���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CEnemyKillRangeDecisionView  , LPTkyEnemyKillRangeDecisionView  ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CEnemyKillRangeDecisionView ();
	CEnemyKillRangeDecisionView (const CEnemyKillRangeDecisionView  &);
	VIRTUAL ~CEnemyKillRangeDecisionView  ();

	//�͈͏��Ǘ�
	CElementRange<INT> *m_range;

	//��l���̍��W�x�N�g��
	Vector *m_pos;

	//�E�������G�̐�
	INT m_KillEnemy;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_EnemyKillRangeDecisionView";}
//�ݒ�擾
	CElementRange<INT>* GetEnemyKillViewRange(){return m_range;}
	VOID GetEnemyKillViewRange(CElementRange<INT>* t){m_range = t;}

	VOID SetVector(Vector *v){m_pos = v;}
	Vector*  GetVector(){return m_pos;}

//����
	STATIC LPTkyEnemyKillRangeDecisionView CreateEnemyKillRangeDecision(Vector *p)
	{
		LPTkyEnemyKillRangeDecisionView  v =  new CEnemyKillRangeDecisionView();
		v->SetVector(p);
		return v;
	}
};










//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMovementPathView   //�V���O���g���Ȃ��
//�@�ړ��̋O�Ղ�ۑ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CMovementPathView  , LPTkyMovementPathView  ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CMovementPathView ();
	CMovementPathView (const CMovementPathView  &);
	VIRTUAL ~CMovementPathView  ();

	//���
	LPTkyAutoKeyInput			m_KeyPath;
	LPTkyAutoMouseInput		m_MousePath;

	//���̃t���[����
	DWORD							m_NowKeyFrame;
	DWORD							m_NowMouseFrame;

	//�L�[�̏��
	BYTE	m_key;
	//��]����(�}�E�X��)
	Vector m_rotate;
	//�}�E�X���
	DIMOUSESTATE m_mouse;

	Vector m_base;

	//�ۑ�������
	wstring m_move;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_MovementPathView";}

	VOID SetBase(Vector v){m_base = v;}

	//�ݒ�
	VOID SetMoveName(wstring name){m_move = name;}

//����
	STATIC LPTkyMovementPathView CreateMovementPath(wstring name)
	{
		LPTkyMovementPathView t = new CMovementPathView();
		t->SetMoveName(name);
		return t;
	}

};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CAdventureWindowiew 
//�@��Ƀe�L�X�g�ɁA�E�B���h�E��񂾂������Ă����A���̂��ƂɃe�L�X�g���𑗂�܂���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eAdventure{ TKYADVENTURE_SHOW , TKYADVENTURE_OPEN , TKYADVENTURE_ENDPROCESS , TKYADVENTURE_END , TKYADVENTURE_ESCAPE };
CLASS_POINTA( CAdventureWindowiew , LPTkyAdventureWindowiew ) :public IGameView
{
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CAdventureWindowiew();
	CAdventureWindowiew(const CAdventureWindowiew &);
	VIRTUAL ~CAdventureWindowiew ();

	//�e�L�X�g�E�B���h�E�̏��
	LpTkyManageTextWindow m_ManageAdventureWindow;
	//���̃E�B���h�E
	LPTkyAdventureWindowiew m_NextAdventureWIndow;

	//���̏��
	eAdventure m_adventure;

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_AdventureWinodwView";}
//�ݒ�擾
	LpTkyManageTextWindow GetManageAdventureWindow(){return  m_ManageAdventureWindow;};
	VOID SetManageAdventureWindow(LpTkyManageTextWindow t){*m_ManageAdventureWindow = *t;};

	LPTkyAdventureWindowiew GetNextAdventureWinodw(){return m_NextAdventureWIndow;}
	VOID SetNextAdventureWinodw(LPTkyAdventureWindowiew t)
	{
		IF( m_NextAdventureWIndow == t )return;

		IF(m_NextAdventureWIndow==NULL)
		{
			//�v�f���Ȃ�������ǉ�
			m_NextAdventureWIndow = t;
		}
		else
		{
			//�������玟��
			m_NextAdventureWIndow->SetNextAdventureWinodw(t);
		}
	}

	//�v�f�̊J��
	VOID DeleteNext()
	{
		IF(m_NextAdventureWIndow==NULL)
		{
			return;
		}
		else
		{
			//�������玟��
			m_NextAdventureWIndow->DeleteNext();
			m_NextAdventureWIndow->DestroyOwnObject();
			m_NextAdventureWIndow = NULL;
		}
	}


//�擾
	STATIC LPTkyAdventureWindowiew CreateAdventureWinodwView(){return new CAdventureWindowiew();}

	STATIC LPTkyAdventureWindowiew CreateAdventureWinodwView(TTextSet set)
	{
		LPTkyAdventureWindowiew r =  new CAdventureWindowiew();
		r->GetManageAdventureWindow()->SetTextInfo( set.m_text , set.m_window );
		return r;
	}

};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTitleView 
//�@�^�C�g�����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTitleView , LPTkyTitleView ) :public IGameView
{
private:

	LPTkyTitle m_Title;

	CTitleView(TTitle);
	CTitleView( const CTitleView& );
	VIRTUAL ~CTitleView();

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_TitleView";}

	VOID SetTitle( TTitle t )
	{
		m_Title->SetTitle(t);
	}

//����
	STATIC LPTkyTitleView CreateTitle( TTitle t )
	{
		return new CTitleView(t);
	}

};








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CScoreView 
//�@�^�C�g�����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CScoreView , LPTkyScoreView ) :public IGameView
{
private:

	LPTkyScore m_Score;

	CScoreView(TScore);
	CScoreView( const CScoreView& );
	VIRTUAL ~CScoreView();

public:
//GameView �v�f
	VIRTUAL BOOL onCreate();	//������
	VIRTUAL BOOL onUpdate(float step);	//�X�V
	VIRTUAL VOID onRender();	//�`��
//�C�x���g�v�f
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_ScoreView";}

	LPTkyScore GetScore(){return m_Score;}
	VOID SetScore( TScore t )
	{
		m_Score->SetScore(t);
	}

//����
	STATIC LPTkyScoreView CreateScore( TScore t )
	{
		return new CScoreView(t);
	}

};
