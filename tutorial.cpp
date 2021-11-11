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
#include "score.h"
#include "sound.h"
#include "setup.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MOVEWORD			"data/TEXTURE/word/move.png"
#define JUMPWORD			"data/TEXTURE/word/jump.png"
#define SELECTWORD			"data/TEXTURE/word/select.png"
#define BACKWORD			"data/TEXTURE/word/Back.png"
#define MOVEMETHOD_KEY		"data/TEXTURE/word/AorD.png"
#define JUMPMETHOD_KEY		"data/TEXTURE/word/spase.png"
#define SELECTMETHOD_KEY	"data/TEXTURE/word/enter.png"
#define MOVEMETHOD_PAD		"data/TEXTURE/word/stick.png"
#define JUMPMETHOD_PAD		"data/TEXTURE/word/AorB.png"
#define SELECTMETHOD_PAD	"data/TEXTURE/word/AorB.png"
#define BACK_PAD			"data/TEXTURE/word/XorY.png"

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureMoveWord = NULL;		// テクスチャへのポインタ	(移動文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffMoveWord = NULL;		// 頂点バッファへのポインタ	(移動文字)
static LPDIRECT3DTEXTURE9		s_pTextureMoveMethod[2] = {};	// テクスチャへのポインタ	(移動方法文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffMoveMethod = NULL;	// 頂点バッファへのポインタ	(移動方法文字)
static LPDIRECT3DTEXTURE9		s_pTextureJumpWord = NULL;		// テクスチャへのポインタ	(跳躍文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffJumpWord = NULL;		// 頂点バッファへのポインタ	(跳躍文字)
static LPDIRECT3DTEXTURE9		s_pTextureJumpMethod[2] = {};	// テクスチャへのポインタ	(跳躍方法文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffJumpMethod = NULL;	// 頂点バッファへのポインタ	(跳躍方法文字)
static LPDIRECT3DTEXTURE9		s_pTextureSelectWord = NULL;	// テクスチャへのポインタ	(決定文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelectWord = NULL;	// 頂点バッファへのポインタ	(決定文字)
static LPDIRECT3DTEXTURE9		s_pTextureSelectMethod[2] = {};	// テクスチャへのポインタ	(決定方法文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelectMethod = NULL;	// 頂点バッファへのポインタ	(決定方法文字)
static LPDIRECT3DTEXTURE9		s_pTextureBackWord = NULL;		// テクスチャへのポインタ	(決定文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBackWord = NULL;		// 頂点バッファへのポインタ	(決定文字)
static LPDIRECT3DTEXTURE9		s_pTextureBackMethod[2] = {};	// テクスチャへのポインタ	(決定方法文字)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBackMethod = NULL;	// 頂点バッファへのポインタ	(決定方法文字)

//=========================================
// 初期化処理
//=========================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		MOVEWORD,
		&s_pTextureMoveWord);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		MOVEMETHOD_KEY,
		&s_pTextureMoveMethod[0]);
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		MOVEMETHOD_PAD,
		&s_pTextureMoveMethod[1]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		JUMPWORD,
		&s_pTextureJumpWord);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		JUMPMETHOD_KEY,
		&s_pTextureJumpMethod[0]);
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		JUMPMETHOD_PAD,
		&s_pTextureJumpMethod[1]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTWORD,
		&s_pTextureSelectWord);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTMETHOD_KEY,
		&s_pTextureSelectMethod[0]);
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTMETHOD_PAD,
		&s_pTextureSelectMethod[1]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		BACKWORD,
		&s_pTextureBackWord);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTMETHOD_KEY,
		&s_pTextureBackMethod[0]);
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		BACK_PAD,
		&s_pTextureBackMethod[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMoveWord,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMoveMethod,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffJumpWord,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffJumpMethod,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSelectWord,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSelectMethod,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBackWord,
		NULL);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBackMethod,
		NULL);

	VERTEX_2D *pVtx;

	// 移動文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffMoveWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(656.0f, 200.0f, 0.0f), 256, 64);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffMoveWord->Unlock();

	// 移動方法文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffMoveMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(1256.0f, 200.0f, 0.0f), 256, 64);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffMoveMethod->Unlock();

	// 跳躍文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffJumpWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(656.0f, 400.0f, 0.0f), 256, 64);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffJumpWord->Unlock();

	// 跳躍方法文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffJumpMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(1256.0f, 400.0f, 0.0f), 256, 64);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffJumpMethod->Unlock();


	// 選択文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffSelectWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(656.0f, 600.0f, 0.0f), 256, 64);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffSelectWord->Unlock();

	// 選択方法文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffSelectMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(1256.0f, 600.0f, 0.0f), 256, 64);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffSelectMethod->Unlock();

	// 戻る文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBackWord->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(1278.0f, 950.0f, 0.0f), 128, 32);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffBackWord->Unlock();

	// 戻る方法文字
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBackMethod->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(1478.0f, 950.0f, 0.0f), 128, 32);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffBackMethod->Unlock();

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
	SetBlock(D3DXVECTOR3(300.0f,800.0f,0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(950.0f,25.0f,0.0f), 25.0f, 650.0f);
	SetBlock(D3DXVECTOR3(1600.0f,800.0f,0.0f), 800.0f, 25.0f);
	SetBlock(D3DXVECTOR3(950.0f, SCREEN_HEIGHT - 25.0f, 0.0f), 25.0f, 650.0f);

	// スコアの初期化処理
	InitScore();

	// 音楽の再生
	PlaySound(SOUND_LABEL_BGM001);
}

//=========================================
// 終了処理
//=========================================
void UninitTutorial(void)
{
	// テクスチャの破棄
	if (s_pTextureMoveWord != NULL)
	{
		s_pTextureMoveWord->Release();
		s_pTextureMoveWord = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffMoveWord != NULL)
	{
		s_pVtxBuffMoveWord->Release();
		s_pVtxBuffMoveWord = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		// テクスチャの破棄
		if (s_pTextureMoveMethod[i] != NULL)
		{						
			s_pTextureMoveMethod[i]->Release();
			s_pTextureMoveMethod[i] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffMoveMethod != NULL)
	{
		s_pVtxBuffMoveMethod->Release();
		s_pVtxBuffMoveMethod = NULL;
	}

	// テクスチャの破棄
	if (s_pTextureJumpWord != NULL)
	{
		s_pTextureJumpWord->Release();
		s_pTextureJumpWord = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffJumpWord != NULL)
	{
		s_pVtxBuffJumpWord->Release();
		s_pVtxBuffJumpWord = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		// テクスチャの破棄
		if (s_pTextureJumpMethod[i] != NULL)
		{
			s_pTextureJumpMethod[i]->Release();
			s_pTextureJumpMethod[i] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffJumpMethod != NULL)
	{
		s_pVtxBuffJumpMethod->Release();
		s_pVtxBuffJumpMethod = NULL;
	}

	// テクスチャの破棄
	if (s_pTextureSelectWord != NULL)
	{
		s_pTextureSelectWord->Release();
		s_pTextureSelectWord = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffSelectWord != NULL)
	{
		s_pVtxBuffSelectWord->Release();
		s_pVtxBuffSelectWord = NULL;
	}

	for (int i = 0; i < 2; i++)
	{
		// テクスチャの破棄
		if (s_pTextureSelectMethod[i] != NULL)
		{
			s_pTextureSelectMethod[i]->Release();
			s_pTextureSelectMethod[i] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffSelectMethod != NULL)
	{
		s_pVtxBuffSelectMethod->Release();
		s_pVtxBuffSelectMethod = NULL;
	}

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

	// エネミーの更新処理
	UpdateEnemy();

	if (GetJoypadTrigger(JOYKEY_Y) || GetJoypadTrigger(JOYKEY_X) || GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(MODE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_I))
	{
		SetEnemy(D3DXVECTOR3(1000.0f,500.0f,0.0f), BALL_HOMING01);
	}
}

//=========================================
// 描画処理
//=========================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 背景の描画処理
	DrawBG();

	// 移動文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffMoveWord);
	// 描画処理
	RectDraw(pDevice, s_pTextureMoveWord, 0);
	// 移動方法文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffMoveMethod);
	// 描画処理
	RectDraw(pDevice, s_pTextureMoveMethod[GetUseJoyPad()], 0);

	// 跳躍文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffJumpWord);
	// 描画処理
	RectDraw(pDevice, s_pTextureJumpWord, 0);
	// 跳躍方法文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffJumpMethod);
	// 描画処理
	RectDraw(pDevice, s_pTextureJumpMethod[GetUseJoyPad()], 0);
	
	// 選択文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffSelectWord);
	// 描画処理
	RectDraw(pDevice, s_pTextureSelectWord, 0);
	// 選択方法文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffSelectMethod);
	// 描画処理
	RectDraw(pDevice, s_pTextureSelectMethod[GetUseJoyPad()], 0);

	// 戻る文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffBackWord);
	// 描画処理
	RectDraw(pDevice, s_pTextureBackWord, 0);
	// 戻る方法文字
	// 描写処理に前提として必要な処理
	InitDraw(pDevice, s_pVtxBuffBackMethod);
	// 描画処理
	RectDraw(pDevice, s_pTextureBackMethod[GetUseJoyPad()], 0);

	// エフェクトの描画処理
	DrawEffect();
	// パーティクルの描画処理
	DrawParticle();
	// プレイヤーの描画処理
	DrawPlayer();
	// ブロックの描画処理
	DrawBlock();
	// エネミーの描画処理
	DrawEnemy();
}

//=========================================
// 文字表示処理
//=========================================
void TextTutorial(void)
{

}
