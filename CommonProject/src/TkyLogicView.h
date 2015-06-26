#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TkyEvent.h"
#include "TKYTASK.h"
#include <map>
/*

�Q�[�����W�b�N

�Q�[���r���[

�̃C���^�[�t�F�C�X�̒�`

*/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���C���^�[�t�F�C�X
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IGameBase , LPTkyGameBase ):public IEventLisner
{
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���r���[
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IGameView , LPTkyGameView ) :public IGameBase 
{
protected:
public:

//�Q�[���r���[�v�f

	//�^�X�N�����i�K�ŌĂ΂��
	//������
	VIRTUAL BOOL onCreate() = 0;
	//�X�V
	VIRTUAL BOOL onUpdate(float step) = 0;

	//�r���[�v�f
	//�`��
	VIRTUAL VOID onRender() = 0;

//�C�x���g�v�f

	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str ) = 0;

	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;

//�^�X�N�v�f

};

//�Q�[���r���[�i�[�p�^�X�N
CLASS_POINTA( CGameViewTask , LPTkyGameViewTask ) : public ITkyMultiTask
{
private:

	LPTkyGameView m_view;

	CGameViewTask(  )
	{
	}
VIRTUAL ~	CGameViewTask()
	{
	}

public:
	//�^�X�N�̐���
	VIRTUAL BOOL onCreate()
	{
		//�r���[�̏���������
		return m_view->onCreate();
	}
	//�^�X�N�̎��s
	VIRTUAL BOOL onUpdate(float step)
	{
		//�r���[�̍X�V
		return m_view->onUpdate(step);
	}

	//����
	STATIC LPTkyGameViewTask CreateGameViewTask( LPTkyGameView view )
	{
		LPTkyGameViewTask t = new CGameViewTask();
		t->SetGameView( view );
		return t;
	}


	//�ݒ�
	bool SetGameView( LPTkyGameView view )
	{
		IF( view == NULL )return false;
		m_view = view;
		return true;
	}
	//�擾
	LPTkyGameView GetGameView()
	{
		return m_view;
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[�����W�b�N(�C���^�[�t�F�[�X)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CLASS_POINTA( IGameLogic , LPTkyGameLogic ) :public IGameBase
{
private:

	//�v���Z�X�Ǘ�(View������)
	LPTKYMULTITASKSYSTEM m_TaskManager;
protected:

	//�}�b�v�A�h���X�}�b�v
	map< LPTkyGameView , LPTKYMULTITASK > m_TaskMap;

	//�R���X�g���N�^�A�f�X�g���N�^
	IGameLogic()
	{
		m_TaskManager = CTkyMultiTaskSystem::CreateTaskSystem();
		CEventManager::StaticAddEventLisner(this);
	}
	VIRTUAL ~IGameLogic()
	{
	}

	//�I���r���[
	queue<LPTkyGameView> m_DeleteView;

public:

	//�r���[�̃Z�b�g������
	bool AddGameView( LPTkyGameView view )
	{
		assert(view);

		IF(view == NULL)return false;

		//���b�Z�[�W�ɒǉ�----------------------------------------------
		CEventManager::StaticAddEventLisner(view);
		//---------------------------------------------------------------

		//�^�X�N���̎擾----------------------------------------------
		LPTKYMULTITASK task = CGameViewTask::CreateGameViewTask(view);
		//�^�X�N�ǉ�(����������)
		IF(m_TaskManager->AddTask( task )==false)
		{
			MessageBoxA(NULL,"�^�X�N�G�b���[","ERR",MB_OK);
			return false;
		}
		//---------------------------------------------------------------

		//�^�X�N�ƃr���[�̃}�b�v(�������邽�߂̃}�b�v�ɒǉ�)--------
		m_TaskMap.insert( map<LPTkyGameView,LPTKYMULTITASK>::value_type(view,task) );
		//---------------------------------------------------------------
		
		//�p����ւ̒ǉ�����
		onAddView(view);

		return true;
	}
	//�r���[�̏���
	bool DeleteGameView( LPTkyGameView view )
	{
		//���Ă�[�c�����
		map<LPTkyGameView,LPTKYMULTITASK>::iterator ig;
		//�v�f��T��
		ig = m_TaskMap.find(view);
		//�������Ă�����v�f������
		IF(ig != m_TaskMap.end())
		{
			//�^�X�N�������
			m_TaskManager->DestroyTask(ig->second);
			//���b�Z�[�W�������
			CEventManager::StaticDeleteEventLisner(ig->first);
			//���̂�����
			(ig->first)->DestroyOwnObject();

			ig = m_TaskMap.erase( ig );
			return true;
		}
		return false;
	}
	//�v�f��������
	bool DeleteGameViewOnlyIndex( LPTkyGameView view )
	{
		//���Ă�[�c�����
		map<LPTkyGameView,LPTKYMULTITASK>::iterator ig;
		//�v�f��T��
		ig = m_TaskMap.find(view);
		//�������Ă�����v�f������
		IF(ig != m_TaskMap.end())
		{
			//�^�X�N�������
			m_TaskManager->DestroyTask(ig->second);
			//���b�Z�[�W�������
			CEventManager::StaticDeleteEventLisner(ig->first);

			ig = m_TaskMap.erase( ig );
			return true;
		}
		return false;
	}

	//������
	BOOL Create()
	{
		return onCreate();
	}

	//�X�V
	VOID Update(float step)
	{

		//�I���\��r���[�̏���
		WHILE( !m_DeleteView.empty() )
		{
			LPTkyGameView view = m_DeleteView.front();

			//���̂�����
			this->DeleteGameView( view );

			m_DeleteView.pop();
		}

		onUpdate(step);
		m_TaskManager->Update(step);
	}

	//�`��
	VOID Render()
	{
		list<LPTKYMULTITASK> thread = m_TaskManager->GetThread();
		list<LPTKYMULTITASK>::iterator it = thread.begin();
		WHILE( it != thread.end() )
		{
			( (LPTkyGameViewTask)( *it ))->GetGameView()->onRender();
			it++;
		}

		onRender();
	}

	//���̍H���ŌĂ΂��
	//������
	VIRTUAL BOOL onCreate() = 0;
	//�X�V
	VIRTUAL VOID onUpdate(float step) = 0;
	//�X�V
	VIRTUAL VOID onRender() = 0;

	//View�ݒ莞
	VIRTUAL VOID onAddView( LPTkyGameView view ) = 0;

	//View������
	VIRTUAL VOID onDeleteView( LPTkyGameView view ) = 0;

	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "GAME";
	}
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg )
	{

		//�ǉ����b�Z�[�W��������A���̃r���[��ǉ�����
		IF( msg.m_message == "LOGIC_ADDVIEW" )
		{
			this->AddGameView( (LPTkyGameView)msg.m_value[0] );
		}

		//�I�����b�Z�[�W�����������A�I���r���[�L���[�ɓ���Ă����A���Ƃŏ���
		IF( msg.m_message == "LOGIC_DELETEVIEW" )
		{
			//�������������������
			IF( m_TaskMap[(LPTkyGameView)msg.m_value[0]] == NULL )
			{
				//�ꗗ�������
				this->DeleteGameViewOnlyIndex( (LPTkyGameView)msg.m_value[0] );

				//���b�Z�[�W���Ƃ߂�
				CEventManager::StaticStopEventMessage();
				return;
			}
			//�����̂ɓ����
			this->m_DeleteView.push( (LPTkyGameView)msg.m_value[0] );
			//����
			onDeleteView((LPTkyGameView)msg.m_value[0]);
		}

	}

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�Q�[���A�v���P�[�V�������C���[
/*

WM�̏���
�C�x���g����
�L�[����
Windows��{����

*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

CLASS_POINTA( IApplicationLayer , LPTkyApplicationLayer ) :public IGameBase
{
private:

	LPTkyEventManager m_EventManager;

protected:

	IApplicationLayer()
	{
		m_EventManager = CEventManager::CreateEventManager();
		m_EventManager->AddEventLisner( this );
	}
	VIRTUAL ~IApplicationLayer()
	{
	}

public:

	//������
	BOOL Create()
	{
		return onCreate();
	}
	//�X�V
	BOOL LoopStart()
	{
		return onLoopStep();
	}

	//�X�V����
	VIRTUAL BOOL onLoopStep()=0;

	//������
	VIRTUAL BOOL onCreate()=0;

	//�C�x���g�l�[�����O�����߂�
	VIRTUAL VOID onNameSet( string *str )
	{
		*str = "Application";
	}
	//�C�x���g�����炤
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;

};