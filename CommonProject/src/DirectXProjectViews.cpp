#include "DirectXProjectViews.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���f���r���[
// ��{�I�ȃ��f���̃r���[
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	IModelView::IModelView()
	{
		m_model = NULL;
		m_PhysicsModel = NULL;
	}
	IModelView::~IModelView()
	{
	}
	//���f���̐���
	BOOL IModelView::pCreateModel( wstring name )
	{
		//Xfile���[�h
		IF(name == TEXT("") )
		{
			m_model = NULL;
		}
		else
		{
			m_model = CXFile::CreateXFile(Init3D::CreateInit3D());
			IF_FAILED(m_model->SetXFileW(name))
			{
				return false;
			}
		}
		//���f���̃��[�h
		m_PhysicsModel = PhysicsCircleModel::CreatePhysicsCircleModel( 1.0f );
		m_PhysicsModel->SetWeight( 1.0f );

		return true;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���ʃI�u�W�F�N�g���f��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CNormalModelView::CNormalModelView(wstring name)
	{
		pCreateModel(name);
	}
	CNormalModelView::~CNormalModelView()
	{
	}
//GameView �v�f
	//������
	BOOL CNormalModelView::onCreate(){return true;}
	//�X�V
	BOOL CNormalModelView::onUpdate(float step){return true;}
	//�r���[�v�f
	//�`��
	VOID CNormalModelView::onRender()
	{
		//��l���̓���
		IF(GetXFile()!=NULL)
		{
			static float frame = 0;
			frame = GetXFile()->GetAnime().UpdateFrame( frame ,  0.5f );
			GetXFile()->SetAnimeFreem(frame);
			GetXFile()->DrawMesh(GetPhysicsModel()->GetPos(),Vector());
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CNormalModelView::onEvent( TEventMessage msg ){}
//�Ǝ��v�f
	//����
	LPTkyNormalModelView CNormalModelView::CreateNormalModelView( wstring name )
	{
		return new CNormalModelView(name);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//��Q�����f��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CObstacleModelView::CObstacleModelView(wstring name)
	{
		pCreateModel(name);
	}
	CObstacleModelView::~CObstacleModelView()
	{
	}
//GameView �v�f
	//������
	BOOL CObstacleModelView::onCreate(){return true;}
	//�X�V
	BOOL CObstacleModelView::onUpdate(float step){return true;}
	//�r���[�v�f
	//�`��
	VOID CObstacleModelView::onRender()
	{
		//��l���̓���
		IF(GetXFile()!=NULL)GetXFile()->DrawMesh(GetPhysicsModel()->GetPos(),Vector());
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CObstacleModelView::onEvent( TEventMessage msg ){}
//�Ǝ��v�f
	//����
	LPTkyObstacleModelView CObstacleModelView::CreateObstacleModelView( wstring name )
	{
		return new CObstacleModelView(name);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPS���f��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSModelView::CFPSModelView(Vector *pos,Vector *vec)
	{
		m_pos = pos;
		m_Direction = vec;
		m_rotate = Vector();
	}
	CFPSModelView::~CFPSModelView()
	{
	}
//GameView �v�f
	//������
	BOOL CFPSModelView::onCreate(){return true;}
	//�X�V
	BOOL CFPSModelView::onUpdate(float step)
	{
		LPPhysicsCircleModel model = GetPhysicsModel();
		IF( model != NULL )
		{
			*m_pos = model->GetPos();
			*m_Direction = Vector(0,0,1).Rotate( Vector() , m_rotate.x , m_rotate.y , m_rotate.z   );
		}
		return true;
	}
	//�r���[�v�f
	//�`��
	VOID CFPSModelView::onRender()
	{
		//�����̕`��==============================

		TD3DFONT::CreateFontW(NULL)->SetColor(0,0,0);
		char str[200];
		sprintf_s(str,"�������:%lf X:%lf Y:%lf Z:%lf ",1.0f,m_pos->x,m_pos->y,m_pos->z );
		TD3DFONT::CreateFontW(NULL)->DrawFontA(500,150,str);

		//========================================

		//�����̕`��==============================

		TD3DFONT::CreateFontW(NULL)->SetColor(0,0,0);
		sprintf_s(str,"�}�E�X�萔 X:%d Y:%d Z:%d ",m_mouse.lX,m_mouse.lY,m_mouse.lZ );
		TD3DFONT::CreateFontW(NULL)->DrawFontA(500,50,str);

		//========================================

	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFPSModelView::onEvent( TEventMessage msg )
	{
		//���f�����
		LPPhysicsCircleModel model = this->GetPhysicsModel();

		//�ړ����W
		Vector move = m_Direction->Normal();
		move.y = 0;
		move = move.Normal();

		string EventName = msg.m_message;
		IF(EventName == "KEY_INFO")
		{
			//�L�[���
			CHAR *key = (CHAR*)msg.m_value[0];
			IF( KEYDOWN( key , DIK_Z ) )
			{
				model->Impact( Vector( 0,WorldWolk3D,0 ) );
			}
			IF( KEYDOWN( key , DIK_X ) )
			{
				model->Impact( Vector( 0,-WorldWolk3D,0 ) );
			}
			IF( KEYDOWN( key , DIK_C ) )
			{
//				m_height += -0.01f;

			}
			IF( KEYDOWN( key , DIK_V ) )
			{
//				m_height -= -0.01f;

			}
			IF( KEYDOWN( key , DIK_SPACE ) )
			{
				model->Impact( Vector( 0,WorldJump3D,0 ) );
			}
		}
		EIF(EventName == "KEY_RIGHT")	//�E�ړ�
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*-WorldWolk3D );
		}
		EIF(EventName == "KEY_LEFT")	//���ړ�
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*WorldWolk3D );
		}
		EIF(EventName == "KEY_UP")		//�O�i
		{
			model->Impact( move*WorldWolk3D );
		}
		EIF(EventName == "KEY_DOWN")	//���
		{
			model->Impact( move*-WorldWolk3D );
		}
		EIF(EventName == "MOUSE_INFO")
		{

			m_mouse = *( (DIMOUSESTATE*)msg.m_value[0] );

			m_rotate += Vector( (float)m_mouse.lY / 10.0f , (float)m_mouse.lX / 10.0f , 0 ); 

		}
	}
//�Ǝ��v�f
	//����
	LPTkyFPSModelView CFPSModelView::CreateFPSModelView(Vector *pos,Vector *vec)
	{
		return new CFPSModelView(pos,vec);
	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�X�e�[�W�ǂݍ��݃r���[
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CLoadGameView::CLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info)
	{
		m_MainLogick = logic;
		m_info = info;
	}
	CLoadGameView::~CLoadGameView()
	{
	}
//GameView���
	//������
	BOOL CLoadGameView::onCreate()
	{
		//��Q�����f���̐���
		IF(!m_MainLogick->AddGameView(CObstacleModelView::CreateObstacleModelView( TEXT("mesh/mesh.x") )))return false;

		//���f���̐���(��l��)
		LPTkyNormalModelView modelview = CNormalModelView::CreateNormalModelView( TEXT("") );
		modelview->SetPos(Vector( -106,3.8f,-45 ));
		IF(!m_MainLogick->AddGameView(modelview))return false;

		//���f���̐���(���u�L����)
		LPTkyNormalModelView enemyview = CNormalModelView::CreateNormalModelView( TEXT("mesh/chara2_stand.x") );
		enemyview->SetPos(Vector( -106,-3.8f,-35 ));
		IF(!m_MainLogick->AddGameView(enemyview))return false;

		//��������
		*m_info->vec = Vector(0,0,1);

		//FPS���
		LPTkyFPSModelView fpsview = CFPSModelView::CreateFPSModelView( m_info->pos , m_info->vec );
		//���f���̃Z�b�g
		fpsview->SetPhysicsModel( modelview->GetPhysicsModel() );
		IF(!m_MainLogick->AddGameView(fpsview))return false;


		return true;

	}
	//�X�V
	BOOL CLoadGameView::onUpdate(float step)
	{
		return true;
	}
//�C�x���g���
	//�C�x���g�����炤
	VOID CLoadGameView::onEvent( TEventMessage msg )
	{
	}
	//�r���[�v�f
	//�`��
	VOID CLoadGameView::onRender()
	{
	}
//�Ǝ����
	//����
	LPTkyLoadGameView CLoadGameView::CreateLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info)
	{
		LPTkyLoadGameView view = new CLoadGameView(logic,info);
		return view;
	}
