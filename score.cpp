//====================================
//
// �X�R�A����
// Author YudaKaito
//
//====================================
#include "main.h"
#include "setup.h"
#include "score.h"


// �O���[�o���ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
static D3DXVECTOR3				s_posScore;			// �X�R�A�̈ʒu
static int						s_nScore;			// �X�R�A�̒l

//====================================
// �X�R�A�̏���������
//====================================
void InitScore()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&s_pTexture);

	// �X�R�A�̏��̏�����
	s_posScore = D3DXVECTOR3(450, 200, 0);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		SetRectUpRightPos(pVtx, D3DXVECTOR3(s_posScore.x - 60.0f * i, s_posScore.y, s_posScore.z), 50.0f, 50.0f);

		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx,&(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

		// �e�N�X�`�����W�̐ݒ�
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
// �X�R�A�̏I������
//====================================
void UninitScore()
{
	// �e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//====================================
// �X�R�A�̍X�V����
//====================================
void UpdateScore()
{

}

//====================================
// �X�R�A�̕`�揈��
//====================================
void DrawScore()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//
	InitDraw(pDevice, s_pVtxBuff);

	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//
		SetDraw(pDevice, s_pTexture, nCntScore * 4);
	}

}

//====================================
// �X�R�A�̐ݒ菈��
//====================================
void SetScore(int nScore)
{
	// ���[�J���ϐ�
	s_nScore = nScore;
	VERTEX_2D *pVtx;
	int aNumber[8];
	int nTime = s_nScore;
	int i = 0;
	float aTex[8];
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// �ꌅ���ɕ�����
	while (nTime != 0)
	{
		aNumber[i] = nTime % 10;
		aTex[i] = aNumber[i] * 0.1f;
		nTime /= 10;

		// �e�N�X�`�����W�̐ݒ�
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);

		i++;
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
// �X�R�A�̉��Z����
//====================================
void AddScore(int nValue)
{
	// ���[�J���ϐ�
	s_nScore += nValue;
	VERTEX_2D *pVtx;
	int aNumber[8];
	int nTime = s_nScore;
	int i = 0;
	float aTex[8];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ꌅ���ɕ�����
	while (nTime != 0)
	{
		aNumber[i] = nTime % 10;
		aTex[i] = aNumber[i] * 0.1f;
		nTime /= 10;

		// �e�N�X�`�����W�̐ݒ�
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);

		i++;
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
// �X�R�A�̎擾
//====================================
int GetScore(void)
{
	return s_nScore;
}