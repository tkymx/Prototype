//--------------------------------------------------------------------------------------------------------------
//			座標変換タスクのメインヘッダー
//					座標を登録指定していろいろな動作を行える
//					発生できるようにしている
//--------------------------------------------------------------------------------------------------------------

#include "TKYBASEHEADER.h"
#include "TKYTASK.h"
#include "TKYLIBRARY.h"

#include <assert.h>
#include <vector>
using namespace std;

//-------------------------------------
//座標変換のための情報構造体
//-------------------------------------
struct t_PositionInfo
{
	Vector				m_VectorStart;		//初期座標
	Vector				m_VectorEnd;			//終端座標
	Vector				m_degreeStart;
	Vector				m_degreeEnd;
	DWORD				m_LoopNum;		//移動回数
	DWORD				m_LoopTime;		//単移動時間
};
typedef vector<t_PositionInfo> TPositionInfo ;
//-------------------------------------
//座標変換のための基点クラス(スーパークラス)
//-------------------------------------
class ITkyPositionTask :public ITKYTASK
{
protected:
	Vector *m_Standard;
	TPositionInfo *m_Effect;

	//動的情報
	INT m_NowMainLoop;	//単移動の移動度
	INT m_NowLoop;		//移動種類の遷移
	INT m_NowTime;		//単移動の時間
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
//座標移動タスク
//-------------------------------------
class CTkyTransformationTask :public ITkyPositionTask
{
	int m_movefactor;	//移動遅延
public:
	//コンストラクタ
	CTkyTransformationTask();

	void SetMoveFactor(int);
	void Update();
};

//-------------------------------------
//座標回転タスク
//-------------------------------------
class CTkyRotationTask :public ITkyPositionTask
{
public:
	void Update();
};