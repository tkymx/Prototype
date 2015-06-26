
#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "directx.h"

//FPS�^�C�}�[�̔ԍ�
#define TIMER_FPS 100

//FPS�Œ�
#define LOOK_FPS (80.0f)



#define MAX_LOADSTRING 100
//�A�v���P�[�V����
CLASS_POINTA( IWindowsApplicationLayer , LPTkyWindowsApplicationLayer ) :public IApplicationLayer
{
//	friend class IApplicationLayer;

private:

	//�ϐ�
	UINT			m_looptime	;	//���[�v�^�C��
	UINT			m_fps			;	//FPS
	TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

	//�E�B���h�E�̃T�C�Y
	UINT m_WindowWidth;
	UINT m_WindowHeight;

	// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
	ATOM									MyRegisterClass(HINSTANCE hInstance);
	BOOL									InitInstance(HINSTANCE, int);
	STATIC LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	STATIC INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

	//Window���
	HINSTANCE	m_hInst;								
	HWND		m_hWnd;

protected:

	IWindowsApplicationLayer();
	IWindowsApplicationLayer(const IWindowsApplicationLayer& );
	VIRTUAL ~IWindowsApplicationLayer();

	//���z��
	VIRTUAL	BOOL			pOnCreate()=0;
	VIRTUAL	BOOL			pOnUpdate()=0;
	VIRTUAL	VOID			pOnEvent(TEventMessage msg)=0;

public:

//WindowView�v�f

	//�ݒ�
	VOID SetWindowInfo(HINSTANCE hInstance , UINT w, UINT h);

	//�擾
	HWND GetHWnd()const{return m_hWnd;}
	HINSTANCE GetHInstance()const{return m_hInst;}
	UINT GetFps()const{return m_fps;}
	Vector GetWindowsSize()const{return Vector( (float)m_WindowWidth , (float)m_WindowHeight , 0 );}

//GameAppllication�v�f

	//�X�V����
	VIRTUAL BOOL onLoopStep();

	//������
	VIRTUAL BOOL onCreate();

//�C�x���g�v�f

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );

	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str );

};



