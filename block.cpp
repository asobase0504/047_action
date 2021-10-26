//=========================================
//
//�u���b�N�̏���
//Author YudaKaito
//
//=========================================
#include "main.h"
#include "setup.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				//�e�N�X�`���o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//���_�o�b�t�@
static Block s_aBlock[MAX_BLOCK];								//�u���b�N�̏��
static int s_aSumoke[MAX_BLOCK];

//====================================
//����������
//====================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;
	VERTEX_2D *pVtx;
	Block *pBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&s_pTexture);

	//�u���b�N�̏��̏�����
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		pBlock->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pBlock->bUse = false;
		pBlock->fHeight = 50.0f;
		pBlock->fWidth = 50.0f;
		s_aSumoke[nCntBlock] = 0;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);

		//���_���W�̐ݒ�
		SetRectPos(pVtx, pBlock->pos, pBlock->fHeight, pBlock->fWidth);

		//rhw�̐ݒ�
		//���_�J���[�̐ݒ�
		//�e�N�X�`�����W�̐ݒ�
		SetupRectDefault(pVtx, &(pBlock->col));

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
//�I������
//====================================
void UninitBlock(void)
{
	//�e�N�X�`���̔j��
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

int nCnt = 0;

//====================================
//�X�V����
//====================================
void UpdateBlock(void)
{
	int nCntBlock;			//�u���b�N�̍ő吔��
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	Block *pBlock;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//�A�h���X�̎擾
		pBlock = &(s_aBlock[nCntBlock]);

		if (pBlock->bUse == true)
		{//�u���b�N���g�p����Ă���

			 //���_���W�̐ݒ�
			SetRectPos(pVtx, pBlock->pos, pBlock->fHeight, pBlock->fWidth);

			SetRectColor(pVtx, &(pBlock->col));


			//��ʒ[�Ɍ������ꍇ
			if (pBlock->pos.x <= 0)
			{
				pBlock->bUse = false; //�g�p���Ă��Ȃ���Ԃɂ���
			}
			if (pBlock->pos.y <= 20)
			{
				pBlock->bUse = false; //�g�p���Ă��Ȃ���Ԃɂ���
			}
			if (pBlock->pos.x >= SCREEN_WIDTH)
			{
				pBlock->bUse = false; //�g�p���Ă��Ȃ���Ԃɂ���
			}
			if (pBlock->pos.y >= SCREEN_HEIGHT)
			{
				pBlock->bUse = false; //�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
//�`�揈��
//====================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Block *pBlock;
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//�A�h���X�̎擾
		pBlock = &(s_aBlock[nCntBlock]);

		if (pBlock->bUse == true)
		{//�u���b�N���g�p����Ă���

		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture);

			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);

		}
	}

}

//====================================
//�u���b�N�̐ݒ菈��
//====================================
void SetBlock(D3DXVECTOR3 pos,float fHeight, float fWidth, int type)
{
	int nCntBlock;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	Block *pBlock;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//�A�h���X�̎擾
		pBlock = &(s_aBlock[nCntBlock]);
		if (!pBlock->bUse)
		{//�u���b�N���g�p����Ă��Ȃ��ꍇ

			pBlock->pos = pos;
			pBlock->fHeight = fHeight;
			pBlock->fWidth = fWidth;

			//���_���W�̐ݒ�
			SetRectPos(pVtx, pBlock->pos, pBlock->fHeight, pBlock->fWidth);

			pBlock->bUse = true;	//�g�p���Ă����Ԃɂ���
			break;

		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

}

//====================================
//�u���b�N�ƃv���C���[�̓����蔻�菈��
//====================================
bool CollisionBlock(Player *pPlayer , D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	//�����蔻�菈��
	Block *pBlock = s_aBlock;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		if (pBlock->bUse == true)
		{//�u���b�N�������Ă���

			//��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, s_aBlock[nCntBlock]))
			{
				pPlayer->move.y = 0.0f;
 				pPlayer->pos.y = pBlock->pos.y - pBlock->fHeight;
				pPlayer->nJumpCnt = 0;
				pPlayer->jumpstate = JUMP_NONE;
				bisLanding = true;

				pBlock->col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			}

			//��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, s_aBlock[nCntBlock]))
			{
				pPlayer->move.y = 0.0f;
				pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + pPlayer->fHeigth;
				pBlock->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}

			//��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, s_aBlock[nCntBlock]))
			{
				pPlayer->move.x = 0.0f;
				pPlayer->pos.x = pBlock->pos.x - pBlock->fWidth - pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
//				pPlayer->pos.x = pPlayer->pos.x - 5.0f;
				pBlock->col = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			}

			//�E
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, s_aBlock[nCntBlock]))
			{
				pPlayer->move.x = 0.0f;
				pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
//				pPlayer->pos.x = pPlayer->pos.x + 5.0f;
				pBlock->col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	return bisLanding;
}

//====================================
//�u���b�N�ƃG�l�~�[�̓����蔻�菈��
//====================================
bool CollisionBlockEnemy(Enemy *pEnemy, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	//�����蔻�菈��
	Block *pBlock = s_aBlock;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		if (pBlock->bUse)
		{//�u���b�N�������Ă���

		 //��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, s_aBlock[nCntBlock]))
			{
//				pEnemy->move *= -1.0f;
				pEnemy->pos.y = pBlock->pos.y - pBlock->fHeight - pEnemy->fHeight - 1.0f;

				switch (pEnemy->type)
				{
				case ENEMYTYPE_SPLITBALL_FIRST:
				case ENEMYTYPE_SPLITBALL_SECOND:
				case ENEMYTYPE_SPLITBALL_LAST:
					if (pEnemy->nAtkInterval < 150)
					{
						pEnemy->nLife--;
						pEnemy->nAtkInterval = 150;
					}
					break;
				case ENEMYTYPE_EXTENDBALL_UP:
					pEnemy->nLife--;
					break;
				case ENEMYTYPE_EXTENDBALL_DWON:
				case ENEMYTYPE_EXTENDBALL_LEFT:
				case ENEMYTYPE_EXTENDBALL_RIGHT:
				default:
					break;
				}
				bisLanding = true;

				pBlock->col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			}

			//��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, s_aBlock[nCntBlock]))
			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������
//				pEnemy->move *= -1.0f;
				pEnemy->pos.y = pBlock->pos.y + pBlock->fHeight + pEnemy->fHeight + 5.0f;
				switch (pEnemy->type)
				{
				case ENEMYTYPE_SPLITBALL_FIRST:
				case ENEMYTYPE_SPLITBALL_SECOND:
				case ENEMYTYPE_SPLITBALL_LAST:
					if (pEnemy->nAtkInterval < 150)
					{
						pEnemy->nLife--;
						pEnemy->nAtkInterval = 150;
					}
					break;
				case ENEMYTYPE_EXTENDBALL_UP:
					break;
				case ENEMYTYPE_EXTENDBALL_DWON:
					pEnemy->nLife--;
					break;
				case ENEMYTYPE_EXTENDBALL_LEFT:
				case ENEMYTYPE_EXTENDBALL_RIGHT:
				default:
					break;
				}
				pBlock->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}

			//��
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, s_aBlock[nCntBlock]))
			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������
//				pEnemy->move *= -1.0f;
//				pEnemy->pos.x = pBlock->pos.x - pBlock->fWidth - pEnemy->fWidth - 1.0f;
				pEnemy->pos.x = pEnemy->pos.x - 5.0f;
				if (pEnemy->nAtkInterval < 150)
				{
					pEnemy->nLife--;
					pEnemy->nAtkInterval = 150;
				}
				pBlock->col = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			}

			//�E
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, s_aBlock[nCntBlock]))
			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������
//				pEnemy->move *= -1.0f;
//				pEnemy->pos.x = pBlock->pos.x + pBlock->fWidth + pEnemy->fWidth + 1.0f;
				pEnemy->pos.x = pEnemy->pos.x + 5.0f;
				if (pEnemy->nAtkInterval < 150)
				{
					pEnemy->nLife--;
					pEnemy->nAtkInterval = 150;
				} 
				pBlock->col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();

	return bisLanding;
	return false;
}


//====================================
//�u���b�N�̐����̌������菈��
//====================================
bool CrossingBlock(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2 ,JUDGE_POSITION position,Block block)
{
	//�����蔻�菈��
	Block *pBlock = &(block);
	Segment vBlock;
	Segment vTarget;

	switch (position)
	{
	case POSITION_UP:
		//�u���b�N�̃x�N�g���̊l��
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_DWON:
		//�u���b�N�̃x�N�g���̊l��
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_LEFT:
		//�u���b�N�̃x�N�g���̊l��
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_RIGHT:
		//�u���b�N�̃x�N�g���̊l��
		vBlock.start = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	default:
		break;
	}

	//��Ώۂ̃x�N�g���̊l��
	vTarget.start = *pPos2;
	vTarget.vector = *pPos1 - *pPos2;

	//�x�N�g���̎n�_���m�̋����B
	D3DXVECTOR3 v = vTarget.start - vBlock.start;

	//�u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
	float Bv_Tv = D3DXVec2Cross(&(vBlock.vector), &(vTarget.vector));
	if (Bv_Tv == 0.0f)
	{
		//���s�ł���B
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(vBlock.vector));
	float v_Tv = D3DXVec2Cross(&(v), &(vTarget.vector));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	return !((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f));
}

//====================================
//�u���b�N�̎擾����
//====================================
Block* GetBlock(void)
{
	return s_aBlock;
}
