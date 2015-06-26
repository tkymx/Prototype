#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TKYVECTOR.h"
#include "TkyCollision.h"
#include "TkyPhysics.h"

static VOID TkyWorld()
{
}

//==================== ���̃^�X�N�[�N���X ====================
CLASS_POINTA( CPhysicsModelTask , LPTkyPhysicsModelTask ) : public ITkyMultiTask
{
private:
	LPPhysicsModel			m_model;
	list<LPPhysicsModel>*	m_models;

	CPhysicsModelTask();
	CPhysicsModelTask( LPPhysicsModel model );
	VIRTUAL ~CPhysicsModelTask();

public:
	VIRTUAL BOOL onCreate();
	VIRTUAL BOOL onUpdate(float step);

	//�ݒ�n
	VOID SetModelList(list<LPPhysicsModel> *models){m_models = models;}

	//���f���̎擾
	LPPhysicsModel GetModel(){return m_model;}
	
	//�^�X�N�̐���
	STATIC LPTkyPhysicsModelTask CreatePhysicsModelTask( LPPhysicsModel model );

};

//==================== ����3D�^�X�N�[�N���X ====================
CLASS_POINTA( CPhysics3DModelTask , LPTkyPhysics3DModelTask ) : public ITkyMultiTask
{
private:
	LPPhysicsModel			m_model;
	list<LPPhysicsModel>*	m_models;

	CPhysics3DModelTask();
	CPhysics3DModelTask( LPPhysicsModel model );
	VIRTUAL ~CPhysics3DModelTask();

public:
	VIRTUAL BOOL onCreate();
	VIRTUAL BOOL onUpdate(float step);

	//�ݒ�n
	VOID SetModelList(list<LPPhysicsModel> *models){m_models = models;}

	//���f���̎擾
	LPPhysicsModel GetModel(){return m_model;}
	
	//�^�X�N�̐���
	STATIC LPTkyPhysics3DModelTask CreatePhysics3DModelTask( LPPhysicsModel model );

};


//==================== �����}�l�[�W���[�ɓ����� ====================

//-------------------------------------------
//�������E�̏��
//-------------------------------------------
CLASS_POINTA_OBJECT(CPhysicsWorld,LPTkyPhysicsWorld)
{
protected:

	//�d�͏��
	float m_Gravity;
	//��C��R
	float m_AirGap;
	//�}�b�v�͈̔�
	RECT m_range;

	//���f�����
	list<PhysicsModel*> m_model;

	//�R���X�g���N�^:�f�X�g���N�^	
	CPhysicsWorld();
	VIRTUAL ~CPhysicsWorld();

	//�^�X�N�����N���X
	LPTKYMULTITASKSYSTEM m_task;

	//�N���e�B�J���Z�N�V����
	CRITICAL_SECTION m_critical;	

public:

	//�ǉ��iID��Ԃ��j
	VIRTUAL bool AddPhysicsModel( PhysicsModel *model);
	VIRTUAL bool DeletePhysicsModel( PhysicsModel *model);

	//�擾
	float GetGravity(){return m_Gravity;};
	float GetAirGap(){return m_AirGap;};

	list<PhysicsModel*> GetModels()const{return m_model;}
	DWORD GetModelNum(){return m_model.size();}

	//�ݒ�
	VOID SetGravity(float g){m_Gravity = g;}
	VOID SetAirGap(float a){m_AirGap = a;}
	VOID SetRange(RECT rect ){m_range = rect;}

	//�X�V
	VIRTUAL bool Update( float step );

	//�쐬
	STATIC LPTkyPhysicsWorld CreatePhysicsWorld();
};

//-------------------------------------------
//�������E�̏��3D
// �܂��^�X�N���g�p���ē����蔻�肵�Ă��Ȃ������̂�������
//-------------------------------------------
CLASS_POINTA(CPhysicsWorld3D,LPTkyPhysicsWorld3D) :public CPhysicsWorld
{
private:

	//��Q�����
	list<LPTkyXFile> m_obstaclemodel;

	//�R���X�g���N�^:�f�X�g���N�^	
	CPhysicsWorld3D();
	VIRTUAL ~CPhysicsWorld3D();

public:

	//�ǉ�
	VIRTUAL bool	AddPhysicsModel( PhysicsModel *model);
	bool			AddObtaclePhysicsModel( LPTkyXFile model);

	//����
	bool DeleteObtaclePhysicsModel( LPTkyXFile model);

	//���̈ʒu�����ԋ߂���Q���܂ł̋���( ���Ŗ����� )
	FLOAT GetDistanceObstacle( Vector pos , Vector vec );

	//�X�V
	VIRTUAL bool Update( float step );

	//�쐬
	static LPTkyPhysicsWorld3D CreatePhysicsWorld3D();
};
