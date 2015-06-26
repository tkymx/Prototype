#include "TKYDIRECTINPUT.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
//		���͊֌W
//
//////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------//
//�R���g���X�^
//---------------------------------------------//
CTkyInput::CTkyInput()
{
	CTkyInput::m_IsAcquire=false;
	CTkyInput::g_pDI = NULL;
	CTkyInput::g_pKeyDevice = NULL;
	CTkyInput::g_pMouseDevice = NULL;
}

CTkyInput::~CTkyInput()
{
	this->Release();
}
//---------------------------------------------//
//�ڍאݒ���s��
//---------------------------------------------//
HRESULT CTkyInput::SetInputinfo(HINSTANCE a_hInst,HWND a_hWnd)
{
	HRESULT             hr; 
	HANDLE g_hMouseEvent = NULL;

	//DirectInput �I�u�W�F�N�g�̍쐬
	hr = DirectInput8Create(a_hInst, DIRECTINPUT_VERSION, 
			IID_IDirectInput8, (void**)&g_pDI, NULL); 
	IF(FAILED(hr) )
	{ 
		return FALSE;
		// DirectInput not available; take appropriate action 
	} 

	//�}�E�X���

		//�}�E�X�f�o�C�X�̍쐬
		hr = g_pDI->CreateDevice(GUID_SysMouse, &g_pMouseDevice, NULL);
		IF(FAILED(hr)) {Release();return FALSE;}

		//�}�E�X�f�[�^�`���̐ݒ�
		hr = g_pMouseDevice->SetDataFormat(&c_dfDIMouse);
		IF(FAILED(hr)) {Release();return FALSE;}

		//�}�E�X����̐ݒ�
		hr = g_pMouseDevice->SetCooperativeLevel(a_hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		IF(FAILED(hr)) {Release();return FALSE;}
/*	
		//�o�b�t�@�̎w��炵��
		#define SAMPLE_BUFFER_SIZE  16

		//�C�x���g���쐬����
		g_hMouseEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (g_hMouseEvent == NULL) {return FALSE;}

		//�}�E�X�ɃC�x���g���֘A�Â���
		hr = g_pDIDevice->SetEventNotification(g_hMouseEvent);
		if (FAILED(hr)) {return FALSE;}
*/
		//�p�����[�^�[�̐ݒ�
		DIPROPDWORD dipdw;
		ZeroMemory(&dipdw,sizeof(DIPROPDWORD));
		//��^��
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		//�A�N�Z�X����v���p�e�B��������I�u�W�F�N
		dipdw.diph.dwObj = 0;
		//�g�p���@�H
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL; // ���΍��W���[�h
//		dipdw.dwData = DIPROPAXISMODE_ABS; // ��΍��W���[�h
//		dipdw.dwData = SAMPLE_BUFFER_SIZE;
		//���̐ݒ�
		hr = g_pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		IF(FAILED(hr)) {return FALSE;}

	//�L�[�{�[�h��

		//�L�[�{�[�h�f�o�C�X�̍쐬
		hr = g_pDI->CreateDevice(GUID_SysKeyboard, &g_pKeyDevice, NULL); 
		if(FAILED(hr)) {Release();return FALSE;} 

		//�L�[�{�[�h�f�[�^�t�H�[�}�b�g�̍쐬(��`�ς݂̕ϐ��Ƀf�[�^�t�H�[�}�b�g������)
		hr = g_pKeyDevice->SetDataFormat(&c_dfDIKeyboard); 
		if(FAILED(hr)) {Release();return FALSE;} 

		//�L�[�{�[�h�̓���̐ݒ�
		hr = g_pKeyDevice->SetCooperativeLevel(a_hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
		if(FAILED(hr)){Release();return FALSE;} 

		//�}�E�X�L�[�{�[�h�ւ̃A�N�Z�X���̎擾
		this->SetDeviceAcquire(true);


	return TRUE;
}
//---------------------------------------------//
//�}�E�X�̒l�̎擾
//---------------------------------------------//
DIMOUSESTATE CTkyInput::GetDeviceEvent()
{
	DIMOUSESTATE dims = {0,0,0,0};

	//�A�N�Z�X������������
	if(m_IsAcquire)		
	{
		g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&dims);
	}

	return dims;

}

//---------------------------------------------//
//�L�[�{�[�h�̒l�̎擾
//---------------------------------------------//
bool CTkyInput::GetKeyBoard(char Abuffer[256])				//�L�[�̏�Ԃ��擾����
{
	
	CHAR buffer[256];
	ZeroMemory(buffer,sizeof(buffer));
    HRESULT  hr; 
	hr = g_pKeyDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
    if FAILED(hr) 
    { 
        return false; 
    }
	memcpy(Abuffer,buffer,sizeof(buffer));
	
	return true;
}
//---------------------------------------------//
//�A�N�Z�X���̊Ǘ�
//---------------------------------------------//
void CTkyInput::SetDeviceAcquire(bool a_Acquire)
{
	IF(g_pKeyDevice!=NULL)
	{
		if(a_Acquire)	{
			g_pKeyDevice->Acquire();
			m_IsAcquire = a_Acquire;
		}else{
			m_IsAcquire = a_Acquire;
			g_pKeyDevice->Unacquire();
		}
	}
	IF(g_pMouseDevice!=NULL)
	{
		if(a_Acquire)	{
			g_pMouseDevice->Acquire();
			m_IsAcquire = a_Acquire;
		}else{
			m_IsAcquire = a_Acquire;
			g_pMouseDevice->Unacquire();
		}
	}
}

//---------------------------------------------//
//�J������
//---------------------------------------------//
VOID CTkyInput::Release()
{
    if (g_pDI) 
    { 
		if (g_pKeyDevice) 
        { 
			//�f�o�C�X�̉������
			g_pKeyDevice->Unacquire(); 
			g_pKeyDevice->Release();
			g_pKeyDevice = NULL; 
        } 
		if (g_pMouseDevice) 
        { 
			//�f�o�C�X�̉������
			g_pMouseDevice->Unacquire(); 
			g_pMouseDevice->Release();
			g_pMouseDevice = NULL; 
        } 
		//DirectInput�@�́@�n���h���@�́@�������
        g_pDI->Release();
        g_pDI = NULL; 
    } 
}

