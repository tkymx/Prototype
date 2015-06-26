
#include "TKYFONT.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		�t�H���g
//
//////////////////////////////////////////////////////////////////////////////////////////
vector<CFONT*> CFONT::m_vfont;
LPTKYFONT CFONT::m_single = NULL;
queue<CFONT::TWaitFont> CFONT::m_WaitFont;
//---------------------------------------------//
//�R���g���X�^
//---------------------------------------------//
CFONT::CFONT(LPTKY3D init)
{
	assert(init);
	CFONT::g_pD3D  =		init->GetD3D();					//DirectX9�@�C���^�[�t�F�C�X
	CFONT::g_pD3DDevice =	init->GetD3DDevice();			//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X

	CFONT::g_pFont = NULL;

	D3DXFONT_DESC logFont;
	logFont.Height						= 30;        //�t�H���g�̕����Z���܂��͕����̍���							
	logFont.Width						= 15;        //�t�H���g�̕����̕� (�_���P��)							
	logFont.Weight					= FW_NORMAL; //�t�H���g�̃E�F�C�g (0 �` 1,000 �͈̔�)
	logFont.MipLevels				= 0;         //�v�������~�b�v ���x���̐�
	logFont.Italic						= FALSE;     //�Α̃t�H���g�̏ꍇ�� TRUE �ɐݒ肳��܂�
	logFont.CharSet					= SHIFTJIS_CHARSET;         //�����Z�b�g
	logFont.OutputPrecision		= OUT_TT_ONLY_PRECIS;         //�o�͐��x
	logFont.Quality					= PROOF_QUALITY;//�o�͕i��
	logFont.PitchAndFamily		= FIXED_PITCH | FF_MODERN;//�t�H���g�̃s�b�`�ƃt�@�~��
	wprintf(logFont.FaceName, TEXT("MS P �S�V�b�N"));       //�t�H���g�̏��̖����w�肷�� NULL �I�[������					

	m_state = D3DXCreateFontIndirect(g_pD3DDevice, &logFont, &(CFONT::g_pFont));

	rr=0;
	gg=0;
	bb=0;

}
//---------------------------------------------//
//�f�X�g���N�^
//---------------------------------------------//
CFONT::~CFONT()
{
	SAFE_RELEASE(this->g_pFont);
}
//---------------------------------------------//
//�E�F�C�g�փf�[�^�𑗂�
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
//�t�H���g�̕`��
//---------------------------------------------//
VOID	CFONT::DrawFont(int x, int y, LPCTSTR str,bool wait)
{
	IF(wait)
	{
		pWaitIn(x,y,str);
		return;
	}

	RECT rect = {x,y,100,100};
	g_pFont->DrawText(NULL,							//Sprite�̃|�C���^ NULL�ł�OK
                      str,							//�`�悷�镶����̃|�C���^���w�肵�܂��B
                      -1,                            //�����������܂��B-1�̏ꍇ�͑S�Ă�`�悵�܂��B
                      &rect,                         //�`��̈�
                      DT_LEFT | DT_NOCLIP,           //�����ō������Ȃǂ�ݒ肵�܂��B
                      D3DCOLOR_XRGB(CFONT::rr, CFONT::gg, CFONT::bb)); //�����̐F�����߂܂��B

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

	g_pFont->DrawTextW(NULL,							//Sprite�̃|�C���^ NULL�ł�OK
                      wstr,							//�`�悷�镶����̃|�C���^���w�肵�܂��B
                      -1,                            //�����������܂��B-1�̏ꍇ�͑S�Ă�`�悵�܂��B
                      &rect,                         //�`��̈�
                      DT_LEFT | DT_NOCLIP,           //�����ō������Ȃǂ�ݒ肵�܂��B
                      D3DCOLOR_XRGB(CFONT::rr, CFONT::gg, CFONT::bb)); //�����̐F�����߂܂��B

}
VOID	CFONT::DrawFontW(int x, int y, LPCTSTR str,bool wait)
{

	IF(wait)
	{
		pWaitIn(x,y,str);
		return;
	}

	RECT rect = {x,y,100,100};
	g_pFont->DrawText(NULL,							//Sprite�̃|�C���^ NULL�ł�OK
                      str,							//�`�悷�镶����̃|�C���^���w�肵�܂��B
                      -1,                            //�����������܂��B-1�̏ꍇ�͑S�Ă�`�悵�܂��B
                      &rect,                         //�`��̈�
                      DT_LEFT | DT_NOCLIP,           //�����ō������Ȃǂ�ݒ肵�܂��B
                      D3DCOLOR_XRGB(CFONT::rr, CFONT::gg, CFONT::bb)); //�����̐F�����߂܂��B

}
//---------------------------------------------//
//�t�H���g�̐F�����߂�
//---------------------------------------------//
VOID	CFONT::SetColor(int r,int g,int b)
{

	CFONT::rr=r;
	CFONT::gg=g;
	CFONT::bb=b;

}
//---------------------------------------------//
//�t�H���g�̃T�C�Y���擾
//---------------------------------------------//
Vector CFONT::GetFontSize()
{
	D3DXFONT_DESC logFont;
	this->g_pFont->GetDesc(&logFont);
	return Vector( (float)logFont.Width , (float)logFont.Height , 0 );
}

//---------------------------------------------//
//�t�H���g�̂����߂�
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
//�t�H���g�̂����߂�W
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

