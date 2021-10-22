//=========================================
//
//�Q�[����ʂ̏���
//Author YudaKaito
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

//�O���[�o���ϐ�
int g_nCntEnemy;

//====================================
//����������
//====================================
void InitGame(void)
{
	//�w�i�̏���������
	InitBG();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�l�~�[�̏���������
	InitEnemy();

	//�u���b�N�̏���������
	InitBlock();

	SetBlock({ 500.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,25.0f,0.0f }, 25.0f, 650.0f, 0);
	SetBlock({ 1800.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,SCREEN_HEIGHT-25.0f,0.0f }, 25.0f, 650.0f, 0);

	SetEnemy({ 1100.0f,550.0f,0.0f }, ENEMYTYPE_SPLITBALL_FIRST);
	SetEnemy({ 1100.0f,70.0f,0.0f }, ENEMYTYPE_EXTENDBALL_UP);
}

//====================================
//�I������
//====================================
void UninitGame(void)
{

	//�w�i�̏I������
	UninitBG();

	//�G�t�F�N�g�̏I������
	UnInitEffect();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�v���C���[�̏I������
	UninitPlayer();

	//�G�l�~�[�̏I������
	UninitEnemy();

	//�u���b�N�̏I������
	UninitBlock();
}

//====================================
//�X�V����
//====================================
void UpdateGame(void)
{
	//�w�i�̍X�V����
	UpdateBG();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	//�v���C���[�̍X�V����
	UpdatePlayer();

	//�G�l�~�[�̍X�V����
	UpdateEnemy();

	//�u���b�N�̍X�V����
	UpdateBlock();
}

//====================================
//�`�揈��
//====================================
void DrawGame(void)
{
	//�w�i�̕`�揈��
	DrawBG();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�p�[�e�B�N���̕`�揈��
	DrawParticle();

	//�v���C���[�̕`�揈��
	DrawPlayer();

	//�G�l�~�[�̕`�揈��
	DrawEnemy();

	//�u���b�N�̕`�揈��
	DrawBlock();
}
