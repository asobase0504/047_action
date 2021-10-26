//=========================================
// 
// �Q�[���I����ʂ̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef _RESULT_H_			// ���̃}�N����`������ĂȂ�������
#define _RESULT_H_			// 2�d�C���N���[�h�h�~�̃}�N����`


//-----------------------------------------
// �Q�[���I����Ԃ̗񋓌^
//-----------------------------------------
typedef enum
{
	GAME_CREAR = 0,
	GAME_OVER,
	GAME_MAX
}GAME_CASE;

//-----------------------------------------
// �v���C���[�\���̂̒�`
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
	float fWidth;		// �摜�̕�
	float fHeigth;		// �摜�̍���
	float fLength;		// �Ίp���̒���
	float fAngle;		// �Ίp���̊p�x

}RESULT;

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void GetResult(GAME_CASE Game_Case);


#endif // !_RESULT_H_