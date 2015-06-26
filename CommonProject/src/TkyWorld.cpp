#include "TkyWorld.h"

//============================================
//CPhysicsModelTask
//============================================
	CPhysicsModelTask::CPhysicsModelTask()
	{
		m_model = NULL;
		m_models = NULL;
	}
	CPhysicsModelTask::CPhysicsModelTask( LPPhysicsModel model)
	{
		m_model = model;
		m_models = NULL;
	}
	CPhysicsModelTask::~CPhysicsModelTask()
	{
	}
//タスク要素
	BOOL CPhysicsModelTask::onCreate(){return true;};
	BOOL CPhysicsModelTask::onUpdate(float step)
	{

		IF( m_models == NULL )return true;
		
		//本命モデル
		vector<PhysicsModel*> ColisionModel;

		//モデル選定
		list<PhysicsModel*>::iterator im = m_models->begin();
		WHILE( im != m_models->end() )
		{
			//重複解除
			IF(*im==m_model ){im++;continue;}

			//簡易あたり判定
			IF( CollisionProcessEasy(m_model,*im) )
			{
				ColisionModel.push_back(*im);
			}	
			im++;
		}

		//本命あたり判定
		vector<PhysicsModel*>::iterator ic = ColisionModel.begin();
		WHILE( ic != ColisionModel.end() )
		{

			IF( CollisionProcess(m_model,*ic) )
			{	//あたっていたら衝突イベントへ
				m_model->onCollisionEvent(*ic);
			}

			ic++;

		}

		//衝突判定
		m_model->CollisionRegister();


		return true;
	};
//タスクの生成
	LPTkyPhysicsModelTask CPhysicsModelTask::CreatePhysicsModelTask( LPPhysicsModel model )
	{
		return new CPhysicsModelTask(model);
	}

//============================================
//CPhysics3DModelTask
//============================================
	CPhysics3DModelTask::CPhysics3DModelTask()
	{
		m_model = NULL;
		m_models = NULL;
	}
	CPhysics3DModelTask::CPhysics3DModelTask( LPPhysicsModel model)
	{
		m_model = model;
		m_models = NULL;
	}
	CPhysics3DModelTask::~CPhysics3DModelTask()
	{
	}
//タスク要素
	BOOL CPhysics3DModelTask::onCreate(){return true;};
	BOOL CPhysics3DModelTask::onUpdate(float step)
	{
		
		//本命モデル
		vector<PhysicsModel*> ColisionModel;

		//モデル選定
		list<PhysicsModel*>::iterator im = m_models->begin();
		WHILE( im != m_models->end() )
		{
			//重複解除
			IF(*im==m_model ){im++;continue;}

			//簡易あたり判定
			IF( CollisionProcessEasy(m_model,*im) )
			{
				ColisionModel.push_back(*im);
			}	
			im++;
		}

		//本命あたり判定
		vector<PhysicsModel*>::iterator ic = ColisionModel.begin();
		WHILE( ic != ColisionModel.end() )
		{

			//詳細な当たり判定
			CollisionProcess3D(m_model,*ic);

			ic++;
		}

		return true;
	};
//タスクの生成
	LPTkyPhysics3DModelTask CPhysics3DModelTask::CreatePhysics3DModelTask( LPPhysicsModel model )
	{
		return new CPhysics3DModelTask(model);
	}

//============================================
//物理世界の情報
//============================================
//コンストラクタ:デストラクタ	
	CPhysicsWorld::CPhysicsWorld()
	{
		m_task = CTkyMultiTaskSystem::CreateTaskSystem();
		InitializeCriticalSection(&m_critical);

	}
	CPhysicsWorld::~CPhysicsWorld()
	{
		m_model.clear();
		DeleteCriticalSection(&m_critical);

	}
//追加
	bool CPhysicsWorld::AddPhysicsModel( PhysicsModel *model)
	{
		IF(model==NULL)return false;
		//モデルをセット
		//タスクの追加(追加できなかったら)

		//物体情報の設定
		LPTkyPhysicsModelTask view = NULL;
		IF(m_task->AddTask( view = CPhysicsModelTask::CreatePhysicsModelTask( model) ) == false)
		{			
			return false;
		}
		//その他物体の設定
		view->SetModelList(&m_model);

		//モデルをセット
		m_model.push_back(model);
		//取得したIDを返す
		return true;
	}
//消去
	bool CPhysicsWorld::DeletePhysicsModel( PhysicsModel *model){

//セクションに入る
EnterCriticalSection(&m_critical);

		list<PhysicsModel*>::iterator ip = m_model.begin();
		WHILE( ip != m_model.end() )
		{
			//モデルだった消す
			IF(*ip == model)
			{
				ip = m_model.erase(ip);
				break;
			}
			ip++;
		}

//出る
LeaveCriticalSection(&m_critical);

Sleep(1);

//セクションにまた入る
EnterCriticalSection(&m_critical);

		list<LPTKYMULTITASK> lm = m_task->GetThread();
		list<LPTKYMULTITASK>::iterator im = lm.begin();
		WHILE( im != lm.end() )
		{
			IF( ((LPTkyPhysicsModelTask)(*im))->GetModel() == model )
			{
				//タスクも消す
				m_task->DestroyTask(*im);
				break;
			}
			im++;
		}

//出る
LeaveCriticalSection(&m_critical);

		return true;
	}
//更新
	bool CPhysicsWorld::Update( float step )
	{
		//自然情報
		list<PhysicsModel*>::iterator im = m_model.begin();
		WHILE( im != m_model.end() )
		{
			//もし存在していなかったら消す
			IF( (*im)->GetVisible() == false )
			{
				im = m_model.erase( im );
				continue;
			}
			/*=================================
			物体の更新
			//=================================*/
			//重力の設定
			(*im)->Impact( Vector( 0,m_Gravity*(*im)->GetWeight(),0 ) );
			//空気抵抗
			(*im)->Impact( (*im)->GetVel().Normal() * -m_AirGap * (*im)->GetWeight());
			//=================================

			//ユーザー更新
			(*im)->onPhysicsUpdate(step);
			//速度更新
			(*im)->Apply(step);
			im++;
		}


		//物理(衝突判定)タスク(衝突判定)
		m_task->Update(step);

		//世界調整
		im = m_model.begin();
		WHILE( im != m_model.end() )
		{

			//座標更新
			(*im)->Update(step);
			(*im)->Adjust(m_range);
			im++;
		}
		return true;
	}
//作成
	LPTkyPhysicsWorld CPhysicsWorld::CreatePhysicsWorld(){return new CPhysicsWorld();}

//============================================
//物理世界の情報3D
//============================================
//コンストラクタ:デストラクタ	
	CPhysicsWorld3D::CPhysicsWorld3D()
	{
		m_task = CTkyMultiTaskSystem::CreateTaskSystem();
	}
	CPhysicsWorld3D::~CPhysicsWorld3D()
	{
		m_model.clear();
	}
//追加(Task葉使用していない)
	bool CPhysicsWorld3D::AddPhysicsModel( PhysicsModel *model)
	{
		IF(model==NULL)return false;
		
		//物体情報の設定
		LPTkyPhysics3DModelTask view = NULL;
		IF(m_task->AddTask( view = CPhysics3DModelTask::CreatePhysics3DModelTask( model) ) == false)
		{			
			return false;
		}
		//その他物体の設定
		view->SetModelList(&m_model);

		//モデルをセット
		m_model.push_back(model);
		//取得したIDを返す
		return true;
	}
	bool CPhysicsWorld3D::AddObtaclePhysicsModel( LPTkyXFile model)
	{
		IF(model==NULL)return false;
		//モデルをセット
		m_obstaclemodel.push_back(model);
		//取得したIDを返す
		return true;
	}
//消去
	bool CPhysicsWorld3D::DeleteObtaclePhysicsModel( LPTkyXFile model)
	{

		list<LPTkyXFile>::iterator ix = m_obstaclemodel.begin();

		WHILE( ix != m_obstaclemodel.end() )
		{
			//モデルだった消す
			IF(*ix == model)
			{
				ix = m_obstaclemodel.erase(ix);
				return true;
			}
			ix++;
		}
		return false;
	}
//今の位置から一番近い障害物までの距離( 負で無限遠 )
	FLOAT CPhysicsWorld3D::GetDistanceObstacle( Vector pos , Vector vec )
	{
		FLOAT out  = -1;;

		list<LPTkyXFile>::iterator ix = m_obstaclemodel.begin();
		WHILE(ix != m_obstaclemodel.end())
		{
			//距離
			float l;
			//あたり判定
			IF( (*ix)->CollisionLay(pos,vec,&l) )
			{
				//あったっていたら距離を縮める
				IF( out < 0 )
				{
					out = l;
				}
				else
				{
					out = min( out , l );
				}
			}
			ix++;
		}
		return out;
	}
//更新
	bool CPhysicsWorld3D::Update( float step )
	{
		//自然情報
		list<PhysicsModel*>::iterator im = m_model.begin();
		WHILE( im != m_model.end() )
		{
			//もし存在していなかったら消す
			IF( (*im)->GetVisible() == false )
			{
				im = m_model.erase( im );
				continue;
			}
			/*=================================
			物体の更新
			//=================================*/
			//重力の設定
			(*im)->Impact( Vector( 0,m_Gravity*(*im)->GetWeight(),0 ) );
			//空気抵抗
			(*im)->Impact( (*im)->GetVel().Normal() * -m_AirGap * (*im)->GetWeight());
			//=================================

			//ユーザー更新
			(*im)->onPhysicsUpdate(step);
			//速度更新
			(*im)->Apply(step);
			im++;
		}

		//物理(衝突判定)タスク(衝突判定)
		m_task->Update(step);

		//世界調整
		im = m_model.begin();
		WHILE( im != m_model.end() )
		{

			//座標更新
			(*im)->Update(step);

			//もし無敵モデルだったら使わない
			IF( (*im)->GetMode() != TEXT("MODEL_INVINCIBLE") )
			{
				list<LPTkyXFile>::iterator ix = m_obstaclemodel.begin();
				WHILE(ix != m_obstaclemodel.end())
				{
					//あたり判定
					CollisionXFile( *im , *ix);
					ix++;
				}
			}

			im++;
		}

		return true;
	}
//作成
	LPTkyPhysicsWorld3D CPhysicsWorld3D::CreatePhysicsWorld3D(){return new CPhysicsWorld3D();}
