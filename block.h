#ifndef _BLOCK_H_
#define _BLOCK_H_

//------------------------------------
// include
//------------------------------------
#include "player.h"
#include "enemy.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define MAX_BLOCK	(32)

//------------------------------------
// �u���b�N�\���̂̒�`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// ���݂̈ʒu
	D3DXVECTOR3 rot;	// ����
	D3DXCOLOR col;		// �F
	float fWidth;		// �摜�̕�
	float fHeight;		// �摜�̍���
	bool bUse;			// �g�p���Ă��邩�ǂ���

}Block;

//------------------------------------
// �����\����
//------------------------------------
typedef struct
{
	D3DXVECTOR3 start;	// �n�_
	D3DXVECTOR3 vector;	// �����x�N�g��
}Segment;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitBlock(void);		// ����������
void UninitBlock(void);		// �I������
void UpdateBlock(void);		// �X�V����
void DrawBlock(void);		// �`�揈��
void SetBlock(D3DXVECTOR3 pos, float fHeight, float fWidth);			// �ݒ菈��							
bool CollisionBlock(Player *pPlayer, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);		// �v���C���[�ƃu���b�N�̓����蔻��
bool CollisionBlockEnemy(Enemy *pEnemy, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	// �G�l�~�[�ƃu���b�N�̓����蔻��
bool CrossingBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, JUDGE_POSITION position, Block block,D3DXVECTOR3 *Outpos);	// �����̌�������
Block* GetBlock(void);		// ���擾����
D3DXVECTOR3 GetOut(void);

#endif // !_BLOCK_H_
