
#include "TKYTASK.h"


//--------------------------------------------------------------------------------------------------------------
//			ITkyMultiTask
//--------------------------------------------------------------------------------------------------------------
//�ʉ��
ITkyMultiTask::ITkyMultiTask()
{
	m_next = NULL;
	m_before = NULL;
	m_hid = HTKYTASKID();
	m_step = 0;
}
ITkyMultiTask::~ITkyMultiTask()
{
}
//--------------------------------------------------------------------------------------------------------------
//			CTkyMultiTaskSystem
//--------------------------------------------------------------------------------------------------------------
//�^�X�N�̒ǉ�������
BOOL CTkyMultiTaskSystem::AddTask( LPTKYMULTITASK task)
{
	//��O����
	assert(task);
	//�O�̃^�X�N�̎��̔ԍ�������
	DWORD id = 1;
	IF(!m_Handle.empty())id = m_Handle.back()->GetID().num + 1;
	//ID�̃Z�b�g
	task->SetID(id);
	//�^�X�N�̐���
	IF(task->Create()==false)
	{
		return false;
	}

	//�ǉ�
//	CTkyMultiTaskThread *thread = CTkyMultiTaskThread::CreateTaskThread();
//	thread->SetTask(task);

	//�V���O���X���b�h���͂��������(�ꎞ��~���Ă���)
//	task->Run( TKYTHREAD_PAUSE_LOOP );

	//�n���h���̒ǉ�
//	m_Handle.push_back(thread);
	m_Handle.push_back(task);

	return TRUE;
}
//�^�X�N�̏���
bool CTkyMultiTaskSystem::DestroyTask( LPTKYMULTITASK task )
{
	list<LPTKYMULTITASK>::iterator it = m_Handle.begin();
	while( it != m_Handle.end())
	{
		IF( *it == task )
		{
			//����
			it = m_Handle.erase(it);
			task->DestroyOwnObject();
			return true;
		}
		it++;
	}
	return false;
}
VOID CTkyMultiTaskSystem::Update(float step)
{
	list<LPTKYMULTITASK>::iterator it = m_Handle.begin();

	SWITCH(0)
	{
		case 0: //�P�X���b�h
			while(it != m_Handle.end())
			{
				(*it)->Update(step);
				it++;
			}
		break;
		case 1: //�}���`�X���b�h

			WHILE(it != m_Handle.end())
			{
				(*it)->SetStep(step);
				(*it)->Start();
				it++;
			}

			it = m_Handle.begin();
			WHILE(it != m_Handle.end())
			{
				IF(	(*it)->GetParam() ==  TKYTHREAD_PAUSE_LOOP )
				{
					it++;
				}
			}

		break;
	}

}
//�R���E�f�X�g���N�^
CTkyMultiTaskSystem::CTkyMultiTaskSystem()
{
	m_Handle.clear();

	//�N���e�B�J���Z�N�V�����̏�����
	InitializeCriticalSection(&m_critical);

}
CTkyMultiTaskSystem::~CTkyMultiTaskSystem()
{
	//�^�X�N�̏I��
	list<LPTKYMULTITASK>::iterator ih = m_Handle.begin();
	while(ih != m_Handle.end())
	{
		(*ih)->Stop();
		ih++;
	}
	m_Handle.clear();

	//�N���e�B�J���Z�N�V�����̊J��
	DeleteCriticalSection(&m_critical);

}