//--------------------------------------------------------------------------------------------------------------
//			タスクのメインヘッダー
//					タスクの情報を入れる		・何かの動作タスク　・イベントタスクまでいろいろ
//					発生できるようにしている
//--------------------------------------------------------------------------------------------------------------
#pragma once

#include "TKYBASEHEADER.h"
#include <assert.h>
#include "TkyThread.h"


//--------------------------------------------------------------------------------------------------------------
//			タスクのメインヘッダー
//					タスクの情報を入れる		・何かの動作タスク　・イベントタスクまでいろいろ
//					発生できるようにしている
//--------------------------------------------------------------------------------------------------------------
class ITKYTASK
{

public:
	virtual void Update() = 0;
	virtual bool Cheak() = 0;
};


//--------------------------------------------------------------------------------------------------------------
//			TKYライブラリ基本クラス,構造体
//--------------------------------------------------------------------------------------------------------------

/*

	2011/3/5

	タスクシステムの使用方法

	//準備

	ITkyMultiTask
	を継承し各子タスクを作る	
	↑のメンバで
	static によりNEWをラップする

	CTkyMultiTaskSystem
	を継承しタスク管理を作成する
	↑のメンバで
	static によりNEWをラップする

	//追加
	
	CTkyMultiTaskSystem::AddTask( LPTKYMULTITASK );
	を使用しタスクを追加していくその際
	タスク側ではCreate()が呼び出される

	
	static VOID ITkyMultiTask::SetNowTaskSystem(LPTkyMultiTaskSyatem)
	を使用し、使用するタスクシステムを選択する。

	タスク内では↓を使用しタスクの制御をする。
	static LPTKYMULTITASKSYSTEM m_NowSystem;

	//更新

	メインループ内では↓で更新ができる
	CTkyMultiTaskSystem::UpdateTask();
	その際各タスクではUpdateが呼び出される

	//開放

	タスクシステム内で↓をつかい消去後のListのIteraterをもらう
	CTkyMultiTaskSystem::DestroyTask(DestroyTask( HTKYTASKID ));



*/

//--------------------------------------------------------------------------------------------------------------
//			タスクのメインヘッダー
//					タスクの情報を入れる		・何かの動作タスク　・イベントタスクまでいろいろ
//					発生できるようにしている
//--------------------------------------------------------------------------------------------------------------

//タスクID宣言
CREATE_HANDLE(HTKYTASKID);


CLASS_POINTA(ITkyMultiTask , LPTKYMULTITASK) :public ITkyThread
{

//タスク要素

//--動的--
private:	
	//タスクID
	HTKYTASKID m_hid;

	//次のタスク
	LPTKYMULTITASK m_next;
	LPTKYMULTITASK m_before;

	//ステップ
	float m_step;

public:

	//タスクの生成
	BOOL Create()
	{
		return onCreate();
	}
	//タスクの実行
	BOOL Update(float step)
	{
		onUpdate(step);
		IF(m_next!=NULL)
		{
			m_next->Update(step);
		}
		return true;
	}
	//仮想関数(タスク要素の記述)
	VIRTUAL BOOL onCreate()=0;
	VIRTUAL BOOL onUpdate(float step)=0;

	//ID操作
	//設定
	VOID SetStep(float step){m_step = step;}
	VOID SetID(HTKYTASKID id){m_hid = id;}
	VOID SetID(DWORD id){m_hid.num = id;}
	VOID SetNextTask(LPTKYMULTITASK t){m_next = t;}
	VOID SetBeforeTask(LPTKYMULTITASK t){m_before = t;}
	//取得
	HTKYTASKID GetID(){return m_hid;}
	//確認
	bool	IsID(HTKYTASKID id){return (id.num==m_hid.num) ? true : false;}

protected:

	//コンストラクタ・デストラクタ
	ITkyMultiTask();
	VIRTUAL ~ITkyMultiTask();

//スレッド要素

private:

	//更新仮想化
	VIRTUAL VOID onThreadUpdate()
	{
		Update(m_step);
		this->Pause();
	}

};
//メインのタスク制御スーパークラス
CLASS_POINTA_OBJECT(CTkyMultiTaskSystem , LPTKYMULTITASKSYSTEM )
{
private:
	list<LPTKYMULTITASK> m_Handle;

	//クリティカルセクションの宣言
	CRITICAL_SECTION m_critical;

public:
	CTkyMultiTaskSystem();
	VIRTUAL ~CTkyMultiTaskSystem();

	//タスクの追加をする(今の位置からか指定)(スレッドを作成する)
	BOOL AddTask( LPTKYMULTITASK );
	//タスクの消去
	bool DestroyTask( LPTKYMULTITASK );
	//タスクの更新
	VOID Update(float step);

	//取得
	list<LPTKYMULTITASK> GetThread(){return m_Handle;}

	//生成
	STATIC LPTKYMULTITASKSYSTEM CreateTaskSystem(){return new CTkyMultiTaskSystem();}

protected:

};
