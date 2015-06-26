#include "TkyDirectXApplicationLayer.h"

//�_�u���N���b�N�̑���
const static DWORD WindowDoubleClick = 100;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectX�e�X�g�p
// ��{�I�ȃA�v���P�[�V�������C���[�@�_�C���N�gX�ɓ���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXApplication�v�f
	//�R���X�g���N�^�E�f�X�g���N�^
	IDirectXBaseApplicationLayer::IDirectXBaseApplicationLayer()
	{
		m_info.Initialize();
		m_vec = Vector(0,0,1);
		m_pos = Vector(  );
	}
	IDirectXBaseApplicationLayer::IDirectXBaseApplicationLayer(const IDirectXBaseApplicationLayer& x) :IWindowsApplicationLayer(x)
	{
		m_info = x.GetDirectXInfo();
	}
	IDirectXBaseApplicationLayer::~IDirectXBaseApplicationLayer()
	{
		IF(m_info.m_pDtata!=NULL)m_info.m_pDtata->DestroyOwnObject();
		IF(m_info.m_pFont!=NULL)m_info.m_pFont->DestroyOwnObject();	
		IF(m_info.m_device!=NULL)m_info.m_device->DestroyOwnObject();
		IF(m_info.m_envi!=NULL)m_info.m_envi->DestroyOwnObject();
	}
	//�L�[�̎擾
	VOID				IDirectXBaseApplicationLayer::pKeyBoard()
	{
//		assert(m_keybord);

		IF(m_info.m_device != NULL)
		{
			//�L�[�C�x���g
			CHAR key[256];	
			IF(m_info.m_device->GetKeyBoard(key)==true)
			{
				//�_�u���N���b�N�p
				static DWORD lefttime = 0;
				static DWORD righttime = 0;

				TEventMessage msg = { "" , vector<DWORD>() };

				//�L�[�𑗂�====================
				CEventManager::StaticAddEventMessage( "KEY_INFO" , (DWORD)&key );
				//========================================
				//�オ������Ă�����====================
				IF( KEYDOWN( key , DIK_UP ) )
				{
					msg.m_message = "KEY_UP";
					CEventManager::StaticAddEventMessage( msg );
				}
				//========================================
				//����������Ă�����====================
				IF( KEYDOWN( key , DIK_DOWN ) )
				{
					msg.m_message = "KEY_DOWN";
					CEventManager::StaticAddEventMessage( msg );
				}
				//========================================
				//����������Ă�����====================
				IF( KEYDOWN( key , DIK_LEFT ) )
				{
					msg.m_message = "KEY_LEFT";
					CEventManager::StaticAddEventMessage( msg );
					IF( timeGetTime() - lefttime <= WindowDoubleClick )
				{
					msg.m_message = "KEY_LEFT_DOUBLE";
					CEventManager::StaticAddEventMessage( msg );
					lefttime = 0;
				}
					else
					{
						lefttime = 1;
					}
				}
				else
				{
					IF(lefttime == 1)
					{
						lefttime = timeGetTime();
					}				
				}
				//========================================
				//�E��������Ă�����====================
				IF( KEYDOWN( key , DIK_RIGHT ) )
				{
					msg.m_message = "KEY_RIGHT";
					CEventManager::StaticAddEventMessage( msg );
					IF( timeGetTime() - righttime <= WindowDoubleClick )
					{
						msg.m_message = "KEY_RIGHT_DOUBLE";
						CEventManager::StaticAddEventMessage( msg );
						righttime = 0;
					}
					else
					{
						righttime = 1;
					}
				}
				else
				{
					IF(righttime == 1)
					{
						righttime = timeGetTime();
					}
				}
				//========================================
			}
		}
	}
//WindowsApplication�v�f
	BOOL			IDirectXBaseApplicationLayer::pOnCreate()
	{
		//�f�[�^�̎擾
		m_info.m_pDtata = Init3D::CreateInit3D();
		IF_FAILED( m_info.m_pDtata->Create3DX(GetHWnd()) )
		{
			MessageBoxA( NULL ,"�G���[" , "INIT3D" ,NULL );
			return FALSE;
		}
		//�t�H���g�f�[�^�擾
		m_info.m_pFont = CFONT::CreateFontW(m_info.m_pDtata);
		IF_FAILED( m_info.m_pFont->GetState() )
		{
			MessageBoxA( NULL , "sdadf" , "csdaf" , MB_OK);
			return FALSE;
		}
		//�L�[�{�[�h���
		m_info.m_device = CTkyInput::CreateInput();
		HRESULT h = m_info.m_device->SetInputinfo(GetHInstance(),GetHWnd());
		IF(FAILED(h))
		{
			MessageBoxW( NULL , TEXT("�L�[�{�[�h���s") , TEXT("err") , NULL );
			return false;
		}

		//���̐ݒ�
		m_info.m_envi = CEnvironment::CreateEnvironment();
		m_info.m_envi->SetDevice(m_info.m_pDtata);

		//�Z�b�g�^�C�}�[(�L�[�p)
		SetTimer( GetHWnd() , 10 , 10 , NULL );

		//�J�������
		m_info.m_pos = &m_pos;
		m_info.m_vec = &m_vec;

		//�p����̓���
		IF(!pOnCreateDirectX())
		{
			MessageBoxW( NULL , TEXT("DirectX���s") , TEXT("err") , NULL );
			return false;
		}

	   return true;
	}
	BOOL			IDirectXBaseApplicationLayer::pOnUpdate()
	{
		//�L�[�{�[�h
		pKeyBoard();

		//���X�V
		m_info.m_envi->SetLightParameterAt( &Vector(0,10,0),&Vector( 0,0,0 ),&D3DXVECTOR4( 0.1f,0.1f,0.1f ,1.0 ),&D3DXVECTOR4( 0.9f,0.9f,0.9f , 1.0f ),&D3DXVECTOR4( 0,0,0,0 ) );
		m_info.m_envi->SetCameraParameter( &Vector(0,0,0) , &Vector(0,0,1) ,60.0f , GetWindowsSize().x/GetWindowsSize().y , 0.1f , 500.0f );

		//�p����̍X�V
		IF(!pOnUpdateDirectX( 1.0f ))
		{
			MessageBoxW( NULL , TEXT("DirectXUpdate���s") , TEXT("err") , NULL );
			return false;
		}

		//�J�����̌����̐ݒ�
		GetDirectXInfo().m_envi->SetCameraParameter( &m_pos , &m_vec );

		//���ݒ�
		m_info.m_envi->onExecute();

		//�`��
		IF( m_info.m_pDtata->StartDraw() )
		{
			//��ʂ̏�����
			m_info.m_pDtata->Clear( D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(0,0,0,0));

			//�t�H���g�̕`��
			//�����̕`��==============================
//			m_info.m_pFont->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"Directx�@Hello World!! FPS%d �@X:%d Y:%d Z:%d ",GetFps(),m_info.m_mouse.lX,m_info.m_mouse.lY,m_info.m_mouse.lZ	);
//			m_info.m_pFont->DrawFontA(100,150,str);

			//========================================

			//�p����̕`��
			pOnRenderDirectX();

			m_info.m_pDtata->EndDraw();
		}

		return true;
	}
//�C�x���g�v�f
	VOID			IDirectXBaseApplicationLayer::pOnEvent( TEventMessage msg )
	{
		//�p����̃C�x���g
		pOnEventDirectX( msg );
		//���ʃC�x���g
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			IF( message == WM_MOUSEMOVE )
			{
				m_info.m_absmouse.x = (float)LOWORD(lParam);
				m_info.m_absmouse.y = (float)HIWORD(lParam);
			}
			IF( message == WM_TIMER )
			{
				IF( wParam == 10 )
				{
					//�}�E�X�C�x���g
					m_info.m_mouse = m_info.m_device->GetDeviceEvent();
					CEventManager::StaticAddEventMessage( "MOUSE_INFO" , (DWORD)&m_info.m_mouse );
				}
			}
			IF( message == WM_ACTIVATE )
			{
				//�f�o�C�X
				IF(m_info.m_device!=NULL)m_info.m_device->SetDeviceAcquire(true);
			}
		}
	}

