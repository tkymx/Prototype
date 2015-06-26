#pragma once

#include<d3dx9.h>
#include <dinput.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include"Anime.h"
#include"TKYHLSL.h"
#include"TKYXFILE.h"
#include"TKYXSHADOW.h"
#include"TKY3D.h"
#include "TKYDIRECTINPUT.h"
#include "TKY2DTEXTURE.h"
#include "TKYFONT.h"
#include "TKYLIBRARY.h"
#include "TKYVECTOR.h"

using namespace std;


class VertexBuffer;
class VertexShader;
//--------------------------------------------------//
//�����֐�
//--------------------------------------------------//
VOID AllDestroy();

//--------------------------------------------------//
//���_�o�b�t�@������
//--------------------------------------------------//
class VertexBuffer
{

private:

	LPDIRECT3DVERTEXBUFFER9 g_pVB;

public:

	//�R���g���X�^
	VertexBuffer();

	//���_�o�b�t�@�̏�����
	HRESULT Initialize(LPDIRECT3DDEVICE9 pD3DDev);

	//�`��֐�
	VOID Update(LPDIRECT3DDEVICE9 pD3DDev);

	//�J������
	VOID Close();

};
//--------------------------------------------------//
//���_�o�b�t�@������
//--------------------------------------------------//
class VertexShader
{

private:

	//���_�V�F�[�_
	LPDIRECT3DVERTEXSHADER9 m_pVS;
	//���_�錾
	LPDIRECT3DVERTEXDECLARATION9 m_pVD;
	//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X
	LPDIRECT3DDEVICE9 m_pD3DDev;							

public:

	//�R���g���X�^
	VertexShader(LPDIRECT3DDEVICE9 a_pD3DDev);

	//���_�V�F�[�_�̏�����
	HRESULT InitShader(LPCTSTR Filename);

	//���_�V�F�[�_�̒萔�ݒ�
	VOID SetShader();

	//�J������
	VOID Close();

};


