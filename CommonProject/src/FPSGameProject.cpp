#include "FPSGameProject.h"

//��C��R
const STATIC float WorldAir3D = 0.0009f;
//�d��
const STATIC float WorldGravity3D = -0.03f;

VOID	WriteScore( DWORD NowScore)
{
			DWORD score[3];

			//�ǂݍ���
			CTkyFileRead read;
			//�ǂݍ���
			IF(!read.Open(TEXT("text/score.txt")))
			{
				MessageBoxW(NULL,TEXT("�X�R�A���ǂݍ��߂܂���"),TEXT(""),NULL);
				return;
			}
			//�L�������ǂݍ���
			WHILE(!read.IsEof())
			{
				wstring key = read.GetToWhiteSpace();
				swscanf_s( key.c_str() , TEXT("%u"),&score[0] );
				key = read.GetToWhiteSpace();
				swscanf_s( key.c_str() , TEXT("%u"),&score[1] );
				key = read.GetToWhiteSpace();
				swscanf_s( key.c_str() , TEXT("%u"),&score[2] );
			}
			//�J��
			read.Close();

			//���ԓ���ւ�
			IF( score[0] < NowScore )
			{
				score[2] = score[1];
				score[1] = score[0];
				score[0] = NowScore;
			}
			EIF( score[1] < NowScore )
			{
				score[2] = score[1];
				score[1] = NowScore;
			}
			EIF( score[2] < NowScore )
			{
				score[2] = NowScore;
			}


			//�������݊J�n
			FILE* file;
			file = fopen("text/score.txt","w");
			fprintf( file , "%u %u %u",score[0],score[1],score[2] );
			fclose(file);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//���W�b�N�p�p
// ��{�I��FPS���W�b�N
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�R���X�g���N�^�E�f�X�g���N�^
	CFPSGameLogic::CFPSGameLogic()
	{
		//���E�̐ݒ�
		m_world3d = NULL;

		//���E�̐���
		m_world3d =	CPhysicsWorld3D::CreatePhysicsWorld3D();
		m_world3d->SetAirGap( WorldAir3D );
		m_world3d->SetGravity( WorldGravity3D );

		m_AdvectureWinodwQueue = NULL;
		m_TextWinodwQueue = NULL;

		m_NowScore = 0;
		//���
		m_state = TKYFPSLOGIC_READ;
	}
	CFPSGameLogic::CFPSGameLogic(const CFPSGameLogic& f) : IGameLogic(f)
	{
		//���E�̐ݒ�
		*m_world3d = *f.m_world3d;
	}
	CFPSGameLogic::~CFPSGameLogic()
	{
		IF(m_read!=NULL)m_read->DestroyOwnObject();
		IF(m_loading!=NULL)m_loading->DestroyOwnObject();

		IF(m_world3d!=NULL)m_world3d->DestroyOwnObject();
	}
//GameLogic���
	//������
	BOOL CFPSGameLogic::onCreate() 
	{
		SetWindowTextW( NULL , TEXT("Prototype") );

		//���[�h�摜
		m_loading = CTexture::CreateTexture( Init3D::CreateInit3D() , "texture/loading.png" , 800,600 );
		//�ǂݍ��݃^�X�N
		m_read = CTkyReadThread::CreateReadThread( &ResourceManage , &CharaManage , &TextManage  );
		m_read->Run( TKYTHREAD_ONCE_LOOP );

		//�t�H�O���̐ݒ�
		D3DCAPS9 caps;
		ZeroMemory(&caps, sizeof(D3DCAPS9));    //������
		Init3D::CreateInit3D()->GetD3DDevice()->GetDeviceCaps(&caps);
    
		//�s�N�Z���t�H�O���g���邩�ǂ����m�F
		if((caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) == 0)
		{
			MSGBOX( "�t�H�O�ł��܂���","���s" );
		}
		else
		{
			//�t�H�O�̐ݒ�
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0x00, 0x00, 0x00, 0x00)); //���F�ŕs����
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);      //���_���[�h
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);      //���_���[�h
			float s = 1 , e = 10;
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGSTART, *(DWORD*) (&s)); //�J�n�ʒu
			Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGEND, *(DWORD*) (&e));     //�I���ʒu

		}

		return true;
	}
	//�X�V( View�X�V�������� )
	VOID CFPSGameLogic::onUpdate(float step) 
	{
		IF( m_state == TKYFPSLOGIC_READ )
		{
			m_loading->Update(step);
			//�I�����Ă�����
			IF( m_read->GetParam() == TKYTHREAD_NONE_LOOP )
			{
				//�J��
				m_read->DestroyOwnObject();
				m_read = NULL;
				m_loading->DestroyOwnObject();
				m_loading = NULL;

				//�Q�[���t���[���擾
				GameFlowManage.LoadText( TEXT("text/stage.txt") , &ResourceManage , &CharaManage , &TextManage , m_info->m_pos , m_info->m_vec );
				GameFlowManage.Run( this );

				m_state = TKYFPSLOGIC_RUN;
			}
		}
		EIF( m_state == TKYFPSLOGIC_RUN )
		{
			//���E�̍X�V
			m_world3d->Update(step);
		}
	}
	VOID CFPSGameLogic::onRender()
	{
		IF( m_state == TKYFPSLOGIC_READ )
		{
			m_loading->DrawTexture(Vector(400,300,0),Vector(),Vector(1,1,1),TKYTEXTURE_TYPE_2D_MODE);
		}
		EIF( m_state == TKYFPSLOGIC_RUN )
		{
		}
	};

	//�r���[���������Ƃ��ɌĂ΂��
	VOID CFPSGameLogic::onDeleteView( LPTkyGameView view )
	{

		//��Q���̏���
		IF( view->GetName() == "FPS_Map" )
		{
			m_world3d->DeleteObtaclePhysicsModel( ((LPTkyFPSMapView)view)->GetModel() );
		}
		//���f���̏���
		EIF( view->GetName() == "FPS_Charactor_Hero" || 
			view->GetName() == "FPS_Charactor_Enemy" )
		{
			m_world3d->DeletePhysicsModel( ((LPTkyFPSCharactorView)view)->GetCharactor() );
		}
		//�e�̏���
		EIF( view->GetName() == "FPS_Bullet" )
		{
			m_world3d->DeletePhysicsModel( ((LPTkyBulletView)view)->GetBullet() );
		}	
		//�e�L�X�g�̒ǉ�
		EIF( view->GetName() == "FPS_TextWinodwView" )
		{
			IF( m_TextWinodwQueue == view )
			{
				m_TextWinodwQueue = NULL;
			}
		}
		//�A�h�����x���`���[�̒ǉ�
		EIF( view->GetName() == "FPS_AdventureWinodwView" )
		{
			IF( m_AdvectureWinodwQueue == view )
			{
				m_AdvectureWinodwQueue = NULL;
			}
		}

	}
	//View�ݒ莞(View���͎����I�Ƀ^�X�N�ɒǉ������̂ł��̑��̐ݒ�)
	 VOID CFPSGameLogic::onAddView( LPTkyGameView view )
	{

		//��Q���̒ǉ�
		IF( view->GetName() == "FPS_Map" )
		{
			m_world3d->AddObtaclePhysicsModel( ((LPTkyFPSMapView)view)->GetModel() );
		}

		//���f���̒ǉ�
		EIF( view->GetName() == "FPS_Charactor_Hero" || 
			view->GetName() == "FPS_Charactor_Enemy" )
		{
			m_world3d->AddPhysicsModel( ((LPTkyFPSCharactorView)view)->GetCharactor() );

			//AI������
			wstring ai = ((LPTkyFPSCharactorView)view)->GetCharactor()->GetCharaInfo().m_ai;
			wstring move = ((LPTkyFPSCharactorView)view)->GetCharactor()->GetCharaInfo().m_automove;
			LPTkyFPSCharactorView cview = dynamic_cast<LPTkyFPSCharactorView>(view);
			assert( cview != 0 );

			IF(  ai == TEXT("HERO_AI")  )
			{
/*				//�������݂���񂾂�����
				IF( MessageBoxW(NULL,TEXT("�������݂܂����H"),TEXT("�I��") , MB_OKCANCEL) == IDOK )
			{
//�L�^�p
					//����L�^�r���[
				LPTkyMovementPathView m = CMovementPathView::CreateMovementPath( move );
				m->SetBase( cview->GetCharactor()->GetCharaInfo().m_first );
				AddGameView( m );
					//�L�[�Ő���r���[
				LPTkyFpsMoveAIView l =  CFpsMoveAIView::CreateFpsMoveAI( cview , m_info->m_pos , m_info->m_vec ) ;
				l->SetBaseVector(cview->GetCharactor()->GetCharaInfo().m_first);
				AddGameView( l );
			}
				else
*/				{
//���s�p
					//��������r���[
					AddGameView( CFpsAutoAIView::CreateFpsAutoAI( cview , m_info->m_pos , m_info->m_vec , move.c_str() ) );
				}

			}
			EIF( ai == TEXT("NORMAL_AI") )
			{
				AddGameView( CFpsEnemyAIView::CreateFpsEnemyAI( cview ) );
			}
			EIF( ai == TEXT("ATTACK_LEAVE_AI") )
			{
				AddGameView( CFpsEnemyAttackLeaveAIView::CreateFpsEnemyAttackLeaveAI(cview) );
			}
			EIF( ai == TEXT("STOP_ATTACK_AI") )
			{
				AddGameView( CFpsEnemyStopAttackAIView::CreateFpsEnemyStopAttackAI( cview ) );
			}
			EIF( ai == TEXT("THROW_AI") )
			{
				AddGameView( CFpsEnemyThrowAIView::CreateFpsEnemyThrowAI( cview ) );
			}

		}

		//�e�L�X�g�̒ǉ�
		EIF( view->GetName() == "FPS_TextWinodwView" )
		{
			//�k����Ȃ�������
			IF(m_TextWinodwQueue!=NULL)
			{
				LPTkyTextWindowiew im = m_TextWinodwQueue;
				im->SetNextTextWinodw( dynamic_cast<LPTkyTextWindowiew>(view) );
				this->DeleteGameViewOnlyIndex(view);
			}
			else
			{
				//�v�f�̒ǉ�(�k��������)
				m_TextWinodwQueue = dynamic_cast<LPTkyTextWindowiew>(view);
			}
		}

		//�A�h�����񒃂̒ǉ�
		EIF( view->GetName() == "FPS_AdventureWinodwView" )
		{
			//�k����Ȃ�������
			IF(m_AdvectureWinodwQueue!=NULL)
			{
				LPTkyAdventureWindowiew im = m_AdvectureWinodwQueue;
				(im)->SetNextAdventureWinodw( dynamic_cast<LPTkyAdventureWindowiew>(view) );
				//�J���Ȃ��Ńv���W�F�N�g�������
				this->DeleteGameViewOnlyIndex(view);
			}
			else
			{
				//�v�f�̒ǉ�(�k��������)
				m_AdvectureWinodwQueue = dynamic_cast<LPTkyAdventureWindowiew>(view);
			}
		}

		//�e�̒ǉ�
		EIF( view->GetName() == "FPS_Bullet" )
		{

			//�e�ǉ����Ɏ����Ƒ��x��ݒ肷��
			//�}�E�X���\���̂��獡�̃X�N���[�����W���擾���A���[���h���W���擾����B
			//�������s����ɂ́A���N���X�̕ԊҊ֐��𗘗p����B

			//����
			Vector vec = Vector();

			IF(  GetModelMessageP( ((LPTkyBulletView)view)->GetWeapon()->GetChara() ) == "HERO_CHARACTOR" )
			{
				//��l���̏ꍇ�����擾
//				m_info->m_envi->GetCamera(NULL,&vec,NULL,NULL,NULL,NULL);
				//���[���h�ϊ������x�N�g��
				vec = m_info->m_envi->GetWorldPosition( m_info->m_absmouse );
			}
			else
			{
				//����ȊO�͑��x�������ƂƂ�
				vec = ((LPTkyBulletView)view)->GetBullet()->GetVel();
			}

			//�ʒu�擾
			Vector pos = ((LPTkyBulletView)view)->GetBullet()->GetPos();
			vec = vec.Normal();

			//�擾�������[���h���W���A�����G���W���N���X����A��Q���܂ł̋����֐��ɂ����
			//�����ƂȂ�A�������擾����B

			float distance = m_world3d->GetDistanceObstacle( pos , vec );

			//������ݒ肷��

			TBulletInfo bullet = ((LPTkyBulletView)view)->GetBullet()->GetBulletInfo();

			//����ɂ���Ă����̐ݒ�͕ς��
			//�e�̏ꍇ�͋����̒Z�������̗p�ƂȂ�
			IF(distance > 0)bullet.m_Life = min(distance , bullet.m_Life);

			((LPTkyBulletView)view)->GetBullet()->SetBulletInfo( bullet );

			//���x�̓��[���h���W�ɍU�����x���|�����킹�ċ��߂邱�Ƃ��o����B

			((LPTkyBulletView)view)->GetBullet()->SetVel( vec*bullet.m_AttackSpeed );

			//�r���[�̒ǉ�

			m_world3d->AddPhysicsModel( ((LPTkyBulletView)view)->GetBullet() );
		}

	 
	 }
//�C�x���g���
	//�C�x���g�����炤
	VOID CFPSGameLogic::onEvent( TEventMessage msg )
	{
		IGameLogic::onEvent(msg);
		//�r���[�̒ǉ��A�r���[�̏����̃��b�Z�[�W�����炤

		//�G�t�F�N�g�r���[�̒ǉ�
		IF( msg.m_message == "FPSLOGIC_ADDVIEW_EFFECT"  )
		{
			this->AddGameView( CEffectView::CreateEffect( 
				(CGameEffect*)(msg.m_value[0]),
				*(Vector*)(msg.m_value[1]),
				*(Vector*)(msg.m_value[2]),
				*(Vector*)(msg.m_value[3]),
				(TKYTEXTURE_TYPE)(msg.m_value[4])
				) );
		}

		EIF( msg.m_message == "FPSLOGIC_ADDVIEW_BULLET"  )
		{
			LPTkyBulletView b = CBulletView::CreateBullet( (LPTkyWeapon)(msg.m_value[0]) );
			//���W
			b->GetBullet()->SetPos( *(Vector*)(msg.m_value[1]) );
			//���x
			b->GetBullet()->SetVel( *(Vector*)(msg.m_value[2])  );
			this->AddGameView(b);
		}

		EIF( msg.m_message == "SEND_SCORE" )
		{
			m_NowScore += msg.m_value[0];
		}

		EIF( msg.m_message == "END_STAGE" )
		{
			//�ؑ]���擾
			GameFlowManage.GetBaseData();

			//�Ƃ߂�
			GameFlowManage.Stop();

			//���͂̏I��
			IF( m_TextWinodwQueue!=NULL )
			{
				//�A�����ׂď���
				m_TextWinodwQueue->DeleteNext();
				DELETE_VIEW_MESSAGE( m_TextWinodwQueue );
				m_TextWinodwQueue = NULL;

			}

			//���ɐi�߂�
			IF(!GameFlowManage.Next())
			{
				//�X�R�A�̏�������
				WriteScore(m_NowScore);
				//�X�R�A
				m_NowScore = 0;

				map< LPTkyGameView , LPTKYMULTITASK > m = m_TaskMap;
				map< LPTkyGameView , LPTKYMULTITASK >::iterator im = m.begin();
				WHILE( im != m.end() )
				{
					DELETE_VIEW_MESSAGE( im->second );
					im++;
				}

				//�ŏ�������
				GameFlowManage.LoadText( TEXT("text/stage.txt") , &ResourceManage , &CharaManage , &TextManage , m_info->m_pos , m_info->m_vec );
			}
			//�����ǂ��x�X�^�[�g
			CEventManager::StaticAddEventMessage( "START_SHADE" , 100 );

		}

		EIF( msg.m_message == "END_SHADE_IN" )
		{
			//�V�F�C�h���I�������
			IF( msg.m_value[0] == 100 )
			{
				//�J�n����
				GameFlowManage.Run(this);
			}
		}

		EIF( msg.m_message == "END_TEXTVIEW" )
		{
			//���͂̏I��
			IF( m_TextWinodwQueue!=NULL )
			{
				//�A�����ׂď���
				m_TextWinodwQueue->DeleteNext();
				DELETE_VIEW_MESSAGE( m_TextWinodwQueue );
				m_TextWinodwQueue = NULL;
			}
		}

		EIF( msg.m_message == "RESET_STAGE" )
		{
			//���ׂĂƂ߂�
			do
			{
				//�Ƃ߂�
				GameFlowManage.Stop();
			}
			while( GameFlowManage.Next() );

			m_NowScore = 0;

			//���͂̏I��
			IF( m_TextWinodwQueue!=NULL )
			{
				//�A�����ׂď���
				m_TextWinodwQueue->DeleteNext();
				DELETE_VIEW_MESSAGE( m_TextWinodwQueue );
				m_TextWinodwQueue = NULL;
			}

			map< LPTkyGameView , LPTKYMULTITASK > m = m_TaskMap;
			map< LPTkyGameView , LPTKYMULTITASK >::iterator im = m.begin();
			WHILE( im != m.end() )
			{
				DELETE_VIEW_MESSAGE( im->second );
				im++;
			}

			//�ŏ�������
			GameFlowManage.LoadText( TEXT("text/stage.txt") , &ResourceManage , &CharaManage , &TextManage , m_info->m_pos , m_info->m_vec );
			
			//�J�n����
			GameFlowManage.Run(this);
		}

		//���ʃC�x���g
		IF( msg.m_message == "WINDOWS_EVENT" )
		{
			UINT message = msg.m_value[0];
			WPARAM wParam = msg.m_value[1]; 
			LPARAM lParam = msg.m_value[2];

			//�E�{�^���Ŏ���
			IF( message == WM_RBUTTONDOWN )
			{
//				CEventManager::StaticAddEventMessage( "END_STAGE" , 0 );
			}
			EIF( message == WM_CHAR )
			{
//				IF( wParam == 'A' )	CEventManager::StaticAddEventMessage( "END_STAGE" , 0 );
			}
		}

	}
//����
	LPTkyFPSGameLogic CFPSGameLogic::CreateFPSGameLogic()
	{
		return new CFPSGameLogic();
	}
	LPTkyFPSGameLogic CFPSGameLogic::CreateFPSGameLogic(TDirectXInfo* info)
	{
		LPTkyFPSGameLogic f = CFPSGameLogic::CreateFPSGameLogic();
		f->SetDirectXInfo( info );
		return f;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�A�v���P�[�V�����p�p
// ��{�I�ȃA�v���P�[�V����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//�R���X�g���N�^�E�f�X�g���N�^
	CFPSApplicationLayer::CFPSApplicationLayer()
	{
		m_Logic = CFPSGameLogic::CreateFPSGameLogic( GetDirectXInfoPointa() );

	}
	CFPSApplicationLayer::CFPSApplicationLayer(const CFPSApplicationLayer& f) :IDirectXBaseApplicationLayer(f)
	{
		*this = f;
	}
	CFPSApplicationLayer::~CFPSApplicationLayer()
	{
	}

	//DirectX�p���z�֐�
	BOOL CFPSApplicationLayer::pOnCreateDirectX()
	{
		//�����̑傫��
//		GetDirectXInfo().m_pFont->SetFontInfo(15,30,3, "���C���I" );		

		//�Ђ�
		m_shade.Read(TEXT("texture/shade.png") );

		//���W�b�N�̏�����
		m_Logic->Create();

		return true;

	}
	BOOL CFPSApplicationLayer::pOnUpdateDirectX(float step)
	{
		m_shade.UpdateShade(step);
		IF( m_shade.IsUpdate() )
		{

			//���W�b�N�̍X�V
			m_Logic->Update(step);	

		}

		return true;

	}
	VOID CFPSApplicationLayer::pOnRenderDirectX()
	{

		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE); //�t�H�O�FON

		m_Logic->Render();

		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE); //�t�H�O�FON

		//�e�N�X�`���̕`��i�A���t�@�̂��߁j
		CTexture::DrawWaitTexture();
		//�����̕`��
		CFONT::DrawWaitFont();

		m_shade.DrawShade();

	}
	VOID CFPSApplicationLayer::pOnEventDirectX(TEventMessage msg )
	{
		//�I���C�x���g�����炤���Ƃɂ���Ď��̃f�[�^�̓ǂݍ��݂��n�܂�
		IF( msg.m_message == "START_SHADE" )
		{
			m_shade.Run(msg.m_value[0]);
		}

	}