#ifndef _SCORE_H_
#define _SCORE_H_

//�}�N����`
#define MAX_SCORE	(7)		//����

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif // _SCORE_H_