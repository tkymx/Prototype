
#include "TkyPositionTask.h"

//-------------------------------------
//座標タスク　スーパークラス
//-------------------------------------
bool ITkyPositionTask::Cheak()
{
	//ループしてなかったら(つまりループが終わったら)
	if(ITkyPositionTask::m_NowLoop==0 && ITkyPositionTask::m_NowMainLoop==0)
		return true;
	return false;
}
//-------------------------------------
//座標移動タスク
//-------------------------------------
CTkyTransformationTask::CTkyTransformationTask(){m_movefactor = 0;}
void CTkyTransformationTask::SetMoveFactor(int i){m_movefactor = i;};
void CTkyTransformationTask::Update()
{
	IF(CTkyTransformationTask::m_Effect != NULL)
	{
		if( !CTkyTransformationTask::m_Effect->empty() )
		{
			//時間により遷移する
			if( timeGetTime() - m_NowTime >= CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopTime)
			{
				//座標移動の計算
				Vector l_move = 
					CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorStart + 
						(	CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorEnd - 
						CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorStart	) /
						(float)CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopNum * 
						(float)CTkyTransformationTask::m_NowMainLoop;

				//座標移動時のベクトルの取得委
				Vector l_vec = (CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorEnd - 
					CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_VectorStart).Normal();

				//係数の適用
				//ファクターの設定
				//　係数 - 係数 / ループ回数 * 今のループ回数 ＝ それだけ移動
				l_move += l_vec * m_movefactor * pow(sin( 3.141519f / (float)CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopNum * (float)CTkyTransformationTask::m_NowMainLoop ),2);

				//移動の適用
				*CTkyTransformationTask::m_Standard = l_move;

				//時間の初期化
				m_NowTime = timeGetTime();

				//移動ループが終了したら
				if( ++CTkyTransformationTask::m_NowMainLoop >= (INT)CTkyTransformationTask::m_Effect->at(CTkyTransformationTask::m_NowLoop).m_LoopNum )
				{
					CTkyTransformationTask::m_NowMainLoop = 0;
					//プロセスループが終了したら
					if( ++CTkyTransformationTask::m_NowLoop >= (INT)CTkyTransformationTask::m_Effect->size() )
					{
						//ループの初期化
						CTkyTransformationTask::m_NowLoop = 0;
					}
				}
			}
		}
	}
}
//-------------------------------------
//座標回転タスク
//-------------------------------------
void CTkyRotationTask::Update()
{
	if( !CTkyRotationTask::m_Effect->empty() )
	{
		//時間により遷移する
		if( timeGetTime() - m_NowTime >= CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_LoopTime)
		{

			//移動ベクトルの計算
			Vector l_move = 
				(	CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_degreeEnd - 
				CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_degreeStart	) /
				(float)CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_LoopNum;

			//回転の適用
			CTkyRotationTask::m_Standard->Rotate( 
				CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_VectorStart,
				l_move.x,
				l_move.y,
				l_move.z);

			//時間の初期化
			m_NowTime = timeGetTime();
			//移動ループが終了したら
			if( ++CTkyRotationTask::m_NowMainLoop >= (INT)CTkyRotationTask::m_Effect->at(CTkyRotationTask::m_NowLoop).m_LoopNum )
			{
				++CTkyRotationTask::m_NowMainLoop = 0;
				//プロセスループが終了したら
				if( ++CTkyRotationTask::m_NowLoop >= (INT)CTkyRotationTask::m_Effect->size() )
				{
					//ループの初期化
					CTkyRotationTask::m_NowLoop = 0;
				}
			}
		}
	}
}