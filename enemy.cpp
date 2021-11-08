//====================================
// 
// �G�Ɋւ���֐�
// Author Yuda Kaito
// 
//====================================
#include "main.h"
#include "setup.h"
#include "enemy.h"
#include "particle.h"
#include "effect.h"
#include "player.h"
#include "block.h"
#include "score.h"
#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define EXTENDBALL_TEX	"data/TEXTURE/enemy001.png"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[ENEMYTYPE_MAX] = {};	// �e�N�X�`���o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// ���_�o�b�t�@
static Enemy s_aEnemy[MAX_ENEMY];							// �G�̏��
static int s_nEnemyCnt = 0;									// �G�̐�
int score[] =
{
	100,	// �ʂ�鋅�ŏ�
	50,		// �ʂ�鋅����
	25,		// �ʂ�鋅
	40,		// �L�т�~�A�ォ�牺
	40,		// �L�т�~�A�������
	40,		// �L�т�~�A������E
	40,		// �L�т�~�A�E���獶
	10,		// ���i���钷���`�A�ォ�牺
	10,		// ���i���钷���`�A�������
	10,		// ���i���钷���`�A������E
	10,		// ���i���钷���`�A�E���獶
	0,		// �_���[�W��

};

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
static void NeutralEnemy(Enemy *pEnemy);	// �ҋ@����
static void AttackEnemy	(Enemy *pEnemy);	// �U������
static void DieEnemy	(Enemy *pEnemy);	// ���S����

//====================================
// �G�̏���������
//====================================
void InitEnemy(void)
{
	Enemy *pEnemy;	// �G�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[SPLITBALL_FIRST]);

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[SPLITBALL_SECOND]);

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[SPLITBALL_LAST]);

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		TRIANGLE_TEX,
		&s_pTexture[REFLECT_TRIANGLE]);

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[BALL_HOMING]);

	// �\���̂̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		pEnemy->fWidth = 5.0f;								// �摜�̕�
		pEnemy->fHeight = 5.0f;								// �摜�̍���
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		pEnemy->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		pEnemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		pEnemy->nLife = 100;								// ����
		pEnemy->bUse = false;								// �g�p����Ă��邩�ǂ���
		pEnemy->nAtkInterval = 0;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		InitRect(pVtx);
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
// �G�̏I������
//====================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		// �e�N�X�`���̔j��
		if (s_pTexture[nCntEnemy] != NULL)
		{
			s_pTexture[nCntEnemy]->Release();
			s_pTexture[nCntEnemy] = NULL;
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
// �G�̍X�V����
//====================================
void UpdateEnemy(void)
{
	Enemy *pEnemy;			// �G�̃|�C���^

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// �|�C���^�ɕύX
		pEnemy = &(s_aEnemy[i]);

		if (!(pEnemy->bUse))
		{
			continue;
		}

		switch (pEnemy->state)
		{
		case ENEMYSTATE_SUMMON:
			pEnemy->col.a += 0.05f;
			if (pEnemy->col.a >= 1.0f)
			{
				pEnemy->state = ENEMYSTATE_NEUTRAL;
			}
			break;
		case ENEMYSTATE_NEUTRAL:	// �ҋ@����
			NeutralEnemy(pEnemy);
			break;
		case ENEMYSTATE_ATTACK:		// �U������
			AttackEnemy(pEnemy);
			break;
		default:
			assert(false);
			break;
		}
		// �G�����񂾏ꍇ
		if (pEnemy->nLife <= 0)
		{
			// ���񂾂Ƃ��̏���
			DieEnemy(pEnemy);
		}

		// ���W�̈ړ�����
		pEnemy->pos += pEnemy->move;

		VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;	// �|�C���^�̈ړ�

		if (pEnemy->state != ENEMYSTATE_SUMMON)
		{
			// ���ꂼ��̒��_���W�̓����蔻��
			CollisionBlockEnemy(pEnemy, pVtx[0].pos, pVtx[1].pos);
			CollisionBlockEnemy(pEnemy, pVtx[0].pos, pVtx[2].pos);
			CollisionBlockEnemy(pEnemy, pVtx[1].pos, pVtx[3].pos);
			CollisionBlockEnemy(pEnemy, pVtx[2].pos, pVtx[3].pos);
		}

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
		// �J���[�̐ݒ�
		SetRectColor(pVtx, &(pEnemy->col));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();
	}
}

//====================================
// �G�̑ҋ@����
//====================================
static void NeutralEnemy(Enemy *pEnemy)
{
	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// �ʂ�鋅�̍ŏ�
	case SPLITBALL_SECOND:	// �ʂ�鋅�̂Q���
	case SPLITBALL_LAST:	// �ʂ�鋅�̍Ō�
	{
		pEnemy->col = D3DXCOLOR(0.9f, 0.35f, 0.4f, 1.0f);
		if (pEnemy->nNeutralInterval == 0)
		{
			Player *pPlayer = GetPlayer();	// �v���C���[�̃|�C���^
			float fRotDest;
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = -(pEnemy->move.x + sinf(fRotDest) * 5.0f);
			pEnemy->move.y = -(pEnemy->move.y + cosf(fRotDest) * 5.0f);
		}
		else if(pEnemy->nNeutralInterval >= 30)
		{
			pEnemy->state = ENEMYSTATE_ATTACK;
		}
		pEnemy->nNeutralInterval++;
	}
		break;
	case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
	case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
	case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
	case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
		pEnemy->state = ENEMYSTATE_ATTACK;
		break;
	case REFLECT_TRIANGLE:
	{	
		pEnemy->nNeutralInterval++;
		if (pEnemy->nNeutralInterval >= 100)
		{
			Player *pPlayer = GetPlayer();	// �v���C���[�̃|�C���^
			float fRotDest;
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
		}
	}
		break;
	case BALL_HOMING:	// �Â��Ǐ]������~
	{	
		pEnemy->state = ENEMYSTATE_ATTACK;
	}
	break;
	case DAMEGE_WALL:		// �_���[�W��
		break;
	default:
		assert(false);		// �{���ʂ�Ȃ��ꏊ
		break;
	}
}

//====================================
// �G�̍U������
//====================================
static void AttackEnemy(Enemy *pEnemy)
{
	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// �ʂ�鋅�̍ŏ�
	{
		if (pEnemy->nAtkInterval == 0)
		{
			pEnemy->move.x *= -3.0f;
			pEnemy->move.y *= -3.0f;
		}
		if (pEnemy->nAtkInterval % 5 == 0)
		{
			// �p�[�e�B�N��
			SetParticle(pEnemy->pos, PARTICLE_SPLITBALL_ATTACK);
		}
		pEnemy->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
		break;
	case SPLITBALL_SECOND:	// �ʂ�鋅�̂Q���
	{
		if (pEnemy->nAtkInterval == 0)
		{
			pEnemy->move.x *= -4.0f;
			pEnemy->move.y *= -4.0f;
		}
		if (pEnemy->nAtkInterval % 5 == 0)
		{
			// �p�[�e�B�N��
			SetParticle(pEnemy->pos, PARTICLE_SPLITBALL_ATTACK);
		}
		pEnemy->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
		break;
	case SPLITBALL_LAST:	// �ʂ�鋅�̍Ō�
	{
		if (pEnemy->nAtkInterval == 0)
		{
			pEnemy->move.x *= -5.0f;
			pEnemy->move.y *= -5.0f;
		}
		if (pEnemy->nAtkInterval % 5 == 0)
		{
			// �p�[�e�B�N��
			SetParticle(pEnemy->pos, PARTICLE_SPLITBALL_ATTACK);
		}
		pEnemy->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
		break;
	case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
		pEnemy->move.y = 5.0f;
		break;
	case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
		pEnemy->move.y = -5.0f;
		break;
	case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
		pEnemy->move.x = 5.0f;
		break;
	case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
		pEnemy->move.x = -5.0f;
		break;
	case REFLECT_TRIANGLE:
		break;
	case BALL_HOMING:
		if (pEnemy->nAtkInterval % 1 == 0)
		{
			// �p�[�e�B�N��
			SetParticle(pEnemy->pos, PARTICLE_BALL_HOMING_ATTACK);
		}
		if (pEnemy->nAtkInterval % 20 == 0)
		{
			Player *pPlayer = GetPlayer();	// �v���C���[�̃|�C���^
			float fRotDest;
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = pEnemy->move.x + sinf(fRotDest) * 0.1f * (float)(pEnemy->nAtkInterval / 20);
			pEnemy->move.y = pEnemy->move.y + cosf(fRotDest) * 0.1f * (float)(pEnemy->nAtkInterval / 20);
		}
		break;
	case DAMEGE_WALL:		// �_���[�W��
		break;
	default:
		assert(false);		// �{���ʂ�Ȃ��ꏊ
		break;
	}
	pEnemy->nAtkInterval++;			// �U���Ԋu�̃J�E���g
}

//====================================
// �G�̎��S����
//====================================
static void DieEnemy(Enemy *pEnemy)
{
//	assert(sizeof(score)/sizeof(score[0]) != ENEMYTYPE_MAX);
	AddScore(score[pEnemy->type]);

	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// �ʂ�鋅�̍ŏ�
		SetEnemy(pEnemy->pos, SPLITBALL_SECOND);
		SetEnemy(pEnemy->pos, SPLITBALL_SECOND);
		AddScore(100);
		pEnemy->bUse = false;
		break;
	case SPLITBALL_SECOND:	// �ʂ�鋅�̂Q���
		SetEnemy(pEnemy->pos, SPLITBALL_LAST);
		SetEnemy(pEnemy->pos, SPLITBALL_LAST);
		AddScore(100);
		pEnemy->bUse = false;
		break;
	case SPLITBALL_LAST:	// �ʂ�鋅�̍Ō�
		AddScore(100);
		pEnemy->bUse = false;	// ��������
		break;
	case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
	case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
	case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
	case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
	case REFLECT_TRIANGLE:
	case BALL_HOMING:
		pEnemy->bUse = false;
		break;
	case DAMEGE_WALL:		// �_���[�W��
		break;
	default:
		assert(false);	// �{���ʂ�Ȃ��ꏊ
		break;
	}
}

//====================================
// �G�̕`�揈��
//====================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Enemy *pEnemy;				// �G�̃|�C���^
	int nCntEnemy;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	InitDraw(pDevice, s_pVtxBuff);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);
		if (pEnemy->bUse == true)
		{
			//�e�N�X�`���̕`��
			RectDraw(pDevice, s_pTexture[pEnemy->type], nCntEnemy * 4);
		}
	}
}

//====================================
// �G�̐ݒ菈��
//====================================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy *pEnemy = &(s_aEnemy[i]);

		// �g�p���Ă��Ȃ��Ȃ�ʂ�Ȃ�
		if (pEnemy->bUse)
		{
			continue;
		}

		pEnemy->state = ENEMYSTATE_SUMMON;				// �G�l�~�[��������ԂɈڍs
		pEnemy->type = type;							// ��ސݒ�
		pEnemy->pos = pos;								// �ʒu�ݒ�
		pEnemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����ݒ�
		pEnemy->move = D3DXVECTOR3(0.0f,0.0f,0.0f);		// �ړ��ʐݒ�
		pEnemy->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);// �F�ݒ�
		pEnemy->nLife = 1;								// �����̐ݒ�
		pEnemy->nNeutralInterval = 0;					// �ҋ@�Ԋu
		pEnemy->nAtkInterval = 0;						// �U���Ԋu�̐ݒ�
		pEnemy->bUse = true;							// �g�p���Ă����Ԃɂ���

		s_nEnemyCnt++;	// �G�̐����v���X

		switch (pEnemy->type)
		{
		case SPLITBALL_FIRST:	// �ʂ�鋅�̍ŏ�
			// �摜�̑傫���ݒ�
			pEnemy->fHeight	= 40.0f;	// ����
			pEnemy->fWidth = 40.0f;	// ��
			pEnemy->col = D3DXCOLOR(0.9f, 0.35f, 0.4f, 0.0f);
			break;
		case SPLITBALL_SECOND:	// �ʂ�鋅��2���
			// �ʒu�������U�炷
			pEnemy->pos.x = pos.x + (float)(rand() % 50) - 25.0f;
			pEnemy->pos.y = pos.y + (float)(rand() % 50) - 25.0f;
			// �摜�̑傫���ݒ�
			pEnemy->fHeight	= 30.0f;	// ����
			pEnemy->fWidth = 30.0f;	// ��
			pEnemy->col = D3DXCOLOR(0.9f, 0.35f, 0.4f, 0.0f);
			break;
		case SPLITBALL_LAST:	// �ʂ�鋅�̍Ō�
			// �ʒu�������U�炷
			pEnemy->pos.x = pos.x + (float)(rand() % 20) - 10.0f;
			pEnemy->pos.y = pos.y + (float)(rand() % 20) - 10.0f;
			// �摜�̑傫���ݒ�
			pEnemy->fHeight	= 20.0f;	// ����
			pEnemy->fWidth = 20.0f;		// ��
			pEnemy->col = D3DXCOLOR(0.9f, 0.35f, 0.4f, 0.0f);
			break;
		case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
		case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
			// �摜�̑傫���ݒ�
			pEnemy->fHeight = 10.0f;	// ����
			pEnemy->fWidth = 5.0f;		// ��
			break;
		case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
		case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
			// �摜�̑傫���ݒ�
			pEnemy->fHeight	= 5.0f;		// ����
			pEnemy->fWidth = 10.0f;		// ��
			break;
		case REFLECT_TRIANGLE:
			// �摜�̑傫���ݒ�
			pEnemy->fHeight = 10.0f;	// ����
			pEnemy->fWidth = 10.0f;	// ��
			break;
		case BALL_HOMING:
			pEnemy->fHeight = 10.0f;	// ����
			pEnemy->fWidth = 10.0f;	// ��
			break;
		case DAMEGE_WALL:		// �_���[�W��
			break;
		default:
			assert(false);		// �{���ʂ�Ȃ��ꏊ
			break;
		}

		// ���������ɕύX
		pEnemy->col.a = 0.0f;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;	// �|�C���^���ړ�

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}

//====================================
// �G�̓����菈��
//====================================
void HitEnemy(int nCntEnemy)
{
	Enemy *pEnemy = &(s_aEnemy[nCntEnemy]);		// �G�̃|�C���^

	pEnemy->nLife --;

	if (pEnemy->nLife <= 0)
	{// HP��0�ɂȂ�����
	}

	// �G�̐��𐔂��Ȃ���
	s_nEnemyCnt = 0;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			s_nEnemyCnt++;
		}
	}
	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// �ʂ�鋅�ŏ�
		break;
	case SPLITBALL_SECOND:	// �ʂ�鋅����
		break;
	case SPLITBALL_LAST:	// �ʂ�鋅
		break;
	case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
		break;
	case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
		break;
	case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
		break;
	case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
		break;
	case BALL_HOMING:
		break;
	case DAMEGE_WALL:		// �_���[�W��
		break;
	default:
		assert(false);		// �{���ʂ�Ȃ��ꏊ
		break;
	}
}

//====================================
// �G�̎擾����
//====================================
Enemy *GetEnemy(void)
{
	// �Ԃ�l
	return s_aEnemy;
}

//====================================
// �G�̐��擾����
//====================================
int GetCntEnemy(void)
{
	return s_nEnemyCnt;
}

//====================================
// �v���C���[�Ƃ̓����蔻�菈��
//====================================
bool CollisionEnemy(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	// �����蔻�菈��
	Enemy *pEnemy;

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);
		if (pEnemy->bUse == true)
		{// �u���b�N�������Ă���
			 // ��
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_UP, *pEnemy) ||
				CrossingEnemy(&(pos1), &(pos2), POSITION_DWON, *pEnemy) ||
				CrossingEnemy(&(pos1), &(pos2), POSITION_LEFT, *pEnemy) ||
				CrossingEnemy(&(pos1), &(pos2), POSITION_RIGHT, *pEnemy))
			{
				HitPlayer(1);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	return bisLanding;
}

//====================================
// �G�l�~�[�厲�̐����̌������菈��
//====================================
bool CrossingEnemy(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, JUDGE_POSITION position, Enemy enemy)
{
	// �����蔻�菈��
	Enemy *pEnemy = &(enemy);
	Segment vEnemy;
	Segment vTarget;

	switch (position)
	{
	case POSITION_UP:
		// �u���b�N�̃x�N�g���̊l��
		vEnemy.s  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.v = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z) - vEnemy.s;
		break;
	case POSITION_DWON:
		// �u���b�N�̃x�N�g���̊l��
		vEnemy.s  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.v = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.s;
		break;
	case POSITION_LEFT:
		// �u���b�N�̃x�N�g���̊l��
		vEnemy.s  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.v = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.s;
		break;
	case POSITION_RIGHT:
		// �u���b�N�̃x�N�g���̊l��
		vEnemy.s  = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.v = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.s;
		break;
	default:
		assert(false);
		break;
	}

	// ��Ώۂ̃x�N�g���̊l��
	vTarget.s  = *pPos2;
	vTarget.v = *pPos1 - *pPos2;

	// �x�N�g���̎n�_���m�̋����B
	D3DXVECTOR3 v = vTarget.s - vEnemy.s;

	// �u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
	float Bv_Tv = D3DXVec2Cross(&(vEnemy.v), &(vTarget.v));
	if (Bv_Tv == 0.0f)
	{
		// ���s�ł���B
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(vEnemy.v));
	float v_Tv = D3DXVec2Cross(&(v), &(vTarget.v));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	float eps = 0.000001f;	// �����蔻����ɂ߂�

	return !((hit1 + eps < 0.0f) || (hit1 - eps > 1.0f) || (hit2 + eps < 0.0f) || (hit2 - eps > 1.0f));
}