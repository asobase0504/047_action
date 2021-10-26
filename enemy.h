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
#define EXTENDBALL_TEX	"data/TEXTURE/enemy001.png"
#define MAX_ENEMY	(128)

//------------------------------------
// 敵の状態
//------------------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// 通常状態
	ENEMYSTATE_DAMAGE,		// ダメージ判定
	ENEMYSTATE_MAX
}ENEMYSTATE;

//------------------------------------
// 敵の状態
//------------------------------------
typedef enum
{
	ENEMYTYPE_SPLITBALL_FIRST,	// 別れる球最初
	ENEMYTYPE_SPLITBALL_SECOND,	// 別れる球二回目
	ENEMYTYPE_SPLITBALL_LAST,	// 別れる球
	ENEMYTYPE_EXTENDBALL_UP,	// 伸びる円、上から下
	ENEMYTYPE_EXTENDBALL_DWON,	// 伸びる円、下から上
	ENEMYTYPE_EXTENDBALL_LEFT,	// 伸びる円、左から右
	ENEMYTYPE_EXTENDBALL_RIGHT,	// 伸びる円、右から左
	ENEMYTYPE_MAX,
}ENEMYTYPE;

//------------------------------------
// 敵構造体の定義
//------------------------------------
typedef struct
{
	ENEMYTYPE type;
	D3DXVECTOR3 pos;	// 位置
	float fWidth;		// 画像の幅
	float fHeight;		// 画像の高さ
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 向き
	D3DXCOLOR col;		// 頂点カラー
	int nLife;			// 寿命
	bool bUse;			// 使用されているかどうか
	int nAtkInterval;	// 攻撃間隔
}Enemy;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitEnemy(void);							// 初期化処理
void UninitEnemy(void);							// 終了処理
void UpdateEnemy(void);							// 更新処理
void NeutralEnemy(Enemy *pEnemy);				// 待機処理
void AttackEnemy(Enemy *pEnemy);				// 攻撃処理
void DieEnemy(Enemy *pEnemy);					// 死亡処理
void DeleteEnemy(void);
void DrawEnemy(void);							// 描画処理
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type);	// 設定処理
void HitEnemy(int nCntEnemy, int nDamage);		// 当たり判定処理
int GetCntEnemy(void);							// 敵の数取得処理
Enemy *GetEnemy(void);							// 取得処理
bool CollisionEnemy(Player *pPlayer, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
bool CrossingEnemy(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, JUDGE_POSITION position, Enemy enemy);

#endif // !_ENEMY_H_
