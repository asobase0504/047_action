//====================================
// 
// ゲームのヘッダーファイル
// Author YudaKaito
// 
//====================================
#ifndef _GAME_H_			// このマクロ定義がされてなかったら
#define _GAME_H_			// 2重インクルード防止のマクロ定義

//------------------------------------
// ゲームステータスの列挙型
//------------------------------------
typedef enum
{
	GAMESTATE_NONE = 0,			// 何もしてないとき
	GAMESTATE_NORMAL,			// ゲームプレイ時
	GAMESTATE_END,				// ゲーム終了時
	GAMESTATE_RANKING_INIT,		// ランキングの初期化
	GAMESTATE_RANKING_NORMAL,	// ランキング表示時
}GAMESTATE;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitGame(void);				// 初期化処理
void UninitGame(void);				// 終了処理
void UpdateGame(void);				// 更新処理
void DrawGame(void);				// 描画処理
void RetryGame(void);				// retry処理
void SetGameState(GAMESTATE state);	// ゲームステータスの設定処理
void RankSwitch(void);				// ランキング表示の切り替え

#endif // !_POLYGON_H_