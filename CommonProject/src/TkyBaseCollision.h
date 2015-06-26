#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TKYVECTOR.h"
#include "TkyEvent.h"

//衝突を細かくしたい場合はレートを上げてください

//===================== ベクトル便利関数 =======================

//左回りかどうか
STATIC bool PointDirection2D( Vector t , Vector p , Vector v )
{
	float z = (v*(t - p)).z;
	IF( z < 0.0f )return false;
	return true;
}

//垂直線の取得
STATIC Vector GetVertical( Vector t , Vector v1 , Vector v2 )
{
	//法線を取る
	Vector n = (v1*v2).Normal();

	//ターゲットと法線の位置を見て同じだったらそのまま返す
	IF( PointDirection2D( t , Vector() , v1 ) == PointDirection2D( n , Vector() , v1 ) )
	{
		return n;
	}
	return -1*n;
}

//ベクトルの分解
STATIC Vector DegradeVector( Vector base , Vector n)
{
	return ((n^base)/(n^n))*n;
}

//===================== 基本あたり判定 =======================

//線と線
STATIC bool TkyBaseCollisionLineToLine( Vector *out , LPVECTOR v1 , LPVECTOR v2 )
{
	IF( v1 == NULL )return false;
	IF( v2 == NULL )return false;
	return v1->CollisionVector(v2,out);
}
STATIC bool TkyBaseCollisionLineToLineALL( Vector *out , LPVECTOR v1 , LPVECTOR v2 )
{
	IF( v1 == NULL )return false;
	IF( v2 == NULL )return false;
	return v1->CollisionVector(v2,out);
}

//円と円
STATIC bool TkyBaseCollisionCircleToCircle( Vector *out , Vector *v1 , float r1 , Vector *v2 , float r2 )
{
	IF( v1 == NULL )return false;
	IF( v2 == NULL )return false;

	//中心間ベクトル
	Vector v = *v2 - *v1;

	IF(out!=NULL)*out = *v1 + ( v / 2 );

	//あたったら
	IF( v.GetLarge() < r1+r2 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//線分と円
STATIC bool TkyBaseCollisionLineToCircle( Vector *out , LPVECTOR pos1 , Vector *pos2 , float r2 )
{
	IF( pos1 == NULL )return false;
	IF( pos2 == NULL )return false;

	Vector s1 = *pos2;
	Vector s2 = pos1->GetVectorStart();
	Vector v1 = s1 - s2;
	Vector v2 = pos1->GetVectorEnd() - s2;
	float k = ( v1^v2 )/(v2^v2);

	Vector pv1;
	//はみ出し処理
	IF( k >= 1 )pv1 = (s2+v2)-s1;
	EIF( k <= 0)pv1 =  s2-s1;
	else  pv1 = k*v2 - v1;

	float s = pv1.GetLarge();

	//あたっていたら線上の値を返す	
	IF(out!=NULL)
	{
		*out = s1+pv1;
	}

	//円とのあたり判定
	IF( s < r2 )
	{
		return true;
	}
	else
	{
		return false;
	}
};

//線と速度円
STATIC bool TkyBaseCollisionLineToCircleVel( Vector *out , LPVECTOR v1 , LPVECTOR c1 , float r2 )
{
	IF( v1 == NULL )return false;
	IF( c1 == NULL )return false;

	Vector vv1 = c1->GetVectorStart() - v1->GetVectorStart();
	Vector v2 = v1->GetVector();
	Vector v = c1->GetVector();

	float x = v2^v2;

	Vector a = ( ( v^v2 )/x )*v2 - v ;
	Vector b = ( ( vv1^v2 )/x )*v2 - vv1 ;

	float α = a^a;
	float β = a^b;
	float γ = (b^b) - r2*r2;

	float d = β*β - α*γ;

	//接触していなかったら
	IF( d < 0.0f )return false;

	float t = ( -β - sqrt(d) )/α;

	//Tが範囲内じゃないので終わり
	IF( t <= 0 || t >= 1 )return false;

	*out = c1->GetVectorStart() + t*v;

//	CEventManager::StaticAddEventMessage( "COLLISION_POS" , (DWORD)out );
//	CEventManager::StaticAddEventMessage( "COLLISION_VEC" , (DWORD)c1 );

	*out += GetVertical( *out - v1->GetVectorStart() , v1->GetVector() , v1->GetVector()*c1->GetVector() ).Normal() * -r2;

	IF(TkyBaseCollisionLineToCircle( out , v1 , out , r2 ))
	{

//		CEventManager::StaticAddEventMessage( "COLLISION_POS" , (DWORD)out );

		return true;
	}

	return false;
};


//点と線分への垂線の接触点を取得する(垂直方向の接触位置を返す)
STATIC bool TkyBaseCollisionPointToLine( Vector* out , Vector* point , LPVECTOR line )
{
	IF(point==NULL)return false;
	IF(line == NULL)return false;
	Vector v = *point - line->GetVectorStart();
	Vector n = line->GetVectorEnd()-line->GetVectorStart();

	//点の時
	IF(n.GetLarge() <= 0.0000001f)
	{
		*out = line->GetVectorStart();
		return false;
	}

	float k = ( n^v ) / (n^n);
	IF(out!=NULL)*out = line->GetVectorStart() + k*n;
	IF( k<0 || k>1 )return false;
	return true;
}