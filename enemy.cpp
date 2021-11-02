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

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[ENEMYTYPE_MAX] = {};		// �e�N�X�`���o�b�t�@
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
static void NeutralEnemy(Enemy *pEnemy);				// �ҋ@����
static void AttackEnemy(Enemy *pEnemy);				// �U������
static void DieEnemy(Enemy *pEnemy);					// ���S����

//====================================
// �G�̏���������
//====================================
void InitEnemy(void)
{
	Enemy *pEnemy;				// �G�̃|�C���^
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
		NULL,
		&s_pTexture[EXTENDBALL_UP]);

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
		pEnemy = &(s_aEnemy[nCntEnemy]);

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
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	Enemy *pEnemy;			// �G�̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		if (pEnemy->bUse == true)
		{
			pEnemy->nAtkInterval++;

			// �ҋ@����
			NeutralEnemy(pEnemy);
			// �U������
			AttackEnemy(pEnemy);

			// �G�����񂾏ꍇ
			if (pEnemy->nLife <= 0)
			{
				// ���񂾂Ƃ��̏���
				DieEnemy(pEnemy);
			}

			// ���W�̈ړ�����
			pEnemy->pos += pEnemy->move;

			// ���ꂼ��̒��_���W�̓����蔻��
			CollisionBlockEnemy(pEnemy, pVtx[0].pos, pVtx[1].pos);
			CollisionBlockEnemy(pEnemy, pVtx[1].pos, pVtx[2].pos);
			CollisionBlockEnemy(pEnemy, pVtx[2].pos, pVtx[3].pos);
			CollisionBlockEnemy(pEnemy, pVtx[3].pos, pVtx[0].pos);

			// ���_���W�̐ݒ�
			switch (pEnemy->type)
			{
			case SPLITBALL_FIRST:	// �ʂ�鋅�̍ŏ�
			case SPLITBALL_SECOND:	// �ʂ�鋅�̂Q���
			case SPLITBALL_LAST:	// �ʂ�鋅�̍Ō�

				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);

				break;
			case EXTENDBALL_UP:		// �L�т�~�A�ォ�牺

				SetRectUpRightPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case EXTENDBALL_DWON:	// �L�т�~�A�������
				break;
			case EXTENDBALL_LEFT:	// �L�т�~�A������E
				break;
			case EXTENDBALL_RIGHT:	// �L�т�~�A�E���獶
				break;
			case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case DAMEGE_WALL:		// �_���[�W��
				break;
			default:
				assert(false);	// �{���ʂ�Ȃ��ꏊ
				break;
			}

			// �J���[�̐ݒ�
			SetRectColor(pVtx, &(pEnemy->col));

		}
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
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
		if (pEnemy->nAtkInterval >= 150)
		{
			float fRotDest;
			Player *pPlayer = GetPlayer();	// �v���C���[�̃|�C���^

			// ���������v���C���[�̔��Ε�����move
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = sinf(fRotDest) * 0.5f;
			pEnemy->move.y = cosf(fRotDest) * 0.5f;

			pEnemy->col = D3DXCOLOR(0.9f, 0.35f, 0.4f, 1.0f);
		}
		break;
	case EXTENDBALL_UP:		// �L�т�~�A�ォ�牺
		break;
	case EXTENDBALL_DWON:	// �L�т�~�A�������
		break;
	case EXTENDBALL_LEFT:	// �L�т�~�A������E
		break;
	case EXTENDBALL_RIGHT:	// �L�т�~�A�E���獶
		break;
	case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
		break;
	case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
		break;
	case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
		break;
	case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
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
	Player *pPlayer = GetPlayer();	// �v���C���[�̃|�C���^
	float fRotDest;					// �G�ƃv���C���[�����Ԑ��̊p�x

	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// �ʂ�鋅�̍ŏ�
	case SPLITBALL_SECOND:	// �ʂ�鋅�̂Q���
	case SPLITBALL_LAST:	// �ʂ�鋅�̍Ō�
		if (pEnemy->nAtkInterval <= 150 && pEnemy->nAtkInterval % 5 == 0)
		{
			// �p�[�e�B�N��
			SetParticle(pEnemy->pos, PARTICLE_SPLITBALL_ATTACK);

		}
		if (pEnemy->nAtkInterval >= 300)
		{
			// �ːi�U��
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = sinf(fRotDest) * 20.0f;
			pEnemy->move.y = cosf(fRotDest) * 20.0f;

			pEnemy->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			// �U���T�C�N���̃��Z�b�g
			pEnemy->nAtkInterval = 0;
		}
		break;
	case EXTENDBALL_UP:		// �L�т�~�A�ォ�牺
		pEnemy->fHeight += 1.5f;
		break;
	case EXTENDBALL_DWON:	// �L�т�~�A�������
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fHeight += 5.0f;
		}
		break;
	case EXTENDBALL_LEFT:	// �L�т�~�A������E
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fWidth += 5.0f;
		}
		break;
	case EXTENDBALL_RIGHT:	// �L�т�~�A�E���獶
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fWidth += 5.0f;
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
	case DAMEGE_WALL:		// �_���[�W��
		break;
	default:
		assert(false);		// �{���ʂ�Ȃ��ꏊ
		break;
	}

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
		pEnemy->bUse = false;
		break;	// ��������
	case EXTENDBALL_UP:		// �L�т�~�A�ォ�牺
		AddScore(50);
		pEnemy->bUse = false;
		break;
	case EXTENDBALL_DWON:	// �L�т�~�A�������
		pEnemy->bUse = false;
		break;
	case EXTENDBALL_LEFT:	// �L�т�~�A������E
		pEnemy->bUse = false;
		break;
	case EXTENDBALL_RIGHT:	// �L�т�~�A�E���獶
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
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
// �G�̏I������
//====================================
void DeleteEnemy(void)
{
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
			SetDraw(pDevice, s_pTexture[pEnemy->type], nCntEnemy * 4);
		}
	}
}

//====================================
// �G�̐ݒ菈��
//====================================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	Enemy *pEnemy;	// �G�̃|�C���^
	int nCntEnemy;
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		if (pEnemy->bUse == false)
		{// �G���g�p����Ă��Ȃ��ꍇ

			// �ʒu�ݒ�
			pEnemy->pos = pos;

			// ��ސݒ�
			pEnemy->type = type;

			// �U���Ԋu�̐ݒ�
			pEnemy->nAtkInterval = 0;

			switch (pEnemy->type)
			{
			case SPLITBALL_FIRST:	// �ʂ�鋅�̍ŏ�
				// �G�l�~�[�̑ҋ@����
				NeutralEnemy(pEnemy);
				// �����ݒ�
				pEnemy->nLife = 1;
				// �摜�̑傫���ݒ�
				pEnemy->fHeight	= 60.0f;	// ����
				pEnemy->fWidth	= 60.0f;	// ��
				break;
			case SPLITBALL_SECOND:	// �ʂ�鋅��2���
				// �G�l�~�[�̑ҋ@����
				NeutralEnemy(pEnemy);
				// �ʒu�������U�炷
				pEnemy->pos.x = pos.x + (float)(rand() % 50);
				pEnemy->pos.y = pos.y + -(float)(rand() % 50);
				// �����ݒ�
				pEnemy->nLife = 1;
				// �摜�̑傫���ݒ�
				pEnemy->fHeight	= 50.0f;	// ����
				pEnemy->fWidth	= 50.0f;		// ��
				break;
			case SPLITBALL_LAST:	// �ʂ�鋅�̍Ō�
				// �G�l�~�[�̑ҋ@����
				NeutralEnemy(pEnemy);
				// �ʒu�������U�炷
				pEnemy->pos.x = pos.x + (float)(rand() % 20);
				pEnemy->pos.y = pos.y + -(float)(rand() % 20);
				// �����ݒ�
				pEnemy->nLife = 1;
				// �摜�̑傫���ݒ�
				pEnemy->fHeight	= 40.0f;	// ����
				pEnemy->fWidth	= 40.0f;	// ��
				break;
			case EXTENDBALL_UP:		// �L�т�~�A�ォ�牺
			case EXTENDBALL_DWON:	// �L�т�~�A�������
			case EXTENDBALL_LEFT:	// �L�т�~�A������E
			case EXTENDBALL_RIGHT:	// �L�т�~�A�E���獶
				// �����ݒ�
				pEnemy->nLife = 1;
				// �摜�̑傫���ݒ�
				pEnemy->fHeight	= 10.0f;	// ����
				pEnemy->fWidth	= 10.0f;		// ��
				break;
			case GOSTRAIGHT_UP:		// ���i���钷���`�A�ォ�牺
			case GOSTRAIGHT_DWON:	// ���i���钷���`�A�������
				// �����ݒ�
				pEnemy->nLife = 1;
				// �摜�̑傫���ݒ�
				pEnemy->fHeight	= 10.0f;	// ����
				pEnemy->fWidth	= 5.0f;		// ��
				break;
			case GOSTRAIGHT_LEFT:	// ���i���钷���`�A������E
			case GOSTRAIGHT_RIGHT:	// ���i���钷���`�A�E���獶
				// �����ݒ�
				pEnemy->nLife = 1;
				// �摜�̑傫���ݒ�
				pEnemy->fHeight	= 5.0f;		// ����
				pEnemy->fWidth	= 10.0f;	// ��
				break;
			case DAMEGE_WALL:		// �_���[�W��
				break;
			default:
				assert(false);		// �{���ʂ�Ȃ��ꏊ
				break;
			}

			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
			
			// �g�p���Ă����Ԃɂ���
			pEnemy->bUse = true;

			// �G�̐����v���X
			s_nEnemyCnt++;

			break;
		}
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
// �G�̓����菈��
//====================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	Enemy *pEnemy = &(s_aEnemy[nCntEnemy]);		// �G�̃|�C���^

	pEnemy->nLife -= nDamage;

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
// �u���b�N�̓����蔻�菈��
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
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_UP, *pEnemy))
			{
				HitPlayer(1);
			}

			// ��
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_DWON, *pEnemy))
			{// �u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				HitPlayer(1);
			}

			// ��
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_LEFT, *pEnemy))
			{// �u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				HitPlayer(1);
			}

			// �E
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_RIGHT, *pEnemy))
			{// �u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				HitPlayer(1);
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	return bisLanding;
}

//====================================
// ��k�̂̐����̌������菈��
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
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_DWON:
		// �u���b�N�̃x�N�g���̊l��
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_LEFT:
		// �u���b�N�̃x�N�g���̊l��
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_RIGHT:
		// �u���b�N�̃x�N�g���̊l��
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	default:
		assert(false);
		break;
	}

	// ��Ώۂ̃x�N�g���̊l��
	vTarget.start  = *pPos2;
	vTarget.vector = *pPos1 - *pPos2;

	// �x�N�g���̎n�_���m�̋����B
	D3DXVECTOR3 v = vTarget.start - vEnemy.start;

	// �u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
	float Bv_Tv = D3DXVec2Cross(&(vEnemy.vector), &(vTarget.vector));
	if (Bv_Tv == 0.0f)
	{
		// ���s�ł���B
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(vEnemy.vector));
	float v_Tv = D3DXVec2Cross(&(v), &(vTarget.vector));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	float eps = 0.000001f;	// �����蔻����ɂ߂�

	return !((hit1 + eps < 0.0f) || (hit1 - eps > 1.0f) || (hit2 + eps < 0.0f) || (hit2 - eps > 1.0f));
}