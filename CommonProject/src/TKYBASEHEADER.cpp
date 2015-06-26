#include "TKYBASEHEADER.h"
#include "TkyThread.h"
#include <process.h>
#include <TlHelp32.h>


//�o�^���ꂽ�I�u�W�F�N�g�̐�
STATIC UINT g_ObjectNum=NULL;
//�N���e�B�J���Z�N�V����
STATIC CRITICAL_SECTION g_critical;
//--------------------------------------------------------------------------------------------------------------
//			CTkyObjectThread
//--------------------------------------------------------------------------------------------------------------
CLASS_POINTA( CTkyObjectThread , LPTkyObjectThread ) :public ITkyThread
{
private:

	//�R���X�g���N�^
	CTkyObjectThread()
	{
		m_state = false;
		m_Info = NULL;
	}

	VIRTUAL VOID onThreadUpdate()
	{
		WHILE(1)
		{

			//�N���e�B�J���Z�N�V�����ɓ���
			EnterCriticalSection(&g_critical);

			//�ǉ�����
			m_Info=1;

			WHILE( !m_Insertobject.empty() )
			{
				m_object.push_back( m_Insertobject.front() );
				m_Insertobject.pop();
			}

			//�폜����
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

			//������
			LeaveCriticalSection(&g_critical);
			


			//�������
			m_Info=NULL;

			//�I���Ȃ��ā@
			IF(m_state == true)
			{
				//���ׂďI����Ă�����
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

	//�I�����ǂ���
	bool m_state;
	DWORD m_Info;

public:
	vector<LPTKYOBJECT> m_object;
	queue<LPTKYOBJECT> m_Insertobject;
	queue<LPTKYOBJECT> m_Deleteobject;

	//���ׂď���
	VOID ALLDELETE()
	{
		m_state = true;
		
		Stop();

		m_object.clear();
		WHILE(!m_Insertobject.empty())m_Insertobject.pop();
		WHILE(!m_Deleteobject.empty())m_Deleteobject.pop();
	}

	//���擾
	DWORD GetInfo(){return m_Info;}

	STATIC LPTkyObjectThread CreateObjectThread(){return new CTkyObjectThread();};

};

//--------------------------------------------------------------------------------------------------------------
//			ITkyObject
//--------------------------------------------------------------------------------------------------------------
STATIC LPTkyObjectThread g_ObjectThread;

//���ׂĉ��
VOID ITkyObject::DestroyObject()
{

#ifdef _DEBUG
	//�I�u�W�F�N�g���̊m�F
	CHAR str[MAX_PATH];
	_snprintf( str , MAX_PATH ,  "Object:%d" , g_ObjectNum  );
//	MessageBoxA( NULL , str , "ObjectSTART" ,NULL );
#endif
	
	g_ObjectThread->ALLDELETE();

#ifdef _DEBUG
	//�I�u�W�F�N�g���̊m�F
	_snprintf( str , MAX_PATH ,  "Object:%d" , g_ObjectNum  );
//	MessageBoxA( NULL , str , "ObjectEND" ,NULL );
#endif

	//�X���b�h������
	SAFE_DELETE( g_ObjectThread );

	//�N���e�B�J���Z�N�V�����̉��
	DeleteCriticalSection(&g_critical);

}

//���ׂď�����
VOID ITkyObject::CreateObject()
{
	//�N���e�B�J���Z�N�V����������
	InitializeCriticalSection(&g_critical);

	g_ObjectThread = CTkyObjectThread::CreateObjectThread();
	g_ObjectThread->Run( TKYTHREAD_ONCE_LOOP );
}

//�ʊJ��
bool ITkyObject::DestroyOwnObject()
{

	EnterCriticalSection(&g_critical);

	//�����ǉ�
	g_ObjectThread->m_Deleteobject.push(this);


	LeaveCriticalSection(&g_critical);



	return true;
}

//�����f���[��
VOID ITkyObject::ForceDelete()
{
	assert(this);
	delete(this);
}

ITkyObject::ITkyObject()		//�I�u�W�F�N�g�̒ǉ�������
{
	//�����g_ObjectThread�͒ǉ�����Ȃ��̂ō폜���Ȃ��Ă��悢
	IF( g_ObjectThread!=NULL )
	{

		EnterCriticalSection(&g_critical);

		//�I�u�W�F�N�g�ǉ�
		g_ObjectNum++;
		g_ObjectThread->m_Insertobject.push(this);

		LeaveCriticalSection(&g_critical);

	}
}
ITkyObject::~ITkyObject()
{
	//�I�u�W�F�N�g�̏���
	g_ObjectNum--;
}
