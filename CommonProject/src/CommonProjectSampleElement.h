#pragma once

#include "resource.h"

#include "TKYBASEHEADER.h"
#include "TkyLogicView.h"
#include "TkyPhysics.h"
#include "TkyWorld.h"
#include "TkyWindowsApplication.h"

HINSTANCE	g_hInst;								// ���݂̃C���^�[�t�F�C�X
HWND		g_hWnd;


//�E�B���h�E�̑傫��
const static DWORD WindowWidth = 1200;
const static DWORD WindowHeight = 800;
//�_�u���N���b�N�̑���
const static DWORD WindowDoubleClick = 100;

//�E�B���h�E�̍�
const static Vector WindowGap = Vector(100,0,0);

//���C��
const STATIC float WorldFrictional = 0.1f;
//������
const STATIC float WorldElastic = 0.0f;


//�x���X�N���[�����x
const STATIC float	WorldScrollVelocity = 5.0f;

//��C��R
const STATIC float WorldAir = 0.2f;
//�d��
const STATIC float WorldGravity = 4.0f;

//�d��
const STATIC float ModelWeight = 1.0f;
//�W�����v��
const STATIC float ModelJunp = 12.0f;
//����
const STATIC float ModelForce = 2.0f;
//���C��
const STATIC float ModelFrictional = 1.0f;
//������
const STATIC float ModelElastic = 0.0f;
//�_�b�V��
const STATIC float ModelDash = 30.0f;

//�X�N���[�����
CLASS_POINTA( CScroll , LPTkyScroll )
{
private:
	Vector m_scroll;
	Vector m_realscroll;
	Vector m_window;
public:
	//���W���Z�b�g����
	VOID SetPosision(Vector pos)
	{
		//�X�N���[���̃Z�b�g
		m_scroll =  pos + Vector( -(float)WindowWidth/2 , -(float)WindowHeight/2 , 0  ) + WindowGap;
	}
	//�X�V����
	VOID Update(Vector move)
	{
		m_realscroll.Move( m_scroll , move );
		Vector vec = m_realscroll - m_scroll;
		DWORD lange = min(WindowHeight,WindowHeight)*2/6;
		IF(vec.GetLarge() >= (float)lange )m_realscroll = m_scroll + vec.Normal() * (float)lange; 
	}

	//�ݒ�
	VOID SetScroll(Vector s){ m_scroll = s; }
	VOID SetRealScroll(Vector s){ m_realscroll = s; }
	VOID SetWindowSize(Vector v){m_window = v;}
	//�擾
	Vector GetScroll(){return m_scroll;}
//	Vector GetRealScroll(){return m_realscroll;}
	Vector GetRealScroll(){return m_scroll;}
	Vector GetWindowSize(){return m_window;}
};

//���̏��r���[(�p����ŕ��̏������߂�����)
CLASS_POINTA( IPhysicsView , LPTkyPhysicsView ) :public IGameView
{
protected:

	//�������
	LPPhysicsModel m_model;

public:

	//������
	VIRTUAL BOOL onCreate()=0;
	//�X�V
	VIRTUAL BOOL onUpdate(float step)=0;
	//�j��
	VIRTUAL VOID onRelease() =0;

	//�C�x���g�l�[�����O�����߂�
	VOID onNameSet( string *str )
	{
		*str = "PhysicsView";
	}
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg )=0;

	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender() = 0;

	//���f�����̎擾
	LPPhysicsModel GetModel(){return m_model;}

};

//��l���r���[
CLASS_POINTA( IHeroView , LPTkyHeroView ) :public IPhysicsView
{
private:

	//�ړ����x
	float m_MoveForce;
	//�W�����v�萔
	float m_JumpContant;

	//�X�N���[��
	CScroll m_scroll;

	//�ڐG���W
	vector<Vector> m_pos;
	vector<CVector> m_vec;

protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	IHeroView(Vector pos)
	{
		m_model = PhysicsCircleModel::CreatePhysicsCircleModel( 50 );
		m_JumpContant = ModelJunp;
		m_MoveForce = ModelForce;
		m_model->SetPos(pos);
		m_model->SetFrictional(ModelFrictional);
		m_model->SetElastic(ModelElastic);
		m_model->SetWeight(ModelWeight);
	}
	VIRTUAL ~IHeroView()
	{
		m_pos.clear();
		m_vec.clear();
	}

public:

	//������
	VIRTUAL BOOL onCreate()
	{
	}
	//�X�V
	VIRTUAL BOOL onUpdate(float step)
	{

		//�X�N���[���̐ݒ�
		m_scroll.SetPosision( m_model->GetPos() );
		m_scroll.Update( Vector(1,0.1f,1)*WorldScrollVelocity );
	}
	//�j��
	VIRTUAL VOID onRelease()
	{
	}

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		string EventName = msg.m_message;
		IF(EventName == "KEY_UP")
		{
			m_model->Impact( Vector(0,-m_JumpContant,0) );
		}
		IF(EventName == "KEY_DOWN")
		{
			m_model->Impact( Vector(0,20,0) );
		}
		EIF(EventName == "KEY_LEFT")
		{
			m_model->Impact( Vector(-m_MoveForce,0,0) );
		}
		EIF(EventName == "KEY_LEFT_DOUBLE")
		{
			DWORD Dash = (DWORD)ModelDash;
			m_model->Impact( Vector( -m_MoveForce*ModelDash,-m_MoveForce*Dash,0 ) );
		}
		EIF(EventName == "KEY_RIGHT")
		{
			m_model->Impact( Vector(m_MoveForce,0,0) );
		}
		EIF(EventName == "KEY_RIGHT_DOUBLE")
		{
			DWORD Dash = (DWORD)ModelDash;
			m_model->Impact( Vector( m_MoveForce*ModelDash,-m_MoveForce*Dash,0 ) );
		}
		EIF(EventName == "GET_HERO_SCROLL")
		{
			//�X�N���[������đ��M
			vector<DWORD> ele;
			ele.push_back( (DWORD)(&m_scroll) );
			CEventManager::StaticAddEventMessage( "HERO_SCROLL" , ele );
		}

		EIF(msg.m_message == "COLLISION_POS" )
		{
			m_pos.push_back( *((Vector*)msg.m_value[0]) );
		}
		EIF(msg.m_message == "COLLISION_VEC" )
		{
			m_vec.push_back( *((CVector*)msg.m_value[0]) );
		}

	}

	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender()
	{
		//��l���̓���
		LPTKY3D tky = Init3D::CreateInit3D();
		tky->SetColor(Vector(1,0,0));			//�F�Z�b�g
		tky->DrawCircle( m_model->GetPos() - m_scroll.GetRealScroll() , m_model->GetShape().m_element[0].x , 20 ); //�`��

		//�O��
		tky->SetColor(Vector(0,0,0));			//�F�Z�b�g
		tky->DrawLine( 
			m_model->GetPos() - m_scroll.GetRealScroll() , 
			m_model->GetPos()+m_model->GetVel()  - m_scroll.GetRealScroll(), 
			10 );		

		vector<Vector>::iterator ip = m_pos.begin();
		int i=0;
		WHILE( ip != m_pos.end() )
		{
			i++;

			Vector pos = *ip - m_scroll.GetRealScroll();

			char str[200];
			sprintf_s(str,"%d",i);
			TD3DFONT::CreateFontW(NULL)->DrawFontA( (INT)pos.x , (INT)pos.y - 20 ,str );

			LPTKY3D tky = Init3D::CreateInit3D();
			tky->SetColor(Vector(0,1,1));			//�F�Z�b�g
			tky->DrawCircle( pos , 5, 20 ); //�`��
			ip++;
		}

		vector<CVector>::iterator iv = m_vec.begin();
		i=0;
		WHILE( iv != m_vec.end() )
		{
			i++;

			Vector pos = iv->GetVectorStart() - m_scroll.GetRealScroll();

			char str[200];
			sprintf_s(str,"%d",i);
			TD3DFONT::CreateFontW(NULL)->DrawFontA( (INT)pos.x , (INT)pos.y - 20 ,str );

			tky->SetColor(Vector(0.5f,0.5f,0.5f));			//�F�Z�b�g
			tky->DrawLine( 
				pos , 
				pos+iv->GetVector(), 
				5 );		

			iv++;
		}



		//����
		char str[200];
		sprintf_s(str,"POS (%.3lf,%.3lf,%.3lf) VEL (%.3lf,%.3lf,%.3lf) ACC (%.3lf,%.3lf,%.3lf)"
			,m_model->GetPos().x,m_model->GetPos().y,m_model->GetPos().z
			,m_model->GetVel().x,m_model->GetVel().y,m_model->GetVel().z
			,m_model->GetAcc().x,m_model->GetAcc().y,m_model->GetAcc().z
			);
		TD3DFONT::CreateFontW(NULL)->DrawFontA( 100,120,str );

	}

	//����
	STATIC LPTkyHeroView CreateHereView(Vector pos)
	{
		return new IHeroView(pos);			
	}

};

//����r���[
CLASS_POINTA( IWeaponView , LPTkyWeaponView ) :public IPhysicsView
{
private:

	//�X�N���[��
	LPTkyScroll m_scroll;

protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	IWeaponView(Vector pos)
	{
		m_model = PhysicsCircleModel::CreatePhysicsCircleModel( 50 );
		m_model->SetPos(pos);
		m_model->SetFrictional(ModelFrictional);
		m_model->SetElastic(ModelElastic);
		m_model->SetWeight(ModelWeight);
	}
	VIRTUAL ~IWeaponView()
	{
	}

public:

	//������
	VIRTUAL BOOL onCreate()
	{

	}
	//�X�V
	VIRTUAL BOOL onUpdate(float step)
	{

	}
	//�j��
	VIRTUAL VOID onRelease()
	{
	}

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}
	}

	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender()
	{
		//��l���̓���
		LPTKY3D tky = Init3D::CreateInit3D();
		tky->SetColor(Vector(0,0,1));			//�F�Z�b�g
		tky->DrawCircle( m_model->GetPos() - m_scroll->GetRealScroll() , m_model->GetShape().m_element[0].x , 20 ); //�`��

	}

	//����(��_���W)�������N�_�Ƃ���
	STATIC LPTkyWeaponView CreateWeaponView(Vector *pos)
	{
		return new IWeaponView(*pos);			
	}

	//�擾
	LPPhysicsCircleModel GetModel(){return (LPPhysicsCircleModel)m_model;}

};


//��Q���r���[
CLASS_POINTA( IObstaleView , LPTkyObstaleView ) :public IPhysicsView
{
private:

	//�X�N���[��
	LPTkyScroll m_scroll;

protected:

	//�R���X�g���N�^�E�f�X�g���N�^
	IObstaleView(Vector pos)
	{
		m_model = PhysicsCircleModel::CreatePhysicsCircleModel( 50 );
		m_model->SetPos(pos);
		m_model->SetFrictional(ModelFrictional);
		m_model->SetElastic(ModelElastic);
		m_model->SetWeight(ModelWeight);

		m_scroll=NULL;

	}
	VIRTUAL ~IObstaleView()
	{
	}

public:

	//������
	VIRTUAL BOOL onCreate()
	{

	}
	//�X�V
	VIRTUAL BOOL onUpdate(float step)
	{
		//�X�N���[�����Ȃ�������v��
		IF(m_scroll == NULL)
		{
			vector<DWORD> ele;
			CEventManager::StaticAddEventMessage( "GET_HERO_SCROLL" , ele );
		}
	}
	//�j��
	VIRTUAL VOID onRelease()
	{
	}

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}
	}

	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender()
	{
		//��Q���`��
		LPTKY3D tky = Init3D::CreateInit3D();
		tky->SetColor(Vector(0,0,1));			//�F�Z�b�g
		tky->DrawCircle( m_model->GetPos() - m_scroll->GetRealScroll() , m_model->GetShape().m_element[0].x , 20 ); //�`��

		//�O��
		tky->SetColor(Vector(0,1,1));			//�F�Z�b�g
		tky->DrawLine( 
			m_model->GetPos() - m_scroll->GetRealScroll() , 
			m_model->GetPos()+m_model->GetVel()  - m_scroll->GetRealScroll(), 
			10 );	

	}

	//����
	STATIC LPTkyObstaleView CreateObstaleView(Vector pos)
	{
		return new IObstaleView(pos);			
	}

	//�擾
	LPPhysicsCircleModel GetModel(){return (LPPhysicsCircleModel)m_model;}

};

//�}�b�v�r���[
CLASS_POINTA( IMapView , LPTkyMapView ) :public IPhysicsView
{
private:

	//�X�N���[�����
	LPTkyScroll m_scroll;

protected:

	IMapView( CVector cv )
	{
		Vector pos = (cv.GetVectorEnd()+cv.GetVectorStart())/2;
		m_model = PhysicsLineModel::CreatePhysicsLineModel( cv.GetVectorStart()-pos,cv.GetVectorEnd()-pos );
		m_model->SetFrictional(WorldFrictional);
		m_model->SetElastic(WorldElastic);
		m_model->SetPos(pos);
		m_model->SetWeight(0);
	}
	VIRTUAL ~IMapView()
	{
	}

public:

	//������
	VIRTUAL BOOL onCreate()
	{
		m_scroll=NULL;
	}
	//�X�V
	VIRTUAL BOOL onUpdate(float step)
	{
		//�X�N���[�����Ȃ�������v��
		IF(m_scroll == NULL)
		{
			CEventManager::StaticAddEventMessage( "GET_HERO_SCROLL" , vector<DWORD>() );
		}
	}
	//�j��
	VIRTUAL VOID onRelease()
	{
	}

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}
	}

	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender()
	{
		//�v�f�̊m�F�i�X�N���[���m�F�j
		assert(m_scroll);

		//�}�b�v�`��
		LPTKY3D tky = Init3D::CreateInit3D();
		CVector cv = ( (LPPhysicsLineModel)m_model )->GetLine();

		assert(m_scroll);

		cv.SetVectorEnd( cv.GetVectorEnd() - m_scroll->GetRealScroll() );
		cv.SetVectorStart( cv.GetVectorStart() - m_scroll->GetRealScroll() );
		cv.DrawLineSet( tky , Vector(0,1,0) , 10 );
	}

	//����
	STATIC LPTkyMapView CreateMapView(CVector cv)
	{
		return new IMapView(cv);			
	}

};

//�R�����Q�[�����W�b�N
CLASS_POINTA( CCommonGameLogic , LPTkyCommonGameLogic ) : public IGameLogic
{
private:

	//��ʂ̃X�N���[���
	LPTkyScroll m_scroll;

	LPTkyPhysicsWorld m_World;

	CCommonGameLogic()
	{
	}
	VIRTUAL ~CCommonGameLogic()
	{
		m_World->Release();

	}

public:

	//������
	VIRTUAL BOOL onCreate() 
	{
		//���E�̏��
		m_World = CPhysicsWorld::CreatePhysicsWorld();
		m_World->SetAirGap( WorldAir );	   
		m_World->SetGravity( WorldGravity );
		RECT rect = { 10 , -1000 , 7000-10 , 1000 };
		m_World->SetRange(rect);

		return true;
	}
	//�X�V( View�X�V�������� )
	VIRTUAL VOID onUpdate(float step) 
	{
		//�X�V
		m_World->Update(step);

	}
	//�j��
	VIRTUAL VOID onRelease()
	{
	}

	//View�ݒ莞(View���͎����I�Ƀ^�X�N�ɒǉ������̂ł��̑��̐ݒ�)
	VIRTUAL VOID onAddView( LPTkyGameView view )
	{
		IF( view->GetName() == "PhysicsView" )
		{
			m_World->AddPhysicsModel( ( (LPTkyPhysicsView)view )->GetModel() );
		}
	}

	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "CommonGame";
	}
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "ADD_VIEW" )
		{
			LPTkyObstaleView view = (LPTkyObstaleView)(msg.m_value[0]);
			view->GetModel()->SetPos( view->GetModel()->GetPos() + m_scroll->GetRealScroll() );
			this->AddGameView( view );
		}
		IF( msg.m_message == "HERO_SCROLL"  )
		{
			m_scroll = (LPTkyScroll)msg.m_value[0];
		}

	}

	//����
	STATIC LPTkyCommonGameLogic CreateCommonGameLogic()
	{
		return new CCommonGameLogic();
	}

};

//�R�����E�B���h�E
CLASS_POINTA( CCommonWindowsApplicationLayer , LPTkyCommonWindowsApplicationLayer ) :public IWindowsApplicationLayer
{
private:

	//�Q�[�����W�b�N
	LPTkyCommonGameLogic m_CommonGameLogic;
	
	//�f�o�b�O�p
	//���ł��������\���p
	vector<string> m_str;

	//��{�N���X
	LPTKY3D m_pDtata;
	//�����\���N���X
	LPTKYFONT m_pFont;

	//�f�o�C�X�̎擾�i�L�[�{�[�h�j
	LPTKYINPUT m_keybord;

	//�V������
	VOID				pKeyBoard()
	{
		assert(m_keybord);

		CHAR key[256];
		IF(m_keybord->GetKeyBoard(key)==true)
		{
			//�_�u���N���b�N�p
			static DWORD lefttime = 0;
			static DWORD righttime = 0;

			TEventMessage msg = { "" , vector<DWORD>() };

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

	VIRTUAL BOOL			pOnCreate()
	{


	   //================�V����������=======================================

	   //�������
	   m_CommonGameLogic = CCommonGameLogic::CreateCommonGameLogic();
	   m_CommonGameLogic->onCreate();
	   //�}�b�v�̓o�^
		//�������W
		const int NUM = 9;
		Vector vecs[NUM+1] = 
		{
			Vector( 0,0,0 ),			
			Vector( 100,750,0 ),
			Vector( 400,650,0 ),
			Vector( 700,750,0 ),
			Vector( 1200,800,0 ),
			Vector( 2000,100,0 ),	
			Vector( 3000,800,0 ),
			Vector( 3100,200,0 ),
			Vector( 5000,400,0 ),
			Vector( 7000,100,0 ),

//			Vector( 100,650,0 ),
//			Vector( 1000,750,0 ),

		};

		FOR_I(NUM)
		{
	   //================�V�����X�V=======================================

			m_CommonGameLogic->AddGameView( IMapView::CreateMapView(CVector( vecs[i],vecs[i+1] )) );

	   //==============================================================
		}
	   //�L�����N�^�o�^
	   m_CommonGameLogic->AddGameView( IHeroView::CreateHereView( Vector( WindowWidth/2 , 0 , 0 ) ) );

	   //==============================================================


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
		m_keybord = CTkyInput::CreateInput();
		m_keybord->SetInputinfo(GetHInstance(),GetHWnd());
	
	   return true;
	}
	VIRTUAL BOOL			pOnUpdate()
	{
		//�L�[�{�[�h�̏���
		pKeyBoard();
/*
		static DWORD time;
		static vector<DWORD> time1;
		static vector<DWORD> time2;
		static vector<DWORD> time3;
		static vector<DWORD> time4;
*/
		static vector<DWORD> fps_b;

		IF(fps_b.empty())
		{
			fps_b.push_back(GetFps());
/*			time1.push_back(0);
			time2.push_back(0);
			time3.push_back(0);
			time4.push_back(0);
*/		}
		EIF( fps_b.back() != GetFps())
		{
			fps_b.push_back(GetFps());
/*			time1.push_back(0);
			time2.push_back(0);
			time3.push_back(0);
			time4.push_back(0);
*/		}
/*
		time4.back() += timeGetTime() - time;


		//�ʏ�X�V

		time  = timeGetTime();
*/
//		m_CommonGameLogic->Update( 0.5f  * (60.0f/ ( (GetFps()>=10) ? GetFps() : 60.0f ) ) );
		m_CommonGameLogic->Update( 0.25f );
/*
		time1.back() += (timeGetTime() - time);

		//�C�x���g�X�V

		time  = timeGetTime();
*/
//		CEventManager::CreateEventManager()->Update();
/*
		time2.back() += timeGetTime() - time;


		//�`�揈��

		time  = timeGetTime();
*/
		IF( m_pDtata->StartDraw() )
		{
			m_pDtata->Clear( D3DCLEAR_TARGET , D3DCOLOR_ARGB(255,255,255,255));

		   //================�V�����X�V=======================================

			m_CommonGameLogic->Render();

		   //==============================================================

			//�t�H���g�̕`��
			//�����̕`��==============================
			m_pFont->SetColor(0,0,0);
			char str[200];
			FOR_I(fps_b.size())
			{
//				sprintf_s(str,"Hello World!! FPS is %d update:%d event:%d render:%d other:%d all:%d ",fps_b[i],time1[i],time2[i],time3[i]	,time4[i] , time1[i]+time2[i]+time3[i]+time4[i]	);
				sprintf_s(str,"�������Z�@Hello World!! FPS is %d ",fps_b[i]	);
				m_pFont->DrawFontA(100,150+20*(fps_b.size()-i),str);
			}

			//���ł�������
			FOR_I(m_str.size())
			{
				m_pFont->DrawFontA(500,150+20*(m_str.size()-i),m_str[i].c_str());
			}

			//========================================

			m_pDtata->EndDraw();
		}
/*
		int tmp = timeGetTime() - time;

		time3.back() += tmp;

		time  = timeGetTime();
*/
		return true;
	}
	VIRTUAL	VOID			pOnRelease()
	{
	}
	VIRTUAL	VOID			pOnEvent( TEventMessage msg )
	{

		//�}�E�X�̃��C��	
		STATIC CVector	MouseLine;

		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			IF( message == WM_ACTIVATE )
			{
				IF(m_keybord!=NULL)m_keybord->SetDeviceAcquire(true);
			}
			IF( message == WM_MOUSEMOVE)
			{
				//�I�_�x�N�g���̃Z�b�g
				MouseLine.SetVectorEnd( Vector( LOWORD(lParam) , HIWORD(lParam) , 0  ));
			}
			IF( message == WM_LBUTTONDOWN)
			{
				//�n�_�x�N�g���̃Z�b�g
				MouseLine.SetVectorStart( Vector( LOWORD(lParam)  , HIWORD(lParam)  , 0  ) );
				MouseLine.SetVectorEnd( Vector( LOWORD(lParam), HIWORD(lParam)  , 0 ) );
			}
			IF( message == WM_LBUTTONUP)
			{
				//�ǉ�
				Vector vec = MouseLine.GetVectorEnd()-MouseLine.GetVectorStart();
				LPTkyObstaleView view = IObstaleView::CreateObstaleView(MouseLine.GetVectorStart() + vec/2);
				LPPhysicsCircleModel model = view->GetModel();
				IF(vec.GetLarge()/2 > 1)model->SetCircle(vec.GetLarge()/2);	 
				else model->SetCircle(10);
				model->SetFrictional(ModelFrictional);	   
				model->SetElastic(ModelElastic);
				FLOAT by =  (vec.GetLarge()/2) / 50.0f;
				IF(by <= 0.01)by = 0.01f;
				model->SetWeight(ModelWeight*by);

				//�x�N�g������
				MouseLine = CVector();

				//VIEW�̒ǉ����b�Z�[�W�̑��M
			    TEventMessage send;
				send.m_message = "ADD_VIEW";
				send.m_value.resize(1);
				send.m_value[0] = (DWORD)view;
				CEventManager::StaticAddEventMessage(send);
			}

		}
		EIF(msg.m_message == "COMMON_LOGIC_STRING" )
		{
			m_str.push_back(  string(((LPSTR)msg.m_value[0])) );
		}

	}

	//�R���X�g���N�^�E�f�X�g���N�^
	CCommonWindowsApplicationLayer()
	{
		m_CommonGameLogic = NULL;
		m_pDtata = NULL;
		m_pFont = NULL;	
		m_keybord = NULL;
	}
	VIRTUAL ~CCommonWindowsApplicationLayer()
	{
	}

public:

	STATIC LPTkyCommonWindowsApplicationLayer CreateCommonWindowsApplicationLayer()
	{
		return new CCommonWindowsApplicationLayer();
	}

};

