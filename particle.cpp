//=========================================
//
//�p�[�e�B�N���̏���
//Author YudaKaito
//
//=========================================

//-----------------------------------------
//include
//-----------------------------------------
#include "main.h"
#include "setup.h"
#include "particle.h"
#include <assert.h>
#include "enemy.h"

//-----------------------------------------
//�}�N����`
//-----------------------------------------
#define MAX_PARTCLE	(1024)	//�p�[�e�B�N���̍ő吔
#define MAX_TEX		(MAX_PARTICLE)		//�e�N�X�`���̎��

//-----------------------------------------
//�p�[�e�B�N���\����
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�F
	float fRaduus;		//���a
	int nMaxLife;		//�ő����
	int nLife;			//����
	PARTICLE_TYPE type;	//�p�[�e�B�N���̎��
	bool bUse;			//�g�p���Ă��邩�ǂ���e
}Particle;

//-----------------------------------------
//static�ϐ�
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_TEX] = {};		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static Particle s_aParticle[MAX_PARTCLE];
static float s_fAngle;

//-----------------------------------------
//�v���g�R�[��
//-----------------------------------------
#if 0
float angle = rand() / (float)RAND_MAX * 2.0f * D3DX_PI;
float length = 50.0f + rand() / (float)RAND_MAX * 50.0f;
float diffX = sinf(angle) * length;
float diffY = cosf(angle) * length;
float speed = ? ? ? ? ;
float life = length / speed;

pParticle->pos.x = pos.x + diffX;
pParticle->pos.y = pos.y + diffY;
pParticle->move.x = sinf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.y = cosf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.z = 0.0f;
pParticle->col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
pParticle->fRaduus = 10.0f;
pParticle->nLife = (rand() % 20) + 10;
#endif

//=========================================
//�p�[�e�B�N���̏���������
//=========================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[0]);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[1]);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[2]);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_SPLITBALL_ATTACK]);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		s_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].fRaduus = 0.0f;
		s_aParticle[nCntParticle].nLife = 0;
		s_aParticle[nCntParticle].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	VERTEX_2D* vtxBase = pVtx;

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		assert(pVtx + 3 < vtxBase + (4 * MAX_PARTCLE));

		pParticle = &(s_aParticle[nCntParticle]);

		//���_���W�̐ݒ�
		SetRectPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		//rhw�̐ݒ�
		//�e�N�X�`�����W�̐ݒ�
		//���_�J���[�̐ݒ�
		SetupRectDefault(pVtx, &(pParticle->col));

		SetupRectDefault(pVtx, &(s_aParticle[nCntParticle].col));

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
//�p�[�e�B�N���̏I������
//=========================================
void UninitParticle(void)
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//=========================================
//�p�[�e�B�N���̍X�V����
//=========================================
void UpdateParticle(void)
{
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = s_aParticle + nCntParticle;

		if (!pParticle->bUse)
		{	//�p�[�e�B�N�����g�p����Ă��Ȃ�
			pVtx += 4;
			continue;
		}

		//�p�[�e�B�N�����g�p����Ă���
		//�ʒu�̍X�V
		pParticle->pos += pParticle->move;

		////�̗͂̍X�V
		//pParticle->nLife--;
		//if (pParticle->nLife <= 0)
		//{
		//	pParticle->bUse = false;
		//}

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_JUMP:	//�v���C���[�̃W�����v�p�[�e�B�N��	//�񋓌^�ɕύX����B
			//�̗͂̍X�V
			pParticle->nLife--;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			//�����x�̍X�V
			pParticle->col.a -= (float)1 / pParticle->nMaxLife;
			break;
		case PARTICLE_PLAYER_DEATH:
		{
			Player *pPlayer = GetPlayer();
			
			//�������̏���
			if (pPlayer->state == PLAYERSTATE_REVIVAL)
			{
				if(pParticle->nLife == pParticle->nMaxLife)
				{	//�������̍ŏ��ɂ����ʂ鏈��
					
					//D3DXVECTOR3 v = pPlayer->pos - pParticle->pos;	//�v���C���[�̎��S�ʒu�ƁA�e�j�Ђ̈ʒu�̍������߂�
					D3DXVECTOR3 v = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f) - pParticle->pos;	//�v���C���[�̎��S�ʒu�ƁA�e�j�Ђ̈ʒu�̍������߂�
					pParticle->move.x = v.x / pParticle->nMaxLife;	//nLife���s�������ɒ����ɏW�߂�悤�ɂ���
					pParticle->move.y = v.y / pParticle->nMaxLife;	//nLife���s�������ɒ����ɏW�߂�悤�ɂ���
				}
				pParticle->nLife--;
				if (pParticle->nLife < 0)
				{
					pPlayer->RevivalInterval--;
					pParticle->bUse = false;
				}
			}
			else
			{
				//�ړ��ʂ��X�V(����)
				pParticle->move.x += (0 - pParticle->move.x) * 0.015f;
				pParticle->move.y += (0 - pParticle->move.y) * 0.015f;
			}
		}
			break;
		case PARTICLE_SPLITBALL_ATTACK:
			pParticle->fRaduus--;
			if (pParticle->fRaduus <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		default:
			break;
		}

		pVtx[0].pos.x = pParticle->pos.x - pParticle->fRaduus;
		pVtx[0].pos.y = pParticle->pos.y - pParticle->fRaduus;
		pVtx[0].pos.z = pParticle->pos.z + 0.0f;

		pVtx[1].pos.x = pParticle->pos.x + pParticle->fRaduus;
		pVtx[1].pos.y = pParticle->pos.y - pParticle->fRaduus;
		pVtx[1].pos.z = pParticle->pos.z + 0.0f;

		pVtx[2].pos.x =pParticle->pos.x - pParticle->fRaduus;
		pVtx[2].pos.y =pParticle->pos.y + pParticle->fRaduus;
		pVtx[2].pos.z =pParticle->pos.z + 0.0f;

		pVtx[3].pos.x = pParticle->pos.x + pParticle->fRaduus;
		pVtx[3].pos.y = pParticle->pos.y + pParticle->fRaduus;
		pVtx[3].pos.z = pParticle->pos.z + 0.0f;

		pVtx[0].col = pParticle->col;
		pVtx[1].col = pParticle->col;
		pVtx[2].col = pParticle->col;
		pVtx[3].col = pParticle->col;

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
//�p�[�e�B�N���̕`�揈��
//=========================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���g
	Particle *pParticle;

	//�`��̂��߂̐ݒ�
	pDevice = InitDraw(pDevice, s_pVtxBuff);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (s_aParticle[nCntParticle].bUse == true)
		{//�p�[�e�B�N�����g�p����Ă���
			switch (pParticle->type)
			{
			case PARTICLE_PLAYER_JUMP:	//�v���C���[�̃W�����v��
				//�e�N�X�`�������Z�����œ\��t���ĕ`�悷��
				pDevice = AddSyntheticDraw(pDevice, s_pTexture[pParticle->type], nCntParticle);
				break;
			case PARTICLE_SPLITBALL_ATTACK:	//�ʂ�鋅�̍U����
				//�e�N�X�`����\��t���ĕ`�悷��
				pDevice = SetDraw(pDevice, s_pTexture[pParticle->type], nCntParticle);
				break;
			case PARTICLE_PLAYER_DEATH:
				pDevice = SetDraw(pDevice, s_pTexture[pParticle->type], nCntParticle);
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//=========================================
//�p�[�e�B�N���̐ݒ菈��
//=========================================
void SetParticle(D3DXVECTOR3 pos, PARTICLE_TYPE type)
{
	Particle *pParticle;
	VERTEX_2D *pVtx;
	int nCntParticle;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (pParticle->bUse)
		{//�p�[�e�B�N�����g�p����Ă�
			pVtx += 4;
			continue;
		}

		//�p�[�e�B�N�����g�p����Ă��Ȃ�
		pParticle->	type = type;
		switch (type)
		{
		case PARTICLE_PLAYER_JUMP:	//�v���C���[�̃W�����v�p�[�e�B�N��
			pParticle->pos = pos;
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.1f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.1f);
			pParticle->move.z = 0.0f;
			pParticle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pParticle->fRaduus = 10.0f;
			pParticle->nMaxLife = 50;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_SPLITBALL_ATTACK:	//�ʂ�鋅�̍U���p�[�e�B�N��
			pParticle->pos = pos;
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->fRaduus = 40.0f;
			pParticle->nMaxLife = 50;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_PLAYER_DEATH:
			pParticle->pos = pos;
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 5.0f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 5.0f);
			pParticle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pParticle->fRaduus = 7.0f;
			pParticle->nMaxLife = 75;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		default:
			assert(false);
			break;
		}

		pParticle->bUse = true;

		SetRectPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		pVtx[0].col = pParticle->col;
		pVtx[1].col = pParticle->col;
		pVtx[2].col = pParticle->col;
		pVtx[3].col = pParticle->col;

		pVtx += 4;
		break;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}
