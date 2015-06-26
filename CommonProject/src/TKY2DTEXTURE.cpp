
#include "TKY2DTEXTURE.h"

//===========================================================
//新テクスチャクラス
// 読み込み、表示、アニメーションで表示、アニメーション時の更新、重複用テクスチャマップ
//===========================================================

	queue<CTexture::TWaitTexture> CTexture::m_DrawWaitTexture;
	//コンストラクタ、デストラクタ、コピーコンストラクタ
	CTexture::CTexture()
	{
		//基本情報
		m_device = Init3D::CreateInit3D();
		//テクスチャ情報
		m_texture.Initialize();

	}
	CTexture::CTexture(const CTexture& tex )
	{
		//テクスチャ情報
		m_device = tex.GetDevice();
		m_texture = tex.GetTextureInfo();
	}
	CTexture::~CTexture()
	{
		//テクスチャ情報
		m_texture.Initialize();
	}
	//読み込み
	HRESULT CTexture::SetTexture( string name ,INT width,INT height )
	{
		HRESULT r;
		if(FAILED(r =	D3DXCreateTextureFromFileExA(
			m_device->GetD3DDevice(),
			name.c_str(),              // ファイル名
			0,
			0,
			0,
			0,
			D3DFMT_A8B8G8R8,         
			D3DPOOL_MANAGED,
			D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR,
			D3DCOLOR_ARGB(0, 0, 0, 0),  // 黒色を透過色とする
			NULL,
			NULL,
			&(m_texture.m_pTextures))						//読み込んだテクスチャを返すポインタ
			))
		{
			m_texture.m_pTextures = NULL;
			MessageBoxA(NULL,name.c_str(),"画像読み込めません	",MB_OK);
			return r;
		}

		//画像サイズを取得
		m_texture.m_width = width;    // 幅（ピクセル）
		m_texture.m_height = height;   // 高さ（ピクセル）
		m_texture.m_MaxFrame = 1;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = 1;
		m_texture.m_HorizontalDivision = 1;		//横分割数
		m_texture.m_VerticalDivision = 1;		//縦分割数

		return r;
	}
	HRESULT CTexture::SetTextureW( wstring name ,INT width,INT height )
	{
		HRESULT r;
		if(FAILED(r =	D3DXCreateTextureFromFileExW(
			m_device->GetD3DDevice(),
			name.c_str(),              // ファイル名
			0,
			0,
			0,
			0,
			D3DFMT_A8B8G8R8,         
			D3DPOOL_MANAGED,
			D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR,
			D3DCOLOR_ARGB(0, 0, 0, 0),  // 黒色を透過色とする
			NULL,
			NULL,
			&(m_texture.m_pTextures))						//読み込んだテクスチャを返すポインタ
			))
		{
			m_texture.m_pTextures = NULL;
			MessageBoxW(NULL,name.c_str(),TEXT("画像読み込めません"),MB_OK);
			return r;
		}

		//画像サイズを取得
		m_texture.m_width = width;    // 幅（ピクセル）
		m_texture.m_height = height;   // 高さ（ピクセル）
		m_texture.m_MaxFrame = 1;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = 1;
		m_texture.m_HorizontalDivision = 1;		//横分割数
		m_texture.m_VerticalDivision = 1;		//縦分割数

		return r;
	}
	//ファイル名、総画像数、横画像数、縦画像数,画像の横幅、画像の縦幅,アニメーションスピード
	HRESULT CTexture::SetTexture( string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		HRESULT r;
		//アニメーションのセット
		IF_FAILED(r = SetTexture( name ,width,height ))return r;
		//各アニメーション情報
		m_texture.m_MaxFrame = num;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = speed;
		m_texture.m_HorizontalDivision = Horizontal;		//横分割数
		m_texture.m_VerticalDivision = Vertical;		//縦分割数
		return r;
	}
	HRESULT CTexture::SetTextureW( wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		HRESULT r;
		//アニメーションのセット
		IF_FAILED(r = SetTextureW( name ,width,height ))return r;
		//各アニメーション情報
		m_texture.m_MaxFrame = num;
		m_texture.m_NowFrame = 0;
		m_texture.m_NowTime = timeGetTime();
		m_texture.m_AnimeSpeed = speed;
		m_texture.m_HorizontalDivision = Horizontal;		//横分割数
		m_texture.m_VerticalDivision = Vertical;		//縦分割数
		return r;
	}
	//描画　:　位置、角度、ループする時間　:　一周したかどうか
	BOOL CTexture::DrawTexture(Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type , bool wait)
	{

		DrawTextureIndex(pos,rotate,expand,type,this->m_texture.m_NowFrame,wait);

		return FALSE;
		
	}
	BOOL CTexture::DrawTextureIndex(Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type,BYTE index , bool wait)
	{
		IF( m_texture.m_height > 2024U || m_texture.m_width > 2024U )return false;

		//待ちに追加
		IF( wait == true )
		{
			TWaitTexture t;
			t.m_tex = this;
			t.pos = pos;
			t.rotate = rotate;
			t.expand = expand;
			t.type = type;
			t.Index = index;
			m_DrawWaitTexture.push(t);
			return false;
		}

		
		float width = (float)m_texture.m_width;
		float height = (float)m_texture.m_height;

		BYTE NowFrame = index;

		//横の画像位置
		float Upos = 0;
		//画像数が１以上だったら
		if(this->m_texture.m_MaxFrame>1)Upos = ((float)NowFrame-(NowFrame/this->m_texture.m_HorizontalDivision)*this->m_texture.m_HorizontalDivision)/this->m_texture.m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_texture.m_HorizontalDivision);

		//縦の画像位置
		float Vpos = 0;
		//画像数が１以上だったら
		if(this->m_texture.m_MaxFrame>1)Vpos = (NowFrame/this->m_texture.m_HorizontalDivision)/(float)this->m_texture.m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_texture.m_VerticalDivision);

		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		//サンプラ
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

		IF(m_texture.m_pTextures==NULL)return false;

		//マトリックスの初期化
		D3DXMATRIXA16 TransMat;						//座標変換行列	
		D3DXMatrixIdentity(&TransMat);									//行列の初期化
		m_device->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//座標行列セット

		IF( type == TKYTEXTURE_TYPE_2D_MODE)
		{
			Vector vec[4] = {
				Vector( -width/2 , -height/2 , 0 ),
				Vector( width/2 , -height/2 , 0 ),
				Vector( -width/2 , height/2 , 0 ),
				Vector( width/2 , height/2 , 0 ),
			};

			FOR_I(4)
			{
				vec[i] = vec[i].Expand( Vector() , expand );	//拡大
				vec[i] = vec[i].Rotate( Vector() , rotate.x , rotate.y , rotate.z );
				vec[i] += pos;
			}

			TVERTEX Vertex[4] = {
				// x				y				z				rhw	color															tu							tv
				{ vec[0].x,	vec[0].y,	vec[0].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos					,	Vpos ,},
				{ vec[1].x,	vec[1].y,	vec[1].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	,	Vpos ,},
				{ vec[2].x,	vec[2].y,	vec[2].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos					,	Vpos + Vvec,},
				{ vec[3].x,	vec[3].y,	vec[3].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	,	Vpos + Vvec,},
			};

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1 );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( TVERTEX ) );
		}
		//3dはxy軸で構成され(0,0,1)の向きを向いている
		IF( type == TKYTEXTURE_TYPE_3D_MODE)
		{
/*
			TVERTEX Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ 1.0f,			1.0f,				0,	1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos ,},
				{ -1.0f,			1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos ,},
				{ -1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos + Vvec,},
				{ 1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos + Vvec,},
			};
*/
			CUSTOMVERTEX3DTEXTURE Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ Vector(-10.0f,-1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos ,},
				{ Vector(10.0f,	-1.0f,	0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos ,},
				{ Vector(-10.0f,	1.0f,0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos + Vvec,},
				{ Vector(10.0f,1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos + Vvec,},
			};


			//マップ初期化
			SetDeviceMatrix( m_device->GetD3DDevice() , pos, rotate , expand);

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_CUSTOMVERTEX3DTEXTURE );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( CUSTOMVERTEX3DTEXTURE ) );
		}

		return FALSE;
	}
	BOOL CTexture::DrawTextureAlpha(float alpha,Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type , bool wait)
	{

		DrawTextureIndexAlpha(alpha,pos,rotate,expand,type,this->m_texture.m_NowFrame,wait);

		return FALSE;
		
	}
	BOOL CTexture::DrawTextureIndexAlpha(float alpha,Vector pos,Vector rotate,Vector expand,TKYTEXTURE_TYPE type,BYTE index , bool wait)
	{
		//待ちに追加
		IF( wait == true )
		{
			TWaitTexture t;
			t.m_tex = this;
			t.pos = pos;
			t.rotate = rotate;
			t.expand = expand;
			t.type = type;
			t.Index = index;
			m_DrawWaitTexture.push(t);
			return false;
		}

		
		float width = (float)m_texture.m_width;
		float height = (float)m_texture.m_height;

		BYTE NowFrame = index;

		//横の画像位置
		float Upos = 0;
		//画像数が１以上だったら
		if(this->m_texture.m_MaxFrame>1)Upos = ((float)NowFrame-(NowFrame/this->m_texture.m_HorizontalDivision)*this->m_texture.m_HorizontalDivision)/this->m_texture.m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_texture.m_HorizontalDivision);

		//縦の画像位置
		float Vpos = 0;
		//画像数が１以上だったら
		if(this->m_texture.m_MaxFrame>1)Vpos = (NowFrame/this->m_texture.m_HorizontalDivision)/(float)this->m_texture.m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_texture.m_VerticalDivision);

		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		m_device->GetD3DDevice()->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		//サンプラ
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		m_device->GetD3DDevice()->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

		IF(m_texture.m_pTextures==NULL)return false;

		//マトリックスの初期化
		D3DXMATRIXA16 TransMat;						//座標変換行列	
		D3DXMatrixIdentity(&TransMat);									//行列の初期化
		m_device->GetD3DDevice()->SetTransform( D3DTS_WORLD , &TransMat );	//座標行列セット

		IF( type == TKYTEXTURE_TYPE_2D_MODE)
		{
			Vector vec[4] = {
				Vector( -width/2 , -height/2 , 0 ),
				Vector( width/2 , -height/2 , 0 ),
				Vector( -width/2 , height/2 , 0 ),
				Vector( width/2 , height/2 , 0 ),
			};

			FOR_I(4)
			{
				vec[i] = vec[i].Expand( Vector() , expand );	//拡大
				vec[i] = vec[i].Rotate( Vector() , rotate.x , rotate.y , rotate.z );
				vec[i] += pos;
			}

			INT al = 0xff * alpha;

			TVERTEX Vertex[4] = {
				// x				y				z				rhw	color															tu							tv
				{ vec[0].x,	vec[0].y,	vec[0].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos					,	Vpos ,},
				{ vec[1].x,	vec[1].y,	vec[1].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos + Uvec	,	Vpos ,},
				{ vec[2].x,	vec[2].y,	vec[2].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos					,	Vpos + Vvec,},
				{ vec[3].x,	vec[3].y,	vec[3].z, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,al)	,	Upos + Uvec	,	Vpos + Vvec,},
			};

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1 );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( TVERTEX ) );
		}
		//3dはxy軸で構成され(0,0,1)の向きを向いている
		IF( type == TKYTEXTURE_TYPE_3D_MODE)
		{
/*
			TVERTEX Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ 1.0f,			1.0f,				0,	1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos ,},
				{ -1.0f,			1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos ,},
				{ -1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec,	Vpos + Vvec,},
				{ 1.0f,			-1.0f,				0, 1,		D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				,	Vpos + Vvec,},
			};
*/
			CUSTOMVERTEX3DTEXTURE Vertex[4] = {
				// x				y					z	rhw	color															tu						tv
				{ Vector(-10.0f,-1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos ,},
				{ Vector(10.0f,	-1.0f,	0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos ,},
				{ Vector(-10.0f,	1.0f,0),	Vector(0,0,1)	,	Upos + Uvec,	Vpos + Vvec,},
				{ Vector(10.0f,1.0f,0),		Vector(0,0,1)	,	Upos				,	Vpos + Vvec,},
			};


			//マップ初期化
			SetDeviceMatrix( m_device->GetD3DDevice() , pos, rotate , expand);

			m_device->GetD3DDevice()->SetTexture( 0, m_texture.m_pTextures);
			m_device->GetD3DDevice()->SetVertexShader(NULL);
			m_device->GetD3DDevice()->SetPixelShader(0);
			m_device->GetD3DDevice()->SetFVF( D3DFVF_CUSTOMVERTEX3DTEXTURE );
			m_device->GetD3DDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertex, sizeof( CUSTOMVERTEX3DTEXTURE ) );
		}

		return FALSE;
	}
	//更新
	BOOL CTexture::Update(float step)
	{
/*
		//時間によって遷移
		IF( timeGetTime() - m_texture.m_NowTime > m_texture.m_AnimeSpeed*step )
		{
			//時間の設定
			m_texture.m_NowTime = timeGetTime();
			//アニメーションの遷移
			IF( ++m_texture.m_NowFrame >= m_texture.m_MaxFrame )
			{
				//初期化
				m_texture.m_NowFrame=0;
				//真を返す
				return true;
			}
		}
		//通常遷移したら偽を返す
		*/
		UpdateIndex( step , &m_texture.m_NowTime , &m_texture.m_NowFrame );

		return false;
	}
	BOOL CTexture::UpdateIndex(float step,DWORD *time,BYTE *index)
	{
		IF(time == NULL || index == NULL)MessageBoxW(NULL,TEXT("CTexture::Update(float step,DWORD *time,float *index)"),TEXT("無理"),NULL);

		//時間によって遷移
		IF( timeGetTime() - *time > m_texture.m_AnimeSpeed*step )
		{
			//時間の設定
			*time = timeGetTime();
			//アニメーションの遷移
			IF( ++*index >= m_texture.m_MaxFrame )
			{
				//初期化
				*index=0;
				//真を返す
				return true;
			}
		}
		//通常遷移したら偽を返す
		return false;
	}
	//生成(できなかったらNULLを返す)
	LPTkyTexture CTexture::CreateTexture( LPTKY3D device , string name ,INT width,INT height)
	{
		LPTkyTexture tex = new CTexture();
		tex->SetDevice(device);
		IF_FAILED( tex->SetTexture( name , width , height ) )
		{
			//消去
			tex->DestroyOwnObject();
			return NULL;
		}
		return tex;
	}
	//生成　:　ファイル名、総画像数、横画像数、縦画像数,画像の横幅、画像の縦幅,アニメーションスピード;
	LPTkyTexture CTexture::CreateTexture( LPTKY3D device , string name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		LPTkyTexture tex = new CTexture();
		tex->SetDevice(device);
		IF_FAILED( tex->SetTexture( name , num,Horizontal,Vertical,width,height,speed ) )
		{
			//消去
			tex->DestroyOwnObject();
			return NULL;
		}
		return tex;
	}
	LPTkyTexture CTexture::CreateTextureW( LPTKY3D device , wstring name , INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
	{
		LPTkyTexture tex = new CTexture();
		tex->SetDevice(device);
		IF_FAILED( tex->SetTextureW( name , num,Horizontal,Vertical,width,height,speed ) )
		{
			//消去
			tex->DestroyOwnObject();
			return NULL;
		}
		return tex;
	}

//===========================================================
//終わり
//===========================================================

//////////////////////////////////////////////////////////////////////////////////////////
//
//		2D描画
//
//////////////////////////////////////////////////////////////////////////////////////////
vector<D3DTKY2DTEXTURE*> D3DTKY2DTEXTURE::m_vTex;
//---------------------------------------------//
//コントラスタ
//---------------------------------------------//
D3DTKY2DTEXTURE::D3DTKY2DTEXTURE(LPTKY3D init,HLSLShader* phlsl)
{
	IF(init!=NULL)	D3DTKY2DTEXTURE::m_pD3D = init->GetD3D();
	IF(init!=NULL)D3DTKY2DTEXTURE::m_pD3DDevice = init->GetD3DDevice();
	D3DTKY2DTEXTURE::m_pTextures = NULL;
	D3DTKY2DTEXTURE::m_pHLSL = phlsl;
	D3DTKY2DTEXTURE::m_width = 0;
	D3DTKY2DTEXTURE::m_height = 0;
}
D3DTKY2DTEXTURE::D3DTKY2DTEXTURE(const D3DTKY2DTEXTURE& tky2d)
{
	D3DTKY2DTEXTURE::m_pTextures = NULL;
	*this = tky2d;
	//現在のフレームの初期化
	this->m_NowFrame = 0;
}
D3DTKY2DTEXTURE::~D3DTKY2DTEXTURE()
{

	assert(D3DTKY2DTEXTURE::m_pTextures);
	SAFE_RELEASE(D3DTKY2DTEXTURE::m_pTextures);

}
HRESULT D3DTKY2DTEXTURE::ReadTexture(LPCTSTR filename,INT num,INT Horizontal,INT Vertical,INT width,INT height,DWORD speed)
{
	HRESULT r;
	if(FAILED(r =	D3DXCreateTextureFromFileEx(
									D3DTKY2DTEXTURE::m_pD3DDevice,
									filename,              // ファイル名
									0,
									0,
									0,
									0,
									D3DFMT_A16B16G16R16,         
									D3DPOOL_MANAGED,
									D3DX_FILTER_LINEAR,
									D3DX_FILTER_LINEAR,
									D3DCOLOR_ARGB(0, 0, 0, 0),  // 黒色を透過色とする
									NULL,
									NULL,
									&(D3DTKY2DTEXTURE::m_pTextures)						//読み込んだテクスチャを返すポインタ
								)))
	{
		D3DTKY2DTEXTURE::m_pTextures = NULL;
		MessageBox(NULL,filename,TEXT("画像読み込めません	"),MB_OK);
		return r;
	}

	// サーフェイスを取得
	IDirect3DSurface9 *pSurface;
	D3DTKY2DTEXTURE::m_pTextures->GetSurfaceLevel( 0, &pSurface ); 
	// サーフェイス情報から画像サイズを取得
	D3DSURFACE_DESC SurfaceInfo;
	pSurface->GetDesc( &SurfaceInfo );
	D3DTKY2DTEXTURE::m_width = width;    // 幅（ピクセル）
	D3DTKY2DTEXTURE::m_height = height;   // 高さ（ピクセル）

	//各アニメーション情報
	D3DTKY2DTEXTURE::m_MaxFrame = num;
	D3DTKY2DTEXTURE::m_NowFrame = 0;
	D3DTKY2DTEXTURE::m_NowTime = timeGetTime();
	D3DTKY2DTEXTURE::m_AnimeSpeed = speed;
	D3DTKY2DTEXTURE::m_HorizontalDivision = Horizontal;		//横分割数
	D3DTKY2DTEXTURE::m_VerticalDivision = Vertical;		//縦分割数

	return r;

}
BOOL D3DTKY2DTEXTURE::DrawTexture(float x,float y,INT loop)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//横の画像位置
		float Upos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//縦の画像位置
		float Vpos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		//サンプラ
		D3DTKY2DTEXTURE::m_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_POINT);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_POINT);

		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				, Vpos + Vvec,},
		};

		IF(D3DTKY2DTEXTURE::m_pTextures==NULL)return false;

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTexture( 0, D3DTKY2DTEXTURE::m_pTextures);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetVertexShader(NULL);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1 );
		D3DTKY2DTEXTURE::m_pD3DDevice->SetPixelShader(0);
		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		return TRUE;
}
BOOL D3DTKY2DTEXTURE::DrawTexture(float x,float y,float falpha,INT loop)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//横の画像位置
		float Upos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//縦の画像位置
		float Vpos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,   D3DTA_DIFFUSE);

		UINT alpha = (UINT)(0xff * falpha);


		TVERTEX Vertex[4] = {
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTexture( 0, D3DTKY2DTEXTURE::m_pTextures);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetVertexShader(NULL);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW  | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetPixelShader(0);
		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		return TRUE;
}
BOOL D3DTKY2DTEXTURE::DrawTexture3D(float x,float y,float z,INT loop)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//横の画像位置
		float Upos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//縦の画像位置
		float Vpos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);

		char str[MAX_PATH];
		sprintf_s(str,"NOW:%d,MAX:%d,VERTICAL:%d,UPOS:%lf UVEC:%lf VPOS:%lf VVEC:%lf\n",this->m_NowFrame,this->m_MaxFrame,this->m_VerticalDivision,Upos,Uvec,Vpos,Vvec);

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);

		//座標初期化
		D3DXMATRIXA16 TransMat,RotateMat,matWorld;						//座標変換行列	
		D3DXMatrixIdentity(&TransMat);									//行列の初期化
		D3DXMatrixIdentity(&RotateMat);									//行列の初期化
		D3DXMatrixIdentity(&matWorld);									//合計行列の初期化
		//座標セット
		D3DXMatrixTranslation(&TransMat,x,y,z);							//座標行列移動			
//		D3DXMatrixRotationY(&RotateMat,a_rad);							//y軸回転の行列にする

		matWorld = RotateMat * TransMat;								//行列の合体				 
		D3DTKY2DTEXTURE::m_pD3DDevice->SetTransform( D3DTS_WORLD , &matWorld );	//座標行列セット


		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetTexture( 0, D3DTKY2DTEXTURE::m_pTextures);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetVertexShader(NULL);
		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );
		D3DTKY2DTEXTURE::m_pD3DDevice->SetPixelShader(0);
		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		return TRUE;

}
BOOL D3DTKY2DTEXTURE::DrawTextureHLSL(float x,float y,float z,INT loop,HLSLShader* hlsl)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//横の画像位置
		float Upos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//縦の画像位置
		float Vpos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);


		//座標初期化
		D3DXMATRIXA16 TransMat,RotateMat,matWorld,l_mat;						//座標変換行列	
		D3DXMatrixIdentity(&TransMat);									//行列の初期化
		D3DXMatrixIdentity(&RotateMat);									//行列の初期化
		D3DXMatrixIdentity(&matWorld);									//合計行列の初期化
		//座標セット
		D3DXMatrixTranslation(&TransMat,x,y,z);							//座標行列移動			
//		D3DXMatrixRotationY(&RotateMat,a_rad);							//y軸回転の行列にする

		matWorld = RotateMat * TransMat;								//行列の合体				 


		//視点変換行列の作成-------------------------------------------
		D3DXMatrixIdentity(&l_mat);
		hlsl->m_pEffect->SetMatrix(hlsl->m_hmWVP,&l_mat);			//行列の設定
		//-------------------------------------------------------------

		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );

		hlsl->m_pEffect->SetTexture( hlsl->m_hTex , D3DTKY2DTEXTURE::m_pTextures );


		hlsl->BeginShader();

		hlsl->m_pEffect->BeginPass(D3DTKY_ONLY_TEXTURE);

		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		hlsl->m_pEffect->EndPass();

		hlsl->EndShader();

		return TRUE;

}
BOOL D3DTKY2DTEXTURE::DrawTextureHLSL(float x,float y,float z,float falpha,INT loop,HLSLShader* hlsl)
{
		float width = (float)D3DTKY2DTEXTURE::m_width;
		float height = (float)D3DTKY2DTEXTURE::m_height;

		//横の画像位置
		float Upos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Upos = ((float)this->m_NowFrame-(this->m_NowFrame/this->m_HorizontalDivision)*this->m_HorizontalDivision)/this->m_HorizontalDivision;
		float Uvec =  (1.0f / (float)this->m_HorizontalDivision);

		//縦の画像位置
		float Vpos = 0;
		//画像数が１以上だったら
		if(this->m_MaxFrame>1)Vpos = (this->m_NowFrame/this->m_HorizontalDivision)/(float)this->m_VerticalDivision;
		float Vvec =  (1.0f / (float)this->m_VerticalDivision);


		//座標初期化
		D3DXMATRIXA16 TransMat,RotateMat,matWorld,l_mat;						//座標変換行列	
		D3DXMatrixIdentity(&TransMat);									//行列の初期化
		D3DXMatrixIdentity(&RotateMat);									//行列の初期化
		D3DXMatrixIdentity(&matWorld);									//合計行列の初期化
		//座標セット
		D3DXMatrixTranslation(&TransMat,x,y,z);							//座標行列移動			
//		D3DXMatrixRotationY(&RotateMat,a_rad);							//y軸回転の行列にする

		matWorld = RotateMat * TransMat;								//行列の合体				 


		//視点変換行列の作成-------------------------------------------
		D3DXMatrixIdentity(&l_mat);
		hlsl->m_pEffect->SetMatrix(hlsl->m_hmWVP,&l_mat);			//行列の設定
		//-------------------------------------------------------------

		UINT alpha = (UINT)(0xff * falpha);

		TVERTEX Vertex[4] = {
			// x				y					z rhw color  tu tv
/*			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 0 ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 0 ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	1	, 1,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,0xff)	,	0	, 1,},
			*/			
			{ x-width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos			, Vpos ,},
			{ x+width/2,	y-height/2,		0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos ,},
			{ x+width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos + Uvec	, Vpos + Vvec,},
			{ x-width/2,	y+height/2,	0, 1,	D3DCOLOR_RGBA(0xff,0xff,0xff,alpha)	,	Upos				, Vpos + Vvec,},
		};

		D3DTKY2DTEXTURE::m_pD3DDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );

		hlsl->m_pEffect->SetTexture( hlsl->m_hTex , D3DTKY2DTEXTURE::m_pTextures );


		hlsl->BeginShader();

		hlsl->m_pEffect->BeginPass(D3DTKY_ONLY_TEXTURE);

		D3DTKY2DTEXTURE::m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );

		hlsl->m_pEffect->EndPass();

		hlsl->EndShader();

		return TRUE;

}
BOOL D3DTKY2DTEXTURE::Update()
{
	//時間によって遷移
	if( timeGetTime() - this->m_NowTime > this->m_AnimeSpeed )
	{
		//時間の設定
		this->m_NowTime = timeGetTime();
		//アニメーションの遷移
		if( ++this->m_NowFrame >= this->m_MaxFrame )
		{
			//初期化
			this->m_NowFrame=0;
			//真を返す
			return true;
		}
	}
	//通常遷移したら偽を返す
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
//		2Dアニメーション描画
//
//////////////////////////////////////////////////////////////////////////////////////////
	vector<D3DTKY2DTEXTUREANIMATION*> D3DTKY2DTEXTUREANIMATION::m_vTexAnime;
	D3DTKY2DTEXTUREANIMATION::D3DTKY2DTEXTUREANIMATION()
	{
		D3DTKY2DTEXTUREANIMATION::m_texture = NULL;
		D3DTKY2DTEXTUREANIMATION::m_nowindex = 0;
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}
	D3DTKY2DTEXTUREANIMATION::D3DTKY2DTEXTUREANIMATION(D3DTKY2DTEXTURE* tex)
	{
		D3DTKY2DTEXTUREANIMATION::m_texture = tex;
		D3DTKY2DTEXTUREANIMATION::m_nowindex = 0;
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}
	D3DTKY2DTEXTUREANIMATION::D3DTKY2DTEXTUREANIMATION(const D3DTKY2DTEXTUREANIMATION& texanime)
	{
		D3DTKY2DTEXTUREANIMATION::m_texture = texanime.m_texture;
		D3DTKY2DTEXTUREANIMATION::m_nowindex = 0;
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}
	D3DTKY2DTEXTUREANIMATION::~D3DTKY2DTEXTUREANIMATION()
	{
		for(UINT i=0;i<m_Element.size();i++)
		{
			for(UINT j=0;j<m_Element[i].size();j++)
			{
				m_Element[i].clear();
			}
		}
		D3DTKY2DTEXTUREANIMATION::m_Element.clear();
	}

	void	D3DTKY2DTEXTUREANIMATION::AddElement(vector<ANIMATION> anime)	//要素の追加 引数　アニメーションデータ,入れ方　０：入れた後次に進む　、　１：入れた後次に進まない
	{
		//データ調整用
		D3DTKY2DTEXTUREANIMATION::m_Element.push_back(anime);

	}
	bool	D3DTKY2DTEXTUREANIMATION::Update()								//更新
	{
		bool flag=false;
		//今の動作をする
		for(UINT i=0;i<this->m_Element[this->m_nowindex].size();i++)
		{
			//終了だったら
			if( ++this->m_Element[this->m_nowindex][i].m_nowloop >= this->m_Element[this->m_nowindex][i].m_loopnum )
			{
				//終了後固定
				this->m_Element[this->m_nowindex][i].m_nowloop = this->m_Element[this->m_nowindex][i].m_loopnum;
				//終了フラグだったら
				if(this->m_Element[this->m_nowindex][i].m_endflag)flag = true;
			}
			else	//終了じゃなかったら
			{
				//移動方法
				switch(this->m_Element[this->m_nowindex][i].m_type)
				{
					case MOVE_ANIME:
						this->m_pos += this->m_Element[this->m_nowindex][i].m_value;
					break;
					case MAGNIFY_ANIME:
					break;
					case ROTATE_ANIME:
					break;
					case ALPHA_ANIME:
					break;
				}
			}
		}
		//エンドフラグを探す
		if(flag)
		{
			if( ++this->m_nowindex >= (int)this->m_Element.size() )
			{
				//座標初期化
				this->m_pos = Vector();
				this->m_nowindex = 0;
				//ループ状況の初期化
				for(UINT d=0;d<this->m_Element.size();d++)
				{
						for(UINT i=0;i<this->m_Element[d].size();i++)
						{
							this->m_Element[d][i].m_nowloop=0;
						}
				}
				//もう一度処理を行う
				Update();
				return true;
			}
		}
		return false;
	}
	void	D3DTKY2DTEXTUREANIMATION::Darw()									//描画する
	{
		D3DTKY2DTEXTUREANIMATION::m_texture->DrawTexture(
			D3DTKY2DTEXTUREANIMATION::m_pos.x,
			D3DTKY2DTEXTUREANIMATION::m_pos.y,0);
	}
	void	D3DTKY2DTEXTUREANIMATION::Darw(HLSLShader *hlsl)									//描画する
	{
		D3DTKY2DTEXTUREANIMATION::m_texture->DrawTextureHLSL(
			D3DTKY2DTEXTUREANIMATION::m_pos.x,
			D3DTKY2DTEXTUREANIMATION::m_pos.y,0,0,hlsl);
	}

//////////////////////////////////////////////////////////////////////////////////////////
//
//		2Dアニメーションエフェクト実用
//
//////////////////////////////////////////////////////////////////////////////////////////
//システム
vector<CTKYEFFECT*> CTKYEFFECT::m_vIndex;

CTKYEFFECT::CTKYEFFECT()
{
	m_effect.clear();
}
CTKYEFFECT::~CTKYEFFECT()
{
	m_effect.clear();
}

VOID CTKYEFFECT::SetEffect(LPTKYTEXTURE tex,Vector v)
{
	t_effect effect;
	effect.m_anime = D3DTKY2DTEXTURE::CreateTexture(NULL,NULL);
	*effect.m_anime = *tex;
	effect.m_pos = v;
	m_effect.push_back( effect);
}
VOID CTKYEFFECT::SetEffect(LPTKYTEXTUREANIME texanime,Vector v)
{
	t_effect effect;
	effect.m_anime = D3DTKY2DTEXTURE::CreateTexture(NULL,NULL);
	*effect.m_anime = *texanime->GetTexture();
	effect.m_pos = v;
	m_effect.push_back( effect);
}

VOID CTKYEFFECT::Update()
{
	vector<t_effect>::iterator effect = m_effect.begin();
	FOR( ;effect != m_effect.end();effect++ )
	{
		IF( effect->m_anime->Update() )
		{
			effect = m_effect.erase(effect);
			IF(m_effect.empty())break;
		}
	}

}
VOID CTKYEFFECT::Draw()
{

	vector<t_effect>::iterator effect = m_effect.begin();
	FOR( ;effect != m_effect.end();effect++ )
	{
		effect->m_anime->DrawTexture(
			effect->m_pos.x,
			effect->m_pos.y,
			0
			);
	}

}


CTKYEFFECT* CTKYEFFECT::CreateEffect()
{
	CTKYEFFECT * e = new CTKYEFFECT();
	m_vIndex.push_back(e);
	return e;
}
VOID CTKYEFFECT::Destroy()
{
	FOR_I(m_vIndex.size())SAFE_DELETE(m_vIndex[i]);
	m_vIndex.clear();
}