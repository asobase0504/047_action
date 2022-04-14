//=========================================
// 
// �Q�[����ʂ̏���
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "game.h"
#include "input.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "enemy.h"
#include "ag.h"
#include "bg.h"
#include "map.h"
#include "block.h"
#include "ranking.h"
#include "time.h"
#include "score.h"
#include "summon.h"
#include "sound.h"
#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define RANK_INTERVAL	(100)

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static int s_nRankInterval;
static GAMESTATE s_GameState;

//====================================
// ����������
//====================================
void InitGame(void)
{
	PlaySound(SOUND_LABEL_BGM001);

	s_GameState = GAMESTATE_NORMAL;

	// �w�i�̏���������
	InitBG();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �p�[�e�B�N���̏���������
	InitParticle();

	// �v���C���[�̏���������
	InitPlayer();

	// �G�l�~�[�̏���������
	InitEnemy();

	// �u���b�N�̏���������
	InitBlock();

	// �����L���O�̏���������
	InitRanking();

	// �^�C���̏I������
	InitTime();

	// �X�R�A�̏�����
	InitScore();

	// �u���b�N�̐ݒ�
	SetBlock(D3DXVECTOR3(300.0f,800.0f,0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(950.0f,25.0f,0.0f), 25.0f, 650.0f);
	SetBlock(D3DXVECTOR3(1600.0f,800.0f,0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(950.0f, SCREEN_HEIGHT - 25.0f, 0.0f), 25.0f, 650.0f);
	SetBlock(D3DXVECTOR3(900.0f, 550.0f, 0.0f), 350.0f, 25.0f);
	SetBlock(D3DXVECTOR3(900.0f, 500.0f, 0.0f), 25.0f, 350.0f);

	// �X�R�A�̐ݒ�
	SetScore(0);

	// WAVE�̐ݒ�
	SetWave(0);

	// �����L���O�̓Ǎ�
	ResetRanking();

	// �����N�̕\���Ԋu�̏�����
	s_nRankInterval = 0;

	// �w�i�̏���������
	InitAG();
}

//====================================
// �I������
//====================================
void UninitGame(void)
{
	// �w�i�̏I������
	UninitBG();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �v���C���[�̏I������
	UninitPlayer();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �u���b�N�̏I������
	UninitBlock();

	// �����L���O�̏I������
	UninitRanking();

	// �^�C���̏I������
	UninitTime();

	// �X�R�A�̏I������
	UninitScore();
}

#ifdef _DEBUG

int D_nCnt = 0;

#endif // _DEBUG

//====================================
// �X�V����
//====================================
void UpdateGame(void)
{
	switch (s_GameState)
	{
	case GAMESTATE_NONE:
	{
		RetryGame();
		Player *pPlayer	= GetPlayer();
		pPlayer->state	= PLAYERSTATE_REVIVAL;
		s_GameState		= GAMESTATE_NORMAL;
		break;
	}
	case GAMESTATE_NORMAL:
	{
		//�G�̏���
		UpdateSummon();

		//���Ԃ̍X�V����
		UpdateTime();

		// �X�R�A�̍X�V����
		UpdateScore();

		// �G�l�~�[�̍X�V����
		UpdateEnemy();

		//�v���C���[�����񂾂����肷��
		Player *player = GetPlayer();
		if (player->state == PLAYERSTATE_DEATH)
		{
			StopSound();
			s_GameState = GAMESTATE_END;	//�Q�[���I�����Ɉڍs
		}
	}
		break;
	case GAMESTATE_END:
		//�����L���O�\���܂ł̗]�C
		s_nRankInterval++;
		if (s_nRankInterval >= RANK_INTERVAL)
		{
			SetRanking(GetScore());
			PlaySound(SOUND_LABEL_BGM002);
			s_GameState = GAMESTATE_RANKING_INIT;	//�����L���O�\�����Ɉڍs
		}
		break;
	case GAMESTATE_RANKING_INIT:
		s_GameState = GAMESTATE_RANKING_NORMAL;	//�����L���O�\�����Ɉڍs

		break;
	case GAMESTATE_RANKING_NORMAL:
		UpdateRanking();
		break;
	default:
		assert(false);
		break;
	}

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

	// �w�i�̏���������
	UpdateAG();
}

//====================================
// �`�揈��
//====================================
void DrawGame(void)
{
	// �w�i�̕`�揈��
	DrawBG();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	// �u���b�N�̕`�揈��
	DrawBlock();
	
	// ���Ԃ̕`�揈��
	DrawTime();

	// �X�R�A�̕`�揈��
	DrawScore();
	if (s_GameState == GAMESTATE_RANKING_NORMAL)
	{
		DrawRanking();
	}

	// �w�i�̏���������
	DrawAG();
}


//====================================
// retry����
//====================================
void RetryGame(void)
{
	// �Q�[�����BGM�ɕύX
	PlaySound(SOUND_LABEL_BGM001);

	// ���Ԃ̏I������
	UninitTime();

	// �X�R�A�̏I������
	UninitScore();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �����L���O�̏I������
	UninitRanking();

	// ���Ԃ̏���������
	InitTime();

	// �X�R�A�̏���������
	InitScore();
	SetScore(0);

	// �G�l�~�[�̏���������
	InitEnemy();

	// �����L���O�̏���������
	InitRanking();

	// WAVE�̐ݒ�
	SetWave(0);
}

//====================================
// �Q�[���̃X�e�[�^�X�ݒ菈��
//====================================
void SetGameState(GAMESTATE state)
{
	s_GameState = state;
}
