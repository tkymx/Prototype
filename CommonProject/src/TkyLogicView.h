#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TkyEvent.h"
#include "TKYTASK.h"
#include <map>
/*

ゲームロジック

ゲームビュー

のインターフェイスの定義

*/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームインターフェイス
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IGameBase , LPTkyGameBase ):public IEventLisner
{
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームビュー
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IGameView , LPTkyGameView ) :public IGameBase 
{
protected:
public:

//ゲームビュー要素

	//タスク生成段階で呼ばれる
	//初期化
	VIRTUAL BOOL onCreate() = 0;
	//更新
	VIRTUAL BOOL onUpdate(float step) = 0;

	//ビュー要素
	//描画
	VIRTUAL VOID onRender() = 0;

//イベント要素

	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ) = 0;

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;

//タスク要素

};

//ゲームビュー格納用タスク
CLASS_POINTA( CGameViewTask , LPTkyGameViewTask ) : public ITkyMultiTask
{
private:

	LPTkyGameView m_view;

	CGameViewTask(  )
	{
	}
VIRTUAL ~	CGameViewTask()
	{
	}

public:
	//タスクの生成
	VIRTUAL BOOL onCreate()
	{
		//ビューの初期化処理
		return m_view->onCreate();
	}
	//タスクの実行
	VIRTUAL BOOL onUpdate(float step)
	{
		//ビューの更新
		return m_view->onUpdate(step);
	}

	//生成
	STATIC LPTkyGameViewTask CreateGameViewTask( LPTkyGameView view )
	{
		LPTkyGameViewTask t = new CGameViewTask();
		t->SetGameView( view );
		return t;
	}


	//設定
	bool SetGameView( LPTkyGameView view )
	{
		IF( view == NULL )return false;
		m_view = view;
		return true;
	}
	//取得
	LPTkyGameView GetGameView()
	{
		return m_view;
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームロジック(インターフェース)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IGameLogic , LPTkyGameLogic ) :public IGameBase
{
private:

	//プロセス管理(Viewをもつ)
	LPTKYMULTITASKSYSTEM m_TaskManager;
protected:

	//マップアドレスマップ
	map< LPTkyGameView , LPTKYMULTITASK > m_TaskMap;

	//コンストラクタ、デストラクタ
	IGameLogic()
	{
		m_TaskManager = CTkyMultiTaskSystem::CreateTaskSystem();
		CEventManager::StaticAddEventLisner(this);
	}
	VIRTUAL ~IGameLogic()
	{
	}

	//終了ビュー
	queue<LPTkyGameView> m_DeleteView;

public:

	//ビューのセットをする
	bool AddGameView( LPTkyGameView view )
	{
		assert(view);

		IF(view == NULL)return false;

		//メッセージに追加----------------------------------------------
		CEventManager::StaticAddEventLisner(view);
		//---------------------------------------------------------------

		//タスク情報の取得----------------------------------------------
		LPTKYMULTITASK task = CGameViewTask::CreateGameViewTask(view);
		//タスク追加(初期化処理)
		IF(m_TaskManager->AddTask( task )==false)
		{
			MessageBoxA(NULL,"タスクエッラー","ERR",MB_OK);
			return false;
		}
		//---------------------------------------------------------------

		//タスクとビューのマップ(消去するためのマップに追加)--------
		m_TaskMap.insert( map<LPTkyGameView,LPTKYMULTITASK>::value_type(view,task) );
		//---------------------------------------------------------------
		
		//継承先への追加処理
		onAddView(view);

		return true;
	}
	//ビューの消去
	bool DeleteGameView( LPTkyGameView view )
	{
		//いてれー田を作る
		map<LPTkyGameView,LPTKYMULTITASK>::iterator ig;
		//要素を探す
		ig = m_TaskMap.find(view);
		//見つかっていたら要素を消す
		IF(ig != m_TaskMap.end())
		{
			//タスクから消す
			m_TaskManager->DestroyTask(ig->second);
			//メッセージから消す
			CEventManager::StaticDeleteEventLisner(ig->first);
			//ものを消す
			(ig->first)->DestroyOwnObject();

			ig = m_TaskMap.erase( ig );
			return true;
		}
		return false;
	}
	//要素だけ消去
	bool DeleteGameViewOnlyIndex( LPTkyGameView view )
	{
		//いてれー田を作る
		map<LPTkyGameView,LPTKYMULTITASK>::iterator ig;
		//要素を探す
		ig = m_TaskMap.find(view);
		//見つかっていたら要素を消す
		IF(ig != m_TaskMap.end())
		{
			//タスクから消す
			m_TaskManager->DestroyTask(ig->second);
			//メッセージから消す
			CEventManager::StaticDeleteEventLisner(ig->first);

			ig = m_TaskMap.erase( ig );
			return true;
		}
		return false;
	}

	//初期化
	BOOL Create()
	{
		return onCreate();
	}

	//更新
	VOID Update(float step)
	{

		//終了予定ビューの消去
		WHILE( !m_DeleteView.empty() )
		{
			LPTkyGameView view = m_DeleteView.front();

			//ものを消す
			this->DeleteGameView( view );

			m_DeleteView.pop();
		}

		onUpdate(step);
		m_TaskManager->Update(step);
	}

	//描画
	VOID Render()
	{
		list<LPTKYMULTITASK> thread = m_TaskManager->GetThread();
		list<LPTKYMULTITASK>::iterator it = thread.begin();
		WHILE( it != thread.end() )
		{
			( (LPTkyGameViewTask)( *it ))->GetGameView()->onRender();
			it++;
		}

		onRender();
	}

	//一つ上の工程で呼ばれる
	//初期化
	VIRTUAL BOOL onCreate() = 0;
	//更新
	VIRTUAL VOID onUpdate(float step) = 0;
	//更新
	VIRTUAL VOID onRender() = 0;

	//View設定時
	VIRTUAL VOID onAddView( LPTkyGameView view ) = 0;

	//View消去時
	VIRTUAL VOID onDeleteView( LPTkyGameView view ) = 0;

	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "GAME";
	}
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg )
	{

		//追加メッセージが来たら、そのビューを追加する
		IF( msg.m_message == "LOGIC_ADDVIEW" )
		{
			this->AddGameView( (LPTkyGameView)msg.m_value[0] );
		}

		//終了メッセージをもらったら、終了ビューキューに入れておき、あとで消す
		IF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			//もし消したやつだったら
			IF( m_TaskMap[(LPTkyGameView)msg.m_value[0]] == NULL )
			{
				//一覧から消す
				this->DeleteGameViewOnlyIndex( (LPTkyGameView)msg.m_value[0] );

				//メッセージをとめる
				CEventManager::StaticStopEventMessage();
				return;
			}
			//消すのに入れる
			this->m_DeleteView.push( (LPTkyGameView)msg.m_value[0] );
			//消す
			onDeleteView((LPTkyGameView)msg.m_value[0]);
		}

	}

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームアプリケーションレイヤー
/*

WMの処理
イベント処理
キー入力
Windows基本処理

*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CLASS_POINTA( IApplicationLayer , LPTkyApplicationLayer ) :public IGameBase
{
private:

	LPTkyEventManager m_EventManager;

protected:

	IApplicationLayer()
	{
		m_EventManager = CEventManager::CreateEventManager();
		m_EventManager->AddEventLisner( this );
	}
	VIRTUAL ~IApplicationLayer()
	{
	}

public:

	//初期化
	BOOL Create()
	{
		return onCreate();
	}
	//更新
	BOOL LoopStart()
	{
		return onLoopStep();
	}

	//更新処理
	VIRTUAL BOOL onLoopStep()=0;

	//初期化
	VIRTUAL BOOL onCreate()=0;

	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "Application";
	}
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;

};