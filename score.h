#ifndef _SCORE_H_
#define _SCORE_H_

//�}�N����`
#define MAX_SCORE	(7)		//����

//�v���g�^�C�v�錾
void InitScore(void);			// ����������
void UninitScore(void);			// �I������
void UpdateScore(void);			// �X�V����
void DrawScore(void);			// �`�揈��
void SetScore(int nScore);		// �ݒ菈��
void AddScore(int nValue);		// ���Z����
int GetScore(void);				// �擾����
void DrawNumber(int nNumber);	// �����̕`�揈��

#endif // _SCORE_H_