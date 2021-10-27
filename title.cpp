//=========================================
// 
// �^�C�g���̏���
// Author YudaKaito
// 
//=========================================
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "particle.h"
#include "ranking.h"
#include "setup.h"

#include <assert.h>

// �}�N����`
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)
#define MAX_TEXTURE			(5)

#define SELECTBG			"data/TEXTURE/enemy000.png"
#define TITLE				"data/TEXTURE/�^�C�g��.png"
#define GAMESTART			"data/TEXTURE/GAMESTART.png"
#define TUTORIAL			"data/TEXTURE/TUTORIAL.png"
#define EXIT				"data/TEXTURE/EXIT.png"

// �v���X�G���^�[�̏�Ԃ̎��
typedef enum
{
	TYPE_NOME,	// �ω��Ȃ�
	TYPE_BLINK,	// �_�ŏ��
	TYPE_MAX
}PRESS_TYPE;

// �z�u����I�u�W�F�N�g�̎��
typedef enum
{
	OBJ_BG,
	OBJ_SELECTBG,
	OBJ_TITLE,
	OBJ_GAMESTART,
	OBJ_TUTORIAL,
	OBJ_EXIT,
	OBJ_MAX
}OBJ_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	bool bUse;			// �g�p���Ă邩�ǂ���
}TitleObj;

// �O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 s_pTexture[OBJ_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};			// ���_�o�b�t�@�ւ̃|�C���^
static TitleObj s_Tex[MAX_TEXTURE] = {};
static PRESS_TYPE s_presstype;
static TitleObj press;
static int s_nTypeCnt;	// �k���Ɗg��̊Ԋu
static bool s_bFadeCheek;	// �t�F�[�h���u�Ɉڍs���邩�̏���
static bool s_bPreassUse;
static int s_nFadeCnt;	// �t�F�[�h�����ɍs���܂ł̊Ԋu
static int s_nTimeTitle;

typedef enum {
	LIST_0,
	LIST_1,
	LIST_2,
	LIST_3,
	NUM_LIST
} LIST;
static const int INT_LIST[NUM_LIST] = {
	1,2,3,4
};

//=========================================
// ����������
//=========================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	int nTexCnt;

	// ���y�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[OBJ_BG]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTBG,
		&s_pTexture[OBJ_SELECTBG]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		TITLE,
		&s_pTexture[OBJ_TITLE]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		GAMESTART,
		&s_pTexture[OBJ_GAMESTART]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL,
		&s_pTexture[OBJ_TUTORIAL]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		EXIT,
		&s_pTexture[OBJ_EXIT]);

	// �v���X�^�C�g���̏���������
	s_presstype = TYPE_NOME;						// �_�ŏ��
	s_nTypeCnt = 0;									// �g��Ǝ��k�̃^�C�~���O
	press.pos = D3DXVECTOR3(80.0f, 575.0f, 0.0f);	// ���W
	press.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �J���[
	press.bUse = false;								// �v���X���g�p���Ă���ۂ�

	s_bFadeCheek = false;							// �t�F�[�h�����Ɉڍs���邩�̕ϐ�
	s_nFadeCnt = 0;									// �t�F�[�h�����Ɉڍs����܂ł̊Ԋu
	s_nTimeTitle = 0;

	// �^�C�g����ʂ̏���������
	for (nTexCnt = 0; nTexCnt < OBJ_MAX; nTexCnt++)
	{
		s_Tex[nTexCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Tex[nTexCnt].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		s_Tex[nTexCnt].bUse = true;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * OBJ_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < OBJ_MAX; i++)
	{

		switch (i)
		{
		case OBJ_BG:
			// ���_���W�̐ݒ�
			pVtx[0].pos.x = 0.0f;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SCREEN_WIDTH;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 0.0f;
			pVtx[2].pos.y = SCREEN_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SCREEN_WIDTH;
			pVtx[3].pos.y = SCREEN_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			s_Tex[i].bUse = true;
			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 0.9f, 0.8f, 1.0f)));
			break;
		case OBJ_SELECTBG:
			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH - 350.0f, SCREEN_HEIGHT, 0.0f), 1000.0f, 1000.0f);

			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f)));
			break;
		case OBJ_TITLE:
			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(40.0f, 30.0f, 0.0f), 1380.0f, 415.0f);

			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f)));
			break;
		case OBJ_GAMESTART:
			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(1000.0f, 560.0f, 0.0f), 780.0f, 140.0f);
			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)));
			break;
		case OBJ_TUTORIAL:
			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(1000.0f, 720.0f, 0.0f), 615.0f, 140.0f);
			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)));
			break;
		case OBJ_EXIT:
			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(1000.0f, 880.0f, 0.0f), 270.0f, 140.0f);
			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)));
			break;
		case OBJ_MAX:
		default:
			assert(false);
			break;
		}

		// rhw�̐ݒ�
		InitRectRhw(pVtx);


		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
// �I������
//=========================================
void UninitTitle(void)
{
	int nCnt;
	// �T�E���h�̒�~
	StopSound();

	for (nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}

	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// �X�V����
//=========================================
void UpdateTitle(void)
{
	int nTexCnt;
	s_nTimeTitle++;

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	//// 
	//// �^�C�g���̉摜
	//// 
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//if (s_Tex[4].col.a >= 1.0f && s_Tex[1].col.a <= 1.0f)
	//{// �^�C�g���̉e�̃t�F�[�h
	//	s_Tex[1].col.a += 0.001f;
	//}
	//if (s_Tex[4].col.a <= 1.0f)
	//{// �^�C�g���̃t�F�[�h
	//	s_Tex[4].col.a += 0.005f;
	//}

	//for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	//{
	//	SetRectColor(pVtx, &(s_Tex[nTexCnt].col));
	//	// ���_�J���[�̐ݒ�
	//	pVtx += 4;
	//}

	// �Q�[�����[�h�Ɉڍs
	if (s_bFadeCheek == false)
	{
		if (GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN) == true)
		{// Enter�L�[���p�b�h��B�������ꂽ��
		 // ���艹�̍Đ�
			PlaySound(SOUND_LABEL_SE_ENTER);
			s_bFadeCheek = true;	// �t�F�[�h�����ɓ���
		}

		//// �_�ŏ���
		//switch (s_presstype)
		//{
		//case TYPE_NOME:		// �_�����
		//	// ���_�J���[�̐ݒ�
		//	SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		//	if (s_nTypeCnt == 50)
		//	{	// ������ԂɈڍs
		//		s_presstype = TYPE_BLINK;
		//		s_nTypeCnt = 0;
		//	}
		//	break;
		//case TYPE_BLINK:	// �������
		//	// ���_�J���[�̐ݒ�
		//	SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));
		//	if (s_nTypeCnt == 50)
		//	{	// �_����ԂɈڍs
		//		s_presstype = TYPE_NOME;
		//		s_nTypeCnt = 0;
		//	}
		//	break;
		//default:
		//	break;
		//}
	}
	else if (s_bFadeCheek == true)
	{
		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

		if (s_nFadeCnt % 5 == 0)
		{
			pVtx[0].pos.x -= 7.0f;
			pVtx[0].pos.y -= 1.0f;
			pVtx[0].pos.z += 0.0f;

			pVtx[1].pos.x += 7.0f;
			pVtx[1].pos.y -= 1.0f;
			pVtx[1].pos.z += 0.0f;

			pVtx[2].pos.x -= 7.0f;
			pVtx[2].pos.y += 1.0f;
			pVtx[2].pos.z += 0.0f;

			pVtx[3].pos.x += 7.0f;
			pVtx[3].pos.y += 1.0f;
			pVtx[3].pos.z += 0.0f;
		}
		if (s_nFadeCnt == 20)
		{
			s_presstype = TYPE_BLINK;
			s_nFadeCnt = 0;
			SetFade(MODE_GAME);	// �Q�[�����[�h�Ɉڍs
		}
		s_nFadeCnt++;
	}
	// �؂�ւ��^�C�~���O
	s_nTypeCnt++;

	//if (s_nTimeTitle >= 1500)
	//{
	//	SetFade(MODE_RANKING);
	//}
}

//=========================================
// �`�揈��
//=========================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nTexCnt;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�̕`��
	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice = InitDraw(pDevice, s_pVtxBuff);

	for (nTexCnt = 0; nTexCnt < OBJ_MAX; nTexCnt++)
	{
		if (s_Tex[nTexCnt].bUse == true)
		{// �e���g�p����Ă���
			// �|���S���`��
			// �e�N�X�`���̐ݒ�
			SetDraw(pDevice, s_pTexture[nTexCnt], nTexCnt * 4);
		}
	}
}