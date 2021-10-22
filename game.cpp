//=========================================
//
//ゲーム画面の処理
//Author YudaKaito
//
//=========================================
#include "main.h"
#include "game.h"
#include "input.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "enemy.h"
#include "bg.h"
#include "map.h"
#include "block.h"

//グローバル変数
int g_nCntEnemy;

//====================================
//初期化処理
//====================================
void InitGame(void)
{
	//背景の初期化処理
	InitBG();

	//エフェクトの初期化処理
	InitEffect();

	//パーティクルの初期化処理
	InitParticle();

	//プレイヤーの初期化処理
	InitPlayer();

	//エネミーの初期化処理
	InitEnemy();

	//ブロックの初期化処理
	InitBlock();

	SetBlock({ 500.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,25.0f,0.0f }, 25.0f, 650.0f, 0);
	SetBlock({ 1800.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,SCREEN_HEIGHT-25.0f,0.0f }, 25.0f, 650.0f, 0);

	SetEnemy({ 1100.0f,550.0f,0.0f }, ENEMYTYPE_SPLITBALL_FIRST);
	SetEnemy({ 1100.0f,70.0f,0.0f }, ENEMYTYPE_EXTENDBALL_UP);
}

//====================================
//終了処理
//====================================
void UninitGame(void)
{

	//背景の終了処理
	UninitBG();

	//エフェクトの終了処理
	UnInitEffect();

	//パーティクルの終了処理
	UninitParticle();

	//プレイヤーの終了処理
	UninitPlayer();

	//エネミーの終了処理
	UninitEnemy();

	//ブロックの終了処理
	UninitBlock();
}

//====================================
//更新処理
//====================================
void UpdateGame(void)
{
	//背景の更新処理
	UpdateBG();

	//エフェクトの更新処理
	UpdateEffect();

	//パーティクルの更新処理
	UpdateParticle();

	//プレイヤーの更新処理
	UpdatePlayer();

	//エネミーの更新処理
	UpdateEnemy();

	//ブロックの更新処理
	UpdateBlock();
}

//====================================
//描画処理
//====================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBG();

	//エフェクトの描画処理
	DrawEffect();

	//パーティクルの描画処理
	DrawParticle();

	//プレイヤーの描画処理
	DrawPlayer();

	//エネミーの描画処理
	DrawEnemy();

	//ブロックの描画処理
	DrawBlock();
}
