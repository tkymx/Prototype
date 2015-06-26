#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "vector"

#include "directx.h"
#include "DataStructure.h"
#include "FPSGameViews.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���\�[�X���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CResourceManage )
{
private:
	//X�t�@�C�����
	CStringMatchW<LPTkyXFile> m_x;
	//�摜�t�@�C�����
	CStringMatchW<LPTkyTexture> m_tex;
public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CResourceManage();
	CResourceManage(const CResourceManage&);
	VIRTUAL ~CResourceManage();

	//�ǉ�
	VOID AddXFie(LPTkyXFile,wstring);
	VOID AddTexture(LPTkyTexture,wstring);
	//����
	VOID DeleteXFie(wstring);
	VOID DeleteTexture(wstring);
	//�擾
	LPTkyXFile		GetXFie(wstring);
	LPTkyTexture	GetTexture(wstring);

	//�t�@�C������擾
	VOID LoadResource(wstring name );

};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�L�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TCharaSet )
{
	LPTkyCharactor m_chara;
	LPTkyWeapon m_weapon;
	VOID Initialize()
	{
		m_chara = NULL;
		m_weapon = NULL;
	}
	VOID NewAllocate()
	{
		//�������[�m��
		m_chara = CCharactor::CreateCharactor();
		m_weapon = CWeapon::CreateWeapon( m_chara );
	}
	TCharaSet()	{Initialize();}
	TCharaSet(LPTkyCharactor chara ,LPTkyWeapon weapon)
	{
		m_chara = chara;
		m_weapon = weapon;
	}
};

CLASS( CCharaManage )
{
private:
	//�L�������
	CStringMatchW<TCharaSet> m_chara;
public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CCharaManage();
	CCharaManage(const CCharaManage&);
	VIRTUAL ~CCharaManage();

	//�ǉ�
	VOID AddCharactor(TCharaSet);
	//����
	VOID DeleteCharactor(TCharaSet);
	VOID DeleteCharactor(wstring);	
	//�擾
	TCharaSet Getchara(wstring);

	//�t�@�C������擾
	VOID LoadCharactor(wstring name, CResourceManage* resource);

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�e�L�X�g���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CTextManage )
{
private:
	//�L�������
	CNumberMatch<TTextSet> m_Text;

	//�E�B���h�E���
	CNumberMatch<TTextWindowInfo*> textwindow;

public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CTextManage();
	CTextManage(const CTextManage&);
	VIRTUAL ~CTextManage();

	//�ǉ�
	VOID AddText(INT,TTextSet);
	//����
	VOID DeleteText(INT);
	//�擾
	TTextSet GetText(INT);
	vector<TTextSet*> GetText(INT sn ,INT en);

	//�t�@�C������擾
	VOID LoadText(wstring name, CResourceManage* resource);

};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�͈͏��̕ۑ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CRangeManage )
{
private:
	//�͈͏��
	//�e�L�X�g
	LPTkyTextWindoRangeDecisionView m_TextRangeView;
	//�G
	LPTkyEnemyAppearRangeDecisionView m_EnemyRangeView;
	//�G�ɍU��
	LPTkyEnemyKillRangeDecisionView		m_EnemyKillView;
public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CRangeManage(Vector *pos);
	CRangeManage(const CRangeManage&);
	VIRTUAL ~CRangeManage();

	//�擾
	LPTkyTextWindoRangeDecisionView GetTextRange()
	{
		//�擾����ꍇ�͏�񂪏�����
		LPTkyTextWindoRangeDecisionView t = m_TextRangeView;
		m_TextRangeView = NULL;
		return t;
	}
	LPTkyEnemyAppearRangeDecisionView GetEnemyRange()
	{
		//�擾����ꍇ�͏�����
		LPTkyEnemyAppearRangeDecisionView e = m_EnemyRangeView;
		m_EnemyRangeView = NULL;
		return e;
	}
	LPTkyEnemyKillRangeDecisionView GetEnemyKillRange()
	{
		//�擾����ꍇ�͏�����
		LPTkyEnemyKillRangeDecisionView e = m_EnemyKillView;
		m_EnemyKillView = NULL;
		return e;
	}	

	//�t�@�C������擾
	VOID LoadText(wstring name, CCharaManage *chara , CTextManage *text);
};






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���X�e�[�W���i�C���^�[�t�F�C�X�j
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA_OBJECT( IStageManage , LPTkyStageManage )
{
private:
public:

	//�t�@�C������擾
	VIRTUAL VOID LoadText(wstring name, CResourceManage *resource , CCharaManage *chara,CTextManage *text) = 0;
	//���s
	VIRTUAL VOID Run( LPTkyGameLogic logic) = 0;
	//�I��
	VIRTUAL VOID Stop() = 0;

}; 







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���X�e�[�W���i�V���[�e�B���O�j
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CShootingStageManage , LPTkyShootingStageManage ) :public IStageManage
{
private:


	LPTkyTextWindoRangeDecisionView			textrange_view;			//�e�L�X�g���r���[
	LPTkyEnemyAppearRangeDecisionView		enemyrange_view;		//�G�o���͈͏��
	LPTkyEnemyKillRangeDecisionView				killview;						//�L���r���[
	LPTkyFPSMapView										map_view;					//�}�b�v��� 
	LPTkyScoreView											score_view;					//�X�R�A���
	LPTkyFPSHeroView										hero_view;					//��l���r���[

	Vector *pos , *vec;

	float n,f;

	//�R���X�g���N�^�A�f�X�g���N�^
	CShootingStageManage();
	CShootingStageManage(const CShootingStageManage&);
	VIRTUAL ~CShootingStageManage();

public:

	//�t�@�C������擾
	VIRTUAL VOID LoadText(wstring name, CResourceManage *resource , CCharaManage *chara,CTextManage *text);

	//���s
	VIRTUAL VOID Run( LPTkyGameLogic logic);

	//�I��
	VIRTUAL VOID Stop();

	//�Z�b�e�e�B���O
	VOID SetPosVec( Vector *p ,Vector * v ){ pos = p; vec = v; }

	LPTkyScoreView				GetScore(){return	score_view;}					//�X�R�A���
	LPTkyFPSHeroView			GetChara(){return	hero_view;}					//��l���r���[

	//�擾
	STATIC LPTkyShootingStageManage CreateShootingStage( wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text , Vector *pos , Vector *vec )
	{
		LPTkyShootingStageManage t = new CShootingStageManage();
		t->SetPosVec( pos , vec );
		t->LoadText( name , resource , chara , text );
		return t;
	}

}; 








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���X�e�[�W���i�e�L�X�g�j
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTextStageManage , LPTkyTextStageManage ) :public IStageManage
{
private:

	queue< LPTkyAdventureWindowiew > m_adventure;

	//�R���X�g���N�^�A�f�X�g���N�^
	CTextStageManage();
	CTextStageManage(const CTextStageManage&);
	VIRTUAL ~CTextStageManage();

public:

	//�t�@�C������擾
	VIRTUAL VOID LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text);

	//���s
	VIRTUAL VOID Run( LPTkyGameLogic logic );

	//�I��
	VIRTUAL VOID Stop();

	//�擾
	STATIC LPTkyTextStageManage CreateTextStage( wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text  )
	{
		LPTkyTextStageManage t = new CTextStageManage();
		t->LoadText( name , resource , chara , text  );
		return t;
	}

};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���X�e�[�W���i�^�C�g���j
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTitleManage , LPTkyTitleManage ) :public IStageManage
{
private:

	LPTkyTitleView m_tview;

	CTitleManage();
	CTitleManage( const CTitleManage& t );
	VIRTUAL ~CTitleManage();

public:

	//�t�@�C������擾
	VIRTUAL VOID LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text);

	//���s
	VIRTUAL VOID Run( LPTkyGameLogic logic );

	//�I��
	VIRTUAL VOID Stop();

	//�擾
	STATIC LPTkyTitleManage CreateTitleManage( wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text  )
	{
		LPTkyTitleManage t = new CTitleManage();
		t->LoadText( name , resource , chara , text  );
		return t;
	}

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���̗���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CGameFlowManage ) 
{
private:

	//�X�e�[�W���
	queue< LPTkyStageManage > m_stages;

	//�̗�
	INT m_hitpoint;
	//�X�R�A
	DWORD m_score;

public:
	//�R���X�g���N�^�A�f�X�g���N�^
	CGameFlowManage();
	CGameFlowManage(const CGameFlowManage&);
	VIRTUAL ~CGameFlowManage();

	//�t�@�C������擾
	VOID LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text , Vector *pos , Vector *vec );

	//�ؑ]���̎擾
	VOID GetBaseData();

	//���s
	VOID Run( LPTkyGameLogic logic );

	//���ɐi��
	BOOL Next();

	//�I��
	VOID Stop();

};