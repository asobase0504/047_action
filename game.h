//====================================
// 
// �Q�[���̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//====================================
#ifndef _GAME_H_			// ���̃}�N����`������ĂȂ�������
#define _GAME_H_			// 2�d�C���N���[�h�h�~�̃}�N����`

//------------------------------------
// �Q�[���X�e�[�^�X�̗񋓌^
//------------------------------------
typedef enum
{
	GAMESTATE_NONE = 0,			// �������ĂȂ��Ƃ�
	GAMESTATE_NORMAL,			// �Q�[���v���C��
	GAMESTATE_END,				// �Q�[���I����
	GAMESTATE_RANKING_INIT,		// �����L���O�̏�����
	GAMESTATE_RANKING_NORMAL,	// �����L���O�\����
}GAMESTATE;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitGame(void);				// ����������
void UninitGame(void);				// �I������
void UpdateGame(void);				// �X�V����
void DrawGame(void);				// �`�揈��
void RetryGame(void);				// retry����
void SetGameState(GAMESTATE state);	// �Q�[���X�e�[�^�X�̐ݒ菈��
void RankSwitch(void);				// �����L���O�\���̐؂�ւ�

#endif // !_POLYGON_H_