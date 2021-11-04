//=========================================
// 
// �����L���O�̏���
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "ranking.h"
#include "score.h"
#include "fade.h"
#include <stdio.h>
#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define SELECT_NUMBER	(2)
#define SERECT_WIDTH	(200.0f)
#define SERECT_HEIGTH	(50.0f)

//------------------------------------
// �����L���O�̃X�e�[�^�X�񋓌^
//------------------------------------
typedef enum
{
	RANKSTATE_NONE = 0,
	RANKSTATE_NORMAL,
	RANKSTATE_END,
}RANKSTATE;

//------------------------------------
// �����L���O�X�R�A�\����
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	int			nScore; //�X�R�A
}RankScore;

//------------------------------------
// �O���[�o���ϐ�
//------------------------------------
static RANKSTATE s_RankState;	//�����L���O�����̏��
static LPDIRECT3DTEXTURE9		s_pTextureRank		= NULL;	// �e�N�X�`���ւ̃|�C���^	(�w�i)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRank		= NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(�w�i)
static LPDIRECT3DTEXTURE9		s_pTextureScore		= NULL;	// �e�N�X�`���ւ̃|�C���^	(�X�R�A)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffScore		= NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(�X�R�A)
static LPDIRECT3DTEXTURE9		s_pTextureBg		= NULL;	// �e�N�X�`���ւ̃|�C���^	(����)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBg		= NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(����)
static LPDIRECT3DTEXTURE9		s_pTextureMenu		= NULL;	// �e�N�X�`���ւ̃|�C���^	(���)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffMenu		= NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(���)
static LPDIRECT3DTEXTURE9		s_pTextureSelect[SELECT_NUMBER] = {};	// �e�N�X�`���ւ̃|�C���^	(�I��)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelect	= {};	// ���_�o�b�t�@�ւ̃|�C���^	(�I��)

//�����L���O�\���֘A�̕ϐ�
static RankScore	s_aRankScore[MAX_RANK];	// �����L���O�X�R�A���
static int			s_nRankUpdate = -1;		// �X�V�����NNo.
static int			s_nTimerRanking;		// �����L���O��ʕ\���^�C�}�[
static D3DXCOLOR	s_Timercol;				// ���Ŏ��̃J���[�̐ݒ�
static bool			s_bBlink;				// �X�R�A�̖��ł̐؂�ւ�
static int			s_nTimerBlink;			// �X�R�A�̖��ł̊Ԋu

//���j���[��ʑS�̊֘A�̕ϐ�
static float		s_fWidthMenu;	// ���j���[��ʂ̕�
static float		s_fHeigthMenu;	// ���j���[��ʂ̍���
static int			s_MenuCnt;		// ���j���[��ʂ̃J�E���g

//select�֘A�̕ϐ�
static D3DXCOLOR	s_Selectcol[SELECT_NUMBER];	// select���J���[�̐ݒ�
static int			s_SelectCheck;				// select�����`�F�b�N

//=========================================
// �����L���O�̏���������
//=========================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;
	int nCntRank;
	int nCntScore;
	s_nTimerRanking = 0;
	s_MenuCnt = 33;
	s_bBlink = true;
	s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	s_RankState  = RANKSTATE_NORMAL;

	s_fWidthMenu = 0;
	s_fHeigthMenu = 0;
	s_SelectCheck = 0;

	// �e�N�X�`���̓Ǎ� (�w�i)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureBg);

	// �e�N�X�`���̓Ǎ� (���)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureMenu);

	// �e�N�X�`���̓Ǎ� (����)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking_rank1.png",
		&s_pTextureRank);

	// �e�N�X�`���̓Ǎ� (�X�R�A)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&s_pTextureScore);

	// �e�N�X�`���̓Ǎ� (�I���F������x)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureSelect[0]);

	// �e�N�X�`���̓Ǎ� (�I���F�I��)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureSelect[1]);

	// ���_�o�b�t�@�̐��� (�w�i)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBg,
		NULL);

	// ���_�o�b�t�@�̐��� (���)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMenu,
		NULL);

	// ���_�o�b�t�@�̐��� (����)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRank,
		NULL);

	// ���_�o�b�t�@�̐��� (�X�R�A)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffScore,
		NULL);

	// ���_�o�b�t�@�̐��� (�I��)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SELECT_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSelect,
		NULL);

	// �w�i
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);
	{
		//�����`�̏�����
		InitRect(pVtx);

		// ���_���W�̐ݒ�
		SetRectUpLeftPos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f)));
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffBg->Unlock();

	// ���j���[���
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), s_fWidthMenu, s_fHeigthMenu);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.9f)));

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);

		// rhw�̐ݒ�
		InitRectRhw(pVtx);
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffMenu->Unlock();

	// ���ʂ̏�����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
	{
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			// ���ʂ̏ꏊ������
			s_aRankScore[nCntRank].pos = D3DXVECTOR3(650.0f, 200.0f + nCntRank * 150.0f, 0.0f);

			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, s_aRankScore[nCntRank].pos, 75.0f, 75.0f);

			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f)));

			// �e�N�X�`�����W�̐ݒ�
			SetRectTex(pVtx, 0.0f + 0.2f * nCntRank, 0.2f + 0.2f * nCntRank, 0.0f, 1.0f);

			// rhw�̐ݒ�
			InitRectRhw(pVtx);

			pVtx += 4;
		}
	}
	s_pVtxBuffRank->Unlock();	// ���_�o�b�t�@���A�����b�N����

	// �X�R�A�̏�����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	{
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			RankScore *pScore = &(s_aRankScore[nCntRank]);
			pScore->pos.x += 600;	// �X�R�A�\���ꏊ�ֈړ�
			pScore->pos.y -= 40;	// �X�R�A�\���ꏊ�ֈړ�
			for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
			{
				//���_���W�̐ݒ�
				SetRectUpRightPos(pVtx, D3DXVECTOR3(pScore->pos.x - nCntScore * 65, pScore->pos.y, pScore->pos.z), 60.0f, 70.0f);

				// ���_�J���[�̐ݒ�
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f)));

				// �e�N�X�`�����W�̐ݒ�
				SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

				// rhw�̐ݒ�
				InitRectRhw(pVtx);

				pVtx += 4;
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffScore->Unlock();

	// �I�����
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);
	{
		for (int i = 0; i < 2; i++)
		{
			// ���_���W�̐ݒ�
			SetRectUpRightPos(pVtx, D3DXVECTOR3(800.0f + 400.0f * i, 950.0f, 0.0f), SERECT_WIDTH, SERECT_HEIGTH);

			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));

			// �e�N�X�`�����W�̐ݒ�
			InitRectTex(pVtx);

			// rhw�̐ݒ�
			InitRectRhw(pVtx);
			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffSelect->Unlock();

}

//=========================================
// �����L���O�̏I������
//=========================================
void UninitRanking(void)
{

	// �e�N�X�`���̔j��
	if (s_pTextureRank != NULL)
	{
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffRank != NULL)
	{
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	// �e�N�X�`���̔j��
	if (s_pTextureScore != NULL)
	{
		s_pTextureScore->Release();
		s_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffScore != NULL)
	{
		s_pVtxBuffScore->Release();
		s_pVtxBuffScore = NULL;
	}

	// �e�N�X�`���̔j��
	if (s_pTextureBg != NULL)
	{
		s_pTextureBg->Release();
		s_pTextureBg = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffBg != NULL)
	{
		s_pVtxBuffBg->Release();
		s_pVtxBuffBg = NULL;
	}

	// �e�N�X�`���̔j��
	if (s_pTextureMenu != NULL)
	{
		s_pTextureMenu->Release();
		s_pTextureMenu = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffMenu != NULL)
	{
		s_pVtxBuffMenu->Release();
		s_pVtxBuffMenu = NULL;
	}

	// �e�N�X�`���̔j��
	for (int i = 0; i < 2; i++)
	{
		if (s_pTextureSelect[i] != NULL)
		{
			s_pTextureSelect[i]->Release();
			s_pTextureSelect[i] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuffSelect != NULL)
	{
		s_pVtxBuffSelect->Release();
		s_pVtxBuffSelect = NULL;
	}

}

//=========================================
// �����L���O�̍X�V����
//=========================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;
	
	switch (s_RankState)
	{
	case RANKSTATE_NONE:
		break;
	case RANKSTATE_NORMAL:
		s_nTimerRanking++;
		s_nTimerBlink--;

		// �_�ł̐؂�ւ�����
		if (s_nTimerBlink <= 0 && !(s_bBlink))
		{
			s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);
			s_nTimerBlink = 40;
			s_bBlink = true;

		}
		if (s_nTimerBlink <= 0 && s_bBlink)
		{
			s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			s_nTimerBlink = 40;
			s_bBlink = false;
		}


		// ���j���[��ʂ̍X�V����
		UpdateMenu();

		if (s_MenuCnt <= -10.0f)
		{
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
			for (int i = 0; i < MAX_RANK; i++)
			{
				// ���_�J���[�̐ݒ�
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

				pVtx += 4;
			}
			// ���_�o�b�t�@���A�����b�N����
			s_pVtxBuffRank->Unlock();

			// �X�R�A�̍X�V
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
			for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
			{
				// ����X�V���ꂽ�����L���O���ۂ�
				if (nCntRank == s_nRankUpdate)
				{	// ����̃X�R�A�̓_��
					for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
					{
						// ���_�J���[�̐ݒ�
						SetRectColor(pVtx, &(s_Timercol));

						pVtx += 4;
					}
				}
				else
				{	// �ʏ��Ԃ̐F
					for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
					{
						// ���_�J���[�̐ݒ�
						SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
						pVtx += 4;
					}
				}
			}
			// ���_�o�b�t�@���A�����b�N����
			s_pVtxBuffScore->Unlock();
		}

		//�I���̐؂�ւ�
		if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_D))
		{
			s_SelectCheck = s_SelectCheck ? 0 : 1;
		}

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);
		for (int i = 0; i < SELECT_NUMBER; i++)
		{
			if (i == s_SelectCheck)
			{
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
			}
			else
			{
				SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)));
			}
			pVtx += 4;
		}
		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuffSelect->Unlock();

		switch (s_SelectCheck)
		{
		case 0:
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				SetGameState(GAMESTATE_NONE);
			}

			break;
		case 1:
			// ���Ԑ����������́A�G���^�[�L�[�Ń��U���g��ʂɈڍs
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				//������
				s_RankState = RANKSTATE_NONE;
				s_MenuCnt = 33;
				s_fHeigthMenu = 0.0f;
				s_fWidthMenu = 0.0f;

				// ���艹�̍Đ�
				PlaySound(SOUND_LABEL_SE_ENTER);

				// ���U���g��ʂɈڍs
				SetFade(MODE_RESULT);
			}
			break;
		default:
			assert(false);
			break;
		}
		break;
	case RANKSTATE_END:
		break;
	default:
		assert(false);
		break;
	}
}

//=========================================
// ���j���[�̍X�V����
//=========================================
void UpdateMenu(void)
{
	VERTEX_2D *pVtx;

	// ���j���[�̃o�b�t�@�[
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾

	//������x�k��������g��E�k�����~
	if (!(s_MenuCnt <= -10))
	{
		s_MenuCnt -= 1;

		s_fWidthMenu += s_MenuCnt;
		s_fHeigthMenu += s_MenuCnt;
	}

	// ���_���W�̐ݒ�
	SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), s_fWidthMenu, s_fHeigthMenu);

	s_pVtxBuffMenu->Unlock();	// ���_�o�b�t�@���A�����b�N����
}

//=========================================
// �����L���O�̕`�揈��
//=========================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRank;
	int nCntScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �w�i�̕`��
	InitDraw(pDevice, s_pVtxBuffBg);	// Draw�̏�����
	SetDraw(pDevice, s_pTextureBg, 0);	// �`�揈��

	// �w�i�̕`��
	InitDraw(pDevice, s_pVtxBuffMenu);	// Draw�̏�����
	SetDraw(pDevice, s_pTextureMenu, 0);	// �`�揈��

	// ���ʂ̕`��
	InitDraw(pDevice, s_pVtxBuffRank);		// Draw�̏�����
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		SetDraw(pDevice, s_pTextureRank, nCntRank * 4);	// �`�揈��
	}

	// �X�R�A�̕`��
	InitDraw(pDevice, s_pVtxBuffScore);	// Draw�̏�����
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{// ����
		for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{// �X�R�A
			SetDraw(pDevice, s_pTextureScore, (nCntRank * 4 * MAX_SCORE) + (nCntScore * 4));	// �`�揈��
		}
	}

	InitDraw(pDevice, s_pVtxBuffSelect);
	for (int i = 0; i < SELECT_NUMBER; i++)
	{
		SetDraw(pDevice, s_pTextureSelect[i], i * 4);	// �`�揈��
	}
}

//=========================================
// �����L���O�̃��Z�b�g
//=========================================
void ResetRanking(void)
{
	FILE *pFile;	// �t�@�C���|�C���^��錾
	int nCntRank;

	s_nRankUpdate = -1;	// �X�V�����NNo.�̏�����

						// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{// �����L���O�̓ǂݍ���
			fscanf(pFile, "%d", &s_aRankScore[nCntRank].nScore);
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		FADE pFade = GetFade();		// ���̃t�F�[�h

		if (pFade == FADE_NONE)
		{// �������Ă��Ȃ���ԂȂ�
		 // �t�F�[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
	}
}

//=========================================
// �����L���O�̃Z�[�u
//=========================================
void SaveRanking(void)
{
	FILE *pFile;	// �t�@�C���|�C���^��錾
	int nCntRank;

	// �t�@�C�����J��
	pFile = fopen(FILE_NAME, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{// �����L���O�̏�������
			fprintf(pFile, "%d\n", s_aRankScore[nCntRank].nScore);
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		FADE pFade = GetFade();		// ���̃t�F�[�h

		if (pFade == FADE_NONE)
		{// �������Ă��Ȃ���ԂȂ�
		 // �t�F�[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
	}

}

//=========================================
// �����L���O�̐ݒ菈��
//=========================================
void SetRanking(int nScore)
{
	VERTEX_2D *pVtx;
	int nCntRank;		// �J��Ԃ�
	int nCnt2Rnak;		// �J��Ԃ��Q
	int nCheckDataMin;	// ��ׂď������ق��̒l���i�[

	// �����L���O�̍ŉ��ʂƍ���̃X�R�A���ׂ�
	if (nScore >= s_aRankScore[MAX_RANK - 1].nScore)
	{
		s_nRankUpdate = 5;
		// ��r�������l������B
		s_aRankScore[MAX_RANK - 1].nScore = nScore;
	}

	for (nCntRank = 0; nCntRank < MAX_RANK - 1; nCntRank++)
	{
		for (nCnt2Rnak = nCntRank + 1; nCnt2Rnak < MAX_RANK; nCnt2Rnak++)
		{// ��L�̌J��Ԃ����i�ނ��тɌJ��Ԃ��񐔂�-1�������B

		 // ��̃f�[�^���r����
			if (s_aRankScore[nCntRank].nScore <= s_aRankScore[nCnt2Rnak].nScore)
			{
				// �ϐ����ꎞ�i�[
				nCheckDataMin = s_aRankScore[nCnt2Rnak].nScore;

				// ��r�������l������B
				s_aRankScore[nCnt2Rnak].nScore = s_aRankScore[nCntRank].nScore;
				s_aRankScore[nCntRank].nScore = nCheckDataMin;
			}
		}

	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		int aPosTexU[8];
		int score = s_aRankScore[nCntRank].nScore;
		int nCnt = 0;

		for (nCnt2Rnak = 0; nCnt2Rnak < MAX_SCORE; nCnt2Rnak++)
		{
			// �ꌅ���ɕ�����
			aPosTexU[nCnt2Rnak] = score % 10;
			score /= 10;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);

			pVtx += 4;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuffScore->Unlock();

	// ����X�V���������N�̎擾
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (s_aRankScore[nCntRank].nScore == nScore)
		{
			s_nRankUpdate = nCntRank;
		}
	}

	// �Z�[�u����
	SaveRanking();
}