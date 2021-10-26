//=========================================
//
//ブロックの処理
//Author YudaKaito
//
//=========================================
#include "main.h"
#include "setup.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

//グローバル変数
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;				//テクスチャバッファ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			//頂点バッファ
static Block s_aBlock[MAX_BLOCK];								//ブロックの情報
static int s_aSumoke[MAX_BLOCK];

//====================================
//初期化処理
//====================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;
	VERTEX_2D *pVtx;
	Block *pBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&s_pTexture);

	//ブロックの情報の初期化
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		pBlock->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pBlock->bUse = false;
		pBlock->fHeight = 50.0f;
		pBlock->fWidth = 50.0f;
		s_aSumoke[nCntBlock] = 0;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);

		//頂点座標の設定
		SetRectPos(pVtx, pBlock->pos, pBlock->fHeight, pBlock->fWidth);

		//rhwの設定
		//頂点カラーの設定
		//テクスチャ座標の設定
		SetupRectDefault(pVtx, &(pBlock->col));

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
//終了処理
//====================================
void UninitBlock(void)
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

int nCnt = 0;

//====================================
//更新処理
//====================================
void UpdateBlock(void)
{
	int nCntBlock;			//ブロックの最大数分
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	Block *pBlock;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//アドレスの取得
		pBlock = &(s_aBlock[nCntBlock]);

		if (pBlock->bUse == true)
		{//ブロックが使用されている

			 //頂点座標の設定
			SetRectPos(pVtx, pBlock->pos, pBlock->fHeight, pBlock->fWidth);

			SetRectColor(pVtx, &(pBlock->col));


			//画面端に言った場合
			if (pBlock->pos.x <= 0)
			{
				pBlock->bUse = false; //使用していない状態にする
			}
			if (pBlock->pos.y <= 20)
			{
				pBlock->bUse = false; //使用していない状態にする
			}
			if (pBlock->pos.x >= SCREEN_WIDTH)
			{
				pBlock->bUse = false; //使用していない状態にする
			}
			if (pBlock->pos.y >= SCREEN_HEIGHT)
			{
				pBlock->bUse = false; //使用していない状態にする
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
//描画処理
//====================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Block *pBlock;
	int nCntBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//アドレスの取得
		pBlock = &(s_aBlock[nCntBlock]);

		if (pBlock->bUse == true)
		{//ブロックが使用されている

		 //テクスチャの設定
			pDevice->SetTexture(0, s_pTexture);

			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);

		}
	}

}

//====================================
//ブロックの設定処理
//====================================
void SetBlock(D3DXVECTOR3 pos,float fHeight, float fWidth, int type)
{
	int nCntBlock;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	Block *pBlock;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//アドレスの取得
		pBlock = &(s_aBlock[nCntBlock]);
		if (!pBlock->bUse)
		{//ブロックが使用されていない場合

			pBlock->pos = pos;
			pBlock->fHeight = fHeight;
			pBlock->fWidth = fWidth;

			//頂点座標の設定
			SetRectPos(pVtx, pBlock->pos, pBlock->fHeight, pBlock->fWidth);

			pBlock->bUse = true;	//使用している状態にする
			break;

		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//====================================
//ブロックとプレイヤーの当たり判定処理
//====================================
bool CollisionBlock(Player *pPlayer , D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	//当たり判定処理
	Block *pBlock = s_aBlock;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		if (pBlock->bUse == true)
		{//ブロックが生きてたら

			//上
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, s_aBlock[nCntBlock]))
			{
				pPlayer->move.y = 0.0f;
 				pPlayer->pos.y = pBlock->pos.y - pBlock->fHeight;
				pPlayer->nJumpCnt = 0;
				pPlayer->jumpstate = JUMP_NONE;
				bisLanding = true;

				pBlock->col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			}

			//下
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, s_aBlock[nCntBlock]))
			{
				pPlayer->move.y = 0.0f;
				pPlayer->pos.y = pBlock->pos.y + pBlock->fHeight + pPlayer->fHeigth;
				pBlock->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}

			//左
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, s_aBlock[nCntBlock]))
			{
				pPlayer->move.x = 0.0f;
				pPlayer->pos.x = pBlock->pos.x - pBlock->fWidth - pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
//				pPlayer->pos.x = pPlayer->pos.x - 5.0f;
				pBlock->col = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			}

			//右
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, s_aBlock[nCntBlock]))
			{
				pPlayer->move.x = 0.0f;
				pPlayer->pos.x = pBlock->pos.x + pBlock->fWidth + pPlayer->fWidth;
				pPlayer->rot.z = pPlayer->rotOld.z;
//				pPlayer->pos.x = pPlayer->pos.x + 5.0f;
				pBlock->col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			}
		}
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	return bisLanding;
}

//====================================
//ブロックとエネミーの当たり判定処理
//====================================
bool CollisionBlockEnemy(Enemy *pEnemy, D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	//当たり判定処理
	Block *pBlock = s_aBlock;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pBlock = &(s_aBlock[nCntBlock]);
		if (pBlock->bUse)
		{//ブロックが生きてたら

		 //上
			if (CrossingBlock(&(pos1), &(pos2), POSITION_UP, s_aBlock[nCntBlock]))
			{
//				pEnemy->move *= -1.0f;
				pEnemy->pos.y = pBlock->pos.y - pBlock->fHeight - pEnemy->fHeight - 1.0f;

				switch (pEnemy->type)
				{
				case ENEMYTYPE_SPLITBALL_FIRST:
				case ENEMYTYPE_SPLITBALL_SECOND:
				case ENEMYTYPE_SPLITBALL_LAST:
					if (pEnemy->nAtkInterval < 150)
					{
						pEnemy->nLife--;
						pEnemy->nAtkInterval = 150;
					}
					break;
				case ENEMYTYPE_EXTENDBALL_UP:
					pEnemy->nLife--;
					break;
				case ENEMYTYPE_EXTENDBALL_DWON:
				case ENEMYTYPE_EXTENDBALL_LEFT:
				case ENEMYTYPE_EXTENDBALL_RIGHT:
				default:
					break;
				}
				bisLanding = true;

				pBlock->col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			}

			//下
			if (CrossingBlock(&(pos1), &(pos2), POSITION_DWON, s_aBlock[nCntBlock]))
			{//ブロックの座標と座標が重なり合ったら
//				pEnemy->move *= -1.0f;
				pEnemy->pos.y = pBlock->pos.y + pBlock->fHeight + pEnemy->fHeight + 5.0f;
				switch (pEnemy->type)
				{
				case ENEMYTYPE_SPLITBALL_FIRST:
				case ENEMYTYPE_SPLITBALL_SECOND:
				case ENEMYTYPE_SPLITBALL_LAST:
					if (pEnemy->nAtkInterval < 150)
					{
						pEnemy->nLife--;
						pEnemy->nAtkInterval = 150;
					}
					break;
				case ENEMYTYPE_EXTENDBALL_UP:
					break;
				case ENEMYTYPE_EXTENDBALL_DWON:
					pEnemy->nLife--;
					break;
				case ENEMYTYPE_EXTENDBALL_LEFT:
				case ENEMYTYPE_EXTENDBALL_RIGHT:
				default:
					break;
				}
				pBlock->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}

			//左
			if (CrossingBlock(&(pos1), &(pos2), POSITION_LEFT, s_aBlock[nCntBlock]))
			{//ブロックの座標と座標が重なり合ったら
//				pEnemy->move *= -1.0f;
//				pEnemy->pos.x = pBlock->pos.x - pBlock->fWidth - pEnemy->fWidth - 1.0f;
				pEnemy->pos.x = pEnemy->pos.x - 5.0f;
				if (pEnemy->nAtkInterval < 150)
				{
					pEnemy->nLife--;
					pEnemy->nAtkInterval = 150;
				}
				pBlock->col = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			}

			//右
			if (CrossingBlock(&(pos1), &(pos2), POSITION_RIGHT, s_aBlock[nCntBlock]))
			{//ブロックの座標と座標が重なり合ったら
//				pEnemy->move *= -1.0f;
//				pEnemy->pos.x = pBlock->pos.x + pBlock->fWidth + pEnemy->fWidth + 1.0f;
				pEnemy->pos.x = pEnemy->pos.x + 5.0f;
				if (pEnemy->nAtkInterval < 150)
				{
					pEnemy->nLife--;
					pEnemy->nAtkInterval = 150;
				} 
				pBlock->col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f);
			}
		}
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	return bisLanding;
	return false;
}


//====================================
//ブロックの線分の交差判定処理
//====================================
bool CrossingBlock(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2 ,JUDGE_POSITION position,Block block)
{
	//当たり判定処理
	Block *pBlock = &(block);
	Segment vBlock;
	Segment vTarget;

	switch (position)
	{
	case POSITION_UP:
		//ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_DWON:
		//ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_LEFT:
		//ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x - pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	case POSITION_RIGHT:
		//ブロックのベクトルの獲得
		vBlock.start = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y - pBlock->fHeight, pBlock->pos.z);
		vBlock.vector = D3DXVECTOR3(pBlock->pos.x + pBlock->fWidth, pBlock->pos.y + pBlock->fHeight, pBlock->pos.z) - vBlock.start;
		break;
	default:
		break;
	}

	//被対象のベクトルの獲得
	vTarget.start = *pPos2;
	vTarget.vector = *pPos1 - *pPos2;

	//ベクトルの始点同士の距離。
	D3DXVECTOR3 v = vTarget.start - vBlock.start;

	//ブロックのベクトルと被対象のベクトルが平行か調べる
	float Bv_Tv = D3DXVec2Cross(&(vBlock.vector), &(vTarget.vector));
	if (Bv_Tv == 0.0f)
	{
		//並行である。
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(vBlock.vector));
	float v_Tv = D3DXVec2Cross(&(v), &(vTarget.vector));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	return !((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f));
}

//====================================
//ブロックの取得処理
//====================================
Block* GetBlock(void)
{
	return s_aBlock;
}
