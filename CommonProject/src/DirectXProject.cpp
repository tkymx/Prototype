#include "DirectXProject.h"

//�E�B���h�E�̑傫��
const static DWORD WindowWidth = 1200;
const static DWORD WindowHeight = 800;
//�_�u���N���b�N�̑���
const static DWORD WindowDoubleClick = 100;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���W�b�N�p�p
// ��{�I��FPS���W�b�N
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�R���X�g���N�^�E�f�X�g���N�^
	CDirectXGameLogic::CDirectXGameLogic()
	{
		//���E�̐ݒ�
		m_world3d = NULL;
	}
	CDirectXGameLogic::~CDirectXGameLogic()
	{

	}
//GameLogic���
	//������
	BOOL CDirectXGameLogic::onCreate() 
	{
		//���E�̐���
		m_world3d =	CPhysicsWorld3D::CreatePhysicsWorld3D();
		m_world3d->SetAirGap( WorldAir3D );
		m_world3d->SetGravity( WorldGravity3D );

		return true;
	}
	//�X�V( View�X�V�������� )
	VOID CDirectXGameLogic::onUpdate(float step) 
	{

		//���E�̍X�V
		m_world3d->Update(step);

	}
	//View�ݒ莞(View���͎����I�Ƀ^�X�N�ɒǉ������̂ł��̑��̐ݒ�)
	 VOID CDirectXGameLogic::onAddView( LPTkyGameView view )
	{
		//��Q���̒ǉ�
		IF( view->GetName() == "3DObstaclePhysicsModel" )
		{
			m_world3d->AddObtaclePhysicsModel( ((LPTkyObstacleModelView)view)->GetXFile() );
		}
		//���f���̒ǉ�
		IF( view->GetName() == "3DNormalPhysicsModel" )
		{
			m_world3d->AddPhysicsModel( ((LPTkyNormalModelView)view)->GetPhysicsModel() );
		}
	}
//�C�x���g���
	//�C�x���g�����炤
	VOID CDirectXGameLogic::onEvent( TEventMessage msg )
	{
	}
//�Ǝ����
	//����
	LPTkyDirectXGameLogic CDirectXGameLogic::CreateDirectXGameLogic()
	{
		return new CDirectXGameLogic();
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectX�e�X�g�p
// ��{�I�ȃA�v���P�[�V�������C���[�@�_�C���N�gX�ɓ���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DirectXApplication�v�f
	//�R���X�g���N�^�E�f�X�g���N�^
	CDirectXApplicationLayer::CDirectXApplicationLayer()
	{
		m_pDtata = NULL;
		m_pFont = NULL;	
		m_device = NULL;

		m_Info.pos = &m_pos;
		m_Info.vec = &m_vec;

	}
	CDirectXApplicationLayer::~CDirectXApplicationLayer()
	{

	}
	//�L�[�̎擾
	VOID				CDirectXApplicationLayer::pKeyBoard()
	{
//		assert(m_keybord);

		IF(m_device != NULL)
		{
			//�L�[�C�x���g
			CHAR key[256];	
			IF(m_device->GetKeyBoard(key)==true)
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
	//����
	LPTkyDirectXApplicationLayer CDirectXApplicationLayer::CreateDirectXApplicationLayer()
	{
		return new CDirectXApplicationLayer();
	}
//WindowsApplication�v�f
	BOOL			CDirectXApplicationLayer::pOnCreate()
	{
		//�f�[�^�̎擾
		m_pDtata = Init3D::CreateInit3D();
		IF_FAILED( m_pDtata->Create3DX(GetHWnd()) )
		{
			MessageBoxA( NULL ,"�G���[" , "INIT3D" ,NULL );
			return FALSE;
		}
		//�t�H���g�f�[�^�擾
		m_pFont = TD3DFONT::CreateFontW(m_pDtata);
		IF_FAILED( m_pFont->GetState() )
		{
			MessageBoxA( NULL , "sdadf" , "csdaf" , MB_OK);
			return FALSE;
		}
		//�L�[�{�[�h���
		m_device = CTkyInput::CreateInput();
		HRESULT h = m_device->SetInputinfo(GetHInstance(),GetHWnd());
		IF(FAILED(h))
		{
			return false;
		}

		//���̐ݒ�
		m_envi = CEnvironment::CreateEnvironment();
		m_envi->SetDevice(m_pDtata);

		//���W�b�N
		m_logic = CDirectXGameLogic::CreateDirectXGameLogic();
		m_logic->Create();

		//���擾�r���[

		//�������r���[
		IF(!m_logic->AddGameView(CLoadGameView::CreateLoadGameView(m_logic,&m_Info)))return false;

		//�Z�b�g�^�C�}�[(�L�[�p)
		SetTimer( GetHWnd() , 10 , 10 , NULL );

	   return true;
	}
	BOOL			CDirectXApplicationLayer::pOnUpdate()
	{
		//�L�[�{�[�h
		pKeyBoard();

		//���X�V
		m_envi->SetLightParameterAt( &Vector(0,10,0),&Vector( 0,0,0 ),&D3DXVECTOR4( 0.1f,0.1f,0.1f ,1.0 ),&D3DXVECTOR4( 0.9f,0.9f,0.9f , 1.0f ),&D3DXVECTOR4( 0,0,0,0 ) );
		m_envi->SetCameraParameter( &m_pos , &m_vec ,60.0f , (float)WindowWidth/(float)WindowHeight , 0.1f , 100.0f );

		//���W�b�N
		m_logic->Update(1.0f);

		//���ݒ�
		m_envi->onExecute();

		//�`��
		IF( m_pDtata->StartDraw() )
		{
			m_pDtata->Clear( D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB(255,255,255,255));

			m_pDtata->SetColor(Vector(1,1,0));
			m_pDtata->DrawLine3D( Vector(3,0,3),Vector(-3,0,-3),10 );

			m_pDtata->SetColor(Vector(1,0,0));
			m_pDtata->DrawCircle( Vector(500,800,0),100,20 );
			m_pDtata->DrawLine( Vector(30,600,0),Vector(600,600,0),10 );

			//���W�b�N�̕`��
			m_logic->Render();

			//�t�H���g�̕`��
			//�����̕`��==============================
			m_pFont->SetColor(0,0,0);
			char str[200];
			sprintf_s(str,"Directx�@Hello World!! FPS%d �@X:%d Y:%d Z:%d ",GetFps(),m_mouse.lX,m_mouse.lY,m_mouse.lZ	);
			m_pFont->DrawFontA(100,150,str);

			//========================================

			m_pDtata->EndDraw();
		}

		return true;
	}
//�C�x���g�v�f
	VOID			CDirectXApplicationLayer::pOnEvent( TEventMessage msg )
	{
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			IF( message == WM_TIMER )
			{
				IF( wParam == 10 )
				{
					//�}�E�X�C�x���g
					m_mouse = m_device->GetDeviceEvent();
					CEventManager::StaticAddEventMessage( "MOUSE_INFO" , (DWORD)&m_mouse );
				}
			}
			IF( message == WM_ACTIVATE )
			{
				//�f�o�C�X
				IF(m_device!=NULL)m_device->SetDeviceAcquire(true);
			}
		}
	}

