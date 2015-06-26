#include "DirectXProjectViews.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//モデルビュー
// 基本的なモデルのビュー
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	IModelView::IModelView()
	{
		m_model = NULL;
		m_PhysicsModel = NULL;
	}
	IModelView::~IModelView()
	{
	}
	//モデルの生成
	BOOL IModelView::pCreateModel( wstring name )
	{
		//Xfileロード
		IF(name == TEXT("") )
		{
			m_model = NULL;
		}
		else
		{
			m_model = CXFile::CreateXFile(Init3D::CreateInit3D());
			IF_FAILED(m_model->SetXFileW(name))
			{
				return false;
			}
		}
		//モデルのロード
		m_PhysicsModel = PhysicsCircleModel::CreatePhysicsCircleModel( 1.0f );
		m_PhysicsModel->SetWeight( 1.0f );

		return true;
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//普通オブジェクトモデル
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CNormalModelView::CNormalModelView(wstring name)
	{
		pCreateModel(name);
	}
	CNormalModelView::~CNormalModelView()
	{
	}
//GameView 要素
	//初期化
	BOOL CNormalModelView::onCreate(){return true;}
	//更新
	BOOL CNormalModelView::onUpdate(float step){return true;}
	//ビュー要素
	//描画
	VOID CNormalModelView::onRender()
	{
		//主人公の動画
		IF(GetXFile()!=NULL)
		{
			static float frame = 0;
			frame = GetXFile()->GetAnime().UpdateFrame( frame ,  0.5f );
			GetXFile()->SetAnimeFreem(frame);
			GetXFile()->DrawMesh(GetPhysicsModel()->GetPos(),Vector());
		}
	}
//イベント要素
	//イベントをもらう
	VOID CNormalModelView::onEvent( TEventMessage msg ){}
//独自要素
	//生成
	LPTkyNormalModelView CNormalModelView::CreateNormalModelView( wstring name )
	{
		return new CNormalModelView(name);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//障害物モデル
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CObstacleModelView::CObstacleModelView(wstring name)
	{
		pCreateModel(name);
	}
	CObstacleModelView::~CObstacleModelView()
	{
	}
//GameView 要素
	//初期化
	BOOL CObstacleModelView::onCreate(){return true;}
	//更新
	BOOL CObstacleModelView::onUpdate(float step){return true;}
	//ビュー要素
	//描画
	VOID CObstacleModelView::onRender()
	{
		//主人公の動画
		IF(GetXFile()!=NULL)GetXFile()->DrawMesh(GetPhysicsModel()->GetPos(),Vector());
	}
//イベント要素
	//イベントをもらう
	VOID CObstacleModelView::onEvent( TEventMessage msg ){}
//独自要素
	//生成
	LPTkyObstacleModelView CObstacleModelView::CreateObstacleModelView( wstring name )
	{
		return new CObstacleModelView(name);
	}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//FPSモデル
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CFPSModelView::CFPSModelView(Vector *pos,Vector *vec)
	{
		m_pos = pos;
		m_Direction = vec;
		m_rotate = Vector();
	}
	CFPSModelView::~CFPSModelView()
	{
	}
//GameView 要素
	//初期化
	BOOL CFPSModelView::onCreate(){return true;}
	//更新
	BOOL CFPSModelView::onUpdate(float step)
	{
		LPPhysicsCircleModel model = GetPhysicsModel();
		IF( model != NULL )
		{
			*m_pos = model->GetPos();
			*m_Direction = Vector(0,0,1).Rotate( Vector() , m_rotate.x , m_rotate.y , m_rotate.z   );
		}
		return true;
	}
	//ビュー要素
	//描画
	VOID CFPSModelView::onRender()
	{
		//文字の描画==============================

		TD3DFONT::CreateFontW(NULL)->SetColor(0,0,0);
		char str[200];
		sprintf_s(str,"距離情報:%lf X:%lf Y:%lf Z:%lf ",1.0f,m_pos->x,m_pos->y,m_pos->z );
		TD3DFONT::CreateFontW(NULL)->DrawFontA(500,150,str);

		//========================================

		//文字の描画==============================

		TD3DFONT::CreateFontW(NULL)->SetColor(0,0,0);
		sprintf_s(str,"マウス定数 X:%d Y:%d Z:%d ",m_mouse.lX,m_mouse.lY,m_mouse.lZ );
		TD3DFONT::CreateFontW(NULL)->DrawFontA(500,50,str);

		//========================================

	}
//イベント要素
	//イベントをもらう
	VOID CFPSModelView::onEvent( TEventMessage msg )
	{
		//モデル情報
		LPPhysicsCircleModel model = this->GetPhysicsModel();

		//移動座標
		Vector move = m_Direction->Normal();
		move.y = 0;
		move = move.Normal();

		string EventName = msg.m_message;
		IF(EventName == "KEY_INFO")
		{
			//キー情報
			CHAR *key = (CHAR*)msg.m_value[0];
			IF( KEYDOWN( key , DIK_Z ) )
			{
				model->Impact( Vector( 0,WorldWolk3D,0 ) );
			}
			IF( KEYDOWN( key , DIK_X ) )
			{
				model->Impact( Vector( 0,-WorldWolk3D,0 ) );
			}
			IF( KEYDOWN( key , DIK_C ) )
			{
//				m_height += -0.01f;

			}
			IF( KEYDOWN( key , DIK_V ) )
			{
//				m_height -= -0.01f;

			}
			IF( KEYDOWN( key , DIK_SPACE ) )
			{
				model->Impact( Vector( 0,WorldJump3D,0 ) );
			}
		}
		EIF(EventName == "KEY_RIGHT")	//右移動
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*-WorldWolk3D );
		}
		EIF(EventName == "KEY_LEFT")	//左移動
		{
			model->Impact( (move*Vector(0,1,0)).Normal()*WorldWolk3D );
		}
		EIF(EventName == "KEY_UP")		//前進
		{
			model->Impact( move*WorldWolk3D );
		}
		EIF(EventName == "KEY_DOWN")	//後退
		{
			model->Impact( move*-WorldWolk3D );
		}
		EIF(EventName == "MOUSE_INFO")
		{

			m_mouse = *( (DIMOUSESTATE*)msg.m_value[0] );

			m_rotate += Vector( (float)m_mouse.lY / 10.0f , (float)m_mouse.lX / 10.0f , 0 ); 

		}
	}
//独自要素
	//生成
	LPTkyFPSModelView CFPSModelView::CreateFPSModelView(Vector *pos,Vector *vec)
	{
		return new CFPSModelView(pos,vec);
	}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ステージ読み込みビュー
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//コンストラクタ・デストラクタ
	CLoadGameView::CLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info)
	{
		m_MainLogick = logic;
		m_info = info;
	}
	CLoadGameView::~CLoadGameView()
	{
	}
//GameView情報
	//初期化
	BOOL CLoadGameView::onCreate()
	{
		//障害物モデルの生成
		IF(!m_MainLogick->AddGameView(CObstacleModelView::CreateObstacleModelView( TEXT("mesh/mesh.x") )))return false;

		//モデルの生成(主人公)
		LPTkyNormalModelView modelview = CNormalModelView::CreateNormalModelView( TEXT("") );
		modelview->SetPos(Vector( -106,3.8f,-45 ));
		IF(!m_MainLogick->AddGameView(modelview))return false;

		//モデルの生成(モブキャラ)
		LPTkyNormalModelView enemyview = CNormalModelView::CreateNormalModelView( TEXT("mesh/chara2_stand.x") );
		enemyview->SetPos(Vector( -106,-3.8f,-35 ));
		IF(!m_MainLogick->AddGameView(enemyview))return false;

		//初期向き
		*m_info->vec = Vector(0,0,1);

		//FPS情報
		LPTkyFPSModelView fpsview = CFPSModelView::CreateFPSModelView( m_info->pos , m_info->vec );
		//モデルのセット
		fpsview->SetPhysicsModel( modelview->GetPhysicsModel() );
		IF(!m_MainLogick->AddGameView(fpsview))return false;


		return true;

	}
	//更新
	BOOL CLoadGameView::onUpdate(float step)
	{
		return true;
	}
//イベント情報
	//イベントをもらう
	VOID CLoadGameView::onEvent( TEventMessage msg )
	{
	}
	//ビュー要素
	//描画
	VOID CLoadGameView::onRender()
	{
	}
//独自情報
	//生成
	LPTkyLoadGameView CLoadGameView::CreateLoadGameView(LPTkyGameLogic logic,LPTkyFPSInfo info)
	{
		LPTkyLoadGameView view = new CLoadGameView(logic,info);
		return view;
	}
