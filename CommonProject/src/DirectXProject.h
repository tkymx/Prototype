#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyWindowsApplication.h"

#include "DirectXProjectViews.h"
#include "DataStructure.h"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ロジック
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CDirectXGameLogic , LPTkyDirectXGameLogic ) : public IGameLogic
{
private:
	//世界情報
	LPTkyPhysicsWorld3D m_world3d;
	//コンストラクタ・デストラクタ
	CDirectXGameLogic();
	VIRTUAL ~CDirectXGameLogic();
public:
//GameLogic情報
	//初期化
	VIRTUAL BOOL onCreate() ;
	//更新( View更新よりも速い )
	VIRTUAL VOID onUpdate(float step);
	//View設定時(View情報は自動的にタスクに追加されるのでその他の設定)
	VIRTUAL VOID onAddView( LPTkyGameView view );
//イベント情報
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "DirectXGame";
	}
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
//独自情報
	//生成
	STATIC LPTkyDirectXGameLogic CreateDirectXGameLogic();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXテスト用
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CDirectXApplicationLayer , LPTkyDirectXApplicationLayer ) :public IWindowsApplicationLayer
{
private:
//DirectX情報

	//基本クラス
	LPTKY3D m_pDtata;
	//文字表示クラス
	LPTKYFONT m_pFont;
	//デバイスの取得
	LPTKYINPUT m_device;
	//環境
	LPTkyEnvironment m_envi;

//その他雑変数

	//高さ
	Vector m_pos;
	//視点
	Vector m_vec;
	//FPS情報
	TFPSInfo m_Info;
	//マウス情報
	DIMOUSESTATE m_mouse;
	//ゲームロジック
	LPTkyDirectXGameLogic m_logic;

//DirectXApplication要素

private:

	//コンストラクタ・デストラクタ
	CDirectXApplicationLayer();
	VIRTUAL ~CDirectXApplicationLayer();

	//キーの取得
	VOID				pKeyBoard();
public:
	STATIC LPTkyDirectXApplicationLayer CreateDirectXApplicationLayer();

//WindowsApplication要素	

private:
	VIRTUAL BOOL			pOnCreate();
	VIRTUAL BOOL			pOnUpdate();
//イベント要素
private:
	VIRTUAL	VOID			pOnEvent( TEventMessage msg );
};