
#include "FPSGameViews.h"

//����
const STATIC float WorldWolk3D = 0.001f;
//�W�����v��
const STATIC float WorldJump3D = 0.1f;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�L�����N�^�[�r���[
//�@�L�����N�^���̊�{�ƂȂ��
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	IFPSCharactorView::IFPSCharactorView()
	{
		//�L�����N�^���
		m_chara = CCharactor::CreateCharactor();
		m_weapon = CWeapon::CreateWeapon(m_chara);
	}
	IFPSCharactorView::IFPSCharactorView(const IFPSCharactorView& f) :IGameView(f)
	{
		assert(m_chara);
		assert(m_weapon);
		*m_chara = *f.GetCharactor();
		*m_weapon = *f.GetWeapon();
	}
	IFPSCharactorView::~IFPSCharactorView()
	{
		m_chara->DestroyOwnObject();
		m_weapon->DestroyOwnObject();
	}
	//�ݒ�(�f�[�^�����邾���Ȃ̂Ń|�C���^�Ƃ��Ă݂Ȃ�)
	BOOL IFPSCharactorView::SetCharactorWeaponData( CCharactor* c, CWeapon* w)
	{
		IF(c!=NULL && m_chara!=NULL)*m_chara = *c;
		IF(w!=NULL && m_weapon!=NULL)*m_weapon = *w;
		m_weapon->SetChara(m_chara);
		return true;
	}
//GameView �v�f
	BOOL IFPSCharactorView::onCreate() 	//������
	{
		return true;
	}
	BOOL IFPSCharactorView::onUpdate(float step)	//�X�V
	{
		m_chara->UpdateCharactor(step);
		m_weapon->UpdateAttack(step);

		//��������ł������
		IF( m_chara->IsDead() )
		{
			DELETE_VIEW_MESSAGE(this);
			return true;
		}

		return true;
	}
	VOID IFPSCharactorView::onRender()	//�`��
	{
		m_chara->DrawCharactor();
		m_weapon->DrawAttack();
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID IFPSCharactorView::onEvent( TEventMessage msg )
	{
		//�U����������āA����ɂ���čU�����J�n����

		//�I����������ďI������( �I���̓|�C���^�ɂ���čs���A�C�x���g�͗��Ȃ���������Ȃ� )


		//���񂾏ꍇ�͎��񂾃C�x���g
		//���񂾂��Ƃ�`����

	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSHeroView :: �L�����N�^�[�r���[
//�@��l���p�@���̂Ƃ���͉��ɂ����邱�Ƃ͂Ȃ�
//�@�@�@�@�@�@��l�������񂾏ꍇ�͓���Ȃ��Ƃ��N����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSHeroView::CFPSHeroView()
	{
		//�ڐG���̃��b�Z�[�W�ݒ�
		m_chara->SetCollisionMessage( TEventMessage::Initializel( "HERO_CHARACTOR" , (DWORD)m_chara ) );
	}
	CFPSHeroView::CFPSHeroView(const CFPSHeroView& f) :IFPSCharactorView(f)
	{
	}
	CFPSHeroView::~CFPSHeroView()
	{
	}
//GameView �v�f
	BOOL CFPSHeroView::onCreate()  	//������
	{
		//�e�Ăяo��
		IFPSCharactorView::onCreate();

		m_chara->SetCollisionMessage( TEventMessage::Initializel( "HERO_CHARACTOR" , (DWORD)m_chara ) );

		return true;
	}
	BOOL CFPSHeroView::onUpdate(float step)	//�X�V
	{
		//�e�Ăяo��
		return IFPSCharactorView::onUpdate(step);


	}
	VOID CFPSHeroView::onRender()	//�`��
	{
		//�e�Ăяo��
		IFPSCharactorView::onRender();
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFPSHeroView::onEvent( TEventMessage msg )
	{
		//�e�Ăяo��
		IFPSCharactorView::onEvent(msg);

		//���ʃC�x���g
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//���{�^���������ꂽ��e����
			IF( message == WM_LBUTTONDOWN )
			{
				m_weapon->Attack( Vector( (float)LOWORD(lParam) , (float)HIWORD(lParam) , 0  ) );


//				LPTkyBulletView b = CBulletView::CreateBullet( m_weapon );
//				b->GetBullet()->SetPos( m_chara->GetPos() );
//				CEventManager::StaticAddEventMessage( "LOGIC_ADDVIEW" , (DWORD)b );
			}
		}

	}
//�Ǝ��v�f
	LPTkyFPSHeroView CFPSHeroView::CreateFPSHeroView()
	{
		return new CFPSHeroView();
	}
	LPTkyFPSHeroView CFPSHeroView::CreateFPSHeroView( CCharactor* c, CWeapon* w)
	{
		LPTkyFPSHeroView h = CreateFPSHeroView();
		h->SetCharactorWeaponData(c,w);
		return h;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFPSEnemyView :: �L�����N�^�[�r���[
//�@�K�p�@���̂Ƃ���͉��ɂ����邱�Ƃ͂Ȃ�
//�@�@�@�@�@�@�G�����񂾂�����Ȃ���΂Ȃ�Ȃ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSEnemyView::CFPSEnemyView()
	{
		//�ڐG���̃��b�Z�[�W�ݒ�
		m_chara->SetCollisionMessage( TEventMessage::Initializel( "ENEMY_CHARACTOR" , (DWORD)m_chara ) );
	}
	CFPSEnemyView::CFPSEnemyView(const CFPSEnemyView& f) :IFPSCharactorView(f)
	{
	}
	CFPSEnemyView::~CFPSEnemyView()
	{
	}
//GameView �v�f
	BOOL CFPSEnemyView::onCreate()  	//������
	{
		//�e�Ăяo��
		IFPSCharactorView::onCreate();

		m_chara->SetCollisionMessage( TEventMessage::Initializel( "ENEMY_CHARACTOR" , (DWORD)m_chara ) );

		return true;
	}
	BOOL CFPSEnemyView::onUpdate(float step)	//�X�V
	{
		//�e�Ăяo��
		return IFPSCharactorView::onUpdate(step);
	}
	VOID CFPSEnemyView::onRender()	//�`��
	{
		//�e�Ăяo��
		IFPSCharactorView::onRender();
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFPSEnemyView::onEvent( TEventMessage msg )
	{
		//�e�Ăяo��
		IFPSCharactorView::onEvent(msg);

		//�G�`�F�b�N
		IF( msg.m_message == "ENEMY_CHECK" )
		{
			(*((INT*)msg.m_value[0]))++;
		}

		//�I��
		EIF( msg.m_message == "END_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}

		//�`�F�b�N�ŏI���
		EIF( msg.m_message == "RESET_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}

	}
//�Ǝ��v�f
	LPTkyFPSEnemyView CFPSEnemyView::CreateFPSEnemyView()
	{
		return new CFPSEnemyView();
	}
	LPTkyFPSEnemyView CFPSEnemyView::CreateFPSEnemyView( CCharactor* c, CWeapon* w)
	{
		LPTkyFPSEnemyView h = CreateFPSEnemyView();
		h->SetCharactorWeaponData(c,w);
		return h;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CBulletView 
//�@�e�p�A�ǉ����ꂽ��ɏ�Q���Ɣ�r������������܂�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CBulletView::CBulletView()
	{
		//�������
		m_Bullet = CBullet::CreateBullet();
		//���ˈʒu
		m_BatteryPosition = Vector();
	}
	CBulletView::CBulletView(const CBulletView& b) : IGameView(b)
	{
		assert(m_Bullet);
		*m_Bullet = *b.GetBullet();
	}
	CBulletView::~CBulletView()
	{
		IF( m_Bullet != NULL )m_Bullet->DestroyOwnObject();
	}
//GameView �v�f
	BOOL CBulletView::onCreate()		//������
	{
		m_Bullet->SetCollisionMessage( TEventMessage::Initializel( "BULLET_MODEL" , (DWORD)m_Bullet ) );

		//�����ʒu�̐ݒ�
		m_Bullet->SetBatteryPosition( m_Bullet->GetPos() ) ;
		return true;
	}
	BOOL CBulletView::onUpdate(float step)	//�X�V
	{
		IF( m_Bullet->IsFinish() )
		{
			CEventManager::StaticAddEventMessage( "LOGIC_DELETEVIEW" , (DWORD)(this) );
		}

		return true;
	}
	VOID CBulletView::onRender()	//�`��
	{
		//�����悤�`��
		LPTKY3D d = Init3D::CreateInit3D();
		d->SetColor(Vector(0,0,0));
		d->DrawLine3D( m_Bullet->GetPos()-Vector(0,0.1f,0) , m_Bullet->GetPos() + m_Bullet->GetVel() , m_Bullet->GetCircle() );

		//�����̕`��==============================

		Vector m_pos  = m_Bullet->GetPos();

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"�e�̋������:%lf X:%lf Y:%lf Z:%lf ",1.0f,m_pos.x,m_pos.y,m_pos.z );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,350,str);

		//========================================

		//	moderunobyouga 

		IF( m_Bullet->GetChara()!=NULL )
		{
			IF( m_Bullet->GetChara()->GetCharaInfo().m_ai == TEXT("THROW_AI")  )
			{
				Vector rotate = Vector();		//�\����]
				Vector attackdirection = m_Bullet->GetVel();
				rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;

				m_Bullet->GetChara()->GetAnime().DrawAnimationData( TEXT("CHARACTOR_THROW") , m_pos , rotate , Vector(1,1,1) );

			}
		}

	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CBulletView::onEvent( TEventMessage msg )
	{

		//�`�F�b�N�ŏI���
		IF( msg.m_message == "END_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}
		//�`�F�b�N�ŏI���
		IF( msg.m_message == "RESET_STAGE" )
		{
			DELETE_VIEW_MESSAGE(this);
		}

		EIF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			IF( m_Bullet->GetChara() == ((LPTkyFPSCharactorView)msg.m_value[0])->GetCharactor() )
			{
				m_Bullet->SetChara(NULL);
			}
		}


	}
//�Ǝ��v�f
	//����
	LPTkyBulletView CBulletView::CreateBullet( )
	{
		return new CBulletView();
	}
	LPTkyBulletView CBulletView::CreateBullet( LPTkyWeapon w )
	{
		LPTkyBulletView b = CreateBullet( );
		b->GetBullet()->SetBulletInfo( w->GetBulletInfo() );
		b->GetBullet()->SetChara(w->GetChara());
		b->SetWeapon(w);
		return b;
	}
	LPTkyBulletView CBulletView::CreateBullet( CBullet* c, LPTkyWeapon w )
	{
		LPTkyBulletView b = CreateBullet( );
		b->SetBullet(c);
		b->SetWeapon(w);
		b->GetBullet()->SetChara( w->GetChara() );
		return b;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMapView 
//�@�}�b�v�͂��������ɂ��邾���ł����񂾂�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSMapView::CFPSMapView()
	{
		m_map = NULL;
	}
	CFPSMapView::CFPSMapView(const CFPSMapView& f)
	{
		m_map = f.GetModel();
	}
	CFPSMapView::~CFPSMapView()
	{
	}
//GameView �v�f
	BOOL CFPSMapView::onCreate()	//������
	{
		return true;
	}
	BOOL CFPSMapView::onUpdate(float step)	//�X�V
	{
		return true;
	}
	VOID CFPSMapView::onRender()	//�`��
	{
		m_map->DrawMesh( Vector() , Vector() );
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFPSMapView::onEvent( TEventMessage msg )
	{
	}
//�Ǝ��v�f
	//����
	LPTkyFPSMapView CFPSMapView::CreateMap( )
	{
		return new CFPSMapView();
	}
	LPTkyFPSMapView CFPSMapView::CreateMap( LPTkyXFile x)
	{
		LPTkyFPSMapView f = CreateMap( );
		f->SetModel(x);
		return f;
	}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEffectView 
//�@�I������玩���Ŏ����I�ɏI����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CEffectView::CEffectView()
	{
		m_Effect = CGameEffect();
		m_pos = Vector();
		m_rotate = Vector();
		m_expand = Vector();
		m_type = TKYTEXTURE_TYPE_3D_MODE;
	}
	CEffectView::CEffectView(const CEffectView& e)
	{
		m_Effect = *e.GetEffect();
		m_pos = e.GetPos();
		m_rotate = e.GetRotate();
		m_expand = e.GetExpand();
	}
	CEffectView::~CEffectView()
	{
	}
//GameView �v�f
	BOOL CEffectView::onCreate()	//������
	{
		return true;
	}
	BOOL CEffectView::onUpdate(float step)	//�X�V
	{
		//�X�V
		m_Effect.UpdateEffect(step);
		//�I���`�F�b�N
		IF(m_Effect.IsOneLoopEnd())
		{
			DELETE_VIEW_MESSAGE(this);
		}
		return true;
	}
	VOID CEffectView::onRender()	//�`��
	{
		IF( m_type == TKYTEXTURE_TYPE_2D_MODE )
		{
			m_Effect.DrawEffect2D(m_pos);
		}
		EIF( m_type == TKYTEXTURE_TYPE_3D_MODE )
		{
			m_Effect.DrawEffect(m_pos,m_rotate,m_expand);
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CEffectView::onEvent( TEventMessage msg )
	{
	}
//�Ǝ��v�f
	//����
	LPTkyEffectView CEffectView::CreateEffect( )
	{
		return new CEffectView();
	}
	LPTkyEffectView CEffectView::CreateEffect( CGameEffect *Effect ,	Vector pos , Vector m_rotate , Vector expand , TKYTEXTURE_TYPE type )
	{
		LPTkyEffectView e = CreateEffect( );
		e->SetEffect( Effect );
		e->SetPos(pos);
		e->SetRotate(m_rotate);
		e->SetExpand(expand);
		e->SetTextureType(type);
		return e;
	}



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//IFpsAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	IFpsAIView::IFpsAIView()
	{
		m_charaview = NULL;
	}
	IFpsAIView::IFpsAIView(const IFpsAIView & f)
	{
		m_charaview = f.GetCharactorView();
	}
	IFpsAIView::~IFpsAIView ()
	{
	}
	VOID IFpsAIView::onEvent( TEventMessage msg )
	{
		IF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			//�S���̓G�������玩��������
			IF( (DWORD)m_charaview == msg.m_value[0] )
			{
				DELETE_VIEW_MESSAGE(this);
			}
		}

	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsMoveAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsMoveAIView::CFpsMoveAIView()
	{
		m_rotate = Vector();
		
		m_Direction = NULL;
		m_pos = NULL;

		m_mouse = DIMOUSESTATE();
		
		m_BaseVector = Vector(0,0,1);
	}
	CFpsMoveAIView::CFpsMoveAIView(const CFpsMoveAIView& f) :IFpsAIView(f)
	{
		m_rotate = f.m_rotate;
		
		m_Direction = f.m_Direction;
		m_pos = f.m_pos;

		m_mouse = f.m_mouse;

		m_BaseVector = f.m_BaseVector;
	}
	CFpsMoveAIView::~CFpsMoveAIView()
	{
	}
//GameView �v�f
	BOOL CFpsMoveAIView::onCreate()	//������
	{
		return true;
	}
	BOOL CFpsMoveAIView::onUpdate(float step)	//�X�V
	{

		*m_pos = m_charaview->GetCharactor()->GetPos();

		Vector v = (m_BaseVector.GetRadVec() - Vector(0,0,1).GetRadVec())*(-1);

		*m_Direction = Vector(0,0,1).Rotate( Vector() , m_rotate.x, m_rotate.y + D3DXToDegree(v.y) , m_rotate.z);

		//���̍��W�𑗐M����
		CEventManager::StaticAddEventMessage( "HERO_POSITION" , (DWORD)( m_pos ) );

		return true;
	}
	VOID CFpsMoveAIView::onRender()	//�`��
	{

		//�����̕`��==============================

		CFONT::CreateFontW(NULL)->SetColor(255,255,255);
		char str[200];
		sprintf_s(str,"�������:%lf X:%lf Y:%lf Z:%lf ",1.0f,m_pos->x,m_pos->y,m_pos->z );
		CFONT::CreateFontW(NULL)->DrawFontA(10,130,str);

		//========================================

		//�����̕`��==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		sprintf_s(str,"�}�E�X�萔 X:%d Y:%d Z:%d ",m_mouse.lX,m_mouse.lY,m_mouse.lZ );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

		//========================================
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFpsMoveAIView::onEvent( TEventMessage msg )
	{

		IFpsAIView::onEvent(msg);

		//���f�����
		LPPhysicsCircleModel model = this->GetCharactorView()->GetCharactor();

		//�ړ����W
		Vector move = m_Direction->Normal();
		move.y = 0;

		//�L�[�A�}�E�X����
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

		IF( msg.m_message == "MOVE_STOP"  )
		{
			//�X�g�b�v
			m_charaview->GetCharactor()->SetAcc(Vector());
			m_charaview->GetCharactor()->SetVel(Vector());
		}

	}
//�Ǝ��v�f
	//����
	LPTkyFpsMoveAIView CFpsMoveAIView::CreateFpsMoveAI( )
	{
		return new CFpsMoveAIView();
	}
	LPTkyFpsMoveAIView CFpsMoveAIView::CreateFpsMoveAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction )
	{
		LPTkyFpsMoveAIView m = CreateFpsMoveAI( );
		m->SetCharactorView( chara );
		m->SetCmameraVector( pos , Direction );
		return m;
	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsAutoAIView 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//�J�����̍��W�A����
	Vector *m_pos,*m_Direction;

	//�I�[�g���
	LPTkyAutoKeyInput			m_keyinfo;
	LPTkyAutoMouseInput		m_mouseinfo;
*/
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsAutoAIView::CFpsAutoAIView(wstring name)
	{
		m_pos = NULL;
		m_Direction = NULL;
		m_keyinfo = CAutoKeyInput::CreateAutoKeyInput();
		m_mouseinfo = CAutoMouseInput::CreateAutoMouseInput();
		m_run = true;
		m_file = name;
	}
	CFpsAutoAIView::CFpsAutoAIView(const CFpsAutoAIView& t)
	{
		m_pos = t.m_pos;
		m_Direction = t.m_Direction;
		*m_keyinfo = *(t.m_keyinfo);
		*m_mouseinfo = *(t.m_mouseinfo);
		m_run = t.m_run;
	}
	CFpsAutoAIView::~CFpsAutoAIView()
	{
		m_keyinfo->DestroyOwnObject();
		m_mouseinfo->DestroyOwnObject();
	}
//GameView �v�f
	BOOL GetKeyInfo( wifstream *read, TAutoKeyInput *t)
	{
		wstring wstr;

		WHILE(!read->eof())
		{
			WCHAR word;
			read->get(word);
			IF( word == TEXT('[') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT(']') )
					{
						//���l�̎擾
						swscanf_s(wstr.c_str(),TEXT("%u"),&t->m_eTkyAutoKey);
						break;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('<') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT('>') )
					{
						//���l�̎擾
						swscanf_s(wstr.c_str(),TEXT("%u"),&t->m_FrameNum);						
						return false;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('E') )
			{
				WCHAR w[10];
				read->getline(w,10);
				//���[�v�I��
				return true;
			}
		}
		//���[�v�I��
		return false;
	}
	BOOL GetMouseInfo( wifstream *read, TAutoMouseInput *t)
	{
		wstring wstr;

		//�ʒu�i�[
		Vector v = Vector();

		WHILE(!read->eof())
		{
			WCHAR word;
			read->get(word);

			//���W���
			IF( word == TEXT('[') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT(']') )
					{
						//���l�̎擾
						swscanf_s(wstr.c_str(),TEXT("%f,%f,%f"),&v.x,&v.y,&v.z);
						break;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('<') )
			{
				wstr.clear();
				WHILE(!read->eof())
				{
					WCHAR word;
					read->get(word);
					IF( word == TEXT('>') )
					{
						//���l�̎擾
						swscanf_s(wstr.c_str(),TEXT("%u"),&t->m_FrameNum);						
						t->m_MousePos = v;
						return false;
					}
					else
					{
						wstr += word;
					}
				}
			}
			EIF( word == TEXT('E') )
			{
				WCHAR w[10];
				read->getline(w,10);
				//���[�v�I��
				return true;
			}
		}
		//���[�v�I��
		return false;
	}
	BOOL CFpsAutoAIView::onCreate()	//������
	{
		//�擾������
		wifstream read;
		read.open(m_file.c_str());

		//��{�����i�[�p
		WCHAR wstr[MAX_PATH];
		//�Ō�܂�
		WHILE(!read.eof())
		{
			//�f�[�^�擾
			read.getline(wstr,MAX_PATH);

			IF( wstring(wstr) == TEXT("KEYINFO") )
			{
				WHILE(!read.eof())
				{
					TAutoKeyInput t;
					IF( GetKeyInfo(&read,&t) )
					{
						//�I������������
						break;
					}
					m_keyinfo->AddKeyInput(t);
				}
			}
			EIF( wstring(wstr) == TEXT("MOUSEINFO") )
			{
				WHILE(!read.eof())
				{
					TAutoMouseInput t;
					IF( GetMouseInfo(&read,&t) )
					{
						//�I������������
						break;
					}
					m_mouseinfo->AddMouseInput(t);
				}
			}

		}
		return true;
	}
	BOOL CFpsAutoAIView::onUpdate(float step)	//�X�V
	{

		//���f�����
		LPPhysicsCircleModel model = m_charaview->GetCharactor();

		IF(m_run)
		{
			//�L�[���
			DWORD key = m_keyinfo->GetNowKeyInput().m_eTkyAutoKey;

			//�}�E�X���
			Vector move = m_mouseinfo->GetNowMouseInput().Normal();
			move.y = 0;

						IF( key & TKYKEY_RIGHT )	//�E�ړ�
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*-WorldWolk3D );
		}
						IF( key & TKYKEY_LEFT )	//���ړ�
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*WorldWolk3D );
		}
						IF( key & TKYKEY_UP )		//�O�i
		{
			model->Impact( move*WorldWolk3D );
		}
						IF( key & TKYKEY_DOWN )	//���
		{
			model->Impact( move*-WorldWolk3D );
		}
		}
		else
		{
			model->SetVel(Vector());
			model->SetAcc(Vector());
		}

		//���W���
		*m_pos = model->GetPos();
		//�}�E�X���
		*m_Direction = m_mouseinfo->GetNowMouseInput();

		IF(m_run)
		{

			//����i�߂�
			bool end;
			end = m_keyinfo->NextFrame();
			end = m_mouseinfo->NextFrame();

			//�V���[�e�B���O�̏I��
			IF(!end){
				CEventManager::StaticAddEventMessage("END_SHOOTING_STAGE",NULL);
			}

		}

		//���̍��W�𑗐M����
		CEventManager::StaticAddEventMessage( "HERO_POSITION" , (DWORD)( m_pos ) );

		return true;
	}
	VOID CFpsAutoAIView::onRender()	//�`��
	{
		//�����̕`��==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"���̃t���[�� Mouse:%d, Key:%d", m_mouseinfo->GetNowFrame() ,  m_keyinfo->GetNowFrame() );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,450,str);

		//========================================
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFpsAutoAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		IF( msg.m_message == "MOVE_STOP"  )
		{
			m_charaview->GetCharactor()->SetAcc(Vector());
			m_charaview->GetCharactor()->SetVel(Vector());
			m_run = false;
		}
		EIF( msg.m_message == "MOVE_START"  )
		{
			m_run = true;
		}
	}
	//����
	LPTkyFpsAutoAIView CFpsAutoAIView::CreateFpsAutoAI( wstring name )
	{
		return new CFpsAutoAIView(name);
	}
	LPTkyFpsAutoAIView CFpsAutoAIView::CreateFpsAutoAI( LPTkyFPSCharactorView chara , Vector *pos , Vector *Direction , wstring name )
	{
		LPTkyFpsAutoAIView f = CreateFpsAutoAI(name);
		f->SetCharactorView( chara );
		f->SetCmameraVector( pos , Direction );
		return f;
	}

	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAIView 
//�@�G�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyAIView::CFpsEnemyAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyAIView::CFpsEnemyAIView(const CFpsEnemyAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyAIView::~CFpsEnemyAIView()
	{
	}
//GameView �v�f
	BOOL CFpsEnemyAIView::onCreate()	//������
	{
		return true;
	}
	BOOL CFpsEnemyAIView::onUpdate(float step)	//�X�V
	{
		//�L�������̍X�V
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//�\����]
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//�L�������擾
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//��]�̐ݒ�
			chara.m_rotate = rotate;
			//�U�������̐ݒ�
			chara.m_AttackDirection = attackdirection;
			//�c���@������
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		const float LENGTH = 2;
		IF( m_heropos != NULL )
		{
			Vector size = *m_heropos - m_charaview->GetCharactor()->GetPos();
			
			//�߂Â�����
			IF(  size.GetLarge() > LENGTH  )
			{
				//�L�����̈ړ�
				m_charaview->GetCharactor()->Impact(  size.Normal() *WorldWolk3D );
			}
			else
			{
				//���x�Œ�
				m_charaview->GetCharactor()->SetVel(Vector());
				//�U�����I����Ă����玟�U��
				IF( m_charaview->GetWeapon()->EndAttack() )
				{

					m_charaview->GetWeapon()->Attack(Vector());

				}
			}

		}

		return true;
	}
	VOID CFpsEnemyAIView::onRender()	//�`��
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//�����̕`��==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"��]���  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFpsEnemyAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//��l���̍��W
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//�Ǝ��v�f
	//����
	LPTkyFpsEnemyAIView CFpsEnemyAIView::CreateFpsEnemyAI( )
	{
		return new CFpsEnemyAIView();
	}
	LPTkyFpsEnemyAIView CFpsEnemyAIView::CreateFpsEnemyAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyAIView e = CreateFpsEnemyAI( );
		e->SetCharactorView( chara );
		return e;
	}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyAttackLeaveAIView 
//�@�G�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyAttackLeaveAIView::CFpsEnemyAttackLeaveAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyAttackLeaveAIView::CFpsEnemyAttackLeaveAIView(const CFpsEnemyAttackLeaveAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyAttackLeaveAIView::~CFpsEnemyAttackLeaveAIView()
	{
	}
//GameView �v�f
	BOOL CFpsEnemyAttackLeaveAIView::onCreate()	//������
	{
		return true;
	}
	BOOL CFpsEnemyAttackLeaveAIView::onUpdate(float step)	//�X�V
	{
		//�L�������̍X�V
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//�\����]
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//�L�������擾
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//��]�̐ݒ�
			chara.m_rotate = rotate;
			//�U�������̐ݒ�
			chara.m_AttackDirection = attackdirection;
			//�c���@������
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		const float LENGTH = 2;
		IF( m_heropos != NULL )
		{
			Vector size = *m_heropos - m_charaview->GetCharactor()->GetPos();
			
			//�߂Â�����
			IF(  size.GetLarge() > LENGTH  )
			{
				//�L�����̈ړ�
				m_charaview->GetCharactor()->Impact(  size.Normal() *WorldWolk3D );
			}
			else
			{
				//���x�Œ�
				m_charaview->GetCharactor()->SetVel(Vector());
				//�U�����I����Ă����玟�U��
				IF( m_charaview->GetWeapon()->EndAttack() )
				{

					m_charaview->GetWeapon()->Attack(Vector());

				}
			}

		}

		return true;
	}
	VOID CFpsEnemyAttackLeaveAIView::onRender()	//�`��
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//�����̕`��==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"��]���  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFpsEnemyAttackLeaveAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//��l���̍��W
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//�Ǝ��v�f
	//����
	LPTkyFpsEnemyAttackLeaveAIView CFpsEnemyAttackLeaveAIView::CreateFpsEnemyAttackLeaveAI( )
	{
		return new CFpsEnemyAttackLeaveAIView();
	}
	LPTkyFpsEnemyAttackLeaveAIView CFpsEnemyAttackLeaveAIView::CreateFpsEnemyAttackLeaveAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyAttackLeaveAIView e = CreateFpsEnemyAttackLeaveAI( );
		e->SetCharactorView( chara );
		return e;
	}





	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyThrowAIView 
//�@�G�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyThrowAIView::CFpsEnemyThrowAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyThrowAIView::CFpsEnemyThrowAIView(const CFpsEnemyThrowAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyThrowAIView::~CFpsEnemyThrowAIView()
	{
	}
//GameView �v�f
	BOOL CFpsEnemyThrowAIView::onCreate()	//������
	{
		return true;
	}
	BOOL CFpsEnemyThrowAIView::onUpdate(float step)	//�X�V
	{
		//�L�������̍X�V
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//�\����]
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//�L�������擾
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//��]�̐ݒ�
			chara.m_rotate = rotate;
			//�U�������̐ݒ�
			chara.m_AttackDirection = attackdirection;
			//�c���@������
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		static DWORD time = timeGetTime();

		//��b�����ɍU��
		IF( timeGetTime() - time  >= 3000 )
		{
			time  = timeGetTime();
			IF( m_charaview->GetWeapon()->EndAttack() )
			{
				m_charaview->GetWeapon()->Attack(Vector());
			}
		}

		static string state = m_charaview->GetWeapon()->GetNowState();

		state = m_charaview->GetWeapon()->GetNowState();

		return true;
	}
	VOID CFpsEnemyThrowAIView::onRender()	//�`��
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//�����̕`��==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"��]���  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFpsEnemyThrowAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//��l���̍��W
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//�Ǝ��v�f
	//����
	LPTkyFpsEnemyThrowAIView CFpsEnemyThrowAIView::CreateFpsEnemyThrowAI( )
	{
		return new CFpsEnemyThrowAIView();
	}
	LPTkyFpsEnemyThrowAIView CFpsEnemyThrowAIView::CreateFpsEnemyThrowAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyThrowAIView e = CreateFpsEnemyThrowAI( );
		e->SetCharactorView( chara );
		return e;
	}







	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CFpsEnemyStopAttackAIView 
//�@�G�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFpsEnemyStopAttackAIView::CFpsEnemyStopAttackAIView()
	{
		m_heropos = NULL;
	}
	CFpsEnemyStopAttackAIView::CFpsEnemyStopAttackAIView(const CFpsEnemyStopAttackAIView& t) :IFpsAIView(t)
	{
	}
	CFpsEnemyStopAttackAIView::~CFpsEnemyStopAttackAIView()
	{
	}
//GameView �v�f
	BOOL CFpsEnemyStopAttackAIView::onCreate()	//������
	{
		return true;
	}
	BOOL CFpsEnemyStopAttackAIView::onUpdate(float step)	//�X�V
	{
		//�L�������̍X�V
		IF( m_heropos != NULL )
		{
			Vector rotate = Vector();		//�\����]
			Vector attackdirection = *m_heropos - m_charaview->GetCharactor()->GetPos();
			rotate.y =  -( attackdirection ).GetRadVec().y - D3DX_PI/2;
			//�L�������擾
			TCharaInfo chara =  m_charaview->GetCharactor()->GetCharaInfo();
			//��]�̐ݒ�
			chara.m_rotate = rotate;
			//�U�������̐ݒ�
			chara.m_AttackDirection = attackdirection;
			//�c���@������
			m_charaview->GetCharactor()->SetCharaInfo(chara);
		}

		static DWORD time = timeGetTime();

		//��b�����ɍU��
		IF( timeGetTime() - time  >= 3000 )
		{
			time  = timeGetTime();
			IF( m_charaview->GetWeapon()->EndAttack() )
			{
				m_charaview->GetWeapon()->Attack(Vector());
			}
		}


		return true;
	}
	VOID CFpsEnemyStopAttackAIView::onRender()	//�`��
	{

		Vector rotate = Vector();
		IF( m_heropos != NULL )
		{
			rotate.y =  -( *m_heropos - m_charaview->GetCharactor()->GetPos() ).GetRadVec().y - D3DX_PI/2;

			//�����̕`��==============================

//			CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//			char str[200];
//			sprintf_s(str,"��]���  :%lf ",rotate.y );
//			CFONT::CreateFontW(NULL)->DrawFontA(10,50,str);

			//========================================
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CFpsEnemyStopAttackAIView::onEvent( TEventMessage msg )
	{
		IFpsAIView::onEvent(msg);

		//��l���̍��W
		IF( msg.m_message == "HERO_POSITION" )
		{
			m_heropos = (Vector*)msg.m_value[0];
		}

	}
//�Ǝ��v�f
	//����
	LPTkyFpsEnemyStopAttackAIView CFpsEnemyStopAttackAIView::CreateFpsEnemyStopAttackAI( )
	{
		return new CFpsEnemyStopAttackAIView();
	}
	LPTkyFpsEnemyStopAttackAIView CFpsEnemyStopAttackAIView::CreateFpsEnemyStopAttackAI( LPTkyFPSCharactorView chara )
	{
		LPTkyFpsEnemyStopAttackAIView e = CreateFpsEnemyStopAttackAI( );
		e->SetCharactorView( chara );
		return e;
	}





















//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindowiew 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CTextWindowiew::CTextWindowiew()
	{
		m_ManageTextWindow = CManageTextWindow::CreateManageTextWinodw();
		m_ManageTextWindow->StartTextWinodw();
		m_NextTextWIndow = NULL;
	}
	CTextWindowiew::CTextWindowiew(const CTextWindowiew &m)
	{
		assert(this->m_ManageTextWindow);
		//�l��n��
		*this->m_ManageTextWindow = *m.m_ManageTextWindow;
		//���̂�͓n���Ȃ�
		m_NextTextWIndow = NULL;
	}
	CTextWindowiew::~CTextWindowiew ()
	{
		m_ManageTextWindow->DestroyOwnObject();
	}
//GameView �v�f
	BOOL CTextWindowiew::onCreate()	//������
	{

		return true;
	}
	BOOL CTextWindowiew::onUpdate(float step)	//�X�V
	{
		//�����e�L�X�g���I�������ۂɎ����������玟�֐i��
		IF( m_ManageTextWindow->GetState() == TKYMTW_WINODW_LEAVE )
		{
			IF( m_NextTextWIndow !=NULL )
			{
				//�I��
				DELETE_VIEW_MESSAGE(this);
				//�e�L�X�g���[�h�ł͂��߂�
				m_NextTextWIndow->GetManageTextWindow()->StartTextWinodw(TKYMTW_TEXT);
				//�r���[�̒ǉ�
				ADD_VIEW_MESSAGE( m_NextTextWIndow );
			}
		}

		//�ʏ퓮��
		IF( m_ManageTextWindow->UpdateTextWinodw(step) )
		{
			//�I��
			DELETE_VIEW_MESSAGE(this);
			//���̃r���[�̍쐬
			IF( m_NextTextWIndow !=NULL )
			{
				//���ʃ��[�h�ł͂��߂�
				m_NextTextWIndow->GetManageTextWindow()->StartTextWinodw();
				//�r���[�̒ǉ�
				ADD_VIEW_MESSAGE( m_NextTextWIndow );
			}
		}
		return true;
	}
	VOID CTextWindowiew::onRender()	//�`��
	{
		m_ManageTextWindow->DrawTextWindow();
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CTextWindowiew::onEvent( TEventMessage msg )
	{
	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTextWindoRangeDecisionView  
//�@�e�L�X�g�͈̔͂��m�F
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//�͈͏��Ǘ�
	CElementRange<LPTkyTextWindowiew> *m_range;
	//��l���̍��W�x�N�g��
	Vector *m_pos;
*/
	//�R���X�g���N�^�E�f�X�g���N�^
	CTextWindoRangeDecisionView::CTextWindoRangeDecisionView ()
	{
		m_range = new CElementRange<LPTkyTextWindowiew>();
		m_pos = NULL;
	}
	CTextWindoRangeDecisionView::CTextWindoRangeDecisionView (const CTextWindoRangeDecisionView  &t)
	{
		this->m_pos = t.m_pos;
		*this->m_range = *t.m_range;
	}
	CTextWindoRangeDecisionView::~CTextWindoRangeDecisionView  ()
	{
		list<TElementRange<LPTkyTextWindowiew>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyTextWindowiew>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//�v�f�̊J��
			WHILE( !it->m_Element.empty() )
			{
//				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.front()->DestroyOwnObject();
				it->m_Element.pop();
			}
			it++;
		}

		SAFE_DELETE( m_range );
	} 
//GameView �v�f
	BOOL CTextWindoRangeDecisionView::onCreate()	//������
	{
		return true;
	}
	BOOL CTextWindoRangeDecisionView::onUpdate(float step)	//�X�V
	{
		IF(m_range == NULL)return false;
		//����
		TElementRange<LPTkyTextWindowiew> t = m_range->Decision( *m_pos );
		//�k����Ȃ������玟�ɐi��
		IF(!t.m_Element.empty())
		{
			//���̕���������
			WHILE( !t.m_Element.empty() )
			{
				//����
				ADD_VIEW_MESSAGE( t.m_Element.front() );
				//�͂�
				t.m_Element.pop();
			}
			//����
			m_range->DeleteRange( *m_pos );
		}

		return false;
	}
	VOID CTextWindoRangeDecisionView::onRender()	//�`��
	{
		IF(m_range == NULL)return;
		list<TElementRange<LPTkyTextWindowiew>> mm = m_range->GetElement();

		list<TElementRange<LPTkyTextWindowiew>>::iterator im = mm.begin();
		WHILE( im != mm.end() )
		{
			Vector v = Vector( im->pos.x , m_pos->y+0.1f  ,  im->pos.z  );

			//����
			LPTKY3D d = Init3D::CreateInit3D();
			d->SetColor(Vector(0.5,0,0));
			d->DrawLine3D( *m_pos - Vector(0,0.1f,0) ,  v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r )  , 0.1f );

			//�͈�
			d->SetColor(Vector(1.0f,0,0));
			d->DrawLine3D( v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r) ,  v  , 0.1f );

			im++;
		}
	}

	//�J�������ǉ�
	VOID CTextWindoRangeDecisionView::Delete()
	{
		list<TElementRange<LPTkyTextWindowiew>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyTextWindowiew>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//�v�f�̊J��
			WHILE( !it->m_Element.empty() )
			{
				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.pop();
			}
			it++;
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CTextWindoRangeDecisionView::onEvent( TEventMessage msg )
	{
	}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyAppearRangeDecisionView  
//�@�e�L�X�g�͈̔͂��m�F
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�t�@�C������̓ǂݍ���

	//�R���X�g���N�^�E�f�X�g���N�^
	CEnemyAppearRangeDecisionView::CEnemyAppearRangeDecisionView ()
	{
		m_range = new CElementRange<LPTkyFPSEnemyView>();
		m_pos = NULL;
	}
	CEnemyAppearRangeDecisionView::CEnemyAppearRangeDecisionView (const CEnemyAppearRangeDecisionView  &t)
	{
		this->m_pos = t.m_pos;
		*this->m_range = *t.m_range;		
	}
	CEnemyAppearRangeDecisionView::~CEnemyAppearRangeDecisionView  ()
	{
		list<TElementRange<LPTkyFPSEnemyView>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyFPSEnemyView>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//�v�f�̊J��
			WHILE( !it->m_Element.empty() )
			{
//				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.front()->DestroyOwnObject();
				it->m_Element.pop();
			}
			it++;
		}
		SAFE_DELETE( m_range );
	}

//GameView �v�f
	BOOL CEnemyAppearRangeDecisionView::onCreate()	//������
	{
		return true;
	}
	BOOL CEnemyAppearRangeDecisionView::onUpdate(float step)	//�X�V
	{
		IF(m_range == NULL)return false;
		//����
		TElementRange<LPTkyFPSEnemyView> t = m_range->Decision( *m_pos );
		//�k����Ȃ�������
		IF( !t.m_Element.empty() )
		{
			//���̕���������
			WHILE( !t.m_Element.empty() )
			{
				//����
				ADD_VIEW_MESSAGE( t.m_Element.front() );
				//�͂�
				t.m_Element.pop();
			}
			//����
			m_range->DeleteRange( *m_pos );
		}
		return false;
	}
	VOID CEnemyAppearRangeDecisionView::onRender()	//�`��
	{
		IF(m_range == NULL)return;
		list<TElementRange<LPTkyFPSEnemyView>> mm = m_range->GetElement();

		list<TElementRange<LPTkyFPSEnemyView>>::iterator im = mm.begin();
		WHILE( im != mm.end() )
		{
			Vector v = Vector( im->pos.x , m_pos->y+0.1f  ,  im->pos.z  );

			//����
			LPTKY3D d = Init3D::CreateInit3D();
			d->SetColor(Vector(0,0,0.5f));
			d->DrawLine3D( *m_pos - Vector(0,0.1f,0) ,  v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r )  , 0.1f );

			//�͈�
			d->SetColor(Vector(0,0,1.0f));
			d->DrawLine3D( v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r) ,  v  , 0.1f );

			im++;
		}
	}

	VOID CEnemyAppearRangeDecisionView::Delete()
	{
		list<TElementRange<LPTkyFPSEnemyView>> tw =  m_range->GetElement();
		list<TElementRange<LPTkyFPSEnemyView>>::iterator it = tw.begin();
		WHILE( it != tw.end() )
		{
			//�v�f�̊J��
			WHILE( !it->m_Element.empty() )
			{
				DELETE_VIEW_MESSAGE( it->m_Element.front() );
				it->m_Element.pop();
			}
			it++;
		}
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CEnemyAppearRangeDecisionView::onEvent( TEventMessage msg )
	{
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CEnemyKillRangeDecisionView   //�V���O���g���Ȃ��
//�@�G��|�����߂ɂƂ܂�Ƃ���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//�͈͏��Ǘ�
	CElementRange<INT> *m_range;

	//��l���̍��W�x�N�g��
	Vector *m_pos;

	//�E�������G�̐�
	UINT m_KillEnemy;

*/
	//�R���X�g���N�^�E�f�X�g���N�^
	CEnemyKillRangeDecisionView::CEnemyKillRangeDecisionView ()
	{
		m_range = new CElementRange<INT>();
		m_pos = NULL;
		m_KillEnemy = 0;
	}
	CEnemyKillRangeDecisionView::CEnemyKillRangeDecisionView (const CEnemyKillRangeDecisionView  &t)
	{
		this->m_pos = t.m_pos;
		*this->m_range = *t.m_range;		
		this->m_KillEnemy = t.m_KillEnemy;
	}
	CEnemyKillRangeDecisionView::~CEnemyKillRangeDecisionView  ()
	{
		SAFE_DELETE( m_range );
	}

	//GameView �v�f
	BOOL CEnemyKillRangeDecisionView::onCreate()	//������
	{
		return true;
	}
	BOOL CEnemyKillRangeDecisionView::onUpdate(float step)	//�X�V
	{
		//�G�����Ȃ��Ȃ�����ĊJ����
		IF( m_KillEnemy == 0  )
		{
			CEventManager::StaticAddEventMessage( "MOVE_START" , 0 );
			m_KillEnemy = -1;
		}

		//����
		TElementRange<int> t = m_range->Decision( *m_pos );
		//�k����Ȃ�������
		IF( t.r > EPSILON )
		{

			//�G�̐��𐔂���
			m_KillEnemy = 0;
			CEventManager::StaticAddEventMessage( "ENEMY_CHECK" , (DWORD)&m_KillEnemy );
			//�X�g�b�v�𑗂�
			IF(m_KillEnemy<=0)m_KillEnemy = 0;
			CEventManager::StaticAddEventMessage( "MOVE_STOP" , 0 );

			//����
			m_range->DeleteRange( *m_pos );
		}
		return false;
	}
	VOID CEnemyKillRangeDecisionView::onRender()	//�`��
	{
		list<TElementRange<int>> mm = m_range->GetElement();
		list<TElementRange<int>>::iterator im = mm.begin();
		WHILE( im != mm.end() )
		{
			Vector v = Vector( im->pos.x , m_pos->y+0.1f  ,  im->pos.z  );

			//����
			LPTKY3D d = Init3D::CreateInit3D();
			d->SetColor(Vector(0,0.5f,0));
			d->DrawLine3D( *m_pos - Vector(0,0.1f,0) ,  v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r )  , 0.1f );

			//�͈�
			d->SetColor(Vector(0,0,1.0f));
			d->DrawLine3D( v + ( (*m_pos - Vector(0,0.1f,0) -v).Normal() * im->r) ,  v  , 0.1f );

			im++;
		}

		//�����̕`��==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"���̓G�̐�,%d",  m_KillEnemy );
//		CFONT::CreateFontW(NULL)->DrawFontA(100,100,str);

		//========================================
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CEnemyKillRangeDecisionView::onEvent( TEventMessage msg )
	{
		//�G�����񂾂�L�������炵��0�ɂȂ�����X�^�[�g�𑗂�
		IF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			IF(((LPTkyGameView)msg.m_value[0] )->GetName() ==  "FPS_Charactor_Enemy" )
			{
				m_KillEnemy--;
				IF( m_KillEnemy <= 0 )m_KillEnemy = 0;
			}
		}

	}




	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CMovementPathView  
//�@�ړ��̋O�Ղ�ۑ�
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//���
	LPTkyAutoKeyInput			m_KeyPath;
	LPTkyAutoMouseInput		m_MousePath;
	//���̃t���[����
	DWORD							m_NowFrame;
	//�t�@�C������(��������)
	wofstream						m_Write;							
*/
	//�R���X�g���N�^�E�f�X�g���N�^
	CMovementPathView::CMovementPathView ()
	{
		//�v�f�Ƃ�
		m_KeyPath = CAutoKeyInput::CreateAutoKeyInput();
		m_MousePath = CAutoMouseInput::CreateAutoMouseInput();
		m_NowKeyFrame = NULL;
		m_NowMouseFrame = NULL;
		m_mouse = DIMOUSESTATE();
		m_rotate = Vector();
		m_key = NULL;
		m_move.clear();
	}
	CMovementPathView::CMovementPathView (const CMovementPathView  & a)
	{
		*m_KeyPath = *a.m_KeyPath;
		*m_MousePath = *a.m_MousePath;
		m_NowKeyFrame = a.m_NowKeyFrame;
		m_NowMouseFrame = a.m_NowMouseFrame;
		m_mouse = a.m_mouse;
		m_rotate = a.m_rotate;
		m_key = a.m_key;
	}
	CMovementPathView::~CMovementPathView  ()
	{
		//�������݂���񂾂�����
		IF( MessageBoxW(NULL,TEXT("����̌��ʂ��������݂܂����H"),TEXT("�I��") , MB_OKCANCEL) == IDOK )
		{

			//�Ō�̏��
			//�L�[���ǉ�
			m_KeyPath->AddKeyInput( TAutoKeyInput( m_key , m_NowKeyFrame )  );
			//�}�E�X���̒ǉ�
			m_MousePath->AddMouseInput( 
				Vector(0,0,1).Rotate( Vector() , m_rotate.x , m_rotate.y , m_rotate.z ) ,
				m_NowMouseFrame  );

			//�������݊J�n
			wofstream write;
			write.open( m_move.c_str() );

			//���o��
			write << TEXT("KEYINFO") << endl;

			queue<TAutoKeyInput> key = m_KeyPath->GetKeyInfo();
			WHILE( !key.empty() )
			{
				write << TEXT("[") << key.front().m_eTkyAutoKey << TEXT("]<") << key.front().m_FrameNum << TEXT(">") << endl; 
				key.pop();
			}

			write << TEXT("END") << endl;
			write << TEXT("MOUSEINFO") << endl;
		
			queue<TAutoMouseInput> mouse = m_MousePath->getNouseInof();
			WHILE( !mouse.empty() )
			{
				write	<< TEXT("[") << mouse.front().m_MousePos.x << TEXT(",") << mouse.front().m_MousePos.y << TEXT(",") << mouse.front().m_MousePos.z << TEXT("]<")
							<< mouse.front().m_FrameNum << TEXT(">") << endl;
				mouse.pop();
			}

			write << TEXT("END") << endl;

			write.close();
		}
		//�I��
		m_KeyPath->DestroyOwnObject();
		m_MousePath->DestroyOwnObject();
	}
//GameView �v�f
	BOOL CMovementPathView::onCreate()	//������
	{
		return true;
	}
	BOOL CMovementPathView::onUpdate(float step)	//�X�V
	{
		//�i�߂�
		m_NowKeyFrame++;
		m_NowMouseFrame++;

		//�O�̐���
		static BYTE before_key = m_key;
		static DIMOUSESTATE before_mouse = m_mouse;

		//�L�[�}�E�X�̏�������
//		IF( m_NowKeyFrame >= 1 )
		IF( before_key != m_key )
		{
			//�L�[���ǉ�
			m_KeyPath->AddKeyInput( TAutoKeyInput( before_key , m_NowKeyFrame )  );
			//�t���[���̏�����
			m_NowKeyFrame = NULL;
		}
//		IF( m_NowMouseFrame >= 1 )
		IF( before_mouse.lX != m_mouse.lX || 
			before_mouse.lY != m_mouse.lY || 
			before_mouse.lZ != m_mouse.lZ)
		{

			Vector v = (m_base.GetRadVec() - Vector(0,0,1).GetRadVec())*(-1);

			//�}�E�X���̒ǉ�
			m_MousePath->AddMouseInput( 
				Vector(0,0,1).Rotate( Vector() , m_rotate.x , m_rotate.y + D3DXToDegree(v.y) , m_rotate.z ) ,
				m_NowMouseFrame  );

			//�t���[��������
			m_NowMouseFrame = 0;
		}

		//�L�[���
		before_key = m_key;
		before_mouse = m_mouse;

		m_key = 0;

		return true;
	}
	VOID CMovementPathView::onRender()	//�`��
	{

		//�����̕`��==============================

//		CFONT::CreateFontW(NULL)->SetColor(0,0,0);
//		char str[200];
//		sprintf_s(str,"���̃t���[��,%d,%d",  m_NowKeyFrame , m_NowMouseFrame );
//		CFONT::CreateFontW(NULL)->DrawFontA(10,250,str);

		//========================================

	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CMovementPathView::onEvent( TEventMessage msg )
	{
		//�L�[�C�x���g�Ńt���[����ς����艽�Ƃ������肷��
		//�}�E�X�C�x���g�Ń}�E�X�ɂ��Ă��



		//�L�[�A�}�E�X����
		string EventName = msg.m_message;

		IF(EventName == "KEY_RIGHT")	//�E�ړ�
		{m_key = m_key | TKYKEY_RIGHT;}
		
		IF(EventName == "KEY_LEFT")	//���ړ�
		{m_key = m_key | TKYKEY_LEFT;}
		
		IF(EventName == "KEY_UP")		//�O�i
		{m_key = m_key | TKYKEY_UP;}
		
		IF(EventName == "KEY_DOWN")	//���
		{m_key = m_key | TKYKEY_DOWN;}

		IF(EventName == "MOUSE_INFO")
		{
			m_mouse = *( (DIMOUSESTATE*)msg.m_value[0] );
			m_rotate += Vector( (float)m_mouse.lY / 10.0f , (float)m_mouse.lX / 10.0f , 0 ); 
		}

		EIF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			LPTkyGameView view = (LPTkyGameView)msg.m_value[0];

			IF( view->GetName() == "FPS_Charactor_Hero" )
			{
				DELETE_VIEW_MESSAGE(this);
			}
		}

	}









//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CAdventureWindowiew 
//�@FPS�悤��AI��S�������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CAdventureWindowiew::CAdventureWindowiew()
	{
		m_ManageAdventureWindow = CManageTextWindow::CreateManageTextWinodw();
		m_ManageAdventureWindow->StartTextWinodw();
		m_NextAdventureWIndow = NULL;

		//���s��
		m_adventure = TKYADVENTURE_SHOW;

	}
	CAdventureWindowiew::CAdventureWindowiew(const CAdventureWindowiew &m)
	{
		assert(this->m_ManageAdventureWindow);
		//�l��n��
		*this->m_ManageAdventureWindow = *m.m_ManageAdventureWindow;
		//���̂�͓n���Ȃ�
		m_NextAdventureWIndow = NULL;
		//���[�h
		m_adventure = m.m_adventure;
	}
	CAdventureWindowiew::~CAdventureWindowiew ()
	{
		m_ManageAdventureWindow->DestroyOwnObject();
	}
//GameView �v�f
	BOOL CAdventureWindowiew::onCreate()	//������
	{

		return true;
	}
	BOOL CAdventureWindowiew::onUpdate(float step)	//�X�V
	{
		SWITCH( m_adventure )
		{

			case TKYADVENTURE_SHOW:

				//�ʏ�X�V
				m_ManageAdventureWindow->UpdateTextWinodw(step);

				//�I�������
				IF( m_ManageAdventureWindow->GetState() == TKYMTW_WINODW_LEAVE )
				{
					//�S���\���ɂ���
					m_ManageAdventureWindow->EndTextWinodw();
					//�I�������I���ɂ���
					m_adventure = TKYADVENTURE_END;
				}

			break;
			case TKYADVENTURE_OPEN:

				//�I�[�v������END�ɂ���
				m_ManageAdventureWindow->EndTextWinodw();
				m_adventure = TKYADVENTURE_END;

			break;
			case TKYADVENTURE_END:
			break;

			case TKYADVENTURE_ENDPROCESS:

				//�I���v���Z�X
				IF( m_ManageAdventureWindow->UpdateTextWinodw( step ) )
				{
					DELETE_VIEW_MESSAGE(this);

					CEventManager::StaticAddEventMessage( "END_STAGE" , NULL  );

				}

			break;

			case TKYADVENTURE_ESCAPE:

				//�I��
				DELETE_VIEW_MESSAGE(this);

				//�e�L�X�g���[�h�ł͂��߂�
				m_NextAdventureWIndow->GetManageAdventureWindow()->StartTextWinodw(TKYMTW_TEXT);

				//�r���[�̒ǉ�
				ADD_VIEW_MESSAGE( m_NextAdventureWIndow );

			break;

		}

		return true;
	}
	VOID CAdventureWindowiew::onRender()	//�`��
	{
		m_ManageAdventureWindow->DrawTextWindow();
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CAdventureWindowiew::onEvent( TEventMessage msg )
	{

		//���ʃC�x���g
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//���{�^���������ꂽ��e����
			IF( message == WM_LBUTTONDOWN || message == WM_MOUSEWHEEL)
			{

				IF( m_adventure == TKYADVENTURE_SHOW )
				{
					m_adventure = TKYADVENTURE_OPEN;
				}
				EIF( m_adventure == TKYADVENTURE_END )
				{
					//���֍s������
					m_adventure = TKYADVENTURE_ESCAPE;

					IF( m_NextAdventureWIndow == NULL )
					{
						//�I������
						m_adventure = TKYADVENTURE_ENDPROCESS;
					}
				}

			}

		}

	}






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CTitleView 
//�@�^�C�g�����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*

	LPTkyTitle m_Title;
*/

	CTitleView::CTitleView(TTitle t)
	{
		m_Title = CTitle::CreateTitle(t);
	}
	CTitleView::CTitleView( const CTitleView& t)
	{
		*m_Title = *t.m_Title;
	}
	CTitleView::~CTitleView()
	{
		m_Title->DestroyOwnObject();
	}

//GameView �v�f
	BOOL CTitleView::onCreate()	//������
	{
		return true;
	}
	BOOL CTitleView::onUpdate(float step)	//�X�V
	{
		m_Title->UpdateTitle(step);

		//�����Ȃ�������I���
		IF( m_Title->IsEnd() )
		{
			CEventManager::StaticAddEventMessage( "END_STAGE" , NULL );
		}

		return true;
	}
	VOID CTitleView::onRender()	//�`��
	{
		m_Title->DrawTitle();
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CTitleView::onEvent( TEventMessage msg )
	{
		//���ʃC�x���g
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//���{�^���������ꂽ��e����
			IF( message == WM_LBUTTONDOWN )
			{
				//�����ǂ��x�X�^�[�g
				CEventManager::StaticAddEventMessage( "START_SHADE" , 10 );
			}

			//�}�E�X����������
			IF( message == WM_MOUSEMOVE )
			{
				m_Title->SetPos( Vector( (float)LOWORD(lParam) ,  (float)HIWORD(lParam) ,0 ) ) ;
			}

		}
		EIF( msg.m_message == "END_SHADE_IN")
		{
			IF( msg.m_value[0] == 10 )
			{
				m_Title->Next();
			}
		}

	}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//CScoreView 
//�@�^�C�g�����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	LPTkyScore m_Score;
*/

	CScoreView::CScoreView(TScore t)
	{
		m_Score = CScore::CreateScore(t);
	}
	CScoreView::CScoreView( const CScoreView& s)
	{
		*m_Score = *s.m_Score;
	}
	CScoreView::~CScoreView()
	{
		m_Score->DestroyOwnObject();
	}

//GameView �v�f
	BOOL CScoreView::onCreate()	//������
	{
		return true;
	}
	BOOL CScoreView::onUpdate(float step)	//�X�V
	{
		m_Score->UpdateScore(step);

		//���Z�b�g
		IF( m_Score->GetScore().m_state == TKYSCORE_END )
		{
			CEventManager::StaticAddEventMessage( "END_STAGE" , NULL );
		}
		IF( m_Score->GetScore().m_state == TKYSCORE_RESET )
		{
			CEventManager::StaticAddEventMessage( "RESET_STAGE" , NULL );
		}

		return true;
	}
	VOID CScoreView::onRender()	//�`��
	{
		m_Score->DrawScore();
	}
//�C�x���g�v�f
	//�C�x���g�����炤
	VOID CScoreView::onEvent( TEventMessage msg )
	{
		//�X�R�A��񂪑����Ă���

		//���ʃC�x���g
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//���{�^���������ꂽ��e����
			IF( message == WM_LBUTTONDOWN )
			{
				//���ʏ�Ԃ�������
				IF( m_Score->GetScore().m_state == TKYSCORE_RESULT  )
				{
					//�X�R�A�𑗂�
//					CEventManager::StaticAddEventMessage( "SEND_SCORE" , m_Score->GetScore().m_score );

					//s�q���[�e�B���I��������(�I���ɂ���)
					TScore t = m_Score->GetScore();
					t.m_state = TKYSCORE_END;
					m_Score->SetScore(t);


				}
				EIF( m_Score->GetScore().m_state == TKYSCORE_GAMEOVER   )
				{
					//s�q���[�e�B���I��������(�I���ɂ���)
					TScore t = m_Score->GetScore();
					t.m_state = TKYSCORE_RESET;
					m_Score->SetScore(t);					
				}

			}
		}
		EIF( msg.m_message == "END_SHOOTING_STAGE" )
		{
			CEventManager::StaticAddEventMessage( "END_TEXTVIEW" , 0 );

			//�I�����Ă��Ȃ�������
			IF( m_Score->GetScore().m_state != TKYSCORE_END && 
				m_Score->GetScore().m_state != TKYSCORE_GAMEOVER  && 
				m_Score->GetScore().m_state != TKYSCORE_RESET)
			{

				//s�q���[�e�B���I��������(���ʂɂ���)
				TScore t = m_Score->GetScore();
				t.m_state = TKYSCORE_RESULT;
				m_Score->SetScore(t);
			}			

		}
		EIF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			//�L������������������
			TScore t = m_Score->GetScore();
			IF( t.m_chara == ((LPTkyFPSCharactorView)msg.m_value[0])->GetCharactor() )
			{
				IF( ((LPTkyFPSCharactorView)msg.m_value[0])->GetCharactor()->IsDead()  )
				{

					//���ꂪ�����ŏI����ɃQ�[���I�[�o�[���\�������
					CEventManager::StaticAddEventMessage( "END_TEXTVIEW" , 0 );

					t.m_chara = NULL;
					t.m_state = TKYSCORE_GAMEOVER;
					m_Score->SetScore(t);
				}
			}
		}


		//�X�R�A�Ȃ�
		EIF( msg.m_message ==  "SCORE_BATTERY")
		{
			TScore t = m_Score->GetScore();
			t.m_BatteryNum++;
			m_Score->SetScore(t);
		}
		EIF( msg.m_message ==  "SCORE_HIT")
		{
			TScore t = m_Score->GetScore();
			t.m_HitNum++;
			m_Score->SetScore(t);
		}
		EIF( msg.m_message ==  "SCORE_SCORE")
		{
			TScore t = m_Score->GetScore();
			t.m_score+=msg.m_value[0];
			m_Score->SetScore(t);
		}


	}
