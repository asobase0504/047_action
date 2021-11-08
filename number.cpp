//====================================
//
// スコア処理
// Author YudaKaito
//
//====================================
#include "main.h"
#include "setup.h"
#include "number.h"

// グローバル変数
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// 頂点バッファへのポインタ

//====================================
// スコアの初期化処理
//====================================
void InitNumber()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number002.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	InitRectPos(pVtx);

	// rhwの設定
	InitRectRhw(pVtx);

	// 頂点カラーの設定
	SetRectColor(pVtx, &(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)));

	// テクスチャ座標の設定
	SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

	pVtx += 4;

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
// スコアの終了処理
//====================================
void UninitNumber()
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
void UpdateNumber()
{

}

//====================================
// スコアの描画処理
//====================================
void DrawNumber()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 描画設定
	InitDraw(pDevice, s_pVtxBuff);

	// テクスチャの描画
	RectDraw(pDevice, s_pTexture, 0);
}

//====================================
// 数字の設定処理
//====================================
void SetNumber(VERTEX_2D *pVtx, int nNumber)
{
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float Tex = nNumber * 0.1f;

	// テクスチャ座標の設定
	SetRectTex(pVtx, 0.0f, 1.0f, 0.0f + Tex, 0.1f + Tex);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}