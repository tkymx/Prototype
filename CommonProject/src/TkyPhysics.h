#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TKYVECTOR.h"
#include "TkyBaseCollision.h"
#include "TKYTASK.h"
#include "TkyEvent.h"

//���E���x
static const Vector ModelLimitedVelocityAbs = Vector(60,60,0);

//==================== ���̃X�[�p�[�N���X ====================

//���̏��
CLASS_POINTA_OBJECT(Model , LPModel)
{
protected:
	Vector	m_pos;				//���ݒn
	Vector	m_vel;				//���x
	Vector	m_acc;				//�����x
	Vector	m_Force;			//�O��
	float		m_Weight;			//�d��
	float		m_Frictional;		//���C��
	float		m_Elastic;			//������

	bool		m_visible;			//�����Ă��邩

	wstring	m_mode;				//���̂̏��
	TEventMessage m_ColMsg;		//�Փˎ��̃��b�Z�[�W


	//�Փˎ��̃p�����[�^
	struct TCollisionIns
	{
		float		m_CollisionTime;			//�Փ˂�������
		Vector	m_CollisionLeaveVel;	//�Փˌ�͂Ȃꂽ���̑��x
		Vector	m_CollisionOutVel;		//�Փ˕��̐ڐG�n�_
		LPModel m_CollisionModel;

	};
	vector<TCollisionIns> m_collisionIns;	

	Vector	m_CollisionVel;			//�Փˌ㑬�x
	vector<Vector>	m_CollisionBaseVel;		//�Փ˕��̐ڐG�n�_
	vector<Vector>	m_CollisionLeaveVel;	//�Փˌ�͂Ȃꂽ���̑��x
	float		m_CollisionTime;			//�Փ˂�������

	//�Փ˃��f��
	vector<LPModel> m_CollisionModel;

	//�N���e�B�J���Z�N�V����
	CRITICAL_SECTION m_critical;

	//�R���X�g���N�^
	Model(){Initialize(); m_visible=true; };
	Model(Vector pos,float weight){Initialize();m_pos=pos;m_Weight=weight;};
	Model(const Model &model){*this=model;m_Force=Vector();}
	VIRTUAL ~Model()
	{
		//�N���e�B�J���Z�N�V�����̉��
//		DeleteCriticalSection(&m_critical);
	};

public:

	//������
	VOID Initialize(){
		m_pos=Vector();
		m_vel=Vector();
		m_acc=Vector();
		m_CollisionVel=Vector();
		m_CollisionLeaveVel.clear();
		m_CollisionBaseVel.clear();
		m_Weight=m_Frictional = 1.0f;
		m_Elastic= 0.0f ;
		m_CollisionTime = 1.0f;
		m_visible = true;

		m_mode = TEXT("MODEL_NORMAL");
		m_ColMsg.m_message.clear();
		m_ColMsg.m_value.clear();

	//�N���e�B�J���Z�N�V����������
	InitializeCriticalSection(&m_critical);

	};
	//�K�p
	VOID Apply(float time)		
	{
		//�d�����������͈ړ����Ȃ�
		IF(!ISEPSILON(m_Weight , 0))
		{
			//�����x�̍X�V
			m_acc = m_Force/m_Weight * time;
			//���x�̍X�V(�Փ˃x�N�g���̒ǉ�)
			m_vel += (m_acc )*time;
		}

	}
	//���W�X�V
	VOID Update(float time)		//�X�V
	{
		float f = EPSILON;
		//�d�����������͈ړ����Ȃ�
		IF(!ISEPSILON(m_Weight , 0))
		{
			//�Փ˃x�N�g���̒���

			//�ړ��O�̍��W
			Vector m_beforepos = m_pos;
			Vector m_beforevel = m_vel;
			Vector m_beforeacc = m_acc;

	//		m_vel +=  m_CollisionVel;	

			//���x�̍X�V(�Փ˃x�N�g���̒ǉ�)
	//		m_pos +=  m_CollisionLeaveVel;		
			m_vel +=  GetCollisionLeaveVelTotal();	

			IF( m_vel.x > EPSILON )
			{
				m_CollisionLeaveVel.clear();
			}

			//���W�̍X�V
			m_pos += m_vel ;

			//���Ɍ��������˃x�N�g��
			m_vel +=  m_CollisionVel;	

	/*
			//�O�Ɣ�ׂđ��x���O�p��������0�ɂ���
			IF( abs(m_vel.x + m_beforevel.x) < abs( (m_vel*time).x ) )m_vel.x = 0;
	*/

		}

		//�O�͂̏�����
		m_Force = Vector();
		//�Փ˃x�N�g���̏�����
		m_CollisionVel = Vector(); 
		//�Փˌ�x�N�g���̏�����
		m_CollisionLeaveVel.clear();
		//���
		m_CollisionBaseVel.clear();
		//�Փ˃��f���̏���
		m_CollisionModel.clear();
		//�R���V�����Z�b�g
		m_collisionIns.clear();
	}
	//�Ռ�
	VOID Impact(Vector vec)	//�Ռ�
	{
		//�������G��������H���Ȃ�
		IF(m_mode == TEXT("MODEL_INVINCIBLE") )
		{
			return;
		}
		//�O�͂̐ݒ�
		m_Force += vec;

	}
	//�Փ�(��X�p���\��)
	VOID Collision(Model *model)
	{
		//���U�K�p��̑��x
		Vector vel1 = m_vel + this->GetCollisionLeaveVelTotal();
		Vector vel2 = model->GetVel() + model->GetCollisionLeaveVelTotal();

		//�������ƏՓ˂��Ă�������

		//�d���萔---------------------
		IF(m_Weight==0)return;
		//�d���萔
		float w = (1/m_Weight+1.0f/model->GetWeight());
		//err����
		IF(model->GetWeight()==0)w = 1/m_Weight;
		//-------------------------------

		//���ʔ��˂�����B-------------
		Vector n = model->GetSlope(this);	//���˗p�X���[�v
		Vector pos;								//�ڐG���W
		//������y���������߂�
		TkyBaseCollisionPointToLine(&pos,&(vel1),&CVector(Vector(),n));
		Vector vy = vel1 - (pos); //y�ɂ���
		//����̂�y���������߂�
		TkyBaseCollisionPointToLine(&pos,&(vel2),&CVector(Vector(),n)); //x�����߂�
		Vector uy = vel2 - (pos);	//y�ɂ���
		//---------------------------------

		//���̓��m�̌v�Z----------------
//		Vector j = ( power/* - m_vel*/ +  -1*vy )*(m_Elastic*model->GetElastic()+1)/w;
		Vector j = ( uy +  -1*vy )*(m_Elastic+1)/w;
		//�Փˌ�x�N�g���ɒǉ�
		m_CollisionVel += j/m_Weight;
		//---------------------------------
	}
	VOID CollisionRegister()
	{
		const float cc = 0.1f;

		float time = 0;
		vector<TCollisionIns>::iterator im = m_collisionIns.begin();
		WHILE( im != m_collisionIns.end() )
		{
			IF( ISEPSILON(time,0) )
			{
				time  = im->m_CollisionTime;
			}
			else
			{
				//��������
				IF( time - im->m_CollisionTime > cc )
				{
					break;
				}
			}

			//����
			LPModel model = im->m_CollisionModel;

			//�Փˌv�Z
			Collision( model );	

			im++;
		}
	}

	//���f�����w�肷��Ƃ��̈ʒu�ɂ��������X���[�v���擾�ł���
	VIRTUAL Vector GetSlope(Model *model)
	{
		Vector v = model->GetPos() - m_pos;
		return ( m_vel*v*v ).Normal();
	};
	//�����擾
	Vector GetBase(Model *model)
	{
		Vector v = model->GetPos() - m_pos;
		return ( v ).Normal();
	};
	//���W����
	VOID Adjust(RECT rect)
	{

		IF( m_pos.x <= (float)rect.left )	{m_pos.x = (float)rect.left	;m_vel.x=0;};
		IF( m_pos.x >= (float)rect.right )	{m_pos.x = (float)rect.right	;m_vel.x=0;};
		IF( m_pos.y <= (float)rect.top )	{m_pos.y = (float)rect.top		;m_vel.y=0;};
		IF( m_pos.y >= (float)rect.bottom )	{m_pos.y = (float)rect.bottom	;m_vel.y=0;};

		IF( m_vel.x <= -ModelLimitedVelocityAbs.x )m_vel.x = -ModelLimitedVelocityAbs.x;
		IF( m_vel.x >= ModelLimitedVelocityAbs.x )m_vel.x = ModelLimitedVelocityAbs.x;
		IF( m_vel.y <= -ModelLimitedVelocityAbs.y )m_vel.y = -ModelLimitedVelocityAbs.y;
		IF( m_vel.y >= ModelLimitedVelocityAbs.y )m_vel.y = ModelLimitedVelocityAbs.y;

	}
	
	//�擾
	Vector GetPos(){return m_pos;}
	Vector GetVel(){return m_vel;}
	Vector GetAcc(){return m_acc;}
	float GetWeight(){return m_Weight;};
	float GetFrictional(){return m_Frictional;};
	float GetElastic(){return m_Elastic;};
	bool GetVisible(){return m_visible;}
	//MODEL_NORMAL,MODEL_INVINCIBLE
	wstring GetMode(){return m_mode;}
	TEventMessage	GetCollisionMessage(){return m_ColMsg;}
	vector<Vector> GetCollisionLeaveVel(){return m_CollisionLeaveVel;}
	Vector GetCollisionLeaveVelTotal()
	{
		Vector v = Vector();
		vector<Vector>::iterator iv = m_CollisionLeaveVel.begin();
		WHILE( iv != m_CollisionLeaveVel.end() )
		{
			v += *iv;
			iv++;
		}
		return v;
/*		
		IF(m_CollisionLeaveVel.empty())return Vector();
		IF(m_CollisionLeaveVel.size()==1)return m_CollisionLeaveVel[0];

		Vector n0 = GetVertical(Vector(),m_CollisionLeaveVel[0],m_CollisionLeaveVel[0]*m_CollisionLeaveVel[1]);
		Vector n1 = GetVertical(Vector(),m_CollisionLeaveVel[1],m_CollisionLeaveVel[0]*m_CollisionLeaveVel[1]);
		Vector v;
		TkyBaseCollisionLineToLine(&v,
			&(CVector( Vector(),n0 )+m_CollisionLeaveVel[0]),
			&(CVector( Vector(),n1 )+m_CollisionLeaveVel[1])
			);
		return v;
		*/
	}

	//�ݒ�n
	VOID SetPos(Vector vec){m_pos=vec;}
	VOID SetVel(Vector vec){m_vel=vec;}
	VOID SetAcc(Vector vec){m_acc=vec;}
	VOID SetWeight(float c){m_Weight=c;};
	VOID SetFrictional(float c){m_Frictional=c;};
	VOID SetElastic(float c){m_Elastic=c;};
	VOID SetVisible(bool t){m_visible=t;}
	//MODEL_NORMAL,MODEL_INVINCIBLE
	VOID SetMode(wstring m){m_mode = m;}
	VOID SetCollisionMessage(TEventMessage v){m_ColMsg = v;}

	//�ǉ�
	VOID AddCollisionModel(LPModel m){ENTER_SECTION(m_critical , m_CollisionModel.push_back(m);)}
	VOID AddCollisionVel(Vector v){ENTER_SECTION(m_critical , m_CollisionVel+=v;)}
	VOID AddCollisionBaseVel(Vector v){ENTER_SECTION(m_critical , m_CollisionBaseVel.push_back(v);)}
	VOID AddCollisionTime(float t){ENTER_SECTION(m_critical ,  IF(m_CollisionTime > t )m_CollisionTime = t;)}
	VOID AddCollisionLeaveVel(Vector v){ENTER_SECTION(m_critical , m_CollisionLeaveVel.push_back(v);)}
	VOID AddCollisionIns( float t , Vector v , Vector out , LPModel model )
	{
		//�Փˎ���
		t = (out-m_pos).GetLarge();

		TCollisionIns c = {t,v,out,model};
		m_collisionIns.push_back(c);

		//���ԏ��ɂ����������Ԃɑ}��
		vector<TCollisionIns>::iterator ic = m_collisionIns.begin();
		WHILE( ic != m_collisionIns.end() )
		{
			IF( t > ic->m_CollisionTime )
			{
				//�}�����ďI���
				ENTER_SECTION(m_critical , ic = m_collisionIns.insert(ic,c);)
				return;
			}
			ic++;
		}

		//��ԉ����������ԍŌ�ɒǉ�
		ENTER_SECTION(m_critical , m_collisionIns.push_back(c);)
		return;
	}

	//����
	VOID DeleteCollisionInfo()
	{
		ENTER_SECTION( m_critical ,  
			m_CollisionBaseVel.clear();
			m_collisionIns.clear();
			m_CollisionLeaveVel.clear();
			m_CollisionModel.clear();
			m_CollisionVel = Vector();
			m_CollisionTime = NULL;
		)
	}

	//�J��
	VOID Release(){};

	//����
	STATIC LPModel CreateModel(){ return new Model();};
	STATIC LPModel CreateModel(Vector pos,float weight){ return new Model(pos,weight);};
	STATIC LPModel CreateModel(const Model &model){ return new Model(model);}

};

//==================== �������̏ڍ׏�� ====================
//�`����
enum ePhisicsShape{TKY_PHYISICS_SHAPE_NULL,TKY_PHYISICS_SHAPE_LINE,TKY_PHYISICS_SHAPE_CIRCLE};
//�����v����
typedef struct 
{
	//�`����
	ePhisicsShape m_shape;
	//���W��
	vector<Vector> m_element;
	//ID(�o�^���Ɏg��)
//	DWORD m_ID;
}TTkyShape;
//�������̏��(Model�p��)
CLASS_POINTA(PhysicsModel,LPPhysicsModel):public Model
{
protected:
	//�`����
	TTkyShape m_shape;
	//�A�N�Z�X
	bool m_access;

	//�R���X�g���N�^
	PhysicsModel(){m_shape.m_shape = TKY_PHYISICS_SHAPE_NULL;m_access=true;};

public:

	//�ݒ�
	bool SetShape(TTkyShape *s){IF(s==NULL)return false;m_shape = *s;return true;};
	void  SetAccess(bool t){m_access = t;}
	//�擾
	TTkyShape GetShape(){return m_shape;}
	bool  GetAccess(){return m_access;}

	//�C�x���g�֌W
	VIRTUAL bool onCollisionEvent(PhysicsModel *model){	return true; };
	
	//�X�V�C�x���g
	VIRTUAL VOID onPhysicsUpdate(float step)
	{

	}

	//����
	STATIC LPPhysicsModel CreatePhysicsModel(){return new PhysicsModel();}
};

//==================== �������̌`���� ====================

//���`��̕��� ( Vecter[0]:xyz�X�^�[�g�����W Vecter[1]:xyz�G���h�����W  )
CLASS_POINTA( PhysicsLineModel , LPPhysicsLineModel ):public PhysicsModel
{
protected:
	PhysicsLineModel()
	{
	}
	VIRTUAL ~PhysicsLineModel()
	{
	}
public:
	//���`�̍쐬
	bool SetLine(Vector sv,Vector ev)
	{
		m_shape.m_shape = TKY_PHYISICS_SHAPE_LINE;
		m_shape.m_element.push_back( sv );
		m_shape.m_element.push_back( ev );
		return true;
	}
	//���`��̎擾
	bool GetLine(Vector *sv,Vector *ev)
	{
		IF(m_shape.m_shape == TKY_PHYISICS_SHAPE_LINE)
		{
			IF(sv!=NULL)*sv = m_shape.m_element[0];
			IF(ev!=NULL)*ev = m_shape.m_element[1];
		}
		else return false;
		return true;
	}
	CVector GetLine()
	{
		return CVector( m_pos+m_shape.m_element[0] , m_pos+m_shape.m_element[1] );
	}
	//�n���ƕ����x�N�g�����w�肷��Ƃ��̈ʒu�ɂ��������X���[�v���擾�ł���
	VIRTUAL Vector GetSlope(Model *model)
	{
		assert(model);
		return ( m_shape.m_element[1]-m_shape.m_element[0] );
	}
	//������
	Vector GetBase(Model *model)
	{
		Vector v = model->GetPos() - m_pos;
		Vector s =  m_shape.m_element[1]-m_shape.m_element[0];
		Vector b = v*s*s.Normal();

		//�ړ��x�N�g���̂ق���������
		Vector c = m_vel * b;
		IF(c.z >= 0)return b;
		else
		{
			return b*(-1);
		}
	};
	//����
	STATIC LPPhysicsLineModel CreatePhysicsLineModel(){return new PhysicsLineModel();}
	STATIC LPPhysicsLineModel CreatePhysicsLineModel(Vector sv,Vector ev)
	{
		LPPhysicsLineModel m =  new PhysicsLineModel();
		m->SetLine( sv,ev );
		return m;
	}

};
//�ی`��̕��� ( Vecter[1]:x,y,z:���a  )
CLASS_POINTA( PhysicsCircleModel , LPPhysicsCircleModel ):public PhysicsModel
{
protected:
	PhysicsCircleModel()
	{
		m_shape.m_shape = TKY_PHYISICS_SHAPE_CIRCLE;
		m_shape.m_element.resize(1);
	}
	VIRTUAL ~PhysicsCircleModel()
	{
	}
public:
	//�~�`�̍쐬
	bool SetCircle(float r)
	{
		m_shape.m_element[0] = Vector(r,r,r);
		return true;
	}
	//�~�`��̎擾
	bool GetCircle(float* r)
	{
		IF(m_shape.m_shape == TKY_PHYISICS_SHAPE_CIRCLE)
		{
			IF(r!=NULL)*r = m_shape.m_element[0].x;
		}
		else return false;
		return true;
	}
	float GetCircle()
	{
		return m_shape.m_element[0].x;
	}


	//����
	STATIC LPPhysicsCircleModel CreatePhysicsCircleModel(){return new PhysicsCircleModel();}
	STATIC LPPhysicsCircleModel CreatePhysicsCircleModel(float r)
	{
		LPPhysicsCircleModel m =  new PhysicsCircleModel();
		m->SetCircle( r );
		return m;
	}

};

//======================================================


