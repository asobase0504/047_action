//=========================================
// 
// ゲーム画面の処理
// Author YudaKaito
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
#include "ranking.h"
#include "time.h"
#include "score.h"
#include "summon.h"
#include <assert.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define RANK_INTERVAL	(100)

//------------------------------------
// ゲームステータスの列挙型
//------------------------------------
typedef enum
{
	GAMESTATE_NONE = 0,			// 何もしてないとき
	GAMESTATE_NORMAL,			// ゲームプレイ時
	GAMESTATE_END,				// ゲーム終了時
	GAMESTATE_RANKING_INIT,		// ランキングの初期化
	GAMESTATE_RANKING_NORMAL,	// ランキング表示時
}GAMESTATE;

//------------------------------------
// スタティック変数
//------------------------------------
static int s_nRankInterval;
static GAMESTATE s_GameState;

//====================================
// 初期化処理
//====================================
void InitGame(void)
{
	s_GameState = GAMESTATE_NORMAL;

	// 背景の初期化処理
	InitBG();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// プレイヤーの初期化処理
	InitPlayer();

	// エネミーの初期化処理
	InitEnemy();

	// ブロックの初期化処理
	InitBlock();

	// ランキングの初期化処理
	InitRanking();

	// タイムの終了処理
	InitTime();

	// スコアの初期化
	InitScore();

	// ブロックの設定
	SetBlock({ 500.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,25.0f,0.0f }, 25.0f, 650.0f, 0);
	SetBlock({ 1800.0f,800.0f,0.0f }, 800.0f, 25.0f, 0);
	SetBlock({ 1150.0f,SCREEN_HEIGHT-25.0f,0.0f }, 25.0f, 650.0f, 0);

	//// エネミーの設定
	SetEnemy({ 1100.0f,550.0f,0.0f }, GOSTRAIGHT_RIGHT);
	SetEnemy({ 1100.0f,570.0f,0.0f }, GOSTRAIGHT_RIGHT);
	//SetEnemy({ 1100.0f,70.0f,0.0f }, EXTENDBALL_UP);

	// スコアの設定
	SetScore(1234);

	// ランキングの読込
	ResetRanking();

	s_nRankInterval = 0;
}

//====================================
// 終了処理
//====================================
void UninitGame(void)
{
	// 背景の終了処理
	UninitBG();

	// エフェクトの終了処理
	UnInitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// プレイヤーの終了処理
	UninitPlayer();

	// エネミーの終了処理
	UninitEnemy();

	// ブロックの終了処理
	UninitBlock();

	// ランキングの終了処理
	UninitRanking();

	// タイムの終了処理
	UninitTime();

	// スコアの終了処理
	UninitScore();
}

//====================================
// 更新処理
//====================================
void UpdateGame(void)
{
	SetEnemy({ 1100.0f,550.0f,0.0f }, GOSTRAIGHT_RIGHT);
	SetEnemy({ 1100.0f,570.0f,0.0f }, GOSTRAIGHT_RIGHT);

	switch (s_GameState)
	{
	case GAMESTATE_NONE:
		RetryGame();
		break;
	case GAMESTATE_NORMAL:
	{
		//敵の召喚
//		UpdateSummon();

		//時間の更新処理
		UpdateTime();

		// スコアの更新処理
		UpdateScore();

		// エネミーの更新処理
		UpdateEnemy();

		//プレイヤーが死んだか判定する
		Player *player = GetPlayer();
		if (player->state == PLAYERSTATE_DEATH)
		{
			s_GameState = GAMESTATE_END;	//ゲーム終了時に移行
		}
	}
		break;
	case GAMESTATE_END:
		//ランキング表示までの余韻
		s_nRankInterval++;
		if (s_nRankInterval >= RANK_INTERVAL)
		{
			s_GameState = GAMESTATE_RANKING_INIT;	//ランキング表示時に移行
		}
		break;
	case GAMESTATE_RANKING_INIT:
		SetRanking(GetScore());
		s_GameState = GAMESTATE_RANKING_NORMAL;	//ランキング表示時に移行
		break;
	case GAMESTATE_RANKING_NORMAL:
		UpdateRanking();
		break;
	default:
		assert(false);
		break;
	}

	// 背景の更新処理
	UpdateBG();

	// エフェクトの更新処理
	UpdateEffect();

	// パーティクルの更新処理
	UpdateParticle();

	// プレイヤーの更新処理
	UpdatePlayer();

	// ブロックの更新処理
	UpdateBlock();
}

//====================================
// 描画処理
//====================================
void DrawGame(void)
{
	// 背景の描画処理
	DrawBG();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// プレイヤーの描画処理
	DrawPlayer();

	// エネミーの描画処理
	DrawEnemy();

	// ブロックの描画処理
	DrawBlock();
	
	// 時間の描画処理
	DrawTime();

	// スコアの描画処理
	DrawScore();
	if (s_GameState == GAMESTATE_RANKING_NORMAL)
	{
		DrawRanking();
	}
}


//====================================
// retry処理
//====================================
void RetryGame(void)
{
	// 時間の終了処理
	UninitTime();

	// スコアの終了処理
	UninitScore();

	// エネミーの終了処理
	UninitEnemy();

	// 時間の初期化処理
	InitTime();

	// スコアの初期化処理
	InitScore();

	// エネミーの初期化処理
	InitEnemy();
}
