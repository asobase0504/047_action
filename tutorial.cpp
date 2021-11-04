//=========================================
// 
// チュートリアルの処理
// Author YudaKaito
// 
//=========================================
#include "tutorial.h"
#include "input.h"
#include <assert.h>
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "bg.h"
#include "block.h"

//=========================================
// 初期化処理
//=========================================
void InitTutorial(void)
{
	// 背景の初期化処理
	InitBG();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// プレイヤーの初期化処理
	InitPlayer();

	// ブロックの初期化処理
	InitBlock();

	// エネミーの初期化処理
	InitEnemy();

	// ブロックの設定
	SetBlock(D3DXVECTOR3(500.0f, 800.0f, 0.0f), 800.0f, 25.0f, 0);
	SetBlock(D3DXVECTOR3(1150.0f, 25.0f, 0.0f), 25.0f, 650.0f, 0);
	SetBlock(D3DXVECTOR3(1800.0f, 800.0f, 0.0f), 800.0f, 25.0f, 0);
	SetBlock(D3DXVECTOR3(1150.0f, SCREEN_HEIGHT - 25.0f, 0.0f), 25.0f, 650.0f, 0);
}

//=========================================
// 終了処理
//=========================================
void UninitTutorial(void)
{
	// 背景の終了処理
	UninitBG();

	// エフェクトの終了処理
	UninitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// プレイヤーの終了処理
	UninitPlayer();

	// ブロックの終了処理
	UninitBlock();

	// エネミーの終了処理
	UninitEnemy();
}

//=========================================
// 更新処理
//=========================================
void UpdateTutorial(void)
{
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

	if (GetJoypadTrigger(JOYKEY_A) || GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TITLE);
	}
}

//=========================================
// 描画処理
//=========================================
void DrawTutorial(void)
{
	// 背景の描画処理
	DrawBG();

	// エフェクトの描画処理
	DrawEffect();

	// パーティクルの描画処理
	DrawParticle();

	// プレイヤーの描画処理
	DrawPlayer();

	// ブロックの描画処理
	DrawBlock();
}

//=========================================
// 文字表示処理
//=========================================
void TextTutorial(void)
{

}
