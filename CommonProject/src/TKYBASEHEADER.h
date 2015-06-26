#pragma once

#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_SECURE_NO_WARNINGS


#include <string>

#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <queue>
#include <assert.h>

using namespace std;

#define IF(x) if(x)
#define EIF(x) else if(x)
#define FOR(x) for(x)
#define FOR_I(x) for(unsigned int i=0;i<x;i++)
#define FOR_J(x) for(unsigned int j=0;j<x;j++)
#define FOR_K(x) for(unsigned int k=0;k<x;k++)
#define SWITCH(x) switch(x)
#define WHILE(x) while(x)
#define CLASS(x) class x
#define CLASSIN(x,y) class x :public y
#define STRUCT(x) struct x
#define STATIC static 
#define VIRTUAL virtual 

#define IF_FAILED(x) if(FAILED(x))

//便利マクロ
#define SIGN(x) (  (x==0) ? (0) : ( (  (x<0) ? (-1) : (+1)  ) )  )

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#ifndef V
#define V(x)           { hr = x; }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return hr; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p!=NULL) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p!=NULL) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_INPUT
#define SAFE_INPUT(p,q)      { if(p==NULL) { (p)=(q); } }
#endif

//イプシロン
#define EPSILON std::numeric_limits<float>::epsilon()
inline bool ISEPSILON(float x,float y)
{
	//イプシロン比較
	return ( abs(x - y) < EPSILON );
}

//ハンドルの定義
#define CREATE_HANDLE(name) struct name##__{unsigned int num;};typedef name##__ name;
//ポインタ状態の作成
#define TYPEDEF(name,pname) typedef name pname##;

//すべての基幹クラス
CLASS(ITkyObject);
TYPEDEF(ITkyObject*,LPTKYOBJECT)

//オブジェクト専用
#define CLASS_OBJECT(x) class x : public ITkyObject
//基本仕様
#define DEFAULT_OBJECT_RELEASE()		\
VOID Release(){delete(this);}			\

#define CLASS_POINTA_OBJECT(name,pointa) \
	CLASS(name);				\
	TYPEDEF(name,*pointa);	\
	CLASS_OBJECT(name)		\

#define CLASS_POINTA(name,pointa) \
	CLASS(name);				\
	TYPEDEF(name,*pointa);	\
	CLASS(name)		\

//クリティカルセクション
#define ENTER_SECTION(m,d) EnterCriticalSection(&m);{d}LeaveCriticalSection(&m);

//メッセージボックス
#define MSGBOX(text,hed) MessageBoxW(NULL,TEXT(text),TEXT(hed),NULL);

//--------------------------------------------------------------------------------------------------------------
//			TKYシステムROOTクラスのメインヘッダー
//--------------------------------------------------------------------------------------------------------------
//オブジェクトクラス
CLASS(ITkyObject)
{
//--静的--
private:

public:

	//一つ開放
	bool DestroyOwnObject();

	//強制でリーと
	VOID ForceDelete();

	//すべての初めに呼ぶ
	STATIC VOID CreateObject();
	//すべて解放(最後にこれを呼び出すようにしたい)
	STATIC VOID DestroyObject();

//--動的--
public:
	//個別解放(変更しない限りこれが解放宣言)
	VOID Release();
protected:
	ITkyObject();		//オブジェクトの追加をする
	VIRTUAL ~ITkyObject();
};