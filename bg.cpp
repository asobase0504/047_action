//=========================================
// 
// �w�i�̏���
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "setup.h"
#include "bg.h"

//-----------------------------------------
// �w�i�\���̂̒�`
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
	GRADATION gradation;	// �O���f�[�V�����̏��
} BG;

//-----------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = {};		// �e�N�X�`���o�b�t�@
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;		// ���_�o�b�t�@
float g_aPosTexV[NUM_BG];	// �e�N�X�`�����W�̊J�n�ʒu(V�l)
BG g_aBG[NUM_BG];

//====================================
// �w�i�̏���������
//====================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���g
	int nCntBG;
	BG *pBG;


	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg000.jpg",
		&g_pTextureBG[0]);
	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg004.png",
		&g_pTextureBG[1]);

	// �w�i�̏��̏�����
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pBG = &(g_aBG[nCntBG]);
		pBG->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBG->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBG->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBG->bUse = false;
		g_aPosTexV[nCntBG];
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pBG = &(g_aBG[nCntBG]);

		pVtx[0].pos.x = pBG->pos.x;
		pVtx[0].pos.y = pBG->pos.y;
		pVtx[0].pos.z = pBG->pos.z;

		pVtx[1].pos.x = pBG->pos.x + SCREEN_WIDTH;
		pVtx[1].pos.y = pBG->pos.y;
		pVtx[1].pos.z = pBG->pos.z;

		pVtx[2].pos.x = pBG->pos.x;
		pVtx[2].pos.y = pBG->pos.y + SCREEN_HEIGHT;
		pVtx[2].pos.z = pBG->pos.z;

		pVtx[3].pos.x = pBG->pos.x + SCREEN_WIDTH;
		pVtx[3].pos.y = pBG->pos.y + SCREEN_HEIGHT;
		pVtx[3].pos.z = pBG->pos.z;



		switch (nCntBG)
		{
		case 0:	// �O���f�[�V������
			// ���_�J���[�̐ݒ�
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			break;
		case 1:	// �O���f�[�V������
			SetRectColor(pVtx, &(D3DXCOLOR(0.95f, 1.0f, 0.9f, 1.0f)));
			break;
		default:
			break;
		}

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();

}

//====================================
// �w�i�̏I������
//====================================
void UninitBG(void)
{
	int nCntBG;

	// �e�N�X�`���̔j��
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_pTextureBG[nCntBG] != NULL)
		{
			g_pTextureBG[nCntBG]->Release();
			g_pTextureBG[nCntBG] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

}

//====================================
// �w�i�̍X�V����
//====================================
void UpdateBG(void)
{

}

//====================================
// �w�i�̕`�揈��
//====================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���g
	int nCntBG;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
	}
}
