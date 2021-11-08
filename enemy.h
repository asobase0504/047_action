//=========================================
// 
// �G�̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

//------------------------------------
// include
//------------------------------------
#include "player.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define SPLITBALL_TEX	"data/TEXTURE/enemy000.png"
#define TRIANGLE_TEX	"data/TEXTURE/enemy003.png"
#define MAX_ENEMY	(128)

//------------------------------------
// �G�̏��
//------------------------------------
typedef enum
{
	ENEMYSTATE_SUMMON = 0,	// ������
	ENEMYSTATE_NEUTRAL,		// �ʏ���
	ENEMYSTATE_ATTACK,		// �_���[�W����
	ENEMYSTATE_MAX
}ENEMYSTATE;

//------------------------------------
// �G�̏��
//------------------------------------
typedef enum
{
	SPLITBALL_FIRST,	// �ʂ�鋅�ŏ�
	SPLITBALL_SECOND,	// �ʂ�鋅����
	SPLITBALL_LAST,		// �ʂ�鋅
	GOSTRAIGHT_UP,		// ���i���钷���`�A�ォ�牺
	GOSTRAIGHT_DWON,	// ���i���钷���`�A�������
	GOSTRAIGHT_LEFT,	// ���i���钷���`�A������E
	GOSTRAIGHT_RIGHT,	// ���i���钷���`�A�E���獶
	REFLECT_TRIANGLE,	// ���˂���O�p
	DAMEGE_WALL,		// �_���[�W��
	ENEMYTYPE_MAX
}ENEMYTYPE;

//------------------------------------
// �G�\���̂̒�`
//------------------------------------
typedef struct
{
	ENEMYTYPE type;			// �G�l�~�[�̎��
	ENEMYSTATE state;		// �G�l�~�[�̍��̏��
	D3DXVECTOR3 pos;		// �ʒu
	float fWidth;			// �摜�̕�
	float fHeight;			// �摜�̍���
	D3DXVECTOR3 move;		// �ړ���
	D3DXVECTOR3 rot;		// ����
	D3DXCOLOR col;			// ���_�J���[
	int nLife;				// ����
	bool bUse;				// �g�p����Ă��邩�ǂ���
	int nNeutralInterval;	// �ҋ@�Ԋu
	int nAtkInterval;		// �U���Ԋu
}Enemy;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitEnemy	(void);								// ����������
void UninitEnemy(void);								// �I������
void UpdateEnemy(void);								// �X�V����
void DrawEnemy	(void);								// �`�揈��
void SetEnemy	(D3DXVECTOR3 pos, ENEMYTYPE type);	// �ݒ菈��
Enemy *GetEnemy	(void);								// �擾����
int GetCntEnemy	(void);								// �G�̐��擾����
void HitEnemy	(int nCntEnemy);					// �����蔻�菈��

bool CollisionEnemy(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	//�����蔻�菈��
bool CrossingEnemy(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, JUDGE_POSITION position, Enemy enemy);	//�����蔻��̌v�Z����
#endif // !_ENEMY_H_
