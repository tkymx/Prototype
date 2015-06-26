#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TKYVECTOR.h"
#include "TkyCollision.h"
#include "TkyPhysics.h"

static VOID TkyWorld()
{
}

//==================== 物体タスクークラス ====================
CLASS_POINTA( CPhysicsModelTask , LPTkyPhysicsModelTask ) : public ITkyMultiTask
{
private:
	LPPhysicsModel			m_model;
	list<LPPhysicsModel>*	m_models;

	CPhysicsModelTask();
	CPhysicsModelTask( LPPhysicsModel model );
	VIRTUAL ~CPhysicsModelTask();

public:
	VIRTUAL BOOL onCreate();
	VIRTUAL BOOL onUpdate(float step);

	//設定系
	VOID SetModelList(list<LPPhysicsModel> *models){m_models = models;}

	//モデルの取得
	LPPhysicsModel GetModel(){return m_model;}
	
	//タスクの生成
	STATIC LPTkyPhysicsModelTask CreatePhysicsModelTask( LPPhysicsModel model );

};

//==================== 物体3Dタスクークラス ====================
CLASS_POINTA( CPhysics3DModelTask , LPTkyPhysics3DModelTask ) : public ITkyMultiTask
{
private:
	LPPhysicsModel			m_model;
	list<LPPhysicsModel>*	m_models;

	CPhysics3DModelTask();
	CPhysics3DModelTask( LPPhysicsModel model );
	VIRTUAL ~CPhysics3DModelTask();

public:
	VIRTUAL BOOL onCreate();
	VIRTUAL BOOL onUpdate(float step);

	//設定系
	VOID SetModelList(list<LPPhysicsModel> *models){m_models = models;}

	//モデルの取得
	LPPhysicsModel GetModel(){return m_model;}
	
	//タスクの生成
	STATIC LPTkyPhysics3DModelTask CreatePhysics3DModelTask( LPPhysicsModel model );

};


//==================== 物理マネージャーに当たる ====================

//-------------------------------------------
//物理世界の情報
//-------------------------------------------
CLASS_POINTA_OBJECT(CPhysicsWorld,LPTkyPhysicsWorld)
{
protected:

	//重力情報
	float m_Gravity;
	//空気抵抗
	float m_AirGap;
	//マップの範囲
	RECT m_range;

	//モデル情報
	list<PhysicsModel*> m_model;

	//コンストラクタ:デストラクタ	
	CPhysicsWorld();
	VIRTUAL ~CPhysicsWorld();

	//タスク完備クラス
	LPTKYMULTITASKSYSTEM m_task;

	//クリティカルセクション
	CRITICAL_SECTION m_critical;	

public:

	//追加（IDを返す）
	VIRTUAL bool AddPhysicsModel( PhysicsModel *model);
	VIRTUAL bool DeletePhysicsModel( PhysicsModel *model);

	//取得
	float GetGravity(){return m_Gravity;};
	float GetAirGap(){return m_AirGap;};

	list<PhysicsModel*> GetModels()const{return m_model;}
	DWORD GetModelNum(){return m_model.size();}

	//設定
	VOID SetGravity(float g){m_Gravity = g;}
	VOID SetAirGap(float a){m_AirGap = a;}
	VOID SetRange(RECT rect ){m_range = rect;}

	//更新
	VIRTUAL bool Update( float step );

	//作成
	STATIC LPTkyPhysicsWorld CreatePhysicsWorld();
};

//-------------------------------------------
//物理世界の情報3D
// まだタスクを使用して当たり判定していないがそのうちする
//-------------------------------------------
CLASS_POINTA(CPhysicsWorld3D,LPTkyPhysicsWorld3D) :public CPhysicsWorld
{
private:

	//障害物情報
	list<LPTkyXFile> m_obstaclemodel;

	//コンストラクタ:デストラクタ	
	CPhysicsWorld3D();
	VIRTUAL ~CPhysicsWorld3D();

public:

	//追加
	VIRTUAL bool	AddPhysicsModel( PhysicsModel *model);
	bool			AddObtaclePhysicsModel( LPTkyXFile model);

	//消去
	bool DeleteObtaclePhysicsModel( LPTkyXFile model);

	//今の位置から一番近い障害物までの距離( 負で無限遠 )
	FLOAT GetDistanceObstacle( Vector pos , Vector vec );

	//更新
	VIRTUAL bool Update( float step );

	//作成
	static LPTkyPhysicsWorld3D CreatePhysicsWorld3D();
};
