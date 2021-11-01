#ifndef _SCORE_H_
#define _SCORE_H_

//マクロ定義
#define MAX_SCORE	(7)		//桁数

//プロトタイプ宣言
void InitScore(void);			// 初期化処理
void UninitScore(void);			// 終了処理
void UpdateScore(void);			// 更新処理
void DrawScore(void);			// 描画処理
void SetScore(int nScore);		// 設定処理
void AddScore(int nValue);		// 加算処理
int GetScore(void);				// 取得処理
void DrawNumber(int nNumber);	// 数字の描画処理

#endif // _SCORE_H_