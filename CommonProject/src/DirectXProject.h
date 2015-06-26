#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyWindowsApplication.h"

#include "DirectXProjectViews.h"
#include "DataStructure.h"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���W�b�N
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CDirectXGameLogic , LPTkyDirectXGameLogic ) : public IGameLogic
{
private:
	//���E���
	LPTkyPhysicsWorld3D m_world3d;
	//�R���X�g���N�^�E�f�X�g���N�^
	CDirectXGameLogic();
	VIRTUAL ~CDirectXGameLogic();
public:
//GameLogic���
	//������
	VIRTUAL BOOL onCreate() ;
	//�X�V( View�X�V�������� )
	VIRTUAL VOID onUpdate(float step);
	//View�ݒ莞(View���͎����I�Ƀ^�X�N�ɒǉ������̂ł��̑��̐ݒ�)
	VIRTUAL VOID onAddView( LPTkyGameView view );
//�C�x���g���
	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "DirectXGame";
	}
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg );
//�Ǝ����
	//����
	STATIC LPTkyDirectXGameLogic CreateDirectXGameLogic();
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectX�e�X�g�p
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( CDirectXApplicationLayer , LPTkyDirectXApplicationLayer ) :public IWindowsApplicationLayer
{
private:
//DirectX���

	//��{�N���X
	LPTKY3D m_pDtata;
	//�����\���N���X
	LPTKYFONT m_pFont;
	//�f�o�C�X�̎擾
	LPTKYINPUT m_device;
	//��
	LPTkyEnvironment m_envi;

//���̑��G�ϐ�

	//����
	Vector m_pos;
	//���_
	Vector m_vec;
	//FPS���
	TFPSInfo m_Info;
	//�}�E�X���
	DIMOUSESTATE m_mouse;
	//�Q�[�����W�b�N
	LPTkyDirectXGameLogic m_logic;

//DirectXApplication�v�f

private:

	//�R���X�g���N�^�E�f�X�g���N�^
	CDirectXApplicationLayer();
	VIRTUAL ~CDirectXApplicationLayer();

	//�L�[�̎擾
	VOID				pKeyBoard();
public:
	STATIC LPTkyDirectXApplicationLayer CreateDirectXApplicationLayer();

//WindowsApplication�v�f	

private:
	VIRTUAL BOOL			pOnCreate();
	VIRTUAL BOOL			pOnUpdate();
//�C�x���g�v�f
private:
	VIRTUAL	VOID			pOnEvent( TEventMessage msg );
};