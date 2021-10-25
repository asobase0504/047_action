//=========================================
//
//プレイヤーのヘッダーファイル
//Author YudaKaito
//
//=========================================
#ifndef _PLAYER_H_			//このマクロ定義がされてなかったら
#define _PLAYER_H_			//2重インクルード防止のマクロ定義

//------------------------------------
//マクロ定義
//------------------------------------
#define PLAYER_TEX		("data/TEXTURE/player00.png")	//プレイヤーのテクスチャ
#define PLAYER_WIDTH	(35.0f)							//プレイヤーの幅
#define PLAYER_HEIGTH	(35.0f)							//プレイヤーの高さ
#define PLAYER_MOVE		(0.05f)							//プレイヤー移動量
#define PLAYER_JUMPMOVE	(15.0f)							//プレイヤーの跳躍量
#define PLAYER_JUMPMAX	(10)							//プレイヤーの跳躍回数
#define WOARD_GRAVITY	(0.3f)							//世界の重力

//------------------------------------
//プレイヤーの状態
//------------------------------------
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//出現状態(点滅)
	PLAYERSTATE_NORMAL,		//通常状態
	PLAYERSTATE_DEATH,		//死亡状態
	PLAYERSTATE_REVIVAL,	//復活状態
	PLAYERSTATE_MAX
}PLAYRSTATE;

//------------------------------------
//プレイヤーの状態
//------------------------------------
typedef enum
{
	JUMP_NONE = 0,
	JUMP_NOW,
	JUMP_MAX
}JUMPSTATE;

//------------------------------------
//中心座標の状態
//------------------------------------
typedef enum
{
	PLAYER_POS_LEFT = 0,
	PLAYER_POS_RIGHT ,
}PLAYER_CENTERPOS;

//------------------------------------
//プレイヤー構造体の定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 posOld;			//位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//向き
	D3DXVECTOR3 rotOld;			//位置
	int nLife;					//体力
	float fWidth;				//画像の幅
	float fHeigth;				//画像の高さ
	float fLength;				//対角線の長さ
	float fAngle;				//対角線の角度
	float fMove;				//移動量
	int  nJumpCnt;				//跳躍回数
	bool bUse;					//表示状態
	JUMPSTATE jumpstate;		//跳躍の状態
	int nCntAnim;				//アニメーションのカウンター
	int nPatternAnim;			//アニメーションのパターン番号
	int nDirectionMove;			//向き
	PLAYRSTATE state;			//ステータス
	PLAYER_CENTERPOS Centerpos;	//中心座標の位置の切り替え
	int RevivalInterval;		//復活タイミング
}Player;

//------------------------------------
//プロトタイプ宣言
//------------------------------------
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void MovePlayer(void);
void HitPlayer(int nDamege);
Player* GetPlayer(void);
void RectPlayer(VERTEX_2D *pVtx, PLAYER_CENTERPOS Centerpos);

#endif // !_POLYGON_H_
