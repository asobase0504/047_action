//=========================================
//
//�v���C���[�̃w�b�_�[�t�@�C��
//Author YudaKaito
//
//=========================================
#ifndef _PLAYER_H_			//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_			//2�d�C���N���[�h�h�~�̃}�N����`

//------------------------------------
//�}�N����`
//------------------------------------
#define PLAYER_TEX		("data/TEXTURE/player00.png")	//�v���C���[�̃e�N�X�`��
#define PLAYER_WIDTH	(35.0f)							//�v���C���[�̕�
#define PLAYER_HEIGTH	(35.0f)							//�v���C���[�̍���
#define PLAYER_MOVE		(0.05f)							//�v���C���[�ړ���
#define PLAYER_JUMPMOVE	(15.0f)							//�v���C���[�̒�����
#define PLAYER_JUMPMAX	(10)							//�v���C���[�̒����
#define WOARD_GRAVITY	(0.3f)							//���E�̏d��

//------------------------------------
//�v���C���[�̏��
//------------------------------------
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//�o�����(�_��)
	PLAYERSTATE_NORMAL,		//�ʏ���
	PLAYERSTATE_DEATH,		//���S���
	PLAYERSTATE_REVIVAL,	//�������
	PLAYERSTATE_MAX
}PLAYRSTATE;

//------------------------------------
//�v���C���[�̏��
//------------------------------------
typedef enum
{
	JUMP_NONE = 0,
	JUMP_NOW,
	JUMP_MAX
}JUMPSTATE;

//------------------------------------
//���S���W�̏��
//------------------------------------
typedef enum
{
	PLAYER_POS_LEFT = 0,
	PLAYER_POS_RIGHT ,
}PLAYER_CENTERPOS;

//------------------------------------
//�v���C���[�\���̂̒�`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 posOld;			//�ʒu
	D3DXVECTOR3 move;			//�ړ���
	D3DXVECTOR3 rot;			//����
	D3DXVECTOR3 rotOld;			//�ʒu
	int nLife;					//�̗�
	float fWidth;				//�摜�̕�
	float fHeigth;				//�摜�̍���
	float fLength;				//�Ίp���̒���
	float fAngle;				//�Ίp���̊p�x
	float fMove;				//�ړ���
	int  nJumpCnt;				//�����
	bool bUse;					//�\�����
	JUMPSTATE jumpstate;		//����̏��
	int nCntAnim;				//�A�j���[�V�����̃J�E���^�[
	int nPatternAnim;			//�A�j���[�V�����̃p�^�[���ԍ�
	int nDirectionMove;			//����
	PLAYRSTATE state;			//�X�e�[�^�X
	PLAYER_CENTERPOS Centerpos;	//���S���W�̈ʒu�̐؂�ւ�
	int RevivalInterval;		//�����^�C�~���O
}Player;

//------------------------------------
//�v���g�^�C�v�錾
//------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void MovePlayer(void);
void HitPlayer(int nDamege);
Player* GetPlayer(void);
void RectPlayer(VERTEX_2D *pVtx, PLAYER_CENTERPOS Centerpos);

#endif // !_POLYGON_H_
