//====================================
//
//�G�t�F�N�g�̏���
//Author YudaKaito
//
//====================================

//------------------------------------
//include
//------------------------------------
#include "main.h"
#include "effect.h"
#include "player.h"

//------------------------------------
//�}�N����`
//------------------------------------
#define MAX_EFFECT	(4096)	//�G�t�F�N�g�̍ő吔

//------------------------------------
//static�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static Effect s_aEffect[MAX_EFFECT];				//�G�t�F�N�g�̏��

//====================================
//����������
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	VERTEX_2D *pVtx;
	Effect *pEffeck;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEX,
		&s_pTexture);

	//�G�t�F�N�g�̏��̏�����
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

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);

		//���_���W�̐ݒ�
		SetRectPos(pVtx, pEffeck->pos, pEffeck->fHeigth, pEffeck->fWidth);

		//rhw�̐ݒ�
		//�e�N�X�`�����W�̐ݒ�
		//���_�J���[�̐ݒ�
		SetupRectDefault(pVtx, &(pEffeck->col));

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
//�I������
//====================================
void UnInitEffect(void)
{
	//�e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//====================================
//�X�V����
//====================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;
	int nCntEffect;
	Effect *pEffeck;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse == true)
		{
			//�G�t�F�N�g�̃A�h���X�̎擾
			pEffeck = &(s_aEffect[nCntEffect]);

			//���_���W�̐ݒ�
			SetRectPos(pVtx, pEffeck->pos, pEffeck->fHeigth, pEffeck->fWidth);

			//���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(pEffeck->col));

			//�J���[�̓����x�̌���
			pEffeck->col.a -= 0.05f;

			//�傫�����k��
			pEffeck->fWidth += 5.0f;
			pEffeck->fHeigth += 5.0f;

			//�̗͂̍X�V
			pEffeck->nLife--;
			if (pEffeck->nLife <= 0)
			{	
				pEffeck->bUse = false;
			}

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
//�`�揈��
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture);

			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//====================================
//�ݒ菈��
//====================================
void SetEffect(D3DXVECTOR3 pos, EFFCT_TYPE type)
{
	int nCntEffect;
	Effect *pEffeck;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�G�t�F�N�g�̃A�h���X�̎擾
		pEffeck = &(s_aEffect[nCntEffect]);
		if (pEffeck->bUse == false)
		{	
			pEffeck->pos = pos;

			switch (type)
			{
			case EFFECT_SPLITBALL_MOVE:
				pEffeck->col = {1.0f,1.0f,1.0f,1.0f};
				pEffeck->fWidth = 10;
				pEffeck->fHeigth = 10;
				pEffeck->nLife = 0;
				break;
			default:
				pEffeck->col;
				pEffeck->fWidth = 0;
				pEffeck->fHeigth = 0;
				pEffeck->nLife = 0;
				break;
			}
			pEffeck->bUse = true;
			break;
		}
	}
}