#ifndef _SCORE_H_
#define _SCORE_H_

//マクロ定義
#define MAX_SCORE	(7)		//桁数

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif // _SCORE_H_