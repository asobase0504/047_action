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
} AG;

//-----------------------------------------
// グローバル変数
//-----------------------------------------
static LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = {};	// テクスチャバッファ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;		// 頂点バッファ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEN = NULL;		// 頂点バッファ
static float g_aPosTexV[NUM_BG];	// テクスチャ座標の開始位置(V値)
static AG g_aBG[NUM_BG];

//====================================
// 背景の初期化処理
//====================================
void InitAG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント
	int nCntBG;
	AG *pBG;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_pTextureBG[0]);
	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy002.png",
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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	{
		pBG = &(g_aBG[0]);

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
		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)));

		// テクスチャ座標の設定
		InitRectTex(pVtx);
		// rhwの設定
		InitRectRhw(pVtx);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEN,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEN->Lock(0, 0, (void**)&pVtx, 0);
	{
		pBG->pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
		pVtx[0].pos.x = pBG->pos.x;
		pVtx[0].pos.y = pBG->pos.y;
		pVtx[0].pos.z = pBG->pos.z;

		pVtx[1].pos.x = pBG->pos.x + 200.0f;
		pVtx[1].pos.y = pBG->pos.y;
		pVtx[1].pos.z = pBG->pos.z;

		pVtx[2].pos.x = pBG->pos.x;
		pVtx[2].pos.y = pBG->pos.y + 200.0f;
		pVtx[2].pos.z = pBG->pos.z;

		pVtx[3].pos.x = pBG->pos.x + 200.0f;
		pVtx[3].pos.y = pBG->pos.y + 200.0f;
		pVtx[3].pos.z = pBG->pos.z;
		SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));

		// テクスチャ座標の設定
		InitRectTex(pVtx);
		// rhwの設定
		InitRectRhw(pVtx);
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffEN->Unlock();

}

//====================================
// 背景の終了処理
//====================================
void UninitAG(void)
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
void UpdateAG(void)
{

}

//====================================
// 背景の描画処理
//====================================
void DrawAG(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント
	int nCntBG;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBG[0]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	
	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, g_pVtxBuffEN, 0, sizeof(VERTEX_2D));
	
	// テクスチャの設定
	pDevice->SetTexture(1, g_pTextureBG[1]);

	//スポット
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(1, D3DTSS_COLOROP	, D3DTOP_BLENDTEXTUREALPHA);
	pDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	pDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
}
