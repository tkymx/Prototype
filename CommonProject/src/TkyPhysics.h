#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TKYVECTOR.h"
#include "TkyBaseCollision.h"
#include "TKYTASK.h"
#include "TkyEvent.h"

//限界速度
static const Vector ModelLimitedVelocityAbs = Vector(60,60,0);

//==================== 物体スーパークラス ====================

//物体情報
CLASS_POINTA_OBJECT(Model , LPModel)
{
protected:
	Vector	m_pos;				//現在地
	Vector	m_vel;				//速度
	Vector	m_acc;				//加速度
	Vector	m_Force;			//外力
	float		m_Weight;			//重さ
	float		m_Frictional;		//摩擦力
	float		m_Elastic;			//反発力

	bool		m_visible;			//見えているか

	wstring	m_mode;				//物体の状態
	TEventMessage m_ColMsg;		//衝突時のメッセージ


	//衝突時のパラメータ
	struct TCollisionIns
	{
		float		m_CollisionTime;			//衝突した時間
		Vector	m_CollisionLeaveVel;	//衝突後はなれた時の速度
		Vector	m_CollisionOutVel;		//衝突物の接触地点
		LPModel m_CollisionModel;

	};
	vector<TCollisionIns> m_collisionIns;	

	Vector	m_CollisionVel;			//衝突後速度
	vector<Vector>	m_CollisionBaseVel;		//衝突物の接触地点
	vector<Vector>	m_CollisionLeaveVel;	//衝突後はなれた時の速度
	float		m_CollisionTime;			//衝突した時間

	//衝突モデル
	vector<LPModel> m_CollisionModel;

	//クリティカルセクション
	CRITICAL_SECTION m_critical;

	//コンストラクタ
	Model(){Initialize(); m_visible=true; };
	Model(Vector pos,float weight){Initialize();m_pos=pos;m_Weight=weight;};
	Model(const Model &model){*this=model;m_Force=Vector();}
	VIRTUAL ~Model()
	{
		//クリティカルセクションの解放
//		DeleteCriticalSection(&m_critical);
	};

public:

	//初期化
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

	//クリティカルセクション初期化
	InitializeCriticalSection(&m_critical);

	};
	//適用
	VOID Apply(float time)		
	{
		//重さが無い時は移動しない
		IF(!ISEPSILON(m_Weight , 0))
		{
			//加速度の更新
			m_acc = m_Force/m_Weight * time;
			//速度の更新(衝突ベクトルの追加)
			m_vel += (m_acc )*time;
		}

	}
	//座標更新
	VOID Update(float time)		//更新
	{
		float f = EPSILON;
		//重さが無い時は移動しない
		IF(!ISEPSILON(m_Weight , 0))
		{
			//衝突ベクトルの訂正

			//移動前の座標
			Vector m_beforepos = m_pos;
			Vector m_beforevel = m_vel;
			Vector m_beforeacc = m_acc;

	//		m_vel +=  m_CollisionVel;	

			//速度の更新(衝突ベクトルの追加)
	//		m_pos +=  m_CollisionLeaveVel;		
			m_vel +=  GetCollisionLeaveVelTotal();	

			IF( m_vel.x > EPSILON )
			{
				m_CollisionLeaveVel.clear();
			}

			//座標の更新
			m_pos += m_vel ;

			//次に向けた反射ベクトル
			m_vel +=  m_CollisionVel;	

	/*
			//前と比べて速度が０継だったら0にする
			IF( abs(m_vel.x + m_beforevel.x) < abs( (m_vel*time).x ) )m_vel.x = 0;
	*/

		}

		//外力の初期化
		m_Force = Vector();
		//衝突ベクトルの初期化
		m_CollisionVel = Vector(); 
		//衝突後ベクトルの初期化
		m_CollisionLeaveVel.clear();
		//基準線
		m_CollisionBaseVel.clear();
		//衝突モデルの消去
		m_CollisionModel.clear();
		//コリションセット
		m_collisionIns.clear();
	}
	//衝撃
	VOID Impact(Vector vec)	//衝撃
	{
		//もし無敵だったら食らわない
		IF(m_mode == TEXT("MODEL_INVINCIBLE") )
		{
			return;
		}
		//外力の設定
		m_Force += vec;

	}
	//衝突(後々継承予定)
	VOID Collision(Model *model)
	{
		//離散適用後の速度
		Vector vel1 = m_vel + this->GetCollisionLeaveVelTotal();
		Vector vel2 = model->GetVel() + model->GetCollisionLeaveVelTotal();

		//○←○と衝突してきた感じ

		//重さ定数---------------------
		IF(m_Weight==0)return;
		//重さ定数
		float w = (1/m_Weight+1.0f/model->GetWeight());
		//err処理
		IF(model->GetWeight()==0)w = 1/m_Weight;
		//-------------------------------

		//平面反射をする。-------------
		Vector n = model->GetSlope(this);	//反射用スロープ
		Vector pos;								//接触座標
		//自分のy成分を求める
		TkyBaseCollisionPointToLine(&pos,&(vel1),&CVector(Vector(),n));
		Vector vy = vel1 - (pos); //yにする
		//相手ののy成分を求める
		TkyBaseCollisionPointToLine(&pos,&(vel2),&CVector(Vector(),n)); //xを求める
		Vector uy = vel2 - (pos);	//yにする
		//---------------------------------

		//物体同士の計算----------------
//		Vector j = ( power/* - m_vel*/ +  -1*vy )*(m_Elastic*model->GetElastic()+1)/w;
		Vector j = ( uy +  -1*vy )*(m_Elastic+1)/w;
		//衝突後ベクトルに追加
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
				//長さ判定
				IF( time - im->m_CollisionTime > cc )
				{
					break;
				}
			}

			//実体
			LPModel model = im->m_CollisionModel;

			//衝突計算
			Collision( model );	

			im++;
		}
	}

	//モデルを指定するとその位置におおじたスロープを取得できる
	VIRTUAL Vector GetSlope(Model *model)
	{
		Vector v = model->GetPos() - m_pos;
		return ( m_vel*v*v ).Normal();
	};
	//垂線取得
	Vector GetBase(Model *model)
	{
		Vector v = model->GetPos() - m_pos;
		return ( v ).Normal();
	};
	//座標調整
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
	
	//取得
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

	//設定系
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

	//追加
	VOID AddCollisionModel(LPModel m){ENTER_SECTION(m_critical , m_CollisionModel.push_back(m);)}
	VOID AddCollisionVel(Vector v){ENTER_SECTION(m_critical , m_CollisionVel+=v;)}
	VOID AddCollisionBaseVel(Vector v){ENTER_SECTION(m_critical , m_CollisionBaseVel.push_back(v);)}
	VOID AddCollisionTime(float t){ENTER_SECTION(m_critical ,  IF(m_CollisionTime > t )m_CollisionTime = t;)}
	VOID AddCollisionLeaveVel(Vector v){ENTER_SECTION(m_critical , m_CollisionLeaveVel.push_back(v);)}
	VOID AddCollisionIns( float t , Vector v , Vector out , LPModel model )
	{
		//衝突時間
		t = (out-m_pos).GetLarge();

		TCollisionIns c = {t,v,out,model};
		m_collisionIns.push_back(c);

		//時間順にあたった時間に挿入
		vector<TCollisionIns>::iterator ic = m_collisionIns.begin();
		WHILE( ic != m_collisionIns.end() )
		{
			IF( t > ic->m_CollisionTime )
			{
				//挿入して終わり
				ENTER_SECTION(m_critical , ic = m_collisionIns.insert(ic,c);)
				return;
			}
			ic++;
		}

		//一番遠かったら一番最後に追加
		ENTER_SECTION(m_critical , m_collisionIns.push_back(c);)
		return;
	}

	//消す
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

	//開放
	VOID Release(){};

	//生成
	STATIC LPModel CreateModel(){ return new Model();};
	STATIC LPModel CreateModel(Vector pos,float weight){ return new Model(pos,weight);};
	STATIC LPModel CreateModel(const Model &model){ return new Model(model);}

};

//==================== 物理物体詳細情報 ====================
//形状情報
enum ePhisicsShape{TKY_PHYISICS_SHAPE_NULL,TKY_PHYISICS_SHAPE_LINE,TKY_PHYISICS_SHAPE_CIRCLE};
//物理計上情報
typedef struct 
{
	//形状情報
	ePhisicsShape m_shape;
	//座標状況
	vector<Vector> m_element;
	//ID(登録時に使う)
//	DWORD m_ID;
}TTkyShape;
//物理物体情報(Model継承)
CLASS_POINTA(PhysicsModel,LPPhysicsModel):public Model
{
protected:
	//形状情報
	TTkyShape m_shape;
	//アクセス
	bool m_access;

	//コンストラクタ
	PhysicsModel(){m_shape.m_shape = TKY_PHYISICS_SHAPE_NULL;m_access=true;};

public:

	//設定
	bool SetShape(TTkyShape *s){IF(s==NULL)return false;m_shape = *s;return true;};
	void  SetAccess(bool t){m_access = t;}
	//取得
	TTkyShape GetShape(){return m_shape;}
	bool  GetAccess(){return m_access;}

	//イベント関係
	VIRTUAL bool onCollisionEvent(PhysicsModel *model){	return true; };
	
	//更新イベント
	VIRTUAL VOID onPhysicsUpdate(float step)
	{

	}

	//生成
	STATIC LPPhysicsModel CreatePhysicsModel(){return new PhysicsModel();}
};

//==================== 物理物体形状情報 ====================

//線形状の物体 ( Vecter[0]:xyzスタート差座標 Vecter[1]:xyzエンド差座標  )
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
	//線形の作成
	bool SetLine(Vector sv,Vector ev)
	{
		m_shape.m_shape = TKY_PHYISICS_SHAPE_LINE;
		m_shape.m_element.push_back( sv );
		m_shape.m_element.push_back( ev );
		return true;
	}
	//線形状の取得
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
	//始動と方向ベクトルを指定するとその位置におおじたスロープを取得できる
	VIRTUAL Vector GetSlope(Model *model)
	{
		assert(model);
		return ( m_shape.m_element[1]-m_shape.m_element[0] );
	}
	//垂直線
	Vector GetBase(Model *model)
	{
		Vector v = model->GetPos() - m_pos;
		Vector s =  m_shape.m_element[1]-m_shape.m_element[0];
		Vector b = v*s*s.Normal();

		//移動ベクトルのほうを向ける
		Vector c = m_vel * b;
		IF(c.z >= 0)return b;
		else
		{
			return b*(-1);
		}
	};
	//生成
	STATIC LPPhysicsLineModel CreatePhysicsLineModel(){return new PhysicsLineModel();}
	STATIC LPPhysicsLineModel CreatePhysicsLineModel(Vector sv,Vector ev)
	{
		LPPhysicsLineModel m =  new PhysicsLineModel();
		m->SetLine( sv,ev );
		return m;
	}

};
//丸形状の物体 ( Vecter[1]:x,y,z:半径  )
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
	//円形の作成
	bool SetCircle(float r)
	{
		m_shape.m_element[0] = Vector(r,r,r);
		return true;
	}
	//円形状の取得
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


	//生成
	STATIC LPPhysicsCircleModel CreatePhysicsCircleModel(){return new PhysicsCircleModel();}
	STATIC LPPhysicsCircleModel CreatePhysicsCircleModel(float r)
	{
		LPPhysicsCircleModel m =  new PhysicsCircleModel();
		m->SetCircle( r );
		return m;
	}

};

//======================================================


