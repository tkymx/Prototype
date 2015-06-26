#pragma once

#include "TKYBASEHEADER.h"
#include "TKY3D.h"
#include "TKYLIBRARY.h"

/*
CVectorクラス

Vectorの位置を示している

それを使い

・ベクトルの要素取得
bool GetPoint( Vector* , UINT , Vector);
引数　取得した座標情報、指定情報、値

・　分割ベクトルの取得
bool CutVector( LPVECTOR , LPVECTOR , UINT , float );
引数　分割後の一つ目、二つ目、分割座標、値
返却値　成功したかどうか
TKYVECTOR_CUT_X,TKYVECTOR_CUT_Y,TKYVECTOR_CUT_Z

・　接触を判定
bool CollisionVector( LPVECTOR ,  Vector* )
引数　LPCVECTOR , 接触点取得のベクトル
返却値 あたったかどうか
おもに２Dで使われるベクトル間の接触判定

*/

CLASS(CVector);
TYPEDEF(CVector,*LPVECTOR)

enum EM_Cvector{CVECTOR_X , CVECTOR_Y , CVECTOR_Z };


CLASS(CVector)
{
private:
	Vector m_Start;
	Vector m_End;
public:

	//コンストラクタ
	CVector();
	CVector(Vector,Vector);
	VIRTUAL ~CVector();

	//ベクトルの代入
	void	SetVector(Vector s ,Vector e){m_Start = s;m_End = e;};
	void	SetVectorStart(Vector s){m_Start = s;};
	void	SetVectorEnd(Vector e){m_End = e;};
	//ベクトルの取得
	Vector GetVector(){return m_End-m_Start;}
	VOID	  GetVector(Vector* s,Vector* e){*s = m_Start;*e = m_End;}
	Vector GetVectorStart(){return m_Start;};
	Vector GetVectorEnd(){return m_End;};

	//ベクトルの要素取得
	bool GetPoint( Vector* , EM_Cvector , float);
	//ベクトルの切断
	bool CutVector( LPVECTOR , LPVECTOR , EM_Cvector , float );
	//ベクトルの衝突
	bool CollisionVector( LPVECTOR ,  Vector* );

	//ベクトルの中間取得
	Vector GetVectorMiddle(FLOAT t)
	{
		return m_Start + (m_End-m_Start) * t;
	}

	//描画
	bool DrawLineSet(LPTKY3D l3,Vector color,float thick);

	//演算子
	CVector operator+ (const Vector &v){return CVector(m_Start+v,m_End+v);}

};