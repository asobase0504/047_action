//=========================================
// 
// �w�i�̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef _AG_H_
#define _AG_H_

//-----------------------------------------
// include
//-----------------------------------------
#define NUM_AG	(2)	// �w�i�̐�

//-----------------------------------------
// �O���f�[�V�����̗񋓌^
//-----------------------------------------
typedef enum
{
	AGRADATION_TOP = 0,
	AGRADATION_BOT,
	AGRADATION_MAX,
}GRADATIONAG;

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
void InitAG(void);
void UninitAG(void);
void UpdateAG(void);
void DrawAG(void);

#endif // !_AG_H_
