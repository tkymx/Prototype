#pragma once

#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include <list>
#include <vector>
#include <queue>

/*

�C�x���g�����ȈՔ�
�C�x���g���X�i�[

����̓A�v���P�[�V�����\���ň�ԑ厖�ȗv�f�ł���

���ׂĂ̓V���O���g�����ꂽ

�C�x���g�}�l�[�W�����o�R����
���ׂĂɎ󂯌p�����

�C�x���g�}�l�[�W����
���ׂĂ̗v�f�ɑ΂��Čp������Ȃ���΂Ȃ�Ȃ�

STATIC�v�f�𗘗p����
�C�x���g���X�i�[�̐����A����
���b�Z�[�W�̑��M���s��



�����ꂩ���邱�� 4/3 0:17
�C�x���g���X�i�[�̏�����
�X���b�h����\��



*/

//�C�x���g���
struct TEventMessage
{
	//�C�x���g�l�[��
	string m_message;
	//�C�x���g�v�f
	vector<DWORD> m_value;

	static TEventMessage Initializel(string str,DWORD d)
	{
		TEventMessage m;
		m.m_message = str;
		m.m_value.push_back(d);
		return m;
	}

};

//�C�x���g�f�[�^
CLASS_POINTA_OBJECT( IEventLisner , LPTkyEventLisner )
{
private:

	//�C�x���g�l�[��
	string m_name;

protected:
	
	//�R���X�g���N�^
	IEventLisner();
	VIRTUAL ~IEventLisner();


public:

	//���O�����߂�
	VIRTUAL VOID onNameSet( string *str ) = 0;
	//���O���擾����
	VIRTUAL string GetName();

	//�C�x���g�擾
	VIRTUAL VOID onEvent( TEventMessage msg ) = 0;

	//������
	VOID Create();

};

//�C�x���g�}�l�[�W���[ (�V���O���g��)
CLASS_POINTA_OBJECT( CEventManager , LPTkyEventManager )
{
private:
	//�V���O���g���C�x���g
	STATIC LPTkyEventManager m_gManager;
	
	//�C�x���g���X�i�[
	list<LPTkyEventLisner> m_EventLisner;
	//���b�Z�[�WQue
	queue<TEventMessage> m_message;

	//�R���X�g���N�^�A�f�X�g���N�^
	CEventManager();
	VIRTUAL ~CEventManager();

	//�X�g�b�v
	bool m_stop;

public:

	//�C�x���g���X�i�[�̒ǉ�
	bool AddEventLisner( LPTkyEventLisner Event);
	//�C�x���g���X�i�[�̏���
	bool DeleteEventLisner( LPTkyEventLisner Event);
	//�C�x���g���b�Z�[�W�[�̒ǉ�
	VOID AddEventMessage( TEventMessage message );
	//���b�Z�[�W�̃X�g�b�v
	VOID StopEventMessage( );

	//�C�x���g�X�V
	VOID Update();

	//����
	STATIC LPTkyEventManager CreateEventManager();

	//���u����
	//�C�x���g���X�i�[�̒ǉ�
	STATIC bool StaticAddEventLisner( LPTkyEventLisner Event);
	//�C�x���g���X�i�[�̏���
	STATIC bool StaticDeleteEventLisner( LPTkyEventLisner Event);

	//�C�x���g���b�Z�[�W�[�̒ǉ�
	STATIC VOID StaticAddEventMessage( TEventMessage message );
	//�C�x���g���b�Z�[�W�[�̒ǉ�
	STATIC VOID StaticAddEventMessage( string str , vector<DWORD> ele );
	//�C�x���g���b�Z�[�W�[�̒ǉ�
	STATIC VOID StaticAddEventMessage( string str , DWORD messe );
	//�C�x���g���b�Z�[�W�[�̒�~
	STATIC VOID StaticStopEventMessage(  );

};








