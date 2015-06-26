#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "vector"

#include "directx.h"
#include "DataStructure.h"
#include "FPSGameViews.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//リソース情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CResourceManage )
{
private:
	//Xファイル情報
	CStringMatchW<LPTkyXFile> m_x;
	//画像ファイル情報
	CStringMatchW<LPTkyTexture> m_tex;
public:
	//コンストラクタ、デストラクタ
	CResourceManage();
	CResourceManage(const CResourceManage&);
	VIRTUAL ~CResourceManage();

	//追加
	VOID AddXFie(LPTkyXFile,wstring);
	VOID AddTexture(LPTkyTexture,wstring);
	//消去
	VOID DeleteXFie(wstring);
	VOID DeleteTexture(wstring);
	//取得
	LPTkyXFile		GetXFie(wstring);
	LPTkyTexture	GetTexture(wstring);

	//ファイルから取得
	VOID LoadResource(wstring name );

};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//キャラ情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TCharaSet )
{
	LPTkyCharactor m_chara;
	LPTkyWeapon m_weapon;
	VOID Initialize()
	{
		m_chara = NULL;
		m_weapon = NULL;
	}
	VOID NewAllocate()
	{
		//メモリー確保
		m_chara = CCharactor::CreateCharactor();
		m_weapon = CWeapon::CreateWeapon( m_chara );
	}
	TCharaSet()	{Initialize();}
	TCharaSet(LPTkyCharactor chara ,LPTkyWeapon weapon)
	{
		m_chara = chara;
		m_weapon = weapon;
	}
};

CLASS( CCharaManage )
{
private:
	//キャラ情報
	CStringMatchW<TCharaSet> m_chara;
public:
	//コンストラクタ、デストラクタ
	CCharaManage();
	CCharaManage(const CCharaManage&);
	VIRTUAL ~CCharaManage();

	//追加
	VOID AddCharactor(TCharaSet);
	//消去
	VOID DeleteCharactor(TCharaSet);
	VOID DeleteCharactor(wstring);	
	//取得
	TCharaSet Getchara(wstring);

	//ファイルから取得
	VOID LoadCharactor(wstring name, CResourceManage* resource);

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//テキスト情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CTextManage )
{
private:
	//キャラ情報
	CNumberMatch<TTextSet> m_Text;

	//ウィンドウ情報
	CNumberMatch<TTextWindowInfo*> textwindow;

public:
	//コンストラクタ、デストラクタ
	CTextManage();
	CTextManage(const CTextManage&);
	VIRTUAL ~CTextManage();

	//追加
	VOID AddText(INT,TTextSet);
	//消去
	VOID DeleteText(INT);
	//取得
	TTextSet GetText(INT);
	vector<TTextSet*> GetText(INT sn ,INT en);

	//ファイルから取得
	VOID LoadText(wstring name, CResourceManage* resource);

};





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//範囲情報の保存
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CRangeManage )
{
private:
	//範囲情報
	//テキスト
	LPTkyTextWindoRangeDecisionView m_TextRangeView;
	//敵
	LPTkyEnemyAppearRangeDecisionView m_EnemyRangeView;
	//敵に攻撃
	LPTkyEnemyKillRangeDecisionView		m_EnemyKillView;
public:
	//コンストラクタ、デストラクタ
	CRangeManage(Vector *pos);
	CRangeManage(const CRangeManage&);
	VIRTUAL ~CRangeManage();

	//取得
	LPTkyTextWindoRangeDecisionView GetTextRange()
	{
		//取得する場合は情報が消える
		LPTkyTextWindoRangeDecisionView t = m_TextRangeView;
		m_TextRangeView = NULL;
		return t;
	}
	LPTkyEnemyAppearRangeDecisionView GetEnemyRange()
	{
		//取得する場合は消える
		LPTkyEnemyAppearRangeDecisionView e = m_EnemyRangeView;
		m_EnemyRangeView = NULL;
		return e;
	}
	LPTkyEnemyKillRangeDecisionView GetEnemyKillRange()
	{
		//取得する場合は消える
		LPTkyEnemyKillRangeDecisionView e = m_EnemyKillView;
		m_EnemyKillView = NULL;
		return e;
	}	

	//ファイルから取得
	VOID LoadText(wstring name, CCharaManage *chara , CTextManage *text);
};






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームステージ情報（インターフェイス）
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA_OBJECT( IStageManage , LPTkyStageManage )
{
private:
public:

	//ファイルから取得
	VIRTUAL VOID LoadText(wstring name, CResourceManage *resource , CCharaManage *chara,CTextManage *text) = 0;
	//実行
	VIRTUAL VOID Run( LPTkyGameLogic logic) = 0;
	//終了
	VIRTUAL VOID Stop() = 0;

}; 







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームステージ情報（シューティング）
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CShootingStageManage , LPTkyShootingStageManage ) :public IStageManage
{
private:


	LPTkyTextWindoRangeDecisionView			textrange_view;			//テキスト情報ビュー
	LPTkyEnemyAppearRangeDecisionView		enemyrange_view;		//敵出現範囲情報
	LPTkyEnemyKillRangeDecisionView				killview;						//キルビュー
	LPTkyFPSMapView										map_view;					//マップ情報 
	LPTkyScoreView											score_view;					//スコア情報
	LPTkyFPSHeroView										hero_view;					//主人公ビュー

	Vector *pos , *vec;

	float n,f;

	//コンストラクタ、デストラクタ
	CShootingStageManage();
	CShootingStageManage(const CShootingStageManage&);
	VIRTUAL ~CShootingStageManage();

public:

	//ファイルから取得
	VIRTUAL VOID LoadText(wstring name, CResourceManage *resource , CCharaManage *chara,CTextManage *text);

	//実行
	VIRTUAL VOID Run( LPTkyGameLogic logic);

	//終了
	VIRTUAL VOID Stop();

	//セッテティング
	VOID SetPosVec( Vector *p ,Vector * v ){ pos = p; vec = v; }

	LPTkyScoreView				GetScore(){return	score_view;}					//スコア情報
	LPTkyFPSHeroView			GetChara(){return	hero_view;}					//主人公ビュー

	//取得
	STATIC LPTkyShootingStageManage CreateShootingStage( wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text , Vector *pos , Vector *vec )
	{
		LPTkyShootingStageManage t = new CShootingStageManage();
		t->SetPosVec( pos , vec );
		t->LoadText( name , resource , chara , text );
		return t;
	}

}; 








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームステージ情報（テキスト）
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTextStageManage , LPTkyTextStageManage ) :public IStageManage
{
private:

	queue< LPTkyAdventureWindowiew > m_adventure;

	//コンストラクタ、デストラクタ
	CTextStageManage();
	CTextStageManage(const CTextStageManage&);
	VIRTUAL ~CTextStageManage();

public:

	//ファイルから取得
	VIRTUAL VOID LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text);

	//実行
	VIRTUAL VOID Run( LPTkyGameLogic logic );

	//終了
	VIRTUAL VOID Stop();

	//取得
	STATIC LPTkyTextStageManage CreateTextStage( wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text  )
	{
		LPTkyTextStageManage t = new CTextStageManage();
		t->LoadText( name , resource , chara , text  );
		return t;
	}

};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームステージ情報（タイトル）
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CTitleManage , LPTkyTitleManage ) :public IStageManage
{
private:

	LPTkyTitleView m_tview;

	CTitleManage();
	CTitleManage( const CTitleManage& t );
	VIRTUAL ~CTitleManage();

public:

	//ファイルから取得
	VIRTUAL VOID LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text);

	//実行
	VIRTUAL VOID Run( LPTkyGameLogic logic );

	//終了
	VIRTUAL VOID Stop();

	//取得
	STATIC LPTkyTitleManage CreateTitleManage( wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text  )
	{
		LPTkyTitleManage t = new CTitleManage();
		t->LoadText( name , resource , chara , text  );
		return t;
	}

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームの流れ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS( CGameFlowManage ) 
{
private:

	//ステージ情報
	queue< LPTkyStageManage > m_stages;

	//体力
	INT m_hitpoint;
	//スコア
	DWORD m_score;

public:
	//コンストラクタ、デストラクタ
	CGameFlowManage();
	CGameFlowManage(const CGameFlowManage&);
	VIRTUAL ~CGameFlowManage();

	//ファイルから取得
	VOID LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text , Vector *pos , Vector *vec );

	//木曽情報の取得
	VOID GetBaseData();

	//実行
	VOID Run( LPTkyGameLogic logic );

	//次に進む
	BOOL Next();

	//終了
	VOID Stop();

};