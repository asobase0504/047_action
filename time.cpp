//=========================================
// 
// 時間の処理
// Author YudaKaito
// 
//=========================================

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "time.h"
#include "setup.h"

#include <assert.h>

//------------------------------------
//マクロ定義
//------------------------------------
#define TIME_POS	"data/TEXTURE/number001.png"
#define TIME_MAX	(6)

//------------------------------------
//  static 変数
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				// テクスチャバッファ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// 頂点バッファ

static int s_nTime;			//時間
static D3DXVECTOR3 s_pos;	//数値の位置
static D3DXCOLOR s_col;

//=========================================
// 初期化処理
//=========================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		TIME_POS,
		&s_pTexture);

	//タイムの初期化処理
	s_pos = D3DXVECTOR3(450.0f, 150.0f, 0.0f);
	s_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	s_nTime = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
							
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < TIME_MAX; i++)
	{
		// 頂点座標の設定
		SetRectUpRightPos(pVtx, D3DXVECTOR3(s_pos.x - 60.0f * i, s_pos.y, s_pos.z), 50.0f, 50.0f);

		// カラーの設定
		SetRectColor(pVtx, &(s_col));

		// rhwの設定 
		InitRectRhw(pVtx);

		// テクスチャ座標の設定
		SetRectTex(pVtx, 0.0f, 1.0f, 0.0f, 0.1f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();


}

//=========================================
// 終了処理
//=========================================
void UninitTime(void)
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

//=========================================
// 更新処理
//=========================================
void UpdateTime(void)
{
	s_nTime++;
	int aNumber[8];

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	//繰り上げ処理
	if (s_nTime % 100 >= 60)
	{
		s_nTime += 40;
	}
	assert(s_nTime % 100 < 60);	// 60フレーム以降の場合 assert

	if (s_nTime % 10000 >= 6000)
	{
		s_nTime += 4000;
	}
	assert(s_nTime % 100 < 60);//60フレーム以降の場合 assert

	int nTime = s_nTime;
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

//=========================================
// 描画処理
//=========================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	InitDraw(pDevice, s_pVtxBuff);

	for (int i = 0; i < TIME_MAX; i++)
	{
		pDevice = SetDraw(pDevice, s_pTexture, i * 4);
	}
}