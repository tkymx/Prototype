
#include "TKYFONT.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		フォント
//
//////////////////////////////////////////////////////////////////////////////////////////
vector<CFONT*> CFONT::m_vfont;
LPTKYFONT CFONT::m_single = NULL;
queue<CFONT::TWaitFont> CFONT::m_WaitFont;
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
CFONT::CFONT(LPTKY3D init)
{
	assert(init);
	CFONT::g_pD3D  =		init->GetD3D();					//DirectX9　インターフェイス
	CFONT::g_pD3DDevice =	init->GetD3DDevice();			//DirectX9　インターフェイスのデバイス

	CFONT::g_pFont = NULL;

	D3DXFONT_DESC logFont;
	logFont.Height						= 30;        //フォントの文字セルまたは文字の高さ							
	logFont.Width						= 15;        //フォントの文字の幅 (論理単位)							
	logFont.Weight					= FW_NORMAL; //フォントのウェイト (0 ～ 1,000 の範囲)
	logFont.MipLevels				= 0;         //要求されるミップ レベルの数
	logFont.Italic						= FALSE;     //斜体フォントの場合は TRUE に設定されます
	logFont.CharSet					= SHIFTJIS_CHARSET;         //文字セット
	logFont.OutputPrecision		= OUT_TT_ONLY_PRECIS;         //出力精度
	logFont.Quality					= PROOF_QUALITY;//出力品質
	logFont.PitchAndFamily		= FIXED_PITCH | FF_MODERN;//フォントのピッチとファミリ
	wprintf(logFont.FaceName, TEXT("MS P ゴシック"));       //フォントの書体名を指定する NULL 終端文字列					

	m_state = D3DXCreateFontIndirect(g_pD3DDevice, &logFont, &(CFONT::g_pFont));

	rr=0;
	gg=0;
	bb=0;

}
//---------------------------------------------//
//デストラクタ
//---------------------------------------------//
CFONT::~CFONT()
{
	SAFE_RELEASE(this->g_pFont);
}
//---------------------------------------------//
//ウェイトへデータを送る
//---------------------------------------------//
VOID	CFONT::pWaitIn(int x, int y, LPCTSTR str)
{
		TWaitFont f;
		f.str = str;
		f.pos = Vector( (float)x , (float)y ,0 );
		f.color = Vector( (float)rr , (float)gg , (float)bb );

		m_WaitFont.push(f);
}
//---------------------------------------------//
//フォントの描画
//---------------------------------------------//
VOID	CFONT::DrawFont(int x, int y, LPCTSTR str,bool wait)
{
	IF(wait)
	{
		pWaitIn(x,y,str);
		return;
	}

	RECT rect = {x,y,100,100};
	g_pFont->DrawText(NULL,							//Spriteのポインタ NULLでもOK
                      str,							//描画する文字列のポインタを指定します。
                      -1,                            //文字数をします。-1の場合は全てを描画します。
                      &rect,                         //描画領域
                      DT_LEFT | DT_NOCLIP,           //ここで左揃えなどを設定します。
                      D3DCOLOR_XRGB(CFONT::rr, CFONT::gg, CFONT::bb)); //文字の色を決めます。

}
VOID	CFONT::DrawFontA(int x, int y, LPCSTR str,bool wait)
{

	RECT rect = {x,y,100,100};
	WCHAR wstr[MAX_PATH];
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,str,strlen(str)+1,wstr,MAX_PATH);

	IF(wait)
	{
		pWaitIn(x,y,wstr);
		return;
	}

	g_pFont->DrawTextW(NULL,							//Spriteのポインタ NULLでもOK
                      wstr,							//描画する文字列のポインタを指定します。
                      -1,                            //文字数をします。-1の場合は全てを描画します。
                      &rect,                         //描画領域
                      DT_LEFT | DT_NOCLIP,           //ここで左揃えなどを設定します。
                      D3DCOLOR_XRGB(CFONT::rr, CFONT::gg, CFONT::bb)); //文字の色を決めます。

}
VOID	CFONT::DrawFontW(int x, int y, LPCTSTR str,bool wait)
{

	IF(wait)
	{
		pWaitIn(x,y,str);
		return;
	}

	RECT rect = {x,y,100,100};
	g_pFont->DrawText(NULL,							//Spriteのポインタ NULLでもOK
                      str,							//描画する文字列のポインタを指定します。
                      -1,                            //文字数をします。-1の場合は全てを描画します。
                      &rect,                         //描画領域
                      DT_LEFT | DT_NOCLIP,           //ここで左揃えなどを設定します。
                      D3DCOLOR_XRGB(CFONT::rr, CFONT::gg, CFONT::bb)); //文字の色を決めます。

}
//---------------------------------------------//
//フォントの色をきめる
//---------------------------------------------//
VOID	CFONT::SetColor(int r,int g,int b)
{

	CFONT::rr=r;
	CFONT::gg=g;
	CFONT::bb=b;

}
//---------------------------------------------//
//フォントのサイズを取得
//---------------------------------------------//
Vector CFONT::GetFontSize()
{
	D3DXFONT_DESC logFont;
	this->g_pFont->GetDesc(&logFont);
	return Vector( (float)logFont.Width , (float)logFont.Height , 0 );
}

//---------------------------------------------//
//フォントのをきめる
//---------------------------------------------//
void CFONT::SetFontInfo(int w,int h,int s,char *name)
{
	D3DXFONT_DESC logFont;
	this->g_pFont->GetDesc(&logFont);

	IF(w>=0)logFont.Width = w;
	IF(h>=0)logFont.Height = h;
	IF(s>=0)logFont.Weight = s;
	IF(name!=NULL)
	{
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,name,32,logFont.FaceName,32);
	}
	D3DXCreateFontIndirect(g_pD3DDevice, &logFont, &(CFONT::g_pFont));
}
//---------------------------------------------//
//フォントのをきめるW
//---------------------------------------------//
void CFONT::SetFontInfoW(int w,int h,int s,wchar_t *name)
{
	D3DXFONT_DESC logFont;
	this->g_pFont->GetDesc(&logFont);

	IF(w>=0)logFont.Width = w;
	IF(h>=0)logFont.Height = h;
	IF(s>=0)logFont.Weight = s;
	IF(name!=NULL)
	{
		wcscpy_s( logFont.FaceName , 32 , name );
	}
	D3DXCreateFontIndirect(g_pD3DDevice, &logFont, &(CFONT::g_pFont));
}

