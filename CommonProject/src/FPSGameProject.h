#pragma once

#include "resource.h"

#include <queue>


#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyDirectXApplicationLayer.h"

#include "DataStructure.h"
#include "FPSGameViews.h"

#include "ResourceManage.h"

//ウィンドウの大きさ
const static DWORD WindowWidth = 800;
const static DWORD WindowHeight = 600;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ファイル読み込みタスク
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//ファイル読み込みよう
CLASS_POINTA( CTkyReadThread , LPTkyReadThread ) :public ITkyThread
{
private:

	CResourceManage *m_resource;
	CCharaManage	*m_chara;
	CTextManage		*m_text;

	CTkyReadThread(CResourceManage *r,CCharaManage *c,CTextManage	*t)
	{
		m_resource = r;
		m_chara = c;
		m_text  = t;
	}
	VIRTUAL ~CTkyReadThread()
	{
	}

	VIRTUAL VOID onThreadUpdate()
	{
		//リソース作成
		m_resource->LoadResource(TEXT("text/resource.txt"));
		//キャラの作成
		m_chara->LoadCharactor( TEXT("text/charactor.txt") , m_resource );
		//テキストの作成
		m_text->LoadText( TEXT("text/text.txt") , m_resource );
	}

public:

	STATIC LPTkyReadThread CreateReadThread(CResourceManage *r,CCharaManage *c,CTextManage	*t){return new CTkyReadThread(r,c,t);};

};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//クラス情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS(CFPSApplicationLayer);
CLASS(CFPSGameLogic );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPS構造体
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TFPSInfo )
{
	Vector *pos;
	Vector *vec;
};
TYPEDEF( TFPSInfo* , LPTkyFPSInfo );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ロジック
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eFpsLogic{ TKYFPSLOGIC_READ , TKYFPSLOGIC_RUN };
CLASS_POINTA( CFPSGameLogic , LPTkyFPSGameLogic ) : public IGameLogic
{

private:
	
	//世界情報
	LPTkyPhysicsWorld3D m_world3d;

	//コンストラクタ・デストラクタ
	CFPSGameLogic();
	CFPSGameLogic(const CFPSGameLogic&);
	VIRTUAL ~CFPSGameLogic();

	//DirectXの情報
	TDirectXInfo *m_info;

	//主要ゲームデータ
	CResourceManage ResourceManage;
	CCharaManage CharaManage;
	CTextManage TextManage;

	//基本データ
	CGameFlowManage GameFlowManage;

	//今の状態
	eFpsLogic m_state;

	//今のスコア
	DWORD m_NowScore;

	//読み込みタスク
	LPTkyReadThread m_read;

	//読み込み画像
	LPTkyTexture m_loading;

	//テキストウィンドウの順番待ちキュー
	LPTkyTextWindowiew m_TextWinodwQueue;
	LPTkyAdventureWindowiew m_AdvectureWinodwQueue;

public:
//GameLogic情報
	//初期化
	VIRTUAL BOOL onCreate() ;
	//更新( View更新よりも速い )
	VIRTUAL VOID onUpdate(float step);
	//更新
	VIRTUAL VOID onRender();

	//View設定時(View情報は自動的にタスクに追加されるのでその他の設定)
	VIRTUAL VOID onAddView( LPTkyGameView view );
	//View消去時
	VIRTUAL VOID onDeleteView( LPTkyGameView view );

//イベント情報
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "FPS_GameLogic";
	}
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
//独自情報
	//生成
	STATIC LPTkyFPSGameLogic CreateFPSGameLogic();
	STATIC LPTkyFPSGameLogic CreateFPSGameLogic(TDirectXInfo* info);
//取得設定
	TDirectXInfo*	GetDirectXInfo()const{return m_info;}
	VOID				SetDirectXInfo( TDirectXInfo *t ){m_info = t;}
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPSゲーム用
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSApplicationLayer , LPTkyFPSApplicationLayer ) :public IDirectXBaseApplicationLayer
{
private:

	//ロジック情報
	LPTkyFPSGameLogic m_Logic;

	//シェイド情報
	CShade m_shade;

	//コンストラクタ・デストラクタ
	CFPSApplicationLayer();
	CFPSApplicationLayer(const CFPSApplicationLayer&);
	VIRTUAL ~CFPSApplicationLayer();

protected:

	//DirectX用仮想関数
	VIRTUAL BOOL pOnCreateDirectX();
	VIRTUAL BOOL pOnUpdateDirectX(float step);
	VIRTUAL VOID pOnRenderDirectX();
	VIRTUAL VOID pOnEventDirectX(TEventMessage msg );

public:

	//生成
	STATIC LPTkyFPSApplicationLayer CreateFPSApplicationLayer()
	{
		return new CFPSApplicationLayer();
	}

};