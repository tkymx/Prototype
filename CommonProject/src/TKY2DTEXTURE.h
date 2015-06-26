#pragma once

#include<d3dx9.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include "TKYXFILE.h"
#include "TKYHLSL.h"
#include "TKY3D.h"
#include "TKYLIBRARY.h"

using namespace std;

//===========================================================
//新テクスチャクラス
// 読み込み、表示、アニメーションで表示、アニメーション時の更新、重複用テクスチャマップ
//===========================================================

STRUCT( TTexutureInfo )  
{
public:
	//画像情報
	LPDIRECT3DTEXTURE9		m_pTextures;			//テクスチャを格納する
	//動的情報
	UINT m_width,m_height;								//画像の大きさ
	DWORD							m_AnimeSpeed;			//アニメの遷移時間(ms)
	BYTE								m_NowFrame;			//今のアニメのフレーム
	BYTE								m_MaxFrame;				//フレームの数
	BYTE								m_HorizontalDivision;	//横分割数
	BYTE								m_VerticalDivision;		//縦分割数
	//テンポラリ情報
	DWORD							m_NowTime;				//今の時間

	//初期化
	VOID Initialize()
	{
		m_pTextures = NULL;			//テクスチャを格納する
		//動的情報
		m_width = NULL;
		m_height = NULL;								//画像の大きさ
		m_AnimeSpeed = NULL;			//アニメの遷移時間(ms)
		m_MaxFrame = NULL;;				//フレームの数
		m_HorizontalDivision = NULL;;	//横分割数
		m_VerticalDivision = NULL;;		//縦分割数
		//テンポラリ情報
		m_NowFrame = NULL;			//今のアニメのフレーム
		m_NowTime = NULL;				//今の時間
	}

};

//表示する状態
enum TKYTEXTURE_TYPE{ TKYTEXTURE_TYPE_3D_MODE , TKYTEXTURE_TYPE_2D_MODE };
//テクスチャクラス
CLASS_POINTA_OBJECT( CTexture , LPTkyTexture )
{
private:
	//コンストラクタ、デストラクタ、コピーコンストラクタ
	CTexture();
	CTexture(const CTexture&);
	VIRTUAL ~CTexture();

	//基本情報
	LPTKY3D m_device;

	//テクスチャ情報
	TTexutureInfo m_texture;

	//アルファ合成のための
	STRUCT( TWaitTexture )
	{
		LPTkyTexture m_tex;
		Vector pos;
		Vector rotate;
		Vector expand; 
		TKYTEXTURE_TYPE type; 
		BYTE Index;
	};
	STATIC queue<TWaitTexture> m_DrawWaitTexture;

public:

	//読み込み
	HRESULT SetTexture( string name ,INT width,INT height);
	HRESULT SetTextureW( wstring name ,INT width,INT height);
	HRESULT SetTexture( string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//ファイル名、総画像数、横画像数、縦画像数,画像の横幅、画像の縦幅,アニメーションスピード
	HRESULT SetTextureW( wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//ファイル名、総画像数、横画像数、縦画像数,画像の横幅、画像の縦幅,アニメーションスピード
	
	//描画　:　位置、角度、ループする時間　(wait ウェイトに送って後で描画する) :　一周したかどうか
	BOOL DrawTextureIndex(Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , BYTE Index , bool wait = true); 
	BOOL DrawTexture(Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , bool wait = true ); 
	BOOL DrawTextureIndexAlpha(float alpha,Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , BYTE Index , bool wait = true); 
	BOOL DrawTextureAlpha(float alpha,Vector pos,Vector rotate,Vector expand, TKYTEXTURE_TYPE type , bool wait = true ); 

	//更新
	BOOL Update(float step);
	BOOL UpdateIndex(float step,DWORD *time,BYTE *index);

	//プロパティ
	LPTKY3D		GetDevice()const{return m_device;}
	VOID				SetDevice(LPTKY3D d){m_device = d;}
	TTexutureInfo	GetTextureInfo()const{return m_texture;}
	VOID				SetTextureInfo(TTexutureInfo t){m_texture = t;}

	//生成(できなかったらNULLを返す)
	STATIC LPTkyTexture CreateTexture( LPTKY3D device , string name ,INT width,INT height);
	STATIC LPTkyTexture CreateTexture( LPTKY3D device , string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//ファイル名、総画像数、横画像数、縦画像数,画像の横幅、画像の縦幅,アニメーションスピード;
	STATIC LPTkyTexture CreateTextureW( LPTKY3D device , wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//ファイル名、総画像数、横画像数、縦画像数,画像の横幅、画像の縦幅,アニメーションスピード;

	//描画
	STATIC VOID DrawWaitTexture()
	{
		//すべて描画
		WHILE( !m_DrawWaitTexture.empty() )
		{
			TWaitTexture t = m_DrawWaitTexture.front();
			t.m_tex->DrawTextureIndex(t.pos,t.rotate,t.expand,t.type,t.Index,false);
			m_DrawWaitTexture.pop();
		}
	}

};

//===========================================================
//終わり
//===========================================================

//-------------------------------------------------------------
// デバッグ用に表示するテクスチャ用の構造体
//-------------------------------------------------------------
typedef struct {
    FLOAT       p[4];
	D3DCOLOR	diffuse;
    FLOAT       tu, tv;
} TVERTEX;

//-------------------------------------------------------------
// アニメーションの構造体
//-------------------------------------------------------------
enum AnimeAction{ MOVE_ANIME , MAGNIFY_ANIME , ROTATE_ANIME , ALPHA_ANIME };
typedef struct
{
	//静的情報
	INT				m_type;		//アニメーションフラグ
	Vector			m_value;	//値
	DWORD			m_loopnum;   //移動回数
	//動的情報
	DWORD			m_nowloop;	//今の回数
	BOOL				m_endflag;//終了フラグ
}ANIMATION;

//--------------------------------------------------//
//2D描画の作成
//--------------------------------------------------//
class D3DTKY2DTEXTURE
{
private:
	LPDIRECT3D9					m_pD3D;				//DirectX9　インターフェイス
	LPDIRECT3DDEVICE9		m_pD3DDevice;		//DirectX9　インターフェイスのデバイス
	HLSLShader					*m_pHLSL;			//HLSL情報
	LPDIRECT3DTEXTURE9		m_pTextures;			//テクスチャを格納する
	UINT m_width,m_height;								//画像の大きさ
	//動的情報
	DWORD							m_AnimeSpeed;			//アニメの遷移時間(ms)
	BYTE								m_NowFrame;			//今のアニメのフレーム
	BYTE								m_MaxFrame;				//フレームの数
	BYTE								m_HorizontalDivision;	//横分割数
	BYTE								m_VerticalDivision;		//縦分割数
	//テンポラリ情報
	DWORD							m_NowTime;				//今の時間

	//システム情報
	static vector<D3DTKY2DTEXTURE*> m_vTex;

	D3DTKY2DTEXTURE(LPTKY3D,LPTKYHLSL);
	VIRTUAL ~D3DTKY2DTEXTURE();

public:
	D3DTKY2DTEXTURE(const D3DTKY2DTEXTURE&);
	HRESULT ReadTexture(LPCTSTR filename,INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed);		//ファイル名、総画像数、横画像数、縦画像数,画像の横幅、画像の縦幅,アニメーションスピード
	BOOL DrawTexture(float x,float y,INT loop);
	BOOL DrawTexture(float x,float y,float alpha,INT loop);
	BOOL DrawTexture3D(float x,float y,float z,INT loop);
	BOOL DrawTextureHLSL(float x,float y,float z,INT loop,LPTKYHLSL);
	BOOL DrawTextureHLSL(float x,float y,float z,float falpha,INT loop,LPTKYHLSL);
	BOOL Update();
	VOID Initialize(){m_NowFrame=0;};
	//サイズを返す
	Vector GetSize(){return Vector( (float)m_width , (float)m_height , 0);}
	//直接セット
	VOID SetTexture(LPDIRECT3DTEXTURE9 tex,INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{	
		SAFE_RELEASE(m_pTextures);
		m_pTextures = tex;	
		D3DTKY2DTEXTURE::m_width = width;    // 幅（ピクセル）
		D3DTKY2DTEXTURE::m_height = height;   // 高さ（ピクセル）
		D3DTKY2DTEXTURE::m_MaxFrame = num;
		D3DTKY2DTEXTURE::m_NowFrame = 0;
		D3DTKY2DTEXTURE::m_NowTime = timeGetTime();
		D3DTKY2DTEXTURE::m_AnimeSpeed = speed;
		D3DTKY2DTEXTURE::m_HorizontalDivision = Horizontal;		//横分割数
		D3DTKY2DTEXTURE::m_VerticalDivision = Vertical;		//縦分割数
	};
	//今の消去
	VOID Release(){D3DTKY2DTEXTURE::~D3DTKY2DTEXTURE();};

	//テクスチャの取得
	static D3DTKY2DTEXTURE *CreateTexture(LPTKY3D d,LPTKYHLSL h){D3DTKY2DTEXTURE *i = new D3DTKY2DTEXTURE(d,h);m_vTex.push_back(i); return i; }
	static VOID Destroy(){FOR_I(m_vTex.size())SAFE_DELETE(m_vTex[i]);}

};
typedef D3DTKY2DTEXTURE* LPTKYTEXTURE;

//--------------------------------------------------//
//2D描画アニメーションの作成
//実行はグローバルとローカルからなる
//--------------------------------------------------//
class D3DTKY2DTEXTUREANIMATION
{
private:
	LPTKYTEXTURE					   m_texture;		//メインの画像
	vector<vector<ANIMATION>>	m_Element;		//行動の要素
	INT										m_nowindex;		//今の行動

	Vector									m_pos;				//今の描画位置

	//システム情報
	static vector<D3DTKY2DTEXTUREANIMATION*> m_vTexAnime;

	D3DTKY2DTEXTUREANIMATION();
	D3DTKY2DTEXTUREANIMATION(LPTKYTEXTURE);
	VIRTUAL ~D3DTKY2DTEXTUREANIMATION();

public:

	D3DTKY2DTEXTUREANIMATION(const D3DTKY2DTEXTUREANIMATION&);

	void	AddElement(vector<ANIMATION>);	//要素の追加 引数　アニメーションデータ,入れ方　０：入れた後次に進む　、　１：入れた後次に進まない
	bool	Update();								//更新
	void	Darw();									//描画する
	void	Darw(HLSLShader *hlsl);									//描画する

	//取得
	LPTKYTEXTURE GetTexture(){return m_texture;}

	//テクスチャの取得
	static D3DTKY2DTEXTUREANIMATION *CreateTextureAnime(){D3DTKY2DTEXTUREANIMATION *i = new D3DTKY2DTEXTUREANIMATION();m_vTexAnime.push_back(i); return i; }
	static D3DTKY2DTEXTUREANIMATION *CreateTextureAnime(LPTKYTEXTURE t){D3DTKY2DTEXTUREANIMATION *i = new D3DTKY2DTEXTUREANIMATION(t);m_vTexAnime.push_back(i); return i; }
	static VOID Destroy(){FOR_I(m_vTexAnime.size())SAFE_DELETE(m_vTexAnime[i]);}

};
typedef D3DTKY2DTEXTUREANIMATION* LPTKYTEXTUREANIME;

//--------------------------------------------------//
//2D描画アニメーションのエフェクト実用クラス
//--------------------------------------------------//
typedef struct 
{
	LPTKYTEXTURE m_anime;
	Vector m_pos;
}t_effect;
class CTKYEFFECT
{
private:

	//値保持
	vector<t_effect> m_effect;

	//システム
	static vector<CTKYEFFECT*> m_vIndex;

	CTKYEFFECT();
	VIRTUAL ~CTKYEFFECT();

public:

	VOID SetEffect(LPTKYTEXTURE,Vector);
	VOID SetEffect(LPTKYTEXTUREANIME,Vector);

	VOID Update();
	VOID Draw();

	static CTKYEFFECT* CreateEffect();
	static VOID Destroy();
};
typedef CTKYEFFECT* LPTKYEFFECT;

