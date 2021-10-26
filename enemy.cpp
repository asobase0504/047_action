//====================================
//
//�G�Ɋւ���֐�
//Author Yuda Kaito
//
//====================================
#include "main.h"
#include "setup.h"
#include "enemy.h"
#include "particle.h"
#include "effect.h"
#include "player.h"
#include "block.h"
#include <assert.h>

//------------------------------------
//�}�N����`
//------------------------------------

//------------------------------------
//�X�^�e�B�b�N�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[ENEMYTYPE_MAX] = {};		//�e�N�X�`���o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//���_�o�b�t�@
static Enemy s_aEnemy[MAX_ENEMY];							//�G�̏��
static int s_nEnemyCnt = 0;									//�G�̐�

//====================================
//�G�̏���������
//====================================
void InitEnemy(void)
{
	Enemy *pEnemy;				//�G�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[ENEMYTYPE_SPLITBALL_FIRST]);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[ENEMYTYPE_SPLITBALL_SECOND]);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[ENEMYTYPE_SPLITBALL_LAST]);

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[ENEMYTYPE_EXTENDBALL_UP]);

	//�\���̂̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		pEnemy->fWidth = 5.0f;								//�摜�̕�
		pEnemy->fHeight = 5.0f;								//�摜�̍���
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		pEnemy->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		pEnemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		pEnemy->nLife = 100;								//����
		pEnemy->bUse = false;								//�g�p����Ă��邩�ǂ���
		pEnemy->nAtkInterval = 0;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		//���_���W�̐ݒ�
		SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fHeight, pEnemy->fWidth);

		//���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(pEnemy->col));
		//�e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		//rhw�̐ݒ�
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
//�G�̏I������
//====================================
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		//�e�N�X�`���̔j��
		if (s_pTexture[nCntEnemy] != NULL)
		{
			s_pTexture[nCntEnemy]->Release();
			s_pTexture[nCntEnemy] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//====================================
//�G�̍X�V����
//====================================
void UpdateEnemy(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	Enemy *pEnemy;			//�G�̃|�C���^
	int nCntEnemy;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		if (pEnemy->bUse == true)
		{
			pEnemy->nAtkInterval++;

			//�ҋ@����
			NeutralEnemy(pEnemy);
			//�U������
			AttackEnemy(pEnemy);

			//�G�����񂾏ꍇ
			if (pEnemy->nLife <= 0)
			{
				//���񂾂Ƃ��̏���
				DieEnemy(pEnemy);
			}

			//���W�̈ړ�����
			pEnemy->pos += pEnemy->move;

			float bisLanding;
			//���ꂼ��̒��_���W�̓����蔻��
			bisLanding = CollisionBlockEnemy(pEnemy, pVtx[0].pos, pVtx[1].pos);
			bisLanding = CollisionBlockEnemy(pEnemy, pVtx[1].pos, pVtx[2].pos);
			bisLanding = CollisionBlockEnemy(pEnemy, pVtx[2].pos, pVtx[3].pos);
			bisLanding = CollisionBlockEnemy(pEnemy, pVtx[3].pos, pVtx[0].pos);

			//���_���W�̐ݒ�
			SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fHeight, pEnemy->fWidth);

			//�J���[�̐ݒ�
			SetRectColor(pVtx, &(pEnemy->col));

		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
//�G�̑ҋ@����
//====================================
void NeutralEnemy(Enemy *pEnemy)
{
	switch (pEnemy->type)
	{
	case ENEMYTYPE_SPLITBALL_FIRST:		//�ʂ�鋅�̍ŏ�
	case ENEMYTYPE_SPLITBALL_SECOND:	//�ʂ�鋅�̂Q���
	case ENEMYTYPE_SPLITBALL_LAST:		//�ʂ�鋅�̍Ō�
		if (pEnemy->nAtkInterval >= 150)
		{
			float fRotDest;
			Player *pPlayer = GetPlayer();	//�v���C���[�̃|�C���^

			//���������v���C���[�̔��Ε�����move
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = -(sinf(fRotDest) * 1.0f);
			pEnemy->move.y = -(cosf(fRotDest) * 1.0f);

			pEnemy->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	case ENEMYTYPE_EXTENDBALL_UP:			//�L�т�~�A�ォ�牺
		break;
	case ENEMYTYPE_EXTENDBALL_DWON:			//�L�т�~�A�������
		break;
	case ENEMYTYPE_EXTENDBALL_LEFT:			//�L�т�~�A������E
		break;
	case ENEMYTYPE_EXTENDBALL_RIGHT:		//�L�т�~�A�E���獶
		break;
	default:
		assert(false);	//�{���ʂ�Ȃ��ꏊ
		break;
	}
}

//====================================
//�G�̍U������
//====================================
void AttackEnemy(Enemy *pEnemy)
{
	Player *pPlayer = GetPlayer();	//�v���C���[�̃|�C���^
	float fRotDest;					//�G�ƃv���C���[�����Ԑ��̊p�x

	switch (pEnemy->type)
	{
	case ENEMYTYPE_SPLITBALL_FIRST:		//�ʂ�鋅�̍ŏ�
	case ENEMYTYPE_SPLITBALL_SECOND:	//�ʂ�鋅�̂Q���
	case ENEMYTYPE_SPLITBALL_LAST:		//�ʂ�鋅�̍Ō�
		if (pEnemy->nAtkInterval <= 150 && pEnemy->nAtkInterval % 5 == 0)
		{
			//�p�[�e�B�N��
			SetParticle(pEnemy->pos, PARTICLE_SPLITBALL_ATTACK);

		}
		if (pEnemy->nAtkInterval >= 300)
		{
			//�ːi�U��
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = sinf(fRotDest) * 10.0f;
			pEnemy->move.y = cosf(fRotDest) * 10.0f;

			pEnemy->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			//�U���T�C�N���̃��Z�b�g
			pEnemy->nAtkInterval = 0;
		}
		break;
	case ENEMYTYPE_EXTENDBALL_UP:			//�L�т�~�A�ォ�牺
		pEnemy->fHeight++;
		break;
	case ENEMYTYPE_EXTENDBALL_DWON:			//�L�т�~�A�������
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fHeight += 5;
		}
		break;
	case ENEMYTYPE_EXTENDBALL_LEFT:			//�L�т�~�A������E
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fWidth += 5;
		}
		break;
	case ENEMYTYPE_EXTENDBALL_RIGHT:		//�L�т�~�A�E���獶
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fWidth += 5;
		}
		break;
	default:
		assert(false);	//�{���ʂ�Ȃ��ꏊ
		break;
	}

}

//====================================
//�G�̎��S����
//====================================
void DieEnemy(Enemy *pEnemy)
{
	switch (pEnemy->type)
	{
	case ENEMYTYPE_SPLITBALL_FIRST:		//�ʂ�鋅�̍ŏ�
		SetEnemy(pEnemy->pos, ENEMYTYPE_SPLITBALL_SECOND);
		SetEnemy(pEnemy->pos, ENEMYTYPE_SPLITBALL_SECOND);
		pEnemy->bUse = false;
		break;
	case ENEMYTYPE_SPLITBALL_SECOND:	//�ʂ�鋅�̂Q���
		SetEnemy(pEnemy->pos, ENEMYTYPE_SPLITBALL_LAST);
		SetEnemy(pEnemy->pos, ENEMYTYPE_SPLITBALL_LAST);
		pEnemy->bUse = false;
		break;
	case ENEMYTYPE_SPLITBALL_LAST:		//�ʂ�鋅�̍Ō�
		break;	//��������
	case ENEMYTYPE_EXTENDBALL_UP:		//�L�т�~�A�ォ�牺
		pEnemy->bUse = false;
		break;
	case ENEMYTYPE_EXTENDBALL_DWON:		//�L�т�~�A�������
		break;
	case ENEMYTYPE_EXTENDBALL_LEFT:		//�L�т�~�A������E
		break;
	case ENEMYTYPE_EXTENDBALL_RIGHT:	//�L�т�~�A�E���獶
		break;
	default:
		assert(false);	//�{���ʂ�Ȃ��ꏊ
		break;
	}
}

//====================================
//�G�̕`�揈��
//====================================
void DeleteEnemy(void)
{
}
//====================================
//�G�̕`�揈��
//====================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Enemy *pEnemy;				//�G�̃|�C���^
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);
		if (pEnemy->bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture[pEnemy->type]);

			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//====================================
//�G�̐ݒ菈��
//====================================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	Enemy *pEnemy;	//�G�̃|�C���^
	Player *pPlayer = GetPlayer();	//�v���C���[�̃|�C���^
	int nCntEnemy;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		if (pEnemy->bUse == false)
		{//�G���g�p����Ă��Ȃ��ꍇ

			//�ʒu�ݒ�
			pEnemy->pos = pos;

			//��ސݒ�
			pEnemy->type = type;

			pEnemy->nAtkInterval = 0;

			float fRotDest;	//�G�ƃv���C���[�����Ԑ��̊p�x
			switch (pEnemy->type)
			{
			case ENEMYTYPE_SPLITBALL_FIRST:
				//���������v���C���[�̔��Ε�����move
				fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
				pEnemy->move.x = -(sinf(fRotDest) * 1.0f);
				pEnemy->move.y = -(cosf(fRotDest) * 1.0f);
				//�����ݒ�
				pEnemy->nLife = 5;
				//�摜�̑傫���ݒ�
				pEnemy->fHeight = 60.0f;	//����
				pEnemy->fWidth = 60.0f;	//��
				break;
			case ENEMYTYPE_SPLITBALL_SECOND:
				//���������v���C���[�̔��Ε�����move
				fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
				pEnemy->move.x = -(sinf(fRotDest) * 1.0f);
				pEnemy->move.y = -(cosf(fRotDest) * 1.0f);
				//�ʒu�������U�炷
				pEnemy->pos.x = pos.x + (float)(rand() % 50);
				pEnemy->pos.y = pos.y + -(float)(rand() % 50);
				//�����ݒ�
				pEnemy->nLife = 5;
				//�摜�̑傫���ݒ�
				pEnemy->fHeight = 50.0f;	//����
				pEnemy->fWidth = 50.0f;		//��
				break;
			case ENEMYTYPE_SPLITBALL_LAST:
				//���������v���C���[�̔��Ε�����move
				fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
				pEnemy->move.x = -(sinf(fRotDest) * 1.0f);
				pEnemy->move.y = -(cosf(fRotDest) * 1.0f);
				//�ʒu�������U�炷
				pEnemy->pos.x = pos.x + (float)(rand() % 20);
				pEnemy->pos.y = pos.y + -(float)(rand() % 20);
				//�����ݒ�
				pEnemy->nLife = 5;
				//�摜�̑傫���ݒ�
				pEnemy->fHeight = 40.0f;	//����
				pEnemy->fWidth = 40.0f;		//��
				break;
			case ENEMYTYPE_EXTENDBALL_UP:			//�L�т�~�A�ォ�牺
			case ENEMYTYPE_EXTENDBALL_DWON:			//�L�т�~�A�������
			case ENEMYTYPE_EXTENDBALL_LEFT:			//�L�т�~�A������E
			case ENEMYTYPE_EXTENDBALL_RIGHT:		//�L�т�~�A�E���獶
				//�����ݒ�
				pEnemy->nLife = 10;
				//�摜�̑傫���ݒ�
				pEnemy->fHeight = 10.0f;	//����
				pEnemy->fWidth = 10.0f;		//��
				break;
			default:
				assert(false);	//�{���ʂ�Ȃ��ꏊ
				break;
			}

			//���_���W�̐ݒ�
			SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fHeight, pEnemy->fWidth);
			
			//���_�J���[�̐ݒ�
			pEnemy->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�����ݒ�

			pEnemy->bUse = true;	//�g�p���Ă����Ԃɂ���

			//�G�̐����v���X
			s_nEnemyCnt++;

			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//====================================
//�G�̓����菈��
//====================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	Enemy *pEnemy = &(s_aEnemy[nCntEnemy]);		//�G�̃|�C���^

	pEnemy->nLife -= nDamage;

	if (pEnemy->nLife <= 0)
	{//HP��0�ɂȂ�����
	}

	//�G�̐��𐔂��Ȃ���
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
//�G�̎擾����
//====================================
Enemy *GetEnemy(void)
{
	//�Ԃ�l
	return s_aEnemy;
}

//====================================
//�G�̐��擾����
//====================================
int GetCntEnemy(void)
{
	return s_nEnemyCnt;
}

//====================================
//�u���b�N�̓����蔻�菈��
//====================================
bool CollisionEnemy(Player *pPlayer, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	//�����蔻�菈��
	Enemy *pEnemy;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);
		if (pEnemy->bUse == true)
		{//�u���b�N�������Ă���

			 //��
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_UP, *pEnemy))
			{
				HitPlayer(1);
			}

			//��
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_DWON, *pEnemy))
			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				HitPlayer(1);
			}

			//��
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_LEFT, *pEnemy))
			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				HitPlayer(1);
			}

			//�E
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_RIGHT, *pEnemy))
			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				HitPlayer(1);
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	return bisLanding;
}

//====================================
//��k�̂̐����̌������菈��
//====================================
bool CrossingEnemy(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, JUDGE_POSITION position, Enemy enemy)
{
	//�����蔻�菈��
	Enemy *pEnemy = &(enemy);
	Segment vEnemy;
	Segment vTarget;

	switch (position)
	{
	case POSITION_UP:
		//�u���b�N�̃x�N�g���̊l��
		vEnemy.start = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_DWON:
		//�u���b�N�̃x�N�g���̊l��
		vEnemy.start = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_LEFT:
		//�u���b�N�̃x�N�g���̊l��
		vEnemy.start = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_RIGHT:
		//�u���b�N�̃x�N�g���̊l��
		vEnemy.start = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	default:
		break;
	}

	//��Ώۂ̃x�N�g���̊l��
	vTarget.start = *pPos2;
	vTarget.vector = *pPos1 - *pPos2;

	//�x�N�g���̎n�_���m�̋����B
	D3DXVECTOR3 v = vTarget.start - vEnemy.start;

	//�u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
	float Bv_Tv = D3DXVec2Cross(&(vEnemy.vector), &(vTarget.vector));
	if (Bv_Tv == 0.0f)
	{
		//���s�ł���B
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(vEnemy.vector));
	float v_Tv = D3DXVec2Cross(&(v), &(vTarget.vector));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	float eps = 0.000001f;	//�����蔻����ɂ߂�

	return !((hit1 + eps < 0.0f) || (hit1 - eps > 1.0f) || (hit2 + eps < 0.0f) || (hit2 - eps > 1.0f));
}