#ifndef _BLOCK_H_
#define _BLOCK_H_

//------------------------------------
// include
//------------------------------------
#include "player.h"
#include "enemy.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MAX_BLOCK	(32)

//------------------------------------
// ブロック構造体の定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 現在の位置
	D3DXVECTOR3 rot;	// 向き
	D3DXCOLOR col;		// 色
	float fWidth;		// 画像の幅
	float fHeight;		// 画像の高さ
	bool bUse;			// 使用しているかどうか

}Block;

//------------------------------------
// 線分構造体
//------------------------------------
typedef struct
{
	D3DXVECTOR3 start;	// 始点
	D3DXVECTOR3 vector;	// 方向ベクトル
}Segment;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitBlock(void);		// 初期化処理
void UninitBlock(void);		// 終了処理
void UpdateBlock(void);		// 更新処理
void DrawBlock(void);		// 描画処理
void SetBlock(D3DXVECTOR3 pos, float fHeight, float fWidth);			// 設定処理							
bool CollisionBlock(Player *pPlayer, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);		// プレイヤーとブロックの当たり判定
bool CollisionBlockEnemy(Enemy *pEnemy, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	// エネミーとブロックの当たり判定
bool CrossingBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, JUDGE_POSITION position, Block block,D3DXVECTOR3 *Outpos);	// 線分の交差判定
Block* GetBlock(void);		// 情報取得処理
D3DXVECTOR3 GetOut(void);

#endif // !_BLOCK_H_
