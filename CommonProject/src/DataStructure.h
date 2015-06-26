#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "vector"

#include "TkyPhysics.h"
#include "TKY2DTEXTURE.h"
#include "TKYFONT.h"


using namespace std;

#define GetModelMessageP( pmodel ) pmodel->GetCollisionMessage().m_message

//================
//　データ設計を参考
//================

static void all()
{
}

//============================================================
//ゲームのデータ構造
//============================================================

//読み込みデータ
STRUCT( TGameText )
{
	wstring		m_key;
	wstring		m_text;
	DWORD m_element;

	VOID Initialize()
	{
		m_key.clear();
		m_text.clear();
		m_element = NULL;
	}

	TGameText(){Initialize();}
	TGameText( wstring k ,wstring t , DWORD e )
	{
		m_key = k;
		m_text = t;
		m_element = e;
	}
};
typedef vector<TGameText> TGameTextInfo;

//ゲームロードインターフェイス
interface IGameLoadText
{
public:
	VIRTUAL BOOL onLoadGameDate(TGameTextInfo info) = 0;
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//エフェクト情報
//	エフェクト情報はこれから、時間による、回転や透過など、どんどん進化していく予定
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//エフェクトデータ
STRUCT( TGameEffect )
{
	LPTkyTexture	m_texture;	//画像情報
	DWORD			m_life;				//アニメーションの寿命(ループ回数)
	
	BYTE					m_NowTexFrame;
	DWORD				m_NowTexTime;

	VOID Initialize()
	{
		m_texture = NULL;
		m_life = NULL;
		m_NowTexFrame = 0;
		m_NowTexTime = 0;
	}
	TGameEffect(){Initialize();}
	TGameEffect(LPTkyTexture texture , DWORD life)
	{
		Initialize();
		m_texture = texture;
		m_life = life;
	}
};
//エフェクト行動クラス
enum eTky_State{TKYSTATE_RUN,TKYSTATE_STOP};
CLASS_POINTA( CGameEffect , LPTkyGameEffect )
{
private:
	TGameEffect m_effect;
	//状態
	UINT m_state;
	//ループ一回したかフラグ
	bool m_oneLoop;
public:
	//コンストラクタ、デストラクタ、
	CGameEffect();
	CGameEffect( const CGameEffect& );
	VIRTUAL ~CGameEffect();

	BOOL CreateEffect(TGameEffect);
	BOOL UpdateEffect(float step);
	BOOL DrawEffect(Vector pos,Vector rotate , Vector expand);
	BOOL DrawEffect2D(Vector pos);

	VOID RUN(){m_state = TKYSTATE_RUN;};
	VOID STOP(){m_state = TKYSTATE_STOP;};

	bool					IsOneLoopEnd(){return m_oneLoop;}

	//設定取得
	VOID				SetEffectInfo(TGameEffect t){m_effect = t;}	
	TGameEffect	GetEffectInfo()const{return m_effect;}

};
//エフェクト管理クラス
CLASS_POINTA( CManageEffect , LPTkyManageEffect )
{
private:
	//アニメーションのマッチ情報
	CStringMatchW< CGameEffect > m_EffectMatch;
public:

	//コンストラクタ、デストラクタ
	CManageEffect();
	CManageEffect(const CManageEffect&);
	VIRTUAL ~CManageEffect();

	//追加
	BOOL AddEffectData( wstring,LPTkyTexture , UINT life );
	BOOL AddEffectData( wstring, TGameEffect* );
	//消去
	BOOL DeleteEffectData( wstring );
	//更新(実際は使用する必要がない)
	BOOL UpdateEffectData( wstring , float step );
	//再生
	BOOL DrawEffectData( wstring , Vector vec , Vector rotate , Vector expand );
	BOOL DrawEffectData2D( wstring , Vector vec );
	//設定
	BOOL RunEffectData( wstring );
	BOOL StopEffectData( wstring );
	//取得
	LPTkyGameEffect GetEffect(wstring name );

	//ループが終わっているか見る
	BOOL IsFinish( wstring );

	//取得設定
	CStringMatchW< CGameEffect >	GetEffectMatch()const{return m_EffectMatch;}
	VOID										SetEffectMatch(CStringMatchW< CGameEffect > am){ m_EffectMatch = am; }

};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//キャラクタ情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//-----------------------------
//アニメーション情報
//-----------------------------
//アニメーション構造体
STRUCT( TGameAnimation )
{
	LPTkyXFile m_model;
	FLOAT m_NowFrame;
	VOID Initialize()
	{
		m_model = NULL;
		m_NowFrame = NULL;
	}

	TGameAnimation(){Initialize();}
	TGameAnimation( LPTkyXFile model , FLOAT NowFrame )
	{
		m_model = model;
		m_NowFrame = NowFrame;
	}
};
//アニメーション行動クラス
CLASS_POINTA( CGameAnimation , LPTkyGameAnimation )
{
private:
	TGameAnimation m_Animation;
	bool m_oneLoop;
public:
	//コンストラクタ、デストラクタ、
	CGameAnimation();
	CGameAnimation( const CGameAnimation& );
	VIRTUAL ~CGameAnimation();

	BOOL CreateAnimation(TGameAnimation);
	BOOL UpdateAnimation(float step);
	BOOL DrawAnimation(Vector pos,Vector rotate , Vector expand);

	//設定取得
	VOID				SetAnimationInfo(TGameAnimation t){m_Animation = t;}	
	TGameAnimation	GetAnimationInfo()const{return m_Animation;}

	//初期化
	VOID				InitializeFrame(){m_Animation.m_NowFrame = 0;};

	bool					IsOneLoopEnd(){return m_oneLoop;}

};
//アニメーション管理クラス
//TKYMODEL_STAND　＝　立っている
CLASS_POINTA( CManageAnimation , LPTkyManageAnimation )
{
private:
	//アニメーションのマッチ情報
	CStringMatchW< CGameAnimation > m_XAnimeMatch;
public:

	//コンストラクタ、デストラクタ
	CManageAnimation();
	CManageAnimation(const CManageAnimation&);
	VIRTUAL ~CManageAnimation();

	//初期化
	BOOL InitializeAnimationData( wstring);
	//追加
	BOOL AddAnimationData( wstring,LPTkyXFile );
	//消去
	BOOL DeleteAnimationData( wstring );
	//更新
	BOOL UpdateAnimationData( wstring ,float step);
	//再生
	BOOL DrawAnimationData( wstring , Vector vec , Vector rotate , Vector expand );

	//ループが終わっているか見る
	BOOL IsFinish( wstring );

	//取得設定
	CStringMatchW<CGameAnimation >	GetAnimeMatch()const{return m_XAnimeMatch;}
	VOID										SetAnimeMatch(CStringMatchW< CGameAnimation > am){ m_XAnimeMatch = am; }

};

//-----------------------------
//キャラ情報
//-----------------------------
STRUCT( TCharaInfo )
{
	wstring m_name;	//名前

	INT m_HitPoint;	//体力
	INT m_Velocity;	//速度
	FLOAT m_Length;	//大きさ
	FLOAT m_CollisonLength;	//大きさ

	wstring m_ai;					//AI
	wstring m_automove;		//移動するやつ

	//表示するときの回転
	Vector m_rotate;
	//攻撃方向
	Vector m_AttackDirection;

	//初期角度
	Vector m_first;

	VOID Initialize()
	{
		m_name = TEXT("");
		m_Length = NULL;
		m_HitPoint = m_Velocity = NULL;
		m_CollisonLength = NULL;
		m_ai.clear();
		m_automove.clear();
		m_rotate = Vector(0,0,1);
		m_AttackDirection = Vector(0,0,1);
		m_first = Vector(0,0,1);
	}
	TCharaInfo(){Initialize();}
	TCharaInfo( wstring name,INT h , INT v, FLOAT l ,wstring ai   )
	{
		m_name = name;
		m_HitPoint = h;
		m_Velocity = v;
		m_Length = l;
		m_ai = ai;
	}
};
//メインキャラクタークラス
CLASS_POINTA( CCharactor , LPTkyCharactor ) :public PhysicsCircleModel , IGameLoadText
{
private:

//独自要素

	//コンストラクタ、デストラクタ
	CCharactor();
	CCharactor(const CCharactor&);
	VIRTUAL ~CCharactor();

	//アニメーション情報
	CManageAnimation m_AnimationData;
	//エフェクト管理
	CManageEffect m_EffectData;

	//キャラ情報
	TCharaInfo m_chara;

	//今の状態 ( CHARACTOR_STAND ) 
	string m_state;	

public:

//PhysicsCircleModel要素

	//接触情報
	VIRTUAL bool onCollisionEvent(PhysicsModel *model);

//独自要素

	//更新
	BOOL UpdateCharactor( float step );
	//描画(位置情報はPhysicsCircleModelからとる)
	VOID DrawCharactor();

	//イベント(送られてきたイベントによりエフェクトイベントでも作成する)
	VOID EventCharactor( string name, DWORD message );

	//ダメージ
	VOID Damage( float damage);

	//取得設定
	CManageAnimation GetManageAnime()const{return m_AnimationData;}
	CManageEffect GetManageEffect()const{return m_EffectData;}
	VOID SetManageAnime(CManageAnimation a){m_AnimationData = a;}
	VOID SetManageEffect(CManageEffect e){m_EffectData = e;}
	string		GetState()const{return m_state;}
	VOID		SetState(string s){m_state = s;}
	TCharaInfo	GetCharaInfo()const{return m_chara;}
	VOID			SetCharaInfo(TCharaInfo c)
	{
		m_chara = c;
		SetCircle( c.m_Length );
	}

	CManageAnimation GetAnime(){return m_AnimationData;}

	//確認
	BOOL					IsDead(){return ( m_chara.m_HitPoint <= 0 && m_state == "CHARACTOR_NULL" );}

	//依存取得
	Vector GetNowRotate()
	{
		Vector v = Vector();
		v.y = m_vel.GetRadVec().y;
		return v;
	}

//生成
	STATIC LPTkyCharactor CreateCharactor()
	{
		return new CCharactor();
	}

//IgameLoadText要素

	VIRTUAL BOOL onLoadGameDate(TGameTextInfo info);

};

//-----------------------------
//攻撃（弾）
//　ビューとして登録されあたったかどうかなど(最初に生成されず　武器によって作られる)
//-----------------------------
//攻撃情報
STRUCT( TBulletInfo ) 
{
	INT m_AttackPower;	//攻撃力
	FLOAT m_AttackSpeed;	//攻撃速度(寿命までの微笑移動距離みたいなも)
	FLOAT m_AttackRange;	//攻撃範囲
	FLOAT m_Life;				//寿命（弾の場合移動した長さになる,負だと永遠）

	VOID Initialize()
	{
		m_AttackPower  = 0;
		m_Life = m_AttackRange = m_AttackSpeed = 0;
	}
	TBulletInfo(){Initialize();}
	TBulletInfo(INT ap,FLOAT as,FLOAT ar , FLOAT l)
	{
		m_AttackPower = ap;
		m_AttackRange = ar;
		m_AttackSpeed = as;
		m_Life = l;
	}
};
//攻撃クラス
CLASS_POINTA( CBullet , LPTkyBullet ) :public PhysicsCircleModel
{
private:

//独自要素

	//コンストラクタ、デストラクタ
	CBullet();
	CBullet(const CBullet&);
	VIRTUAL ~CBullet();
	
	//攻撃情報
	TBulletInfo m_Bullet;

	//初期位置
	Vector m_OriginPos;


	//発射キャラ
	LPTkyCharactor m_BatteryChara;

	//終了フラグ
	bool m_end;

	//出発位置
	Vector m_BatteryPosition;

public:

//PhysicsCircleModel要素

	//接触情報
	VIRTUAL bool onCollisionEvent(PhysicsModel *model);

	//発射一設定
	VOID SetBatteryPosition(Vector v){m_BatteryPosition = v;}

	//終了しているか取得
	bool IsFinish();

	//取得設定
	TBulletInfo	GetBulletInfo()const{return m_Bullet;}
	VOID			SetBulletInfo(TBulletInfo bi)
	{ 
		m_Bullet = bi; 
		SetCircle( (float)m_Bullet.m_AttackRange );
	}

	LPTkyCharactor GetChara(){return m_BatteryChara;}
	VOID SetChara(LPTkyCharactor chara){m_BatteryChara = chara;}

	//生成
	STATIC LPTkyBullet CreateBullet()
	{
		return new CBullet();
	}
	STATIC LPTkyBullet CreateBullet( TBulletInfo b , LPTkyCharactor chara )
	{
		LPTkyBullet c = CreateBullet();
		c->SetBulletInfo(b);
		c->SetChara(chara);
		return c;
	}

};


//-----------------------------
//武器
//　ビューとして常駐し、攻撃いベントにより攻撃する
//	 *終了時にフレームが止まるように設計されていない
//-----------------------------
//武器情報
//　のちにタイミング以外も追加し、よりよい物となっていく
STRUCT( TWeaponInfo ) 
{

	//攻撃時弾遅れ(アニメーションが始まってから実際に発射するまでの時間)
	FLOAT m_FiringBulletsLate;		

	VOID Initialize()
	{
		m_FiringBulletsLate = NULL;
	}
	TWeaponInfo(){Initialize();}
	TWeaponInfo( FLOAT f)
	{
		m_FiringBulletsLate = f;
	}
};
//武器クラス
//
CLASS_POINTA_OBJECT( CWeapon , LPTkyWeapon ) , IGameLoadText
{
private:

//独自要素

	//コンストラクタ、デストラクタ
	CWeapon();
	CWeapon(const CWeapon&);
	VIRTUAL ~CWeapon();
	
	//攻撃情報(生成するときにクラスの返還する)
	TBulletInfo m_Bullet;
	//武器情報
	TWeaponInfo m_Weapon;

	//アニメーション情報
	CManageAnimation m_AnimationData;
	//エフェクト管理
	CManageEffect m_EffectData;

	//今所状態  (何もしていない　、　発射準備　、　発射　、　弾がまだ動いている)みたいな
	//WEAPON_SHOT , WEAPON_PRELAUNCH , WEAPON_SHOT
	string m_NowState;		

	//jikann 
	float m_time;

	//武器を使用するキャラクタの情報
	LPTkyCharactor m_chara;

	//攻撃時のマウスの位置
	Vector m_pos;

	//攻撃状態
	VOID pAttack();

public:

	//攻撃
	BOOL Attack(Vector);
	//攻撃中なら弾をどこかで生成する
	BOOL UpdateAttack(float step);
	//攻撃中なら描画
	VOID DrawAttack();
	//攻撃中が終わったイベント
	BOOL EndAttack();

	//取得設定
	TBulletInfo			GetBulletInfo()const{return m_Bullet;}
	VOID					SetBulletInfo(TBulletInfo bi){ m_Bullet = bi; }
	TWeaponInfo		GetWeaponInfo()const{return m_Weapon;}
	VOID					SetWeaponInfo(TWeaponInfo bi){ m_Weapon = bi; }
	string					GetNowState()const{return m_NowState;}
	VOID					SetNowState(string s){m_NowState = s;}
	LPTkyCharactor	GetChara()const{return m_chara;}
	VOID					SetChara(LPTkyCharactor c){m_chara=c;}


	//生成
	STATIC LPTkyWeapon CreateWeapon()
	{
		return new CWeapon();
	}
	STATIC LPTkyWeapon CreateWeapon( LPTkyCharactor chara )
	{
		LPTkyWeapon w =  new CWeapon();
		w->SetChara( chara );
		return w;
	}

//IgameLoadText要素

	VIRTUAL BOOL onLoadGameDate(TGameTextInfo info);

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//マップ情報(障害物)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//まだ完ぺき実装はしていない
CLASS_POINTA( CGameMap , LPTkyGameMap )
{
private:

	CGameMap();
	CGameMap(const CGameMap&);
	VIRTUAL ~CGameMap();

	//マップ情報Xファイル
	LPTkyXFile m_model;

public:



};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//イベント情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//-----------------------------
//テキスト情報
//		テキスト画像と、文章などを設定すると出てて来る
//-----------------------------
//立ち絵情報
STRUCT( TTextWindowCharaInfo )
{
	LPTkyTexture m_tex;
	Vector m_pos;

	VOID Initialize()
	{
		m_tex = NULL;
		m_pos = Vector();
	}
	TTextWindowCharaInfo()
	{
		Initialize();
	}
	TTextWindowCharaInfo(LPTkyTexture tex,Vector p)
	{
		m_tex = tex;
		m_pos = p;
	}
};
//移動情報は今のところ拡大のみ
STRUCT( TTextWindowInfo )
{
	LPTkyTexture		m_WindowTexture;					//ウィンドウの画像	
	Vector					m_textpos;								//ウィンドウの左上からの表示する相対距離

	//立ち絵情報
	vector<TTextWindowCharaInfo> m_BackChara;

	CVector				m_WindowPosEndStart;			//移動情報
	CVector				m_WindowRotateEndStart;		//回転情報
	CVector				m_WindowExpandEndStart;	//拡大情報
	CVector				m_WindowAlphaEndStart;		//透過情報

	TTextWindowInfo()
	{
		m_WindowTexture = NULL;
		m_textpos = Vector();
		m_WindowPosEndStart = CVector();
		m_WindowRotateEndStart = CVector();
		m_WindowExpandEndStart = CVector();
		m_WindowAlphaEndStart = CVector();
		m_BackChara.clear();
	}
	TTextWindowInfo( LPTkyTexture tex,Vector tp,CVector p,CVector r,CVector e,CVector a , vector<TTextWindowCharaInfo> c = vector<TTextWindowCharaInfo>() )
	{
		m_WindowTexture = tex;
		m_textpos = tp;
		m_WindowPosEndStart = p;
		m_WindowRotateEndStart = r;
		m_WindowExpandEndStart = e;
		m_WindowAlphaEndStart = a;
		m_BackChara = c;
	}
};
//テキスト情報
//　一文字の大きさなどは、フォントクラスから
STRUCT( TTextInfo )
{
	RECT m_Render;					//ウィンドウ内で文字が書ける範囲
	wstring		m_string;				//文字列
	wstring		m_name;				//ヘッド文字列
	FLOAT		m_ShowSpeed;	//表示スピード
	TTextInfo()
	{
		m_Render = RECT();
		m_string = TEXT("");
		m_ShowSpeed = NULL;
	}
	TTextInfo( RECT r,wstring w , wstring name ,FLOAT ss )
	{
		m_Render = r;
		m_string = w;
		m_name = name;
		m_ShowSpeed = ss;
	}
};
//情報のセット
STRUCT( TTextSet )
{
	TTextInfo		m_text;
	TTextWindowInfo *m_window;

	VOID Initialize()
	{
		m_text = TTextInfo();
		m_window = NULL;
	}
	TTextSet()	{Initialize();}
	TTextSet(TTextInfo text,TTextWindowInfo *window)
	{
		m_text = text;
		m_window = window;
	}
};


//文字表示クラス
// 文字の表示を受け持つ、
CLASS( CShowText )
{
private:
	//テキスト情報
	TTextInfo m_text;
	//今の表示されている文字列(列で分ける)
	vector<wstring> m_ShowString;
	//今の表示文字数
	FLOAT	m_NowCharacterNum;
public:
	//コンストラクタ、デストラクタ
	CShowText();
	CShowText(const CShowText&);
	VIRTUAL ~CShowText();

	//文字の情報初期化
	VOID	InitializeText();
	//更新(終了していたら真を返す)
	bool		UpdateText(float step);
	//描画
	VOID	DrawShowText(Vector pos);

	//すべて見せる
	VOID AllShow(){ WHILE(!UpdateText(1.0f)); }

	//設定取得
	TTextInfo	GetTextInfo(){return m_text;}
	VOID	SetTextInfo(TTextInfo t){m_text = t;}
};
//ウィンドウ表示クラス
enum eTKYSHOWWINODW{ TKYSHOWWINODW_NULL , TKYSHOWWINODW_ENTER , TKYSHOWWINODW_LEAVE , TKYSHOWWINODW_SHOW };
CLASS( CShowWindow )
{
private:
	//ウィンドウ情報
	TTextWindowInfo *m_window;
	//今の時間
	FLOAT m_time;
	//ウィンドウの状態
	eTKYSHOWWINODW m_state;
public:
	//コンストラクタ、デストラクタ
	CShowWindow();
	CShowWindow( const CShowWindow& );
	VIRTUAL ~CShowWindow();

	//初期化
	VOID	InitializeTextWindow( eTKYSHOWWINODW state );
	//更新(終了時に真)
	bool		UpdateTextWindow(float step);
	//描画
	VOID	DrawTextWindow();
	//テキストの位置の取得
	Vector GetTextPos();

	//設定取得
	TTextWindowInfo GeTTextWindowInfoInfo(){return *m_window;}
	VOID	 SeTTextWindowInfo(TTextWindowInfo *t){m_window = t;}
};

//文字とウィンドウの動機クラス
enum eManageTextWindow{ TKYMTW_NULL , TKYMTW_WINODW_ENTER , TKYMTW_TEXT , TKYMTW_WINODW_LEAVE};
CLASS_POINTA_OBJECT( CManageTextWindow , LpTkyManageTextWindow ) , IGameLoadText
{
private:
	//コンストラクタ、デストラクタ
	CManageTextWindow();
	CManageTextWindow(const CManageTextWindow&);
	VIRTUAL ~CManageTextWindow();

	//テキスト情報
	CShowText			m_Textinfo;
	//ウィンドウ情報
	CShowWindow		m_Winodwinfo;
	//今の状態
	eManageTextWindow m_state;

public:

	//開始
	BOOL StartTextWinodw();
	BOOL StartTextWinodw(eManageTextWindow);
	//更新(終了時に真)
	BOOL UpdateTextWinodw( float step );
	//描画
	VOID DrawTextWindow();

	//一気に終了させる
	VOID EndTextWinodw(  );


	//設定
	VOID SetTextInfo( CShowText text , CShowWindow window )
	{
		m_Textinfo = text;
		m_Winodwinfo = window;
	}
	VOID SetTextInfo( TTextInfo text , TTextWindowInfo *window )
	{
		m_Textinfo.SetTextInfo( text );
		m_Winodwinfo.SeTTextWindowInfo( window );
	}

	//取得
	eManageTextWindow GetState(){return m_state;}

	//生成
	STATIC LpTkyManageTextWindow CreateManageTextWinodw(){return new CManageTextWindow;}

	//情報のセット
	VIRTUAL BOOL onLoadGameDate(TGameTextInfo text);
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//範囲クラス
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//範囲情報構造体
STRUCT( TRange )
{
	Vector pos;
	FLOAT r;
	TRange()
	{
		pos = Vector();
		r = 0;
	}
	TRange( Vector v, FLOAT r1 )
	{
		pos = v;
		r = r1;
	}
};

//範囲情報
template < class T >
STRUCT( TElementRange ) :public TRange
{
	//テキスト情報queue	
	queue<T> m_Element;

	TElementRange()
	{
		//初期化
		WHILE(!m_Element.empty())m_Element.pop();
	}
	TElementRange( Vector v, FLOAT r1 , queue<T> t )
	{
		pos = v;
		r = r1;
		m_Element = t;
	}
};
//範囲管理クラス
template < class T >
CLASS( CElementRange )
{
private:
	//イベント情報
	list<TElementRange<T>> m_Element;
public:
	//コンストラクタ、デストラクタ
	CElementRange()
	{
		m_Element.clear();
	}
	CElementRange(const CElementRange& t)
	{
		m_Element = t.m_Element;
	}
	VIRTUAL ~CElementRange()
	{
		m_Element.clear();
	}

	//範囲情報の追加
	VOID AddRange(TElementRange<T> t)
	{
		m_Element.push_back(t);
	}

	//判定
	TElementRange<T> Decision(Vector p)
	{
		list<TElementRange<T>>::iterator im = m_Element.begin();
		WHILE( im != m_Element.end() )
		{
			float l = ( Vector( im->pos.x , 0 , im->pos.z ) - Vector( p.x , 0 , p.z ) ).GetLarge();
			//範囲内だったら
			IF( l < im->r )
			{
				return *im;
			}
			im++;
		}
		return TElementRange<T>();
	}

	//範囲の消去
	VOID DeleteRange( Vector p )
	{
		list<TElementRange<T>>::iterator im = m_Element.begin();
		WHILE( im != m_Element.end() )
		{
			float l = ( Vector( im->pos.x , 0 , im->pos.z ) - Vector( p.x , 0 , p.z ) ).GetLarge();
			//範囲内だったら
			IF( l < im->r )
			{
				im = m_Element.erase( im );
				return;
			}
			im++;
		}
	}

	//取得
	list<TElementRange<T>> GetElement(){ return m_Element;}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//自動移動クラス
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//キー情報
enum eTkyAutoKey{ TKYKEY_UP = 0x01 , TKYKEY_DOWN = 0x02 , TKYKEY_LEFT = 0x04 , TKYKEY_RIGHT = 0x08 };
STRUCT( TAutoKeyInput )
{
	//キー情報
	UINT m_eTkyAutoKey;
	//フレーム数
	DWORD m_FrameNum;
	//
	TAutoKeyInput()
	{
		m_eTkyAutoKey = NULL;
		m_FrameNum = NULL;
	}
	TAutoKeyInput(UINT e,DWORD f)
	{
		m_eTkyAutoKey = e;
		m_FrameNum = f;
	}
};
//マウス情報
STRUCT( TAutoMouseInput )
{
	//キー情報
	Vector m_MousePos;
	//フレーム数
	DWORD m_FrameNum;
	//
	TAutoMouseInput()
	{
		m_MousePos = Vector();
		m_FrameNum = NULL;
	}
	TAutoMouseInput(Vector cv,DWORD f)
	{
		m_MousePos = cv;
		m_FrameNum = f;
	}
};
//自動移動制御クラス
CLASS_POINTA_OBJECT( CAutoKeyInput , LPTkyAutoKeyInput )
{
private:
	CAutoKeyInput();
	CAutoKeyInput(const CAutoKeyInput&);
	VIRTUAL ~CAutoKeyInput();

	//キー情報
	queue<TAutoKeyInput> m_KeyInfo;
	//フレーム情報
	DWORD m_NowFrame;

public:

	//キー情報の追加
	VOID AddKeyInput( TAutoKeyInput key );
	//今のキー情報の取得
	TAutoKeyInput GetNowKeyInput();
	//フレームを進める(もし進まなかったらFALSEを返す)
	BOOL NextFrame();

	//生成
	STATIC LPTkyAutoKeyInput CreateAutoKeyInput()
	{
		return new CAutoKeyInput();
	}

	//取得
	queue<TAutoKeyInput> GetKeyInfo(){return m_KeyInfo;}

	//今のフレーム情報
	DWORD GetNowFrame(){return m_NowFrame;}

};
//自動視点制御クラス
CLASS_POINTA_OBJECT( CAutoMouseInput , LPTkyAutoMouseInput )
{
private:
	CAutoMouseInput();
	CAutoMouseInput(const CAutoMouseInput&);
	VIRTUAL ~CAutoMouseInput();

	//キー情報
	queue<TAutoMouseInput> m_MouseInfo;
	//フレーム情報
	DWORD m_NowFrame;

public:

	//マウス情報の追加
	VOID AddMouseInput( TAutoMouseInput Mouse );
	//マウス情報の追加
	VOID AddMouseInput( Vector , DWORD );
	//今のマウスの位置取得
	Vector GetNowMouseInput();
	//フレームを進める(もし進まなかったらFALSEを返す)
	BOOL NextFrame();

	//生成
	STATIC LPTkyAutoMouseInput CreateAutoMouseInput()
	{
		return new CAutoMouseInput();
	}

	//取得
	queue<TAutoMouseInput> getNouseInof(){return m_MouseInfo;}

	//今のフレーム情報
	DWORD GetNowFrame(){return m_NowFrame;}

};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//タイトルクラス
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TTitle )
{
	LPTkyTexture m_logo;
	LPTkyTexture m_back;
	LPTkyTexture m_start;
	LPTkyTexture m_score;
	LPTkyTexture m_ranking;
	TTitle()
	{
		m_logo = NULL;
		m_back= NULL;
		m_start= NULL;
		m_score= NULL;
		m_ranking = NULL;
	}
};
CLASS_POINTA_OBJECT( CTitle , LPTkyTitle )
{
private:

	TTitle m_title;

	//TITLE_LOGO,TITLE_BACK,TITLE_END,TITLE_SCORE
	wstring m_state;

	Vector pos;

	DWORD m_Score[3];

	CTitle();
	CTitle(const CTitle&);
	VIRTUAL ~CTitle();
public:

	VOID UpdateTitle(float step);
	VOID DrawTitle();

	VOID Next();
	BOOL IsEnd();

	TTitle GetTitle(){return m_title;}
	VOID SetTitle(TTitle t){m_title  = t;}

	VOID SetPos(Vector t){pos = t;}

	wstring GetState(){return m_state;}

	STATIC LPTkyTitle CreateTitle( TTitle t )
	{
		LPTkyTitle ti = new CTitle();
		ti->SetTitle(t);
		return ti;
	}
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//スコアクラス
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eScore{ TKYSCORE_START,TKYSCORE_PLAY,TKYSCORE_RESULT,TKYSCORE_END,TKYSCORE_GAMEOVER,TKYSCORE_RESET };
STRUCT( TScore )
{
	DWORD m_score;				//スコア
	DWORD m_BatteryNum;		//発射数
	DWORD m_HitNum;			//ヒット数

	DWORD m_time;					//時間
	DWORD m_starttime;					//開始時間

	LPTkyCharactor m_chara;	//キャラ情報
	
	//結果画面の背景
	LPTkyTexture	m_ScoreBack;

	//シューティング時のスコア
	LPTkyTexture m_ShootingScore;

	//結果画面の背景
	LPTkyTexture	m_Gameover;

	//今の状態
	eScore m_state;

	VOID initialize()
	{
		m_score = NULL;
		m_BatteryNum = NULL;
		m_HitNum = NULL;
		m_time = NULL;
		m_chara = NULL;
		m_state = TKYSCORE_START;
		m_starttime = NULL;
		m_Gameover = NULL;
	}
};

CLASS_POINTA_OBJECT( CScore , LPTkyScore )
{
private:

	TScore m_score;

	CScore();
	CScore(const CScore&);
	VIRTUAL ~CScore();
public:

	VOID UpdateScore(float step);
	VOID DrawScore();

	TScore GetScore(){return m_score;}
	VOID SetScore(TScore t){m_score = t;}

	STATIC LPTkyScore CreateScore(TScore t)
	{
		LPTkyScore s = new CScore();
		s->SetScore(t);
		return s;
	}

};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//シェイドクラス
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum eShade{ TKYSHADE_OUT , TKYSHADE_MIDDLE , TKYSHADE_IN , TKYSHADE_NULL };
CLASS( CShade )
{
private:
	LPTkyTexture m_shade;
	eShade m_State;
	float m_alpha;

	//じっこうもとID
	DWORD m_run_id;

public:

	CShade();
	CShade(const CShade&);
	VIRTUAL ~CShade();

	VOID Read(wstring name);

	VOID UpdateShade(float step);
	VOID DrawShade();

	//アップデートしてもいいか
	bool IsUpdate();

	//はじめる
	VOID Run(DWORD id);

};