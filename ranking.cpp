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
#include "ranking.h"
// #include "score.h"
#include "fade.h"
#include <stdio.h>
#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define MENU_MAX_WIDTH	(400.0f)
#define MENU_MAX_HEIGTH	(500.0f)

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
static LPDIRECT3DTEXTURE9		s_pTextureRank = NULL;	// �e�N�X�`���ւ̃|�C���^	(�w�i)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRank = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(�w�i)
static LPDIRECT3DTEXTURE9		s_pTextureScore = NULL;	// �e�N�X�`���ւ̃|�C���^	(�X�R�A)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffScore = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(�X�R�A)
static LPDIRECT3DTEXTURE9		s_pTextureBg = NULL;	// �e�N�X�`���ւ̃|�C���^	(����)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBg = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(����)
static LPDIRECT3DTEXTURE9		s_pTextureMenu = NULL;	// �e�N�X�`���ւ̃|�C���^	(���)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffMenu = NULL;	// ���_�o�b�t�@�ւ̃|�C���^	(���)

static RankScore	s_aRankScore[MAX_RANK];	// �����L���O�X�R�A���
static int			s_nRankUpdate = -1;		// �X�V�����NNo.
static int			s_nTimerRanking;		// �����L���O��ʕ\���^�C�}�[
static D3DXCOLOR	s_Timercol;				// ���Ŏ��̃J���[�̐ݒ�
static bool			s_bBlink;				// �X�R�A�̖��ł̐؂�ւ�
static int			s_nTimerBlink;			// �X�R�A�̖��ł̊Ԋu

static D3DXVECTOR3	s_MenuPos;		// ���j���[��ʂ̈ʒu
static float		fWidthMenu;		// ���j���[��ʂ̕�
static float		fHeigthMenu;	// ���j���[��ʂ̍���
static int			s_MenuCnt;	// ���j���[��ʂ̃J�E���g


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
		"data/TEXTURE/number001.png",
		&s_pTextureScore);


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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK * MAX_RANKSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffScore,
		NULL);

	// 
	// �w�i
	// 
	s_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	{
		// �����`�̏�����
		// rhw�̐ݒ�
		// �e�N�X�`�����W�̐ݒ�
		InitRect(pVtx);

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

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f)));
	}
	s_pVtxBuffBg->Unlock();		// ���_�o�b�t�@���A�����b�N����

	// 
	// ���j���[���
	// 
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	{
		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), fWidthMenu, fHeigthMenu);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.9f)));

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);

		// rhw�̐ݒ�
		InitRectRhw(pVtx);
	}
	s_pVtxBuffMenu->Unlock();	// ���_�o�b�t�@���A�����b�N����

	// 
	// ���ʂ̏�����
	// 
	s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
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
			SetRextTex(pVtx, 0.0f + 0.2f * nCntRank, 0.2f + 0.2f * nCntRank, 0.0f, 1.0f);

			// rhw�̐ݒ�
			InitRectRhw(pVtx);

			pVtx += 4;
		}
	}
	s_pVtxBuffRank->Unlock();	// ���_�o�b�t�@���A�����b�N����

	// 
	// �X�R�A�̏�����
	// 
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	{
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			s_aRankScore[nCntRank].pos.x += 100;	// �X�R�A�\���ꏊ�ֈړ�
			for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
			{
				pVtx[0].pos.x = s_aRankScore[nCntRank].pos.x - 20.0f + nCntScore * 45;
				pVtx[0].pos.y = s_aRankScore[nCntRank].pos.y - 20.0f;
				pVtx[0].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

				pVtx[1].pos.x = s_aRankScore[nCntRank].pos.x + 20.0f + nCntScore * 45;
				pVtx[1].pos.y = s_aRankScore[nCntRank].pos.y - 20.0f;
				pVtx[1].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

				pVtx[2].pos.x = s_aRankScore[nCntRank].pos.x - 20.0f + nCntScore * 45;
				pVtx[2].pos.y = s_aRankScore[nCntRank].pos.y + 20.0f;
				pVtx[2].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

				pVtx[3].pos.x = s_aRankScore[nCntRank].pos.x + 20.0f + nCntScore * 45;
				pVtx[3].pos.y = s_aRankScore[nCntRank].pos.y + 20.0f;
				pVtx[3].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;


				// ���_�J���[�̐ݒ�
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f)));

				// �e�N�X�`�����W�̐ݒ�
				SetRextTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

				// rhw�̐ݒ�
				InitRectRhw(pVtx);

				pVtx += 4;
			}
		}
	}
	s_pVtxBuffScore->Unlock();	// ���_�o�b�t�@���A�����b�N����
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

}

//=========================================
// �����L���O�̍X�V����
//=========================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;
	
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
		s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		for (int i = 0; i < MAX_RANK; i++)
		{
			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

			pVtx += 4;
		}
		s_pVtxBuffRank->Unlock();	// ���_�o�b�t�@���A�����b�N����

		s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			if (nCntRank == s_nRankUpdate)
			{	// ����̃X�R�A�̓_��
				for (int nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
				{
					// ���_�J���[�̐ݒ�
					SetRectColor(pVtx, &(s_Timercol));

					pVtx += 4;
				}
			}
			else
			{	// �ʏ��Ԃ̐F
				for (int nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
				{
					// ���_�J���[�̐ݒ�
					SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
					pVtx += 4;
				}
			}
		}
		s_pVtxBuffScore->Unlock();	// ���_�o�b�t�@���A�����b�N����
	}

	// ���Ԑ����������́A�G���^�[�L�[�Ń��U���g��ʂɈڍs
	if (s_nTimerRanking >= 3000 || GetKeyboardTrigger(DIK_RETURN))
	{
		s_MenuCnt = 33;
		fHeigthMenu = 0.0f;
		// ���艹�̍Đ�
		PlaySound(SOUND_LABEL_SE_ENTER);

		// ���U���g��ʂɈڍs
		SetFade(MODE_RESULT);
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
	if (!(s_MenuCnt <= -10.0f))
	{
		s_MenuCnt -= 1.0f;

		fWidthMenu += s_MenuCnt;
		fHeigthMenu += s_MenuCnt;
	}

	// ���_���W�̐ݒ�
	SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), fWidthMenu, fHeigthMenu);

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
		for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
		{// �X�R�A
			SetDraw(pDevice, s_pTextureScore, (nCntRank * 4 * MAX_RANKSCORE) + (nCntScore * 4));	// �`�揈��
		}
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

		// �ꌅ���ɕ�����
		while (score != 0)
		{
			aPosTexU[nCnt] = score % 10;
			score /= 10;
			nCnt++;
		}

		for (nCnt2Rnak = 0; nCnt2Rnak < MAX_RANKSCORE; nCnt2Rnak++)
		{
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