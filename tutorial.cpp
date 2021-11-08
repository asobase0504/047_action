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

//=========================================
// ����������
//=========================================
void InitTutorial(void)
{
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
	SetBlock(D3DXVECTOR3(500.0f, 800.0f, 0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(1150.0f, 25.0f, 0.0f), 25.0f, 650.0f);
	SetBlock(D3DXVECTOR3(1800.0f, 800.0f, 0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(1150.0f, SCREEN_HEIGHT - 25.0f, 0.0f), 25.0f, 650.0f);

	// �X�R�A�̏���������
	InitScore();
}

//=========================================
// �I������
//=========================================
void UninitTutorial(void)
{
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

	if (GetJoypadTrigger(JOYKEY_A) || GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TITLE);
	}

	if (GetJoypadTrigger(JOYKEY_A) || GetKeyboardTrigger(DIK_I))
	{
		SetEnemy(D3DXVECTOR3(1000.0f,500.0f,0.0f), SPLITBALL_FIRST);
	}
}

//=========================================
// �`�揈��
//=========================================
void DrawTutorial(void)
{
	// �w�i�̕`�揈��
	DrawBG();

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
