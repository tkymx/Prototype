#pragma once

#include<d3dx9.h>
#include<vector>
#include <mmsystem.h>
#include <assert.h>
#include "TKYBASEHEADER.h"
#include "TKYHLSL.h"
#include "TKY3D.h"
#include "Anime.h"

static const UINT MAP_SIZE = 2048;
#define _FOR_I(x) for( UINT i=0;i<x;i++ )


//レンダリングパス（テクニック）
enum Rendering{
	D3DTKY_TEXTURE		,
	D3DTKY_WHITE		,
	D3DTKY_DEPTH		,
	D3DTKY_SHADOW		,
	D3DTKY_SAVE_SHADOW	,
	D3DTKY_SHADOW_PLUS	,
	D3DTKY_SHADOW_SPOT	,
	D3DTKY_ONLY_TEXTURE	
};


//========== 新しい内容 ==========

CLASS_POINTA_OBJECT( CXFile , LPTkyXFile )
{
private:
	//デバイス情報
	LPTKY3D m_device;


	//Xファイル読み込みインターフェイス
//	LPD3DXMESH				m_pMesh;				//メッシュデータを格納する
	D3DMATERIAL9			*m_pMeshMaterials;		//マテリアルを格納する
	LPDIRECT3DTEXTURE9		*m_pMeshTextures;		//テクスチャを格納する
	DWORD					m_dwNumMaterials;		//マテリアルの数を格納する

	//Xファイル読み込みインターフェイス
	LPD3DXMESH				m_pMesh;				//メッシュデータを格納する
	AnimetionClass			anime;					//アニメーションクラス

	//コンストラクタ
	CXFile(LPTKY3D d);
	CXFile(const CXFile&);
	VIRTUAL ~CXFile();
public:

	//Xファイルの設定
	HRESULT SetXFileW(wstring name);
	//表示
	HRESULT DrawMesh( Vector pos , Vector rotate );
	
	//フレームの設定
	VOID SetAnimeFreem(FLOAT freem);

	//レイのあたり判定
	BOOL CollisionLay( Vector laypos, Vector layvec , FLOAT* length , Vector xp = Vector() , Vector xr = Vector()  );

	//設定
	VOID SetDevice(LPTKY3D d){m_device=d;}
	VOID SetMesh(LPD3DXMESH x){m_pMesh=x;}
	//取得
	LPTKY3D						GetDevice()const{return m_device;}
	LPD3DXMESH					GetMesh()const{return m_pMesh;}
	D3DMATERIAL9*				GetMaterial()const{return m_pMeshMaterials;}				//マテリアルを格納する
	LPDIRECT3DTEXTURE9*	GetTexture()const{return m_pMeshTextures;}				//テクスチャを格納する
	DWORD							GetMaterialNum()const{return m_dwNumMaterials;}	//マテリアルの数を格納する
	AnimetionClass*				GetAnime(){return &anime;}								//アニメーションクラス

	//生成
	STATIC LPTkyXFile CreateXFile(LPTKY3D d);
	STATIC LPTkyXFile CreateXFile(LPTKY3D d,wstring name);

};


//========== 古い内容 ==========

//--------------------------------------------------//
//Xファイルを読み込み
//--------------------------------------------------//
class XfileLoad{

private:

	LPDIRECT3D9				m_pD3D;					//DirectX9　インターフェイス
	LPDIRECT3DDEVICE9	m_pD3DDevice;		//DirectX9　インターフェイスのデバイス
	HLSLShader				*m_pHLSL;				//HLSL情報

	//Xファイル読み込みインターフェイス
	LPD3DXMESH				m_pMesh;				//メッシュデータを格納する
	AnimetionClass			anime;					//アニメーションクラス

	//プライベート関数
	void pPrepara(float a_x,float a_y,float a_z,float a_rad,int a_pass);
	void pDrawMesh(D3DMATERIAL9* ,LPDIRECT3DTEXTURE9,LPD3DXMESH,INT,INT);		//環境光、反射光、テクスチャ、メッシュ , パス , サブセット


public:

	//コンストラクタ
	XfileLoad();
	XfileLoad(LPTKY3D pinit3d,HLSLShader *a_pHLSL);
	VIRTUAL ~XfileLoad();

	//Xファイル読み込み
	HRESULT ReadXFile(LPWSTR filename);

	//完璧に描き込む
	bool	DrawAnimeXFile(float x,float y,float z,float rad,int pass);

	//設定系
	void  SetAnimeFreem(INT);

	//開放する
	void	CleanXFile();

	//取得系
	LPD3DXMESH GetMesh()const{
		return m_pMesh;
	};
	FLOAT GetMaxFreem()const{return anime.NumFreem;};
	AnimetionClass GetAnimetionClass()const{return anime;};

};
//--------------------------------------------------//
//Xファイルアニメーションを読み込み
//--------------------------------------------------//
class D3DTKYXANIMETION
{
private:
	//取得する情報
	XfileLoad *m_model;	//モデルのインデックス番号
	DWORD m_time;			//フレームの遷移速度
	//取得しない情報
	int m_maxloop;
	int m_nowloop;		//今のフレーム
	DWORD m_nowtime;	//今の時間
public:
	D3DTKYXANIMETION();
	D3DTKYXANIMETION(const D3DTKYXANIMETION&);
	D3DTKYXANIMETION(XfileLoad*,INT);//基本情報,移動時間
	VIRTUAL ~D3DTKYXANIMETION();
	//描画に必要なもの
	bool NextModel();
	void DrawModel(D3DXVECTOR3 pos,FLOAT rad,INT pass);
	//データ設定系
	void SetNowLoop(INT loop){m_nowloop = loop;}
	//データ取得系
	LPD3DXMESH GetModelMesh();
};