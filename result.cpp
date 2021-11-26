//=========================================
// 
// ���U���g��ʂ̏���
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 s_pTexture[GAME_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
RESULT s_result;
GAME_CASE s_GameCase;

//=========================================
// ����������
//=========================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���g

								// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg004.jpg",
		&s_pTexture[0]);

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg016.jpg",
		&s_pTexture[1]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

							// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//=========================================
// �I������
//=========================================
void UninitResult(void)
{
	int nCntResult;
	// ���y�̒�~
	StopSound();

	for (nCntResult = 0; nCntResult < GAME_MAX; nCntResult++)
	{
		// �e�N�X�`���̔j��
		if (s_pTexture[nCntResult] != NULL)
		{
			s_pTexture[nCntResult]->Release();
			s_pTexture[nCntResult] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//=========================================
// �X�V����
//=========================================
void UpdateResult(void)
{
	if (GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		// ���艹�̍Đ�
		PlaySound(SOUND_LABEL_SE_ENTER);

		SetFade(MODE_TITLE);
	}

}

//=========================================
// �`�揈��
//=========================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTexture[s_GameCase]);

	// �|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=========================================
// �`�揈��
//=========================================
void GetResult(GAME_CASE Game_Case)
{
	s_GameCase = Game_Case;
}