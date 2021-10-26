//=========================================
//
//�v���C���[�̏���
//Author YudaKaito
//
//=========================================

//-----------------------------------------
//include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "setup.h"
#include <assert.h>
#include "player.h"
#include "particle.h"
#include "effect.h"
#include "block.h"

//-----------------------------------------
//�X�^�e�B�b�N�ϐ�
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;			//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	//���_�o�b�t�@�ւ̃|�C���^
static Player s_player;
static D3DXVECTOR3 OldPos[4];

//=========================================
//�v���C���[�̏���������
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���g
	Player *pPlayer;

	pPlayer = &(s_player);
	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEX,
		&s_pTexture);

	pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT-50.0f, 0.0f);	//�ʒu��������
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ړ��ʂ�������
	pPlayer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//������������
	pPlayer->nLife = 3;												//�̗͂�
	pPlayer->fMove = PLAYER_MOVE;									//�ړ����x
	pPlayer->fWidth = PLAYER_WIDTH;									//�v���C���[�̕�
	pPlayer->fHeigth = PLAYER_HEIGTH;								//�v���C���[�̍���
	pPlayer->bUse = true;											//�v���C���[�̕\���̗L��
	pPlayer->state = PLAYERSTATE_APPEAR;							//�v���C���[�̃X�e�[�^�X
	pPlayer->RevivalInterval = 0;									//�����̃C���^�[�o��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���S���W�����̒������Z�o����B
	pPlayer->fLength = sqrtf(pPlayer->fWidth  * pPlayer->fWidth + pPlayer->fHeigth * pPlayer->fHeigth);

	//���S���W�����̒��_�̊p�x���Z�o����
	pPlayer->fAngle = atan2f(pPlayer->fWidth, pPlayer->fHeigth);


	//���_���W�̐ݒ�
	pVtx[0].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[0].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[1].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z - (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[2].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z - (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[3].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	//���_�J���[�̐ݒ�
	//�e�N�X�`�����W�̐ݒ�
	SetupRectDefault(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
//�v���C���[�̏I������
//=========================================
void UninitPlayer(void)
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

//=========================================
//�v���C���[�̍X�V����
//=========================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//int nAnimationTime;
	Player *pPlayer;
	pPlayer = &(s_player);

	switch (pPlayer->state)
	{
	case PLAYERSTATE_APPEAR:	//�v���C���[���o����
		pPlayer->state = PLAYERSTATE_NORMAL;
		pPlayer->bUse = true;
		pPlayer->move.y = 0.0f;
		pPlayer->rot.z = 0.0f;
		pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	//�ʒu��������
		break;
	case PLAYERSTATE_NORMAL:	//�v���C���[��������
		//�d�͂̉��Z
		pPlayer->move.y += WOARD_GRAVITY;

		//�O��̍��W���X�V
		pPlayer->posOld = pPlayer->pos;

		//�O��̉�]���X�V
		pPlayer->rotOld = pPlayer->rot;

		//�ړ�����
		MovePlayer();

		//�W�����v����
		if (pPlayer->nJumpCnt < PLAYER_JUMPMAX)
		{//�W�����v��
			if (GetKeyboardTrigger(DIK_SPACE))
			{
				pPlayer->jumpstate = JUMP_NOW;
				pPlayer->nJumpCnt++;
				//			pPlayer->pos.y -= 12.0f;
				pPlayer->move.y = 0.0f;
				pPlayer->move.y += -PLAYER_JUMPMOVE;

				for (int i = 0; i < 40; i++)
				{
					SetParticle(pPlayer->pos, PARTICLE_PLAYER_JUMP);
				}

			}
		}

		//��]����
		if (GetKeyboardPress(DIK_Q))
		{
			pPlayer->rot.z += 0.1f;

			//���S���W�̈ڍs
			if (pPlayer->rot.z >= 0.0f && pPlayer->Centerpos == PLAYER_POS_RIGHT)
			{
				pPlayer->pos.x = pPlayer->pos.x - sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->pos.y = pPlayer->pos.y - cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->Centerpos = PLAYER_POS_LEFT;
			}
		}
		if (GetKeyboardPress(DIK_E))
		{
			pPlayer->rot.z -= 0.1f;

			//���S���W�̈ڍs
			if (pPlayer->rot.z <= 0.0f && pPlayer->Centerpos == PLAYER_POS_LEFT)
			{
				pPlayer->pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->Centerpos = PLAYER_POS_RIGHT;
			}
		}

		//�ʒu���X�V
		pPlayer->pos.x += pPlayer->move.x;
		pPlayer->pos.y += pPlayer->move.y;

		//�ړ��ʂ��X�V(����)
		pPlayer->move.x += (0 - pPlayer->move.x) * 0.025f;

		//��]���ď��ɕӂ��ʂ����Ƃ�
		if (-D3DX_PI / 2 >= pPlayer->rot.z)
		{
			pPlayer->rot.z = 0;

			pPlayer->pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.z = 0.0f;

		}
		else if (D3DX_PI / 2 <= pPlayer->rot.z)
		{
			pPlayer->rot.z = 0;

			pPlayer->pos.x = pPlayer->pos.x - sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.y = pPlayer->pos.y - cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.z = 0.0f;

		}

		//�}�b�v�[�ɂ������ꍇ���΂̃}�b�v�[�ɏo��
		if (pPlayer->pos.x - pPlayer->fWidth >= SCREEN_WIDTH)
		{
			pPlayer->pos.x = -pPlayer->fWidth;
		}
		else if (pPlayer->pos.x + pPlayer->fWidth <= 0)
		{
			pPlayer->pos.x = SCREEN_WIDTH + pPlayer->fWidth;
		}

		//���ɒ�������W�����v�����̃��Z�b�g
		if (pPlayer->pos.y > SCREEN_HEIGHT)
		{
			pPlayer->nJumpCnt = 0;
			pPlayer->jumpstate = JUMP_NONE;
			pPlayer->move.y = 0.0f;
			pPlayer->pos.y = SCREEN_HEIGHT;
		}
		break;
	case PLAYERSTATE_DEATH:	//�v���C���[�����񂾏ꍇ
		pPlayer->bUse = false;

		if (GetKeyboardPress(DIK_R))
		{
			pPlayer->state = PLAYERSTATE_REVIVAL;
		}
		break;
	case PLAYERSTATE_REVIVAL:
		if (pPlayer->RevivalInterval <= 0)
		{
			pPlayer->state = PLAYERSTATE_APPEAR;
		}
		break;
	default:
		break;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	RectPlayer(pVtx,pPlayer->Centerpos);

	bool bisLanding;

	//���ꂼ��̒��_���W�̓����蔻��
	bisLanding = CollisionBlock(pPlayer, pVtx[0].pos, pVtx[1].pos);
	bisLanding = CollisionBlock(pPlayer, pVtx[1].pos, pVtx[2].pos);
	bisLanding = CollisionBlock(pPlayer, pVtx[2].pos, pVtx[3].pos);
	bisLanding = CollisionBlock(pPlayer, pVtx[3].pos, pVtx[0].pos);

	//�G�Ƃ̓����蔻��
	CollisionEnemy(pPlayer, pVtx[0].pos, pVtx[1].pos);
	CollisionEnemy(pPlayer, pVtx[1].pos, pVtx[2].pos);
	CollisionEnemy(pPlayer, pVtx[2].pos, pVtx[3].pos);
	CollisionEnemy(pPlayer, pVtx[3].pos, pVtx[0].pos);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
//�v���C���[�̕`�揈��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���g
	Player *pPlayer = GetPlayer();

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (pPlayer->bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexture);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=========================================
//�v���C���[�̈ړ�����
//=========================================
void MovePlayer(void)
{
	Player *pPlayer;
	pPlayer = &(s_player);

	//�L�[�{�[�h�̈ړ�����
	switch (pPlayer->jumpstate)
	{
	case JUMP_NOW:
		if (GetJoypadPress(JOYKEY_LEFT) || GetKeyboardPress(DIK_A))
		{//A�L�[�������ꂽ
			pPlayer->move.x += sinf(D3DX_PI * -0.5f) * (pPlayer->fMove * 0.5f);
		}
		else if (GetJoypadPress(JOYKEY_RIGHT) || GetKeyboardPress(DIK_D))
		{//D�L�[�������ꂽ
			pPlayer->move.x += sinf(D3DX_PI * 0.5f) * (pPlayer->fMove * 0.5f);
		}
		break;
	case JUMP_NONE:
		if (GetJoypadPress(JOYKEY_LEFT) || GetKeyboardPress(DIK_A))
		{//A�L�[�������ꂽ
			pPlayer->move.x += sinf(D3DX_PI * -0.5f) * pPlayer->fMove;
		}
		else if (GetJoypadPress(JOYKEY_RIGHT) || GetKeyboardPress(DIK_D))
		{//D�L�[�������ꂽ
			pPlayer->move.x += sinf(D3DX_PI * 0.5f) * pPlayer->fMove;
		}
		break;
	default:
		assert(false);
		break;
	}
}

//=========================================
//�v���C���[�̃_���[�W����
//=========================================
void HitPlayer(int nDamege)
{
	Player *pPlayer = &(s_player);
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{
		for (int i = 0; i < 32; i++)
		{
			pPlayer->RevivalInterval++;
			SetParticle(pPlayer->pos, PARTICLE_PLAYER_DEATH);
		}
		pPlayer->state = PLAYERSTATE_DEATH;
		pPlayer->nLife--;
	}
}

//=========================================
//�v���C���[�̒��_���W�̐ݒ�
//=========================================
void RectPlayer(VERTEX_2D *pVtx, PLAYER_CENTERPOS Centerpos)
{
	Player *pPlayer = &(s_player);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (Centerpos)
	{
	case PLAYER_POS_RIGHT:
		//���_���W�̐ݒ�
		pVtx[0].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[0].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[1].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z - D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[2].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z - D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pPlayer->pos.x;
		pVtx[3].pos.y = pPlayer->pos.y;
		pVtx[3].pos.z = 0.0f;

		break;
	case PLAYER_POS_LEFT:
		//���_���W�̐ݒ�
		pVtx[0].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[0].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[1].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pPlayer->pos.x;
		pVtx[2].pos.y = pPlayer->pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[3].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[3].pos.z = 0.0f;
		break;
	default:
		assert(false);
		break;
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
//�v���C���[�̎擾����
//=========================================
Player* GetPlayer(void)
{
	return &(s_player);
}
