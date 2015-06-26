#pragma once

#include<d3dx9.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include "TKY3D.h"
#include "TKYLIBRARY.h"

//--------------------------------------------------//
//フォントの作成
//--------------------------------------------------//
CLASS_POINTA_OBJECT( CFONT , LPTKYFONT )
{

private:

	LPDIRECT3D9				g_pD3D;					//DirectX9　インターフェイス
	LPDIRECT3DDEVICE9		g_pD3DDevice;			//DirectX9　インターフェイスのデバイス
	LPD3DXFONT  g_pFont;     //D3DXFont

	int rr,gg,bb;

	//取得状態
	HRESULT m_state;

	static vector<CFONT*> m_vfont;

	static LPTKYFONT m_single;

	CFONT(LPTKY3D init);
	VIRTUAL ~CFONT();

	STRUCT( TWaitFont )
	{
		wstring str;
		Vector pos;
		Vector color;
	};
	static queue<TWaitFont> m_WaitFont;

	VOID pWaitIn(int x, int y, LPCTSTR str);

public:

	void DrawFont(int x,int y,LPCTSTR str,bool wait = true);
	void DrawFontA(int x,int y,LPCSTR str,bool wait = true);
	void DrawFontW(int x,int y,LPCTSTR str,bool wait = true);
	void SetColor(int r,int g,int b);
	void SetFontInfo(int w,int h,int s,char *name);
	void SetFontInfoW(int w,int h,int s,wchar_t *name);

	Vector GetFontSize();

	static CFONT *CreateFont(LPTKY3D t)
	{
		IF(t==NULL)return m_single;
		IF(m_single==NULL)m_single = new CFONT(t); 
		return m_single;
	}

	//取得
	HRESULT GetState(){return m_state;}

	//すべて描画
	static VOID DrawWaitFont()
	{
		//すべて描画
		WHILE( !m_WaitFont.empty() )
		{
			TWaitFont t = m_WaitFont.front();
			m_single->SetColor( (int)t.color.x , (int)t.color.y , (int)t.color.z );
			m_single->DrawFontW( (int)t.pos.x , (int)t.pos.y , t.str.c_str() ,  false);
			m_WaitFont.pop();
		}
	}

};
typedef  CFONT* LPTKYFONT;
