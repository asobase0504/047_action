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
	SetBlock(D3DXVECTOR3(500.0f, 800.0f, 0.0f), 800.0f, 25.0f, 0);
	SetBlock(D3DXVECTOR3(1150.0f, 25.0f, 0.0f), 25.0f, 650.0f, 0);
	SetBlock(D3DXVECTOR3(1800.0f, 800.0f, 0.0f), 800.0f, 25.0f, 0);
	SetBlock(D3DXVECTOR3(1150.0f, SCREEN_HEIGHT - 25.0f, 0.0f), 25.0f, 650.0f, 0);
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

	if (GetJoypadTrigger(JOYKEY_A) || GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TITLE);
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
}

//=========================================
// �����\������
//=========================================
void TextTutorial(void)
{

}
