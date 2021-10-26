//====================================
// 
// ゲームのヘッダーファイル
// Author YudaKaito
// 
//====================================
#ifndef _GAME_H_			// このマクロ定義がされてなかったら
#define _GAME_H_			// 2重インクルード防止のマクロ定義

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitGame(void);	// 初期化処理
void UninitGame(void);	// 終了処理
void UpdateGame(void);	// 更新処理
void DrawGame(void);	// 描画処理
void RankSwitch(void);	// ランキング表示の切り替え
#endif // !_POLYGON_H_