#include "ResourceManage.h"

//////////////////////////////////////���\�[�X�Ǘ��N���X//////////////////////////////////////

//-----------------------------
//���\�[�X�Ǘ��N���X
//-----------------------------
/*	//X�t�@�C�����
	CStringMatchW<LPTkyXFile> m_chara;
	//�摜�t�@�C�����
	CStringMatchW<LPTkyTexture> m_chara;
*/
	//�R���X�g���N�^�A�f�X�g���N�^
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
		//���̊֐��ł̓������͊J������Ȃ�
		m_x.Destroy();
		m_tex.Destroy();
	}

	//�ǉ�
	VOID CResourceManage::AddXFie(LPTkyXFile x,wstring name)
	{
		m_x.AddString( name , x );
	}
	VOID CResourceManage::AddTexture(LPTkyTexture x,wstring name)
	{
		m_tex.AddString( name , x );
	}
	//����
	VOID CResourceManage::DeleteXFie(wstring name)
	{
		m_x.Destroy(name);
	}
	VOID CResourceManage::DeleteTexture(wstring name)
	{
		m_tex.Destroy(name);
	}
	//�擾
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

	//�t�@�C������擾(���\�[�X�����Ȃ���)
	VOID CResourceManage::LoadResource(wstring name )
	{

		CTkyFileRead read;

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
		}

		//�L�������ǂݍ���
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("XFILE") )
			{
				wstring name = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring element = read.GetToElementFromElement(TEXT('<'),TEXT('>'));
				wstring speed = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				float sp=0 ;	//�X�s�[�h
				swscanf_s( speed.c_str() , TEXT("%f"),&sp );

				LPTkyXFile x =CXFile::CreateXFile( Init3D::CreateInit3D() , element );
				x->GetAnime()->SetAnimeSpeed(sp);

				//�t�@�C���̒ǉ�
				m_x.AddString( name ,  x );
			}

			EIF( key == TEXT("TEXTURE") )
			{
				wstring name = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring tex = read.GetToElementFromElement(TEXT('<'),TEXT(','));
				wstring element = read.GetToElement(TEXT('>'));
				INT n=0 , ho=0 , ver=0 , w=0 , h=0 , s=0 ;	//���A�������A�������A���A�����A�X�s�[�h
				swscanf_s( element.c_str() , TEXT("%d,%d,%d,%d,%d,%d"),&n,&ho,&ver,&w,&h,&s );
				//�t�@�C���̒ǉ�
				m_tex.AddString( name , CTexture::CreateTextureW( Init3D::CreateInit3D() , tex , n , ho, ver ,w , h , (DWORD)s )  );
			}

		}

		//�J��
		read.Close();

	}


//////////////////////////////////////�L�����N�^�Ǘ��N���X//////////////////////////////////////
/*
	//�L�������
	CStringMatchW<LPTkyCharactor> m_chara;
*/
	//�R���X�g���N�^�A�f�X�g���N�^
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

	//�ǉ�
	VOID CCharaManage::AddCharactor(TCharaSet chara)
	{
		//�L�����̖��O�Œǉ�
		m_chara.AddString( chara.m_chara->GetCharaInfo().m_name , chara );
	}
	//����
	VOID CCharaManage::DeleteCharactor(TCharaSet chara)
	{
		//�L�����̖��O�ŏ���
		m_chara.Destroy( chara.m_chara->GetCharaInfo().m_name );
	}
	VOID CCharaManage::DeleteCharactor(wstring name)
	{
		//�L�����̖��O�ŏ���
		m_chara.Destroy( name );
	}
	//�擾
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

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
		}

		//�L�������ǂݍ���
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("CHARACTOR") )
			{
				//���\����
				TGameTextInfo CharaText;

				//��{���
				wstring CharaName = read.GetToElementFromElement(TEXT('['),TEXT(','));
				wstring name = read.GetToElement(TEXT(']'));
				wstring AIName = read.GetToElementFromElement(TEXT('('),TEXT(')'));

				int HitPoint = NULL , Velocity = NULL;
				float Length = NULL;
				swscanf_s( name.c_str() , TEXT("%d,%d,%f"),&HitPoint,&Velocity,&Length );
				TCharaInfo chara(CharaName,HitPoint,Velocity,Length,AIName);
				CharaText.push_back( TGameText( TEXT("CHARACTOR_SET_BASEINFO") , TEXT("") , (DWORD)( &chara) ) );

				//������
				wstring bullet = read.GetToElementFromElement(TEXT('<'),TEXT('>'));
				float FlyWait = NULL,AttackRange = NULL,AttackSpeed = NULL,AttackLength = NULL;
				float AttackPower = NULL;
				swscanf_s( bullet.c_str() , TEXT("%f,%f,%f,%f,%f"),&FlyWait,&AttackPower,&AttackRange,&AttackSpeed,&AttackLength );
				CharaText.push_back( TGameText( TEXT("WEAPON_SET_BASEINFO") , TEXT("") , (DWORD)( &TWeaponInfo( FlyWait  ) ) ) );
				CharaText.push_back( TGameText( TEXT("ATTACK_SET_BASEINFO") , TEXT("") , (DWORD)( &TBulletInfo((int)AttackPower,AttackRange,AttackSpeed,AttackLength) ) ) );

				//�A�j���[�V�����G�t�F�N�g���
				WHILE(!read.IsEof())
				{
					wstring value = read.GetToElementFromElement(TEXT('{'),TEXT('}'));
					IF( value == TEXT("END") )break;

					//���������Ƃ�����K�v������
					wchar_t *ValueName,*ElementName,*ResourceName,*Next;
					ValueName = wcstok_s( const_cast<wchar_t*>(value.c_str())  , TEXT(",") , &Next  );
					ElementName = wcstok_s( NULL , TEXT(",") , &Next  );
					ResourceName = wcstok_s( NULL , TEXT(",") , &Next  );


					//�A�j���[�V������������
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
					//�G�t�F�N�g��������
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

				//��l�����̐ݒ�
				TCharaSet charaset;
				charaset.NewAllocate();
				charaset.m_chara->onLoadGameDate(CharaText);
				charaset.m_weapon->onLoadGameDate(CharaText);
				m_chara.AddString( CharaName , charaset );
			}

		}

		//�J��
		read.Close();
	}



//////////////////////////////////////�e�L�X�g�Ǘ��N���X//////////////////////////////////////

/*
	//�L�������
	CNumberMatch<TTextSet> m_Text;
*/

	//�R���X�g���N�^�A�f�X�g���N�^
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

	//�ǉ�
	VOID CTextManage::AddText(INT n,TTextSet text)
	{
		m_Text.AddString(n,text);
	}
	//����
	VOID CTextManage::DeleteText(INT n)
	{
		m_Text.Destroy(n);
	}
	//�擾
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

	//�t�@�C������擾
	VOID CTextManage::LoadText(wstring name, CResourceManage* resource)
	{

		CTkyFileReadCW read;

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
		}

		//���̃e�L�X�g�ԍ�
		UINT NowTextNumber = NULL;

		//�L�������ǂݍ���
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();


			IF( key == TEXT("WINDOW") )
			{
				TTextWindowInfo *window = new TTextWindowInfo();

				wstring windownumber  = read.GetToElementFromElement(TEXT('['),TEXT(','));
				//�e�N�X�`���摜���O
				wstring wname  = read.GetToElement(TEXT(','));
				wstring wposstart  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wposend  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wrotatestart  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wrotateposend  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wexpandstart  = read.GetToElementFromElement(TEXT('('),TEXT(')'));
				wstring wexpandend  = read.GetToElementFromElement(TEXT('('),TEXT(')'));

				//�摜�i���o�[
				INT number = NULL;
				swscanf_s( windownumber.c_str(), TEXT("%d"),&number );

				//�ړ��x�N�^�[
				Vector sp,sr,se,ep,er,ee;
				swscanf_s( wposstart.c_str()		, TEXT("%f,%f,%f"),&sp.x,&sp.y,&sp.z );
				swscanf_s( wposend.c_str()			, TEXT("%f,%f,%f"),&ep.x,&ep.y,&ep.z );
				swscanf_s( wrotatestart.c_str()		, TEXT("%f,%f,%f"),&sr.x,&sr.y,&sr.z );
				swscanf_s( wrotateposend.c_str()	, TEXT("%f,%f,%f"),&er.x,&er.y,&er.z );
				swscanf_s( wexpandstart.c_str()		, TEXT("%f,%f,%f"),&se.x,&se.y,&se.z );
				swscanf_s( wexpandend.c_str()		, TEXT("%f,%f,%f"),&ee.x,&ee.y,&ee.z );

				//�v�f�̒ǉ�
				window->m_textpos = Vector(0,0,0);
				window->m_WindowTexture = resource->GetTexture(wname);
				window->m_WindowPosEndStart = CVector( sp , ep );
				window->m_WindowRotateEndStart = CVector( sr , er );
				window->m_WindowExpandEndStart = CVector( se , ee );

				//�����G�̐ݒ�
				WHILE(!read.IsEof())
				{
					//�L�����摜
					wstring charaname = read.GetToElementFromElement(TEXT('<'),TEXT('>'));
					IF( charaname == TEXT("END") )break;

					//�ʒu
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

				//�E�B���h�E�i���o�[�A�e�L�X�g�i���o�[�A�e�L�X�g�X�s�[�h
				FLOAT WindowNumber=NULL , TextNumber=NULL , TextSpeed=NULL;
				swscanf_s( info.c_str()		, TEXT("%f,%f,%f"),&TextNumber,&WindowNumber,&TextSpeed );
				
				//�e�L�X�g�̑���
				textinfo.m_ShowSpeed = TextSpeed;

				locale::global(locale("japanese"));

				//�w�b�h���
				textinfo.m_name = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				//�e�L�X�g�̒��g
				textinfo.m_string  = read.GetToElementFromElement(TEXT('"'),TEXT('"'));

				//�E�B���h�E��Ԃ̎擾
				TTextWindowInfo **tw;
				IF(!textwindow.GetElement( (INT)WindowNumber , &tw ))
				{
					MSGBOX("�v�f��������܂���","�E�B���h�E��Ԃ̎擾");
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

		//�J��
		read.Close();

	}


//////////////////////////////////////�͈͎擾�N���X//////////////////////////////////////
/*
	//�͈͏��
	//�e�L�X�g
	LPTkyTextWindoRangeDecisionView m_TextRangeView;
	//�G
	LPTkyEnemyAppearRangeDecisionView m_EnemyRangeView;
*/
	//�R���X�g���N�^�A�f�X�g���N�^
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

	//�t�@�C������擾
	VOID CRangeManage::LoadText(wstring name, CCharaManage *chara , CTextManage *text)
	{

		CTkyFileRead read;

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
		}

		//�L�������ǂݍ���
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("TEXTRANGE") )
			{
				//���\����
				TElementRange<LPTkyTextWindowiew> element;

				wstring Range = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring Circle = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				Vector pos;
				swscanf_s( Range.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );
				FLOAT r;
				swscanf_s( Circle.c_str() , TEXT("%f"),&r );

				element.pos = pos;
				element.r = r;
	
				//�e�L�X�g���
				wstring textrange = read.GetToElementFromElement(TEXT('{'),TEXT('}'));

				UINT start = 0, end = 0;
				swscanf_s( textrange.c_str()		, TEXT("%u,%u"),&start,&end );

				vector<TTextSet*> textinfo = text->GetText(start,end);

				//�v�f�̒ǉ�
				vector<TTextSet*>::iterator it = textinfo.begin();
				WHILE( it != textinfo.end() )
				{
					element.m_Element.push( CTextWindowiew::CreateTextWinodwView(*(*it)) );
					it++;
				}

				//�e�L�X�g�͈͂ɒǉ�
				m_TextRangeView->GetTextWinodwViewRange()->AddRange( element );
			}

			EIF( key == TEXT("ENEMYRANGE") )
			{
				//���\����
				TElementRange<LPTkyFPSEnemyView> element;

				wstring Range = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring Circle = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				Vector pos;
				swscanf_s( Range.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );
				FLOAT r;
				swscanf_s( Circle.c_str() , TEXT("%f"),&r );

				element.pos = pos;
				element.r = r;
	
				//�v�f�̐ݒ�
				WHILE(!read.IsEof())
				{
					//�L�������
					wstring enemyname = read.GetToElementFromElement(TEXT('{'),TEXT('}'));
					IF( enemyname == TEXT("END") )break;

					wstring enemypos = read.GetToElementFromElement(TEXT('('),TEXT(')'));
					Vector pos;
					swscanf_s( enemypos.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );

					//�G�r���[
					TCharaSet tc = chara->Getchara( enemyname ); 
					LPTkyFPSEnemyView view = CFPSEnemyView::CreateFPSEnemyView( tc.m_chara , tc.m_weapon );
					view->GetCharactor()->SetPos( pos );

					//�r���[�̃Z�b�g
					element.m_Element.push( view );
				}		

				//�G�͈͂ɒǉ�
				m_EnemyRangeView->GetEnemyApperViewRange()->AddRange( element );
			}


			IF( key == TEXT("ATTACKRANGE") )
			{
				//���\����
				TElementRange<int> element;

				wstring Range = read.GetToElementFromElement(TEXT('['),TEXT(']'));
				wstring Circle = read.GetToElementFromElement(TEXT('<'),TEXT('>'));

				Vector pos;
				swscanf_s( Range.c_str() , TEXT("%f,%f,%f"),&pos.x,&pos.y,&pos.z );
				FLOAT r;
				swscanf_s( Circle.c_str() , TEXT("%f"),&r );

				element.pos = pos;
				element.r = r;

				//�e�L�X�g�͈͂ɒǉ�
				m_EnemyKillView->GetEnemyKillViewRange()->AddRange( element );
			}

		
		}

		//�J��
		read.Close();

	}











//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���X�e�[�W���i�V���[�e�B���O�j
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	LPTkyFPSHeroView										hero_view;					//��l���r���[
	LPTkyTextWindoRangeDecisionView			textrange_view;			//�e�L�X�g���r���[
	LPTkyEnemyAppearRangeDecisionView		enemyrange_view;		//�G�o���͈͏��
	LPTkyEnemyKillRangeDecisionView				killview;						//�L���r���[
	LPTkyFPSMapView										map_view;					//�}�b�v��� 
	LPTkyScoreView											score_view;					//�X�R�A���
*/

	//�R���X�g���N�^�A�f�X�g���N�^
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

	//�t�@�C������擾
	VOID CShootingStageManage::LoadText(wstring name, CResourceManage *resource , CCharaManage *chara,CTextManage *text)
	{

		CTkyFileRead read;

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
		}

		//�L�������ǂݍ���
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

		//�J��
		read.Close();

	}

	//���s
	VOID CShootingStageManage::Run( LPTkyGameLogic Logic)
	{
		Logic->AddGameView( hero_view );
		Logic->AddGameView( map_view );
		Logic->AddGameView( textrange_view );
		Logic->AddGameView( enemyrange_view );
		Logic->AddGameView( killview );
		Logic->AddGameView( score_view );

		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGSTART, *(DWORD*) (&n)); //�J�n�ʒu
		Init3D::CreateInit3D()->GetD3DDevice()->SetRenderState(D3DRS_FOGEND, *(DWORD*) (&f));     //�I���ʒu

	}

	//�I��
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
//�Q�[���X�e�[�W���i�e�L�X�g�j
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	queue< LPTkyAdventureWindowiew > m_adventure;
*/

	//�R���X�g���N�^�A�f�X�g���N�^
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

	//�t�@�C������擾
	VOID CTextStageManage::LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text)
	{

		CTkyFileRead read;

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
			return;
		}

		//�L�������ǂݍ���
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("TEXT") )
			{
				
				wstring range = read.GetToElementFromElement( TEXT('[') , TEXT(']') );
				wstring name;
				INT sp,ep;
				swscanf_s( range.c_str() , TEXT("%d,%d"),&sp , &ep );

				//�v�f�̎擾
				vector< TTextSet* > texts = text->GetText( sp ,ep );

				//���
				vector< TTextSet* >::iterator it = texts.begin();
				WHILE( it != texts.end() )
				{
					//�v�f�̒ǉ�
					m_adventure.push( CAdventureWindowiew::CreateAdventureWinodwView( *(*it) ) );
					it++;
				}
			}
		}

		//�J��
		read.Close();

	}


	//���s
	VOID CTextStageManage::Run( LPTkyGameLogic logic )
	{
		//�v�f�̐��������[�v
		queue<LPTkyAdventureWindowiew> m = m_adventure;
		WHILE( !m.empty() )
		{
			//�v�f�̒ǉ�
			logic->AddGameView( m.front() );
			m.pop();
		}
	}

	VOID CTextStageManage::Stop()
	{
	}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���X�e�[�W���i�^�C�g���j
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	LPTkyTitleView m_tview;
*/

	//�R���X�g���N�^
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

	//�t�@�C������擾
	VOID CTitleManage::LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text)
	{

		CTkyFileRead read;

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
			return;
		}

		//�L�������ǂݍ���
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("TITLE") )
			{
				
				TTitle title;

				//�v�f�̐ݒ�
				WHILE(!read.IsEof())
				{
					//�L�������
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

				//�v�f�̍쐬
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

		//�J��
		read.Close();

	}

	//���s
	VOID CTitleManage::Run( LPTkyGameLogic logic )
	{
		logic->AddGameView( m_tview );
	}

	//�I��
	VOID CTitleManage::Stop()
	{
		DELETE_VIEW_MESSAGE(m_tview);
	}



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���̗���
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
	//�X�e�[�W���
	queue< IStageManage* > m_stages;
*/

	//�R���X�g���N�^�A�f�X�g���N�^
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

	//�t�@�C������擾
	VOID CGameFlowManage::LoadText(wstring name , CResourceManage *resource , CCharaManage *chara,CTextManage *text ,Vector *pos , Vector *vec )
	{
		m_hitpoint = 0;
		m_score = 0;


		CTkyFileRead read;

		//�ǂݍ���
		IF(!read.Open(name))
		{
			MessageBoxW(NULL,TEXT("�t�@�C�����ǂݍ��߂Ȃ������悤�ł�"),name.c_str(),NULL);
			return;
		}

		//�L�������ǂݍ���
		WHILE(!read.IsEof())
		{
			wstring key = read.GetToWhiteSpace();

			IF( key == TEXT("GAMEFLOW") )
			{
				
				wstring game = read.GetToElementFromElement( TEXT('[') , TEXT(']') );
				wstring info = read.GetToElementFromElement( TEXT('<') , TEXT('>') );

				IF( game == TEXT("TITLE") )
				{
					//�e�L�X�g���[�h�̎擾
					m_stages.push( CTitleManage::CreateTitleManage( info , resource , chara , text ) );
				}
				IF( game == TEXT("TEXT") )
				{
					//�e�L�X�g���[�h�̎擾
					m_stages.push( CTextStageManage::CreateTextStage( info , resource , chara , text ) );
				}
				EIF( game == TEXT("SHOOTING") )
				{
					//�V���[�e�B���O���[�h�̎擾
					m_stages.push( CShootingStageManage::CreateShootingStage( info , resource , chara , text , pos , vec ) );
				}

			}
		}

		//�J��
		read.Close();

	}

	//���s
	VOID CGameFlowManage::Run( LPTkyGameLogic logic )
	{
		IF(m_stages.empty())return;
		//�r���[�̃Z�b�g
		m_stages.front()->Run( logic );
	}

	//���擾
	VOID CGameFlowManage::GetBaseData()
	{
		//�̗͂Ȃǂ��擾
		IF( typeid(* (m_stages.front()) ) == typeid(CShootingStageManage) )
		{
			m_hitpoint = ((LPTkyShootingStageManage)m_stages.front())->GetChara()->GetCharactor()->GetCharaInfo().m_HitPoint;
			m_score = ((LPTkyShootingStageManage)m_stages.front())->GetScore()->GetScore()->GetScore().m_score;
		}
	}

	//�������s
	BOOL CGameFlowManage::Next( )
	{
		IF(m_stages.empty())return false;
		//�X�e�[�W�����ɂ���
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

	//�I��
	VOID CGameFlowManage::Stop()
	{
		IF(m_stages.empty())return;

		//�Ƃ܂�
		m_stages.front()->Stop();
		//����
		m_stages.front()->DestroyOwnObject();
	}

