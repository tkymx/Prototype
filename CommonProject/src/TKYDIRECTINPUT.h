#pragma once

#include<d3dx9.h>
#include <dinput.h>
#include "TKYBASEHEADER.h"

#define KEYDOWN(name,key) (name[key] & 0x80)

CLASS(CTkyInput);
TYPEDEF(CTkyInput,*LPTKYINPUT);

/*
�����ł̓f�o�C�X�֌W�̎擾���s��

SetInputinfo(int type,HINSTANCE a_hIns,HWND a_hWnd);	
�Ńf�o�C�X���Ƃɏ�����������

DIMOUSESTATE2	GetDeviceEvent();					//�}�E�X�̏�Ԃ��擾����
bool					GetKeyBoard(char*);				//�L�[�̏�Ԃ��擾����
�ŃL�[���擾����
�擾����ۂ̓^�C�}�[�ȂǏ�ɒʉ߂���n�_�ɔz�u���邱��

�L�[�{�[�h����KEYDOWN(name,key)
�}�N�����g�p����
key�ɂ�
http://msdn.microsoft.com/ja-jp/library/cc351922.aspx
���Q�Ƃ���

*/

//--------------------------------------------------//
//�f�o�C�X�̑���̍쐬
//--------------------------------------------------//
enum tinput{KEYBORD,MOUSE};	//�f�o�C�X�̎��
CLASS_OBJECT(CTkyInput)
{

private:

	bool	m_IsAcquire;				//�A�N�Z�X���̗L��
	LPDIRECTINPUT8 g_pDI;					//DirectInput�@�́@�n���h��

	LPDIRECTINPUTDEVICE8  g_pKeyDevice;		//�L�[�f�o�C�X�̏���
	LPDIRECTINPUTDEVICE8  g_pMouseDevice;		//�}�E�X�f�o�C�X�̏���

	CTkyInput();						//���������s��
	VIRTUAL ~CTkyInput();					//�f�X�g���N�^

public:



	HRESULT SetInputinfo(HINSTANCE a_hIns,HWND a_hWnd);				//�C���v�b�g�̐ݒ���s��

	DIMOUSESTATE	GetDeviceEvent();					//�}�E�X�̏�Ԃ��擾����
	bool					GetKeyBoard(char buffer[256]);				//�L�[�̏�Ԃ��擾����

	void	SetDeviceAcquire(bool a_Acquire);				//�f�o�C�X�̃A�N�Z�X���̊Ǘ�

	//�������
	VOID Release();		
	static LPTKYINPUT CreateInput(){return new CTkyInput();};

};
