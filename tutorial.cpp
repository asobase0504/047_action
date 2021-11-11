//=========================================
// 
// �`���[�g���A���̏���
// Author YudaKaito
// 
//=========================================
#include "tutorial.h"
#include "input.h"
#include <assert.h>
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "bg.h"
#include "block.h"
#include "score.h"
#include "sound.h"
#include "setup.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define MOVEWORD			"data/TEXTURE/word/move.png"
#define JUMPWORD			"data/TEXTURE/word/jump.png"
#define SELECTWORD			"data/TEXTURE/word/select.png"
#define BACKWORD			"data/TEXTURE/word/Back.png"
#define MOVEMETHOD_KEY		"data/TEXTURE/word/AorD.png"
#define JUMPMETHOD_KEY		"data/TEXTURE/word/spase.png"
#define SELECTMETHOD_KEY	"data/TEXTURE/word/enter.png"
#define MOVEMETHOD_PAD		"data/TEXTURE/word/stick.png"
#define JUMPMETHOD_PAD		"data/TEXTURE/word/AorB.png"
#define SELECTMETHOD_PAD	"data/TEXTURE/word/AorB.png"
#define BACK_PAD			"data/TEXTURE/word/XorY.png"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureMoveWord = NULL;		// �e�N�X�`���ւ̃|�C���^	(�ړ�����)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffMoveWord = NULL;		// ���_�o�b�t�@�ւ̃|�C���^	(�ړ�����)
static LPDIRECT3DTEXTURE9		s_pTextureMoveMethod[2] = {};	// �e�N�X�`���ւ̃|�C���^	(�ړ����@����)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffMoveMethod = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(�ړ����@����)
static LPDIRECT3DTEXTURE9		s_pTextureJumpWord = NULL;		// �e�N�X�`���ւ̃|�C���^	(��������)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffJumpWord = NULL;		// ���_�o�b�t�@�ւ̃|�C���^	(��������)
static LPDIRECT3DTEXTURE9		s_pTextureJumpMethod[2] = {};	// �e�N�X�`���ւ̃|�C���^	(�������@����)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffJumpMethod = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(�������@����)
static LPDIRECT3DTEXTURE9		s_pTextureSelectWord = NULL;	// �e�N�X�`���ւ̃|�C���^	(���蕶��)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelectWord = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(���蕶��)
static LPDIRECT3DTEXTURE9		s_pTextureSelectMethod[2] = {};	// �e�N�X�`���ւ̃|�C���^	(������@����)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelectMethod = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(������@����)
static LPDIRECT3DTEXTURE9		s_pTextureBackWord = NULL;		// �e�N�X�`���ւ̃|�C���^	(���蕶��)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBackWord = NULL;		// ���_�o�b�t�@�ւ̃|�C���^	(���蕶��)
static LPDIRECT3DTEXTURE9		s_pTextureBackMethod[2] = {};	// �e�N�X�`���ւ̃|�C���^	(������@����)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBackMethod = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(������@����)

//=========================================
// ����������
//=========================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		MOVEWORD,
		&s_pTextureMoveWord);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		MOVEMETHOD_KEY,
		&s_pTextureMoveMethod[0]);
	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		MOVEMETHOD_PAD,
		&s_pTextureMoveMethod[1]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		JUMPWORD,
		&s_pTextureJumpWord);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		JUMPMETHOD_KEY,
		&s_pTextureJumpMethod[0]);
	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		JUMPMETHOD_PAD,
		&s_pTextureJumpMethod[1]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTWORD,
		&s_pTextureSelectWord);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTMETHOD_KEY,
		&s_pTextureSelectMethod[0]);
	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTMETHOD_PAD,
		&s_pTextureSelectMethod[1]);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		BACKWORD,
		&s_pTextureBackWord);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTMETHOD_KEY,
		&s_pTextureBackMethod[0]);
	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		BACK_PAD,
		&s_pTextureBackMethod[1]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMoveWord,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMoveMethod,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffJumpWord,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffJumpMethod,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSelectWord,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSelectMethod,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBackWord,
		NULL);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBackMethod,
		NULL);

	VERTEX_2D *pVtx;

	// �ړ�����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMoveWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(656.0f, 200.0f, 0.0f), 256, 64);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMoveWord->Unlock();

	// �ړ����@����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMoveMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(1256.0f, 200.0f, 0.0f), 256, 64);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMoveMethod->Unlock();

	// ��������
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffJumpWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(656.0f, 400.0f, 0.0f), 256, 64);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffJumpWord->Unlock();

	// �������@����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffJumpMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(1256.0f, 400.0f, 0.0f), 256, 64);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffJumpMethod->Unlock();


	// �I�𕶎�
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffSelectWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(656.0f, 600.0f, 0.0f), 256, 64);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffSelectWord->Unlock();

	// �I����@����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffSelectMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(1256.0f, 600.0f, 0.0f), 256, 64);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffSelectMethod->Unlock();

	// �߂镶��
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBackWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(1278.0f, 950.0f, 0.0f), 128, 32);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBackWord->Unlock();

	// �߂���@����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBackMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(1478.0f, 950.0f, 0.0f), 128, 32);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBackMethod->Unlock();

	// �w�i�̏���������
	InitBG();
	// �G�t�F�N�g�̏���������
	InitEffect();
	// �p�[�e�B�N���̏���������
	InitParticle();
	// �v���C���[�̏���������
	InitPlayer();
	// �u���b�N�̏���������
	InitBlock();
	// �G�l�~�[�̏���������
	InitEnemy();

	// �u���b�N�̐ݒ�
	SetBlock(D3DXVECTOR3(300.0f,800.0f,0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(950.0f,25.0f,0.0f), 25.0f, 650.0f);
	SetBlock(D3DXVECTOR3(1600.0f,800.0f,0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(950.0f, SCREEN_HEIGHT - 25.0f, 0.0f), 25.0f, 650.0f);

	// �X�R�A�̏���������
	InitScore();

	// ���y�̍Đ�
	PlaySound(SOUND_LABEL_BGM001);
}

//=========================================
// �I������
//=========================================
void UninitTutorial(void)
{
	// �e�N�X�`���̔j��
	if (s_pTextureMoveWord != NULL)
	{
		s_pTextureMoveWord->Release();
		s_pTextureMoveWord = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffMoveWord != NULL)
	{
		s_pVtxBuffMoveWord->Release();
		s_pVtxBuffMoveWord = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		// �e�N�X�`���̔j��
		if (s_pTextureMoveMethod[i] != NULL)
		{						
			s_pTextureMoveMethod[i]->Release();
			s_pTextureMoveMethod[i] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffMoveMethod != NULL)
	{
		s_pVtxBuffMoveMethod->Release();
		s_pVtxBuffMoveMethod = NULL;
	}

	// �e�N�X�`���̔j��
	if (s_pTextureJumpWord != NULL)
	{
		s_pTextureJumpWord->Release();
		s_pTextureJumpWord = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffJumpWord != NULL)
	{
		s_pVtxBuffJumpWord->Release();
		s_pVtxBuffJumpWord = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		// �e�N�X�`���̔j��
		if (s_pTextureJumpMethod[i] != NULL)
		{
			s_pTextureJumpMethod[i]->Release();
			s_pTextureJumpMethod[i] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffJumpMethod != NULL)
	{
		s_pVtxBuffJumpMethod->Release();
		s_pVtxBuffJumpMethod = NULL;
	}

	// �e�N�X�`���̔j��
	if (s_pTextureSelectWord != NULL)
	{
		s_pTextureSelectWord->Release();
		s_pTextureSelectWord = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffSelectWord != NULL)
	{
		s_pVtxBuffSelectWord->Release();
		s_pVtxBuffSelectWord = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		// �e�N�X�`���̔j��
		if (s_pTextureSelectMethod[i] != NULL)
		{
			s_pTextureSelectMethod[i]->Release();
			s_pTextureSelectMethod[i] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffSelectMethod != NULL)
	{
		s_pVtxBuffSelectMethod->Release();
		s_pVtxBuffSelectMethod = NULL;
	}

	// �w�i�̏I������
	UninitBG();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �v���C���[�̏I������
	UninitPlayer();

	// �u���b�N�̏I������
	UninitBlock();

	// �G�l�~�[�̏I������
	UninitEnemy();
}

//=========================================
// �X�V����
//=========================================
void UpdateTutorial(void)
{
	// �w�i�̍X�V����
	UpdateBG();

	// �G�t�F�N�g�̍X�V����
	UpdateEffect();

	// �p�[�e�B�N���̍X�V����
	UpdateParticle();

	// �v���C���[�̍X�V����
	UpdatePlayer();

	// �u���b�N�̍X�V����
	UpdateBlock();

	// �G�l�~�[�̍X�V����
	UpdateEnemy();

	if (GetJoypadTrigger(JOYKEY_Y) || GetJoypadTrigger(JOYKEY_X) || GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_I))
	{
		SetEnemy(D3DXVECTOR3(1000.0f,500.0f,0.0f), BALL_HOMING01);
	}
}

//=========================================
// �`�揈��
//=========================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �w�i�̕`�揈��
	DrawBG();

	// �ړ�����
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffMoveWord);
	// �`�揈��
	RectDraw(pDevice, s_pTextureMoveWord, 0);
	// �ړ����@����
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffMoveMethod);
	// �`�揈��
	RectDraw(pDevice, s_pTextureMoveMethod[GetUseJoyPad()], 0);

	// ��������
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffJumpWord);
	// �`�揈��
	RectDraw(pDevice, s_pTextureJumpWord, 0);
	// �������@����
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffJumpMethod);
	// �`�揈��
	RectDraw(pDevice, s_pTextureJumpMethod[GetUseJoyPad()], 0);
	
	// �I�𕶎�
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffSelectWord);
	// �`�揈��
	RectDraw(pDevice, s_pTextureSelectWord, 0);
	// �I����@����
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffSelectMethod);
	// �`�揈��
	RectDraw(pDevice, s_pTextureSelectMethod[GetUseJoyPad()], 0);

	// �߂镶��
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffBackWord);
	// �`�揈��
	RectDraw(pDevice, s_pTextureBackWord, 0);
	// �߂���@����
	// �`�ʏ����ɑO��Ƃ��ĕK�v�ȏ���
	InitDraw(pDevice, s_pVtxBuffBackMethod);
	// �`�揈��
	RectDraw(pDevice, s_pTextureBackMethod[GetUseJoyPad()], 0);

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();
	// �p�[�e�B�N���̕`�揈��
	DrawParticle();
	// �v���C���[�̕`�揈��
	DrawPlayer();
	// �u���b�N�̕`�揈��
	DrawBlock();
	// �G�l�~�[�̕`�揈��
	DrawEnemy();
}

//=========================================
// �����\������
//=========================================
void TextTutorial(void)
{

}
