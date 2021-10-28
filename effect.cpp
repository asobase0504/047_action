//====================================
// 
// エフェクトの処理
// Author YudaKaito
// 
//====================================

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "effect.h"
#include "player.h"
#include <assert.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define MAX_EFFECT	(4096)	// エフェクトの最大数

//------------------------------------
// static変数
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[EFFECT_MAX] = {};		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static Effect s_aEffect[MAX_EFFECT];				// エフェクトの情報

//====================================
// 初期化処理
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	VERTEX_2D *pVtx;
	Effect *pEffeck;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEX,
		&s_pTexture[EFFECT_SPLITBALL_MOVE]);

	// エフェクトの情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);

		pEffeck->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffeck->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pEffeck->fWidth = 0.0f;
		pEffeck->fHeigth = 0.0f;
		pEffeck->nLife = 100;
		pEffeck->bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, pEffeck->pos, pEffeck->fWidth, pEffeck->fHeigth);
		// 頂点カラーの設定
		SetRectColor(pVtx, &(pEffeck->col));
		// テクスチャ座標の設定
		InitRectTex(pVtx);
		// rhwの設定
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
// 終了処理
//====================================
void UnInitEffect(void)
{
	for (int i = 0; i < EFFECT_MAX; i++)
	{
		// テクスチャの破棄
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//====================================
// 更新処理
//====================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;
	int nCntEffect;
	Effect *pEffeck;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse == true)
		{
			// エフェクトのアドレスの取得
			pEffeck = &(s_aEffect[nCntEffect]);

			// 頂点座標の設定
			SetRectCenterPos(pVtx, pEffeck->pos, pEffeck->fWidth, pEffeck->fHeigth);

			// 頂点カラーの設定
			SetRectColor(pVtx, &(pEffeck->col));

			// カラーの透明度の減少
			pEffeck->col.a -= 0.05f;

			// 大きさを縮小
			pEffeck->fWidth += 5.0f;
			pEffeck->fHeigth += 5.0f;

			// 体力の更新
			pEffeck->nLife--;
			if (pEffeck->nLife <= 0)
			{	
				pEffeck->bUse = false;
			}

			pVtx += 4;
		}
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
// 描画処理
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	Effect *pEffeck;

	// デバイスの取得
	pDevice = GetDevice();

	InitDraw(pDevice, s_pVtxBuff);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);
		if (s_aEffect[nCntEffect].bUse == true)
		{
			switch (pEffeck->type)
			{
			case EFFECT_SPLITBALL_MOVE:
				AddSetDraw(pDevice, s_pTexture[EFFECT_SPLITBALL_MOVE], nCntEffect * 4);
				break;
			default:
				break;
			}
		}
	}
}

//====================================
// 設定処理
//====================================
void SetEffect(D3DXVECTOR3 pos, EFFCT_TYPE type)
{
	int nCntEffect;
	Effect *pEffeck;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// エフェクトのアドレスの取得
		pEffeck = &(s_aEffect[nCntEffect]);
		if (pEffeck->bUse == false)
		{	
			pEffeck->pos	= pos;
			pEffeck->type	= type;

			switch (type)
			{
			case EFFECT_SPLITBALL_MOVE:
				pEffeck->col = {1.0f,1.0f,1.0f,1.0f};
				pEffeck->fWidth = 10;
				pEffeck->fHeigth = 10;
				pEffeck->nLife = 0;
				break;
			default:
				assert(false);
				break;
			}
			pEffeck->bUse = true;
			break;
		}
	}
}