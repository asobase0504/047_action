//====================================
// 
// �G�t�F�N�g�̏���
// Author YudaKaito
// 
//====================================

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "effect.h"
#include "player.h"
#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define MAX_EFFECT	(4096)	// �G�t�F�N�g�̍ő吔

//------------------------------------
// static�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[EFFECT_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
static Effect s_aEffect[MAX_EFFECT];				// �G�t�F�N�g�̏��

//====================================
// ����������
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	VERTEX_2D *pVtx;
	Effect *pEffeck;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEX,
		&s_pTexture[EFFECT_SPLITBALL_MOVE]);

	// �G�t�F�N�g�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);

		pEffeck->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffeck->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pEffeck->fWidth = 0.0f;
		pEffeck->fHeigth = 0.0f;
		pEffeck->nLife = 100;
		pEffeck->bUse = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, pEffeck->pos, pEffeck->fWidth, pEffeck->fHeigth);
		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(pEffeck->col));
		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
// �I������
//====================================
void UnInitEffect(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		// �e�N�X�`���̔j��
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//====================================
// �X�V����
//====================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;
	int nCntEffect;
	Effect *pEffeck;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse == true)
		{
			// �G�t�F�N�g�̃A�h���X�̎擾
			pEffeck = &(s_aEffect[nCntEffect]);

			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, pEffeck->pos, pEffeck->fWidth, pEffeck->fHeigth);

			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(pEffeck->col));

			// �J���[�̓����x�̌���
			pEffeck->col.a -= 0.05f;

			// �傫�����k��
			pEffeck->fWidth += 5.0f;
			pEffeck->fHeigth += 5.0f;

			// �̗͂̍X�V
			pEffeck->nLife--;
			if (pEffeck->nLife <= 0)
			{	
				pEffeck->bUse = false;
			}

			pVtx += 4;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
// �`�揈��
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	Effect *pEffeck;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	InitDraw(pDevice, s_pVtxBuff);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);
		if (s_aEffect[nCntEffect].bUse == true)
		{
			switch (pEffeck->type)
			{
			case EFFECT_SPLITBALL_MOVE:
				AddSetDraw(pDevice, s_pTexture[EFFECT_SPLITBALL_MOVE], nCntEffect * 4);
				break;
			default:
				break;
			}
		}
	}
}

//====================================
// �ݒ菈��
//====================================
void SetEffect(D3DXVECTOR3 pos, EFFCT_TYPE type)
{
	int nCntEffect;
	Effect *pEffeck;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// �G�t�F�N�g�̃A�h���X�̎擾
		pEffeck = &(s_aEffect[nCntEffect]);
		if (pEffeck->bUse == false)
		{	
			pEffeck->pos	= pos;
			pEffeck->type	= type;

			switch (type)
			{
			case EFFECT_SPLITBALL_MOVE:
				pEffeck->col = {1.0f,1.0f,1.0f,1.0f};
				pEffeck->fWidth = 10;
				pEffeck->fHeigth = 10;
				pEffeck->nLife = 0;
				break;
			default:
				assert(false);
				break;
			}
			pEffeck->bUse = true;
			break;
		}
	}
}