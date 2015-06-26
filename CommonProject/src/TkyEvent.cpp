#include "TkyEvent.h"

//===========================================
//イベントデータ
//===========================================
	IEventLisner::IEventLisner()
	{
		m_name = "NONE";
	}
	IEventLisner::~IEventLisner()
	{
		m_name.clear();
	}

	//名前を取得する
	string IEventLisner::GetName(){return m_name;}

	//初期化
	VOID IEventLisner::Create()
	{
		onNameSet( &m_name );
	}

//===========================================
//イベントマネージャー (シングルトン)
//===========================================


	//コンストラクタ、デストラクタ
	CEventManager::CEventManager()
	{
		m_stop = false;
	}
	CEventManager::~CEventManager()
	{
	}

	//イベントリスナーの追加
	bool CEventManager::AddEventLisner( LPTkyEventLisner Event)
	{
		IF( Event == NULL )return false;
		Event->Create();
		m_EventLisner.push_back(Event);

		return true;
	}
	//イベントリスナーの消去
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
	//イベントメッセージーの追加
	VOID CEventManager::AddEventMessage( TEventMessage message )
	{
		//メッセージすぐ更新
		list<LPTkyEventLisner> eventlisner = m_EventLisner;
		list<LPTkyEventLisner>::iterator ie = eventlisner.begin();
		WHILE( ie != eventlisner.end() )
		{
			//イベントの起動
			(*ie)->onEvent( message );
			ie++;

			//ストップの場合抜ける
			IF(m_stop == true)
			{
				m_stop = false;
				break;
			}
		}
		//メッセージの消去
		message.m_message.clear();
		message.m_value.clear();

//		m_message.push(message);

	}
	VOID CEventManager::StopEventMessage()
	{
		m_stop = true;
	}

	//イベント更新
	VOID CEventManager::Update()
	{
		//今は使っていなく、メッセージが着たらすぐ送信するようにしている
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
	//生成
	LPTkyEventManager CEventManager::CreateEventManager()
	{
		IF( m_gManager != NULL )return m_gManager;
		return m_gManager = new CEventManager();
	}
	//遠隔動作
	//イベントリスナーの追加
	bool CEventManager::StaticAddEventLisner( LPTkyEventLisner Event)
	{
		IF(Event==NULL)return false;
		m_gManager->AddEventLisner(Event);
		return true;
	}
	//イベントリスナーの消去
	bool CEventManager::StaticDeleteEventLisner( LPTkyEventLisner Event)
	{
		return m_gManager->DeleteEventLisner(Event);
	}
	//イベントメッセージーの追加
	VOID CEventManager::StaticAddEventMessage( TEventMessage message )
	{
		m_gManager->AddEventMessage(message);
	}
	//イベントメッセージーの追加
	VOID CEventManager::StaticAddEventMessage( string str , vector<DWORD> ele )
	{
		TEventMessage message = { str , ele };
		m_gManager->AddEventMessage(message);
	}
	//イベントメッセージーの追加
	VOID CEventManager::StaticAddEventMessage( string str , DWORD messe )
	{
		vector<DWORD> ele;
		ele.push_back(messe);
		TEventMessage message = { str , ele };
		m_gManager->AddEventMessage(message);
	}
	//メッセージの停止
	VOID CEventManager::StaticStopEventMessage(  )
	{
		m_gManager->StopEventMessage();
	}


LPTkyEventManager CEventManager::m_gManager = NULL;