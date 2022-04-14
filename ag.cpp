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
} AG;

//-----------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------
static LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = {};	// �e�N�X�`���o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;		// ���_�o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEN = NULL;		// ���_�o�b�t�@
static float g_aPosTexV[NUM_BG];	// �e�N�X�`�����W�̊J�n�ʒu(V�l)
static AG g_aBG[NUM_BG];

//====================================
// �w�i�̏���������
//====================================
void InitAG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���g
	int nCntBG;
	AG *pBG;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_pTextureBG[0]);
	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy002.png",
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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	{
		pBG = &(g_aBG[0]);

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
		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		// rhw�̐ݒ�
		InitRectRhw(pVtx);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEN,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEN->Lock(0, 0, (void**)&pVtx, 0);
	{
		pBG->pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
		pVtx[0].pos.x = pBG->pos.x;
		pVtx[0].pos.y = pBG->pos.y;
		pVtx[0].pos.z = pBG->pos.z;

		pVtx[1].pos.x = pBG->pos.x + 200.0f;
		pVtx[1].pos.y = pBG->pos.y;
		pVtx[1].pos.z = pBG->pos.z;

		pVtx[2].pos.x = pBG->pos.x;
		pVtx[2].pos.y = pBG->pos.y + 200.0f;
		pVtx[2].pos.z = pBG->pos.z;

		pVtx[3].pos.x = pBG->pos.x + 200.0f;
		pVtx[3].pos.y = pBG->pos.y + 200.0f;
		pVtx[3].pos.z = pBG->pos.z;
		SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		// rhw�̐ݒ�
		InitRectRhw(pVtx);
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEN->Unlock();

}

//====================================
// �w�i�̏I������
//====================================
void UninitAG(void)
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
void UpdateAG(void)
{

}

//====================================
// �w�i�̕`�揈��
//====================================
void DrawAG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���g
	int nCntBG;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBG[0]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	
	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEN, 0, sizeof(VERTEX_2D));
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(1, g_pTextureBG[1]);

	//�X�|�b�g
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP	, D3DTOP_BLENDTEXTUREALPHA);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
}
