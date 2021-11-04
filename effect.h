//=========================================
// 
// �G�t�F�N�g�̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//------------------------------------
// �G�t�F�N�g�̎�ނ̗񋓌^�̒�`
//------------------------------------
typedef enum
{
	EFFECT_SPLITBALL_MOVE = 0,
	EFFECT_MAX,
}EFFCT_TYPE;

//------------------------------------
// �G�t�F�N�g�̍\���̂̒�`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXCOLOR col;		// �F
	float fHeigth;		// ����
	float fWidth;		// ��
	int nLife;			// ����(�\������)
	bool bUse;			// �g�p���Ă��邩�ǂ���
	EFFCT_TYPE type;		//�G�t�F�N�g�^�C�v
}Effect;

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
void InitEffect(void);		// ����������
void UninitEffect(void);	// �I������
void UpdateEffect(void);	// �X�V����
void DrawEffect(void);		// �`�揈��
void SetEffect(D3DXVECTOR3 pos, EFFCT_TYPE type);	// �ݒ菈��

#endif // _EFFECT_H_