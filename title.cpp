//=========================================
//
//�^�C�g���̏���
//Author YudaKaito
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

//�}�N����`
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)
#define MAX_TEXTURE			(5)

//�v���X�G���^�[�̏�Ԃ̎��
typedef enum
{
	TYPE_NOME,	//�ω��Ȃ�
	TYPE_BLINK,	//�_�ŏ��
	TYPE_MAX
}PRESS_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	bool bUse;			//�g�p���Ă邩�ǂ���
}TitleObj;

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 s_pTextureTitle[MAX_TEXTURE] = {};		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffTitle = {};				//���_�o�b�t�@�ւ̃|�C���^
static TitleObj s_Tex[MAX_TEXTURE] = {};
static LPDIRECT3DTEXTURE9 s_pTextureTitlePress = NULL;			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffTitlePress = NULL;		//���_�o�b�t�@�ւ̃|�C���^
static PRESS_TYPE s_presstype;
static TitleObj press;
static int s_nTypeCnt;	//�k���Ɗg��̊Ԋu
static bool s_bFadeCheek;	//�t�F�[�h���u�Ɉڍs���邩�̏���
static bool s_bPreassUse;
static int s_nFadeCnt;	//�t�F�[�h�����ɍs���܂ł̊Ԋu
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
//����������
//=========================================
void InitTitle(void)
{
	//int sum = 0;
	//for (int i = 0; i < 5; ++i)
	//{
	//	assert(i >= 0 && i < NUM_LIST);
	//	sum += INT_LIST[i];
	//}

	//LIST hoge = LIST_3;
	//int aaa = 0;
	//switch (hoge)
	//{
	//case LIST_0:
	//	break;
	//case LIST_1:

	//	aaa = 2;

	//	break;
	//case LIST_2:

	//	aaa = 3;

	//	break;
	//default:
	//	assert(false);
	//	break;
	//}



	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���g
	int nTexCnt;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���y�̍Đ�
	PlaySound(SOUND_LABEL_BGM000);

	//�e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg003.jpg",
		&s_pTextureTitle[0]);

	//�e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg011.png",
		&s_pTextureTitle[1]);

	//�e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg012.png",
		&s_pTextureTitle[2]);

	//�e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg013.png",
		&s_pTextureTitle[3]);

	//�e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg014.png",
		&s_pTextureTitle[4]);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/press_enter01.png",
		&s_pTextureTitlePress);

	//�v���X�^�C�g���̏���������
	s_presstype = TYPE_NOME;						//�_�ŏ��
	s_nTypeCnt = 0;									//�g��Ǝ��k�̃^�C�~���O
	press.pos = D3DXVECTOR3(80.0f, 575.0f, 0.0f);	//���W
	press.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	//�J���[
	press.bUse = false;								//�v���X���g�p���Ă���ۂ�

	s_bFadeCheek = false;							//�t�F�[�h�����Ɉڍs���邩�̕ϐ�
	s_nFadeCnt = 0;									//�t�F�[�h�����Ɉڍs����܂ł̊Ԋu
	s_nTimeTitle = 0;

	//�^�C�g����ʂ̏���������
	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		s_Tex[nTexCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Tex[nTexCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		s_Tex[nTexCnt].bUse = true;
	}
	s_Tex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	s_Tex[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTitle,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTitlePress,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		switch (nTexCnt)
		{
		case 0:
			//���_���W�̐ݒ�
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

			s_Tex[nTexCnt].bUse = true;
			break;
		case 1:
			//���_���W�̐ݒ�
			pVtx[0].pos.x = 1.0f;
			pVtx[0].pos.y = 125.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = 1.0f + 1220.0f;
			pVtx[1].pos.y = 125.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 1.0f;
			pVtx[2].pos.y = 510.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = 1.0f + 1220.0f;
			pVtx[3].pos.y = 510.0f;
			pVtx[3].pos.z = 0.0f;
			break;
		case 2:
			//���_���W�̐ݒ�
			pVtx[0].pos.x = 510.0f;
			pVtx[0].pos.y = 130.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SCREEN_WIDTH;
			pVtx[1].pos.y = 130.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 510.0f;
			pVtx[2].pos.y = SCREEN_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SCREEN_WIDTH;
			pVtx[3].pos.y = SCREEN_HEIGHT;
			pVtx[3].pos.z = 0.0f;
			break;
		case 3:
			//���_���W�̐ݒ�
			pVtx[0].pos.x = 240.0f;
			pVtx[0].pos.y = 285.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = 1040.0f;
			pVtx[1].pos.y = 285.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 240.0f;
			pVtx[2].pos.y = 435.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = 1040.0f;
			pVtx[3].pos.y = 435.0f;
			pVtx[3].pos.z = 0.0f;
			break;
		case 4:
			//���_���W�̐ݒ�
			pVtx[0].pos.x = 200.0f;
			pVtx[0].pos.y = 230.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = 1110.0f;
			pVtx[1].pos.y = 230.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 200.0f;
			pVtx[2].pos.y = 520.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = 1110.0f;
			pVtx[3].pos.y = 520.0f;
			pVtx[3].pos.z = 0.0f;
			break;
		default:
			break;
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = s_Tex[nTexCnt].col;
		pVtx[1].col = s_Tex[nTexCnt].col;
		pVtx[2].col = s_Tex[nTexCnt].col;
		pVtx[3].col = s_Tex[nTexCnt].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffTitle->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffTitlePress->Lock(0, 0, (void**)&pVtx, 0);

	//�����`�̏�����
	//rhw�̐ݒ�
	//���_�J���[�̐ݒ�
	//�e�N�X�`�����W�̐ݒ�
	InitRect(pVtx);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = press.pos.x;
	pVtx[0].pos.y = press.pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = press.pos.x + TITLEPRESS_WIDTH;
	pVtx[1].pos.y = press.pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = press.pos.x;
	pVtx[2].pos.y = press.pos.y + TITLEPRESS_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = press.pos.x + TITLEPRESS_WIDTH;
	pVtx[3].pos.y = press.pos.y + TITLEPRESS_HEIGHT;
	pVtx[3].pos.z = 0.0f;


	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffTitlePress->Unlock();
}

//=========================================
//�I������
//=========================================
void UninitTitle(void)
{
	int nCnt;
	//�T�E���h�̒�~
	StopSound();

	for (nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureTitle[nCnt] != NULL)
		{
			s_pTextureTitle[nCnt]->Release();
			s_pTextureTitle[nCnt] = NULL;
		}

	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffTitle != NULL)
	{
		s_pVtxBuffTitle->Release();
		s_pVtxBuffTitle = NULL;
	}

	//�e�N�X�`���̔j��
	if (s_pTextureTitlePress != NULL)
	{
		s_pTextureTitlePress->Release();
		s_pTextureTitlePress = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffTitlePress != NULL)
	{
		s_pVtxBuffTitlePress->Release();
		s_pVtxBuffTitlePress = NULL;
	}
}

//=========================================
//�X�V����
//=========================================
void UpdateTitle(void)
{
	int nTexCnt;
	s_nTimeTitle++;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//
	//�^�C�g���̉摜
	//
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (s_Tex[4].col.a >= 1.0f && s_Tex[1].col.a <= 1.0f)
	{//�^�C�g���̉e�̃t�F�[�h
		s_Tex[1].col.a += 0.001f;
	}
	if (s_Tex[4].col.a <= 1.0f)
	{//�^�C�g���̃t�F�[�h
		s_Tex[4].col.a += 0.005f;
	}

	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		SetRectColor(pVtx, &(s_Tex[nTexCnt].col));
		//���_�J���[�̐ݒ�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffTitlePress->Unlock();

	//
	//�v���X�^�C�g��
	//
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffTitlePress->Lock(0, 0, (void**)&pVtx, 0);

	//�Q�[�����[�h�Ɉڍs
	if (s_bFadeCheek == false)
	{
		if (GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN) == true)
		{//Enter�L�[���p�b�h��B�������ꂽ��
		 //���艹�̍Đ�
			PlaySound(SOUND_LABEL_SE_ENTER);
			s_bFadeCheek = true;	//�t�F�[�h�����ɓ���
		}

		//�_�ŏ���
		switch (s_presstype)
		{
		case TYPE_NOME:		//�_�����
			//���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			if (s_nTypeCnt == 50)
			{	//������ԂɈڍs
				s_presstype = TYPE_BLINK;
				s_nTypeCnt = 0;
			}
			break;
		case TYPE_BLINK:	//�������
			//���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));
			if (s_nTypeCnt == 50)
			{	//�_����ԂɈڍs
				s_presstype = TYPE_NOME;
				s_nTypeCnt = 0;
			}
			break;
		default:
			break;
		}
	}
	else if (s_bFadeCheek == true)
	{
		//���_�J���[�̐ݒ�
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
			SetFade(MODE_GAME);	//�Q�[�����[�h�Ɉڍs
		}
		s_nFadeCnt++;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffTitlePress->Unlock();

	//�؂�ւ��^�C�~���O
	s_nTypeCnt++;

	if (s_nTimeTitle >= 1500)
	{
		SetFade(MODE_RANKING);
	}
}

//=========================================
//�`�揈��
//=========================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nTexCnt;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�w�i�̕`��
	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice = InitDraw(pDevice, s_pVtxBuffTitle);

	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		if (s_Tex[nTexCnt].bUse == true)
		{//�e���g�p����Ă���
			//�|���S���`��
			//�e�N�X�`���̐ݒ�
			pDevice = SetDraw(pDevice, s_pTextureTitle[nTexCnt], nTexCnt);
		}
	}

	//PRESSENTER�̕`��
	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice = InitDraw(pDevice, s_pVtxBuffTitlePress);
	//�|���S���`��
	pDevice = SetDraw(pDevice, s_pTextureTitlePress, 0);
}