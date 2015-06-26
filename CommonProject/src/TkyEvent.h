#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include <list>
#include <vector>
#include <queue>

/*

イベント処理簡易版
イベントリスナー

これはアプリケーション構造で一番大事な要素である

すべてはシングルトンされた

イベントマネージャを経由して
すべてに受け継がれる

イベントマネージャは
すべての要素に対して継承されなければならない

STATIC要素を利用して
イベントリスナーの生成、消去
メッセージの送信を行う



＊これからやること 4/3 0:17
イベントリスナーの処理は
スレッドする予定



*/

//イベント情報
struct TEventMessage
{
	//イベントネーム
	string m_message;
	//イベント要素
	vector<DWORD> m_value;

	static TEventMessage Initializel(string str,DWORD d)
	{
		TEventMessage m;
		m.m_message = str;
		m.m_value.push_back(d);
		return m;
	}

};

//イベントデータ
CLASS_POINTA_OBJECT( IEventLisner , LPTkyEventLisner )
{
private:

	//イベントネーム
	string m_name;

protected:
	
	//コンストラクタ
	IEventLisner();
	VIRTUAL ~IEventLisner();


public:

	//名前を決める
	VIRTUAL VOID onNameSet( string *str ) = 0;
	//名前を取得する
	VIRTUAL string GetName();

	//イベント取得
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;

	//初期化
	VOID Create();

};

//イベントマネージャー (シングルトン)
CLASS_POINTA_OBJECT( CEventManager , LPTkyEventManager )
{
private:
	//シングルトンイベント
	STATIC LPTkyEventManager m_gManager;
	
	//イベントリスナー
	list<LPTkyEventLisner> m_EventLisner;
	//メッセージQue
	queue<TEventMessage> m_message;

	//コンストラクタ、デストラクタ
	CEventManager();
	VIRTUAL ~CEventManager();

	//ストップ
	bool m_stop;

public:

	//イベントリスナーの追加
	bool AddEventLisner( LPTkyEventLisner Event);
	//イベントリスナーの消去
	bool DeleteEventLisner( LPTkyEventLisner Event);
	//イベントメッセージーの追加
	VOID AddEventMessage( TEventMessage message );
	//メッセージのストップ
	VOID StopEventMessage( );

	//イベント更新
	VOID Update();

	//生成
	STATIC LPTkyEventManager CreateEventManager();

	//遠隔動作
	//イベントリスナーの追加
	STATIC bool StaticAddEventLisner( LPTkyEventLisner Event);
	//イベントリスナーの消去
	STATIC bool StaticDeleteEventLisner( LPTkyEventLisner Event);

	//イベントメッセージーの追加
	STATIC VOID StaticAddEventMessage( TEventMessage message );
	//イベントメッセージーの追加
	STATIC VOID StaticAddEventMessage( string str , vector<DWORD> ele );
	//イベントメッセージーの追加
	STATIC VOID StaticAddEventMessage( string str , DWORD messe );
	//イベントメッセージーの停止
	STATIC VOID StaticStopEventMessage(  );

};








