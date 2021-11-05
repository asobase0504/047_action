//=========================================
// 
// ブロックの処理
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "setup.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

// グローバル変数
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				// テクスチャバッファ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// 頂点バッファ
static Block s_aBlock[MAX_BLOCK];								// ブロックの情報
static int s_aSumoke[MAX_BLOCK];

//====================================
// 初期化処理
//====================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;
	VERTEX_2D *pVtx;
	Block *pBlock;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&s_pTexture);

	// ブロックの情報の初期化
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		pBlock->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->col = D3DXCOLOR(0.5f, 0.35f, 0.25f, 1.0f);
		pBlock->bUse = false;
		pBlock->fHeight = 50.0f;
		pBlock->fWidth = 50.0f;
		s_aSumoke[nCntBlock] = 0;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, pBlock->pos, pBlock->fWidth, pBlock->fHeight);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(pBlock->col));
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
void UninitBlock(void)
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

int nCnt = 0;

//====================================
// 更新処理
//====================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int i = 0; i < MAX_BLOCK; i++)
	{
		// アドレスの取得
		Block *pBlock = &(s_aBlock[i]);

		if (pBlock->bUse)
		{// ブロックが使用されている

			// 頂点座標の設定
			SetRectCenterPos(pVtx, pBlock->pos, pBlock->fWidth, pBlock->fHeight);
			// 色の設定
			SetRectColor(pVtx, &(pBlock->col));

		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
// 描画処理
//====================================
void DrawBlock(void)
{
	// デバイスの取得
	 LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 描画処理
	InitDraw(pDevice, s_pVtxBuff);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		// アドレスの取得
		Block *pBlock = &(s_aBlock[i]);

		if (pBlock->bUse == true)
		{// ブロックが使用されている
			//描画処理
			SetDraw(pDevice, s_pTexture, i * 4);
		}
	}
}

//====================================
// ブロックの設定処理
//====================================
void SetBlock(D3DXVECTOR3 pos,float fHeight, float fWidth)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		// アドレスの取得
		Block *pBlock = &(s_aBlock[i]);
		if (!pBlock->bUse)
		{// ブロックが使用されていない場合

			pBlock->pos = pos;			// 座標の設定
			pBlock->fHeight = fHeight;	// 高さの設定
			pBlock->fWidth = fWidth;	// 幅の設定

			// 頂点座標の設定
			SetRectCenterPos(pVtx, pBlock->pos, pBlock->fWidth, pBlock->fHeight);

			pBlock->bUse = true;	// 使用している状態にする
			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

D3DXVECTOR3 g_Outpos;

//====================================
// ブロックとプレイヤーの当たり判定処理
//====================================
bool CollisionBlock(Player *pPlayer , D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	// 当たり判定処理
	D3DXVECTOR3 Outpos = D3DXVECTOR3(0.0f,0.0f,0.0f);	//当たり判定の交点
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_BLOCK; i++)
	{
		Block *pBlock = &(s_aBlock[i]);
		if (pBlock->bUse)
		{// ブロックが生きてたら

			// ブロック上
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, *pBlock, NULL))
			{
				pPlayer->move.y = 0.0f;
				pPlayer->pos.y = pBlock->pos.y - pBlock->fHeight;
				pPlayer->nJumpCnt = 0;
				pPlayer->jumpstate = JUMP_NONE;
				bisLanding = true;

			}

			// ブロック下
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, *pBlock, &(Outpos)))
			{
				D3DXVECTOR3 p;
				p = Outpos;
  				pPlayer->move.y = 0.0f;
				pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + p.y;
			}

			// ブロック左
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, *pBlock, NULL))
			{
				pPlayer->move.x = 0.0f;
				pPlayer->pos.x = pBlock->pos.x - pBlock->fWidth - pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
// 				pPlayer->pos.x = pPlayer->pos.x - 5.0f;
			}

			// ブロック右
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, *pBlock, NULL))
			{
				pPlayer->move.x = 0.0f;
				pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
// 				pPlayer->pos.x = pPlayer->pos.x + 5.0f;
			}
		}
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	g_Outpos = Outpos;
	return bisLanding;
}

//====================================
// ブロックとエネミーの当たり判定処理
//====================================
bool CollisionBlockEnemy(Enemy *pEnemy, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	// 当たり判定処理
	Block *pBlock = s_aBlock;
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		if (pBlock->bUse)
		{// ブロックが生きてたら

		 // 上
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, s_aBlock[nCntBlock], NULL))
			{

				bisLanding = true;

			}

			// 下
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, s_aBlock[nCntBlock], NULL))
			{// ブロックの座標と座標が重なり合ったら
 				pEnemy->move.y *= -1.0f;
//				pEnemy->pos.y = pBlock->pos.y + pBlock->fHeight + pEnemy->fHeight + 5.0f;
				switch (pEnemy->type)
				{
				case SPLITBALL_FIRST:
				case SPLITBALL_SECOND:
				case SPLITBALL_LAST:
						pEnemy->nLife--;
					break;
				case EXTENDBALL_UP:
					break;
				case EXTENDBALL_DWON:
					pEnemy->nLife--;
					break;
				case EXTENDBALL_LEFT:
				case EXTENDBALL_RIGHT:
				default:
					break;
				}
			}

			// 左
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, s_aBlock[nCntBlock], NULL))
			{// ブロックの座標と座標が重なり合ったら
 				pEnemy->move.x *= -1.0f;
// 				pEnemy->pos.x = pBlock->pos.x - pBlock->fWidth - pEnemy->fWidth - 1.0f;
				pEnemy->pos.x = pEnemy->pos.x - 5.0f;
					pEnemy->nLife--;
			}

			// 右
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, s_aBlock[nCntBlock], NULL))
			{// ブロックの座標と座標が重なり合ったら
 				pEnemy->move.x *= -1.0f;
// 				pEnemy->pos.x = pBlock->pos.x + pBlock->fWidth + pEnemy->fWidth + 1.0f;
					pEnemy->nLife--;
					pEnemy->nAtkInterval = 150;
			}
		}
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	return bisLanding;
}


//====================================
// ブロックの線分の交差判定処理
//====================================
bool CrossingBlock(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2 ,JUDGE_POSITION position,Block block, D3DXVECTOR3 *Outpos)
{
	// 当たり判定処理
	Block *pBlock = &(block);
	Segment vBlock;
	Segment vTarget;

	switch (position)
	{
	case POSITION_UP:
		// ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_DWON:
		// ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_LEFT:
		// ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_RIGHT:
		// ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	default:
		break;
	}

	// 被対象のベクトルの獲得
	vTarget.start = *pPos2;
	vTarget.vector = *pPos1 - *pPos2;

	// ベクトルの始点同士の距離。
	D3DXVECTOR3 v = vTarget.start - vBlock.start;

	// ブロックのベクトルと被対象のベクトルが平行か調べる
	float Bv_Tv = D3DXVec2Cross(&(vBlock.vector), &(vTarget.vector));
	if (Bv_Tv == 0.0f)
	{
		// 並行である。
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(vBlock.vector));
	float v_Tv = D3DXVec2Cross(&(v), &(vTarget.vector));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
	{
		return false;
	}

	if (Outpos != NULL)
	{
		Outpos->x = vBlock.start.x + vBlock.vector.x * hit1;
		Outpos->y = vBlock.start.y + vBlock.vector.y * hit1;
	}
	return true;
}

//====================================
// ブロックの取得処理
//====================================
Block* GetBlock(void)
{
	return s_aBlock;
}

D3DXVECTOR3 GetOut(void)
{
	return g_Outpos;
}
