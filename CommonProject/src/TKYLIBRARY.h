#pragma once

#include<d3dx9.h>
#include<vector>
#include<map>
#include<fstream>
#include "TKYBASEHEADER.h"

//---------------------------------------------------
//ベクトル計算
//---------------------------------------------------
//ベクトル制御
class Vector : public D3DXVECTOR3
{
public:

	Vector(){x=y=z=0;};
	Vector(float _x,float _y,float _z){x=_x;y=_y;z=_z;};

	Vector(D3DXVECTOR3 v3){x=v3.x ;y=v3.y ;z=v3.z;};

	VOID	operator =	(const Vector &v)	{x=v.x;y=v.y;z=v.z;}
	VOID	operator =	(const D3DXVECTOR3 v3){x=v3.x;y=v3.y;z=v3.z;}
	bool	operator == (const Vector &v)	{if(x==v.x&&y==v.y&&z==v.z)return true;else return false;}
	bool	operator != (const Vector &v)	{if(x!=v.x||y!=v.y||z!=v.z)return true;else return false;}
	Vector	operator +	(const Vector &v)	{return Vector(x+v.x,y+v.y,z+v.z);}
	void	operator += (const Vector &v)	{x+=v.x;y+=v.y;z+=v.z;}
	Vector	operator -	(const Vector &v)	{return Vector(x-v.x,y-v.y,z-v.z);}
	void	operator -= (const Vector &v)	{x-=v.x;y-=v.y;z-=v.z;}
	Vector	operator *	(const Vector &v)	{return Vector(y*v.z-z*v.y , z*v.x-x*v.z , x*v.y-y*v.x);}
	void	operator *= (const Vector &v)	{Vector vv =Vector(y*v.z-z*v.y , z*v.x-x*v.z , x*v.y-y*v.x);x=vv.x;y=vv.y;z=vv.z;}
	Vector	operator *	(const float i)		{return Vector((float)(x*i),(float)(y*i),(float)(z*i));}
	void	operator *= (const float i)		{x=(float)(x*i);y=(float)(y*i);z=(float)(z*i);}
	Vector	operator /	(const float i)		{return Vector((float)(x/i),(float)(y/i),(float)(z/i));}
	void	operator /= (const float i)		{x=(float)(x/i);y=(float)(y/i);z=(float)(z/i);}
	Vector	operator /	(const Vector &v)	{return Vector( x/v.x , y/v.y , z/v.z );}
	void	operator /= (const Vector &v)	{x/=v.x; y/=v.y; z/=v.z;}
	float	operator ^	(const Vector &v)	{return x*v.x+y*v.y+z*v.z;}
	float	GetRad()	{return atan2((float)y,(float)x);};
	Vector	GetRadVec()	{return Vector(atan2((float)y,(float)z),atan2((float)z,(float)x),atan2((float)y,(float)x));};
	float	GetRad(Vector &v){return (float)D3DXToDegree( acos( ((*this).Normal()^v.Normal()) ) );};
	float	GetLarge(){return sqrt( (float)(x*x+y*y+z*z) );};
	Vector	Normal(){float size = GetLarge();IF(size==0)return Vector();return Vector( x/size, y/size , z/size );};
	Vector	Rotate(Vector vv, float rx,float ry,float rz)
	{
		Vector v;
		Vector tmp = *this-vv;
		Vector cosv = Vector( cos(D3DXToRadian(rx)),cos(D3DXToRadian(ry)),cos(D3DXToRadian(rz)) );
		Vector sinv = Vector( sin(D3DXToRadian(rx)),sin(D3DXToRadian(ry)),sin(D3DXToRadian(rz)) );
		//X回転
		v.x = tmp.x;
		v.y = tmp.y*cosv.x-tmp.z*sinv.x;
		v.z = tmp.y*sinv.x+tmp.z*cosv.x;
		tmp = v;
		//Y回転
		v.x = tmp.x*cosv.y + tmp.z*sinv.y;
		v.y = tmp.y;
		v.z = -tmp.x*sinv.y + tmp.z*cosv.y;
		tmp=v;
		//Z回転
		v.x = tmp.x*cosv.z - tmp.y*sinv.z;
		v.y = tmp.x*sinv.z + tmp.y*cosv.z;
		v.z = tmp.z;
		return v;
	}
	bool	Move(Vector &v,Vector s){
		Vector vv =Vector(v.x-x,v.y-y,v.z-z);
		if( abs(vv.GetLarge()) < s.GetLarge() )
		{
			*this = v;
			return true;
		}
		else
		{
			*this+=(vv.Normal()*s.GetLarge());
		}
		return false;
	}
	Vector	Expand(Vector pos,Vector expand)
	{
		Vector v = *this - pos;
		return Vector( v.x*expand.x , v.y*expand.y , v.z*expand.z );
	}
};


//ベクトル制御
template <class T>
class VectorMulti : public D3DXVECTOR3
{
public:
	T x,y,z;

	VectorMulti(){x=y=z=0;};
	VectorMulti(T _x,T _y,T _z){x=_x;y=_y;z=_z;};
	bool	  operator == (VectorMulti &v){if(x==v.x&&y==v.y&&z==v.z)return true;else return false;}
	VectorMulti operator + (VectorMulti &v){return VectorMulti(x+v.x,y+v.y,z+v.z);}
	void	  operator += (VectorMulti &v){x+=v.x;y+=v.y;z+=v.z;}
	VectorMulti operator - (VectorMulti &v){return VectorMulti(x-v.x,y-v.y,z-v.z);}
	void	  operator -= (VectorMulti &v){x-=v.x;y-=v.y;z-=v.z;}
	VectorMulti operator * (VectorMulti &v){return VectorMulti(y*v.z-z*v.y , z*v.x-x*v.z , x*v.y-y*v.x);}
	void operator *= (VectorMulti &v){VectorMulti vv =VectorMulti(y*v.z-z*v.y , z*v.x-x*v.z , x*v.y-y*v.x);x=vv.x;y=vv.y;z=vv.z;}
	VectorMulti operator * (float i){return VectorMulti((T)(x*i),(T)(y*i),(T)(z*i));}
	void	  operator *= (float i){x=(T)(x*i);y=(T)(y*i);z=(T)(z*i);}
	float	GetRad(){return atan2((float)y,(float)x);};
	float	GetLarge(){return sqrt( (float)(x*x+y*y+z*z) );};
	VectorMulti<float> Normal(){float size = GetLarge();return VectorMulti<float>( x/size, y/size , z/size );};
	bool	Move(VectorMulti &v,float s){
		VectorMulti vv =VectorMulti(v.x-x,v.y-y,v.z-z);
		if( vv.GetLarge() < s )
		{
			x=v.x;y=v.y;z=v.z;
			return true;
		}
		else
		{
			x+=(T)(vv.Normal().x*s);
			y+=(T)(vv.Normal().y*s);
			z+=(T)(vv.Normal().z*s);
			return false;
		}
	}
};


//---------------------------------------------------
//ユーティリティ関数
//---------------------------------------------------
//ベクタ配列変換
template< class T >
void VectorToScalar(T* &_array , vector<T> _varray )
{
	_array = new T[_varray.size()];
	for( UINT d=0;d<_varray.size();d++ )_array[d] = _varray[d];	
}

//範囲内かどうか( 大小 )
template< class T >
bool IsRange( T t1 , T t2 , T x )
{
	//大小
	IF( t1 <= x && t2 >= x )
	{
		return true;
	}
	//小大
	IF( t1 >= x && t2 <= x )
	{
		return true;
	}
	return false;
}

//回転軸のセット
static D3DXMATRIXA16 SetDeviceMatrix( LPDIRECT3DDEVICE9 device , Vector pos , Vector rotate = Vector() , Vector expand = Vector() )
{
		//座標セット
		D3DXMATRIXA16 TransMat,RotateMat,ExpandMat;						//座標変換行列	
		D3DXMatrixIdentity(&TransMat);									//行列の初期化
		D3DXMatrixIdentity(&RotateMat);									//行列の初期化
		//
		D3DXMatrixTranslation(&TransMat,pos.x,pos.y,pos.z);							//座標行列移動			
		D3DXMatrixRotationX(&RotateMat,rotate.x);//timeGetTime()/500.0f);	//y軸回転の行列にする
		D3DXMatrixRotationY(&RotateMat,rotate.y);//timeGetTime()/500.0f);	//y軸回転の行列にする
		D3DXMatrixRotationZ(&RotateMat,rotate.z);//timeGetTime()/500.0f);	//y軸回転の行列にする

		D3DXMatrixScaling(&ExpandMat,expand.x,expand.y,expand.z);//timeGetTime()/500.0f);	//y軸回転の行列にする

		RotateMat = ExpandMat * RotateMat * TransMat;									//行列の合体				 

		device->SetTransform( D3DTS_WORLD , &RotateMat );	//座標行列セット

		return RotateMat;
}
//回転軸の取得
static D3DXMATRIXA16 GetWorkdMatrix( Vector pos , Vector rotate )
{
		//座標セット
		D3DXMATRIXA16 Mat,TransMat,RotateMat[3];						//座標変換行列	
		D3DXMatrixIdentity(&Mat);									//行列の初期化
		D3DXMatrixIdentity(&TransMat);									//行列の初期化
		D3DXMatrixIdentity(&RotateMat[0]);									//行列の初期化
		D3DXMatrixIdentity(&RotateMat[1]);									//行列の初期化
		D3DXMatrixIdentity(&RotateMat[2]);									//行列の初期化
		//
		
		D3DXMatrixTranslation(&TransMat,pos.x,pos.y,pos.z);							//座標行列移動			
		D3DXMatrixRotationX(&RotateMat[0],( rotate.x ));
		D3DXMatrixRotationY(&RotateMat[1],( rotate.y ));
		D3DXMatrixRotationZ(&RotateMat[2],( rotate.z ));
		Mat = RotateMat[0] * RotateMat[1] * RotateMat[2] * TransMat;									//行列の合体				 
		return Mat;
}

//--------------------------------------------------//
//　クラス　CStringMatch< T >
//　説明　　文字列を登録して取得する関数
//--------------------------------------------------//
template < class T >
CLASS(CStringMatch)
{
private:
	map<string,T> m_element;
public:
	VOID AddString( string str , T temp ){ m_element.insert( pair<string,T>(str,temp) ); }
	bool		GetElement( string str , T *temp)
	{ 
		map<string,T>::iterator im;
		im = m_element.find(str);
		IF(im == m_element.end() )return false;
		temp = &im->second;
		return  true;
	}
	BOOL Destroy(string name)
	{
		map<string,T>::iterator im = m_element.find( name );
		IF(im == m_element.end() )return false;
		im = m_element.erase(im);
		return true;
	}
	VOID Destroy()
	{
		m_element.clear();
	}
};
template < class T >
CLASS(CStringMatchW)
{
private:
	map<wstring,T> m_element;
public:
	VOID AddString( wstring str , T temp ){ m_element.insert( pair<wstring,T>(str,temp) ); }
	bool		GetElement( wstring str , T **temp)
	{ 
		map<wstring,T>::iterator im;
		im = m_element.find(str);
		IF(im == m_element.end() )
		{
			MessageBox(NULL,str.c_str(),TEXT("ないよ"),NULL);
			return false;
		}
		*temp = &im->second;
		return  true;
	}
	BOOL Destroy(wstring name)
	{
		map<wstring,T>::iterator im = m_element.find( name );
		IF(im == m_element.end() )return false;
		im = m_element.erase(im);
		return true;
	}
	VOID Destroy()
	{
		m_element.clear();
	}
};
template < class T >
CLASS(CNumberMatch)
{
private:
	map<INT,T> m_element;
public:
	map<INT,T> GetElement(){return m_element;};
	VOID AddString( INT n , T temp ){ m_element.insert( pair<INT,T>(n,temp) ); }
	bool		GetElement( INT n , T **temp)
	{ 
		map<INT,T>::iterator im;
		im = m_element.find(n);
		IF(im == m_element.end() )
		{
			MessageBox(NULL,TEXT("CNumberMatch NUMBER"),TEXT("ないよ"),NULL);
			return false;
		}
		*temp = &im->second;
		return  true;
	}
	vector<T*> GetElement( INT sn , INT en )
	{
		vector<T*> out;

		map<INT,T>::iterator im;
		//範囲だけループ
		FOR( int i = sn ; i <= en ; i++ )
		{
			im = m_element.find(i);
			IF(im == m_element.end() )
			{
				WCHAR str[MAX_PATH];
				wsprintf( str , TEXT("Number %d TO %d "),sn,en );
				MessageBox(NULL,str,TEXT("ないよ"),NULL);
			}
			else
			{
				//要素があったら追加
				out.push_back(&im->second);
			}
		}
		return  out;
	}
	BOOL Destroy(INT n)
	{
		map<INT,T>::iterator im = m_element.find( n );
		IF(im == m_element.end() )return false;
		im = m_element.erase(im);
		return true;
	}
	VOID Destroy()
	{
		m_element.clear();
	}
};


//--------------------------------------------------//
//　クラス　CTkyFileRead
//　説明　　ファイル読み込み関数
//--------------------------------------------------//
CLASS( CTkyFileRead )
{
private:
	wifstream m_read;
public:
	bool Open(wstring name)
	{
		m_read.open(name);
		return !m_read.fail();
	}
	VOID Close()
	{
		m_read.close();
	}

	wstring GetToLine()						//一行読み込み(260文字分か　一行全部　EOFまで)
	{
		WCHAR w[MAX_PATH];
		m_read.getline( w , MAX_PATH );
		return wstring(w);
	}
	wstring GetToWhiteSpace()			//ホワイトスペースまで読み込み
	{
		wstring w;
		m_read >> w;
		return w;
	}
	wstring GetToElement(WCHAR _w)		//ある文字まで読み込み	
	{
		wstring ws;
		WHILE(!m_read.eof())
		{
			WCHAR w;
			//文字読み込み
			w = m_read.get();
			//文字を比べる同じだったr終了
			IF( w == _w )break;
			//違ったら文字列格納
			ws += w;
		}
		return ws;
	}
	wstring GetToElementFromElement(WCHAR sw,WCHAR ew)		//ある文字まで読み込み	
	{
		GetToElement( sw );
		return GetToElement( ew );
	}

	bool IsEof(){return m_read.eof();}
};



//--------------------------------------------------//
//　クラス　CTkyFileRead
//　説明　　ファイル読み込み関数
//--------------------------------------------------//
CLASS( CTkyFileReadCW )
{
private:
	FILE* m_read;

	VOID WM( wstring str , char * c, int  n = MAX_PATH)
	{
		size_t t=0;
		wcstombs(c,str.c_str(), str.length() );
		c[str.length()] = '\0';
	}
	wstring MW( char * c, int  n = MAX_PATH)
	{
		WCHAR str[MAX_PATH] = {TEXT('\0')};
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,c,n,str,MAX_PATH);
		str[ string(c).size() ] = TEXT('\0');
		str[ string(c).size()+1 ] = TEXT('\0');
		return wstring(str);
	}

public:
	BOOL Open(wstring name)
	{
		char str[MAX_PATH];
		WM(name,str);

		errno_t t;

		t = fopen_s( &m_read , str , "r" );

		return (m_read != NULL);
	}
	VOID Close()
	{
		fclose(m_read);
	}

	wstring GetToLine()						//一行読み込み(260文字分か　一行全部　EOFまで)
	{
		CHAR str[MAX_PATH];
		fgets( str , MAX_PATH , m_read ); 
		return MW( str );
	}
	wstring GetToWhiteSpace()			//ホワイトスペースまで読み込み
	{
		CHAR str[MAX_PATH];
		IF(fscanf( m_read , "%s" , str ) == EOF)
		{
//			MessageBoxA( NULL , " GetToWhiteSpaceでエラーだよもういっかい起動してね "  , "" , NULL );
		}
		return MW(str);
	}
	wstring GetToElement(WCHAR _w)		//ある文字まで読み込み	
	{
		wstring wstr;
		wstr += _w;
		char s[10];
		WM(wstr , s , 10);

		string str;
		char c;

		//文字読み込み
		WHILE( (c = fgetc(m_read)) != EOF )
		{
			//文字を比べる同じだったr終了
			IF( c == s[0] )break;
			//違ったら文字列格納
			str += c;
		}

		return MW( const_cast<char*>(str.c_str()) , str.size() );
	}
	wstring GetToElementFromElement(WCHAR sw,WCHAR ew)		//ある文字まで読み込み	
	{
		GetToElement( sw );
		return GetToElement( ew );
	}

	BOOL IsEof(){return feof(m_read);}
};