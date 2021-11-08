//=========================================
// 
// 背景の処理
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "setup.h"
#include "bg.h"

//-----------------------------------------
// 背景構造体の定義
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rot;		// 向き
	bool bUse;				// 使用しているかどうか
	GRADATION gradation;	// グラデーションの状態
} BG;

//-----------------------------------------
// グローバル変数
//-----------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = {};		// テクスチャバッファ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;		// 頂点バッファ
float g_aPosTexV[NUM_BG];	// テクスチャ座標の開始位置(V値)
BG g_aBG[NUM_BG];

//====================================
// 背景の初期化処理
//====================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント
	int nCntBG;
	BG *pBG;


	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg000.jpg",
		&g_pTextureBG[0]);
	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg004.png",
		&g_pTextureBG[1]);

	// 背景の情報の初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pBG = &(g_aBG[nCntBG]);
		pBG->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBG->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBG->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBG->bUse = false;
		g_aPosTexV[nCntBG];
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pBG = &(g_aBG[nCntBG]);

		pVtx[0].pos.x = pBG->pos.x;
		pVtx[0].pos.y = pBG->pos.y;
		pVtx[0].pos.z = pBG->pos.z;

		pVtx[1].pos.x = pBG->pos.x + SCREEN_WIDTH;
		pVtx[1].pos.y = pBG->pos.y;
		pVtx[1].pos.z = pBG->pos.z;

		pVtx[2].pos.x = pBG->pos.x;
		pVtx[2].pos.y = pBG->pos.y + SCREEN_HEIGHT;
		pVtx[2].pos.z = pBG->pos.z;

		pVtx[3].pos.x = pBG->pos.x + SCREEN_WIDTH;
		pVtx[3].pos.y = pBG->pos.y + SCREEN_HEIGHT;
		pVtx[3].pos.z = pBG->pos.z;



		switch (nCntBG)
		{
		case 0:	// グラデーション上
			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			break;
		case 1:	// グラデーション下
			SetRectColor(pVtx, &(D3DXCOLOR(0.95f, 1.0f, 0.9f, 1.0f)));
			break;
		default:
			break;
		}

		// テクスチャ座標の設定
		InitRectTex(pVtx);
		// rhwの設定
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();

}

//====================================
// 背景の終了処理
//====================================
void UninitBG(void)
{
	int nCntBG;

	// テクスチャの破棄
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_pTextureBG[nCntBG] != NULL)
		{
			g_pTextureBG[nCntBG]->Release();
			g_pTextureBG[nCntBG] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

}

//====================================
// 背景の更新処理
//====================================
void UpdateBG(void)
{

}

//====================================
// 背景の描画処理
//====================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント
	int nCntBG;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 2);
	}
}
