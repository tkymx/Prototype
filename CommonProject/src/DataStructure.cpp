
#include "DataStructure.h"


//============================================================
//�Q�[���̃f�[�^�\��
//============================================================

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�G�t�F�N�g���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�G�t�F�N�g�̍쐬
STATIC VOID CreateEffectMessage(CGameEffect cc , Vector pos )
{
		TEventMessage message;
		message.m_message = "FPSLOGIC_ADDVIEW_EFFECT";
		message.m_value.resize(5);
		message.m_value[0] = (DWORD)&cc;
		message.m_value[1] = (DWORD)(&pos);
		message.m_value[2] = (DWORD)(&Vector());
		message.m_value[3] = (DWORD)(&Vector());
		message.m_value[4] = (DWORD)(TKYTEXTURE_TYPE_2D_MODE);
		CEventManager::StaticAddEventMessage( message );
}


//////////////////////////////////////�G�t�F�N�g�s���N���X//////////////////////////////////////
	CGameEffect::CGameEffect()
	{
		m_effect.Initialize();
	}
	CGameEffect::CGameEffect( const CGameEffect& c)
	{
		*this = c;
	}
	CGameEffect::~CGameEffect()
	{
	}
	BOOL CGameEffect::CreateEffect(TGameEffect t)
	{
		IF( t.m_texture != NULL )
		{
			m_effect = t;
			m_state = TKYSTATE_RUN;
			m_effect.m_NowTexFrame = 0;
			m_effect.m_NowTexTime = 0;
		}
		return false;
	}
	BOOL CGameEffect::UpdateEffect(float step)
	{
		IF(m_state == TKYSTATE_RUN)
		{
			IF(m_effect.m_texture ==NULL)return false;
			IF(m_effect.m_texture->UpdateIndex( 1.0f , &m_effect.m_NowTexTime , &m_effect.m_NowTexFrame ))
			{
				//���[�v���I��������Ƃ�����
				m_oneLoop = true;
			}
			else
			{
				//���[�v���I����Ă��Ȃ�
				m_oneLoop = false;
			}
		}
		return true;
	}
	BOOL CGameEffect::DrawEffect(Vector pos,Vector rotate , Vector expand)
	{
		IF(m_effect.m_texture ==NULL)return false;
		return m_effect.m_texture->DrawTextureIndex( pos , rotate , expand , TKYTEXTURE_TYPE_3D_MODE , m_effect.m_NowTexFrame );
	}
	BOOL CGameEffect::DrawEffect2D(Vector pos)
	{
		IF(m_effect.m_texture ==NULL)return false;
		return m_effect.m_texture->DrawTextureIndex( pos , Vector() , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE ,m_effect.m_NowTexFrame  );
	}

//////////////////////////////////////�G�t�F�N�g�Ǘ��N���X//////////////////////////////////////
	//�R���X�g���N�^�A�f�X�g���N�^
	CManageEffect::CManageEffect()
	{
		m_EffectMatch.Destroy();
	}
	CManageEffect::CManageEffect(const CManageEffect& m)
	{
		this->m_EffectMatch = m.GetEffectMatch();
	}
	CManageEffect::~CManageEffect()
	{
		m_EffectMatch.Destroy();
	}
	//�ǉ�
	BOOL CManageEffect::AddEffectData( wstring name,LPTkyTexture tex, UINT life )
	{
//		IF( tex == NULL )return false;
		TGameEffect gm(tex,life);
		CGameEffect ce;
		ce.CreateEffect(gm);
		m_EffectMatch.AddString( name , ce );
		return true;
	}
	BOOL CManageEffect::AddEffectData( wstring name, TGameEffect* t )
	{
		IF( t == NULL )return false;
		CGameEffect ce;
		ce.CreateEffect(*t);
		m_EffectMatch.AddString( name , ce );
		return true;
	}
	//����
	BOOL CManageEffect::DeleteEffectData( wstring name )
	{
		return m_EffectMatch.Destroy( name );
	}
	//�X�V
	BOOL CManageEffect::UpdateEffectData( wstring name, float step )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			IF(ce!=NULL)ce->UpdateEffect( step );
			return true;
		}
		return false;
	}
	//�Đ�
	BOOL CManageEffect::DrawEffectData( wstring name, Vector vec , Vector rotate , Vector expand )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			IF(ce!=NULL)ce->DrawEffect( vec , rotate , expand );
			return true;
		}
		return false;
	}
	BOOL CManageEffect::DrawEffectData2D( wstring name, Vector vec )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			IF(ce!=NULL)ce->DrawEffect2D( vec );
			return true;
		}
		return false;
	}
	//�ݒ�
	BOOL CManageEffect::RunEffectData( wstring name )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			IF(ce!=NULL)ce->RUN();
			return true;
		}
		return false;
	}
	BOOL CManageEffect::StopEffectData( wstring name )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			IF(ce!=NULL)ce->STOP();
			return true;
		}
		return false;
	}

	//�擾
	LPTkyGameEffect CManageEffect::GetEffect(wstring name )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			return ce;
		}
		return NULL;
	}

	//���[�v���I�����Ă��邩�ǂ���
	BOOL CManageEffect::IsFinish( wstring name )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			return ce->IsOneLoopEnd();
		}
		return false;
	}
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�L�����N�^���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//////////////////////////////////////�A�j���[�V�����s���N���X//////////////////////////////////////

	//�R���X�g���N�^�A�f�X�g���N�^�A
	CGameAnimation::CGameAnimation()
	{
		m_Animation.Initialize();
		m_oneLoop = false;
	}
	CGameAnimation::CGameAnimation( const CGameAnimation& x)
	{
		m_Animation = x.GetAnimationInfo();
		m_oneLoop = false;
	}
	CGameAnimation::~CGameAnimation()
	{
	}
	BOOL CGameAnimation::CreateAnimation(TGameAnimation t)
	{
		//���f�����Ȃ��Ă��ʂ�
		m_Animation = t;
		return true;
	}
	BOOL CGameAnimation::UpdateAnimation(float step)
	{
		//�t���[����i�߂�
		IF(m_Animation.m_model!=NULL)
		{
			m_Animation.m_NowFrame  =  m_Animation.m_model->GetAnime()->UpdateFrame( m_Animation.m_NowFrame , step );
			//0��������
			IF( ISEPSILON( m_Animation.m_NowFrame , 0 ) )
			{
				m_oneLoop = true;
				return true;
			}
			else
			{
				m_oneLoop = false;
				return true;
			}
		}

		//���f�����Ȃ�������
		m_oneLoop = true;
		return true;

	}
	BOOL CGameAnimation::DrawAnimation(Vector pos,Vector rotate , Vector expand)
	{
		IF(m_Animation.m_model!=NULL)
		{
			m_Animation.m_model->SetAnimeFreem( m_Animation.m_NowFrame );
			m_Animation.m_model->DrawMesh( pos,rotate );
		}
		return true;
	}

//////////////////////////////////////�A�j���[�V�����Ǘ��N���X//////////////////////////////////////
	
	//�R���X�g���N�^�A�f�X�g���N�^
	CManageAnimation::CManageAnimation()
	{
		m_XAnimeMatch.Destroy();
	}
	CManageAnimation::CManageAnimation(const CManageAnimation&  a)
	{
		m_XAnimeMatch = a.GetAnimeMatch();
	}
	CManageAnimation::~CManageAnimation()
	{
		m_XAnimeMatch.Destroy();
	}
	//������
	BOOL CManageAnimation::InitializeAnimationData( wstring name )
	{
		CGameAnimation *ga = NULL;
		IF( m_XAnimeMatch.GetElement( name , &ga ) )
		{
			IF(ga!=NULL)ga->InitializeFrame();
			return true;
		}
		return false;
	}
	//�ǉ�
	BOOL CManageAnimation::AddAnimationData( wstring name,LPTkyXFile model)
	{
		//���f�����Ȃ��Ă��������Ƃɂ���
//		IF( model == NULL )return false;
		TGameAnimation am(model , NULL );
		CGameAnimation ga;
		ga.SetAnimationInfo(am);
		m_XAnimeMatch.AddString( name , ga );
		return true;
	}
	//����
	BOOL CManageAnimation::DeleteAnimationData( wstring name)
	{
		return m_XAnimeMatch.Destroy( name );
	}
	//�X�V
	BOOL CManageAnimation::UpdateAnimationData( wstring name ,float step)
	{
		CGameAnimation *ga = NULL;
		IF( m_XAnimeMatch.GetElement( name , &ga ) )
		{
			IF(ga!=NULL)
			{
				//���[�v���I�����������
				ga->UpdateAnimation(step);
			}
			return true;
		}
		return false;
	}
	//�Đ�
	BOOL CManageAnimation::DrawAnimationData( wstring name, Vector vec , Vector rotate , Vector expand )
	{
		CGameAnimation *ga = NULL;
		IF( m_XAnimeMatch.GetElement( name , &ga ) )
		{
			IF(ga!=NULL)ga->DrawAnimation(vec,rotate,expand);
			return true;
		}
		return false;
	}
	//���[�v���I�����Ă��邩�ǂ���
	BOOL CManageAnimation::IsFinish( wstring name )
	{
		CGameAnimation *ga = NULL;
		IF( m_XAnimeMatch.GetElement( name , &ga ) )
		{
			IF(ga!=NULL)
			{
				return ga->IsOneLoopEnd();
			}
		}
		return false;
	}

////////////////////////////////////�U���N���X//////////////////////////////////////

	//�R���X�g���N�^�A�f�X�g���N�^
	CBullet::CBullet()
	{
		//������
		m_Bullet.Initialize();
		m_OriginPos = Vector();
		Initialize();
		m_end = false;
		//���[�h�̐ݒ�
		SetMode(TEXT("MODEL_INVINCIBLE"));
		//�Փˎ����b�Z�[�W
		SetCollisionMessage( TEventMessage::Initializel( "BULLET_MODEL" , (DWORD)this ) );
		//���
		m_BatteryChara = NULL;
	}
	CBullet::CBullet(const CBullet& c) : PhysicsCircleModel(c)
	{
		m_Bullet = c.GetBulletInfo();
		m_OriginPos = c.m_OriginPos;
	}
	CBullet::~CBullet()
	{
	}
	//�ڐG���(�ԋp�n�A�Փˌ㗣��邩�ǂ���)
	bool CBullet::onCollisionEvent(PhysicsModel *model)
	{
		//���ʂ̍U���Ȃ炠�����瓖����������L�^����
		//�U�����I���܂ł����ɓ�����Ȃ��悤�ɂ���

		//�e�Ȃ�A��������I���

		//�L�������
		LPTkyCharactor BatteryChara = this->GetChara();
		IF( BatteryChara == NULL )return false;

		//�ł����L�����̖��O
		string bullet = BatteryChara->GetCollisionMessage().m_message;

		//�L�����N�^�̔���
		IF( model->GetCollisionMessage().m_message == "ENEMY_CHARACTOR" && bullet == "HERO_CHARACTOR" )
		{
			//�G�ɂ�������I��
			m_end = true;
			return false;
		}
		EIF( model->GetCollisionMessage().m_message == "HERO_CHARACTOR" && bullet == "ENEMY_CHARACTOR" )
		{
			m_end = true;
			return false;
		}
		
		return false;
	}
	//�I�����Ă��邩�擾
	bool CBullet::IsFinish()
	{
		//�o���ʒu����̑��΋���
		float l = ( GetPos() - m_BatteryPosition ).GetLarge();

		//�����\�苗���܂ōs������I��
		IF( l > GetBulletInfo().m_Life )
		{
			m_end = true;
		}

		return m_end;
	}


//////////////////////////////////////����//////////////////////////////////////

	//�r���[�Ƃ��ď풓���A�U�����x���g�ɂ��U������
	//�R���X�g���N�^�A�f�X�g���N�^
	CWeapon::CWeapon()
	{
		//�U�����
		m_Bullet.Initialize();
		//������
		m_Weapon.Initialize();

		//���̏��
		m_NowState = "WEAPON_NONE";
	}
	CWeapon::CWeapon(const CWeapon& w) : IGameLoadText(w)
	{
		//�U�����
		m_Bullet = w.GetBulletInfo();
		//������
		m_Weapon = w.GetWeaponInfo();

		//���̏��
		m_NowState = w.GetNowState();

	}
	CWeapon::~CWeapon()
	{
		//�U�����
		m_Bullet.Initialize();
		//������
		m_Weapon.Initialize();
	}
	//�U�����
	VOID CWeapon::pAttack()
	{
		//�e�C�x���g�̍쐬
		TEventMessage bullet;
		bullet.m_message = "FPSLOGIC_ADDVIEW_BULLET";
		bullet.m_value.resize(3);
		bullet.m_value[0] = (DWORD)this;
		bullet.m_value[1] = (DWORD)(&m_chara->GetPos());
		bullet.m_value[2] = (DWORD)(&(m_chara->GetCharaInfo().m_AttackDirection));
		CEventManager::StaticAddEventMessage( bullet );

		//�G�t�F�N�g�̐���
		CreateEffectMessage( *m_EffectData.GetEffect(TEXT("WEAPON_EFFECT_FLASH")) , Init3D::CreateInit3D()->GetScreenSize()/2 );

		CreateEffectMessage( *m_EffectData.GetEffect(TEXT("WEAPON_EFFECT_SHOT")) , m_pos );

		//��l����������U�����b�Z�[�W
		IF( m_chara->GetCollisionMessage().m_message == "HERO_CHARACTOR" )
		{
			//���˃C�x���g
			CEventManager::StaticAddEventMessage( "SCORE_BATTERY" , NULL);
		}

	}
	//�U��
	BOOL CWeapon::Attack(Vector p)
	{
		//�������˂��ĂȂ������甭��
		IF(	m_NowState == "WEAPON_NONE")
		{
			//��������ł��Ȃ�������
			IF( m_chara->GetCharaInfo().m_HitPoint > 0  )
			{

				//�U�����n�߂�
				m_NowState = "WEAPON_PRELAUNCH";
				m_pos = p;		//���ݒn�̐ݒ�
				IF( ISEPSILON( m_Weapon.m_FiringBulletsLate , 0 ) )
			{
				pAttack();	//�U������
			}
				m_time = 0;


				m_chara->SetState( "CHARACTOR_ATTACK" );
			}
			return true;
		}
		return false;
	}
	//�U�����Ȃ�e���ǂ����Ő�������
	BOOL CWeapon::UpdateAttack(float step)
	{
		//����HP��0��������I��
		IF(m_chara->GetCharaInfo().m_HitPoint <= 0)
		{
			m_NowState = "WEAPON_NONE";
			m_time = 0;
			IF( m_chara->GetState() != "CHARACTOR_DEAD" && m_chara->GetState() != "CHARACTOR_NULL" )
			{
				m_chara->SetState( "CHARACTOR_DEAD" );
			}
		}

		//�U��������Ȃ�������
		IF( m_chara->GetState() != "CHARACTOR_ATTACK" )
		{
				m_NowState = "WEAPON_NONE";
				m_time = 0;
				m_AnimationData.InitializeAnimationData( TEXT("WEAPON_ANIMATION_ATTACK") );
		}

		IF(	m_NowState == "WEAPON_NONE")
		{
			m_time = 0;
		}
		EIF(	m_NowState == "WEAPON_PRELAUNCH")
		{
			//�i�߂�
			m_time += step;
			//���ꎞ�Ԉȏ�Ȃ����甭��
			IF( m_time > (float)m_Weapon.m_FiringBulletsLate )
			{
				m_NowState = "WEAPON_SHOT";
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
//		�����쐬�����̂ł����ł͍U���C�x���g�̍쐬
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
				IF( !ISEPSILON( m_Weapon.m_FiringBulletsLate , 0) )
				{
					pAttack();
				}
			}
		}
		EIF(	m_NowState == "WEAPON_SHOT")
		{

			//�A�j���[�V�������I�������
			IF( m_AnimationData.IsFinish( TEXT("WEAPON_ANIMATION_ATTACK") ) )
			{
				m_NowState = "WEAPON_NONE";
				m_time = 0;
				m_chara->SetState( "CHARACTOR_WALK" );
			}

		}

		//�A�j���[�V�����̍X�V
		IF(m_NowState == "WEAPON_PRELAUNCH" || m_NowState == "WEAPON_SHOT")
		{
			m_AnimationData.UpdateAnimationData( TEXT("WEAPON_ANIMATION_ATTACK") , step );
		}

		return true;
	}
	VOID CWeapon::DrawAttack()
	{

		IF(m_NowState == "WEAPON_PRELAUNCH" || m_NowState == "WEAPON_SHOT")
		{

			m_AnimationData.DrawAnimationData( TEXT("WEAPON_ANIMATION_ATTACK") , m_chara->GetPos() , m_chara->GetCharaInfo().m_rotate , Vector(1,1,1) );

		}

	}
	//�U�������I������C�x���g
	BOOL CWeapon::EndAttack()
	{
		//���˒���������I���ɂ���
		IF(	m_NowState == "WEAPON_NONE")
		{
			return true;
		}
		return false;
	}
	//�ǂݍ���
	BOOL CWeapon::onLoadGameDate(TGameTextInfo info)
	{
		TGameTextInfo::iterator it = info.begin();
		WHILE( it != info.end() )
		{
			//����A�j���[�V�����t�@�C���̒ǉ�
			IF( it->m_key == TEXT("WEAPON_SET_ANIMATION") )
			{
				m_AnimationData.AddAnimationData( it->m_text , (LPTkyXFile)it->m_element );
			}
			//����G�t�F�N�g�t�@�C���̒ǉ�
			EIF( it->m_key == TEXT("WEAPON_SET_EFFECT") )
			{
				m_EffectData.AddEffectData( it->m_text , (LPTkyTexture)it->m_element , 1 );
			}
			//�����{���
			IF( it->m_key == TEXT("WEAPON_SET_BASEINFO") )
			{
				m_Weapon = *((TWeaponInfo*)it->m_element);
			}
			//�e��{���
			EIF( it->m_key == TEXT("ATTACK_SET_BASEINFO") )
			{
				m_Bullet = *((TBulletInfo*)it->m_element);
			}

			it++;
		}
		return true;
	}

//////////////////////////////////////���C���L�����N�^�[�N���X//////////////////////////////////////
//�Ǝ��v�f
	//�R���X�g���N�^�A�f�X�g���N�^
	CCharactor::CCharactor()
	{
		m_chara.Initialize();
		m_state = "CHARACTOR_WALK";
		SetMode(TEXT("SET_NORMAL"));
		SetCollisionMessage( TEventMessage::Initializel( "CHARACTOR" , (DWORD)this ) );
	}
	CCharactor::CCharactor(const CCharactor& c) : PhysicsCircleModel(c)
	{
		*this = c;
	}
	CCharactor::~CCharactor()
	{
//		m_AnimationData.~CManageAnimation();
//		m_EffectData.~CManageEffect();
		m_state.clear();
	}
//PhysicsCircleModel�v�f
	//�ڐG���(�ԋp�n�A�Փˌ㗣��邩�ǂ���)  (2��U�������̂Œ���)
	bool CCharactor::onCollisionEvent(PhysicsModel *model)
	{
		string mstr = GetModelMessageP( model );
		string pstr = GetModelMessageP( this );


		IF( mstr == "BULLET_MODEL" )
		{
		
			LPTkyCharactor BatteryChara = ((LPTkyBullet)model->GetCollisionMessage().m_value[0])->GetChara();
			IF( BatteryChara == NULL )return false;

			string bullet = BatteryChara->GetCollisionMessage().m_message;

			IF( pstr == "ENEMY_CHARACTOR" &&  bullet == "HERO_CHARACTOR" )
			{
				//�e�����������C�x���g
				CEventManager::StaticAddEventMessage( "SCORE_HIT" , NULL);

				//����ł����炠����Ȃ�
				IF( m_chara.m_HitPoint > 0  )
				{
					//�e���������Ă�����ꏊ�ɂ��_��
					//���ς���肻��ɂ���ē_�������߂�
					float score_rate = ( (m_pos-model->GetPos()).Normal() ^ model->GetVel().Normal() )  * 100.0f;
					CEventManager::StaticAddEventMessage( "SCORE_SCORE" , (DWORD)score_rate );


					//�e�ɂ������Ď������G��������
					//�U��������Ȃ�������
					IF( m_state != "CHARACTOR_ATTACK" )
					{
						m_AnimationData.InitializeAnimationData( TEXT("TKYMODEL_HIT") );
						m_state = "CHARACTOR_HIT";
					}

					//�̗͌���
					Damage(( (LPTkyBullet)model->GetCollisionMessage().m_value[0] )->GetBulletInfo().m_AttackPower );
					IF( m_state != "CHARACTOR_DEAD" && m_chara.m_HitPoint <= 0 )
					{
						//�U������Ď��񂾂�
						m_AnimationData.InitializeAnimationData( TEXT("TKYMODEL_DEAD") );
						m_state = "CHARACTOR_DEAD";
					}

				}
				else
				{
					return false;
				}
				return false;
			}
			IF( pstr == "HERO_CHARACTOR" &&  bullet == "ENEMY_CHARACTOR"  )
			{

				Damage(( (LPTkyBullet)model->GetCollisionMessage().m_value[0] )->GetBulletInfo().m_AttackPower);
				IF( m_chara.m_HitPoint <= 0  )
				{

					//�����O��������Q�[���I�[�o�[��
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
//					�Q�[���I�[�o�[�G�t�F�N�g�̒ǉ�
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

				}

				return false;
			}
		}


		return false;
	}

	//�_���[�W
	VOID CCharactor::Damage( float damage)
	{
		m_chara.m_HitPoint -= (int)damage;

		//�_���[�W�G�t�F�N�g
		CreateEffectMessage( *m_EffectData.GetEffect( TEXT("TKYEFFECT_DAMAGE") ) , Vector( 400,300,0 ) );

	}

	
//�Ǝ��v�f
	//�X�V
	BOOL CCharactor::UpdateCharactor( float step )
	{
		IF( m_state == "CHARACTOR_STAND" )
		{
			IF(!m_AnimationData.UpdateAnimationData( TEXT("TKYMODEL_STAND") , step ))
			{
				return false;
			}
		}
		EIF( m_state == "CHARACTOR_WALK" )
		{
			IF(!m_AnimationData.UpdateAnimationData( TEXT("TKYMODEL_WALK") , step ))
			{
				return false;
			}
		}
		EIF( m_state == "CHARACTOR_HIT" )
		{
			IF(!m_AnimationData.UpdateAnimationData( TEXT("TKYMODEL_HIT") , step ))
			{
				return false;
			}
			//���[�v���I����Ă����������
			IF( m_AnimationData.IsFinish(TEXT("TKYMODEL_HIT"))  ) m_state = "CHARACTOR_WALK";
		}
		EIF( m_state == "CHARACTOR_DEAD" )
		{
			IF(!m_AnimationData.UpdateAnimationData( TEXT("TKYMODEL_DEAD") , step ))
			{
				return false;
			}
			//���[�v���I����Ă����玟�̃��[�v
			IF( m_AnimationData.IsFinish(TEXT("TKYMODEL_DEAD"))  )
			{
				m_state = "CHARACTOR_NULL";
			}
		}
		EIF( m_state == "CHARACTOR_NULL" )
		{
			//�������Ȃ�
		}
		EIF( m_state == "CHARACTOR_ATTACK" )
		{
			//�U����
		}
		else
		{
			MessageBoxW( NULL , TEXT("�L�����X�V���sNONE") , TEXT("err") , NULL );
		}

		return true;
	}
	//�`��(�ʒu����PhysicsCircleModel����Ƃ�)
	VOID CCharactor::DrawCharactor()
	{

		IF( m_state == "CHARACTOR_STAND" )
		{
			IF(!m_AnimationData.DrawAnimationData( TEXT("TKYMODEL_STAND") , GetPos() , m_chara.m_rotate , Vector() ))
			{
			}
		}
		EIF( m_state == "CHARACTOR_WALK" )
		{
			IF(!m_AnimationData.DrawAnimationData( TEXT("TKYMODEL_WALK") , GetPos() , m_chara.m_rotate , Vector() ))
			{
			}
		}
		EIF( m_state == "CHARACTOR_HIT" )
		{
			IF(!m_AnimationData.DrawAnimationData( TEXT("TKYMODEL_HIT") , GetPos() , m_chara.m_rotate , Vector() ))
			{
			}
		}
		EIF( m_state == "CHARACTOR_DEAD" )
		{
			IF(!m_AnimationData.DrawAnimationData( TEXT("TKYMODEL_DEAD") , GetPos() , m_chara.m_rotate , Vector() ))
			{
			}
		}
	}
	//�C�x���g(�����Ă����C�x���g�ɂ��G�t�F�N�g�C�x���g�ł��쐬����)
	VOID CCharactor::EventCharactor( string name, DWORD message )
	{
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
//			�����Ă������b�Z�ɂ�������
//			�l�X�ȃC�x���g���쐬����B
//			�@	�G�b�t�F�N�g�r���[��AI�r���[�Ȃ�
//				�ł���΃��b�Z�[�W�ɂ̓��W�b�N�̃|�C���^���ق����Ƃ���(-_-)
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
	}

	//�Q�[���f�[�^�̓ǂݍ���
	BOOL CCharactor::onLoadGameDate(TGameTextInfo info)
	{
		TGameTextInfo::iterator it = info.begin();
		WHILE( it != info.end() )
		{
			//X�t�@�C���̒ǉ�
			IF( it->m_key == TEXT("CHARACTOR_SET_ANIMATION") )
			{
				m_AnimationData.AddAnimationData( it->m_text , (LPTkyXFile)it->m_element );
			}
			//�G�t�F�N�g�̒ǉ�
			EIF( it->m_key == TEXT("CHARACTOR_SET_EFFECT") )
			{
				m_EffectData.AddEffectData( it->m_text , &TGameEffect( (LPTkyTexture)it->m_element , 1 ) );
			}
			//��{���
			EIF( it->m_key == TEXT("CHARACTOR_SET_BASEINFO") )
			{
				SetCharaInfo( *((TCharaInfo*)it->m_element) );
			}

			it++;
		}
		return true;
	}



//////////////////////////////////////�����\���N���X//////////////////////////////////////
// �����̕\�����󂯎��A
/*
	//�e�L�X�g���
	TTextInfo m_text;
	//���̕\������Ă��镶����(��ŕ�����)
	vector<wstring> m_ShowString;
	//���̕\��������
	FLOAT	m_NowCharacterNum;
*/
	//�R���X�g���N�^�A�f�X�g���N�^
	CShowText::CShowText()
	{
		m_text = TTextInfo();
		m_ShowString.clear();
		m_NowCharacterNum = NULL;
	}
	CShowText::CShowText(const CShowText& t)
	{
		*this = t;
	}
	CShowText::~CShowText()
	{
		m_ShowString.clear();
	}

	//�����̏�񏉊���
	VOID	CShowText::InitializeText()
	{
		m_ShowString.clear();
		m_NowCharacterNum = NULL;		
	}
	//�X�V(�I�����Ă�����^��Ԃ�)
	bool		CShowText::UpdateText(float step)
	{
		//�I������
		IF( m_text.m_string.size() < m_NowCharacterNum )return true;

		//������i�߂�
		m_NowCharacterNum += m_text.m_ShowSpeed;

		//��s,��̕������Ԃ�
//		DWORD m_RowString = (DWORD)(  ( (float)abs(m_text.m_Render.right - m_text.m_Render.left)  /  CFONT::CreateFontW(NULL)->GetFontSize().x )  );
		DWORD m_RowString = 18;
		DWORD m_CulString	= (DWORD)(m_NowCharacterNum / m_RowString + 1 );

		//������̏�����
		m_ShowString.clear();

		//�������ݒ�
		FOR_I( m_CulString )
		{
			IF( i == m_CulString-1 )
			{
				//������̍��
				m_ShowString.push_back( m_text.m_string.substr( i * m_RowString , (DWORD)( m_NowCharacterNum - (float)(i * m_RowString) ) ) );
			}
			else
			{
				//������̍��
				m_ShowString.push_back( m_text.m_string.substr( i * m_RowString , m_RowString ) );
			}
		}

		return false;
	}
	//�`��
	VOID	CShowText::DrawShowText(Vector pos)
	{
		LPTKYFONT font = CFONT::CreateFontW(NULL);
		
		pos.x -= m_text.m_Render.right/2;
		pos.y -= m_text.m_Render.bottom/2;

		//�w�b�h�`��
		font->SetColor(0,0,0);
		font->DrawFontW( (int)pos.x , (int)pos.y  , m_text.m_name.c_str() );
		//�񐔂��擾
		vector<wstring>::iterator iw = m_ShowString.begin();
		FOR( int i = 0; iw != m_ShowString.end() ; i++)
		{
			//���̕��`�悷��
			font->DrawFontW( (int)pos.x , (int)(pos.y + (i+1) * font->GetFontSize().y) ,(iw)->c_str()  );
			iw++;
		}

	}

//////////////////////////////////////�E�B���h�E�\���N���X//////////////////////////////////////
//�E�B���h�E�\���N���X
/*
	//�E�B���h�E���
	TTextWindowInfo m_window;
	//���̎���
	FLOAT m_time;
	//�E�B���h�E�̏��
	wstring m_state;
*/
	//�R���X�g���N�^�A�f�X�g���N�^
	CShowWindow::CShowWindow()
	{
		m_window = NULL;
		m_time = NULL;
		m_state = TKYSHOWWINODW_NULL;
	}
	CShowWindow::CShowWindow( const CShowWindow& t)
	{
		*this = t;
	}
	CShowWindow::~CShowWindow()
	{
	}

	//������
	VOID	CShowWindow::InitializeTextWindow( eTKYSHOWWINODW e )
	{
		m_state = e;
		m_time = NULL;
	}
	//�X�V
	bool		CShowWindow::UpdateTextWindow(float step)
	{
		//�I��
		IF( m_time > 1.0f )return true;
		//���Ԃ�i�߂�
		m_time += step * 0.1f;
		return false;
	}
	//�`��
	VOID	CShowWindow::DrawTextWindow()
	{
		FLOAT time = m_time;
		SWITCH( m_state )
		{
			case TKYSHOWWINODW_ENTER:
			break;
			case TKYSHOWWINODW_LEAVE:
				time = 1.0f - time;
			break;
			case TKYSHOWWINODW_SHOW:
				time = 1.0;
			break;
			case TKYSHOWWINODW_NULL:
				time = NULL;
				return;
			break;
		}
		//�`��
		IF(m_window->m_WindowTexture!=NULL)
		{
			//�����G�̕\��
			vector<TTextWindowCharaInfo>::iterator it = m_window->m_BackChara.begin();
			WHILE( it != m_window->m_BackChara.end() )
			{
				//�QD�����G�`��
				it->m_tex->DrawTexture( it->m_pos , Vector() , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );
				it++;
			}

			//�E�B���h�E�̕\��
			m_window->m_WindowTexture->DrawTexture( 
				m_window->m_WindowPosEndStart.GetVectorMiddle(time) ,
				m_window->m_WindowRotateEndStart.GetVectorMiddle(time) ,
				m_window->m_WindowExpandEndStart.GetVectorMiddle(time) ,
				TKYTEXTURE_TYPE_2D_MODE
				);

		}
	}
	//�e�L�X�g�̈ʒu�̎擾
	Vector CShowWindow::GetTextPos()
	{
//		return m_window.m_textpos;
//		return m_window.m_WindowPosEndStart.GetVectorMiddle(1.0f);
		return Vector( 460,500,0 );
	}

//////////////////////////////////////�����ƃE�B���h�E�̓��@�N���X//////////////////////////////////////
/*
	//�e�L�X�g���
	CShowText			m_Textinfo;
	//�E�B���h�E���
	CShowWindow		m_Winodwinfo;
*/
	//�R���X�g���N�^�A�f�X�g���N�^
	CManageTextWindow::CManageTextWindow()
	{
	}
	CManageTextWindow::CManageTextWindow(const CManageTextWindow& w)
	{
		*this = w;
	}
	CManageTextWindow::~CManageTextWindow()
	{
	}

	//�J�n
	BOOL CManageTextWindow::StartTextWinodw()
	{
		//���郂�[�h�ɂ���
		m_state = TKYMTW_WINODW_ENTER;
		m_Winodwinfo.InitializeTextWindow( TKYSHOWWINODW_ENTER );
		return true;
	}
	BOOL CManageTextWindow::StartTextWinodw( eManageTextWindow info )
	{
		//���郂�[�h�ɂ���
		m_state = info;
		SWITCH( m_state )
		{
			case TKYMTW_WINODW_ENTER:
				m_Winodwinfo.InitializeTextWindow( TKYSHOWWINODW_ENTER );
			break;
			case TKYMTW_TEXT:
				m_Textinfo.InitializeText();
				m_Winodwinfo.InitializeTextWindow( TKYSHOWWINODW_SHOW );
			break;			
			case TKYMTW_WINODW_LEAVE:
				m_Textinfo.InitializeText();
				m_Winodwinfo.InitializeTextWindow( TKYSHOWWINODW_LEAVE );
			break;
			case TKYMTW_NULL:
				m_Winodwinfo.InitializeTextWindow( TKYSHOWWINODW_NULL );
			break;
		}
		return true;
	}
	//�X�V(�I�����ɐ^)
	BOOL CManageTextWindow::UpdateTextWinodw( float step )
	{
		SWITCH( m_state )
		{
			case TKYMTW_WINODW_ENTER:
				IF( m_Winodwinfo.UpdateTextWindow(step) )
				{
					//�e�L�X�g���[�h�ɂ���
					m_state = TKYMTW_TEXT;
					m_Textinfo.InitializeText();
					m_Winodwinfo.InitializeTextWindow(TKYSHOWWINODW_SHOW);
				}
			break;
			case TKYMTW_TEXT:
				IF( m_Textinfo.UpdateText(step) )
				{
					//����郂�[�h�ɂ���
					m_state = TKYMTW_WINODW_LEAVE;
					m_Textinfo.InitializeText();
					m_Winodwinfo.InitializeTextWindow(TKYSHOWWINODW_LEAVE);
				}
			break;			
			case TKYMTW_WINODW_LEAVE:
				IF( m_Winodwinfo.UpdateTextWindow(step) )
				{
					//NULL���[�h�ɂ���
					m_state = TKYMTW_NULL;
					m_Winodwinfo.InitializeTextWindow(TKYSHOWWINODW_NULL);
					return true;
				}
			break;

			case TKYMTW_NULL:
				return true;
			break;
		}
		return false;
	}

	//��C�ɏI��
	VOID CManageTextWindow::EndTextWinodw()
	{
		m_Textinfo.AllShow();
	}

	//�`��
	VOID CManageTextWindow::DrawTextWindow()
	{
		m_Winodwinfo.DrawTextWindow();
		m_Textinfo.DrawShowText( m_Winodwinfo.GetTextPos() );
	}  

	//���̃Z�b�g
	BOOL CManageTextWindow::onLoadGameDate(TGameTextInfo info)
	{
		TGameTextInfo::iterator it = info.begin();
		WHILE( it != info.end() )
		{
			//�e�L�X�g���Z�b�g
			IF( it->m_key == TEXT("TEXT_SET_BASEINFO") )
			{
				TTextInfo* t = ((TTextInfo*)it->m_element);
				m_Textinfo.SetTextInfo( *t );
			}
			//�E�B���h�E���Z�b�g
			EIF( it->m_key == TEXT("TEXTWINDOW_SET_BASEINFO") )
			{
				TTextWindowInfo* t = ((TTextWindowInfo*)it->m_element);
				m_Winodwinfo.SeTTextWindowInfo( t );
			}

			it++;
		}

		//�X�^�[�g�錾
		StartTextWinodw();

		return true;
	}

	
//////////////////////////////////////�I�[�g�L�[���̓N���X//////////////////////////////////////
/*
	//�L�[���
	queue<TAutoKeyInput> m_KeyInfo;
	//�t���[�����
	DWORD m_NowFrame;
*/
	CAutoKeyInput::CAutoKeyInput()
	{
		WHILE(!m_KeyInfo.empty())m_KeyInfo.pop();
		m_NowFrame = NULL;
	}
	CAutoKeyInput::CAutoKeyInput(const CAutoKeyInput& a)
	{
		*this = a;
	}
	CAutoKeyInput::~CAutoKeyInput()
	{
		WHILE(!m_KeyInfo.empty())m_KeyInfo.pop();
		m_NowFrame = NULL;
	}
	//�L�[���̒ǉ�
	VOID CAutoKeyInput::AddKeyInput( TAutoKeyInput key )
	{
		//�ǉ�
		m_KeyInfo.push(key);
	}
	//���̃L�[���̎擾
	TAutoKeyInput CAutoKeyInput::GetNowKeyInput()
	{
		IF( m_KeyInfo.empty() )return TAutoKeyInput();
		return m_KeyInfo.front();
	}
	//�t���[����i�߂�(�����i�܂Ȃ�������FALSE��Ԃ�)
	BOOL CAutoKeyInput::NextFrame()
	{
		IF( m_KeyInfo.empty() )return false;

		m_NowFrame++;
		IF( m_KeyInfo.front().m_FrameNum <= m_NowFrame )
		{
			//�O���̂ĂāA������
			WHILE(!m_KeyInfo.empty())
			{
				m_KeyInfo.pop();
				IF(m_KeyInfo.empty())break;
				IF(m_KeyInfo.front().m_FrameNum > 0)break;
			}
			m_NowFrame=0;
		}
		return true;
	}


//////////////////////////////////////�I�[�g�}�E�X���̓N���X//////////////////////////////////////
/*
	//�L�[���
	queue<TAutoMouseInput> m_MouseInfo;
	//�t���[�����
	DWORD m_NowFrame;
*/
	CAutoMouseInput::CAutoMouseInput()
	{
		WHILE(!m_MouseInfo.empty())m_MouseInfo.pop();
		m_NowFrame = NULL;
	}
	CAutoMouseInput::CAutoMouseInput(const CAutoMouseInput& a)
	{
		*this = a;
	}
	CAutoMouseInput::~CAutoMouseInput()
	{
		WHILE(!m_MouseInfo.empty())m_MouseInfo.pop();
		m_NowFrame = NULL;
	}
	//�L�[���̒ǉ�
	VOID CAutoMouseInput::AddMouseInput( TAutoMouseInput Mouse )
	{
		//�ǉ�
		m_MouseInfo.push(Mouse);
	}
	VOID CAutoMouseInput::AddMouseInput( Vector v ,DWORD f )
	{
		//�ǉ�(�O�̗v�f�̎��̂����n�܂�)
		m_MouseInfo.push( TAutoMouseInput(v , f )  );
	}
	//���̃L�[���̎擾
	Vector CAutoMouseInput::GetNowMouseInput()
	{
		IF( m_MouseInfo.empty() )return Vector();
		//���̈ʒu
		return m_MouseInfo.front().m_MousePos;
	}
	//�t���[����i�߂�(�����i�܂Ȃ�������FALSE��Ԃ�)
	BOOL CAutoMouseInput::NextFrame()
	{
		IF( m_MouseInfo.empty() )return false;

		m_NowFrame++;
		IF( m_MouseInfo.front().m_FrameNum <= m_NowFrame )
		{
			//���ɐi�ށA���������O��������i�܂Ȃ�
			WHILE(!m_MouseInfo.empty())
			{
				m_MouseInfo.pop();
				IF(m_MouseInfo.empty())break;
				IF(m_MouseInfo.front().m_FrameNum > 0)break;
			}
			m_NowFrame=0;
		}
		return true;
	}





//////////////////////////////////////�^�C�g���N���X//////////////////////////////////////
/*

	TTitle m_title;

	//TITLE_LOGO,TITLE_BACK,TITLE_END
	wstring m_state;
*/

	CTitle::CTitle()
	{
		m_state = TEXT("TITLE_LOGO");
		m_title = TTitle();

		m_Score[0] = m_Score[2] = m_Score[2] = 0;

	}
	CTitle::CTitle(const CTitle& c)
	{
		*this = c;
	}
	CTitle::~CTitle()
	{
		IF(m_title.m_back!=NULL)m_title.m_back->DestroyOwnObject();
		IF(m_title.m_logo!=NULL)m_title.m_logo->DestroyOwnObject();
		IF(m_title.m_ranking!=NULL)m_title.m_ranking->DestroyOwnObject();
		IF(m_title.m_score!=NULL)m_title.m_score->DestroyOwnObject();
		IF(m_title.m_start!=NULL)m_title.m_start->DestroyOwnObject();
	}

	VOID CTitle::UpdateTitle(float step)
	{
		IF( m_state == TEXT("TITLE_LOGO") ) 
		{
			m_title.m_logo->Update(step);
		}
		EIF( m_state == TEXT("TITLE_BACK") ) 
		{
			m_title.m_back->Update(step);
			m_title.m_score->Update(step);
			m_title.m_start->Update(step);
		}
	}
	BOOL RectCheck( Vector pos , Vector size , Vector s )
	{
		RECT r = { (LONG)size.x  - (LONG)s.x/2 , (LONG)size.y - (LONG)s.y/2 , (LONG)size.x  + (LONG)s.x/2 , (LONG)size.y + (LONG)s.y/2 };
		IF( r.left < pos.x && r.right > pos.x && r.top < pos.y && r.bottom > pos.y )
			return true;
		return false;
	}

	VOID CTitle::DrawTitle()
	{
		IF( m_state == TEXT("TITLE_LOGO") ) 
		{
			m_title.m_logo->DrawTexture( Vector(400,300,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );
		}
		EIF( m_state == TEXT("TITLE_SCORE") ) 
		{
			m_title.m_ranking->DrawTexture( Vector(400,300,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );

			//�X�R�A
			CFONT::CreateFontW(NULL)->SetColor(255,255,255);
			char str[200];
			sprintf_s(str,"%d", m_Score[0] );
			CFONT::CreateFontW(NULL)->DrawFontA(610,300,str);
			sprintf_s(str,"%d", m_Score[1] );
			CFONT::CreateFontW(NULL)->DrawFontA(610,420,str);
			sprintf_s(str,"%d", m_Score[2] );
			CFONT::CreateFontW(NULL)->DrawFontA(610,504,str);

		}
		EIF( m_state == TEXT("TITLE_BACK") ) 
		{
			m_title.m_back->DrawTexture( Vector(400,300,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );

			//�������Ă���(�X�^�[�g) 
			IF( RectCheck( pos , Vector( 400,350,0 ) , Vector( (float)m_title.m_start->GetTextureInfo().m_width , (float)m_title.m_start->GetTextureInfo().m_height , 0 ) ) )
			{
				m_title.m_start->DrawTextureIndex( Vector(400,350,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE , 1 );
			}
			else
			{
				m_title.m_start->DrawTextureIndex( Vector(400,350,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE , 0 );
			}
			
			//�������Ă���(�X�R�A)
			IF( RectCheck( pos , Vector( 400,450,0 ) , Vector( (float)m_title.m_score->GetTextureInfo().m_width , (float)m_title.m_score->GetTextureInfo().m_height , 0 ) ) )
			{
				m_title.m_score->DrawTextureIndex( Vector(400,450,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE , 1 );
			}
			else
			{
				m_title.m_score->DrawTextureIndex( Vector(400,450,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE , 0 );
			}

		}
	}

	VOID CTitle::Next()
	{
		IF( m_state == TEXT("TITLE_LOGO") ) 
		{
			m_state = TEXT("TITLE_BACK");
		}
		EIF( m_state == TEXT("TITLE_SCORE") ) 
		{
			m_state = TEXT("TITLE_BACK");
		}
		EIF( m_state == TEXT("TITLE_BACK") ) 
		{
			//���������Ă���(�v���C)
			IF( RectCheck( pos , Vector( 400,350,0 ) , Vector( (float)m_title.m_start->GetTextureInfo().m_width , (float)m_title.m_start->GetTextureInfo().m_height , 0 ) ) )
			{
				m_state = TEXT("TITLE_END");
			}
			IF( RectCheck( pos , Vector( 400,450,0 ) , Vector( (float)m_title.m_score->GetTextureInfo().m_width , (float)m_title.m_score->GetTextureInfo().m_height , 0 ) ) )
			{


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
					swscanf_s( key.c_str() , TEXT("%u"),&m_Score[0] );
					key = read.GetToWhiteSpace();
					swscanf_s( key.c_str() , TEXT("%u"),&m_Score[1] );
					key = read.GetToWhiteSpace();
					swscanf_s( key.c_str() , TEXT("%u"),&m_Score[2] );
				}

				//�J��
				read.Close();

				m_state = TEXT("TITLE_SCORE");
			}
		}
	}
	BOOL CTitle::IsEnd()
	{
		return (m_state == TEXT("TITLE_END") );
	}




//////////////////////////////////////�X�R�A�N���X//////////////////////////////////////
/*
	TScore m_score;
*/

	CScore::CScore()
	{
		m_score.initialize();
	}
	CScore::CScore(const CScore& t)
	{
		*this = t;
	}
	CScore::~CScore()
	{
		m_score.initialize();
	}

	VOID CScore::UpdateScore(float step)
	{
		IF( m_score.m_state == TKYSCORE_START )
		{
			//���Ԏ擾����
			m_score.m_time = timeGetTime();
			//���ւ���
			m_score.m_state = TKYSCORE_PLAY;
		}
		EIF( m_score.m_state == TKYSCORE_PLAY )
		{
			m_score.m_ShootingScore->Update(step);
		}
		EIF( m_score.m_state == TKYSCORE_RESULT )
		{
			m_score.m_ScoreBack->Update(step);
		}
	}


	VOID CScore::DrawScore()
	{

		IF( m_score.m_state == TKYSCORE_START )
		{
		}
		EIF( m_score.m_state == TKYSCORE_PLAY )
		{
			m_score.m_ScoreBack->DrawTexture( Vector(400,300,0) , Vector() , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );

			INT hitpoint = NULL;
			IF( m_score.m_chara != NULL )hitpoint = m_score.m_chara->GetCharaInfo().m_HitPoint;

			//�����̕`��==============================
			IF( m_score.m_chara != NULL )
			{
				CFONT::CreateFontW(NULL)->SetColor(0,0,0);
				char str[200];
				sprintf_s(str,"%d", hitpoint );
				CFONT::CreateFontW(NULL)->DrawFontA(700,50,str);
				sprintf_s(str,"%d", m_score.m_score );
				CFONT::CreateFontW(NULL)->DrawFontA(510,70,str);
			}
			//========================================

		}
		EIF( m_score.m_state == TKYSCORE_GAMEOVER )
		{
			m_score.m_Gameover->DrawTexture( Vector(400,300,0) , Vector() , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );
		}
		EIF( m_score.m_state == TKYSCORE_RESULT )
		{
			m_score.m_ShootingScore->DrawTexture( Vector(400,300,0) , Vector() , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );

			//�����̕`��==============================
			IF( m_score.m_chara != NULL )
			{
				CFONT::CreateFontW(NULL)->SetColor(0,0,0);
				char str[200];
				sprintf_s(str,"%d", m_score.m_BatteryNum );
				CFONT::CreateFontW(NULL)->DrawFontA(690,240,str);
				float r = ( (float)m_score.m_HitNum / (float)(m_score.m_BatteryNum*4) );
				sprintf_s(str,"%d��",  (UINT)(  r * 100.0f ) );
				CFONT::CreateFontW(NULL)->DrawFontA(640,320,str);
				sprintf_s(str,"%d", m_score.m_score );
				CFONT::CreateFontW(NULL)->DrawFontA(540,400,str);

				IF( r > 0.6 ){sprintf_s(str,"S");}
				EIF( r > 0.5 ){sprintf_s(str,"A");}
				EIF( r > 0.4 ){sprintf_s(str,"B+" );}
				EIF( r > 0.3 ){sprintf_s(str,"B" );}
				EIF( r > 0.2 ){sprintf_s(str,"C" );}
				CFONT::CreateFontW(NULL)->DrawFontA(650,510,str);
			}
			//========================================

		}
	}






//////////////////////////////////////�V�F�C�h�N���X//////////////////////////////////////
/*
	LPTkyTexture m_shade;
	eShade m_State;
	float m_alpha;
*/

	CShade::CShade()
	{
		m_shade = NULL;
		m_State = TKYSHADE_NULL;
		m_alpha = 0.0f;
	}
	CShade::CShade(const CShade& s)
	{
		*m_shade = *s.m_shade;
		m_alpha = s.m_alpha;
		m_State = s.m_State;
		m_run_id = NULL;
	}
	CShade::~CShade()
	{
		m_shade->DestroyOwnObject();
	}

	VOID CShade::Read(wstring name)
	{
		m_shade = CTexture::CreateTextureW( Init3D::CreateInit3D() , name , 1,1,1,800,600,1 );
	}

	VOID CShade::UpdateShade(float step)
	{
		SWITCH( m_State )
		{
			case TKYSHADE_IN:
				m_alpha += step*0.1f;
				IF( m_alpha >= 1.0f )
				{
					//�V�F�[�h�C���̏I��
					m_alpha = 1.0f;
					m_State = TKYSHADE_MIDDLE;
					CEventManager::StaticAddEventMessage( "END_SHADE_IN" , m_run_id);
				}
			break;
			case TKYSHADE_MIDDLE:
				m_State = TKYSHADE_OUT;
			break;
			case TKYSHADE_OUT:
				m_alpha -= step*0.1f;
				IF( m_alpha <= 0.0f )
				{
					m_alpha = 0.0f;
					m_State = TKYSHADE_NULL;
					CEventManager::StaticAddEventMessage( "END_SHADE" , m_run_id);
					m_run_id = 1<< 21;
				}
			break;
			case TKYSHADE_NULL:
			break;
		}
	}
	VOID CShade::DrawShade()
	{
		IF( m_State != TKYSHADE_NULL )
		{
			IF( m_shade != NULL )m_shade->DrawTextureAlpha( m_alpha , Vector( 400,300,0 ) , Vector() , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE , false );
		}
	}

	//�A�b�v�f�[�g���Ă�������
	bool CShade::IsUpdate()
	{
		IF( m_State == TKYSHADE_MIDDLE || m_State == TKYSHADE_NULL )
		{
			return true;
		}
		return false;
	}

	//�͂��߂�
	VOID CShade::Run(DWORD id)
	{
		IF( m_run_id == id )
			return;
		m_State = TKYSHADE_IN;
		m_alpha = 0;
		m_run_id = id;
	}
