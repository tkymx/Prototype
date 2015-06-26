#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyDirectXApplicationLayer.h"

#include "DataStructure.h"
#include "FPSGameViews.h"


//����
const STATIC float WorldWolk3D = 0.001f;
//��C��R
const STATIC float WorldAir3D = 0.0009f;
//�d��
const STATIC float WorldGravity3D = -0.03f;
//�W�����v��
const STATIC float WorldJump3D = 0.1f;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPS�\����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct 
{
	Vector *pos;
	Vector *vec;
}TFPSInfo;
typedef TFPSInfo* LPTkyFPSInfo;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���f���r���[
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IModelView , LPTkyModelView ) :public IGameView
{
private:
	//���f���f�[�^
	LPTkyXFile m_model;
	//�������f���f�[�^
	LPPhysicsCircleModel m_PhysicsModel;

protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	IModelView();
	VIRTUAL ~IModelView();

	//����
	BOOL pCreateModel( wstring str );

public:

//GameView �v�f

	//������
	VIRTUAL BOOL onCreate() = 0;
	//�X�V
	VIRTUAL BOOL onUpdate(float step) = 0;
	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender() = 0;

//�C�x���g�v�f

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "3DPhysicsModel";
	}

//�Ǝ��v�f

	//���f���̎擾
	LPPhysicsCircleModel GetPhysicsModel(){return m_PhysicsModel;}
	//XFILE�̎擾
	LPTkyXFile GetXFile(){return m_model;}

	//���W�ݒ�
	VOID SetPos(Vector vec){m_PhysicsModel->SetPos(vec);}
	//���f���̐ݒ�
	VOID SetPhysicsModel(LPPhysicsCircleModel pcm){m_PhysicsModel = pcm;}
	//XFILE�̐ݒ�
	VOID SetXFile(LPTkyXFile tx){m_model = tx;}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���ʃI�u�W�F�N�g���f��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CNormalModelView , LPTkyNormalModelView ) :public IModelView
{
private:
	//�R���X�g���N�^�E�f�X�g���N�^
	CNormalModelView(wstring name);
	VIRTUAL ~CNormalModelView();
public:

//GameView �v�f

	//������
	VIRTUAL BOOL onCreate();
	//�X�V
	VIRTUAL BOOL onUpdate(float step);
	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender();

//�C�x���g�v�f

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VOID CNormalModelView::onNameSet( string *str )
	{
		*str = "3DNormalPhysicsModel";
	}

//�Ǝ��v�f

	//����
	STATIC LPTkyNormalModelView CreateNormalModelView( wstring name );
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//��Q�����f��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CObstacleModelView , LPTkyObstacleModelView ) :public IModelView
{
private:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObstacleModelView(wstring name);
	VIRTUAL ~CObstacleModelView();
public:

//GameView �v�f

	//������
	VIRTUAL BOOL onCreate();
	//�X�V
	VIRTUAL BOOL onUpdate(float step);
	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender();

//�C�x���g�v�f

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "3DObstaclePhysicsModel";
	}

//�Ǝ��v�f

	//����
	STATIC LPTkyObstacleModelView CreateObstacleModelView( wstring name );
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPS���f��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSModelView , LPTkyFPSModelView ) :public IModelView
{
private:
	//���W�A����
	Vector *m_pos,*m_Direction;

	//��]����
	Vector m_rotate;

	//�}�E�X���
	DIMOUSESTATE m_mouse;

	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSModelView(Vector *pos,Vector *vec);
	VIRTUAL ~CFPSModelView();
public:

//GameView �v�f

	//������
	VIRTUAL BOOL onCreate();
	//�X�V
	VIRTUAL BOOL onUpdate(float step);
	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender();

//�C�x���g�v�f

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "3DFPSPhysicsModel";
	}

//�Ǝ��v�f

	//����
	STATIC LPTkyFPSModelView CreateFPSModelView(Vector *pos,Vector *vec);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�X�e�[�W�ǂݍ��݃r���[
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CLoadGameView , LPTkyLoadGameView ) :public IGameView
{
private:
	//���b�W��f�[�^
	LPTkyGameLogic m_MainLogick;
	//FPS�f�[�^
	LPTkyFPSInfo m_info;
protected:
	//�R���X�g���N�^�E�f�X�g���N�^
	CLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info);
	VIRTUAL ~CLoadGameView();
public:
//GameView���
	//������
	VIRTUAL BOOL onCreate();
	//�X�V
	VIRTUAL BOOL onUpdate(float step);
	//�`��
	VIRTUAL VOID onRender();
//�C�x���g���
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "LoadGameView";
	}
//�Ǝ��v�f
	//����
	STATIC LPTkyLoadGameView CreateLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info);
};
