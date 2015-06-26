
#include "DataStructure.h"


//============================================================
//ゲームのデータ構造
//============================================================

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//エフェクト情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//エフェクトの作成
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


//////////////////////////////////////エフェクト行動クラス//////////////////////////////////////
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
				//ループが終わったことを示す
				m_oneLoop = true;
			}
			else
			{
				//ループが終わっていない
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

//////////////////////////////////////エフェクト管理クラス//////////////////////////////////////
	//コンストラクタ、デストラクタ
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
	//追加
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
	//消去
	BOOL CManageEffect::DeleteEffectData( wstring name )
	{
		return m_EffectMatch.Destroy( name );
	}
	//更新
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
	//再生
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
	//設定
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

	//取得
	LPTkyGameEffect CManageEffect::GetEffect(wstring name )
	{
		CGameEffect *ce = NULL;
		IF( m_EffectMatch.GetElement( name , &ce ) )
		{
			return ce;
		}
		return NULL;
	}

	//ループが終了しているかどうか
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
//キャラクタ情報
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//////////////////////////////////////アニメーション行動クラス//////////////////////////////////////

	//コンストラクタ、デストラクタ、
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
		//モデルがなくても通す
		m_Animation = t;
		return true;
	}
	BOOL CGameAnimation::UpdateAnimation(float step)
	{
		//フレームを進める
		IF(m_Animation.m_model!=NULL)
		{
			m_Animation.m_NowFrame  =  m_Animation.m_model->GetAnime()->UpdateFrame( m_Animation.m_NowFrame , step );
			//0だったら
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

		//モデルがなかったら
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

//////////////////////////////////////アニメーション管理クラス//////////////////////////////////////
	
	//コンストラクタ、デストラクタ
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
	//初期化
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
	//追加
	BOOL CManageAnimation::AddAnimationData( wstring name,LPTkyXFile model)
	{
		//モデルがなくてもいいことにする
//		IF( model == NULL )return false;
		TGameAnimation am(model , NULL );
		CGameAnimation ga;
		ga.SetAnimationInfo(am);
		m_XAnimeMatch.AddString( name , ga );
		return true;
	}
	//消去
	BOOL CManageAnimation::DeleteAnimationData( wstring name)
	{
		return m_XAnimeMatch.Destroy( name );
	}
	//更新
	BOOL CManageAnimation::UpdateAnimationData( wstring name ,float step)
	{
		CGameAnimation *ga = NULL;
		IF( m_XAnimeMatch.GetElement( name , &ga ) )
		{
			IF(ga!=NULL)
			{
				//ループが終わったか見る
				ga->UpdateAnimation(step);
			}
			return true;
		}
		return false;
	}
	//再生
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
	//ループが終了しているかどうか
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

////////////////////////////////////攻撃クラス//////////////////////////////////////

	//コンストラクタ、デストラクタ
	CBullet::CBullet()
	{
		//初期化
		m_Bullet.Initialize();
		m_OriginPos = Vector();
		Initialize();
		m_end = false;
		//モードの設定
		SetMode(TEXT("MODEL_INVINCIBLE"));
		//衝突時メッセージ
		SetCollisionMessage( TEventMessage::Initializel( "BULLET_MODEL" , (DWORD)this ) );
		//情報
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
	//接触情報(返却地、衝突後離れるかどうか)
	bool CBullet::onCollisionEvent(PhysicsModel *model)
	{
		//普通の攻撃ならあったら当たったやつを記録して
		//攻撃が終わるまでそいつに当たらないようにする

		//弾なら、あったら終わり

		//キャラ情報
		LPTkyCharactor BatteryChara = this->GetChara();
		IF( BatteryChara == NULL )return false;

		//打ったキャラの名前
		string bullet = BatteryChara->GetCollisionMessage().m_message;

		//キャラクタの判定
		IF( model->GetCollisionMessage().m_message == "ENEMY_CHARACTOR" && bullet == "HERO_CHARACTOR" )
		{
			//敵にあったら終了
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
	//終了しているか取得
	bool CBullet::IsFinish()
	{
		//出発位置からの相対距離
		float l = ( GetPos() - m_BatteryPosition ).GetLarge();

		//もし予定距離まで行ったら終了
		IF( l > GetBulletInfo().m_Life )
		{
			m_end = true;
		}

		return m_end;
	}


//////////////////////////////////////武器//////////////////////////////////////

	//ビューとして常駐し、攻撃いベントにより攻撃する
	//コンストラクタ、デストラクタ
	CWeapon::CWeapon()
	{
		//攻撃情報
		m_Bullet.Initialize();
		//武器情報
		m_Weapon.Initialize();

		//今の情報
		m_NowState = "WEAPON_NONE";
	}
	CWeapon::CWeapon(const CWeapon& w) : IGameLoadText(w)
	{
		//攻撃情報
		m_Bullet = w.GetBulletInfo();
		//武器情報
		m_Weapon = w.GetWeaponInfo();

		//今の情報
		m_NowState = w.GetNowState();

	}
	CWeapon::~CWeapon()
	{
		//攻撃情報
		m_Bullet.Initialize();
		//武器情報
		m_Weapon.Initialize();
	}
	//攻撃状態
	VOID CWeapon::pAttack()
	{
		//弾イベントの作成
		TEventMessage bullet;
		bullet.m_message = "FPSLOGIC_ADDVIEW_BULLET";
		bullet.m_value.resize(3);
		bullet.m_value[0] = (DWORD)this;
		bullet.m_value[1] = (DWORD)(&m_chara->GetPos());
		bullet.m_value[2] = (DWORD)(&(m_chara->GetCharaInfo().m_AttackDirection));
		CEventManager::StaticAddEventMessage( bullet );

		//エフェクトの生成
		CreateEffectMessage( *m_EffectData.GetEffect(TEXT("WEAPON_EFFECT_FLASH")) , Init3D::CreateInit3D()->GetScreenSize()/2 );

		CreateEffectMessage( *m_EffectData.GetEffect(TEXT("WEAPON_EFFECT_SHOT")) , m_pos );

		//主人公だったら攻撃メッセージ
		IF( m_chara->GetCollisionMessage().m_message == "HERO_CHARACTOR" )
		{
			//発射イベント
			CEventManager::StaticAddEventMessage( "SCORE_BATTERY" , NULL);
		}

	}
	//攻撃
	BOOL CWeapon::Attack(Vector p)
	{
		//もし発射してなかったら発射
		IF(	m_NowState == "WEAPON_NONE")
		{
			//もし死んでいなかったら
			IF( m_chara->GetCharaInfo().m_HitPoint > 0  )
			{

				//攻撃を始める
				m_NowState = "WEAPON_PRELAUNCH";
				m_pos = p;		//現在地の設定
				IF( ISEPSILON( m_Weapon.m_FiringBulletsLate , 0 ) )
			{
				pAttack();	//攻撃処理
			}
				m_time = 0;


				m_chara->SetState( "CHARACTOR_ATTACK" );
			}
			return true;
		}
		return false;
	}
	//攻撃中なら弾をどこかで生成する
	BOOL CWeapon::UpdateAttack(float step)
	{
		//もしHPが0だったら終了
		IF(m_chara->GetCharaInfo().m_HitPoint <= 0)
		{
			m_NowState = "WEAPON_NONE";
			m_time = 0;
			IF( m_chara->GetState() != "CHARACTOR_DEAD" && m_chara->GetState() != "CHARACTOR_NULL" )
			{
				m_chara->SetState( "CHARACTOR_DEAD" );
			}
		}

		//攻撃中じゃなかったら
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
			//進める
			m_time += step;
			//送れ時間以上なったら発射
			IF( m_time > (float)m_Weapon.m_FiringBulletsLate )
			{
				m_NowState = "WEAPON_SHOT";
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
//		もう作成したのでここでは攻撃イベントの作成
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
				IF( !ISEPSILON( m_Weapon.m_FiringBulletsLate , 0) )
				{
					pAttack();
				}
			}
		}
		EIF(	m_NowState == "WEAPON_SHOT")
		{

			//アニメーションが終わったら
			IF( m_AnimationData.IsFinish( TEXT("WEAPON_ANIMATION_ATTACK") ) )
			{
				m_NowState = "WEAPON_NONE";
				m_time = 0;
				m_chara->SetState( "CHARACTOR_WALK" );
			}

		}

		//アニメーションの更新
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
	//攻撃中が終わったイベント
	BOOL CWeapon::EndAttack()
	{
		//発射中だったら終わりにする
		IF(	m_NowState == "WEAPON_NONE")
		{
			return true;
		}
		return false;
	}
	//読み込み
	BOOL CWeapon::onLoadGameDate(TGameTextInfo info)
	{
		TGameTextInfo::iterator it = info.begin();
		WHILE( it != info.end() )
		{
			//武器アニメーションファイルの追加
			IF( it->m_key == TEXT("WEAPON_SET_ANIMATION") )
			{
				m_AnimationData.AddAnimationData( it->m_text , (LPTkyXFile)it->m_element );
			}
			//武器エフェクトファイルの追加
			EIF( it->m_key == TEXT("WEAPON_SET_EFFECT") )
			{
				m_EffectData.AddEffectData( it->m_text , (LPTkyTexture)it->m_element , 1 );
			}
			//武器基本情報
			IF( it->m_key == TEXT("WEAPON_SET_BASEINFO") )
			{
				m_Weapon = *((TWeaponInfo*)it->m_element);
			}
			//弾基本情報
			EIF( it->m_key == TEXT("ATTACK_SET_BASEINFO") )
			{
				m_Bullet = *((TBulletInfo*)it->m_element);
			}

			it++;
		}
		return true;
	}

//////////////////////////////////////メインキャラクタークラス//////////////////////////////////////
//独自要素
	//コンストラクタ、デストラクタ
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
//PhysicsCircleModel要素
	//接触情報(返却地、衝突後離れるかどうか)  (2回攻撃されるので注意)
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
				//弾があたったイベント
				CEventManager::StaticAddEventMessage( "SCORE_HIT" , NULL);

				//死んでいたらあたらない
				IF( m_chara.m_HitPoint > 0  )
				{
					//弾があたっていたら場所により点数
					//内積を取りそれによって点数を決める
					float score_rate = ( (m_pos-model->GetPos()).Normal() ^ model->GetVel().Normal() )  * 100.0f;
					CEventManager::StaticAddEventMessage( "SCORE_SCORE" , (DWORD)score_rate );


					//弾にあたって自分が敵だったら
					//攻撃中じゃなかったら
					IF( m_state != "CHARACTOR_ATTACK" )
					{
						m_AnimationData.InitializeAnimationData( TEXT("TKYMODEL_HIT") );
						m_state = "CHARACTOR_HIT";
					}

					//体力現象
					Damage(( (LPTkyBullet)model->GetCollisionMessage().m_value[0] )->GetBulletInfo().m_AttackPower );
					IF( m_state != "CHARACTOR_DEAD" && m_chara.m_HitPoint <= 0 )
					{
						//攻撃されて死んだら
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

					//もし０だったらゲームオーバーへ
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
//					ゲームオーバーエフェクトの追加
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee

				}

				return false;
			}
		}


		return false;
	}

	//ダメージ
	VOID CCharactor::Damage( float damage)
	{
		m_chara.m_HitPoint -= (int)damage;

		//ダメージエフェクト
		CreateEffectMessage( *m_EffectData.GetEffect( TEXT("TKYEFFECT_DAMAGE") ) , Vector( 400,300,0 ) );

	}

	
//独自要素
	//更新
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
			//ループが終わっていたら歩きへ
			IF( m_AnimationData.IsFinish(TEXT("TKYMODEL_HIT"))  ) m_state = "CHARACTOR_WALK";
		}
		EIF( m_state == "CHARACTOR_DEAD" )
		{
			IF(!m_AnimationData.UpdateAnimationData( TEXT("TKYMODEL_DEAD") , step ))
			{
				return false;
			}
			//ループが終わっていたら次のループ
			IF( m_AnimationData.IsFinish(TEXT("TKYMODEL_DEAD"))  )
			{
				m_state = "CHARACTOR_NULL";
			}
		}
		EIF( m_state == "CHARACTOR_NULL" )
		{
			//何もしない
		}
		EIF( m_state == "CHARACTOR_ATTACK" )
		{
			//攻撃中
		}
		else
		{
			MessageBoxW( NULL , TEXT("キャラ更新失敗NONE") , TEXT("err") , NULL );
		}

		return true;
	}
	//描画(位置情報はPhysicsCircleModelからとる)
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
	//イベント(送られてきたイベントによりエフェクトイベントでも作成する)
	VOID CCharactor::EventCharactor( string name, DWORD message )
	{
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
//			送られてきたメッセにおうじて
//			様々なイベントを作成する。
//			　	エッフェクトビューやAIビューなど
//				できればメッセージにはロジックのポインタがほしいところ(-_-)
//eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
	}

	//ゲームデータの読み込み
	BOOL CCharactor::onLoadGameDate(TGameTextInfo info)
	{
		TGameTextInfo::iterator it = info.begin();
		WHILE( it != info.end() )
		{
			//Xファイルの追加
			IF( it->m_key == TEXT("CHARACTOR_SET_ANIMATION") )
			{
				m_AnimationData.AddAnimationData( it->m_text , (LPTkyXFile)it->m_element );
			}
			//エフェクトの追加
			EIF( it->m_key == TEXT("CHARACTOR_SET_EFFECT") )
			{
				m_EffectData.AddEffectData( it->m_text , &TGameEffect( (LPTkyTexture)it->m_element , 1 ) );
			}
			//基本情報
			EIF( it->m_key == TEXT("CHARACTOR_SET_BASEINFO") )
			{
				SetCharaInfo( *((TCharaInfo*)it->m_element) );
			}

			it++;
		}
		return true;
	}



//////////////////////////////////////文字表示クラス//////////////////////////////////////
// 文字の表示を受け持つ、
/*
	//テキスト情報
	TTextInfo m_text;
	//今の表示されている文字列(列で分ける)
	vector<wstring> m_ShowString;
	//今の表示文字数
	FLOAT	m_NowCharacterNum;
*/
	//コンストラクタ、デストラクタ
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

	//文字の情報初期化
	VOID	CShowText::InitializeText()
	{
		m_ShowString.clear();
		m_NowCharacterNum = NULL;		
	}
	//更新(終了していたら真を返す)
	bool		CShowText::UpdateText(float step)
	{
		//終了条件
		IF( m_text.m_string.size() < m_NowCharacterNum )return true;

		//文字を進める
		m_NowCharacterNum += m_text.m_ShowSpeed;

		//一行,列の文字列を返す
//		DWORD m_RowString = (DWORD)(  ( (float)abs(m_text.m_Render.right - m_text.m_Render.left)  /  CFONT::CreateFontW(NULL)->GetFontSize().x )  );
		DWORD m_RowString = 18;
		DWORD m_CulString	= (DWORD)(m_NowCharacterNum / m_RowString + 1 );

		//文字列の初期化
		m_ShowString.clear();

		//文字列を設定
		FOR_I( m_CulString )
		{
			IF( i == m_CulString-1 )
			{
				//文字列の搾取
				m_ShowString.push_back( m_text.m_string.substr( i * m_RowString , (DWORD)( m_NowCharacterNum - (float)(i * m_RowString) ) ) );
			}
			else
			{
				//文字列の搾取
				m_ShowString.push_back( m_text.m_string.substr( i * m_RowString , m_RowString ) );
			}
		}

		return false;
	}
	//描画
	VOID	CShowText::DrawShowText(Vector pos)
	{
		LPTKYFONT font = CFONT::CreateFontW(NULL);
		
		pos.x -= m_text.m_Render.right/2;
		pos.y -= m_text.m_Render.bottom/2;

		//ヘッド描画
		font->SetColor(0,0,0);
		font->DrawFontW( (int)pos.x , (int)pos.y  , m_text.m_name.c_str() );
		//列数を取得
		vector<wstring>::iterator iw = m_ShowString.begin();
		FOR( int i = 0; iw != m_ShowString.end() ; i++)
		{
			//その分描画する
			font->DrawFontW( (int)pos.x , (int)(pos.y + (i+1) * font->GetFontSize().y) ,(iw)->c_str()  );
			iw++;
		}

	}

//////////////////////////////////////ウィンドウ表示クラス//////////////////////////////////////
//ウィンドウ表示クラス
/*
	//ウィンドウ情報
	TTextWindowInfo m_window;
	//今の時間
	FLOAT m_time;
	//ウィンドウの状態
	wstring m_state;
*/
	//コンストラクタ、デストラクタ
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

	//初期化
	VOID	CShowWindow::InitializeTextWindow( eTKYSHOWWINODW e )
	{
		m_state = e;
		m_time = NULL;
	}
	//更新
	bool		CShowWindow::UpdateTextWindow(float step)
	{
		//終了
		IF( m_time > 1.0f )return true;
		//時間を進める
		m_time += step * 0.1f;
		return false;
	}
	//描画
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
		//描画
		IF(m_window->m_WindowTexture!=NULL)
		{
			//立ち絵の表示
			vector<TTextWindowCharaInfo>::iterator it = m_window->m_BackChara.begin();
			WHILE( it != m_window->m_BackChara.end() )
			{
				//２D立ち絵描画
				it->m_tex->DrawTexture( it->m_pos , Vector() , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE );
				it++;
			}

			//ウィンドウの表示
			m_window->m_WindowTexture->DrawTexture( 
				m_window->m_WindowPosEndStart.GetVectorMiddle(time) ,
				m_window->m_WindowRotateEndStart.GetVectorMiddle(time) ,
				m_window->m_WindowExpandEndStart.GetVectorMiddle(time) ,
				TKYTEXTURE_TYPE_2D_MODE
				);

		}
	}
	//テキストの位置の取得
	Vector CShowWindow::GetTextPos()
	{
//		return m_window.m_textpos;
//		return m_window.m_WindowPosEndStart.GetVectorMiddle(1.0f);
		return Vector( 460,500,0 );
	}

//////////////////////////////////////文字とウィンドウの動機クラス//////////////////////////////////////
/*
	//テキスト情報
	CShowText			m_Textinfo;
	//ウィンドウ情報
	CShowWindow		m_Winodwinfo;
*/
	//コンストラクタ、デストラクタ
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

	//開始
	BOOL CManageTextWindow::StartTextWinodw()
	{
		//入るモードにする
		m_state = TKYMTW_WINODW_ENTER;
		m_Winodwinfo.InitializeTextWindow( TKYSHOWWINODW_ENTER );
		return true;
	}
	BOOL CManageTextWindow::StartTextWinodw( eManageTextWindow info )
	{
		//入るモードにする
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
	//更新(終了時に真)
	BOOL CManageTextWindow::UpdateTextWinodw( float step )
	{
		SWITCH( m_state )
		{
			case TKYMTW_WINODW_ENTER:
				IF( m_Winodwinfo.UpdateTextWindow(step) )
				{
					//テキストモードにする
					m_state = TKYMTW_TEXT;
					m_Textinfo.InitializeText();
					m_Winodwinfo.InitializeTextWindow(TKYSHOWWINODW_SHOW);
				}
			break;
			case TKYMTW_TEXT:
				IF( m_Textinfo.UpdateText(step) )
				{
					//離れるモードにする
					m_state = TKYMTW_WINODW_LEAVE;
					m_Textinfo.InitializeText();
					m_Winodwinfo.InitializeTextWindow(TKYSHOWWINODW_LEAVE);
				}
			break;			
			case TKYMTW_WINODW_LEAVE:
				IF( m_Winodwinfo.UpdateTextWindow(step) )
				{
					//NULLモードにする
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

	//一気に終了
	VOID CManageTextWindow::EndTextWinodw()
	{
		m_Textinfo.AllShow();
	}

	//描画
	VOID CManageTextWindow::DrawTextWindow()
	{
		m_Winodwinfo.DrawTextWindow();
		m_Textinfo.DrawShowText( m_Winodwinfo.GetTextPos() );
	}  

	//情報のセット
	BOOL CManageTextWindow::onLoadGameDate(TGameTextInfo info)
	{
		TGameTextInfo::iterator it = info.begin();
		WHILE( it != info.end() )
		{
			//テキスト情報セット
			IF( it->m_key == TEXT("TEXT_SET_BASEINFO") )
			{
				TTextInfo* t = ((TTextInfo*)it->m_element);
				m_Textinfo.SetTextInfo( *t );
			}
			//ウィンドウ情報セット
			EIF( it->m_key == TEXT("TEXTWINDOW_SET_BASEINFO") )
			{
				TTextWindowInfo* t = ((TTextWindowInfo*)it->m_element);
				m_Winodwinfo.SeTTextWindowInfo( t );
			}

			it++;
		}

		//スタート宣言
		StartTextWinodw();

		return true;
	}

	
//////////////////////////////////////オートキー入力クラス//////////////////////////////////////
/*
	//キー情報
	queue<TAutoKeyInput> m_KeyInfo;
	//フレーム情報
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
	//キー情報の追加
	VOID CAutoKeyInput::AddKeyInput( TAutoKeyInput key )
	{
		//追加
		m_KeyInfo.push(key);
	}
	//今のキー情報の取得
	TAutoKeyInput CAutoKeyInput::GetNowKeyInput()
	{
		IF( m_KeyInfo.empty() )return TAutoKeyInput();
		return m_KeyInfo.front();
	}
	//フレームを進める(もし進まなかったらFALSEを返す)
	BOOL CAutoKeyInput::NextFrame()
	{
		IF( m_KeyInfo.empty() )return false;

		m_NowFrame++;
		IF( m_KeyInfo.front().m_FrameNum <= m_NowFrame )
		{
			//前を捨てて、初期化
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


//////////////////////////////////////オートマウス入力クラス//////////////////////////////////////
/*
	//キー情報
	queue<TAutoMouseInput> m_MouseInfo;
	//フレーム情報
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
	//キー情報の追加
	VOID CAutoMouseInput::AddMouseInput( TAutoMouseInput Mouse )
	{
		//追加
		m_MouseInfo.push(Mouse);
	}
	VOID CAutoMouseInput::AddMouseInput( Vector v ,DWORD f )
	{
		//追加(前の要素の次のやつから始まる)
		m_MouseInfo.push( TAutoMouseInput(v , f )  );
	}
	//今のキー情報の取得
	Vector CAutoMouseInput::GetNowMouseInput()
	{
		IF( m_MouseInfo.empty() )return Vector();
		//今の位置
		return m_MouseInfo.front().m_MousePos;
	}
	//フレームを進める(もし進まなかったらFALSEを返す)
	BOOL CAutoMouseInput::NextFrame()
	{
		IF( m_MouseInfo.empty() )return false;

		m_NowFrame++;
		IF( m_MouseInfo.front().m_FrameNum <= m_NowFrame )
		{
			//次に進む、もし次が０だったら進まない
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





//////////////////////////////////////タイトルクラス//////////////////////////////////////
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

			//スコア
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

			//あたっている(スタート) 
			IF( RectCheck( pos , Vector( 400,350,0 ) , Vector( (float)m_title.m_start->GetTextureInfo().m_width , (float)m_title.m_start->GetTextureInfo().m_height , 0 ) ) )
			{
				m_title.m_start->DrawTextureIndex( Vector(400,350,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE , 1 );
			}
			else
			{
				m_title.m_start->DrawTextureIndex( Vector(400,350,0) , Vector(0,0,0) , Vector(1,1,1) , TKYTEXTURE_TYPE_2D_MODE , 0 );
			}
			
			//あたっている(スコア)
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
			//あったっている(プレイ)
			IF( RectCheck( pos , Vector( 400,350,0 ) , Vector( (float)m_title.m_start->GetTextureInfo().m_width , (float)m_title.m_start->GetTextureInfo().m_height , 0 ) ) )
			{
				m_state = TEXT("TITLE_END");
			}
			IF( RectCheck( pos , Vector( 400,450,0 ) , Vector( (float)m_title.m_score->GetTextureInfo().m_width , (float)m_title.m_score->GetTextureInfo().m_height , 0 ) ) )
			{


				CTkyFileRead read;

				//読み込み
				IF(!read.Open(TEXT("text/score.txt")))
				{
					MessageBoxW(NULL,TEXT("スコアが読み込めません"),TEXT(""),NULL);
					return;
				}

				//キャラ情報読み込み
				WHILE(!read.IsEof())
				{
					wstring key = read.GetToWhiteSpace();
					swscanf_s( key.c_str() , TEXT("%u"),&m_Score[0] );
					key = read.GetToWhiteSpace();
					swscanf_s( key.c_str() , TEXT("%u"),&m_Score[1] );
					key = read.GetToWhiteSpace();
					swscanf_s( key.c_str() , TEXT("%u"),&m_Score[2] );
				}

				//開放
				read.Close();

				m_state = TEXT("TITLE_SCORE");
			}
		}
	}
	BOOL CTitle::IsEnd()
	{
		return (m_state == TEXT("TITLE_END") );
	}




//////////////////////////////////////スコアクラス//////////////////////////////////////
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
			//時間取得して
			m_score.m_time = timeGetTime();
			//次へいく
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

			//文字の描画==============================
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

			//文字の描画==============================
			IF( m_score.m_chara != NULL )
			{
				CFONT::CreateFontW(NULL)->SetColor(0,0,0);
				char str[200];
				sprintf_s(str,"%d", m_score.m_BatteryNum );
				CFONT::CreateFontW(NULL)->DrawFontA(690,240,str);
				float r = ( (float)m_score.m_HitNum / (float)(m_score.m_BatteryNum*4) );
				sprintf_s(str,"%d％",  (UINT)(  r * 100.0f ) );
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






//////////////////////////////////////シェイドクラス//////////////////////////////////////
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
					//シェードインの終了
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

	//アップデートしてもいいか
	bool CShade::IsUpdate()
	{
		IF( m_State == TKYSHADE_MIDDLE || m_State == TKYSHADE_NULL )
		{
			return true;
		}
		return false;
	}

	//はじめる
	VOID CShade::Run(DWORD id)
	{
		IF( m_run_id == id )
			return;
		m_State = TKYSHADE_IN;
		m_alpha = 0;
		m_run_id = id;
	}
