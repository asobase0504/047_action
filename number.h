//=========================================
// 
// 数のヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//プロトタイプ宣言
void InitNumber(void);			// 初期化処理
void UninitNumber(void);		// 終了処理
void DrawNumber(void);			// 描画処理
void SetNumber(VERTEX_2D *pVtx, int nNumber);	// 設定処理

#endif // !_NUMBER_H_