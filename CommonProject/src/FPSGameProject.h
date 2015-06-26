#pragma once

#include "resource.h"

#include <queue>


#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyDirectXApplicationLayer.h"

#include "DataStructure.h"
#include "FPSGameViews.h"

#include "ResourceManage.h"

//�E�B���h�E�̑傫��
const static DWORD WindowWidth = 800;
const static DWORD WindowHeight = 600;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�t�@�C���ǂݍ��݃^�X�N
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//�t�@�C���ǂݍ��݂悤
CLASS_POINTA( CTkyReadThread , LPTkyReadThread ) :public ITkyThread
{
private:

	CResourceManage *m_resource;
	CCharaManage	*m_chara;
	CTextManage		*m_text;

	CTkyReadThread(CResourceManage *r,CCharaManage *c,CTextManage	*t)
	{
		m_resource = r;
		m_chara = c;
		m_text  = t;
	}
	VIRTUAL ~CTkyReadThread()
	{
	}

	VIRTUAL VOID onThreadUpdate()
	{
		//���\�[�X�쐬
		m_resource->LoadResource(TEXT("text/resource.txt"));
		//�L�����̍쐬
		m_chara->LoadCharactor( TEXT("text/charactor.txt") , m_resource );
		//�e�L�X�g�̍쐬
		m_text->LoadText( TEXT("text/text.txt") , m_resource );
	}

public:

	STATIC LPTkyReadThread CreateReadThread(CResourceManage *r,CCharaManage *c,CTextManage	*t){return new CTkyReadThread(r,c,t);};

};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�N���X���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS(CFPSApplicationLayer);
CLASS(CFPSGameLogic );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPS�\����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TFPSInfo )
{
	Vector *pos;
	Vector *vec;
};
TYPEDEF( TFPSInfo* , LPTkyFPSInfo );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���W�b�N
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eFpsLogic{ TKYFPSLOGIC_READ , TKYFPSLOGIC_RUN };
CLASS_POINTA( CFPSGameLogic , LPTkyFPSGameLogic ) : public IGameLogic
{

private:
	
	//���E���
	LPTkyPhysicsWorld3D m_world3d;

	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSGameLogic();
	CFPSGameLogic(const CFPSGameLogic&);
	VIRTUAL ~CFPSGameLogic();

	//DirectX�̏��
	TDirectXInfo *m_info;

	//��v�Q�[���f�[�^
	CResourceManage ResourceManage;
	CCharaManage CharaManage;
	CTextManage TextManage;

	//��{�f�[�^
	CGameFlowManage GameFlowManage;

	//���̏��
	eFpsLogic m_state;

	//���̃X�R�A
	DWORD m_NowScore;

	//�ǂݍ��݃^�X�N
	LPTkyReadThread m_read;

	//�ǂݍ��݉摜
	LPTkyTexture m_loading;

	//�e�L�X�g�E�B���h�E�̏��ԑ҂��L���[
	LPTkyTextWindowiew m_TextWinodwQueue;
	LPTkyAdventureWindowiew m_AdvectureWinodwQueue;

public:
//GameLogic���
	//������
	VIRTUAL BOOL onCreate() ;
	//�X�V( View�X�V�������� )
	VIRTUAL VOID onUpdate(float step);
	//�X�V
	VIRTUAL VOID onRender();

	//View�ݒ莞(View���͎����I�Ƀ^�X�N�ɒǉ������̂ł��̑��̐ݒ�)
	VIRTUAL VOID onAddView( LPTkyGameView view );
	//View������
	VIRTUAL VOID onDeleteView( LPTkyGameView view );

//�C�x���g���
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "FPS_GameLogic";
	}
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
//�Ǝ����
	//����
	STATIC LPTkyFPSGameLogic CreateFPSGameLogic();
	STATIC LPTkyFPSGameLogic CreateFPSGameLogic(TDirectXInfo* info);
//�擾�ݒ�
	TDirectXInfo*	GetDirectXInfo()const{return m_info;}
	VOID				SetDirectXInfo( TDirectXInfo *t ){m_info = t;}
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPS�Q�[���p
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSApplicationLayer , LPTkyFPSApplicationLayer ) :public IDirectXBaseApplicationLayer
{
private:

	//���W�b�N���
	LPTkyFPSGameLogic m_Logic;

	//�V�F�C�h���
	CShade m_shade;

	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSApplicationLayer();
	CFPSApplicationLayer(const CFPSApplicationLayer&);
	VIRTUAL ~CFPSApplicationLayer();

protected:

	//DirectX�p���z�֐�
	VIRTUAL BOOL pOnCreateDirectX();
	VIRTUAL BOOL pOnUpdateDirectX(float step);
	VIRTUAL VOID pOnRenderDirectX();
	VIRTUAL VOID pOnEventDirectX(TEventMessage msg );

public:

	//����
	STATIC LPTkyFPSApplicationLayer CreateFPSApplicationLayer()
	{
		return new CFPSApplicationLayer();
	}

};