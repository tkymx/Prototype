#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"

#include "TKYXFILE.h"
#include "TkyBaseCollision.h"
#include "TkyEvent.h"
#include "TkyPhysics.h"

#include "DataStructure.h"

//�Փ˂��ׂ����������ꍇ�̓��[�g���グ�Ă�������


//=================���f�����g�p����������ʒu=====================

//�����Ɛ����̂�����ڐG�_
STATIC bool TkyCollisionLineToLine( Vector *out , PhysicsLineModel *v1 , PhysicsLineModel *v2  )
{
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}
	return TkyBaseCollisionLineToLine(out , 
		&CVector(v1->GetShape().m_element[0]+v1->GetPos()	,	v1->GetShape().m_element[1]+v1->GetPos()) , 
		&CVector(v2->GetShape().m_element[0]+v2->GetPos()	,	v2->GetShape().m_element[1]+v2->GetPos())
		);
};

//�~�Ɖ~�̂�����ڐG�_
STATIC bool TkyCollisionCircleToCircle( Vector *out , PhysicsCircleModel *c1 , PhysicsCircleModel *c2  )
{
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}
	return TkyBaseCollisionCircleToCircle(out , 
		&c1->GetPos()		,		c1->GetShape().m_element[0].x , 
		&c2->GetPos()		,		c2->GetShape().m_element[0].x 
		);
}

//���x���l�������~�Ɖ~�̂����蔻��(�Փ˂͂܂����Ă��Ȃ�)
STATIC bool TkyCollisionVelCircleToCircle(  PhysicsCircleModel *c1 , PhysicsCircleModel *c2  )
{
	CVector cv = CVector( c1->GetPos() , c1->GetPos()+c1->GetVel()-c2->GetVel() );
	return TkyBaseCollisionLineToCircle( NULL , &cv , &c2->GetPos() , c1->GetCircle() + c2->GetCircle() );
}
//���x���l�������~�Ɖ~�̂�����(�ڐG�_��t��Ԃ�)(�Փ˂͂܂����Ă��Ȃ�)
STATIC bool TkyCollisionPointVelCircleToCircle( float *t , PhysicsCircleModel *c1 , PhysicsCircleModel *c2  )
{
	//�����ڐG���Ă�����
	IF( TkyCollisionVelCircleToCircle( c1,c2 ) )
	{
		Vector p = c1->GetPos() - c2->GetPos();
		Vector v = c1->GetVel() - c2->GetVel();
		float r = c1->GetCircle()+c2->GetCircle();
		float c = (p^p)-r*r;
		
		//�d�Ȃ��Ă���
		IF( c < 0.0f )
		{
			*t = -1.0f;
			return true;
		}
		float a = v^v;
		float g = std::numeric_limits<float>::epsilon();
		//�ڐG���Ă��Ȃ�
		IF( a < g )
		{
			return false;
		}
		float b = v^p;
		//���݂��߂Â��悤�ɂ͓����Ȃ�
		IF(b>=0.0f)
		{
			return false;
		}
		float d  = b*b - a*c;		
		IF(d<0.0f)
		{
			return false;
		}
		//�ڐG���Ă���
		IF(t!=NULL)
		{
			*t = ( -b + sqrt(d) ) / a;
			//�͈͓�����Ȃ�
			IF(*t<0||*t>1)
			{
				*t = ( -b - sqrt(d) ) / a;
				//����ł��͈͓�����Ȃ�
				IF(*t<0||*t>1)
				{
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

//�����Ɖ~�̂�����ڐG�_(Out�̒l�͐ڐG�_�̎��̍��W)
STATIC bool TkyCollisionLineToCircle( Vector *out , PhysicsLineModel *v1 , PhysicsCircleModel *c1  )
{
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}
	//�����蔻��
	bool h =  TkyBaseCollisionLineToCircle(out , 
		&(v1->GetLine()+v1->GetVel()), 
		&(c1->GetPos()+c1->GetVel())		,		c1->GetCircle() 
		);

	//�����ɐ����Ȉʒu�֔��a�����ړ�����
	Vector v = GetVertical( c1->GetPos() - v1->GetLine().GetVectorStart() , v1->GetLine().GetVector() , v1->GetLine().GetVector() * c1->GetVel() ).Normal() * c1->GetCircle();

	//�����Ƌ��̐ڐG�ʒu
	IF(h==false)
	{
		Vector out2;
		//�Փˎ��̔��a�����͈͂𑝂₷
		Vector r = (c1->GetVel()).Normal() * c1->GetCircle();	
		IF(TkyBaseCollisionLineToLine(
			&out2,
			&(v1->GetLine()+v1->GetVel()), 
			&(CVector( c1->GetPos() - r, c1->GetPos()+c1->GetVel() + r )  )
			)==true)
		{

			h = true;
		}
	}

	//�ڐG�ʒu�̎��̋��̍��W�����߂�
	*out += v;
	
	IF(h==true)
		return true;
	else 
		return false;
}
//�����Ƒ��x�~�̂�����ڐG�_(���x����)
STATIC bool TkyCollisionLineToVelCircle( Vector *out , PhysicsLineModel *v1 , PhysicsCircleModel *c1  )
{
	//�A�E�g�͏Փˌ�̍��W
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}

	//�����������Ă�����
	IF(TkyCollisionLineToCircle(out,v1,c1))
	{
		return true;
	}
	//���x������������x��Ȃ̂Ŕp�~�\��
/*
	//�����蔻��
	IF(TkyBaseCollisionLineToCircleVel(
		out,
		&v1->GetLine(),
		&(CVector( Vector(),c1->GetVel() ) + c1->GetPos() ),
		c1->GetCircle()
		))
	{
		return true;
	}
*/

	return false;	

/*
	//�~�����̐���
	Vector n = GetVertical( c1->GetPos() - v1->GetLine().GetVectorStart() , v1->GetLine().GetVector() , v1->GetLine().GetVector()*c1->GetVel()  );
	n *= c1->GetCircle();

	//���˕Ԃ镪�̌v��
	float s = ( c1->GetVel() * n  ).z / ( v1->GetLine().GetVector() * c1->GetVel() ).z;
	Vector ss = v1->GetLine().GetVector()*s ;
	Vector d =  n + ss;

	//�V��������W�����߂�
	Vector r = v1->GetLine().GetVector().Normal() * c1->GetCircle();
	Vector np2 = v1->GetLine().GetVectorStart() + d - r;
	Vector nv2 = v1->GetLine().GetVector() +  r*2;

	return TkyBaseCollisionLineToLine( out,
		&CVector( c1->GetPos() , c1->GetPos()+c1->GetVel() ),
		&CVector( np2 , np2+nv2 )
		);
		*/
}

//================== ���f���Ԃ̗���鏈�����󂯎��� ======================

//�����Ɛ����̂�����ڐG�_(Leave������)
STATIC bool TkyLeaveCollisionLineToLine( Vector *out , PhysicsLineModel *v1 , PhysicsLineModel *v2  )
{
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}
	bool h = TkyBaseCollisionLineToLine(out , 
		&CVector(v1->GetShape().m_element[0]+v1->GetPos()	,	v1->GetShape().m_element[1]+v1->GetPos()) , 
		&CVector(v2->GetShape().m_element[0]+v2->GetPos()	,	v2->GetShape().m_element[1]+v2->GetPos())
		);
	IF(h)
	{
		v1->SetPos( v1->GetPos() * 2 - *out);
	}
	return h;
};

//�~�Ɖ~�̂�����ڐG�_
STATIC bool TkyLeaveCollisionCircleToCircle( Vector *out , PhysicsCircleModel *c1 , PhysicsCircleModel *c2  )
{
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}
/*	bool h = TkyBaseCollisionCircleToCircle(out , 
		&(c1->GetPos())		,		c1->GetShape().m_element[0].x , 
		&(c2->GetPos())		,		c2->GetShape().m_element[0].x 
		);
*/
	float t;
	bool h = TkyCollisionPointVelCircleToCircle(&t,c1,c2);
	IF(h)
	{
/*		Vector p = c1->GetPos() - c2->GetPos();
		Vector v = c1->GetVel() - c2->GetVel();
		float x = c1->GetCircle() + c2->GetCircle(); 

		//���̌��� -�̉�
		float s = ( -2*(p^v) - sqrtf( 4*(p^v)*(p^v) - 4*(v^v)*( (p^p) - x*x) ) ) / ( 2*(v^v) );
*/
		Vector vel1 = c1->GetVel()*(1.0f-t); 
		Vector vel2 = c2->GetVel()*(1.0f-t);
/*
		//�`�惁�b�Z�[�W
		char str[MAX_PATH];
//		_snprintf_s(str,MAX_PATH," s�̒l:%f v^v:%f p^v:%f p^p:%f \n",s,v^v,p^v,p^p);
		_snprintf_s(str,MAX_PATH," ���������� ");
		TEventMessage msg;
		msg.m_message = "COMMON_LOGIC_STRING";
		msg.m_value.push_back( (DWORD)(str) );
		CEventManager::StaticAddEventMessage( msg );
*/
//		MessageBoxA(NULL,str,"",NULL);

		//�������Ă��Ă��~�̒���������
		IF(t<0)
		{

//�d�����l�������Փ�
			float s = (c1->GetShape().m_element[0].x + c2->GetShape().m_element[0].x) - (c1->GetPos()-c2->GetPos()).GetLarge();

			//�ʂ̕��̂���̗�
	//		Vector power = c2->GetCollisionLeaveVel() + c1->GetCollisionLeaveVel();

			Vector ec1 =  (c1->GetPos()-c2->GetPos()).Normal()*s;// + power ;
			Vector ec2 = (c2->GetPos()-c1->GetPos()).Normal()*s;// + power;

			//�ǂ�������0�̂Ƃ�
			IF(c1->GetWeight() <= 0)vel2 = ec2 ;
			EIF(c2->GetWeight() <= 0)vel1 =ec1;
			else
			{	//�ǂ�����d�ʂ�����Ƃ�
				float w = c1->GetWeight() + c2->GetWeight();
				vel1 = ec1 * (c2->GetWeight()/w ) ;
				vel2 = ec2 * (c1->GetWeight()/w ) ;
			}

/*
			//���̂܂ܗ����
			float s = c1->GetCircle() + c2->GetCircle() - ( c1->GetPos() - c2->GetPos() ).GetLarge();

			vel1 =  (c1->GetPos()-c2->GetPos()).Normal()*s;// + power ;
//			vel2 = (c2->GetPos()-c1->GetPos()).Normal()*s;// + power;
			vel2 = Vector();
*/
		}

//		c1->SetPos( c1->GetPos() + vel1 );
//		c2->SetPos( c2->GetPos() + vel2 );
		c1->AddCollisionLeaveVel(vel1);
		c2->AddCollisionLeaveVel(vel2);
		c1->AddCollisionTime(t);
		c2->AddCollisionTime(t);
		c1->AddCollisionBaseVel(c1->GetBase(c2));
		c2->AddCollisionBaseVel(c2->GetBase(c1));

		c1->AddCollisionIns(t,vel1,c2->GetPos()+vel2,c2);
		c2->AddCollisionIns(t,vel2,c1->GetPos()+vel1,c1);
	}
	return h;
}

//�~�Ɖ~�̂�����ڐG�_
STATIC bool TkyLeaveCollision3DCircleTo3DCircle( Vector *out , PhysicsCircleModel *c1 , PhysicsCircleModel *c2  )
{
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}
	bool h = TkyCollisionCircleToCircle(&o,c1,c2);
	IF(h)
	{
		//���˕Ԃ�x�N�g��
		Vector vel1 = Vector();
		Vector vel2 = Vector();

		//�������Ă��Ă��~�̒���������
		float s = (c1->GetShape().m_element[0].x + c2->GetShape().m_element[0].x) - (c1->GetPos()-c2->GetPos()).GetLarge();

		Vector ec1 =  (c1->GetPos()-c2->GetPos()).Normal()*s;
		Vector ec2 = (c2->GetPos()-c1->GetPos()).Normal()*s;

		//�ǂ�������0�̂Ƃ�
		IF(c1->GetWeight() <= 0)vel2 = ec2 ;
		EIF(c2->GetWeight() <= 0)vel1 =ec1;
		else
		{	//�ǂ�����d�ʂ�����Ƃ�
			float w = c1->GetWeight() + c2->GetWeight();
			vel1 = ec1 * (c2->GetWeight()/w ) ;
			vel2 = ec2 * (c1->GetWeight()/w ) ;
		}

		//�␳���x
		c1->AddCollisionLeaveVel(vel1);
		c2->AddCollisionLeaveVel(vel2);
	}
	return h;
}

//�����Ɖ~�̂�����ڐG�_
STATIC bool TkyLeaveCollisionLineToCircle( Vector *out , PhysicsLineModel *v1 , PhysicsCircleModel *c1  )
{
	Vector o;
	IF(out == NULL)
	{
		out = &o;
	}
	bool h =  TkyCollisionLineToVelCircle(out ,v1,c1 );
	IF(h)
	{
		//���̈ʒu
//		CEventManager::StaticAddEventMessage( "COLLISION_POS" , (DWORD)out );

//		Vector v = *out - c1->GetPos();
//		float s = c1->GetShape().m_element[0].x - v.GetLarge();
//		v1->SetPos( v1->GetPos() + v.Normal() * s ); 

		//�����x�N�g��
		Vector vel1 = Vector();  
		Vector vel2 = Vector();
/*
		Vector newpos = c1->GetPos()+c1->GetVel();

		//���x����
//		float dv = DegradeVector( c1->GetVel() , *out - newpos ).GetLarge();
		float s = c1->GetCircle() - (*out - newpos).GetLarge();// + dv;

		//�ʂ̕��̂���̗�
//		Vector power = v1->GetCollisionLeaveVel() + c1->GetCollisionLeaveVel();


		Vector ev1 = (*out - c1->GetPos()).Normal()*s;// + power ;
		Vector ec1 = (c1->GetPos() - *out).Normal()*s;// + power;

		//�ǂ�������0�̂Ƃ�
		IF(c1->GetWeight() <= 0)	vel1 = ev1;
		EIF(v1->GetWeight() <= 0)	vel2 = ec1;
		else
		{	//�ǂ�����d�ʂ�����Ƃ�
			float w = c1->GetWeight() + v1->GetWeight();
			vel1 = ev1*(c1->GetWeight()/w ) ;
			vel2 = ec1*(v1->GetWeight()/w ) ;
		}
*/

		//�����x�N�g��
		Vector newpos = c1->GetPos()+c1->GetVel();
		vel2 = *out - (newpos);

		float t = (c1->GetPos()-*out).GetLarge() / c1->GetVel().GetLarge();

//		v1->SetPos( v1->GetPos() + vel1 );
//		c1->SetPos( c1->GetPos() + vel2 );
		v1->AddCollisionLeaveVel(vel1);
		c1->AddCollisionLeaveVel(vel2);
		v1->AddCollisionTime(t);
		c1->AddCollisionTime(t);
		v1->AddCollisionBaseVel(v1->GetBase(c1));
		c1->AddCollisionBaseVel(c1->GetBase(v1));

		c1->AddCollisionIns(t,vel1, *out ,v1);
		v1->AddCollisionIns(t,vel2, *out ,c1);
/*
		float s = c1->GetShape().m_element[0].x - (*out - c1->GetPos()).GetLarge();
		//�ǂ�������0�̂Ƃ�
		IF(c1->GetWeight() <= 0)v1->SetPos( v1->GetPos() + (*out-c1->GetPos()).Normal()*s);
		EIF(v1->GetWeight() <= 0)c1->SetPos( c1->GetPos() + (c1->GetPos()-*out).Normal()*s);
		else
		{	//�ǂ�����d�ʂ�����Ƃ�
			float w = c1->GetWeight() + v1->GetWeight();
			c1->SetPos( c1->GetPos() + (c1->GetPos()-*out).Normal()*s* (v1->GetWeight()/w ) );
			v1->SetPos( v1->GetPos() + (*out-c1->GetPos()).Normal()*s* (c1->GetWeight()/w ) );
		}
*/		
	}
	
	return h;
}

//================== �Փ˂̕��@������ ======================

//�����蔻�菈��(�ȒP)
static bool CollisionProcessEasy( PhysicsModel *m1 , PhysicsModel *m2 )
{
	ePhisicsShape s1 = m1->GetShape().m_shape;
	ePhisicsShape s2 = m2->GetShape().m_shape;

	Vector v1=m1->GetPos() + m1->GetVel()/2;
	Vector v2=m2->GetPos() + m2->GetVel()/2;
	float r1=0,r2=0;

	IF( s1 ==  TKY_PHYISICS_SHAPE_LINE && s2 == TKY_PHYISICS_SHAPE_LINE)
	{
		r1 = (m1->GetShape().m_element[0] - v1).GetLarge();
		r2 = (m2->GetShape().m_element[0] - v2).GetLarge();
	}
	EIF( s1 ==  TKY_PHYISICS_SHAPE_CIRCLE && s2 == TKY_PHYISICS_SHAPE_CIRCLE)
	{
		r1 = (m1->GetVel()/2).GetLarge() + (m1->GetShape().m_element[0] ).x;
		r2 = (m2->GetVel()/2).GetLarge() + (m2->GetShape().m_element[0] ).x;
	}
	EIF( s1 ==  TKY_PHYISICS_SHAPE_LINE && s2 == TKY_PHYISICS_SHAPE_CIRCLE)
	{
		r1 = (m1->GetShape().m_element[0] - v1).GetLarge();
		r2 = (m2->GetVel()/2).GetLarge() + (m2->GetShape().m_element[0] ).x;
	}
	EIF( s1 ==  TKY_PHYISICS_SHAPE_CIRCLE && s2 == TKY_PHYISICS_SHAPE_LINE)
	{
		r1 = (m1->GetVel()/2).GetLarge() + (m1->GetShape().m_element[0] ).x;
		r2 = (m2->GetShape().m_element[0] - v2).GetLarge();
	}

	return TkyBaseCollisionCircleToCircle( NULL , &v1 , r1 , &v2 , r2 );
}
//�����蔻�菈��
static bool CollisionProcess( PhysicsModel *m1 , PhysicsModel *m2 )
{
	ePhisicsShape s1 = m1->GetShape().m_shape;
	ePhisicsShape s2 = m2->GetShape().m_shape;

	bool hit = false;
	Vector pos;

	IF( s1 ==  TKY_PHYISICS_SHAPE_CIRCLE && s2 == TKY_PHYISICS_SHAPE_CIRCLE)
	{
/*		PhysicsCircleModel *cm1 = PhysicsCircleModel::CreatePhysicsCircleModel();
		PhysicsCircleModel *cm2 = PhysicsCircleModel::CreatePhysicsCircleModel();
		*cm1 = *((PhysicsCircleModel*)m1);
		*cm2 = *((PhysicsCircleModel*)m2);
*/		hit = TkyLeaveCollisionCircleToCircle(&pos,(PhysicsCircleModel*)m1,(PhysicsCircleModel*)m2);
//		cm2->DestroyOwnObject();
//		cm2->DestroyOwnObject();
	}
	EIF( s1 ==  TKY_PHYISICS_SHAPE_LINE && s2 == TKY_PHYISICS_SHAPE_CIRCLE)
	{
/*		PhysicsLineModel *lm1 = PhysicsLineModel::CreatePhysicsLineModel();
		PhysicsCircleModel *cm2 = PhysicsCircleModel::CreatePhysicsCircleModel();
		*lm1	=	*((PhysicsLineModel*)m1);
		*cm2 = *((PhysicsCircleModel*)m2);
*/		hit = TkyLeaveCollisionLineToCircle(NULL,(PhysicsLineModel*)m1,(PhysicsCircleModel*)m2);
//		lm1->DestroyOwnObject();
//		cm2->DestroyOwnObject();
	}
	EIF( s1 ==  TKY_PHYISICS_SHAPE_CIRCLE && s2 == TKY_PHYISICS_SHAPE_LINE)
	{
/*		PhysicsLineModel *lm2 = PhysicsLineModel::CreatePhysicsLineModel();
		PhysicsCircleModel *cm1 = PhysicsCircleModel::CreatePhysicsCircleModel();
		*lm2	=	*((PhysicsLineModel*)m2);
		*cm1 = *((PhysicsCircleModel*)m1);
		*/		
		hit = TkyLeaveCollisionLineToCircle(NULL,(PhysicsLineModel*)m2,(PhysicsCircleModel*)m1);
//		lm2->DestroyOwnObject();
//		cm1->DestroyOwnObject();
	}

	//�Փ˔���
	IF(hit)
	{
//		m1->Collision(m2);
//		m2->Collision(m1);

		//���f���̒ǉ�
		m1->AddCollisionModel(m2);
		m2->AddCollisionModel(m1);

	}		
	return true;
}

//�����蔻�菈��
static bool CollisionProcess3D( PhysicsModel *m1 , PhysicsModel *m2 )
{
	ePhisicsShape s1 = m1->GetShape().m_shape;
	ePhisicsShape s2 = m2->GetShape().m_shape;

	bool hit = false;
	Vector pos;

	IF( s1 ==  TKY_PHYISICS_SHAPE_CIRCLE && s2 == TKY_PHYISICS_SHAPE_CIRCLE)
	{
		hit = TkyLeaveCollision3DCircleTo3DCircle(&pos,(PhysicsCircleModel*)m1,(PhysicsCircleModel*)m2);
	}
	EIF( s1 ==  TKY_PHYISICS_SHAPE_LINE && s2 == TKY_PHYISICS_SHAPE_CIRCLE)
	{
		MessageBoxW(NULL,TEXT("TkyCollisionLL") , TEXT("CollisionProcess3D�Ή����Ă��Ȃ�") , NULL);
		hit = TkyLeaveCollisionLineToCircle(NULL,(PhysicsLineModel*)m1,(PhysicsCircleModel*)m2);
	}
	EIF( s1 ==  TKY_PHYISICS_SHAPE_CIRCLE && s2 == TKY_PHYISICS_SHAPE_LINE)
	{
		MessageBoxW(NULL,TEXT("TkyCollisionCL") , TEXT("CollisionProcess3D�Ή����Ă��Ȃ�") , NULL);
		hit = TkyLeaveCollisionLineToCircle(NULL,(PhysicsLineModel*)m2,(PhysicsCircleModel*)m1);
	}

	//�Փ˔���
	IF(hit)
	{
		//���f���̒ǉ�
		m1->AddCollisionModel(m2);
		m2->AddCollisionModel(m1);

		//����Ȃ�������
		IF( !m1->onCollisionEvent(m2) )m1->DeleteCollisionInfo();
		IF( !m2->onCollisionEvent(m1) )m2->DeleteCollisionInfo();

	}		
	return hit;
}

//�����蔻��
STATIC BOOL CollisionXFile( PhysicsModel *m1 , LPTkyXFile mesh )
{

	BOOL hit = false;

	//�ۂ�������
	IF( m1->GetShape().m_shape == TKY_PHYISICS_SHAPE_CIRCLE )
	{

		FLOAT Length = 0.0f;

		FLOAT height = m1->GetShape().m_element[0].x;
		//�L�����������炠����͈͂�����ɂ���
		IF( GetModelMessageP( m1 ) == "HERO_CHARACTOR" )
		{
			height = (( LPTkyCharactor )m1->GetCollisionMessage().m_value[0]  )->GetCircle();
		}
		EIF( GetModelMessageP( m1 ) == "ENEMY_CHARACTOR" )
		{
			height = (( LPTkyCharactor )m1->GetCollisionMessage().m_value[0]  )->GetCircle();
		}

		hit = mesh->CollisionLay(
			m1->GetPos() ,
			Vector(0,-1,0),
			&Length
			); 

		//�������Ă�����
		IF(hit)
		{
			//�����ɂ����W���C��
			IF(Length < height)
			{

				m1->SetPos(m1->GetPos() + Vector(0,-1,0)*(Length) + Vector(0,height,0));

				//y���x����
				m1->SetVel( m1->GetVel() - Vector(0,m1->GetVel().y,0) );
			}
		}

	}
	return hit;
}
