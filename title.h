#ifndef _TITLE_H_			// このマクロ定義がされてなかったら
#define _TITLE_H_			// 2重インクルード防止のマクロ定義

#include "main.h"

// プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 向き
	float fWidth;		// 画像の幅
	float fHeigth;		// 画像の高さ
	float fLength;		// 対角線の長さ
	float fAngle;		// 対角線の角度

}TITLE;

// プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif // !_POLYGON_H_

