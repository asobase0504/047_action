//=========================================
// 
// ランキングの処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "ranking.h"
#include "score.h"
#include "fade.h"
#include <stdio.h>
#include <assert.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define SELECT_NUMBER	(2)
#define SERECT_WIDTH	(200.0f)
#define SERECT_HEIGTH	(50.0f)

//------------------------------------
// ランキングのステータス列挙型
//------------------------------------
typedef enum
{
	RANKSTATE_NONE = 0,
	RANKSTATE_NORMAL,
	RANKSTATE_END,
}RANKSTATE;

//------------------------------------
// ランキングスコア構造体
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	int			nScore; //スコア
}RankScore;

//------------------------------------
// グローバル変数
//------------------------------------
static RANKSTATE s_RankState;	//ランキング処理の状態
static LPDIRECT3DTEXTURE9		s_pTextureRank		= NULL;	// テクスチャへのポインタ	(背景)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffRank		= NULL;	// 頂点バッファへのポインタ	(背景)
static LPDIRECT3DTEXTURE9		s_pTextureScore		= NULL;	// テクスチャへのポインタ	(スコア)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffScore		= NULL;	// 頂点バッファへのポインタ	(スコア)
static LPDIRECT3DTEXTURE9		s_pTextureBg		= NULL;	// テクスチャへのポインタ	(順位)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffBg		= NULL;	// 頂点バッファへのポインタ	(順位)
static LPDIRECT3DTEXTURE9		s_pTextureMenu		= NULL;	// テクスチャへのポインタ	(画面)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffMenu		= NULL;	// 頂点バッファへのポインタ	(画面)
static LPDIRECT3DTEXTURE9		s_pTextureSelect[SELECT_NUMBER] = {};	// テクスチャへのポインタ	(選択)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelect	= {};	// 頂点バッファへのポインタ	(選択)

//ランキング表示関連の変数
static RankScore	s_aRankScore[MAX_RANK];	// ランキングスコア情報
static int			s_nRankUpdate = -1;		// 更新ランクNo.
static int			s_nTimerRanking;		// ランキング画面表示タイマー
static D3DXCOLOR	s_Timercol;				// 明滅時のカラーの設定
static bool			s_bBlink;				// スコアの明滅の切り替え
static int			s_nTimerBlink;			// スコアの明滅の間隔

//メニュー画面全体関連の変数
static float		s_fWidthMenu;	// メニュー画面の幅
static float		s_fHeigthMenu;	// メニュー画面の高さ
static int			s_MenuCnt;		// メニュー画面のカウント

//select関連の変数
static D3DXCOLOR	s_Selectcol[SELECT_NUMBER];	// select中カラーの設定
static int			s_SelectCheck;				// select側をチェック

//=========================================
// ランキングの初期化処理
//=========================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;
	int nCntRank;
	int nCntScore;
	s_nTimerRanking = 0;
	s_MenuCnt = 33;
	s_bBlink = true;
	s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	s_RankState  = RANKSTATE_NORMAL;

	s_fWidthMenu = 0;
	s_fHeigthMenu = 0;
	s_SelectCheck = 0;

	// テクスチャの読込 (背景)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureBg);

	// テクスチャの読込 (画面)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureMenu);

	// テクスチャの読込 (順位)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking_rank1.png",
		&s_pTextureRank);

	// テクスチャの読込 (スコア)
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&s_pTextureScore);

	// テクスチャの読込 (選択：もう一度)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureSelect[0]);

	// テクスチャの読込 (選択：終了)
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureSelect[1]);

	// 頂点バッファの生成 (背景)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffBg,
		NULL);

	// 頂点バッファの生成 (画面)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffMenu,
		NULL);

	// 頂点バッファの生成 (順位)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRank,
		NULL);

	// 頂点バッファの生成 (スコア)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffScore,
		NULL);

	// 頂点バッファの生成 (選択)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SELECT_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffSelect,
		NULL);

	// 背景
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);
	{
		//長方形の初期化
		InitRect(pVtx);

		// 頂点座標の設定
		SetRectUpLeftPos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f)));
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffBg->Unlock();

	// メニュー画面
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);
	{
		// 頂点座標の設定
		SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), s_fWidthMenu, s_fHeigthMenu);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.9f)));

		// テクスチャ座標の設定
		InitRectTex(pVtx);

		// rhwの設定
		InitRectRhw(pVtx);
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffMenu->Unlock();

	// 順位の初期化
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
	{
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			// 順位の場所初期化
			s_aRankScore[nCntRank].pos = D3DXVECTOR3(650.0f, 200.0f + nCntRank * 150.0f, 0.0f);

			// 頂点座標の設定
			SetRectCenterPos(pVtx, s_aRankScore[nCntRank].pos, 75.0f, 75.0f);

			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f)));

			// テクスチャ座標の設定
			SetRectTex(pVtx, 0.0f + 0.2f * nCntRank, 0.2f + 0.2f * nCntRank, 0.0f, 1.0f);

			// rhwの設定
			InitRectRhw(pVtx);

			pVtx += 4;
		}
	}
	s_pVtxBuffRank->Unlock();	// 頂点バッファをアンロックする

	// スコアの初期化
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	{
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			RankScore *pScore = &(s_aRankScore[nCntRank]);
			pScore->pos.x += 600;	// スコア表示場所へ移動
			pScore->pos.y -= 40;	// スコア表示場所へ移動
			for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
			{
				//頂点座標の設定
				SetRectUpRightPos(pVtx, D3DXVECTOR3(pScore->pos.x - nCntScore * 65, pScore->pos.y, pScore->pos.z), 60.0f, 70.0f);

				// 頂点カラーの設定
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f)));

				// テクスチャ座標の設定
				SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

				// rhwの設定
				InitRectRhw(pVtx);

				pVtx += 4;
			}
		}
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffScore->Unlock();

	// 選択画面
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);
	{
		for (int i = 0; i < 2; i++)
		{
			// 頂点座標の設定
			SetRectUpRightPos(pVtx, D3DXVECTOR3(800.0f + 400.0f * i, 950.0f, 0.0f), SERECT_WIDTH, SERECT_HEIGTH);

			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)));

			// テクスチャ座標の設定
			InitRectTex(pVtx);

			// rhwの設定
			InitRectRhw(pVtx);
			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffSelect->Unlock();

}

//=========================================
// ランキングの終了処理
//=========================================
void UninitRanking(void)
{

	// テクスチャの破棄
	if (s_pTextureRank != NULL)
	{
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffRank != NULL)
	{
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	// テクスチャの破棄
	if (s_pTextureScore != NULL)
	{
		s_pTextureScore->Release();
		s_pTextureScore = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffScore != NULL)
	{
		s_pVtxBuffScore->Release();
		s_pVtxBuffScore = NULL;
	}

	// テクスチャの破棄
	if (s_pTextureBg != NULL)
	{
		s_pTextureBg->Release();
		s_pTextureBg = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffBg != NULL)
	{
		s_pVtxBuffBg->Release();
		s_pVtxBuffBg = NULL;
	}

	// テクスチャの破棄
	if (s_pTextureMenu != NULL)
	{
		s_pTextureMenu->Release();
		s_pTextureMenu = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffMenu != NULL)
	{
		s_pVtxBuffMenu->Release();
		s_pVtxBuffMenu = NULL;
	}

	// テクスチャの破棄
	for (int i = 0; i < 2; i++)
	{
		if (s_pTextureSelect[i] != NULL)
		{
			s_pTextureSelect[i]->Release();
			s_pTextureSelect[i] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffSelect != NULL)
	{
		s_pVtxBuffSelect->Release();
		s_pVtxBuffSelect = NULL;
	}

}

//=========================================
// ランキングの更新処理
//=========================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;
	
	switch (s_RankState)
	{
	case RANKSTATE_NONE:
		break;
	case RANKSTATE_NORMAL:
		s_nTimerRanking++;
		s_nTimerBlink--;

		// 点滅の切り替え処理
		if (s_nTimerBlink <= 0 && !(s_bBlink))
		{
			s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);
			s_nTimerBlink = 40;
			s_bBlink = true;

		}
		if (s_nTimerBlink <= 0 && s_bBlink)
		{
			s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
			s_nTimerBlink = 40;
			s_bBlink = false;
		}


		// メニュー画面の更新処理
		UpdateMenu();

		if (s_MenuCnt <= -10.0f)
		{
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
			for (int i = 0; i < MAX_RANK; i++)
			{
				// 頂点カラーの設定
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

				pVtx += 4;
			}
			// 頂点バッファをアンロックする
			s_pVtxBuffRank->Unlock();

			// スコアの更新
			// 頂点バッファをロックし、頂点情報へのポインタを取得
			s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
			for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
			{
				// 今回更新されたランキングか否か
				if (nCntRank == s_nRankUpdate)
				{	// 今回のスコアの点滅
					for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
					{
						// 頂点カラーの設定
						SetRectColor(pVtx, &(s_Timercol));

						pVtx += 4;
					}
				}
				else
				{	// 通常状態の色
					for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
					{
						// 頂点カラーの設定
						SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
						pVtx += 4;
					}
				}
			}
			// 頂点バッファをアンロックする
			s_pVtxBuffScore->Unlock();
		}

		//選択の切り替え
		if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_D))
		{
			s_SelectCheck = s_SelectCheck ? 0 : 1;
		}

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);
		for (int i = 0; i < SELECT_NUMBER; i++)
		{
			if (i == s_SelectCheck)
			{
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
			}
			else
			{
				SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)));
			}
			pVtx += 4;
		}
		// 頂点バッファをアンロックする
		s_pVtxBuffSelect->Unlock();

		switch (s_SelectCheck)
		{
		case 0:
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				SetGameState(GAMESTATE_NONE);
			}

			break;
		case 1:
			// 時間制限もしくは、エンターキーでリザルト画面に移行
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				//初期化
				s_RankState = RANKSTATE_NONE;
				s_MenuCnt = 33;
				s_fHeigthMenu = 0.0f;
				s_fWidthMenu = 0.0f;

				// 決定音の再生
				PlaySound(SOUND_LABEL_SE_ENTER);

				// リザルト画面に移行
				SetFade(MODE_RESULT);
			}
			break;
		default:
			assert(false);
			break;
		}
		break;
	case RANKSTATE_END:
		break;
	default:
		assert(false);
		break;
	}
}

//=========================================
// メニューの更新処理
//=========================================
void UpdateMenu(void)
{
	VERTEX_2D *pVtx;

	// メニューのバッファー
	s_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);	// 頂点バッファをロックし、頂点情報へのポインタを取得

	//ある程度縮小したら拡大・縮小を停止
	if (!(s_MenuCnt <= -10))
	{
		s_MenuCnt -= 1;

		s_fWidthMenu += s_MenuCnt;
		s_fHeigthMenu += s_MenuCnt;
	}

	// 頂点座標の設定
	SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), s_fWidthMenu, s_fHeigthMenu);

	s_pVtxBuffMenu->Unlock();	// 頂点バッファをアンロックする
}

//=========================================
// ランキングの描画処理
//=========================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRank;
	int nCntScore;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景の描画
	InitDraw(pDevice, s_pVtxBuffBg);	// Drawの初期化
	SetDraw(pDevice, s_pTextureBg, 0);	// 描画処理

	// 背景の描画
	InitDraw(pDevice, s_pVtxBuffMenu);	// Drawの初期化
	SetDraw(pDevice, s_pTextureMenu, 0);	// 描画処理

	// 順位の描画
	InitDraw(pDevice, s_pVtxBuffRank);		// Drawの初期化
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		SetDraw(pDevice, s_pTextureRank, nCntRank * 4);	// 描画処理
	}

	// スコアの描画
	InitDraw(pDevice, s_pVtxBuffScore);	// Drawの初期化
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{// 順位
		for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
		{// スコア
			SetDraw(pDevice, s_pTextureScore, (nCntRank * 4 * MAX_SCORE) + (nCntScore * 4));	// 描画処理
		}
	}

	InitDraw(pDevice, s_pVtxBuffSelect);
	for (int i = 0; i < SELECT_NUMBER; i++)
	{
		SetDraw(pDevice, s_pTextureSelect[i], i * 4);	// 描画処理
	}
}

//=========================================
// ランキングのリセット
//=========================================
void ResetRanking(void)
{
	FILE *pFile;	// ファイルポインタを宣言
	int nCntRank;

	s_nRankUpdate = -1;	// 更新ランクNo.の初期化

						// ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開いた場合
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{// ランキングの読み込み
			fscanf(pFile, "%d", &s_aRankScore[nCntRank].nScore);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		FADE pFade = GetFade();		// 今のフェード

		if (pFade == FADE_NONE)
		{// 何もしていない状態なら
		 // フェードの設定
			SetFade(MODE_TITLE);
		}
	}
}

//=========================================
// ランキングのセーブ
//=========================================
void SaveRanking(void)
{
	FILE *pFile;	// ファイルポインタを宣言
	int nCntRank;

	// ファイルを開く
	pFile = fopen(FILE_NAME, "w");

	if (pFile != NULL)
	{// ファイルが開いた場合
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{// ランキングの書き込み
			fprintf(pFile, "%d\n", s_aRankScore[nCntRank].nScore);
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{// ファイルが開かない場合
		FADE pFade = GetFade();		// 今のフェード

		if (pFade == FADE_NONE)
		{// 何もしていない状態なら
		 // フェードの設定
			SetFade(MODE_TITLE);
		}
	}

}

//=========================================
// ランキングの設定処理
//=========================================
void SetRanking(int nScore)
{
	VERTEX_2D *pVtx;
	int nCntRank;		// 繰り返し
	int nCnt2Rnak;		// 繰り返し２
	int nCheckDataMin;	// 比べて小さいほうの値を格納

	// ランキングの最下位と今回のスコアを比べる
	if (nScore >= s_aRankScore[MAX_RANK - 1].nScore)
	{
		s_nRankUpdate = 5;
		// 比較した数値を入れる。
		s_aRankScore[MAX_RANK - 1].nScore = nScore;
	}

	for (nCntRank = 0; nCntRank < MAX_RANK - 1; nCntRank++)
	{
		for (nCnt2Rnak = nCntRank + 1; nCnt2Rnak < MAX_RANK; nCnt2Rnak++)
		{// 上記の繰り返しが進むたびに繰り返し回数が-1ずつされる。

		 // 二つのデータを比較する
			if (s_aRankScore[nCntRank].nScore <= s_aRankScore[nCnt2Rnak].nScore)
			{
				// 変数を一時格納
				nCheckDataMin = s_aRankScore[nCnt2Rnak].nScore;

				// 比較した数値を入れる。
				s_aRankScore[nCnt2Rnak].nScore = s_aRankScore[nCntRank].nScore;
				s_aRankScore[nCntRank].nScore = nCheckDataMin;
			}
		}

	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		int aPosTexU[8];
		int score = s_aRankScore[nCntRank].nScore;
		int nCnt = 0;

		for (nCnt2Rnak = 0; nCnt2Rnak < MAX_SCORE; nCnt2Rnak++)
		{
			// 一桁ずつに分ける
			aPosTexU[nCnt2Rnak] = score % 10;
			score /= 10;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);

			pVtx += 4;
		}
	}
	// 頂点バッファをアンロックする
	s_pVtxBuffScore->Unlock();

	// 今回更新したランクの取得
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (s_aRankScore[nCntRank].nScore == nScore)
		{
			s_nRankUpdate = nCntRank;
		}
	}

	// セーブ処理
	SaveRanking();
}