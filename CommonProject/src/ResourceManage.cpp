#include "ResourceManage.h"

//////////////////////////////////////リソース管理クラス//////////////////////////////////////

//-----------------------------
//リソース管理クラス
//-----------------------------
/*	//Xファイル情報
	CStringMatchW<LPTkyXFile> m_chara;
	//画像ファイル情報
	CStringMatchW<LPTkyTexture> m_chara;
*/
	//コンストラクタ、デストラクタ
	CResourceManage::CResourceManage()
	{
		m_x.Destroy();
		m_tex.Destroy();
	}
	CResourceManage::CResourceManage(const CResourceManage& r)
	{
		*this = r;
	}
	CResourceManage::~CResourceManage()
	{
		//この関数ではメモリは開放されない
		m_x.Destroy();
		m_tex.Destroy();
	}

	//追加
	VOID CResourceManage::AddXFie(LPTkyXFile x,wstring name)
	{
		m_x.AddString( name , x );
	}
	VOID CResourceManage::AddTexture(LPTkyTexture x,wstring name)
	{
		m_tex.AddString( name , x );
	}
	//消去
	VOID CResourceManage::DeleteXFie(wstring name)
	{
		m_x.Destroy(name);
	}
	VOID CResourceManage::DeleteTexture(wstring name)
	{
		m_tex.Destroy(name);
	}
	//取得
	LPTkyXFile		CResourceManage::GetXFie(wstring name)
	{
		LPTkyXFile *x = NULL;
		IF(m_x.GetElement(name , &x ))
		{
			return *x;
		}
		return NULL;
	}
	LPTkyTexture	CResourceManage::GetTexture(wstring name)
	{
		LPTkyTexture *t = NULL;
		IF( m_tex.GetElement(name , &t ) )
		{
			return *t;
		}
		return NULL;
	}

	//ファイルから取得(リソースを見ながら)
	VOID CResourceManage::LoadResource(wstring name )
	{

		CTkyFileRead read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
		}

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("XFILE") )
			{
				wstring name = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring element = read.GetToElementFromElement(TEXT('<'),TEXT('>'));
				wstring speed = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				float sp=0 ;	//スピード
				swscanf_s( speed.c_str() , TEXT("%f"),&sp );

				LPTkyXFile x =CXFile::CreateXFile( Init3D::CreateInit3D() , element );
				x->GetAnime()->SetAnimeSpeed(sp);

				//ファイルの追加
				m_x.AddString( name ,  x );
			}

			EIF( key == TEXT("TEXTURE") )
			{
				wstring name = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring tex = read.GetToElementFromElement(TEXT('<'),TEXT(','));
				wstring element = read.GetToElement(TEXT('>'));
				INT n=0 , ho=0 , ver=0 , w=0 , h=0 , s=0 ;	//個数、水平個数、垂直個数、幅、高さ、スピード
				swscanf_s( element.c_str() , TEXT("%d,%d,%d,%d,%d,%d"),&n,&ho,&ver,&w,&h,&s );
				//ファイルの追加
				m_tex.AddString( name , CTexture::CreateTextureW( Init3D::CreateInit3D() , tex , n , ho, ver ,w , h , (DWORD)s )  );
			}

		}

		//開放
		read.Close();

	}


//////////////////////////////////////キャラクタ管理クラス//////////////////////////////////////
/*
	//キャラ情報
	CStringMatchW<LPTkyCharactor> m_chara;
*/
	//コンストラクタ、デストラクタ
	CCharaManage::CCharaManage()
	{
		m_chara.Destroy();
	}
	CCharaManage::CCharaManage(const CCharaManage& c)
	{
		*this = c;
	}
	CCharaManage::~CCharaManage()
	{
		m_chara.Destroy();
	}

	//追加
	VOID CCharaManage::AddCharactor(TCharaSet chara)
	{
		//キャラの名前で追加
		m_chara.AddString( chara.m_chara->GetCharaInfo().m_name , chara );
	}
	//消去
	VOID CCharaManage::DeleteCharactor(TCharaSet chara)
	{
		//キャラの名前で消去
		m_chara.Destroy( chara.m_chara->GetCharaInfo().m_name );
	}
	VOID CCharaManage::DeleteCharactor(wstring name)
	{
		//キャラの名前で消去
		m_chara.Destroy( name );
	}
	//取得
	TCharaSet CCharaManage::Getchara(wstring name)
	{
		TCharaSet *c = NULL;
		IF( m_chara.GetElement( name , &c ) )
		{
			return *c;
		}
		return TCharaSet();
	}
	VOID CCharaManage::LoadCharactor(wstring name , CResourceManage* resource)
	{
		CTkyFileRead read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
		}

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("CHARACTOR") )
			{
				//情報構造体
				TGameTextInfo CharaText;

				//基本情報
				wstring CharaName = read.GetToElementFromElement(TEXT('['),TEXT(','));
				wstring name = read.GetToElement(TEXT(']'));
				wstring AIName = read.GetToElementFromElement(TEXT('('),TEXT(')'));

				int HitPoint = NULL , Velocity = NULL;
				float Length = NULL;
				swscanf_s( name.c_str() , TEXT("%d,%d,%f"),&HitPoint,&Velocity,&Length );
				TCharaInfo chara(CharaName,HitPoint,Velocity,Length,AIName);
				CharaText.push_back( TGameText( TEXT("CHARACTOR_SET_BASEINFO") , TEXT("") , (DWORD)( &chara) ) );

				//武器情報
				wstring bullet = read.GetToElementFromElement(TEXT('<'),TEXT('>'));
				float FlyWait = NULL,AttackRange = NULL,AttackSpeed = NULL,AttackLength = NULL;
				float AttackPower = NULL;
				swscanf_s( bullet.c_str() , TEXT("%f,%f,%f,%f,%f"),&FlyWait,&AttackPower,&AttackRange,&AttackSpeed,&AttackLength );
				CharaText.push_back( TGameText( TEXT("WEAPON_SET_BASEINFO") , TEXT("") , (DWORD)( &TWeaponInfo( FlyWait  ) ) ) );
				CharaText.push_back( TGameText( TEXT("ATTACK_SET_BASEINFO") , TEXT("") , (DWORD)( &TBulletInfo((int)AttackPower,AttackRange,AttackSpeed,AttackLength) ) ) );

				//アニメーションエフェクト情報
				WHILE(!read.IsEof())
				{
					wstring value = read.GetToElementFromElement(TEXT('{'),TEXT('}'));
					IF( value == TEXT("END") )break;

					//ここを何とかする必要がある
					wchar_t *ValueName,*ElementName,*ResourceName,*Next;
					ValueName = wcstok_s( const_cast<wchar_t*>(value.c_str())  , TEXT(",") , &Next  );
					ElementName = wcstok_s( NULL , TEXT(",") , &Next  );
					ResourceName = wcstok_s( NULL , TEXT(",") , &Next  );


					//アニメーションだったら
					IF(	_wcsicmp( ValueName , TEXT("CHARACTOR_SET_ANIMATION") ) == 0 || 
							_wcsicmp( ValueName , TEXT("WEAPON_SET_ANIMATION") ) == 0)
					{
						IF( _wcsicmp( ResourceName  , TEXT("NULL") ) == 0  )
						{
							CharaText.push_back( TGameText( ValueName , ElementName , (DWORD)NULL ) );
						}
						else
						{
							CharaText.push_back( TGameText( ValueName , ElementName , (DWORD)resource->GetXFie(ResourceName) ) );
						}
					}
					//エフェクトだったら
					EIF(	_wcsicmp( ValueName , TEXT("CHARACTOR_SET_EFFECT") ) == 0 ||
						_wcsicmp( ValueName , TEXT("WEAPON_SET_EFFECT") ) == 0 	)
					{
						IF( _wcsicmp( ResourceName  , TEXT("NULL") ) == 0  )
						{
							CharaText.push_back( TGameText( ValueName , ElementName , (DWORD)NULL ) );
						}
						else
						{
							CharaText.push_back( TGameText( ValueName , ElementName ,  (DWORD)(resource->GetTexture(ResourceName))  ) );
						}
					}
				}

				//主人公情報の設定
				TCharaSet charaset;
				charaset.NewAllocate();
				charaset.m_chara->onLoadGameDate(CharaText);
				charaset.m_weapon->onLoadGameDate(CharaText);
				m_chara.AddString( CharaName , charaset );
			}

		}

		//開放
		read.Close();
	}



//////////////////////////////////////テキスト管理クラス//////////////////////////////////////

/*
	//キャラ情報
	CNumberMatch<TTextSet> m_Text;
*/

	//コンストラクタ、デストラクタ
	CTextManage::CTextManage()
	{
		m_Text.Destroy();
	}
	CTextManage::CTextManage(const CTextManage& t)
	{
		*this = t;
	}
	CTextManage::~CTextManage()
	{
		m_Text.Destroy();
		
		map<int,TTextWindowInfo* > m  = textwindow.GetElement();
		map<int,TTextWindowInfo* >::iterator im = m.begin();
		WHILE( im != m.end())
		{
			SAFE_DELETE( im->second );
			im++;
		}
	}

	//追加
	VOID CTextManage::AddText(INT n,TTextSet text)
	{
		m_Text.AddString(n,text);
	}
	//消去
	VOID CTextManage::DeleteText(INT n)
	{
		m_Text.Destroy(n);
	}
	//取得
	TTextSet CTextManage::GetText(INT n)
	{
		TTextSet *t = NULL;
		IF( m_Text.GetElement( n , &t ) )
		{
			return *t;
		}
		return TTextSet();
	}
	vector<TTextSet*> CTextManage::GetText(INT sn ,INT en)
	{
		return m_Text.GetElement( sn , en );
	}

	//ファイルから取得
	VOID CTextManage::LoadText(wstring name, CResourceManage* resource)
	{

		CTkyFileReadCW read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
		}

		//今のテキスト番号
		UINT NowTextNumber = NULL;

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();


			IF( key == TEXT("WINDOW") )
			{
				TTextWindowInfo *window = new TTextWindowInfo();

				wstring windownumber  = read.GetToElementFromElement(TEXT('['),TEXT(','));
				//テクスチャ画像名前
				wstring wname  = read.GetToElement(TEXT(','));
				wstring wposstart  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wposend  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wrotatestart  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wrotateposend  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wexpandstart  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wexpandend  = read.GetToElementFromElement(TEXT('('),TEXT(')'));

				//画像ナンバー
				INT number = NULL;
				swscanf_s( windownumber.c_str(), TEXT("%d"),&number );

				//移動ベクター
				Vector sp,sr,se,ep,er,ee;
				swscanf_s( wposstart.c_str()		, TEXT("%f,%f,%f"),&sp.x,&sp.y,&sp.z );
				swscanf_s( wposend.c_str()			, TEXT("%f,%f,%f"),&ep.x,&ep.y,&ep.z );
				swscanf_s( wrotatestart.c_str()		, TEXT("%f,%f,%f"),&sr.x,&sr.y,&sr.z );
				swscanf_s( wrotateposend.c_str()	, TEXT("%f,%f,%f"),&er.x,&er.y,&er.z );
				swscanf_s( wexpandstart.c_str()		, TEXT("%f,%f,%f"),&se.x,&se.y,&se.z );
				swscanf_s( wexpandend.c_str()		, TEXT("%f,%f,%f"),&ee.x,&ee.y,&ee.z );

				//要素の追加
				window->m_textpos = Vector(0,0,0);
				window->m_WindowTexture = resource->GetTexture(wname);
				window->m_WindowPosEndStart = CVector( sp , ep );
				window->m_WindowRotateEndStart = CVector( sr , er );
				window->m_WindowExpandEndStart = CVector( se , ee );

				//立ち絵の設定
				WHILE(!read.IsEof())
				{
					//キャラ画像
					wstring charaname = read.GetToElementFromElement(TEXT('<'),TEXT('>'));
					IF( charaname == TEXT("END") )break;

					//位置
					wstring charapos = read.GetToElementFromElement(TEXT('('),TEXT(')'));
					Vector pos;
					swscanf_s( charapos.c_str()		, TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );

					window->m_BackChara.push_back( TTextWindowCharaInfo( resource->GetTexture(charaname) , pos )  );

				}

				textwindow.AddString( number , window );

			}

			IF( key == TEXT("TEXT") )
			{
				TTextInfo textinfo;
				RECT r = { 0,0,590,100 };
				textinfo.m_Render = r;

				wstring info  = read.GetToElementFromElement(TEXT('['),TEXT(']'));

				//ウィンドウナンバー、テキストナンバー、テキストスピード
				FLOAT WindowNumber=NULL , TextNumber=NULL , TextSpeed=NULL;
				swscanf_s( info.c_str()		, TEXT("%f,%f,%f"),&TextNumber,&WindowNumber,&TextSpeed );
				
				//テキストの速さ
				textinfo.m_ShowSpeed = TextSpeed;

				locale::global(locale("japanese"));

				//ヘッド情報
				textinfo.m_name = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				//テキストの中身
				textinfo.m_string  = read.GetToElementFromElement(TEXT('"'),TEXT('"'));

				//ウィンドウ状態の取得
				TTextWindowInfo **tw;
				IF(!textwindow.GetElement( (INT)WindowNumber , &tw ))
				{
					MSGBOX("要素が見つかりません","ウィンドウ状態の取得");
				}

				IF( (INT)TextNumber == -1 )
				{
					NowTextNumber++;
					TextNumber = (float)NowTextNumber;
				}
				else
				{
					NowTextNumber = (INT)TextNumber;
				}

				this->AddText( (INT)TextNumber , TTextSet( textinfo , *tw ) );

			}


		}

		//開放
		read.Close();

	}


//////////////////////////////////////範囲取得クラス//////////////////////////////////////
/*
	//範囲情報
	//テキスト
	LPTkyTextWindoRangeDecisionView m_TextRangeView;
	//敵
	LPTkyEnemyAppearRangeDecisionView m_EnemyRangeView;
*/
	//コンストラクタ、デストラクタ
	CRangeManage::CRangeManage(Vector *pos)
	{
		m_TextRangeView = CTextWindoRangeDecisionView::CreateTextWinodwRangeDecision(pos);
		m_EnemyRangeView = CEnemyAppearRangeDecisionView::CreateEnemyApperRangeDecision(pos);
		m_EnemyKillView = CEnemyKillRangeDecisionView::CreateEnemyKillRangeDecision(pos);
	}
	CRangeManage::CRangeManage(const CRangeManage& t)
	{
		*m_EnemyRangeView = *t.m_EnemyRangeView;
		*m_TextRangeView = *t.m_TextRangeView;
		*m_EnemyKillView = *t.m_EnemyKillView;
	}
	CRangeManage::~CRangeManage()
	{
		IF( m_EnemyRangeView != NULL )m_EnemyRangeView->DestroyOwnObject();
		IF( m_TextRangeView != NULL )m_TextRangeView->DestroyOwnObject();
		IF( m_EnemyKillView != NULL )m_EnemyKillView->DestroyOwnObject();
	}

	//ファイルから取得
	VOID CRangeManage::LoadText(wstring name, CCharaManage *chara , CTextManage *text)
	{

		CTkyFileRead read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
		}

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("TEXTRANGE") )
			{
				//情報構造体
				TElementRange<LPTkyTextWindowiew> element;

				wstring Range = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring Circle = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				Vector pos;
				swscanf_s( Range.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );
				FLOAT r;
				swscanf_s( Circle.c_str() , TEXT("%f"),&r );

				element.pos = pos;
				element.r = r;
	
				//テキスト情報
				wstring textrange = read.GetToElementFromElement(TEXT('{'),TEXT('}'));

				UINT start = 0, end = 0;
				swscanf_s( textrange.c_str()		, TEXT("%u,%u"),&start,&end );

				vector<TTextSet*> textinfo = text->GetText(start,end);

				//要素の追加
				vector<TTextSet*>::iterator it = textinfo.begin();
				WHILE( it != textinfo.end() )
				{
					element.m_Element.push( CTextWindowiew::CreateTextWinodwView(*(*it)) );
					it++;
				}

				//テキスト範囲に追加
				m_TextRangeView->GetTextWinodwViewRange()->AddRange( element );
			}

			EIF( key == TEXT("ENEMYRANGE") )
			{
				//情報構造体
				TElementRange<LPTkyFPSEnemyView> element;

				wstring Range = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring Circle = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				Vector pos;
				swscanf_s( Range.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );
				FLOAT r;
				swscanf_s( Circle.c_str() , TEXT("%f"),&r );

				element.pos = pos;
				element.r = r;
	
				//要素の設定
				WHILE(!read.IsEof())
				{
					//キャラ情報
					wstring enemyname = read.GetToElementFromElement(TEXT('{'),TEXT('}'));
					IF( enemyname == TEXT("END") )break;

					wstring enemypos = read.GetToElementFromElement(TEXT('('),TEXT(')'));
					Vector pos;
					swscanf_s( enemypos.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );

					//敵ビュー
					TCharaSet tc = chara->Getchara( enemyname ); 
					LPTkyFPSEnemyView view = CFPSEnemyView::CreateFPSEnemyView( tc.m_chara , tc.m_weapon );
					view->GetCharactor()->SetPos( pos );

					//ビューのセット
					element.m_Element.push( view );
				}		

				//敵範囲に追加
				m_EnemyRangeView->GetEnemyApperViewRange()->AddRange( element );
			}


			IF( key == TEXT("ATTACKRANGE") )
			{
				//情報構造体
				TElementRange<int> element;

				wstring Range = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring Circle = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				Vector pos;
				swscanf_s( Range.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );
				FLOAT r;
				swscanf_s( Circle.c_str() , TEXT("%f"),&r );

				element.pos = pos;
				element.r = r;

				//テキスト範囲に追加
				m_EnemyKillView->GetEnemyKillViewRange()->AddRange( element );
			}

		
		}

		//開放
		read.Close();

	}











//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームステージ情報（シューティング）
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	LPTkyFPSHeroView										hero_view;					//主人公ビュー
	LPTkyTextWindoRangeDecisionView			textrange_view;			//テキスト情報ビュー
	LPTkyEnemyAppearRangeDecisionView		enemyrange_view;		//敵出現範囲情報
	LPTkyEnemyKillRangeDecisionView				killview;						//キルビュー
	LPTkyFPSMapView										map_view;					//マップ情報 
	LPTkyScoreView											score_view;					//スコア情報
*/

	//コンストラクタ、デストラクタ
	CShootingStageManage::CShootingStageManage()
	{
		hero_view = NULL;
		textrange_view = NULL;
		enemyrange_view = NULL;
		killview = NULL;
		map_view = NULL;
		score_view = NULL;
	}
	CShootingStageManage::CShootingStageManage(const CShootingStageManage& t)
	{
		*hero_view = *t.hero_view;
		*textrange_view = *t.textrange_view;
		*enemyrange_view = *t.enemyrange_view;
		*killview = *t.killview;
		*map_view = *t.map_view;
		*score_view = *t.score_view;
	}
	CShootingStageManage::~CShootingStageManage()
	{
	}

	//ファイルから取得
	VOID CShootingStageManage::LoadText(wstring name, CResourceManage *resource , CCharaManage *chara,CTextManage *text)
	{

		CTkyFileRead read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
		}

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("HERO") )
			{
				wstring name = read.GetToElementFromElement( TEXT('<') , TEXT('>') );
				wstring posname = read.GetToElementFromElement( TEXT('[') , TEXT(']') );

				Vector pos;
				swscanf_s( posname.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );

				hero_view = CFPSHeroView::CreateFPSHeroView();
				hero_view->SetCharactorWeaponData( 
					chara->Getchara( name ).m_chara , 
					chara->Getchara( name ).m_weapon );
				hero_view->GetCharactor()->SetPos( pos );
			}
			EIF( key == TEXT("MAP") )
			{
				wstring map = read.GetToElementFromElement( TEXT('<') , TEXT('>') );

				map_view = CFPSMapView::CreateMap( resource->GetXFie( map ) );

			}
			EIF( key == TEXT("STATE") )
			{
				wstring score = read.GetToElementFromElement( TEXT('<') , TEXT('>') );
				wstring result = read.GetToElementFromElement( TEXT('[') , TEXT(']') );
				wstring gameover = read.GetToElementFromElement( TEXT('(') , TEXT(')') );

				TScore t;
				t.initialize();
				t.m_chara = hero_view->GetCharactor();
				t.m_ScoreBack = resource->GetTexture(score);
				t.m_ShootingScore = resource->GetTexture(result);
				t.m_Gameover = resource->GetTexture(gameover);

				score_view = CScoreView::CreateScore( t );
			}
			EIF( key == TEXT("RANGE") )
			{
				
				wstring range = read.GetToElementFromElement( TEXT('<') , TEXT('>') );

				CRangeManage manage(pos);
				manage.LoadText(range,chara,text);

				textrange_view  = manage.GetTextRange();
				enemyrange_view = manage.GetEnemyRange();
				killview = manage.GetEnemyKillRange();

			}

			EIF( key == TEXT("MOVE") )
			{
				wstring move = read.GetToElementFromElement( TEXT('<') , TEXT('>') );
				TCharaInfo c =  hero_view->GetCharactor()->GetCharaInfo();
				c.m_automove = move;
				hero_view->GetCharactor()->SetCharaInfo(c);
			}

			EIF( key == TEXT("DIRECTION") )
			{
				wstring posname = read.GetToElementFromElement( TEXT('(') , TEXT(')') );
				Vector pos;
				swscanf_s( posname.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );

				TCharaInfo c =  hero_view->GetCharactor()->GetCharaInfo();
				c.m_first = pos;
				hero_view->GetCharactor()->SetCharaInfo(c);
			}

			EIF( key == TEXT("FOG") )
			{
				wstring name = read.GetToElementFromElement( TEXT('[') , TEXT(']') );

				swscanf_s( name.c_str() , TEXT("%f,%f"),&n,&f );
			}
		}

		//開放
		read.Close();

	}

	//実行
	VOID CShootingStageManage::Run( LPTkyGameLogic Logic)
	{
		Logic->AddGameView( hero_view );
		Logic->AddGameView( map_view );
		Logic->AddGameView( textrange_view );
		Logic->AddGameView( enemyrange_view );
		Logic->AddGameView( killview );
		Logic->AddGameView( score_view );

		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGSTART, *(DWORD*) (&n)); //開始位置
		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGEND, *(DWORD*) (&f));     //終了位置

	}

	//終了
	VOID CShootingStageManage::Stop()
	{
		DELETE_VIEW_MESSAGE( hero_view );
		DELETE_VIEW_MESSAGE( map_view );
		textrange_view->Delete();
		DELETE_VIEW_MESSAGE( textrange_view );
		enemyrange_view->Delete();
		DELETE_VIEW_MESSAGE( enemyrange_view );
		DELETE_VIEW_MESSAGE( killview );
		DELETE_VIEW_MESSAGE( score_view );
	}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームステージ情報（テキスト）
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	queue< LPTkyAdventureWindowiew > m_adventure;
*/

	//コンストラクタ、デストラクタ
	CTextStageManage::CTextStageManage()
	{
		WHILE(!m_adventure.empty())m_adventure.pop();
	}
	CTextStageManage::CTextStageManage(const CTextStageManage& t)
	{
		*this = t;
	}
	CTextStageManage::~CTextStageManage()
	{
		WHILE(!m_adventure.empty())m_adventure.pop();
	}

	//ファイルから取得
	VOID CTextStageManage::LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text)
	{

		CTkyFileRead read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
			return;
		}

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("TEXT") )
			{
				
				wstring range = read.GetToElementFromElement( TEXT('[') , TEXT(']') );
				wstring name;
				INT sp,ep;
				swscanf_s( range.c_str() , TEXT("%d,%d"),&sp , &ep );

				//要素の取得
				vector< TTextSet* > texts = text->GetText( sp ,ep );

				//情報
				vector< TTextSet* >::iterator it = texts.begin();
				WHILE( it != texts.end() )
				{
					//要素の追加
					m_adventure.push( CAdventureWindowiew::CreateAdventureWinodwView( *(*it) ) );
					it++;
				}
			}
		}

		//開放
		read.Close();

	}


	//実行
	VOID CTextStageManage::Run( LPTkyGameLogic logic )
	{
		//要素の数だけループ
		queue<LPTkyAdventureWindowiew> m = m_adventure;
		WHILE( !m.empty() )
		{
			//要素の追加
			logic->AddGameView( m.front() );
			m.pop();
		}
	}

	VOID CTextStageManage::Stop()
	{
	}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームステージ情報（タイトル）
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	LPTkyTitleView m_tview;
*/

	//コンストラクタ
	CTitleManage::CTitleManage()
	{
		m_tview = NULL;
	}
	CTitleManage::CTitleManage( const CTitleManage& t )
	{
		*this = t;
	}
	CTitleManage::~CTitleManage()
	{
	}

	//ファイルから取得
	VOID CTitleManage::LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text)
	{

		CTkyFileRead read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
			return;
		}

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("TITLE") )
			{
				
				TTitle title;

				//要素の設定
				WHILE(!read.IsEof())
				{
					//キャラ情報
					wstring name = read.GetToElementFromElement(TEXT('{'),TEXT('}'));
					IF( name == TEXT("END") )break;

					wstring graph = read.GetToElementFromElement(TEXT('['),TEXT(']'));

					IF( name == TEXT("MENU_START") )
					{
						title.m_start = resource->GetTexture(graph);
					}
					EIF( name == TEXT("MENU_SCORE") )
					{
						title.m_score = resource->GetTexture(graph);
					}
					EIF( name == TEXT("MENU_BACK") )
					{
						title.m_back = resource->GetTexture(graph);
					}
					EIF( name == TEXT("MENU_LOGO") )
					{
						title.m_logo = resource->GetTexture(graph);
					}
					EIF( name == TEXT("MENU_RANKING") )
					{
						title.m_ranking = resource->GetTexture(graph);
					}
				}

				//要素の作成
				IF(m_tview == NULL)
				{
					m_tview = CTitleView::CreateTitle(title);
				}
				else
				{
					m_tview->SetTitle( title );
				}
				
			}

		}

		//開放
		read.Close();

	}

	//実行
	VOID CTitleManage::Run( LPTkyGameLogic logic )
	{
		logic->AddGameView( m_tview );
	}

	//終了
	VOID CTitleManage::Stop()
	{
		DELETE_VIEW_MESSAGE(m_tview);
	}



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//ゲームの流れ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//ステージ情報
	queue< IStageManage* > m_stages;
*/

	//コンストラクタ、デストラクタ
	CGameFlowManage::CGameFlowManage()
	{
		WHILE(!m_stages.empty())m_stages.pop();
		m_hitpoint = 0;
		m_score = 0;
	}
	CGameFlowManage::CGameFlowManage(const CGameFlowManage& g)
	{
		*this = g;
	}
	CGameFlowManage::~CGameFlowManage()
	{
		WHILE(!m_stages.empty())m_stages.pop();
	}

	//ファイルから取得
	VOID CGameFlowManage::LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text ,Vector *pos , Vector *vec )
	{
		m_hitpoint = 0;
		m_score = 0;


		CTkyFileRead read;

		//読み込み
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("ファイルが読み込めなかったようです"),name.c_str(),NULL);
			return;
		}

		//キャラ情報読み込み
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("GAMEFLOW") )
			{
				
				wstring game = read.GetToElementFromElement( TEXT('[') , TEXT(']') );
				wstring info = read.GetToElementFromElement( TEXT('<') , TEXT('>') );

				IF( game == TEXT("TITLE") )
				{
					//テキストモードの取得
					m_stages.push( CTitleManage::CreateTitleManage( info , resource , chara , text ) );
				}
				IF( game == TEXT("TEXT") )
				{
					//テキストモードの取得
					m_stages.push( CTextStageManage::CreateTextStage( info , resource , chara , text ) );
				}
				EIF( game == TEXT("SHOOTING") )
				{
					//シューティングモードの取得
					m_stages.push( CShootingStageManage::CreateShootingStage( info , resource , chara , text , pos , vec ) );
				}

			}
		}

		//開放
		read.Close();

	}

	//実行
	VOID CGameFlowManage::Run( LPTkyGameLogic logic )
	{
		IF(m_stages.empty())return;
		//ビューのセット
		m_stages.front()->Run( logic );
	}

	//情報取得
	VOID CGameFlowManage::GetBaseData()
	{
		//体力などを取得
		IF( typeid(* (m_stages.front()) ) == typeid(CShootingStageManage) )
		{
			m_hitpoint = ((LPTkyShootingStageManage)m_stages.front())->GetChara()->GetCharactor()->GetCharaInfo().m_HitPoint;
			m_score = ((LPTkyShootingStageManage)m_stages.front())->GetScore()->GetScore()->GetScore().m_score;
		}
	}

	//次を実行
	BOOL CGameFlowManage::Next( )
	{
		IF(m_stages.empty())return false;
		//ステージを次にする
		m_stages.pop();
		IF(m_stages.empty())return false;

		IF( typeid(* (m_stages.front()) ) == typeid(CShootingStageManage) )
		{
			IF( m_hitpoint != 0 )
			{
				TCharaInfo t = ((LPTkyShootingStageManage)m_stages.front())->GetChara()->GetCharactor()->GetCharaInfo();
				t.m_HitPoint = m_hitpoint;
				((LPTkyShootingStageManage)m_stages.front())->GetChara()->GetCharactor()->SetCharaInfo(t);
			}
			IF( m_score != 0 )
			{
				TScore s = ((LPTkyShootingStageManage)m_stages.front())->GetScore()->GetScore()->GetScore();
				s.m_score = m_score;
				((LPTkyShootingStageManage)m_stages.front())->GetScore()->GetScore()->SetScore(s);
			}
		}

		return true;
	}

	//終了
	VOID CGameFlowManage::Stop()
	{
		IF(m_stages.empty())return;

		//とまる
		m_stages.front()->Stop();
		//消去
		m_stages.front()->DestroyOwnObject();
	}

