//=========================================
// 
// �p�[�e�B�N���̏���
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "setup.h"
#include "particle.h"
#include <assert.h>
#include "enemy.h"

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define MAX_PARTCLE	(1024)	// �p�[�e�B�N���̍ő吔
#define MAX_TEX		(MAX_PARTICLE)		// �e�N�X�`���̎��
#define JUMP_TEX	"data/TEXTURE/jump.png"

//-----------------------------------------
// �p�[�e�B�N���\����
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// �p�x
	D3DXCOLOR col;		// �F
	float fRaduus;		// ���a
	float fLength;		// ���S���W����̔��a
	float fAngle;		// �p�x�̎Z�o
	int nMaxLife;		// �ő����
	int nLife;			// ����
	PARTICLE_TYPE type;	// �p�[�e�B�N���̎��
	bool bUse;			// �g�p���Ă��邩�ǂ���e
}Particle;

//-----------------------------------------
// static�ϐ�
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_PARTICLE] = {};		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
static Particle s_aParticle[MAX_PARTCLE];
static float s_fAngle;

//-----------------------------------------
// �v���g�R�[��
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
// �p�[�e�B�N���̏���������
//=========================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		JUMP_TEX,
		&s_pTexture[PARTICLE_PLAYER_JUMP]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_SPLITBALL_ATTACK]);
	
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING00_ATTACK]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING01_ATTACK]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING00_DIE]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING01_DIE]);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		s_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].fRaduus = 0.0f;
		s_aParticle[nCntParticle].nLife = 0;
		s_aParticle[nCntParticle].bUse = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	VERTEX_2D* vtxBase = pVtx;

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		assert(pVtx + 3 < vtxBase + (4 * MAX_PARTCLE));

		pParticle = &(s_aParticle[nCntParticle]);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(pParticle->col));

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		
		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
// �p�[�e�B�N���̏I������
//=========================================
void UninitParticle(void)
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
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
// �p�[�e�B�N���̍X�V����
//=========================================
void UpdateParticle(void)
{
	Particle *pParticle;

	for (int i = 0; i < MAX_PARTCLE; i++)
	{
		pParticle = s_aParticle + i;

		if (!pParticle->bUse)
		{	// �p�[�e�B�N�����g�p����Ă��Ȃ�
			continue;
		}

		// �p�[�e�B�N�����g�p����Ă���
		// �ʒu�̍X�V
		pParticle->pos += pParticle->move;

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_JUMP:	// �v���C���[�̃W�����v�p�[�e�B�N��	// �񋓌^�ɕύX����B
			pParticle->nLife--;			// �̗͂̍X�V
			pParticle->col.a -= (float)1.0f / pParticle->nMaxLife;			// �����x�̍X�V
			pParticle->fRaduus += 1.5f;	// ���a�̊g��
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		case  PARTICLE_PLAYER_WALK:		// �v���C���[�̈ړ�
			pParticle->nLife -= 1;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_PLAYER_AIR:			// �v���C���[�̋󒆋O��
			pParticle->nLife--;
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_PLAYER_DEATH:
		{
			Player *pPlayer = GetPlayer();
			
			// �������̏���
			if (pPlayer->state == PLAYERSTATE_REVIVAL)
			{
				if(pParticle->nLife == pParticle->nMaxLife)
				{	// �������̍ŏ��ɂ����ʂ鏈��
					
					// D3DXVECTOR3 v = pPlayer->pos - pParticle->pos;	// �v���C���[�̎��S�ʒu�ƁA�e�j�Ђ̈ʒu�̍������߂�
					D3DXVECTOR3 v = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f) - pParticle->pos;	// �v���C���[�̎��S�ʒu�ƁA�e�j�Ђ̈ʒu�̍������߂�
					pParticle->move.x = v.x / pParticle->nMaxLife;	// nLife���s�������ɒ����ɏW�߂�悤�ɂ���
					pParticle->move.y = v.y / pParticle->nMaxLife;	// nLife���s�������ɒ����ɏW�߂�悤�ɂ���
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
				// �ړ��ʂ��X�V(����)
				pParticle->move.x += (0 - pParticle->move.x) * 0.03f;
				pParticle->move.y += (0 - pParticle->move.y) * 0.03f;
			}
		}
			break;
		case PARTICLE_SPLITBALL_ATTACK:
			pParticle->fRaduus -= 2.0f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_BALL_HOMING00_ATTACK:	// �Â��Ǐ]������~�̍U�� 
			// �̗͂̍X�V
			pParticle->nLife--;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			// �����x�̍X�V
			pParticle->col.g -= (float)1 / pParticle->nMaxLife;
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING01_ATTACK:	// �Ǐ]������~�̍U�� 
			// �̗͂̍X�V
			pParticle->nLife--;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			// �����x�̍X�V
			pParticle->col.g -= (float)1 / pParticle->nMaxLife;
			pParticle->col.r -= (float)1 / pParticle->nMaxLife;
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			break;
		case PARTICLE_GOSTRAIGHT_DIE:	// ���i���钷���`���S��
			pParticle->fRaduus -= 0.15f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_BALL_HOMING00_DIE:		// �Â��Ǐ]������~�̎��S��
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			pParticle->fRaduus -= 0.05f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_BALL_HOMING01_DIE:		// �Ǐ]������~�̎��S��
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			pParticle->fRaduus -= 0.05f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		default:
			break;
		}

		// ���S���W�����̒������Z�o����B
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// ���S���W�����̒��_�̊p�x���Z�o����
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		// ���_���W�̐ݒ�
		SetRectCenterRotPos(pVtx, pParticle->pos, pParticle->rot, pParticle->fAngle, pParticle->fLength);
		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(pParticle->col));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

	}
}

//=========================================
// �p�[�e�B�N���̕`�揈��
//=========================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	Particle *pParticle;

	// �`��̂��߂̐ݒ�
	InitDraw(pDevice, s_pVtxBuff);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (s_aParticle[nCntParticle].bUse == true)
		{// �p�[�e�B�N�����g�p����Ă���
			switch (pParticle->type)
			{
			case PARTICLE_PLAYER_JUMP:	// �v���C���[�̃W�����v��
			case PARTICLE_BALL_HOMING00_ATTACK:	// �Â��Ǐ]������~�̍U�� 
			case PARTICLE_SPLITBALL_ATTACK:	// �ʂ�鋅�̍U����
			case PARTICLE_PLAYER_AIR:			// �v���C���[�̋󒆋O��
			case PARTICLE_PLAYER_DEATH:		// �v���C���[�̎��S��
			case PARTICLE_GOSTRAIGHT_DIE:// ���i���钷���`���S��
			case PARTICLE_PLAYER_WALK:		// �v���C���[�̈ړ�
			case PARTICLE_BALL_HOMING01_ATTACK:	// �Ǐ]������~�̍U�� 
			case PARTICLE_BALL_HOMING00_DIE:		// �Â��Ǐ]������~�̎��S��
			case PARTICLE_BALL_HOMING01_DIE:		// �Ǐ]������~�̎��S��
				// �e�N�X�`����\��t���ĕ`�悷��
				RectDraw(pDevice, s_pTexture[pParticle->type], nCntParticle * 4);
				break;
				//// �e�N�X�`�������Z�����œ\��t���ĕ`�悷��
				//RectAddDraw(pDevice, s_pTexture[pParticle->type], nCntParticle * 4);
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//=========================================
// �p�[�e�B�N���̐ݒ菈��
//=========================================
void SetParticle(D3DXVECTOR3 pos, PARTICLE_TYPE type)
{
	Particle *pParticle;
	VERTEX_2D *pVtx;
	int nCntParticle;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (pParticle->bUse)
		{// �p�[�e�B�N�����g�p����Ă�
			pVtx += 4;
			continue;
		}

		// �p�[�e�B�N�����g�p����Ă��Ȃ�
		pParticle->	type = type;
		switch (type)
		{
		case PARTICLE_PLAYER_JUMP:	// �v���C���[�̃W�����v�p�[�e�B�N��
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->move.z = 0.0f;
			pParticle->col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
			pParticle->fRaduus = 10.0f;
			pParticle->nMaxLife = 25;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case  PARTICLE_PLAYER_WALK:		// �v���C���[�̈ړ�
			pParticle->pos = pos;
			pParticle->col = D3DXCOLOR(0.5f, 0.35f, 0.25f, 1.0f);
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->fRaduus = 3.0f;
			pParticle->nMaxLife = 25;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_PLAYER_AIR:			// �v���C���[�̋󒆋O��
		{
			Player *pPlayer = GetPlayer();
			pParticle->pos = pos;
			pParticle->rot = pPlayer->rot;
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
			pParticle->fRaduus = 3.5f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
		}
			break;
		case PARTICLE_PLAYER_DEATH:
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 10.0f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 10.0f);
			pParticle->col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
			pParticle->fRaduus = 7.0f;
			pParticle->nMaxLife = 40;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_SPLITBALL_ATTACK:	// �ʂ�鋅�̍U���p�[�e�B�N��
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->fRaduus = 40.0f;
			pParticle->nMaxLife = 50;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING00_ATTACK:	// �Â��Ǐ]������~�̍U�� 
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 0.8f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->fRaduus = 7.5f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING01_ATTACK:	// �Ǐ]������~�̍U�� 
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->fRaduus = 8.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_GOSTRAIGHT_DIE:// ���i���钷���`���S��
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->fRaduus = 5.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING00_DIE:		// �Â��Ǐ]������~�̎��S��
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->fRaduus = 5.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING01_DIE:		// �Ǐ]������~�̎��S��
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->fRaduus = 5.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		default:
			assert(false);
			break;
		}

		// ���S���W�����̒������Z�o����B
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// ���S���W�����̒��_�̊p�x���Z�o����
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		pParticle->bUse = true;

		SetRectCenterPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		SetRectColor(pVtx, &(pParticle->col));

		pVtx += 4;
		break;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}
