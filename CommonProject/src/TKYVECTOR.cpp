
#include "TKYVECTOR.h"

CVector::CVector()
{
	m_Start = Vector();
	m_End = Vector();
}
CVector::CVector(Vector s,Vector e)
{
	m_Start = s;
	m_End = e;
}
CVector::~CVector()
{
}

//�x�N�g���̗v�f�擾
bool CVector::GetPoint( Vector* onvec, EM_Cvector EM_Cvector, float value)
{
	Vector vec = m_End - m_Start;
	float size=0;
	SWITCH( EM_Cvector )
	{
		case CVECTOR_X:	//�����W�ŃJ�b�g
			//�����ʒu�������番�����Ȃ�
			IF(m_Start.x == value)break;	
			IF(!IsRange<float>( m_Start.x , m_End.x , value ))return false;					
			//�����T�C�Y���擾
			if( m_Start.x < value ) size = (value-m_Start.x)/abs(vec.x);
			if( m_Start.x > value ) size = (m_Start.x-value)/abs(vec.x);
			break;
		case CVECTOR_Y:	//y���W�ŃJ�b�g
			//�����ʒu�������番�����Ȃ�
			IF(m_Start.y == value)break;	
			IF(!IsRange<float>( m_Start.y , m_End.y , value ))return false;					
			//�����T�C�Y���擾
			if( m_Start.y < value ) size = (value-m_Start.y)/abs(vec.y);
			if( m_Start.y > value ) size = (m_Start.y-value)/abs(vec.y);
			break;
		case CVECTOR_Z:	//z���W�ŃJ�b�g
			//�����ʒu�������番�����Ȃ�
			IF(m_Start.z == value)break;	
			IF(!IsRange<float>( m_Start.z , m_End.z , value ))return false;					
			//�����T�C�Y���擾
			if( m_Start.z < value ) size = (value-m_Start.z)/abs(vec.z);
			if( m_Start.z > value ) size = (m_Start.z-value)/abs(vec.z);
			break;
	}

	*onvec = m_Start + vec*size;

	return true;
}

//�x�N�g���̐ؒf
bool CVector::CutVector( LPVECTOR v1, LPVECTOR v2, EM_Cvector EM_Cvector, float value)
{
	//���̍��W���番����Ƃ������
	v1->SetVectorStart(m_Start);
	v2->SetVectorEnd(m_End);

	Vector vec;	//���W�̎擾
	//�ؒf�X�x�N�g���̎擾
	IF(!GetPoint(&vec,EM_Cvector,value))
	{
		return false;
	}
	//�ؒf�_��ݒ�
	v1->SetVectorEnd(vec);
	v2->SetVectorStart(vec);
	return true;
}
//�x�N�g���̏Փ�
bool CVector::CollisionVector( LPVECTOR vec,  Vector* pos)
{
	Vector	s1	= m_Start;
	Vector	v1	= m_End - m_Start;
	Vector	s2	= vec->GetVectorStart();
	Vector	v2	= vec->GetVectorEnd() + vec->GetVectorStart()*(-1);
	Vector	v	= s1-s2;
	Vector	vv	= s2-s1;
	float		t1	= (v*v2).z / (v2*v1).z;	
	float		t2	= (vv*v1).z / (v1*v2).z;	

	IF(pos!=NULL)*pos = s1 + v1*t1;

	IF( t1 < 0.0f || t1 > 1.0f )return false;
	IF( t2 < 0.0f || t2 > 1.0f )return false;

	return true;
}
//�x�N�g���̕`��
bool CVector::DrawLineSet(LPTKY3D l3,Vector color,float thick)
{
	IF(l3==NULL)return false;

	l3->SetColor(color);
	l3->DrawLine( m_Start,m_End,thick );
	return true;
}

