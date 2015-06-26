#pragma once

#include<d3dx9.h>
#include<vector>
#include<map>
#include "TKYBASEHEADER.h"
#include "TKY3D.h"

class HLSLShader;


//世界の見える大きさ
static const FLOAT WORLD_NEAR_Z = 0.1f;
static const FLOAT WORLD_FAR_Z = 10000.0f;

//見えている角度
static const FLOAT WORLD_DEGREE = 30.0f;

//影の見える大きさ
static const FLOAT SHADOW_NEAR_Z = 0.5f;
static const FLOAT SHADOW_FAR_Z = 1000.0f;

//エラーチェック
static void HRESULTCheck(HRESULT hr)
{

		switch(hr)
		{
		case D3DERR_INVALIDCALL:
				   MessageBox(NULL,TEXT("D3DERR_INVALIDCALL"),TEXT("失敗"),MB_OK);	
			break;
		case D3DXERR_INVALIDDATA:
				   MessageBox(NULL,TEXT("D3DXERR_INVALIDDATA"),TEXT("失敗"),MB_OK);	
			break;
		case E_OUTOFMEMORY:
				   MessageBox(NULL,TEXT("E_OUTOFMEMORY"),TEXT("失敗"),MB_OK);	
			break;
		default:
				   MessageBox(NULL,TEXT("その他"),TEXT("失敗"),MB_OK);	
			break;
		}

}


//========== 新しい内容 ==========


//--------------------------------------------------//
//　クラス　CMatrixMaterialSet
//　説明　　環境マトリックスの設定
//--------------------------------------------------//
CLASS(CMatrixMaterialSet)
{
protected:
	D3DXVECTOR3 m_Eye;
	D3DXVECTOR3 m_At;
	D3DXVECTOR3 m_Up;
	FLOAT m_Deg,m_aspect,m_Near,m_Far;
	D3DXMATRIX m_lookup;
	D3DXMATRIX m_Perspective;
	LPTKY3D m_device;
public:
	CMatrixMaterialSet()
	{
		m_Eye = m_At = m_Up = D3DXVECTOR3();
		m_Deg = m_aspect = m_Near = m_Far = NULL;
		m_lookup = m_Perspective = D3DXMATRIX();
		m_device = NULL;
	}
	//設定
	bool SetLookAtMatrix( D3DXVECTOR3* Eye, D3DXVECTOR3* At, D3DXVECTOR3*  Up)
	{
		IF(m_Eye==NULL)m_Eye = D3DXVECTOR3();
		else m_Eye = *Eye;
		IF(m_At==NULL)m_At = m_Eye;
		else m_At = *At;
		IF(m_Up==NULL)m_Up = D3DXVECTOR3(0,1,0);
		else m_Up = *Up;

		D3DXMatrixLookAtLH(&m_lookup,&m_Eye,&m_At,&m_Up);
		return true;
	}
	bool  SetVec(D3DXVECTOR3 *pos,D3DXVECTOR3 *vec)
	{
		D3DXVECTOR3 p = *pos,v = *vec;
		IF(pos==NULL)p = m_Eye;
		else m_Eye = *pos;
		IF(vec==NULL)v = m_Eye;
		else m_At = m_Eye+*vec;

		SetLookAtMatrix( &m_Eye , &m_At , &D3DXVECTOR3(0,1,0) );
	}

	bool SetPerspectiveMatrix( FLOAT _deg,FLOAT _aspect,FLOAT _near,FLOAT _far)
	{
		m_Deg = _deg;m_aspect = _aspect;m_Near = _near;m_Far = _far;
		D3DXMatrixPerspectiveFovLH(&m_Perspective,D3DXToRadian(m_Deg),m_aspect,m_Near,m_Far);
		return true;
	}
	bool SetDevice(LPTKY3D device)
	{
		IF(device==NULL)return false;
		m_device = device;
		return true;
	}
	//取得
	//EYE,AT,UP
	D3DXVECTOR3 GetLookAtValue( string str )
	{
		IF( str == "EYE" )return m_Eye;
		EIF( str == "AT" )return m_At;
		EIF( str == "UP" )return m_Up;	
		return D3DXVECTOR3();
	}
	//DEGREE,ASPECT,NEAR,FAR
	FLOAT GetPerspectiveValue( string str )
	{
		IF( str == "DEGREE" )return m_Deg;
		EIF( str == "ASPECT" )return m_aspect;
		EIF( str == "NEAR" )return m_Near;	
		EIF( str == "FAR" )return m_Far;	
		return NULL;
	}
	//LOOKUP,PERSPECT
	D3DXMATRIX GetMatrix( string str )
	{
		IF( str == "LOOKUP" )return m_lookup;
		EIF( str == "PERSPECT" )return m_Perspective;
		return D3DMATRIX();
	}
	//デバイス情報
	LPTKY3D	GetDevice(){return m_device;};
	//使用
	VIRTUAL bool onExecute()=0;
};
//--------------------------------------------------//
//　クラス　CLigth : public CMatrixMaterialSet
//　説明　　ライトマトリックスの設定
//--------------------------------------------------//
CLASS(CLight) : public CMatrixMaterialSet
{
private:
	D3DMATERIAL9 m_color;
	D3DLIGHT9 m_light;
public:
	//設定
	VOID SetMaterial(D3DXVECTOR4 *m_amb ,D3DXVECTOR4 *m_dif,D3DXVECTOR4 *m_spe)
	{
		IF(m_amb!=NULL)
		{
			m_color.Ambient.r = m_amb->x;
			m_color.Ambient.g = m_amb->y;
			m_color.Ambient.b = m_amb->z;
		}
		IF(m_dif!=NULL)
		{
			m_color.Diffuse.r = m_dif->x;
			m_color.Diffuse.g = m_dif->y;
			m_color.Diffuse.b = m_dif->z;
		}
		IF(m_spe!=NULL)
		{
			m_color.Specular.r = m_spe->x;
			m_color.Specular.g = m_spe->y;
			m_color.Specular.b = m_spe->z;
		}
	}
	//取得
	D3DMATERIAL9 GetMaterial(){return m_color;}
	//使用(仮想)
	VIRTUAL bool onExecute()
	{
		//初期化
		ZeroMemory(&m_light,sizeof(D3DLIGHT9));
		//光源の種類
		m_light.Type = D3DLIGHT_DIRECTIONAL;	//並行光源
		//マテリアル情報
		m_light.Ambient = m_color.Ambient;
		m_light.Diffuse = m_color.Diffuse;
		m_light.Specular = m_color.Specular;
		//始点
		m_light.Position = m_Eye;
		//方向の設定
		D3DXVec3Normalize( (D3DXVECTOR3*)&m_light.Direction,&(m_At-m_Eye) );
		//ライトの登録
		m_device->GetD3DDevice()->SetLight(0,&m_light);
		m_device->GetD3DDevice()->LightEnable(0,TRUE);
		m_device->GetD3DDevice()->SetRenderState(D3DRS_LIGHTING,TRUE);
		m_device->GetD3DDevice()->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_ARGB(0,96,96,96));

		return true;
	}

};
//--------------------------------------------------//
//　クラス　CCamera : public CMatrixMaterialSet
//　説明　　カメラマトリックスの設定
//--------------------------------------------------//
CLASS(CCamera) : public CMatrixMaterialSet
{
private:
public:
		//使用(仮想)
	VIRTUAL bool onExecute()
	{
		m_device->GetD3DDevice()->SetTransform( D3DTS_VIEW , &m_lookup );
		m_device->GetD3DDevice()->SetTransform( D3DTS_PROJECTION , &m_Perspective );
		return true;
	}
};
//--------------------------------------------------//
//　クラス　CEnvironment:CObject
//　説明　　環境の設定
//--------------------------------------------------//
CLASS_POINTA_OBJECT( CEnvironment , LPTkyEnvironment )
{
protected:
	CCamera	m_camera;	
	CLight		m_light;
	LPTKY3D m_device;

	CEnvironment()
	{
	}
	VIRTUAL ~CEnvironment()
	{
	}

public:
	//設定
	VOID SetLightParameter( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , D3DXVECTOR4 *m_amb ,D3DXVECTOR4 *m_dif,D3DXVECTOR4 *m_spe )
	{
		m_light.SetLookAtMatrix( pos , &(*pos+*vec) , &D3DXVECTOR3(0,1,0) );
		m_light.SetMaterial( m_amb,m_dif,m_spe );
	}
	VOID SetLightParameterAt( D3DXVECTOR3 *pos, D3DXVECTOR3 *at , D3DXVECTOR4 *m_amb ,D3DXVECTOR4 *m_dif,D3DXVECTOR4 *m_spe )
	{
		m_light.SetLookAtMatrix( pos , at , &D3DXVECTOR3(0,1,0) );
		m_light.SetMaterial( m_amb,m_dif,m_spe );
	}
	VOID SetLightParameter( CLight *light )
	{
		m_light = *light;
	}
	VOID SetCameraParameter( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , FLOAT _deg,FLOAT _aspect,FLOAT _near,FLOAT _far  )
	{
		m_camera.SetLookAtMatrix( pos , &(*pos+*vec) , &D3DXVECTOR3(0,1,0) );
		m_camera.SetPerspectiveMatrix(_deg,_aspect,_near,_far);
	}
	VOID SetCameraParameter( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec )
	{
		m_camera.SetLookAtMatrix( pos , &(*pos+*vec) , &D3DXVECTOR3(0,1,0) );
	}
	VOID SetCameraParameterAt( D3DXVECTOR3 *pos, D3DXVECTOR3 *at , FLOAT _deg,FLOAT _aspect,FLOAT _near,FLOAT _far  )
	{
		m_camera.SetLookAtMatrix( pos , at , &D3DXVECTOR3(0,1,0) );
		m_camera.SetPerspectiveMatrix(_deg,_aspect,_near,_far);
	}
	VOID SetCameraParameter( CCamera *camera )
	{
		m_camera = *camera;
	}
	VOID SetDevice(LPTKY3D name){m_device = name;m_camera.SetDevice(name);m_light.SetDevice(name);};
	//取得
	LPTKY3D	GetDevice(){return m_device;}
	VOID			GetLight( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , D3DMATERIAL9 *material )
	{
		IF( pos!=NULL )*pos = m_light.GetLookAtValue("EYE");
		IF( vec!=NULL )*vec = m_light.GetLookAtValue("AT") - m_light.GetLookAtValue("EYE");
		IF( material!=NULL )*material = m_light.GetMaterial(); 
	}
	VOID			GetCamera( D3DXVECTOR3 *pos, D3DXVECTOR3 *vec , FLOAT *_deg,FLOAT *_aspect,FLOAT *_near,FLOAT *_far  )
	{
		IF( pos!=NULL )*pos = m_camera.GetLookAtValue("EYE");
		IF( vec!=NULL )*vec = m_camera.GetLookAtValue("AT") - m_camera.GetLookAtValue("EYE");
		IF( _deg!=NULL )*_deg = m_camera.GetPerspectiveValue("DEGREE");
		IF( _aspect!=NULL )*_aspect = m_camera.GetPerspectiveValue("ASPECT");
		IF( _near!=NULL )*_near = m_camera.GetPerspectiveValue("NEAR");
		IF( _far!=NULL )*_far = m_camera.GetPerspectiveValue("FAR");
	}

	//スクリーン座標からワールド座標を取得する
	Vector			GetWorldPosition( Vector screen )
	{
		RECT rect;
		this->m_device->GetD3DDevice()->GetScissorRect(&rect);

		//カメラの行列
		D3DXMATRIX matView = this->m_camera.GetMatrix("LOOKUP");
		D3DXMATRIX matProj = 	this->m_camera.GetMatrix("PERSPECT");

		//射影、ビューの取得
	   const D3DXMATRIX matV = matView;
	   const D3DXMATRIX matP = matProj;
	   // 各行列の逆行列を算出
	   D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	   D3DXMatrixInverse( &InvView, NULL, &matV);
	   D3DXMatrixInverse( &InvPrj, NULL,  &matP );
	   D3DXMatrixIdentity( &VP );
	   VP._11 = rect.right/2.0f; VP._22 = -rect.bottom/2.0f;
	   VP._41 = rect.right/2.0f; VP._42 = rect.bottom/2.0f;
	   //ビューポート
	   D3DXMatrixInverse( &InvViewport, NULL, &VP );

	   // 逆変換
	   D3DXMATRIX tmp = InvViewport * InvPrj * InvView;

		//出力座標
		D3DXVECTOR3 Near = Vector();
		D3DXVECTOR3 Far = Vector();
	   D3DXVec3TransformCoord( &Near, &D3DXVECTOR3(screen.x,screen.y,0), &tmp );
	   D3DXVec3TransformCoord( &Far, &D3DXVECTOR3(screen.x,screen.y,1), &tmp );

	   return (Far - Near);
	}


	//実行(仮想)(HLSL時は違う状況になる予定だから)
	VIRTUAL VOID onExecute()
	{
		m_light.onExecute();
		m_camera.onExecute();
	}
	//開放
	VIRTUAL VOID Release(){};
	//生成
	STATIC LPTkyEnvironment CreateEnvironment(){return new CEnvironment();};
};
//--------------------------------------------------//
//　クラス　CHLSL:CEnvironment
//　説明　　HLSLの設定
//--------------------------------------------------//
CLASS_POINTA(CHLSL,LPTkyHLSL):public CEnvironment
{
private:
	//テクニック情報
	D3DXHANDLE m_hTechnique;
	//ハンドル情報(HLSL 変数)
	CStringMatch<D3DXHANDLE> m_handle;
	//コンストラクタ
	CHLSL();
protected:
	//シェーダ情報
	LPD3DXEFFECT	m_pEffect;	
public:
	//HLSLの生成
	HRESULT SetHLSL( string str)
	{
	//シェーダの読み込み
	LPD3DXBUFFER pErr = NULL;
	HRESULT hr = S_OK;

	IF( FAILED( hr = D3DXCreateEffectFromFileA(
		m_device->GetD3DDevice(),
		str.c_str(),
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErr ) ) )
	{
		//エラーチェック
		HRESULTCheck(hr);
		//文字サイズ取得
		LPSTR s = (LPSTR)pErr->GetBufferPointer();
		//メッセージの取得
		MessageBoxA(NULL,s,"失敗",MB_OK);	
	}else{
		//成功したらハンドルの追加
		onSetHandle( &m_hTechnique,&m_handle );
	}

	SAFE_RELEASE(pErr);

	return hr;
	}
	//初期化(仮想化して継承先で設定できるようにする)
	VIRTUAL VOID onSetHandle( D3DXHANDLE *m_hTechnique,CStringMatch<D3DXHANDLE> *m_handle ) PURE;
	//HLSLの適用
	VIRTUAL VOID onExecuteHLSL( string event ) PURE;

	//開放
	VIRTUAL VOID Release(){};
	//生成
//	STATIC LPTkyHLSL CreateHLSL(){return new CHLSL();};
};
//--------------------------------------------------//
//　クラス　CXFileHLSL:CHLSL
//　説明　　座標変形型HLSLの設定
//--------------------------------------------------//
CLASS_POINTA(CXFileHLSL,LPTkyXFileHLSL):public CHLSL
{
private:
	//コンストラクタ
	CXFileHLSL();
	//登録されたXFile情報
public:
	//初期化(仮想化して継承先で設定できるようにする)
	VIRTUAL VOID onSetHandle( D3DXHANDLE *m_hTechnique,CStringMatch<D3DXHANDLE> *m_handle )
	{
		*m_hTechnique		=	m_pEffect->GetTechniqueByName("TShader");

		//ビューと射影空間への座標変換
		m_handle->AddString(	 "m_hmVP" , m_pEffect->GetParameterByName(NULL,"g_mVP") );
		//ローカル座標から射影空間への座標変換
		m_handle->AddString(	 "m_hmWVP" , m_pEffect->GetParameterByName(NULL,"g_mWVP") );
		
		//ボーン用変換行列の配列
		m_handle->AddString( "m_hmWorldMatrixArray" , m_pEffect->GetParameterByName(NULL,"g_mWorldMatrixArray") );
		//ボーン数(0のときは通常レンダリング)
		m_handle->AddString( "m_hiNumBones" , m_pEffect->GetParameterByName(NULL,"g_NumBones") );

		//テクスチャの設定
		m_handle->AddString( "m_hTex" ,	m_pEffect->GetParameterByName(NULL,"g_Tex") );

		//光源の設定(法線考慮の陰影計算用光源ベクトル)
		m_handle->AddString( "m_hvLight" , m_pEffect->GetParameterByName(NULL,"g_vLight") );

		//カメラの位置(通常描画用(太陽として))
		m_handle->AddString( "m_hvEyePos" , m_pEffect->GetParameterByName(NULL,"g_vEyePos") );

		//光の強さ(ユーザ制御)
		m_handle->AddString( "m_hvLightAmb" , m_pEffect->GetParameterByName(NULL,"g_vLightAmb") );
		m_handle->AddString( "m_hvLightDif" , m_pEffect->GetParameterByName(NULL,"g_vLightDif") );

	}
	//HLSLの適用
	VIRTUAL VOID onExecuteHLSL( string event )
	{
	}
	//生成
	STATIC LPTkyXFileHLSL CreateXFileHLSL(){return new CXFileHLSL();}
};

//========== 古い内容 ==========

//--------------------------------------------------//
//HLSLをつかう
//--------------------------------------------------//
class HLSLShader
{

private:

	D3DXHANDLE		m_hTechnique;		//テクニック
	LPDIRECT3DDEVICE9	m_pD3DDevice;		//DirectX9　インターフェイスのデバイス

	//----------カメラの設定----------
	D3DXMATRIX		m_matCameraView;	//ビュー行列
	D3DXMATRIX		m_matCameraProj;	//射影行列
	D3DXVECTOR4		m_vCameraPos;		//カメラの位置
	D3DXVECTOR4		m_vCameraVec;		//カメラのベクトル
	//--------------------------------

	//----------ライトの設定----------
	D3DXMATRIX		*m_matLightView;	//ライトからみたビュー行列
	D3DXMATRIX		*m_matLightProj;	//ライトからみた射影行列
	D3DXVECTOR4		*m_vLightPos;		//ライトの位置
	D3DXVECTOR4		*m_vLightVec;		//ライトのベクトル
	int				m_NowLight;		    //現在のライト
	int				m_LightNum;			//ライトの個数
	float			m_dacay_c0;			//減衰率値０
	float			m_dacay_c1;			//減衰率値１
	float			m_dacay_c2;			//減衰率値２
	//--------------------------------

public:

	D3DXHANDLE		m_hmWorld;			//ワールド行列
	D3DXHANDLE		m_hmVP;				//ビュー射影行列
	D3DXHANDLE		m_hmWVP;			//ワールド射影行列
	D3DXHANDLE		m_hmWVPB;			//ワールド射影行列バイアス適用後(テクスチャ座標)射影行列
	D3DXHANDLE		m_hmWLP;			//ライトからみたワールド射影行列
	D3DXHANDLE		m_hmLP;				//ライトからみた射影行列
	D3DXHANDLE		m_hmWLPB;			//ライトからみたバイアス適用後(テクスチャ座標)射影行列
	D3DXHANDLE		m_hmB;			//バイアス行列

	D3DXHANDLE		m_hmWorldMatrixArray;			//ボーンの状態記憶のための行列配列
	D3DXHANDLE		m_hiNumBones;						//ブレンディング時のボーン数

	D3DXHANDLE		m_hTex;				//ディティールテクスチャのハンドル
	D3DXHANDLE		m_hShadowTex;		//シャドウテクスチャのハンドル
	D3DXHANDLE		m_hRenderTex;		//レンダリングテクスチャのハンドル

	D3DXHANDLE		m_hvLight;			//光源の陰影処理用ベクトルのハンドル
	D3DXHANDLE		m_hvRealLightPos;	//実際の光源座標のハンドル
	D3DXHANDLE		m_hvRealLightVec;	//実際の光源ベクトルのハンドル
	D3DXHANDLE		m_hvRealCameraPos;	//実際のカメラ座標のハンドル
	D3DXHANDLE		m_hvRealCameraVec;	//実際のカメラベクトルのハンドル

	D3DXHANDLE		m_hvLightAmb;		//環境光のハンドル
	D3DXHANDLE		m_hvLightDif;		//拡散反射光のハンドル
	D3DXHANDLE		m_hvEyePos;			//カメラの位置ベクトル

	//減衰率の計算用変数
	D3DXHANDLE		m_hDecay_c0;
	D3DXHANDLE		m_hDecay_c1;
	D3DXHANDLE		m_hDecay_c2;

	D3DXHANDLE		m_hsBaias;			//バイアス補正値
	D3DXHANDLE		m_hShadowLange;			//バイアス補正値

	LPD3DXEFFECT	m_pEffect;			//シェーダ

	//コントラスタ
	HLSLShader();

	//HLSL読み込み
	HRESULT InitHLSL(LPCTSTR filename,LPDIRECT3DDEVICE9 pD3DDev);

	//----------設定関係------------
	//ライト
	VOID SetLightCount(int num);															//ライトの個数を指定する
	VOID SetLightPos(int num,D3DXVECTOR4 *a_vLightPos){m_vLightPos[num] = *a_vLightPos;}    //ライトの位置を指定する(定数指定)
	VOID SetLightVec(int num,D3DXVECTOR4 *a_vLightVec){m_vLightVec[num] = *a_vLightVec;}    //ライトの位置をベクトルする(定数指定)
	VOID SetLightDecay(int num,float c0,float c1,float c2){m_dacay_c0=c0;m_dacay_c1=c1;m_dacay_c2=c2;}    //ライトの減衰率値(定数指定)
	VOID SetNowLight(int num){m_NowLight = num;}											//いまのライトを決定する。
	//カメラ
	VOID SetCameraPos(D3DXVECTOR4 *a_vCameraPos){m_vCameraPos = *a_vCameraPos;}		//ライトの位置を指定する(定数指定)
	VOID SetCameraVec(D3DXVECTOR4 *a_vCameraVec){m_vCameraVec = *a_vCameraVec;}		//ライトの位置をベクトルする(定数指定)
	//------------------------------


	//シェーダ処理を始める
	VOID BeginShader();

	//シェーダ処理を終了する
	VOID EndShader();

	//----------読み込み系----------
	//ライト
	int			GetLightNum(){return m_LightNum;}						//ライトの数
	D3DXVECTOR4 GetNowLightPos(){return m_vLightPos[m_NowLight];}			//今のライトの位置
	D3DXVECTOR4 GetNowLightVec(){return m_vLightVec[m_NowLight];}		//今のライトのベクトル
	D3DXMATRIX  GetNowLightProj(){return m_matLightProj[m_NowLight];}	//今のライトの射影行列
	D3DXMATRIX  GetNowLightView(){return m_matLightView[m_NowLight];}	//今のライトのビュー行列
	float				GetNowDecayC0(){return m_dacay_c0;}							//今の減衰率値c0を返す
	float				GetNowDecayC1(){return m_dacay_c1;}							//今の減衰率値c1を返す
	float				GetNowDecayC2(){return m_dacay_c2;}							//今の減衰率値c2を返す
	//カメラ
	D3DXVECTOR4 GetCameraPos(){return m_vCameraPos;}					//カメラの位置
	D3DXVECTOR4 GetCameraVec(){return m_vCameraVec;}					//カメラのベクトル
	D3DXMATRIX   GetCameraProj(){return m_matCameraProj;}				//カメラの射影行列
	D3DXMATRIX   GetCameraView(){return m_matCameraView;}				//カメラのビュー行列
	//------------------------------

	// スクリーン座標をワールド座標に変換
	D3DXVECTOR3* CalcScreenToWorld(
	   D3DXVECTOR3* pout,
	   float Sx,  // スクリーンX座標
	   float Sy,  // スクリーンY座標
	   float fZ  // 射影空間でのZ値（0～1）
	); 

	//開放処理
	VOID Close();

};
typedef HLSLShader* LPTKYHLSL;


static D3DXMATRIXA16 MakeWorldMatrix(float a_x,float a_y,float a_z,float a_rad)
{

}