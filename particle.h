//=========================================
// 
// �p�[�e�B�N���̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//-----------------------------------------
// include
//-----------------------------------------

//------------------------------------
// ����ʒu�̗񋓌^�̒�`
//------------------------------------
typedef enum
{
	PARTICLE_PLAYER_JUMP = 0,	// �v���C���[�̒���
	PARTICLE_PLAYER_DEATH,		// �v���C���[�̎��S
	PARTICLE_PLAYER_REVIVAL,	// �v���C���[�̕���
	PARTICLE_SPLITBALL_ATTACK,	// �ʂ�鋅�̍U��
	PARTICLE_BALL_HOMING_ATTACK,	// �Â��Ǐ]������~�̍U�� 
	MAX_PARTICLE
}PARTICLE_TYPE;

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, PARTICLE_TYPE type);

#endif // _PARTICLE_H_
