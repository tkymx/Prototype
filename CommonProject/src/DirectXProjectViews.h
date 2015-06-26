#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyDirectXApplicationLayer.h"

#include "DataStructure.h"
#include "FPSGameViews.h"


//歩き
const STATIC float WorldWolk3D = 0.001f;
//空気抵抗
const STATIC float WorldAir3D = 0.0009f;
//重力
const STATIC float WorldGravity3D = -0.03f;
//ジャンプ力
const STATIC float WorldJump3D = 0.1f;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPS構造体
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct 
{
	Vector *pos;
	Vector *vec;
}TFPSInfo;
typedef TFPSInfo* LPTkyFPSInfo;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//モデルビュー
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IModelView , LPTkyModelView ) :public IGameView
{
private:
	//モデルデータ
	LPTkyXFile m_model;
	//物理モデルデータ
	LPPhysicsCircleModel m_PhysicsModel;

protected:

	//コンストラクタ・デストラクタ
	IModelView();
	VIRTUAL ~IModelView();

	//生成
	BOOL pCreateModel( wstring str );

public:

//GameView 要素

	//初期化
	VIRTUAL BOOL onCreate() = 0;
	//更新
	VIRTUAL BOOL onUpdate(float step) = 0;
	//ビュー要素
	//描画
	VIRTUAL VOID onRender() = 0;

//イベント要素

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "3DPhysicsModel";
	}

//独自要素

	//モデルの取得
	LPPhysicsCircleModel GetPhysicsModel(){return m_PhysicsModel;}
	//XFILEの取得
	LPTkyXFile GetXFile(){return m_model;}

	//座標設定
	VOID SetPos(Vector vec){m_PhysicsModel->SetPos(vec);}
	//モデルの設定
	VOID SetPhysicsModel(LPPhysicsCircleModel pcm){m_PhysicsModel = pcm;}
	//XFILEの設定
	VOID SetXFile(LPTkyXFile tx){m_model = tx;}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//普通オブジェクトモデル
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CNormalModelView , LPTkyNormalModelView ) :public IModelView
{
private:
	//コンストラクタ・デストラクタ
	CNormalModelView(wstring name);
	VIRTUAL ~CNormalModelView();
public:

//GameView 要素

	//初期化
	VIRTUAL BOOL onCreate();
	//更新
	VIRTUAL BOOL onUpdate(float step);
	//ビュー要素
	//描画
	VIRTUAL VOID onRender();

//イベント要素

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VOID CNormalModelView::onNameSet( string *str )
	{
		*str = "3DNormalPhysicsModel";
	}

//独自要素

	//生成
	STATIC LPTkyNormalModelView CreateNormalModelView( wstring name );
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//障害物モデル
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CObstacleModelView , LPTkyObstacleModelView ) :public IModelView
{
private:
	//コンストラクタ・デストラクタ
	CObstacleModelView(wstring name);
	VIRTUAL ~CObstacleModelView();
public:

//GameView 要素

	//初期化
	VIRTUAL BOOL onCreate();
	//更新
	VIRTUAL BOOL onUpdate(float step);
	//ビュー要素
	//描画
	VIRTUAL VOID onRender();

//イベント要素

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "3DObstaclePhysicsModel";
	}

//独自要素

	//生成
	STATIC LPTkyObstacleModelView CreateObstacleModelView( wstring name );
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPSモデル
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSModelView , LPTkyFPSModelView ) :public IModelView
{
private:
	//座標、向き
	Vector *m_pos,*m_Direction;

	//回転かく
	Vector m_rotate;

	//マウス情報
	DIMOUSESTATE m_mouse;

	//コンストラクタ・デストラクタ
	CFPSModelView(Vector *pos,Vector *vec);
	VIRTUAL ~CFPSModelView();
public:

//GameView 要素

	//初期化
	VIRTUAL BOOL onCreate();
	//更新
	VIRTUAL BOOL onUpdate(float step);
	//ビュー要素
	//描画
	VIRTUAL VOID onRender();

//イベント要素

	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "3DFPSPhysicsModel";
	}

//独自要素

	//生成
	STATIC LPTkyFPSModelView CreateFPSModelView(Vector *pos,Vector *vec);
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ステージ読み込みビュー
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CLoadGameView , LPTkyLoadGameView ) :public IGameView
{
private:
	//ロッジ区データ
	LPTkyGameLogic m_MainLogick;
	//FPSデータ
	LPTkyFPSInfo m_info;
protected:
	//コンストラクタ・デストラクタ
	CLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info);
	VIRTUAL ~CLoadGameView();
public:
//GameView情報
	//初期化
	VIRTUAL BOOL onCreate();
	//更新
	VIRTUAL BOOL onUpdate(float step);
	//描画
	VIRTUAL VOID onRender();
//イベント情報
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "LoadGameView";
	}
//独自要素
	//生成
	STATIC LPTkyLoadGameView CreateLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info);
};
