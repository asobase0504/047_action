//=========================================
// 
// ���Ԃ̏���
// Author YudaKaito
// 
//=========================================

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "time.h"
#include "setup.h"

#include <assert.h>

//------------------------------------
//�}�N����`
//------------------------------------
#define TIME_POS	"data/TEXTURE/number001.png"
#define TIME_MAX	(6)

//------------------------------------
//  static �ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				// �e�N�X�`���o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// ���_�o�b�t�@

static int s_nTime;			//����
static D3DXVECTOR3 s_pos;	//���l�̈ʒu
static D3DXCOLOR s_col;

//=========================================
// ����������
//=========================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	
	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		TIME_POS,
		&s_pTexture);

	//�^�C���̏���������
	s_pos = D3DXVECTOR3(450.0f, 150.0f, 0.0f);
	s_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	s_nTime = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
							
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < TIME_MAX; i++)
	{
		// ���_���W�̐ݒ�
		SetRectUpRightPos(pVtx, D3DXVECTOR3(s_pos.x - 60.0f * i, s_pos.y, s_pos.z), 50.0f, 50.0f);

		// �J���[�̐ݒ�
		SetRectColor(pVtx, &(s_col));

		// rhw�̐ݒ� 
		InitRectRhw(pVtx);

		// �e�N�X�`�����W�̐ݒ�
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();


}

//=========================================
// �I������
//=========================================
void UninitTime(void)
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

//=========================================
// �X�V����
//=========================================
void UpdateTime(void)
{
	s_nTime++;
	int aNumber[8];

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	//�J��グ����
	if (s_nTime % 100 >= 60)
	{
		s_nTime += 40;
	}
	assert(s_nTime % 100 < 60);	// 60�t���[���ȍ~�̏ꍇ assert

	if (s_nTime % 10000 >= 6000)
	{
		s_nTime += 4000;
	}
	assert(s_nTime % 100 < 60);//60�t���[���ȍ~�̏ꍇ assert

	int nTime = s_nTime;
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

//=========================================
// �`�揈��
//=========================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	InitDraw(pDevice, s_pVtxBuff);

	for (int i = 0; i < TIME_MAX; i++)
	{
		pDevice = SetDraw(pDevice, s_pTexture, i * 4);
	}
}