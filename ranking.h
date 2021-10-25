//=========================================
//
//ランキングのヘッダー
//Author YudaKaito
//
//=========================================
#ifndef _RANKING_H_
#define _RANKING_H_
//------------------------------------
//マクロ定義
//------------------------------------
#define MAX_RANK		(5)	//ランク数
#define MAX_RANKSCORE	(8)	//桁数
#define FILE_NAME	"data/txt/ranking.txt"

//------------------------------------
//プロトタイプ宣言
//------------------------------------
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SaveRanking(void);
void SetRanking(int nScore);

#endif // !_RANKING_H_
