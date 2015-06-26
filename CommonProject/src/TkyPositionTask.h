//--------------------------------------------------------------------------------------------------------------
//			���W�ϊ��^�X�N�̃��C���w�b�_�[
//					���W��o�^�w�肵�Ă��낢��ȓ�����s����
//					�����ł���悤�ɂ��Ă���
//--------------------------------------------------------------------------------------------------------------

#include "TKYBASEHEADER.h"
#include "TKYTASK.h"
#include "TKYLIBRARY.h"

#include <assert.h>
#include <vector>
using namespace std;

//-------------------------------------
//���W�ϊ��̂��߂̏��\����
//-------------------------------------
struct t_PositionInfo
{
	Vector				m_VectorStart;		//�������W
	Vector				m_VectorEnd;			//�I�[���W
	Vector				m_degreeStart;
	Vector				m_degreeEnd;
	DWORD				m_LoopNum;		//�ړ���
	DWORD				m_LoopTime;		//�P�ړ�����
};
typedef vector<t_PositionInfo> TPositionInfo ;
//-------------------------------------
//���W�ϊ��̂��߂̊�_�N���X(�X�[�p�[�N���X)
//-------------------------------------
class ITkyPositionTask :public ITKYTASK
{
protected:
	Vector *m_Standard;
	TPositionInfo *m_Effect;

	//���I���
	INT m_NowMainLoop;	//�P�ړ��̈ړ��x
	INT m_NowLoop;		//�ړ���ނ̑J��
	INT m_NowTime;		//�P�ړ��̎���
public:
	void SetVector(Vector* v){
		assert(v);
		m_Standard = v;
		m_NowMainLoop = 0;
		m_NowLoop = 0;
		m_NowTime = 0;
	}
	void SetEffect(TPositionInfo *p){
		assert(p);
		m_Effect = p;
		m_NowMainLoop = 0;
		m_NowLoop = 0;
		m_NowTime = 0;
	}
	bool Cheak();
};

//-------------------------------------
//���W�ړ��^�X�N
//-------------------------------------
class CTkyTransformationTask :public ITkyPositionTask
{
	int m_movefactor;	//�ړ��x��
public:
	//�R���X�g���N�^
	CTkyTransformationTask();

	void SetMoveFactor(int);
	void Update();
};

//-------------------------------------
//���W��]�^�X�N
//-------------------------------------
class CTkyRotationTask :public ITkyPositionTask
{
public:
	void Update();
};