//--------------------------------------------------------------------------------------------------------------
//			�^�X�N�̃��C���w�b�_�[
//					�^�X�N�̏�������		�E�����̓���^�X�N�@�E�C�x���g�^�X�N�܂ł��낢��
//					�����ł���悤�ɂ��Ă���
//--------------------------------------------------------------------------------------------------------------
#pragma once

#include "TKYBASEHEADER.h"
#include <assert.h>
#include "TkyThread.h"


//--------------------------------------------------------------------------------------------------------------
//			�^�X�N�̃��C���w�b�_�[
//					�^�X�N�̏�������		�E�����̓���^�X�N�@�E�C�x���g�^�X�N�܂ł��낢��
//					�����ł���悤�ɂ��Ă���
//--------------------------------------------------------------------------------------------------------------
class ITKYTASK
{

public:
	virtual void Update() = 0;
	virtual bool Cheak() = 0;
};


//--------------------------------------------------------------------------------------------------------------
//			TKY���C�u������{�N���X,�\����
//--------------------------------------------------------------------------------------------------------------

/*

	2011/3/5

	�^�X�N�V�X�e���̎g�p���@

	//����

	ITkyMultiTask
	���p�����e�q�^�X�N�����	
	���̃����o��
	static �ɂ��NEW�����b�v����

	CTkyMultiTaskSystem
	���p�����^�X�N�Ǘ����쐬����
	���̃����o��
	static �ɂ��NEW�����b�v����

	//�ǉ�
	
	CTkyMultiTaskSystem::AddTask( LPTKYMULTITASK );
	���g�p���^�X�N��ǉ����Ă������̍�
	�^�X�N���ł�Create()���Ăяo�����

	
	static VOID ITkyMultiTask::SetNowTaskSystem(LPTkyMultiTaskSyatem)
	���g�p���A�g�p����^�X�N�V�X�e����I������B

	�^�X�N���ł́����g�p���^�X�N�̐��������B
	static LPTKYMULTITASKSYSTEM m_NowSystem;

	//�X�V

	���C�����[�v���ł́��ōX�V���ł���
	CTkyMultiTaskSystem::UpdateTask();
	���̍ۊe�^�X�N�ł�Update���Ăяo�����

	//�J��

	�^�X�N�V�X�e�����Ł��������������List��Iterater�����炤
	CTkyMultiTaskSystem::DestroyTask(DestroyTask( HTKYTASKID ));



*/

//--------------------------------------------------------------------------------------------------------------
//			�^�X�N�̃��C���w�b�_�[
//					�^�X�N�̏�������		�E�����̓���^�X�N�@�E�C�x���g�^�X�N�܂ł��낢��
//					�����ł���悤�ɂ��Ă���
//--------------------------------------------------------------------------------------------------------------

//�^�X�NID�錾
CREATE_HANDLE(HTKYTASKID);


CLASS_POINTA(ITkyMultiTask , LPTKYMULTITASK) :public ITkyThread
{

//�^�X�N�v�f

//--���I--
private:	
	//�^�X�NID
	HTKYTASKID m_hid;

	//���̃^�X�N
	LPTKYMULTITASK m_next;
	LPTKYMULTITASK m_before;

	//�X�e�b�v
	float m_step;

public:

	//�^�X�N�̐���
	BOOL Create()
	{
		return onCreate();
	}
	//�^�X�N�̎��s
	BOOL Update(float step)
	{
		onUpdate(step);
		IF(m_next!=NULL)
		{
			m_next->Update(step);
		}
		return true;
	}
	//���z�֐�(�^�X�N�v�f�̋L�q)
	VIRTUAL BOOL onCreate()=0;
	VIRTUAL BOOL onUpdate(float step)=0;

	//ID����
	//�ݒ�
	VOID SetStep(float step){m_step = step;}
	VOID SetID(HTKYTASKID id){m_hid = id;}
	VOID SetID(DWORD id){m_hid.num = id;}
	VOID SetNextTask(LPTKYMULTITASK t){m_next = t;}
	VOID SetBeforeTask(LPTKYMULTITASK t){m_before = t;}
	//�擾
	HTKYTASKID GetID(){return m_hid;}
	//�m�F
	bool	IsID(HTKYTASKID id){return (id.num==m_hid.num) ? true : false;}

protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	ITkyMultiTask();
	VIRTUAL ~ITkyMultiTask();

//�X���b�h�v�f

private:

	//�X�V���z��
	VIRTUAL VOID onThreadUpdate()
	{
		Update(m_step);
		this->Pause();
	}

};
//���C���̃^�X�N����X�[�p�[�N���X
CLASS_POINTA_OBJECT(CTkyMultiTaskSystem , LPTKYMULTITASKSYSTEM )
{
private:
	list<LPTKYMULTITASK> m_Handle;

	//�N���e�B�J���Z�N�V�����̐錾
	CRITICAL_SECTION m_critical;

public:
	CTkyMultiTaskSystem();
	VIRTUAL ~CTkyMultiTaskSystem();

	//�^�X�N�̒ǉ�������(���̈ʒu���炩�w��)(�X���b�h���쐬����)
	BOOL AddTask( LPTKYMULTITASK );
	//�^�X�N�̏���
	bool DestroyTask( LPTKYMULTITASK );
	//�^�X�N�̍X�V
	VOID Update(float step);

	//�擾
	list<LPTKYMULTITASK> GetThread(){return m_Handle;}

	//����
	STATIC LPTKYMULTITASKSYSTEM CreateTaskSystem(){return new CTkyMultiTaskSystem();}

protected:

};
