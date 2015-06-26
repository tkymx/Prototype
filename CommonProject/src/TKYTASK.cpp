
#include "TKYTASK.h"


//--------------------------------------------------------------------------------------------------------------
//			ITkyMultiTask
//--------------------------------------------------------------------------------------------------------------
//個別解放
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
//タスクの追加をする
BOOL CTkyMultiTaskSystem::AddTask( LPTKYMULTITASK task)
{
	//例外処理
	assert(task);
	//前のタスクの次の番号を入れる
	DWORD id = 1;
	IF(!m_Handle.empty())id = m_Handle.back()->GetID().num + 1;
	//IDのセット
	task->SetID(id);
	//タスクの生成
	IF(task->Create()==false)
	{
		return false;
	}

	//追加
//	CTkyMultiTaskThread *thread = CTkyMultiTaskThread::CreateTaskThread();
//	thread->SetTask(task);

	//シングルスレッド時はこれを消す(一時停止しておく)
//	task->Run( TKYTHREAD_PAUSE_LOOP );

	//ハンドルの追加
//	m_Handle.push_back(thread);
	m_Handle.push_back(task);

	return TRUE;
}
//タスクの消去
bool CTkyMultiTaskSystem::DestroyTask( LPTKYMULTITASK task )
{
	list<LPTKYMULTITASK>::iterator it = m_Handle.begin();
	while( it != m_Handle.end())
	{
		IF( *it == task )
		{
			//消去
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
		case 0: //単スレッド
			while(it != m_Handle.end())
			{
				(*it)->Update(step);
				it++;
			}
		break;
		case 1: //マルチスレッド

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
//コン・デストラクタ
CTkyMultiTaskSystem::CTkyMultiTaskSystem()
{
	m_Handle.clear();

	//クリティカルセクションの初期化
	InitializeCriticalSection(&m_critical);

}
CTkyMultiTaskSystem::~CTkyMultiTaskSystem()
{
	//タスクの終了
	list<LPTKYMULTITASK>::iterator ih = m_Handle.begin();
	while(ih != m_Handle.end())
	{
		(*ih)->Stop();
		ih++;
	}
	m_Handle.clear();

	//クリティカルセクションの開放
	DeleteCriticalSection(&m_critical);

}