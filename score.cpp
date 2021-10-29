//====================================
//
// スコア処理
// Author YudaKaito
//
//====================================
#include "main.h"
#include "setup.h"
#include "score.h"


// グローバル変数
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static D3DXVECTOR3				s_posScore;			// スコアの位置
static int						s_nScore;			// スコアの値

//====================================
// スコアの初期化処理
//====================================
void InitScore()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number001.png",
		&s_pTexture);

	// スコアの情報の初期化
	s_posScore = D3DXVECTOR3(450, 200, 0);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SCORE; i++)
	{
		SetRectUpRightPos(pVtx, D3DXVECTOR3(s_posScore.x - 60.0f * i, s_posScore.y, s_posScore.z), 50.0f, 50.0f);

		// rhwの設定
		InitRectRhw(pVtx);

		// 頂点カラーの設定
		SetRectColor(pVtx,&(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

		// テクスチャ座標の設定
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
// スコアの終了処理
//====================================
void UninitScore()
{
	// テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//====================================
// スコアの更新処理
//====================================
void UpdateScore()
{

}

//====================================
// スコアの描画処理
//====================================
void DrawScore()
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//
	InitDraw(pDevice, s_pVtxBuff);

	for (nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
	{
		//
		SetDraw(pDevice, s_pTexture, nCntScore * 4);
	}

}

//====================================
// スコアの設定処理
//====================================
void SetScore(int nScore)
{
	// ローカル変数
	s_nScore = nScore;
	VERTEX_2D *pVtx;
	int aNumber[8];
	int nTime = s_nScore;
	int i = 0;
	float aTex[8];
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// 一桁ずつに分ける
	while (nTime != 0)
	{
		aNumber[i] = nTime % 10;
		aTex[i] = aNumber[i] * 0.1f;
		nTime /= 10;

		// テクスチャ座標の設定
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);

		i++;
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
// スコアの加算処理
//====================================
void AddScore(int nValue)
{
	// ローカル変数
	s_nScore += nValue;
	VERTEX_2D *pVtx;
	int aNumber[8];
	int nTime = s_nScore;
	int i = 0;
	float aTex[8];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 一桁ずつに分ける
	while (nTime != 0)
	{
		aNumber[i] = nTime % 10;
		aTex[i] = aNumber[i] * 0.1f;
		nTime /= 10;

		// テクスチャ座標の設定
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + aTex[i], 0.1f + aTex[i]);

		i++;
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
// スコアの取得
//====================================
int GetScore(void)
{
	return s_nScore;
}