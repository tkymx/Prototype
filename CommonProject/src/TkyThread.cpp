#include "TkyThread.h"

//============================================
//ITkyThread
//============================================

//起動ランチャー
UINT WINAPI ITkyThread::pLauncher(LPVOID value)
{
	reinterpret_cast<ITkyThread*>(value)->Execute();
	return NULL;
}

//実行
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

//コンストラクタ・デストラクタ
ITkyThread::ITkyThread()
{
	m_etp = TKYTHREAD_NONE_LOOP;
	m_handle = NULL;

	//クリティカルセクション初期化
	InitializeCriticalSection(&m_critical);

}
ITkyThread::~ITkyThread()
{
	Stop();

	//クリティカルセクションの解放
	DeleteCriticalSection(&m_critical);
}

//スレッドの実行
VOID ITkyThread::Run( eThreadParam param )
{
	m_handle = (HANDLE)_beginthreadex(NULL, 0, pLauncher , this , 0, NULL);
	m_etp = param;
}

//強制終了(エンドレス時)
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

//一時停止(エンドレス時)
VOID ITkyThread::Pause()
{
	IF(	m_etp == TKYTHREAD_ENDLESS_LOOP)m_etp = TKYTHREAD_PAUSE_LOOP;
}

//スタート(エンドレス時)
VOID ITkyThread::Start()
{
	IF(	m_etp == TKYTHREAD_PAUSE_LOOP)m_etp = TKYTHREAD_ENDLESS_LOOP;
}

//静的データ
UINT ITkyThread::g_ThreadNum = 0;