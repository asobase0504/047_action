//=========================================
// 
// エフェクトのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//------------------------------------
// エフェクトの種類の列挙型の定義
//------------------------------------
typedef enum
{
	EFFECT_SPLITBALL_MOVE = 0,
	EFFECT_MAX,
}EFFCT_TYPE;

//------------------------------------
// エフェクトの構造体の定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	float fHeigth;		// 高さ
	float fWidth;		// 幅
	int nLife;			// 寿命(表示時間)
	bool bUse;			// 使用しているかどうか
	EFFCT_TYPE type;		//エフェクトタイプ
}Effect;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
void InitEffect(void);		// 初期化処理
void UninitEffect(void);	// 終了処理
void UpdateEffect(void);	// 更新処理
void DrawEffect(void);		// 描画処理
void SetEffect(D3DXVECTOR3 pos, EFFCT_TYPE type);	// 設定処理

#endif // _EFFECT_H_