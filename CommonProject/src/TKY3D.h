#pragma once

#include<d3dx9.h>
#include<vector>
#include "TKYBASEHEADER.h"
#include "TKYLIBRARY.h"
#include "TKYPRIMITIVE.h"

using namespace std;
/*
//--------------------------------------------------//
//��`�̈�
//--------------------------------------------------//
//���_�o�b�t�@�[�֌W
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZW|D3DFVF_DIFFUSE)
typedef struct{

	FLOAT x,y,z,rhw;	//�X�N���[�����W�ł̈ʒu
	DWORD color;		//���_�F

}CUSTOMVERTEX;
*/
//--------------------------------------------------//
//DirectX�̏��������s��
//--------------------------------------------------//
CLASS_POINTA_OBJECT(Init3D,LPTKY3D){

private:

	LPDIRECT3D9 g_pD3D;										//DirectX9�@�C���^�[�t�F�C�X
	LPDIRECT3DDEVICE9 g_pD3DDevice;							//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X

	static vector<Init3D*>  m_vIndex;

	Init3D(){ g_pD3D = NULL ; g_pD3DDevice = NULL ; m_primitive = NULL; }
	VIRTUAL ~Init3D()
	{
		clean3D();
	};

	//�F
	DWORD m_Color;
	Vector m_vColor;

	//�v���~�e�B�u�f�[�^
	LPTkyPrimitive m_primitive;

	//�V���O���g��
	STATIC LPTKY3D m_single3D;

public:

	//DirectX��֐�
	HRESULT Create3DX(HWND hWnd);

	void clean3D();

	//�`�揈���֐�
	HRESULT StartDraw();
	void EndDraw();
	void Clear(int type,UINT color); //D3DCLEAR_TARGET , D3DCOLOR_ARGB(255,255,255,255)

	//���{���֐�
	LPDIRECT3D9 GetD3D(){return g_pD3D;};					//DirectX9�@�C���^�[�t�F�C�X
	LPDIRECT3DDEVICE9 GetD3DDevice(){return g_pD3DDevice;};	//DirectX9�@�C���^�[�t�F�C�X�̃f�o�C�X

	//��ʂ̑傫���̎擾
	Vector GetScreenSize()
	{
		RECT r;
		g_pD3DDevice->GetScissorRect(&r);
		return Vector( (float)r.right , (float)r.bottom , 0 );
	}

	//�ʏ�`��
	BOOL SetColor(Vector);
	BOOL DrawLine(Vector,Vector,float);
	BOOL DrawLine3D(Vector,Vector,float);
	BOOL DrawSquare(Vector,Vector,Vector,Vector);
	BOOL DrawCircle(Vector p1,float r,int k);

	//3D�̎擾
//	static Init3D *CreateInit3D(){Init3D *i = new Init3D();m_vIndex.push_back(i); return i; }
//	static VOID Destroy(){FOR_I(m_vIndex.size())SAFE_DELETE(m_vIndex[i]);}
	static Init3D *CreateInit3D()
	{
		IF(m_single3D==NULL)m_single3D = new Init3D();
		return m_single3D;
	}

};
