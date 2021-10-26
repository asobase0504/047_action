//=========================================
// 
// ゲーム終了画面のヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _RESULT_H_			// このマクロ定義がされてなかったら
#define _RESULT_H_			// 2重インクルード防止のマクロ定義


//-----------------------------------------
// ゲーム終了状態の列挙型
//-----------------------------------------
typedef enum
{
	GAME_CREAR = 0,
	GAME_OVER,
	GAME_MAX
}GAME_CASE;

//-----------------------------------------
// プレイヤー構造体の定義
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
	float fWidth;		// 画像の幅
	float fHeigth;		// 画像の高さ
	float fLength;		// 対角線の長さ
	float fAngle;		// 対角線の角度

}RESULT;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void GetResult(GAME_CASE Game_Case);


#endif // !_RESULT_H_