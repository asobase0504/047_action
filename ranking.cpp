//=========================================
//
//�����L���O�̏���
//Author YudaKaito
//
//=========================================
//------------------------------------
//include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "input.h"
#include "sound.h"
#include <stdio.h>
#include "ranking.h"
//#include "score.h"
#include "fade.h"

//------------------------------------
//�����L���O�X�R�A�\����
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nScore;			//�X�R�A
}RankScore;

//------------------------------------
//�O���[�o���ϐ�
//------------------------------------
LPDIRECT3DTEXTURE9 s_pTextureRank = NULL;					//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 s_pTextureRankScore = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 s_pTextureRankbg = NULL;					//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffRank = NULL;				//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffRankScore = NULL;			//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffRankbg = NULL;			//�e�N�X�`���ւ̃|�C���^
RankScore s_aRankScore[MAX_RANK];							//�����L���O�X�R�A���
int s_nRankUpdate = -1;										//�X�V�����NNo.
int s_nTimerRanking;										//�����L���O��ʕ\���^�C�}�[
bool s_bBlink;
int s_nTimerBlink;
D3DXCOLOR s_Timercol;

//=========================================
//�����L���O�̏���������
//=========================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntRank;
	int nCntScore;
	s_nTimerRanking = 0;
	s_bBlink = true;
	s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureRankbg);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking_rank1.png",
		&s_pTextureRank);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&s_pTextureRankScore);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRankbg,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRank,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK * MAX_RANKSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRankScore,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankbg->Lock(0, 0, (void**)&pVtx, 0);

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

	//rhw�̐ݒ�
	//���_�J���[�̐ݒ�
	//�e�N�X�`�����W�̐ݒ�
	SetupRectDefault(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f)));

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankbg->Unlock();

	//�����L���O�̏��̏�����
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		//���ʂ̒l�̏ꏊ�̏�����
		s_aRankScore[nCntRank].pos = D3DXVECTOR3(450.0f, 100.0f + nCntRank * 100.0f, 0.0f);

		//���_���W�̐ݒ�
		SetRectPos(pVtx, s_aRankScore[nCntRank].pos, 50.0f, 50.0f);
		
		//���_�J���[�̐ݒ�
		//�e�N�X�`�����W�̐ݒ�
		//rhw�̐ݒ�
		SetupRectDefault(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + 0.2f * nCntRank);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + 0.2f * nCntRank);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f + 0.2f * nCntRank);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f + 0.2f * nCntRank);

		pVtx += 4;

	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRank->Unlock();


	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		s_aRankScore[nCntRank].pos.x += 100;	//�X�R�A�\���ꏊ�ֈړ�
		for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
		{
			pVtx[0].pos.x = s_aRankScore[nCntRank].pos.x - 20.0f + nCntScore * 45;
			pVtx[0].pos.y = s_aRankScore[nCntRank].pos.y - 20.0f + nCntRank * 100;
			pVtx[0].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

			pVtx[1].pos.x = s_aRankScore[nCntRank].pos.x + 20.0f + nCntScore * 45;
			pVtx[1].pos.y = s_aRankScore[nCntRank].pos.y - 20.0f + nCntRank * 100;
			pVtx[1].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

			pVtx[2].pos.x = s_aRankScore[nCntRank].pos.x - 20.0f + nCntScore * 45;
			pVtx[2].pos.y = s_aRankScore[nCntRank].pos.y + 20.0f + nCntRank * 100;
			pVtx[2].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

			pVtx[3].pos.x = s_aRankScore[nCntRank].pos.x + 20.0f + nCntScore * 45;
			pVtx[3].pos.y = s_aRankScore[nCntRank].pos.y + 20.0f + nCntRank * 100;
			pVtx[3].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;


			//rhw�̐ݒ�
			SetupRectDefault(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

			//���_�J���[�̐ݒ�

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;

		}
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankScore->Unlock();

}

//=========================================
//�����L���O�̏I������
//=========================================
void UninitRanking(void)
{

	//�e�N�X�`���̔j��
	if (s_pTextureRank != NULL)
	{
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffRank != NULL)
	{
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	//�e�N�X�`���̔j��
	if (s_pTextureRankScore != NULL)
	{
		s_pTextureRankScore->Release();
		s_pTextureRankScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffRankScore != NULL)
	{
		s_pVtxBuffRankScore->Release();
		s_pVtxBuffRankScore = NULL;
	}
}

//=========================================
//�����L���O�̍X�V����
//=========================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;
	int nCntRank;
	int nCntScore;

	s_nTimerRanking++;
	s_nTimerBlink--;

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

	if (s_nRankUpdate != -1)
	{

	}
	if (s_nTimerRanking >= 3000 || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//���艹�̍Đ�
		PlaySound(SOUND_LABEL_SE_ENTER);

		SetFade(MODE_RESULT);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (nCntRank == s_nRankUpdate)
		{
			for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
			{
				//���_�J���[�̐ݒ�
				SetRectColor(pVtx, &(s_Timercol));

				pVtx += 4;
			}
		}
		else
		{
			for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
			{
				//���_�J���[�̐ݒ�
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
				pVtx += 4;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankScore->Unlock();
}

//=========================================
//�����L���O�̕`�揈��
//=========================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRank;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRankbg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureRankbg);

	//�|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTextureRank);

		//�|���S���`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{//����
		for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
		{//�X�R�A
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTextureRankScore);

			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntRank * 4 * MAX_RANKSCORE) + (nCntScore * 4), 2);
		}
	}
}

//=========================================
//�����L���O�̃��Z�b�g
//=========================================
void ResetRanking(void)
{
	FILE *pFile;	//�t�@�C���|�C���^��錾
	int nCntRank;

	s_nRankUpdate = -1;	//�X�V�����NNo.�̏�����

						//�t�@�C�����J��
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{//�����L���O�̓ǂݍ���
			fscanf(pFile, "%d", &s_aRankScore[nCntRank].nScore);
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		FADE pFade = GetFade();		//���̃t�F�[�h

		if (pFade == FADE_NONE)
		{//�������Ă��Ȃ���ԂȂ�
		 //�t�F�[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
	}
}

//=========================================
//�����L���O�̃Z�[�u
//=========================================
void SaveRanking(void)
{
	FILE *pFile;	//�t�@�C���|�C���^��錾
	int nCntRank;

	//�t�@�C�����J��
	pFile = fopen(FILE_NAME, "w");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{//�����L���O�̏�������
			fprintf(pFile, "%d\n", s_aRankScore[nCntRank].nScore);
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ��ꍇ
		FADE pFade = GetFade();		//���̃t�F�[�h

		if (pFade == FADE_NONE)
		{//�������Ă��Ȃ���ԂȂ�
		 //�t�F�[�h�̐ݒ�
			SetFade(MODE_TITLE);
		}
	}

}

//=========================================
//�����L���O�̐ݒ菈��
//=========================================
void SetRanking(int nScore)
{
	VERTEX_2D *pVtx;
	int nCntRank;		//�J��Ԃ�
	int nCnt2Rnak;		//�J��Ԃ��Q
	int nCheckDataMin;	//��ׂď������ق��̒l���i�[

	//�����L���O�̍ŉ��ʂƍ���̃X�R�A���ׂ�
	if (nScore >= s_aRankScore[MAX_RANK - 1].nScore)
	{
		s_nRankUpdate = 5;
		//��r�������l������B
		s_aRankScore[MAX_RANK - 1].nScore = nScore;
	}

	for (nCntRank = 0; nCntRank < MAX_RANK - 1; nCntRank++)
	{
		for (nCnt2Rnak = nCntRank + 1; nCnt2Rnak < MAX_RANK; nCnt2Rnak++)
		{//��L�̌J��Ԃ����i�ނ��тɌJ��Ԃ��񐔂�-1�������B

		 //��̃f�[�^���r����
			if (s_aRankScore[nCntRank].nScore <= s_aRankScore[nCnt2Rnak].nScore)
			{
				//�ϐ����ꎞ�i�[
				nCheckDataMin = s_aRankScore[nCnt2Rnak].nScore;

				//��r�������l������B
				s_aRankScore[nCnt2Rnak].nScore = s_aRankScore[nCntRank].nScore;
				s_aRankScore[nCntRank].nScore = nCheckDataMin;
			}
		}

	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		int aPosTexU[8];
		int score = s_aRankScore[nCntRank].nScore;
		int nCnt = 0;

		//�ꌅ���ɕ�����
		while (score != 0)
		{
			aPosTexU[nCnt] = score % 10;
			score /= 10;
			nCnt++;
		}

		for (nCnt2Rnak = 0; nCnt2Rnak < MAX_RANKSCORE; nCnt2Rnak++)
		{
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);

			pVtx += 4;
		}
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffRankScore->Unlock();

	//����X�V���������N�̎擾
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (s_aRankScore[nCntRank].nScore == nScore)
		{
			s_nRankUpdate = nCntRank;
		}
	}

	//�Z�[�u����
	SaveRanking();
}