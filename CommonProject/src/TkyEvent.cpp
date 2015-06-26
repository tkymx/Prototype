#include "TkyEvent.h"

//===========================================
//�C�x���g�f�[�^
//===========================================
	IEventLisner::IEventLisner()
	{
		m_name = "NONE";
	}
	IEventLisner::~IEventLisner()
	{
		m_name.clear();
	}

	//���O���擾����
	string IEventLisner::GetName(){return m_name;}

	//������
	VOID IEventLisner::Create()
	{
		onNameSet( &m_name );
	}

//===========================================
//�C�x���g�}�l�[�W���[ (�V���O���g��)
//===========================================


	//�R���X�g���N�^�A�f�X�g���N�^
	CEventManager::CEventManager()
	{
		m_stop = false;
	}
	CEventManager::~CEventManager()
	{
	}

	//�C�x���g���X�i�[�̒ǉ�
	bool CEventManager::AddEventLisner( LPTkyEventLisner Event)
	{
		IF( Event == NULL )return false;
		Event->Create();
		m_EventLisner.push_back(Event);

		return true;
	}
	//�C�x���g���X�i�[�̏���
	bool CEventManager::DeleteEventLisner( LPTkyEventLisner Event)
	{
		list<LPTkyEventLisner>::iterator ie = m_EventLisner.begin();
		WHILE( ie != m_EventLisner.end() )
		{
			IF( *ie == Event )
			{
				ie = m_EventLisner.erase(ie);
				return true;
			}
			ie++;
		}
		return false;
	}
	//�C�x���g���b�Z�[�W�[�̒ǉ�
	VOID CEventManager::AddEventMessage( TEventMessage message )
	{
		//���b�Z�[�W�����X�V
		list<LPTkyEventLisner> eventlisner = m_EventLisner;
		list<LPTkyEventLisner>::iterator ie = eventlisner.begin();
		WHILE( ie != eventlisner.end() )
		{
			//�C�x���g�̋N��
			(*ie)->onEvent( message );
			ie++;

			//�X�g�b�v�̏ꍇ������
			IF(m_stop == true)
			{
				m_stop = false;
				break;
			}
		}
		//���b�Z�[�W�̏���
		message.m_message.clear();
		message.m_value.clear();

//		m_message.push(message);

	}
	VOID CEventManager::StopEventMessage()
	{
		m_stop = true;
	}

	//�C�x���g�X�V
	VOID CEventManager::Update()
	{
		//���͎g���Ă��Ȃ��A���b�Z�[�W�������炷�����M����悤�ɂ��Ă���
		WHILE( !m_message.empty() )
		{
			TEventMessage message = m_message.front();

			list<LPTkyEventLisner>::iterator ie = m_EventLisner.begin();
			WHILE( ie != m_EventLisner.end() )
			{
				(*ie)->onEvent( message );
				ie++;
			}

			message.m_message.clear();
			message.m_value.clear();
			m_message.pop();
		}
		
	}
	//����
	LPTkyEventManager CEventManager::CreateEventManager()
	{
		IF( m_gManager != NULL )return m_gManager;
		return m_gManager = new CEventManager();
	}
	//���u����
	//�C�x���g���X�i�[�̒ǉ�
	bool CEventManager::StaticAddEventLisner( LPTkyEventLisner Event)
	{
		IF(Event==NULL)return false;
		m_gManager->AddEventLisner(Event);
		return true;
	}
	//�C�x���g���X�i�[�̏���
	bool CEventManager::StaticDeleteEventLisner( LPTkyEventLisner Event)
	{
		return m_gManager->DeleteEventLisner(Event);
	}
	//�C�x���g���b�Z�[�W�[�̒ǉ�
	VOID CEventManager::StaticAddEventMessage( TEventMessage message )
	{
		m_gManager->AddEventMessage(message);
	}
	//�C�x���g���b�Z�[�W�[�̒ǉ�
	VOID CEventManager::StaticAddEventMessage( string str , vector<DWORD> ele )
	{
		TEventMessage message = { str , ele };
		m_gManager->AddEventMessage(message);
	}
	//�C�x���g���b�Z�[�W�[�̒ǉ�
	VOID CEventManager::StaticAddEventMessage( string str , DWORD messe )
	{
		vector<DWORD> ele;
		ele.push_back(messe);
		TEventMessage message = { str , ele };
		m_gManager->AddEventMessage(message);
	}
	//���b�Z�[�W�̒�~
	VOID CEventManager::StaticStopEventMessage(  )
	{
		m_gManager->StopEventMessage();
	}


LPTkyEventManager CEventManager::m_gManager = NULL;