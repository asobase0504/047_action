//=========================================
//
//���C���̃w�b�_�[�t�@�C��
//Author YudaKaito
//
//=========================================
#ifndef _MAIN_H_	//���̃}�N����`������ĂȂ�������
#define _MAIN_H_	//2�d�C���N���[�h�h�~�̃}�N����`


//------------------------------------
//include
//------------------------------------
#include <windows.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION	(0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"					//���y�����ɕK�v
#include "Xinput.h"						//�W���C�p�b�g�����ɕK�v

//------------------------------------
//���C�u�����̃����N
//------------------------------------
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v
#pragma comment(lib,"xinput.lib")	//�W���C�p�b�g�����ɕK�v

#define SCREEN_WIDTH	(1920)			//�E�B���h�E�̕�
#define SCREEN_HEIGHT	(1080)			//�E�C���h�E�̍���
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//------------------------------------
//����ʒu�̗񋓌^�̒�`
//------------------------------------
typedef enum
{
	POSITION_UP = 0,	//�u���b�N�̏��
	POSITION_DWON,		//�u���b�N�̒��
	POSITION_LEFT,		//�u���b�N�̍���
	POSITION_RIGHT,		//�u���b�N�̉E��
}JUDGE_POSITION;

//���(���[�h)�̎��
typedef enum
{
	MODE_TITLE = 0,	//�^�C�g�����
	MODE_GAME,		//�Q�[�����
	MODE_RESULT,	//���U���g���
	MODE_RANKING,	//�����L���O���
	MODE_MAX
}MODE;

//------------------------------------
//���_���[2D]�̍\���̂��`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���W�ϊ��p�W���i1.0f�ŌŒ�)
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`���̍��W
} VERTEX_2D;

//------------------------------------
//�v���g�^�C�v�錾
//------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void);
void SetRectColor(VERTEX_2D *vtx, D3DXCOLOR *inColor);
void SetRectTex(VERTEX_2D *vtx);
void SetRectPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fHeigth, float fWidth);
void SetupRectDefault(VERTEX_2D *vtx, D3DXCOLOR *inColor);
LPDIRECT3DDEVICE9 InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff);
LPDIRECT3DDEVICE9 AddSyntheticDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);
LPDIRECT3DDEVICE9 SetDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);
float D3DXVec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);												//�O�ς̌v�Z����
void SetMode(MODE mode);
MODE GetMode(void);

#endif	// !_MAIN_H_