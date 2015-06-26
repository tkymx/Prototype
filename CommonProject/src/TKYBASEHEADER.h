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

//�֗��}�N��
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

//�C�v�V����
#define EPSILON std::numeric_limits<float>::epsilon()
inline bool ISEPSILON(float x,float y)
{
	//�C�v�V������r
	return ( abs(x - y) < EPSILON );
}

//�n���h���̒�`
#define CREATE_HANDLE(name) struct name##__{unsigned int num;};typedef name##__ name;
//�|�C���^��Ԃ̍쐬
#define TYPEDEF(name,pname) typedef name pname##;

//���ׂĂ̊�N���X
CLASS(ITkyObject);
TYPEDEF(ITkyObject*,LPTKYOBJECT)

//�I�u�W�F�N�g��p
#define CLASS_OBJECT(x) class x : public ITkyObject
//��{�d�l
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

//�N���e�B�J���Z�N�V����
#define ENTER_SECTION(m,d) EnterCriticalSection(&m);{d}LeaveCriticalSection(&m);

//���b�Z�[�W�{�b�N�X
#define MSGBOX(text,hed) MessageBoxW(NULL,TEXT(text),TEXT(hed),NULL);

//--------------------------------------------------------------------------------------------------------------
//			TKY�V�X�e��ROOT�N���X�̃��C���w�b�_�[
//--------------------------------------------------------------------------------------------------------------
//�I�u�W�F�N�g�N���X
CLASS(ITkyObject)
{
//--�ÓI--
private:

public:

	//��J��
	bool DestroyOwnObject();

	//�����Ń��[��
	VOID ForceDelete();

	//���ׂĂ̏��߂ɌĂ�
	STATIC VOID CreateObject();
	//���ׂĉ��(�Ō�ɂ�����Ăяo���悤�ɂ�����)
	STATIC VOID DestroyObject();

//--���I--
public:
	//�ʉ��(�ύX���Ȃ����肱�ꂪ����錾)
	VOID Release();
protected:
	ITkyObject();		//�I�u�W�F�N�g�̒ǉ�������
	VIRTUAL ~ITkyObject();
};