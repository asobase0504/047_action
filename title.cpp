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
#include "score.h"

#include "time.h"

#include <assert.h>

// �}�N����`
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)

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
	OBJ_BG,			// �X�N���[���w�i
	OBJ_SELECTBG,	// �I����ʔw�i
	OBJ_TITLE,		// �^�C�g������
	OBJ_GAMESTART,	// GAMESTART����
	OBJ_TUTORIAL,	// TUTORIAL����
	OBJ_EXIT,		// EXIT����
	OBJ_MAX			// 
}OBJ_TYPE;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 Tex;				// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;					// �ʒu
	D3DXCOLOR col;						// �F
	bool bUse;							// �g�p���Ă邩�ǂ���
	float Height;						// ����
	float Width;						// ��
}OBJECT;

// �O���[�o���ϐ�
static OBJECT s_Object[OBJ_MAX] = {};
static bool	s_bFadeCheek;	// �t�F�[�h���u�Ɉڍs���邩�̏���
static int	s_nFadeCnt;		// �t�F�[�h�����ɍs���܂ł̊Ԋu

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
		&s_Object[OBJ_BG].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTBG,
		&s_Object[OBJ_SELECTBG].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		TITLE,
		&s_Object[OBJ_TITLE].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		GAMESTART,
		&s_Object[OBJ_GAMESTART].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL,
		&s_Object[OBJ_TUTORIAL].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		EXIT,
		&s_Object[OBJ_EXIT].Tex);

	s_bFadeCheek = false;	// �t�F�[�h�����Ɉڍs���邩�̕ϐ�
	s_nFadeCnt = 0;			// �t�F�[�h�����Ɉڍs����܂ł̊Ԋu

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	for (int i = 0; i < OBJ_MAX; i++)
	{
		OBJECT *object = &(s_Object[i]);

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&object->pVtxBuff,
			NULL);

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		object->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		switch (i)
		{
		case OBJ_BG:
			object->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(1.0f, 0.9f, 0.8f, 1.0f);	// �J���[�̐ݒ�
			object->Width = SCREEN_WIDTH;						// ���̐ݒ�
			object->Height = SCREEN_HEIGHT;						// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_SELECTBG:
			object->pos = D3DXVECTOR3(SCREEN_WIDTH - 350.0f, SCREEN_HEIGHT, 0.0f);		// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f);							// �J���[�̐ݒ�
			object->Width = 1000.0f;													// ���̐ݒ�
			object->Height = 1000.0f;													// �����̐ݒ�
			
			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_TITLE:
			object->pos = D3DXVECTOR3(40.0f, 30.0f, 0.0f);		// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 1380.0f;							// ���̐ݒ�
			object->Height = 415.0f;							// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_GAMESTART:
			object->pos = D3DXVECTOR3(1000.0f, 560.0f, 0.0f);	// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 780.0f;								// ���̐ݒ�
			object->Height = 140.0f;							// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_TUTORIAL:
			object->pos = D3DXVECTOR3(1000.0f, 720.0f, 0.0f);	// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 615.0f;								// ���̐ݒ�
			object->Height = 140.0f;							// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_EXIT:
			object->pos = D3DXVECTOR3(1000.0f, 880.0f, 0.0f);	// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 270.0f;								// ���̐ݒ�
			object->Height = 140.0f;							// �����̐ݒ�
			
			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_MAX:
		default:
			assert(false);
			break;
		}

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(object->col));

		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);

		// ���_�o�b�t�@���A�����b�N����
		s_Object[i].pVtxBuff->Unlock();

		// �g�p�ɐ؂�ւ�
		object->bUse = true;
	}
	InitTime();
}

//=========================================
// �I������
//=========================================
void UninitTitle(void)
{
	// �T�E���h�̒�~
	StopSound();

	// �e�N�X�`���̔j��
	for (int i = 0; i < OBJ_MAX; i++)
	{
		if (s_Object[i].Tex != NULL)
		{
			s_Object[i].Tex->Release();
			s_Object[i].Tex = NULL;
		}

	}

	// ���_�o�b�t�@�̔j��
	for (int i = 0; i < OBJ_MAX; i++)
	{
		if (s_Object[i].pVtxBuff != NULL)
		{
			s_Object[i].pVtxBuff->Release();
			s_Object[i].pVtxBuff = NULL;
		}
	}
}

//=========================================
// �X�V����
//=========================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	for (int i = 0; i < OBJ_MAX; i++)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_Object[i].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


		// ���_�o�b�t�@���A�����b�N����
		s_Object[i].pVtxBuff->Unlock();
	}

	// �Q�[�����[�h�Ɉڍs
	if (!(s_bFadeCheek))
	{
		if (GetJoypadTrigger(JOYKEY_A) || GetKeyboardTrigger(DIK_RETURN))
		{// Enter�L�[���p�b�h��A�������ꂽ��
		 // ���艹�̍Đ�
			PlaySound(SOUND_LABEL_SE_ENTER);
			s_bFadeCheek = true;	// �t�F�[�h�����ɓ���
		}
	}
	else if (s_bFadeCheek)
	{
		s_nFadeCnt = 0;
		SetFade(MODE_GAME);	// �Q�[�����[�h�Ɉڍs
	}
}

//=========================================
// �`�揈��
//=========================================
void DrawTitle(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < OBJ_MAX; i++)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		InitDraw(pDevice, s_Object[i].pVtxBuff);

		if (s_Object[i].bUse == true)
		{
			// �|���S���`��
			// �e�N�X�`���̐ݒ�
			SetDraw(pDevice, s_Object[i].Tex, 0);
		}
	}

}