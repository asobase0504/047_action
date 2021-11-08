//====================================
//
// �X�R�A����
// Author YudaKaito
//
//====================================
#include "main.h"
#include "setup.h"
#include "number.h"

// �O���[�o���ϐ�
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

//====================================
// �X�R�A�̏���������
//====================================
void InitNumber()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&s_pTexture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	InitRectPos(pVtx);

	// rhw�̐ݒ�
	InitRectRhw(pVtx);

	// ���_�J���[�̐ݒ�
	SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

	// �e�N�X�`�����W�̐ݒ�
	SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

	pVtx += 4;

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
// �X�R�A�̏I������
//====================================
void UninitNumber()
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
void UpdateNumber()
{

}

//====================================
// �X�R�A�̕`�揈��
//====================================
void DrawNumber()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �`��ݒ�
	InitDraw(pDevice, s_pVtxBuff);

	// �e�N�X�`���̕`��
	RectDraw(pDevice, s_pTexture, 0);
}

//====================================
// �����̐ݒ菈��
//====================================
void SetNumber(VERTEX_2D *pVtx, int nNumber)
{
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float Tex = nNumber * 0.1f;

	// �e�N�X�`�����W�̐ݒ�
	SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + Tex, 0.1f + Tex);

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}