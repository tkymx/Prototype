#pragma once

#include "TKYBASEHEADER.h"
#include "DataStructure.h"
#include "TkyLogicView.h"
#include "TKYDIRECTINPUT.h"
#include "TKYFONT.h"

#include <fstream>

using namespace std;

//ビューを消す
#define ADD_VIEW_MESSAGE(view) CEventManager::StaticAddEventMessage( "LOGIC_ADDVIEW" , (DWORD)(view) );
#define DELETE_VIEW_MESSAGE(view) CEventManager::StaticAddEventMessage( "LOGIC_DELETEVIEW" , (DWORD)(view) );

static BOOL _FPSGameViews(){return true;};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//キャラクタービュー
//　キャラクタ情報の基本となるよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IFPSCharactorView , LPTkyFPSCharactorView ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	IFPSCharactorView();
	IFPSCharactorView(const IFPSCharactorView&);
	VIRTUAL ~IFPSCharactorView();

	//キャラクターデータ
	LPTkyCharactor				m_chara;
	//武器データ
	LPTkyWeapon				m_weapon;

public:
//GameView 要素
	VIRTUAL BOOL onCreate() ;	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Charactor";}
//独自要素
	//設定(データを入れるだけなのでポインタとしてみない)
	BOOL SetCharactorWeaponData( CCharactor* , CWeapon* );
	//取得
	LPTkyCharactor		GetCharactor()const{return m_chara;}	//キャラクターデータ
	LPTkyWeapon				GetWeapon()const{return m_weapon;}	//武器データ
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSHeroView :: キャラクタービュー  //シングルトンなやつ
//　主人公用　今のところは何にもすることはない
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSHeroView , LPTkyFPSHeroView ) :public IFPSCharactorView
{
protected:
	//コンストラクタ・デストラクタ
	CFPSHeroView();
	CFPSHeroView(const CFPSHeroView&);
	VIRTUAL ~CFPSHeroView();
public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Charactor_Hero";}
//独自要素
	STATIC LPTkyFPSHeroView CreateFPSHeroView();
	STATIC LPTkyFPSHeroView CreateFPSHeroView( CCharactor* , CWeapon* );
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSEnemyView :: キャラクタービュー
//　敵用　今のところは何にもすることはない
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSEnemyView , LPTkyFPSEnemyView ) :public IFPSCharactorView
{
protected:
	//コンストラクタ・デストラクタ
	CFPSEnemyView();
	CFPSEnemyView(const CFPSEnemyView&);
	VIRTUAL ~CFPSEnemyView();

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Charactor_Enemy";}
//独自要素
	STATIC LPTkyFPSEnemyView CreateFPSEnemyView();
	STATIC LPTkyFPSEnemyView CreateFPSEnemyView(CCharactor* , CWeapon*);
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CBulletView 
//　弾用、追加された後に障害物と比較され寿命が決まる
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CBulletView , LPTkyBulletView )  :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CBulletView();
	CBulletView(const CBulletView&);
	VIRTUAL ~CBulletView();

	//物質情報
	LPTkyBullet m_Bullet;
	//弾を撃った武器
	LPTkyWeapon	m_ShotWeapon;

	//発射位置
	Vector m_BatteryPosition;



public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Bullet";}



//独自要素
	//生成
	STATIC LPTkyBulletView CreateBullet( );
	STATIC LPTkyBulletView CreateBullet( LPTkyWeapon );
	STATIC LPTkyBulletView CreateBullet( CBullet* , LPTkyWeapon );
	//設定取得
	LPTkyBullet GetBullet()const{return m_Bullet;}
	VOID SetBullet(CBullet *t){*m_Bullet=*t;}
	LPTkyWeapon	GetWeapon()const{return m_ShotWeapon;}
	VOID SetWeapon(LPTkyWeapon w){m_ShotWeapon = w;}
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMapView  //シングルトンなやつ
//　マップはただそこにいるだけでいいんだよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFPSMapView , LPTkyFPSMapView ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CFPSMapView();
	CFPSMapView(const CFPSMapView&);
	VIRTUAL ~CFPSMapView();

	//Xファイル
	LPTkyXFile	m_map;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Map";}
//独自要素
	//生成
	STATIC LPTkyFPSMapView CreateMap( );
	STATIC LPTkyFPSMapView CreateMap( LPTkyXFile );
	//取得設定
	LPTkyXFile	GetModel()const{return m_map;};	//Xファイル
	VOID	SetModel(LPTkyXFile x){m_map = x;};	//Xファイル
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEffectView 
//　終わったら自分で自動的に終わるよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CEffectView , LPTkyEffectView ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CEffectView();
	CEffectView(const CEffectView&);
	VIRTUAL ~CEffectView();

	//弾情報
	CGameEffect m_Effect;
	//表示位置
	Vector m_pos;
	Vector m_rotate;
	Vector m_expand;

	//タイプ情報
	TKYTEXTURE_TYPE m_type;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_Effect";}
//独自要素
	//生成
	STATIC LPTkyEffectView CreateEffect( );
	STATIC LPTkyEffectView CreateEffect( CGameEffect *Effect ,	Vector pos , Vector m_rotate , Vector expand , TKYTEXTURE_TYPE type);
	//設定取得
	LPTkyGameEffect GetEffect()const{return (const LPTkyGameEffect)(&m_Effect);}
	Vector GetPos()const{return m_pos;}
	Vector GetRotate()const{return m_rotate;}
	Vector GetExpand()const{return m_expand;}
	VOID SetEffect(CGameEffect* t){m_Effect=*t;}
	VOID SetPos(Vector v){m_pos=v;}
	VOID SetRotate(Vector v){m_rotate=v;}
	VOID SetExpand(Vector v){m_expand=v;}
	VOID SetTextureType(TKYTEXTURE_TYPE t){m_type = t;}
};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//IFpsAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IFpsAIView , LPTkyFpsAIView ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	IFpsAIView();
	IFpsAIView(const IFpsAIView &);
	VIRTUAL ~IFpsAIView ();

	//キャラクタビュー( 消去時に消去メッセージを出す )
	LPTkyFPSCharactorView m_charaview;

public:
//GameView 要素
	VIRTUAL BOOL onCreate()=0;	//初期化
	VIRTUAL BOOL onUpdate(float step)=0;	//更新
	VIRTUAL VOID onRender()=0;	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsAI";}
//独自要素
	//取得設定
	LPTkyFPSCharactorView GetCharactorView()const{return m_charaview;}
	VOID SetCharactorView(LPTkyFPSCharactorView c){m_charaview = c;}

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsMoveAIView  //シングルトンなやつ
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsMoveAIView , LPTkyFpsMoveAIView ) :public IFpsAIView
{
protected:
	//コンストラクタ・デストラクタ
	CFpsMoveAIView();
	CFpsMoveAIView(const CFpsMoveAIView&);
	VIRTUAL ~CFpsMoveAIView();

	//回転かく(マウスの)
	Vector m_rotate;
	//マウス情報
	DIMOUSESTATE m_mouse;

	//
	Vector m_BaseVector;

	//カメラの座標、向き
	Vector *m_pos,*m_Direction;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsMoveAI";}

	VOID SetBaseVector(Vector v){m_BaseVector = v;}

//独自要素
	//生成
	STATIC LPTkyFpsMoveAIView CreateFpsMoveAI( );
	STATIC LPTkyFpsMoveAIView CreateFpsMoveAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction );
	//設定
	VOID SetCmameraVector(Vector *pos,Vector *Direction){ m_pos = pos,m_Direction = Direction; }
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsAutoAIView  //シングルトンなやつ
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsAutoAIView , LPTkyFpsAutoAIView ) :public IFpsAIView
{
protected:
	//コンストラクタ・デストラクタ
	CFpsAutoAIView(wstring name);
	CFpsAutoAIView(const CFpsAutoAIView&);
	VIRTUAL ~CFpsAutoAIView();

	//カメラの座標、向き
	Vector *m_pos,*m_Direction;



	//オート情報
	LPTkyAutoKeyInput			m_keyinfo;
	LPTkyAutoMouseInput		m_mouseinfo;

	//今の状態
	bool m_run;

	wstring m_file;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsAutoAI";}
//独自要素
	//生成
	STATIC LPTkyFpsAutoAIView CreateFpsAutoAI( wstring name );
	STATIC LPTkyFpsAutoAIView CreateFpsAutoAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction , wstring name );
	//設定
	VOID SetCmameraVector(Vector *pos,Vector *Direction){ m_pos = pos,m_Direction = Direction; }



	//取得
	LPTkyAutoKeyInput			GetAutoKeyInfo(){return m_keyinfo;}
	LPTkyAutoMouseInput		GetAutoMouseInfo(){return m_mouseinfo;}
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyAIView , LPTkyFpsEnemyAIView ) :public IFpsAIView
{
protected:

	//コンストラクタ・デストラクタ
	CFpsEnemyAIView();
	CFpsEnemyAIView(const CFpsEnemyAIView&);
	VIRTUAL ~CFpsEnemyAIView();

	Vector *m_heropos;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyAI";}
//独自要素
	//生成
	STATIC LPTkyFpsEnemyAIView CreateFpsEnemyAI( );
	STATIC LPTkyFpsEnemyAIView CreateFpsEnemyAI( LPTkyFPSCharactorView chara );
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAttackLeaveAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyAttackLeaveAIView , LPTkyFpsEnemyAttackLeaveAIView ) :public IFpsAIView
{
protected:

	//コンストラクタ・デストラクタ
	CFpsEnemyAttackLeaveAIView();
	CFpsEnemyAttackLeaveAIView(const CFpsEnemyAttackLeaveAIView&);
	VIRTUAL ~CFpsEnemyAttackLeaveAIView();

	Vector *m_heropos;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyAttackLeaveAI";}
//独自要素
	//生成
	STATIC LPTkyFpsEnemyAttackLeaveAIView CreateFpsEnemyAttackLeaveAI( );
	STATIC LPTkyFpsEnemyAttackLeaveAIView CreateFpsEnemyAttackLeaveAI( LPTkyFPSCharactorView chara );
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyStopAttackAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyStopAttackAIView , LPTkyFpsEnemyStopAttackAIView ) :public IFpsAIView
{
protected:

	//コンストラクタ・デストラクタ
	CFpsEnemyStopAttackAIView();
	CFpsEnemyStopAttackAIView(const CFpsEnemyStopAttackAIView&);
	VIRTUAL ~CFpsEnemyStopAttackAIView();

	Vector *m_heropos;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyStopAttackAI";}
//独自要素
	//生成
	STATIC LPTkyFpsEnemyStopAttackAIView CreateFpsEnemyStopAttackAI( );
	STATIC LPTkyFpsEnemyStopAttackAIView CreateFpsEnemyStopAttackAI( LPTkyFPSCharactorView chara );
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyThrowAIView 
//　FPSようのAIを担当するよ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CFpsEnemyThrowAIView , LPTkyFpsEnemyThrowAIView ) :public IFpsAIView
{
protected:

	//コンストラクタ・デストラクタ
	CFpsEnemyThrowAIView();
	CFpsEnemyThrowAIView(const CFpsEnemyThrowAIView&);
	VIRTUAL ~CFpsEnemyThrowAIView();

	Vector *m_heropos;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_FpsEnemyThrowAI";}
//独自要素
	//生成
	STATIC LPTkyFpsEnemyThrowAIView CreateFpsEnemyThrowAI( );
	STATIC LPTkyFpsEnemyThrowAIView CreateFpsEnemyThrowAI( LPTkyFPSCharactorView chara );
};







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindowiew 
//　先にテキストに、ウィンドウ情報だけ送っておき、そのあとにテキスト情報を送りまくる
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTextWindowiew , LPTkyTextWindowiew ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CTextWindowiew();
	CTextWindowiew(const CTextWindowiew &);
	VIRTUAL ~CTextWindowiew ();

	//テキストウィンドウの情報
	LpTkyManageTextWindow m_ManageTextWindow;

	//次のウィンドウ
	LPTkyTextWindowiew m_NextTextWIndow;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_TextWinodwView";}
//設定取得
	LpTkyManageTextWindow GetManageTextWindow(){return  m_ManageTextWindow;};
	VOID SetManageTextWindow(LpTkyManageTextWindow t){*m_ManageTextWindow = *t;};

	LPTkyTextWindowiew GetNextTextWinodw(){return m_NextTextWIndow;}
	VOID SetNextTextWinodw(LPTkyTextWindowiew t)
	{
		//もし自分と同じだったら抜ける
		IF( m_NextTextWIndow == t )return;

		IF(m_NextTextWIndow==NULL)
		{
			//要素がなかったら追加
			m_NextTextWIndow = t;
		}
		else
		{
			//あったら次へ
			m_NextTextWIndow->SetNextTextWinodw(t);
		}
	}
	//要素の開放
	VOID DeleteNext()
	{
		IF(m_NextTextWIndow==NULL)
		{
			return;
		}
		else
		{
			//あったら次へ
			m_NextTextWIndow->DeleteNext();
			m_NextTextWIndow->DestroyOwnObject();
			m_NextTextWIndow = NULL;
		}
	}

//取得
	STATIC LPTkyTextWindowiew CreateTextWinodwView(){return new CTextWindowiew();}

	STATIC LPTkyTextWindowiew CreateTextWinodwView(TTextSet set)
	{
		LPTkyTextWindowiew r =  new CTextWindowiew();
		r->GetManageTextWindow()->SetTextInfo( set.m_text , set.m_window );
		return r;
	}

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindoRangeDecisionView  //シングルトンなやつ
//　テキストの範囲か確認		
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTextWindoRangeDecisionView  , LPTkyTextWindoRangeDecisionView  ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CTextWindoRangeDecisionView ();
	CTextWindoRangeDecisionView (const CTextWindoRangeDecisionView  &);
	VIRTUAL ~CTextWindoRangeDecisionView  ();

	//範囲情報管理
	CElementRange<LPTkyTextWindowiew> *m_range;

	//主人公の座標ベクトル
	Vector *m_pos;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_TextWinodwRangeDecisionView";}
//設定取得
	CElementRange<LPTkyTextWindowiew>* GetTextWinodwViewRange(){return m_range;}
	VOID GetTextWinodwViewRange(CElementRange<LPTkyTextWindowiew>* t){m_range = t;}

	//開放処理追加
	VOID Delete();

	VOID SetVector(Vector *v){m_pos = v;}
	Vector*  GetVector(){return m_pos;}

//生成
	STATIC LPTkyTextWindoRangeDecisionView CreateTextWinodwRangeDecision(Vector *p)
	{
		LPTkyTextWindoRangeDecisionView  v =  new CTextWindoRangeDecisionView();
		v->SetVector(p);
		return v;
	}
};






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyAppearRangeDecisionView   //シングルトンなやつ
//　テキストの範囲か確認
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CEnemyAppearRangeDecisionView  , LPTkyEnemyAppearRangeDecisionView  ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CEnemyAppearRangeDecisionView ();
	CEnemyAppearRangeDecisionView (const CEnemyAppearRangeDecisionView  &);
	VIRTUAL ~CEnemyAppearRangeDecisionView  ();

	//範囲情報管理
	CElementRange<LPTkyFPSEnemyView> *m_range;

	//主人公の座標ベクトル
	Vector *m_pos;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_EnemyAppearRangeDecisionView";}
//設定取得
	CElementRange<LPTkyFPSEnemyView>* GetEnemyApperViewRange(){return m_range;}
	VOID GetEnemyApperViewRange(CElementRange<LPTkyFPSEnemyView>* t){m_range = t;}

	//開放処理追加
	VOID Delete();

	VOID SetVector(Vector *v){m_pos = v;}
	Vector*  GetVector(){return m_pos;}

//生成
	STATIC LPTkyEnemyAppearRangeDecisionView CreateEnemyApperRangeDecision(Vector *p)
	{
		LPTkyEnemyAppearRangeDecisionView  v =  new CEnemyAppearRangeDecisionView();
		v->SetVector(p);
		return v;
	}
};






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyKillRangeDecisionView   //シングルトンなやつ
//　敵を倒すためにとまるところ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CEnemyKillRangeDecisionView  , LPTkyEnemyKillRangeDecisionView  ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CEnemyKillRangeDecisionView ();
	CEnemyKillRangeDecisionView (const CEnemyKillRangeDecisionView  &);
	VIRTUAL ~CEnemyKillRangeDecisionView  ();

	//範囲情報管理
	CElementRange<INT> *m_range;

	//主人公の座標ベクトル
	Vector *m_pos;

	//殺したい敵の数
	INT m_KillEnemy;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_EnemyKillRangeDecisionView";}
//設定取得
	CElementRange<INT>* GetEnemyKillViewRange(){return m_range;}
	VOID GetEnemyKillViewRange(CElementRange<INT>* t){m_range = t;}

	VOID SetVector(Vector *v){m_pos = v;}
	Vector*  GetVector(){return m_pos;}

//生成
	STATIC LPTkyEnemyKillRangeDecisionView CreateEnemyKillRangeDecision(Vector *p)
	{
		LPTkyEnemyKillRangeDecisionView  v =  new CEnemyKillRangeDecisionView();
		v->SetVector(p);
		return v;
	}
};










//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMovementPathView   //シングルトンなやつ
//　移動の軌跡を保存
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CMovementPathView  , LPTkyMovementPathView  ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CMovementPathView ();
	CMovementPathView (const CMovementPathView  &);
	VIRTUAL ~CMovementPathView  ();

	//情報
	LPTkyAutoKeyInput			m_KeyPath;
	LPTkyAutoMouseInput		m_MousePath;

	//今のフレーム数
	DWORD							m_NowKeyFrame;
	DWORD							m_NowMouseFrame;

	//キーの情報
	BYTE	m_key;
	//回転かく(マウスの)
	Vector m_rotate;
	//マウス情報
	DIMOUSESTATE m_mouse;

	Vector m_base;

	//保存するやつ
	wstring m_move;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_MovementPathView";}

	VOID SetBase(Vector v){m_base = v;}

	//設定
	VOID SetMoveName(wstring name){m_move = name;}

//生成
	STATIC LPTkyMovementPathView CreateMovementPath(wstring name)
	{
		LPTkyMovementPathView t = new CMovementPathView();
		t->SetMoveName(name);
		return t;
	}

};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CAdventureWindowiew 
//　先にテキストに、ウィンドウ情報だけ送っておき、そのあとにテキスト情報を送りまくる
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eAdventure{ TKYADVENTURE_SHOW , TKYADVENTURE_OPEN , TKYADVENTURE_ENDPROCESS , TKYADVENTURE_END , TKYADVENTURE_ESCAPE };
CLASS_POINTA( CAdventureWindowiew , LPTkyAdventureWindowiew ) :public IGameView
{
protected:
	//コンストラクタ・デストラクタ
	CAdventureWindowiew();
	CAdventureWindowiew(const CAdventureWindowiew &);
	VIRTUAL ~CAdventureWindowiew ();

	//テキストウィンドウの情報
	LpTkyManageTextWindow m_ManageAdventureWindow;
	//次のウィンドウ
	LPTkyAdventureWindowiew m_NextAdventureWIndow;

	//今の状態
	eAdventure m_adventure;

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_AdventureWinodwView";}
//設定取得
	LpTkyManageTextWindow GetManageAdventureWindow(){return  m_ManageAdventureWindow;};
	VOID SetManageAdventureWindow(LpTkyManageTextWindow t){*m_ManageAdventureWindow = *t;};

	LPTkyAdventureWindowiew GetNextAdventureWinodw(){return m_NextAdventureWIndow;}
	VOID SetNextAdventureWinodw(LPTkyAdventureWindowiew t)
	{
		IF( m_NextAdventureWIndow == t )return;

		IF(m_NextAdventureWIndow==NULL)
		{
			//要素がなかったら追加
			m_NextAdventureWIndow = t;
		}
		else
		{
			//あったら次へ
			m_NextAdventureWIndow->SetNextAdventureWinodw(t);
		}
	}

	//要素の開放
	VOID DeleteNext()
	{
		IF(m_NextAdventureWIndow==NULL)
		{
			return;
		}
		else
		{
			//あったら次へ
			m_NextAdventureWIndow->DeleteNext();
			m_NextAdventureWIndow->DestroyOwnObject();
			m_NextAdventureWIndow = NULL;
		}
	}


//取得
	STATIC LPTkyAdventureWindowiew CreateAdventureWinodwView(){return new CAdventureWindowiew();}

	STATIC LPTkyAdventureWindowiew CreateAdventureWinodwView(TTextSet set)
	{
		LPTkyAdventureWindowiew r =  new CAdventureWindowiew();
		r->GetManageAdventureWindow()->SetTextInfo( set.m_text , set.m_window );
		return r;
	}

};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTitleView 
//　タイトル情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTitleView , LPTkyTitleView ) :public IGameView
{
private:

	LPTkyTitle m_Title;

	CTitleView(TTitle);
	CTitleView( const CTitleView& );
	VIRTUAL ~CTitleView();

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_TitleView";}

	VOID SetTitle( TTitle t )
	{
		m_Title->SetTitle(t);
	}

//生成
	STATIC LPTkyTitleView CreateTitle( TTitle t )
	{
		return new CTitleView(t);
	}

};








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CScoreView 
//　タイトル情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CScoreView , LPTkyScoreView ) :public IGameView
{
private:

	LPTkyScore m_Score;

	CScoreView(TScore);
	CScoreView( const CScoreView& );
	VIRTUAL ~CScoreView();

public:
//GameView 要素
	VIRTUAL BOOL onCreate();	//初期化
	VIRTUAL BOOL onUpdate(float step);	//更新
	VIRTUAL VOID onRender();	//描画
//イベント要素
	//イベントをもらう
	VIRTUAL VOID onEvent( TEventMessage msg );
	//イベントネーム名前を決める
	VIRTUAL VOID onNameSet( string *str ){*str = "FPS_ScoreView";}

	LPTkyScore GetScore(){return m_Score;}
	VOID SetScore( TScore t )
	{
		m_Score->SetScore(t);
	}

//生成
	STATIC LPTkyScoreView CreateScore( TScore t )
	{
		return new CScoreView(t);
	}

};
