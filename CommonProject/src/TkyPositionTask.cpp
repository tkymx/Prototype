
#include "TkyPositionTask.h"

//-------------------------------------
//���W�^�X�N�@�X�[�p�[�N���X
//-------------------------------------
bool ITkyPositionTask::Cheak()
{
	//���[�v���ĂȂ�������(�܂胋�[�v���I�������)
	if(ITkyPositionTask::m_NowLoop==0 && ITkyPositionTask::m_NowMainLoop==0)
		return true;
	return false;
}
//-------------------------------------
//���W�ړ��^�X�N
//-------------------------------------
CTkyTransformationTask::CTkyTransformationTask(){m_movefactor = 0;}
void CTkyTransformationTask::SetMoveFactor(int i){m_movefactor = i;};
void CTkyTransformationTask::Update()
{
	IF(CTkyTransformationTask::m_Effect != NULL)
	{
		if( !CTkyTransformationTask::m_Effect->empty() )
		{
			//���Ԃɂ��J�ڂ���
			if( timeGetTime() - m_NowTime >= CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopTime)
			{
				//���W�ړ��̌v�Z
				Vector l_move = 
					CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorStart + 
						(	CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorEnd - 
						CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorStart	) /
						(float)CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopNum * 
						(float)CTkyTransformationTask::m_NowMainLoop;

				//���W�ړ����̃x�N�g���̎擾��
				Vector l_vec = (CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorEnd - 
					CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorStart).Normal();

				//�W���̓K�p
				//�t�@�N�^�[�̐ݒ�
				//�@�W�� - �W�� / ���[�v�� * ���̃��[�v�� �� ���ꂾ���ړ�
				l_move += l_vec * m_movefactor * pow(sin( 3.141519f / (float)CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopNum * (float)CTkyTransformationTask::m_NowMainLoop ),2);

				//�ړ��̓K�p
				*CTkyTransformationTask::m_Standard = l_move;

				//���Ԃ̏�����
				m_NowTime = timeGetTime();

				//�ړ����[�v���I��������
				if( ++CTkyTransformationTask::m_NowMainLoop >= (INT)CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopNum )
				{
					CTkyTransformationTask::m_NowMainLoop = 0;
					//�v���Z�X���[�v���I��������
					if( ++CTkyTransformationTask::m_NowLoop >= (INT)CTkyTransformationTask::m_Effect->size() )
					{
						//���[�v�̏�����
						CTkyTransformationTask::m_NowLoop = 0;
					}
				}
			}
		}
	}
}
//-------------------------------------
//���W��]�^�X�N
//-------------------------------------
void CTkyRotationTask::Update()
{
	if( !CTkyRotationTask::m_Effect->empty() )
	{
		//���Ԃɂ��J�ڂ���
		if( timeGetTime() - m_NowTime >= CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_LoopTime)
		{

			//�ړ��x�N�g���̌v�Z
			Vector l_move = 
				(	CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_degreeEnd - 
				CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_degreeStart	) /
				(float)CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_LoopNum;

			//��]�̓K�p
			CTkyRotationTask::m_Standard->Rotate( 
				CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_VectorStart,
				l_move.x,
				l_move.y,
				l_move.z);

			//���Ԃ̏�����
			m_NowTime = timeGetTime();
			//�ړ����[�v���I��������
			if( ++CTkyRotationTask::m_NowMainLoop >= (INT)CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_LoopNum )
			{
				++CTkyRotationTask::m_NowMainLoop = 0;
				//�v���Z�X���[�v���I��������
				if( ++CTkyRotationTask::m_NowLoop >= (INT)CTkyRotationTask::m_Effect->size() )
				{
					//���[�v�̏�����
					CTkyRotationTask::m_NowLoop = 0;
				}
			}
		}
	}
}