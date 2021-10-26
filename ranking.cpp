//=========================================
//
//ランキングの処理
//Author YudaKaito
//
//=========================================
//------------------------------------
//include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "input.h"
#include "sound.h"
#include <stdio.h>
#include "ranking.h"
//#include "score.h"
#include "fade.h"

//------------------------------------
//ランキングスコア構造体
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nScore;			//スコア
}RankScore;

//------------------------------------
//グローバル変数
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTextureRank = NULL;			//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9 s_pTextureRankScore = NULL;		//テクスチャへのポインタ
static LPDIRECT3DTEXTURE9 s_pTextureRankbg = NULL;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffRank = NULL;		//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffRankScore = NULL;	//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffRankbg = NULL;		//テクスチャへのポインタ
static RankScore s_aRankScore[MAX_RANK];					//ランキングスコア情報
static int s_nRankUpdate = -1;								//更新ランクNo.
static int s_nTimerRanking;									//ランキング画面表示タイマー
static bool s_bBlink;
static int s_nTimerBlink;
static D3DXCOLOR s_Timercol;

//=========================================
//ランキングの初期化処理
//=========================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntRank;
	int nCntScore;
	s_nTimerRanking = 0;
	s_bBlink = true;
	s_Timercol = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTextureRankbg);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/ranking_rank1.png",
		&s_pTextureRank);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&s_pTextureRankScore);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRankbg,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRank,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK * MAX_RANKSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffRankScore,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankbg->Lock(0, 0, (void**)&pVtx, 0);

	//長方形の初期化
	//rhwの設定
	//テクスチャ座標の設定
	InitRect(pVtx);

	//頂点座標の設定
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = SCREEN_WIDTH;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 0.0f;
	pVtx[2].pos.y = SCREEN_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = SCREEN_WIDTH;
	pVtx[3].pos.y = SCREEN_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//頂点カラーの設定
	SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f)));

	//頂点バッファをアンロックする
	s_pVtxBuffRankbg->Unlock();

	//
	//順位の初期化
	//
	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		//順位の場所初期化
		s_aRankScore[nCntRank].pos = D3DXVECTOR3(450.0f, 100.0f + nCntRank * 100.0f, 0.0f);

		//頂点座標の設定
		SetRectCenterPos(pVtx, s_aRankScore[nCntRank].pos, 50.0f, 50.0f);
		
		//頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

		//テクスチャ座標の設定
		SetRextTex(pVtx, 0.0f + 0.2f * nCntRank, 0.2f + 0.2f * nCntRank, 0.0f, 1.0f);

		//rhwの設定
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	s_pVtxBuffRank->Unlock();

	//
	//スコアの初期化
	//
	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		s_aRankScore[nCntRank].pos.x += 100;	//スコア表示場所へ移動
		for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
		{
			pVtx[0].pos.x = s_aRankScore[nCntRank].pos.x - 20.0f + nCntScore * 45;
			pVtx[0].pos.y = s_aRankScore[nCntRank].pos.y - 20.0f;
			pVtx[0].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

			pVtx[1].pos.x = s_aRankScore[nCntRank].pos.x + 20.0f + nCntScore * 45;
			pVtx[1].pos.y = s_aRankScore[nCntRank].pos.y - 20.0f;
			pVtx[1].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

			pVtx[2].pos.x = s_aRankScore[nCntRank].pos.x - 20.0f + nCntScore * 45;
			pVtx[2].pos.y = s_aRankScore[nCntRank].pos.y + 20.0f;
			pVtx[2].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;

			pVtx[3].pos.x = s_aRankScore[nCntRank].pos.x + 20.0f + nCntScore * 45;
			pVtx[3].pos.y = s_aRankScore[nCntRank].pos.y + 20.0f;
			pVtx[3].pos.z = s_aRankScore[nCntRank].pos.z + 0.0f;


			//頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

			//テクスチャ座標の設定
			SetRextTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

			//rhwの設定
			InitRectRhw(pVtx);

			pVtx += 4;
		}
	}
	//頂点バッファをアンロックする
	s_pVtxBuffRankScore->Unlock();

}

//=========================================
//ランキングの終了処理
//=========================================
void UninitRanking(void)
{

	//テクスチャの破棄
	if (s_pTextureRank != NULL)
	{
		s_pTextureRank->Release();
		s_pTextureRank = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuffRank != NULL)
	{
		s_pVtxBuffRank->Release();
		s_pVtxBuffRank = NULL;
	}

	//テクスチャの破棄
	if (s_pTextureRankScore != NULL)
	{
		s_pTextureRankScore->Release();
		s_pTextureRankScore = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuffRankScore != NULL)
	{
		s_pVtxBuffRankScore->Release();
		s_pVtxBuffRankScore = NULL;
	}
}

//=========================================
//ランキングの更新処理
//=========================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;
	int nCntRank;
	int nCntScore;

	s_nTimerRanking++;
	s_nTimerBlink--;

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

	if (s_nRankUpdate != -1)
	{

	}
	//時間制限もしくは、エンターキーでリザルト画面に移行
	if (s_nTimerRanking >= 3000 || GetKeyboardTrigger(DIK_RETURN))
	{
		//決定音の再生
		PlaySound(SOUND_LABEL_SE_ENTER);

		//リザルト画面に移行
		SetFade(MODE_RESULT);
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (nCntRank == s_nRankUpdate)
		{
			for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
			{
				//頂点カラーの設定
				SetRectColor(pVtx, &(s_Timercol));

				pVtx += 4;
			}
		}
		else
		{
			for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
			{
				//頂点カラーの設定
				SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));
				pVtx += 4;
			}
		}
	}
	//頂点バッファをアンロックする
	s_pVtxBuffRankScore->Unlock();
}

//=========================================
//ランキングの描画処理
//=========================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRank;
	int nCntScore;

	// デバイスの取得
	pDevice = GetDevice();

	// Drawの初期化
	InitDraw(pDevice, s_pVtxBuffRankbg);
	// 描画処理
	SetDraw(pDevice, s_pTextureRankbg, 0);

	// Drawの初期化
	InitDraw(pDevice, s_pVtxBuffRank);
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		// 描画処理
		SetDraw(pDevice, s_pTextureRank, nCntRank * 4);
	}

	// Drawの初期化
	InitDraw(pDevice, s_pVtxBuffRankScore);
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{//順位
		for (nCntScore = 0; nCntScore < MAX_RANKSCORE; nCntScore++)
		{//スコア
			// 描画処理
			SetDraw(pDevice, s_pTextureRankScore, (nCntRank * 4 * MAX_RANKSCORE) + (nCntScore * 4));
		}
	}
}

//=========================================
//ランキングのリセット
//=========================================
void ResetRanking(void)
{
	FILE *pFile;	//ファイルポインタを宣言
	int nCntRank;

	s_nRankUpdate = -1;	//更新ランクNo.の初期化

						//ファイルを開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{//ランキングの読み込み
			fscanf(pFile, "%d", &s_aRankScore[nCntRank].nScore);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開かない場合
		FADE pFade = GetFade();		//今のフェード

		if (pFade == FADE_NONE)
		{//何もしていない状態なら
		 //フェードの設定
			SetFade(MODE_TITLE);
		}
	}
}

//=========================================
//ランキングのセーブ
//=========================================
void SaveRanking(void)
{
	FILE *pFile;	//ファイルポインタを宣言
	int nCntRank;

	//ファイルを開く
	pFile = fopen(FILE_NAME, "w");

	if (pFile != NULL)
	{//ファイルが開いた場合
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{//ランキングの書き込み
			fprintf(pFile, "%d\n", s_aRankScore[nCntRank].nScore);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開かない場合
		FADE pFade = GetFade();		//今のフェード

		if (pFade == FADE_NONE)
		{//何もしていない状態なら
		 //フェードの設定
			SetFade(MODE_TITLE);
		}
	}

}

//=========================================
//ランキングの設定処理
//=========================================
void SetRanking(int nScore)
{
	VERTEX_2D *pVtx;
	int nCntRank;		//繰り返し
	int nCnt2Rnak;		//繰り返し２
	int nCheckDataMin;	//比べて小さいほうの値を格納

	//ランキングの最下位と今回のスコアを比べる
	if (nScore >= s_aRankScore[MAX_RANK - 1].nScore)
	{
		s_nRankUpdate = 5;
		//比較した数値を入れる。
		s_aRankScore[MAX_RANK - 1].nScore = nScore;
	}

	for (nCntRank = 0; nCntRank < MAX_RANK - 1; nCntRank++)
	{
		for (nCnt2Rnak = nCntRank + 1; nCnt2Rnak < MAX_RANK; nCnt2Rnak++)
		{//上記の繰り返しが進むたびに繰り返し回数が-1ずつされる。

		 //二つのデータを比較する
			if (s_aRankScore[nCntRank].nScore <= s_aRankScore[nCnt2Rnak].nScore)
			{
				//変数を一時格納
				nCheckDataMin = s_aRankScore[nCnt2Rnak].nScore;

				//比較した数値を入れる。
				s_aRankScore[nCnt2Rnak].nScore = s_aRankScore[nCntRank].nScore;
				s_aRankScore[nCntRank].nScore = nCheckDataMin;
			}
		}

	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		int aPosTexU[8];
		int score = s_aRankScore[nCntRank].nScore;
		int nCnt = 0;

		//一桁ずつに分ける
		while (score != 0)
		{
			aPosTexU[nCnt] = score % 10;
			score /= 10;
			nCnt++;
		}

		for (nCnt2Rnak = 0; nCnt2Rnak < MAX_RANKSCORE; nCnt2Rnak++)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * aPosTexU[nCnt2Rnak], 1.0f);

			pVtx += 4;
		}
	}
	//頂点バッファをアンロックする
	s_pVtxBuffRankScore->Unlock();

	//今回更新したランクの取得
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (s_aRankScore[nCntRank].nScore == nScore)
		{
			s_nRankUpdate = nCntRank;
		}
	}

	//セーブ処理
	SaveRanking();
}