//=========================================
// 
// 敵のヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _ENEMY_H_
#define	_ENEMY_H_

//------------------------------------
// include
//------------------------------------
#include "player.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define SPLITBALL_TEX	"data/TEXTURE/enemy000.png"
#define TRIANGLE_TEX	"data/TEXTURE/enemy003.png"
#define MAX_ENEMY	(128)

//------------------------------------
// 敵の状態
//------------------------------------
typedef enum
{
	ENEMYSTATE_SUMMON = 0,	// 召喚中
	ENEMYSTATE_NEUTRAL,		// 通常状態
	ENEMYSTATE_ATTACK,		// ダメージ判定
	ENEMYSTATE_MAX
}ENEMYSTATE;

//------------------------------------
// 敵の状態
//------------------------------------
typedef enum
{
	SPLITBALL_FIRST,	// 別れる球最初
	SPLITBALL_SECOND,	// 別れる球二回目
	SPLITBALL_LAST,		// 別れる球
	GOSTRAIGHT_UP,		// 直進する長方形、上から下
	GOSTRAIGHT_DWON,	// 直進する長方形、下から上
	GOSTRAIGHT_LEFT,	// 直進する長方形、左から右
	GOSTRAIGHT_RIGHT,	// 直進する長方形、右から左
	REFLECT_TRIANGLE,	// 反射する三角
	DAMEGE_WALL,		// ダメージ壁
	ENEMYTYPE_MAX
}ENEMYTYPE;

//------------------------------------
// 敵構造体の定義
//------------------------------------
typedef struct
{
	ENEMYTYPE type;			// エネミーの種類
	ENEMYSTATE state;		// エネミーの今の状態
	D3DXVECTOR3 pos;		// 位置
	float fWidth;			// 画像の幅
	float fHeight;			// 画像の高さ
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 向き
	D3DXCOLOR col;			// 頂点カラー
	int nLife;				// 寿命
	bool bUse;				// 使用されているかどうか
	int nNeutralInterval;	// 待機間隔
	int nAtkInterval;		// 攻撃間隔
}Enemy;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitEnemy	(void);								// 初期化処理
void UninitEnemy(void);								// 終了処理
void UpdateEnemy(void);								// 更新処理
void DrawEnemy	(void);								// 描画処理
void SetEnemy	(D3DXVECTOR3 pos, ENEMYTYPE type);	// 設定処理
Enemy *GetEnemy	(void);								// 取得処理
int GetCntEnemy	(void);								// 敵の数取得処理
void HitEnemy	(int nCntEnemy);					// 当たり判定処理

bool CollisionEnemy(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	//当たり判定処理
bool CrossingEnemy(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, JUDGE_POSITION position, Enemy enemy);	//当たり判定の計算処理
#endif // !_ENEMY_H_
