//=========================================
// 
// パーティクルのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//-----------------------------------------
// include
//-----------------------------------------

//------------------------------------
// 判定位置の列挙型の定義
//------------------------------------
typedef enum
{
	PARTICLE_PLAYER_JUMP = 0,		// プレイヤーの跳躍
	PARTICLE_PLAYER_WALK,			// プレイヤーの移動
	PARTICLE_PLAYER_DEATH,			// プレイヤーの死亡
	PARTICLE_PLAYER_REVIVAL,		// プレイヤーの復活
	PARTICLE_SPLITBALL_ATTACK,		// 別れる球の攻撃
	PARTICLE_BALL_HOMING00_ATTACK,	// 甘い追従をする円の攻撃 
	PARTICLE_BALL_HOMING01_ATTACK,	// 追従をする円の攻撃 
	PARTICLE_BALL_HOMING00_DIE,		// 甘い追従をする円の死亡時 
	PARTICLE_BALL_HOMING01_DIE,		// 追従をする円の死亡時
	PARTICLE_GOSTRAIGHT_DIE,		// 直進する長方形死亡時
	MAX_PARTICLE
}PARTICLE_TYPE;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, PARTICLE_TYPE type);

#endif // _PARTICLE_H_
