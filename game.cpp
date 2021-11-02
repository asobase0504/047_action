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
#include "bg.h"
#include "map.h"
#include "block.h"
#include "ranking.h"
#include "time.h"
#include "score.h"
#include "summon.h"
#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define RANK_INTERVAL	(100)

//------------------------------------
// �Q�[���X�e�[�^�X�̗񋓌^
//------------------------------------
typedef enum
{
	GAMESTATE_NONE = 0,			// �������ĂȂ��Ƃ�
	GAMESTATE_NORMAL,			// �Q�[���v���C��
	GAMESTATE_END,				// �Q�[���I����
	GAMESTATE_RANKING_INIT,		// �����L���O�̏�����
	GAMESTATE_RANKING_NORMAL,	// �����L���O�\����
}GAMESTATE;

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
	SetBlock({ 500.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,25.0f,0.0f }, 25.0f, 650.0f, 0);
	SetBlock({ 1800.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,SCREEN_HEIGHT-25.0f,0.0f }, 25.0f, 650.0f, 0);

	//// �G�l�~�[�̐ݒ�
	SetEnemy({ 1100.0f,550.0f,0.0f }, GOSTRAIGHT_RIGHT);
	SetEnemy({ 1100.0f,570.0f,0.0f }, GOSTRAIGHT_RIGHT);
	//SetEnemy({ 1100.0f,70.0f,0.0f }, EXTENDBALL_UP);

	// �X�R�A�̐ݒ�
	SetScore(1234);

	// �����L���O�̓Ǎ�
	ResetRanking();

	s_nRankInterval = 0;
}

//====================================
// �I������
//====================================
void UninitGame(void)
{
	// �w�i�̏I������
	UninitBG();

	// �G�t�F�N�g�̏I������
	UnInitEffect();

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

//====================================
// �X�V����
//====================================
void UpdateGame(void)
{
	SetEnemy({ 1100.0f,550.0f,0.0f }, GOSTRAIGHT_RIGHT);
	SetEnemy({ 1100.0f,570.0f,0.0f }, GOSTRAIGHT_RIGHT);

	switch (s_GameState)
	{
	case GAMESTATE_NONE:
		RetryGame();
		break;
	case GAMESTATE_NORMAL:
	{
		//�G�̏���
//		UpdateSummon();

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
			s_GameState = GAMESTATE_END;	//�Q�[���I�����Ɉڍs
		}
	}
		break;
	case GAMESTATE_END:
		//�����L���O�\���܂ł̗]�C
		s_nRankInterval++;
		if (s_nRankInterval >= RANK_INTERVAL)
		{
			s_GameState = GAMESTATE_RANKING_INIT;	//�����L���O�\�����Ɉڍs
		}
		break;
	case GAMESTATE_RANKING_INIT:
		SetRanking(GetScore());
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
}


//====================================
// retry����
//====================================
void RetryGame(void)
{
	// ���Ԃ̏I������
	UninitTime();

	// �X�R�A�̏I������
	UninitScore();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// ���Ԃ̏���������
	InitTime();

	// �X�R�A�̏���������
	InitScore();

	// �G�l�~�[�̏���������
	InitEnemy();
}
