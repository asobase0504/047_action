//=========================================
// 
// �u���b�N�̏���
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "setup.h"
#include "particle.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

// �O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				// �e�N�X�`���o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// ���_�o�b�t�@
static Block s_aBlock[MAX_BLOCK];								// �u���b�N�̏��
static int s_aSumoke[MAX_BLOCK];

//====================================
// ����������
//====================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;
	VERTEX_2D *pVtx;
	Block *pBlock;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&s_pTexture);

	// �u���b�N�̏��̏�����
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		pBlock->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->col = D3DXCOLOR(0.5f, 0.35f, 0.25f, 1.0f);
		pBlock->bUse = false;
		pBlock->fHeight = 50.0f;
		pBlock->fWidth = 50.0f;
		s_aSumoke[nCntBlock] = 0;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, pBlock->pos, pBlock->fWidth, pBlock->fHeight);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(pBlock->col));
		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
// �I������
//====================================
void UninitBlock(void)
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
// �X�V����
//====================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		// �A�h���X�̎擾
		Block *pBlock = &(s_aBlock[i]);

		if (pBlock->bUse)
		{// �u���b�N���g�p����Ă���

			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, pBlock->pos, pBlock->fWidth, pBlock->fHeight);
			// �F�̐ݒ�
			SetRectColor(pVtx, &(pBlock->col));

		}
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
// �`�揈��
//====================================
void DrawBlock(void)
{
	// �f�o�C�X�̎擾
	 LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �`�揈��
	InitDraw(pDevice, s_pVtxBuff);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		// �A�h���X�̎擾
		Block *pBlock = &(s_aBlock[i]);

		if (pBlock->bUse)
		{// �u���b�N���g�p����Ă���
			//�`�揈��
			RectDraw(pDevice, s_pTexture, i * 4);
		}
	}
}

//====================================
// �u���b�N�̐ݒ菈��
//====================================
void SetBlock(D3DXVECTOR3 pos,float fHeight, float fWidth)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		// �A�h���X�̎擾
		Block *pBlock = &(s_aBlock[i]);
		if (!pBlock->bUse)
		{// �u���b�N���g�p����Ă��Ȃ��ꍇ

			pBlock->pos = pos;			// ���W�̐ݒ�
			pBlock->fHeight = fHeight;	// �����̐ݒ�
			pBlock->fWidth = fWidth;	// ���̐ݒ�

			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, pBlock->pos, pBlock->fWidth, pBlock->fHeight);

			pBlock->bUse = true;	// �g�p���Ă����Ԃɂ���
			break;
		}
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

D3DXVECTOR3 g_Outpos;

//====================================
// �u���b�N�ƃv���C���[�̓����蔻�菈��
//====================================
bool CollisionBlock(Player *pPlayer , D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	// �����蔻�菈��
	D3DXVECTOR3 Outpos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//�����蔻��̌�_
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &(s_aBlock[i]);
		if (pBlock->bUse)
		{// �u���b�N�������Ă���

			// �u���b�N��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, *pBlock, &(Outpos)))
			{
				if (pos1.y <= pos2.y)
				{
					pPlayer->pos.y -= pos2.y - Outpos.y;
				}
				else
				{
					pPlayer->pos.y -= pos1.y - Outpos.y;
				}
 				pPlayer->move.y = 0.0f;
				pPlayer->nJumpCnt = 0;
				pPlayer->jumpstate = JUMP_NONE;
				bisLanding = true;
				if (pPlayer->rot.z >= 0.05f || pPlayer->rot.z <= -0.05f)
				{
					SetParticle(pPlayer->pos, PARTICLE_PLAYER_WALK);

				}
			}

			// �u���b�N��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, *pBlock, &(Outpos)))
			{
				if (pos1.y >= pos2.y)
				{
					pPlayer->pos.y += pos1.y - Outpos.y;
				}
				else
				{
					pPlayer->pos.y += pos2.y - Outpos.y;
				}
				pPlayer->move.y = 0.0f;
			}

			// �u���b�N��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, *pBlock, &(Outpos)))
			{
				pPlayer->pos.x = pBlock->pos.x - pBlock->fWidth - pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
			}

			// �u���b�N�E
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, *pBlock, &(Outpos)))
			{
				pPlayer->move.x = 0.0f;
				pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	g_Outpos = Outpos;
	return bisLanding;
}

//====================================
// �u���b�N�ƃG�l�~�[�̓����蔻�菈��
//====================================
bool CollisionBlockEnemy(Enemy *pEnemy, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	D3DXVECTOR3 Outpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����蔻��̌�_
	Block *pBlock = s_aBlock;
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		if (pBlock->bUse)
		{// �u���b�N�������Ă���

			// ��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, s_aBlock[nCntBlock], &(Outpos)))
			{
				if (pos1.y <= pos2.y)
				{
					pEnemy->pos.y -= pos2.y - Outpos.y;
				}
				else
				{
					pEnemy->pos.y -= pos1.y - Outpos.y;
				}
				if (pEnemy->state == ENEMYSTATE_ATTACK)
				{
					pEnemy->nLife--;
				}
			}

			// ��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, s_aBlock[nCntBlock], &(Outpos)))
			{// �u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				if (pos1.y >= pos2.y)
				{
					pEnemy->pos.y += pos1.y - Outpos.y;
				}
				else
				{
					pEnemy->pos.y += pos2.y - Outpos.y;
				}
				if (pEnemy->state == ENEMYSTATE_ATTACK)
				{
					pEnemy->nLife--;
				}
			}

			// ��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, s_aBlock[nCntBlock], &(Outpos)))
			{// �u���b�N�̍��W�ƍ��W���d�Ȃ荇������
 				pEnemy->pos.x = pBlock->pos.x - pBlock->fWidth - pEnemy->fWidth - 1.0f;
				if (pEnemy->state == ENEMYSTATE_ATTACK)
				{
					pEnemy->nLife--;
				}
			}

			// �E
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, s_aBlock[nCntBlock], &(Outpos)))
			{// �u���b�N�̍��W�ƍ��W���d�Ȃ荇������
				pEnemy->pos.x = pBlock->pos.x + pBlock->fWidth + pEnemy->fWidth + 1.0f;
				if (pEnemy->state == ENEMYSTATE_ATTACK)
				{
					pEnemy->nLife--;
				}
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	return bisLanding;
}


//====================================
// �u���b�N�̐����̌������菈��
//====================================
bool CrossingBlock(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2 ,JUDGE_POSITION position,Block block, D3DXVECTOR3 *Outpos)
{
	// �����蔻�菈��
	Block *pBlock = &(block);
	Segment seg1;
	Segment seg2;

	switch (position)
	{
	case POSITION_UP:
		// �u���b�N�̃x�N�g���̊l��
		seg1.s = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		seg1.v = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z) - seg1.s;
		break;
	case POSITION_DWON:
		// �u���b�N�̃x�N�g���̊l��
		seg1.s = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z);
		seg1.v = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - seg1.s;
		break;
	case POSITION_LEFT:
		// �u���b�N�̃x�N�g���̊l��
		seg1.s = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		seg1.v = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - seg1.s;
		break;
	case POSITION_RIGHT:
		// �u���b�N�̃x�N�g���̊l��
		seg1.s = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		seg1.v = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - seg1.s;
		break;
	default:
		break;
	}

	// ��Ώۂ̃x�N�g���̊l��
	seg2.s = *pPos2;
	seg2.v = *pPos1 - *pPos2;

	// �x�N�g���̎n�_���m�̋����B
	D3DXVECTOR3 v = seg2.s - seg1.s;

	// �u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
	float Bv_Tv = D3DXVec2Cross(&(seg1.v), &(seg2.v));
	if (Bv_Tv == 0.0f)
	{
		// ���s�ł���B
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(seg1.v));
	float v_Tv = D3DXVec2Cross(&(v), &(seg2.v));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
	{
		return false;
	}

	if (Outpos != NULL)
	{
		*Outpos = seg1.s + seg1.v * hit1;
	}
	return true;
}

//====================================
// �u���b�N�̎擾����
//====================================
Block* GetBlock(void)
{
	return s_aBlock;
}

//====================================
// ��_�̎擾����<�f�o�b�N�p>
//====================================
D3DXVECTOR3 GetOut(void)
{
	return g_Outpos;
}
