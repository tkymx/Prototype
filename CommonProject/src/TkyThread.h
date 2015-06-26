#pragma once

#include "TKYBASEHEADER.h"
#include <process.h>
#include "TKYLIBRARY.h"

/*

	スレッドクラスの定義
	
	RUNスレッドの実行

	onUpdateを継承することによってスレッドを作成できる

	Stopスレッドの終了

*/

enum eThreadParam
{ 
	TKYTHREAD_NONE_LOOP , 
	TKYTHREAD_PAUSE_LOOP , 
	TKYTHREAD_ONCE_LOOP , 
	TKYTHREAD_ENDLESS_LOOP 
};
CLASS_POINTA_OBJECT( ITkyThread , LPTkyThread )
{
private:

	//調査用変数
	STATIC UINT g_ThreadNum;

	//スレッドパラメーター
	eThreadParam m_etp;
	//スレッドハンドル
	HANDLE m_handle;
	//ランチャー用静的関数
	STATIC UINT WINAPI pLauncher(LPVOID value);
	//実行
	VOID Execute();


	//更新の仮想化
	VIRTUAL VOID onThreadUpdate()=0;

	//クリティカルセクション
	CRITICAL_SECTION m_critical;

protected:

	//コンストラクタ・デストラクタ
	ITkyThread();
	VIRTUAL ~ITkyThread();

public:
	//スレッドの実行
	VOID Run( eThreadParam param );

	//強制終了(エンドレス時)
	VOID Stop();

	//一時停止(エンドレス時)
	VOID Pause();

	//スタート(エンドレス時)
	VOID Start();

	//状態取得
	eThreadParam GetParam(){ return m_etp; }

	//ハンドルの取得
	HANDLE GetHandle()const{return m_handle;}

	//個数の取得(静的)
	STATIC UINT GetThreadNum(){return g_ThreadNum;}

};