#pragma once

#include "TKYBASEHEADER.h"
#include "TKY3D.h"
#include "TKYLIBRARY.h"

/*
CVector�N���X

Vector�̈ʒu�������Ă���

������g��

�E�x�N�g���̗v�f�擾
bool GetPoint( Vector* , UINT , Vector);
�����@�擾�������W���A�w����A�l

�E�@�����x�N�g���̎擾
bool CutVector( LPVECTOR , LPVECTOR , UINT , float );
�����@������̈�ځA��ځA�������W�A�l
�ԋp�l�@�����������ǂ���
TKYVECTOR_CUT_X,TKYVECTOR_CUT_Y,TKYVECTOR_CUT_Z

�E�@�ڐG�𔻒�
bool CollisionVector( LPVECTOR ,  Vector* )
�����@LPCVECTOR , �ڐG�_�擾�̃x�N�g��
�ԋp�l �����������ǂ���
�����ɂQD�Ŏg����x�N�g���Ԃ̐ڐG����

*/

CLASS(CVector);
TYPEDEF(CVector,*LPVECTOR)

enum EM_Cvector{CVECTOR_X , CVECTOR_Y , CVECTOR_Z };


CLASS(CVector)
{
private:
	Vector m_Start;
	Vector m_End;
public:

	//�R���X�g���N�^
	CVector();
	CVector(Vector,Vector);
	VIRTUAL ~CVector();

	//�x�N�g���̑��
	void	SetVector(Vector s ,Vector e){m_Start = s;m_End = e;};
	void	SetVectorStart(Vector s){m_Start = s;};
	void	SetVectorEnd(Vector e){m_End = e;};
	//�x�N�g���̎擾
	Vector GetVector(){return m_End-m_Start;}
	VOID	  GetVector(Vector* s,Vector* e){*s = m_Start;*e = m_End;}
	Vector GetVectorStart(){return m_Start;};
	Vector GetVectorEnd(){return m_End;};

	//�x�N�g���̗v�f�擾
	bool GetPoint( Vector* , EM_Cvector , float);
	//�x�N�g���̐ؒf
	bool CutVector( LPVECTOR , LPVECTOR , EM_Cvector , float );
	//�x�N�g���̏Փ�
	bool CollisionVector( LPVECTOR ,  Vector* );

	//�x�N�g���̒��Ԏ擾
	Vector GetVectorMiddle(FLOAT t)
	{
		return m_Start + (m_End-m_Start) * t;
	}

	//�`��
	bool DrawLineSet(LPTKY3D l3,Vector color,float thick);

	//���Z�q
	CVector operator+ (const Vector &v){return CVector(m_Start+v,m_End+v);}

};