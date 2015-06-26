#include "TkyThread.h"

//============================================
//ITkyThread
//============================================

//�N�������`���[
UINT WINAPI ITkyThread::pLauncher(LPVOID value)
{
	reinterpret_cast<ITkyThread*>(value)->Execute();
	return NULL;
}

//���s
VOID ITkyThread::Execute()
{
	g_ThreadNum++;
	while(1)
	{
		SWITCH(m_etp)
		{
			case TKYTHREAD_NONE_LOOP:
				g_ThreadNum--;
				return;
			break;
			case TKYTHREAD_PAUSE_LOOP:
			break;
			case TKYTHREAD_ONCE_LOOP:
				onThreadUpdate();
				m_etp = TKYTHREAD_NONE_LOOP;
			break;
			case TKYTHREAD_ENDLESS_LOOP:
				onThreadUpdate();
			break;
		}
		Sleep(1);
	}
};

//�R���X�g���N�^�E�f�X�g���N�^
ITkyThread::ITkyThread()
{
	m_etp = TKYTHREAD_NONE_LOOP;
	m_handle = NULL;

	//�N���e�B�J���Z�N�V����������
	InitializeCriticalSection(&m_critical);

}
ITkyThread::~ITkyThread()
{
	Stop();

	//�N���e�B�J���Z�N�V�����̉��
	DeleteCriticalSection(&m_critical);
}

//�X���b�h�̎��s
VOID ITkyThread::Run( eThreadParam param )
{
	m_handle = (HANDLE)_beginthreadex(NULL, 0, pLauncher , this , 0, NULL);
	m_etp = param;
}

//�����I��(�G���h���X��)
VOID ITkyThread::Stop()
{
	IF(m_handle != NULL)
	{
		m_etp = TKYTHREAD_NONE_LOOP;
		WaitForSingleObject( m_handle , INFINITE );
		CloseHandle( m_handle );
		m_handle = NULL;
	}
}

//�ꎞ��~(�G���h���X��)
VOID ITkyThread::Pause()
{
	IF(	m_etp == TKYTHREAD_ENDLESS_LOOP)m_etp = TKYTHREAD_PAUSE_LOOP;
}

//�X�^�[�g(�G���h���X��)
VOID ITkyThread::Start()
{
	IF(	m_etp == TKYTHREAD_PAUSE_LOOP)m_etp = TKYTHREAD_ENDLESS_LOOP;
}

//�ÓI�f�[�^
UINT ITkyThread::g_ThreadNum = 0;