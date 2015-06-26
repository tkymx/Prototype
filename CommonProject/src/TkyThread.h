#pragma once

#include "TKYBASEHEADER.h"
#include <process.h>
#include "TKYLIBRARY.h"

/*

	�X���b�h�N���X�̒�`
	
	RUN�X���b�h�̎��s

	onUpdate���p�����邱�Ƃɂ���ăX���b�h���쐬�ł���

	Stop�X���b�h�̏I��

*/

enum eThreadParam
{ 
	TKYTHREAD_NONE_LOOP , 
	TKYTHREAD_PAUSE_LOOP , 
	TKYTHREAD_ONCE_LOOP , 
	TKYTHREAD_ENDLESS_LOOP 
};
CLASS_POINTA_OBJECT( ITkyThread , LPTkyThread )
{
private:

	//�����p�ϐ�
	STATIC UINT g_ThreadNum;

	//�X���b�h�p�����[�^�[
	eThreadParam m_etp;
	//�X���b�h�n���h��
	HANDLE m_handle;
	//�����`���[�p�ÓI�֐�
	STATIC UINT WINAPI pLauncher(LPVOID value);
	//���s
	VOID Execute();


	//�X�V�̉��z��
	VIRTUAL VOID onThreadUpdate()=0;

	//�N���e�B�J���Z�N�V����
	CRITICAL_SECTION m_critical;

protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	ITkyThread();
	VIRTUAL ~ITkyThread();

public:
	//�X���b�h�̎��s
	VOID Run( eThreadParam param );

	//�����I��(�G���h���X��)
	VOID Stop();

	//�ꎞ��~(�G���h���X��)
	VOID Pause();

	//�X�^�[�g(�G���h���X��)
	VOID Start();

	//��Ԏ擾
	eThreadParam GetParam(){ return m_etp; }

	//�n���h���̎擾
	HANDLE GetHandle()const{return m_handle;}

	//���̎擾(�ÓI)
	STATIC UINT GetThreadNum(){return g_ThreadNum;}

};