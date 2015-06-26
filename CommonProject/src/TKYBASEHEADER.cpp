#include "TKYBASEHEADER.h"
#include "TkyThread.h"
#include <process.h>
#include <TlHelp32.h>


//登録されたオブジェクトの数
STATIC UINT g_ObjectNum=NULL;
//クリティカルセクション
STATIC CRITICAL_SECTION g_critical;
//--------------------------------------------------------------------------------------------------------------
//			CTkyObjectThread
//--------------------------------------------------------------------------------------------------------------
CLASS_POINTA( CTkyObjectThread , LPTkyObjectThread ) :public ITkyThread
{
private:

	//コンストラクタ
	CTkyObjectThread()
	{
		m_state = false;
		m_Info = NULL;
	}

	VIRTUAL VOID onThreadUpdate()
	{
		WHILE(1)
		{

			//クリティカルセクションに入る
			EnterCriticalSection(&g_critical);

			//追加縛り
			m_Info=1;

			WHILE( !m_Insertobject.empty() )
			{
				m_object.push_back( m_Insertobject.front() );
				m_Insertobject.pop();
			}

			//削除縛り
			m_Info=2;

			WHILE( !m_Deleteobject.empty() )
			{
				vector<LPTKYOBJECT>::iterator io = m_object.begin();
				WHILE( io != m_object.end() )
				{
					IF( *io == m_Deleteobject.front() )
					{

						assert(*io);
						(*io)->ForceDelete();

						io = m_object.erase(io);

						break;
					}
					io++;
				}
				m_Deleteobject.pop();
			}

			//抜ける
			LeaveCriticalSection(&g_critical);
			


			//縛り解除
			m_Info=NULL;

			//終わりなって　
			IF(m_state == true)
			{
				//すべて終わっていたら
				IF( m_Insertobject.empty() && m_Deleteobject.empty() )break;
			}
			Sleep(1);
		}

		vector<LPTKYOBJECT>::iterator io = m_object.begin();
		int i=0;
		WHILE( io != m_object.end() )
		{
			i++;
			IF(*io!=this)
			{
				assert(*io);
				(*io)->ForceDelete();
				io++;
			}
		}
	}

	//終了かどうか
	bool m_state;
	DWORD m_Info;

public:
	vector<LPTKYOBJECT> m_object;
	queue<LPTKYOBJECT> m_Insertobject;
	queue<LPTKYOBJECT> m_Deleteobject;

	//すべて消す
	VOID ALLDELETE()
	{
		m_state = true;
		
		Stop();

		m_object.clear();
		WHILE(!m_Insertobject.empty())m_Insertobject.pop();
		WHILE(!m_Deleteobject.empty())m_Deleteobject.pop();
	}

	//情報取得
	DWORD GetInfo(){return m_Info;}

	STATIC LPTkyObjectThread CreateObjectThread(){return new CTkyObjectThread();};

};

//--------------------------------------------------------------------------------------------------------------
//			ITkyObject
//--------------------------------------------------------------------------------------------------------------
STATIC LPTkyObjectThread g_ObjectThread;

//すべて解放
VOID ITkyObject::DestroyObject()
{

#ifdef _DEBUG
	//オブジェクト数の確認
	CHAR str[MAX_PATH];
	_snprintf( str , MAX_PATH ,  "Object:%d" , g_ObjectNum  );
//	MessageBoxA( NULL , str , "ObjectSTART" ,NULL );
#endif
	
	g_ObjectThread->ALLDELETE();

#ifdef _DEBUG
	//オブジェクト数の確認
	_snprintf( str , MAX_PATH ,  "Object:%d" , g_ObjectNum  );
//	MessageBoxA( NULL , str , "ObjectEND" ,NULL );
#endif

	//スレッドを消す
	SAFE_DELETE( g_ObjectThread );

	//クリティカルセクションの解放
	DeleteCriticalSection(&g_critical);

}

//すべて初期化
VOID ITkyObject::CreateObject()
{
	//クリティカルセクション初期化
	InitializeCriticalSection(&g_critical);

	g_ObjectThread = CTkyObjectThread::CreateObjectThread();
	g_ObjectThread->Run( TKYTHREAD_ONCE_LOOP );
}

//個別開放
bool ITkyObject::DestroyOwnObject()
{

	EnterCriticalSection(&g_critical);

	//消去追加
	g_ObjectThread->m_Deleteobject.push(this);


	LeaveCriticalSection(&g_critical);



	return true;
}

//強制デリーと
VOID ITkyObject::ForceDelete()
{
	assert(this);
	delete(this);
}

ITkyObject::ITkyObject()		//オブジェクトの追加をする
{
	//これでg_ObjectThreadは追加されないので削除しなくてもよい
	IF( g_ObjectThread!=NULL )
	{

		EnterCriticalSection(&g_critical);

		//オブジェクト追加
		g_ObjectNum++;
		g_ObjectThread->m_Insertobject.push(this);

		LeaveCriticalSection(&g_critical);

	}
}
ITkyObject::~ITkyObject()
{
	//オブジェクトの消去
	g_ObjectNum--;
}
