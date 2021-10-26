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
#define EXTENDBALL_TEX	"data/TEXTURE/enemy001.png"
#define MAX_ENEMY	(128)

//------------------------------------
// �G�̏��
//------------------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// �ʏ���
	ENEMYSTATE_DAMAGE,		// �_���[�W����
	ENEMYSTATE_MAX
}ENEMYSTATE;

//------------------------------------
// �G�̏��
//------------------------------------
typedef enum
{
	ENEMYTYPE_SPLITBALL_FIRST,	// �ʂ�鋅�ŏ�
	ENEMYTYPE_SPLITBALL_SECOND,	// �ʂ�鋅����
	ENEMYTYPE_SPLITBALL_LAST,	// �ʂ�鋅
	ENEMYTYPE_EXTENDBALL_UP,	// �L�т�~�A�ォ�牺
	ENEMYTYPE_EXTENDBALL_DWON,	// �L�т�~�A�������
	ENEMYTYPE_EXTENDBALL_LEFT,	// �L�т�~�A������E
	ENEMYTYPE_EXTENDBALL_RIGHT,	// �L�т�~�A�E���獶
	ENEMYTYPE_MAX,
}ENEMYTYPE;

//------------------------------------
// �G�\���̂̒�`
//------------------------------------
typedef struct
{
	ENEMYTYPE type;
	D3DXVECTOR3 pos;	// �ʒu
	float fWidth;		// �摜�̕�
	float fHeight;		// �摜�̍���
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// ����
	D3DXCOLOR col;		// ���_�J���[
	int nLife;			// ����
	bool bUse;			// �g�p����Ă��邩�ǂ���
	int nAtkInterval;	// �U���Ԋu
}Enemy;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitEnemy(void);							// ����������
void UninitEnemy(void);							// �I������
void UpdateEnemy(void);							// �X�V����
void NeutralEnemy(Enemy *pEnemy);				// �ҋ@����
void AttackEnemy(Enemy *pEnemy);				// �U������
void DieEnemy(Enemy *pEnemy);					// ���S����
void DeleteEnemy(void);
void DrawEnemy(void);							// �`�揈��
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type);	// �ݒ菈��
void HitEnemy(int nCntEnemy, int nDamage);		// �����蔻�菈��
int GetCntEnemy(void);							// �G�̐��擾����
Enemy *GetEnemy(void);							// �擾����
bool CollisionEnemy(Player *pPlayer, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
bool CrossingEnemy(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, JUDGE_POSITION position, Enemy enemy);

#endif // !_ENEMY_H_
