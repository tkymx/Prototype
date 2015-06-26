#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyWindowsApplication.h"
#include "directx.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectX�\����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
STRUCT( TDirectXInfo )
{
	//��{�N���X
	LPTKY3D m_pDtata;
	//�����\���N���X
	LPTKYFONT m_pFont;
	//�f�o�C�X�̎擾
	LPTKYINPUT m_device;
	//��
	LPTkyEnvironment m_envi;
	//�}�E�X���
	DIMOUSESTATE m_mouse;
	//�}�E�X��Ώ��
	Vector m_absmouse;
	//�J�����̈ʒu�\����
	Vector *m_pos;
	//�J�����̌����\����
	Vector *m_vec;

	//������
	VOID Initialize()
	{
		m_pDtata = NULL;
		m_pFont = NULL;
		m_device = NULL;
		m_envi = NULL;
		m_mouse = DIMOUSESTATE();
		m_absmouse = Vector();
		m_pos = NULL;
		m_vec = NULL;
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectX�e�X�g�p
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IDirectXBaseApplicationLayer , LPTkyDirectXBaseApplicationLayer ) :public IWindowsApplicationLayer
{
private:
//DirectX���

	//DirectX�\����
	TDirectXInfo m_info;

	//����
	Vector m_pos;
	//���_
	Vector m_vec;

private:

//�L�[�̎擾
	VOID							pKeyBoard();
//WindowsApplication�v�f	
	VIRTUAL BOOL			pOnCreate();
	VIRTUAL BOOL			pOnUpdate();
//�C�x���g�v�f
	VIRTUAL	VOID			pOnEvent( TEventMessage msg );

protected:

//DirectXApplication�v�f
	//�R���X�g���N�^�E�f�X�g���N�^
	IDirectXBaseApplicationLayer();
	IDirectXBaseApplicationLayer(const IDirectXBaseApplicationLayer&) ;
	VIRTUAL ~IDirectXBaseApplicationLayer();

	//DirectX�p���z�֐�
	VIRTUAL BOOL pOnCreateDirectX()=0;
	VIRTUAL BOOL pOnUpdateDirectX(float step)=0;
	VIRTUAL VOID pOnRenderDirectX()=0;
	VIRTUAL VOID pOnEventDirectX(TEventMessage msg )=0;

public:

	//����
	Vector GetCameraPosition()const{return m_pos;}
	//���_
	Vector GetCameraDirection()const{return m_vec;}

	//�擾
	//Directx
	TDirectXInfo GetDirectXInfo()const{return m_info;}
	TDirectXInfo* GetDirectXInfoPointa()const{return (TDirectXInfo*)(&m_info);}
	
};