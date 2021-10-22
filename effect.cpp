//====================================
//
//エフェクトの処理
//Author YudaKaito
//
//====================================

//------------------------------------
//include
//------------------------------------
#include "main.h"
#include "effect.h"
#include "player.h"

//------------------------------------
//マクロ定義
//------------------------------------
#define MAX_EFFECT	(4096)	//エフェクトの最大数

//------------------------------------
//static変数
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	//頂点バッファへのポインタ
static Effect s_aEffect[MAX_EFFECT];				//エフェクトの情報

//====================================
//初期化処理
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;
	VERTEX_2D *pVtx;
	Effect *pEffeck;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEX,
		&s_pTexture);

	//エフェクトの情報の初期化
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

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		pEffeck = &(s_aEffect[nCntEffect]);

		//頂点座標の設定
		SetRectPos(pVtx, pEffeck->pos, pEffeck->fHeigth, pEffeck->fWidth);

		//rhwの設定
		//テクスチャ座標の設定
		//頂点カラーの設定
		SetupRectDefault(pVtx, &(pEffeck->col));

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
//終了処理
//====================================
void UnInitEffect(void)
{
	//テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//====================================
//更新処理
//====================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;
	int nCntEffect;
	Effect *pEffeck;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse == true)
		{
			//エフェクトのアドレスの取得
			pEffeck = &(s_aEffect[nCntEffect]);

			//頂点座標の設定
			SetRectPos(pVtx, pEffeck->pos, pEffeck->fHeigth, pEffeck->fWidth);

			//頂点カラーの設定
			SetRectColor(pVtx, &(pEffeck->col));

			//カラーの透明度の減少
			pEffeck->col.a -= 0.05f;

			//大きさを縮小
			pEffeck->fWidth += 5.0f;
			pEffeck->fHeigth += 5.0f;

			//体力の更新
			pEffeck->nLife--;
			if (pEffeck->nLife <= 0)
			{	
				pEffeck->bUse = false;
			}

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
//描画処理
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEffect;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (s_aEffect[nCntEffect].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTexture);

			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//====================================
//設定処理
//====================================
void SetEffect(D3DXVECTOR3 pos, EFFCT_TYPE type)
{
	int nCntEffect;
	Effect *pEffeck;

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//エフェクトのアドレスの取得
		pEffeck = &(s_aEffect[nCntEffect]);
		if (pEffeck->bUse == false)
		{	
			pEffeck->pos = pos;

			switch (type)
			{
			case EFFECT_SPLITBALL_MOVE:
				pEffeck->col = {1.0f,1.0f,1.0f,1.0f};
				pEffeck->fWidth = 10;
				pEffeck->fHeigth = 10;
				pEffeck->nLife = 0;
				break;
			default:
				pEffeck->col;
				pEffeck->fWidth = 0;
				pEffeck->fHeigth = 0;
				pEffeck->nLife = 0;
				break;
			}
			pEffeck->bUse = true;
			break;
		}
	}
}