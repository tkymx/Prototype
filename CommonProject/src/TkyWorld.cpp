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
//�^�X�N�v�f
	BOOL CPhysicsModelTask::onCreate(){return true;};
	BOOL CPhysicsModelTask::onUpdate(float step)
	{

		IF( m_models == NULL )return true;
		
		//�{�����f��
		vector<PhysicsModel*> ColisionModel;

		//���f���I��
		list<PhysicsModel*>::iterator im = m_models->begin();
		WHILE( im != m_models->end() )
		{
			//�d������
			IF(*im==m_model ){im++;continue;}

			//�ȈՂ����蔻��
			IF( CollisionProcessEasy(m_model,*im) )
			{
				ColisionModel.push_back(*im);
			}	
			im++;
		}

		//�{�������蔻��
		vector<PhysicsModel*>::iterator ic = ColisionModel.begin();
		WHILE( ic != ColisionModel.end() )
		{

			IF( CollisionProcess(m_model,*ic) )
			{	//�������Ă�����Փ˃C�x���g��
				m_model->onCollisionEvent(*ic);
			}

			ic++;

		}

		//�Փ˔���
		m_model->CollisionRegister();


		return true;
	};
//�^�X�N�̐���
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
//�^�X�N�v�f
	BOOL CPhysics3DModelTask::onCreate(){return true;};
	BOOL CPhysics3DModelTask::onUpdate(float step)
	{
		
		//�{�����f��
		vector<PhysicsModel*> ColisionModel;

		//���f���I��
		list<PhysicsModel*>::iterator im = m_models->begin();
		WHILE( im != m_models->end() )
		{
			//�d������
			IF(*im==m_model ){im++;continue;}

			//�ȈՂ����蔻��
			IF( CollisionProcessEasy(m_model,*im) )
			{
				ColisionModel.push_back(*im);
			}	
			im++;
		}

		//�{�������蔻��
		vector<PhysicsModel*>::iterator ic = ColisionModel.begin();
		WHILE( ic != ColisionModel.end() )
		{

			//�ڍׂȓ����蔻��
			CollisionProcess3D(m_model,*ic);

			ic++;
		}

		return true;
	};
//�^�X�N�̐���
	LPTkyPhysics3DModelTask CPhysics3DModelTask::CreatePhysics3DModelTask( LPPhysicsModel model )
	{
		return new CPhysics3DModelTask(model);
	}

//============================================
//�������E�̏��
//============================================
//�R���X�g���N�^:�f�X�g���N�^	
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
//�ǉ�
	bool CPhysicsWorld::AddPhysicsModel( PhysicsModel *model)
	{
		IF(model==NULL)return false;
		//���f�����Z�b�g
		//�^�X�N�̒ǉ�(�ǉ��ł��Ȃ�������)

		//���̏��̐ݒ�
		LPTkyPhysicsModelTask view = NULL;
		IF(m_task->AddTask( view = CPhysicsModelTask::CreatePhysicsModelTask( model) ) == false)
		{			
			return false;
		}
		//���̑����̂̐ݒ�
		view->SetModelList(&m_model);

		//���f�����Z�b�g
		m_model.push_back(model);
		//�擾����ID��Ԃ�
		return true;
	}
//����
	bool CPhysicsWorld::DeletePhysicsModel( PhysicsModel *model){

//�Z�N�V�����ɓ���
EnterCriticalSection(&m_critical);

		list<PhysicsModel*>::iterator ip = m_model.begin();
		WHILE( ip != m_model.end() )
		{
			//���f������������
			IF(*ip == model)
			{
				ip = m_model.erase(ip);
				break;
			}
			ip++;
		}

//�o��
LeaveCriticalSection(&m_critical);

Sleep(1);

//�Z�N�V�����ɂ܂�����
EnterCriticalSection(&m_critical);

		list<LPTKYMULTITASK> lm = m_task->GetThread();
		list<LPTKYMULTITASK>::iterator im = lm.begin();
		WHILE( im != lm.end() )
		{
			IF( ((LPTkyPhysicsModelTask)(*im))->GetModel() == model )
			{
				//�^�X�N������
				m_task->DestroyTask(*im);
				break;
			}
			im++;
		}

//�o��
LeaveCriticalSection(&m_critical);

		return true;
	}
//�X�V
	bool CPhysicsWorld::Update( float step )
	{
		//���R���
		list<PhysicsModel*>::iterator im = m_model.begin();
		WHILE( im != m_model.end() )
		{
			//�������݂��Ă��Ȃ����������
			IF( (*im)->GetVisible() == false )
			{
				im = m_model.erase( im );
				continue;
			}
			/*=================================
			���̂̍X�V
			//=================================*/
			//�d�͂̐ݒ�
			(*im)->Impact( Vector( 0,m_Gravity*(*im)->GetWeight(),0 ) );
			//��C��R
			(*im)->Impact( (*im)->GetVel().Normal() * -m_AirGap * (*im)->GetWeight());
			//=================================

			//���[�U�[�X�V
			(*im)->onPhysicsUpdate(step);
			//���x�X�V
			(*im)->Apply(step);
			im++;
		}


		//����(�Փ˔���)�^�X�N(�Փ˔���)
		m_task->Update(step);

		//���E����
		im = m_model.begin();
		WHILE( im != m_model.end() )
		{

			//���W�X�V
			(*im)->Update(step);
			(*im)->Adjust(m_range);
			im++;
		}
		return true;
	}
//�쐬
	LPTkyPhysicsWorld CPhysicsWorld::CreatePhysicsWorld(){return new CPhysicsWorld();}

//============================================
//�������E�̏��3D
//============================================
//�R���X�g���N�^:�f�X�g���N�^	
	CPhysicsWorld3D::CPhysicsWorld3D()
	{
		m_task = CTkyMultiTaskSystem::CreateTaskSystem();
	}
	CPhysicsWorld3D::~CPhysicsWorld3D()
	{
		m_model.clear();
	}
//�ǉ�(Task�t�g�p���Ă��Ȃ�)
	bool CPhysicsWorld3D::AddPhysicsModel( PhysicsModel *model)
	{
		IF(model==NULL)return false;
		
		//���̏��̐ݒ�
		LPTkyPhysics3DModelTask view = NULL;
		IF(m_task->AddTask( view = CPhysics3DModelTask::CreatePhysics3DModelTask( model) ) == false)
		{			
			return false;
		}
		//���̑����̂̐ݒ�
		view->SetModelList(&m_model);

		//���f�����Z�b�g
		m_model.push_back(model);
		//�擾����ID��Ԃ�
		return true;
	}
	bool CPhysicsWorld3D::AddObtaclePhysicsModel( LPTkyXFile model)
	{
		IF(model==NULL)return false;
		//���f�����Z�b�g
		m_obstaclemodel.push_back(model);
		//�擾����ID��Ԃ�
		return true;
	}
//����
	bool CPhysicsWorld3D::DeleteObtaclePhysicsModel( LPTkyXFile model)
	{

		list<LPTkyXFile>::iterator ix = m_obstaclemodel.begin();

		WHILE( ix != m_obstaclemodel.end() )
		{
			//���f������������
			IF(*ix == model)
			{
				ix = m_obstaclemodel.erase(ix);
				return true;
			}
			ix++;
		}
		return false;
	}
//���̈ʒu�����ԋ߂���Q���܂ł̋���( ���Ŗ����� )
	FLOAT CPhysicsWorld3D::GetDistanceObstacle( Vector pos , Vector vec )
	{
		FLOAT out  = -1;;

		list<LPTkyXFile>::iterator ix = m_obstaclemodel.begin();
		WHILE(ix != m_obstaclemodel.end())
		{
			//����
			float l;
			//�����蔻��
			IF( (*ix)->CollisionLay(pos,vec,&l) )
			{
				//���������Ă����狗�����k�߂�
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
//�X�V
	bool CPhysicsWorld3D::Update( float step )
	{
		//���R���
		list<PhysicsModel*>::iterator im = m_model.begin();
		WHILE( im != m_model.end() )
		{
			//�������݂��Ă��Ȃ����������
			IF( (*im)->GetVisible() == false )
			{
				im = m_model.erase( im );
				continue;
			}
			/*=================================
			���̂̍X�V
			//=================================*/
			//�d�͂̐ݒ�
			(*im)->Impact( Vector( 0,m_Gravity*(*im)->GetWeight(),0 ) );
			//��C��R
			(*im)->Impact( (*im)->GetVel().Normal() * -m_AirGap * (*im)->GetWeight());
			//=================================

			//���[�U�[�X�V
			(*im)->onPhysicsUpdate(step);
			//���x�X�V
			(*im)->Apply(step);
			im++;
		}

		//����(�Փ˔���)�^�X�N(�Փ˔���)
		m_task->Update(step);

		//���E����
		im = m_model.begin();
		WHILE( im != m_model.end() )
		{

			//���W�X�V
			(*im)->Update(step);

			//�������G���f����������g��Ȃ�
			IF( (*im)->GetMode() != TEXT("MODEL_INVINCIBLE") )
			{
				list<LPTkyXFile>::iterator ix = m_obstaclemodel.begin();
				WHILE(ix != m_obstaclemodel.end())
				{
					//�����蔻��
					CollisionXFile( *im , *ix);
					ix++;
				}
			}

			im++;
		}

		return true;
	}
//�쐬
	LPTkyPhysicsWorld3D CPhysicsWorld3D::CreatePhysicsWorld3D(){return new CPhysicsWorld3D();}
