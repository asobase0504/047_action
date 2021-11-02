//====================================
// 
// 敵に関する関数
// Author Yuda Kaito
// 
//====================================
#include "main.h"
#include "setup.h"
#include "enemy.h"
#include "particle.h"
#include "effect.h"
#include "player.h"
#include "block.h"
#include "score.h"
#include <assert.h>

//------------------------------------
// マクロ定義
//------------------------------------

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[ENEMYTYPE_MAX] = {};		// テクスチャバッファ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;			// 頂点バッファ
static Enemy s_aEnemy[MAX_ENEMY];							// 敵の情報
static int s_nEnemyCnt = 0;									// 敵の数
int score[] =
{
	100,	// 別れる球最初
	50,		// 別れる球二回目
	25,		// 別れる球
	40,		// 伸びる円、上から下
	40,		// 伸びる円、下から上
	40,		// 伸びる円、左から右
	40,		// 伸びる円、右から左
	10,		// 直進する長方形、上から下
	10,		// 直進する長方形、下から上
	10,		// 直進する長方形、左から右
	10,		// 直進する長方形、右から左
	0,		// ダメージ壁

};

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void NeutralEnemy(Enemy *pEnemy);				// 待機処理
static void AttackEnemy(Enemy *pEnemy);				// 攻撃処理
static void DieEnemy(Enemy *pEnemy);					// 死亡処理

//====================================
// 敵の初期化処理
//====================================
void InitEnemy(void)
{
	Enemy *pEnemy;				// 敵のポインタ
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[SPLITBALL_FIRST]);

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[SPLITBALL_SECOND]);

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[SPLITBALL_LAST]);

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[EXTENDBALL_UP]);

	// 構造体の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		pEnemy->fWidth = 5.0f;								// 画像の幅
		pEnemy->fHeight = 5.0f;								// 画像の高さ
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		pEnemy->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		pEnemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		pEnemy->nLife = 100;								// 寿命
		pEnemy->bUse = false;								// 使用されているかどうか
		pEnemy->nAtkInterval = 0;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		InitRect(pVtx);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
// 敵の終了処理
//====================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < ENEMYTYPE_MAX; nCntEnemy++)
	{
		// テクスチャの破棄
		if (s_pTexture[nCntEnemy] != NULL)
		{
			s_pTexture[nCntEnemy]->Release();
			s_pTexture[nCntEnemy] = NULL;
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
// 敵の更新処理
//====================================
void UpdateEnemy(void)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	Enemy *pEnemy;			// 敵のポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		if (pEnemy->bUse == true)
		{
			pEnemy->nAtkInterval++;

			// 待機処理
			NeutralEnemy(pEnemy);
			// 攻撃処理
			AttackEnemy(pEnemy);

			// 敵が死んだ場合
			if (pEnemy->nLife <= 0)
			{
				// 死んだときの処理
				DieEnemy(pEnemy);
			}

			// 座標の移動処理
			pEnemy->pos += pEnemy->move;

			// それぞれの頂点座標の当たり判定
			CollisionBlockEnemy(pEnemy, pVtx[0].pos, pVtx[1].pos);
			CollisionBlockEnemy(pEnemy, pVtx[1].pos, pVtx[2].pos);
			CollisionBlockEnemy(pEnemy, pVtx[2].pos, pVtx[3].pos);
			CollisionBlockEnemy(pEnemy, pVtx[3].pos, pVtx[0].pos);

			// 頂点座標の設定
			switch (pEnemy->type)
			{
			case SPLITBALL_FIRST:	// 別れる球の最初
			case SPLITBALL_SECOND:	// 別れる球の２回目
			case SPLITBALL_LAST:	// 別れる球の最後

				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);

				break;
			case EXTENDBALL_UP:		// 伸びる円、上から下

				SetRectUpRightPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case EXTENDBALL_DWON:	// 伸びる円、下から上
				break;
			case EXTENDBALL_LEFT:	// 伸びる円、左から右
				break;
			case EXTENDBALL_RIGHT:	// 伸びる円、右から左
				break;
			case GOSTRAIGHT_UP:		// 直進する長方形、上から下
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case GOSTRAIGHT_DWON:	// 直進する長方形、下から上
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case GOSTRAIGHT_LEFT:	// 直進する長方形、左から右
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case GOSTRAIGHT_RIGHT:	// 直進する長方形、右から左
				SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
				break;
			case DAMEGE_WALL:		// ダメージ壁
				break;
			default:
				assert(false);	// 本来通らない場所
				break;
			}

			// カラーの設定
			SetRectColor(pVtx, &(pEnemy->col));

		}
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
// 敵の待機処理
//====================================
static void NeutralEnemy(Enemy *pEnemy)
{
	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// 別れる球の最初
	case SPLITBALL_SECOND:	// 別れる球の２回目
	case SPLITBALL_LAST:	// 別れる球の最後
		if (pEnemy->nAtkInterval >= 150)
		{
			float fRotDest;
			Player *pPlayer = GetPlayer();	// プレイヤーのポインタ

			// 少しだけプレイヤーの反対方向にmove
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = sinf(fRotDest) * 0.5f;
			pEnemy->move.y = cosf(fRotDest) * 0.5f;

			pEnemy->col = D3DXCOLOR(0.9f, 0.35f, 0.4f, 1.0f);
		}
		break;
	case EXTENDBALL_UP:		// 伸びる円、上から下
		break;
	case EXTENDBALL_DWON:	// 伸びる円、下から上
		break;
	case EXTENDBALL_LEFT:	// 伸びる円、左から右
		break;
	case EXTENDBALL_RIGHT:	// 伸びる円、右から左
		break;
	case GOSTRAIGHT_UP:		// 直進する長方形、上から下
		break;
	case GOSTRAIGHT_DWON:	// 直進する長方形、下から上
		break;
	case GOSTRAIGHT_LEFT:	// 直進する長方形、左から右
		break;
	case GOSTRAIGHT_RIGHT:	// 直進する長方形、右から左
		break;
	case DAMEGE_WALL:		// ダメージ壁
		break;
	default:
		assert(false);		// 本来通らない場所
		break;
	}
}

//====================================
// 敵の攻撃処理
//====================================
static void AttackEnemy(Enemy *pEnemy)
{
	Player *pPlayer = GetPlayer();	// プレイヤーのポインタ
	float fRotDest;					// 敵とプレイヤーを結ぶ線の角度

	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// 別れる球の最初
	case SPLITBALL_SECOND:	// 別れる球の２回目
	case SPLITBALL_LAST:	// 別れる球の最後
		if (pEnemy->nAtkInterval <= 150 && pEnemy->nAtkInterval % 5 == 0)
		{
			// パーティクル
			SetParticle(pEnemy->pos, PARTICLE_SPLITBALL_ATTACK);

		}
		if (pEnemy->nAtkInterval >= 300)
		{
			// 突進攻撃
			fRotDest = (float)atan2(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
			pEnemy->move.x = sinf(fRotDest) * 20.0f;
			pEnemy->move.y = cosf(fRotDest) * 20.0f;

			pEnemy->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			// 攻撃サイクルのリセット
			pEnemy->nAtkInterval = 0;
		}
		break;
	case EXTENDBALL_UP:		// 伸びる円、上から下
		pEnemy->fHeight += 1.5f;
		break;
	case EXTENDBALL_DWON:	// 伸びる円、下から上
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fHeight += 5.0f;
		}
		break;
	case EXTENDBALL_LEFT:	// 伸びる円、左から右
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fWidth += 5.0f;
		}
		break;
	case EXTENDBALL_RIGHT:	// 伸びる円、右から左
		if (pEnemy->nAtkInterval >= 150)
		{
			pEnemy->fWidth += 5.0f;
		}
		break;
	case GOSTRAIGHT_UP:		// 直進する長方形、上から下
		pEnemy->move.y = 5.0f;
		break;
	case GOSTRAIGHT_DWON:	// 直進する長方形、下から上
		pEnemy->move.y = -5.0f;
		break;
	case GOSTRAIGHT_LEFT:	// 直進する長方形、左から右
		pEnemy->move.x = 5.0f;
		break;
	case GOSTRAIGHT_RIGHT:	// 直進する長方形、右から左
		pEnemy->move.x = -5.0f;
		break;
	case DAMEGE_WALL:		// ダメージ壁
		break;
	default:
		assert(false);		// 本来通らない場所
		break;
	}

}

//====================================
// 敵の死亡処理
//====================================
static void DieEnemy(Enemy *pEnemy)
{
//	assert(sizeof(score)/sizeof(score[0]) != ENEMYTYPE_MAX);
	AddScore(score[pEnemy->type]);

	switch (pEnemy->type)
	{
	case SPLITBALL_FIRST:	// 別れる球の最初
		SetEnemy(pEnemy->pos, SPLITBALL_SECOND);
		SetEnemy(pEnemy->pos, SPLITBALL_SECOND);
		AddScore(100);
		pEnemy->bUse = false;
		break;
	case SPLITBALL_SECOND:	// 別れる球の２回目
		SetEnemy(pEnemy->pos, SPLITBALL_LAST);
		SetEnemy(pEnemy->pos, SPLITBALL_LAST);
		AddScore(100);
		pEnemy->bUse = false;
		break;
	case SPLITBALL_LAST:	// 別れる球の最後
		AddScore(100);
		pEnemy->bUse = false;
		break;	// 消す処理
	case EXTENDBALL_UP:		// 伸びる円、上から下
		AddScore(50);
		pEnemy->bUse = false;
		break;
	case EXTENDBALL_DWON:	// 伸びる円、下から上
		pEnemy->bUse = false;
		break;
	case EXTENDBALL_LEFT:	// 伸びる円、左から右
		pEnemy->bUse = false;
		break;
	case EXTENDBALL_RIGHT:	// 伸びる円、右から左
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_UP:		// 直進する長方形、上から下
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_DWON:	// 直進する長方形、下から上
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_LEFT:	// 直進する長方形、左から右
		pEnemy->bUse = false;
		break;
	case GOSTRAIGHT_RIGHT:	// 直進する長方形、右から左
		pEnemy->bUse = false;
		break;
	case DAMEGE_WALL:		// ダメージ壁
		break;
	default:
		assert(false);	// 本来通らない場所
		break;
	}
}

//====================================
// 敵の終了処理
//====================================
void DeleteEnemy(void)
{
}

//====================================
// 敵の描画処理
//====================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	Enemy *pEnemy;				// 敵のポインタ
	int nCntEnemy;

	// デバイスの取得
	pDevice = GetDevice();

	InitDraw(pDevice, s_pVtxBuff);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);
		if (pEnemy->bUse == true)
		{
			//テクスチャの描画
			SetDraw(pDevice, s_pTexture[pEnemy->type], nCntEnemy * 4);
		}
	}
}

//====================================
// 敵の設定処理
//====================================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	Enemy *pEnemy;	// 敵のポインタ
	int nCntEnemy;
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);

		if (pEnemy->bUse == false)
		{// 敵が使用されていない場合

			// 位置設定
			pEnemy->pos = pos;

			// 種類設定
			pEnemy->type = type;

			// 攻撃間隔の設定
			pEnemy->nAtkInterval = 0;

			switch (pEnemy->type)
			{
			case SPLITBALL_FIRST:	// 別れる球の最初
				// エネミーの待機動作
				NeutralEnemy(pEnemy);
				// 寿命設定
				pEnemy->nLife = 1;
				// 画像の大きさ設定
				pEnemy->fHeight	= 60.0f;	// 高さ
				pEnemy->fWidth	= 60.0f;	// 幅
				break;
			case SPLITBALL_SECOND:	// 別れる球の2回目
				// エネミーの待機動作
				NeutralEnemy(pEnemy);
				// 位置を少し散らす
				pEnemy->pos.x = pos.x + (float)(rand() % 50);
				pEnemy->pos.y = pos.y + -(float)(rand() % 50);
				// 寿命設定
				pEnemy->nLife = 1;
				// 画像の大きさ設定
				pEnemy->fHeight	= 50.0f;	// 高さ
				pEnemy->fWidth	= 50.0f;		// 幅
				break;
			case SPLITBALL_LAST:	// 別れる球の最後
				// エネミーの待機動作
				NeutralEnemy(pEnemy);
				// 位置を少し散らす
				pEnemy->pos.x = pos.x + (float)(rand() % 20);
				pEnemy->pos.y = pos.y + -(float)(rand() % 20);
				// 寿命設定
				pEnemy->nLife = 1;
				// 画像の大きさ設定
				pEnemy->fHeight	= 40.0f;	// 高さ
				pEnemy->fWidth	= 40.0f;	// 幅
				break;
			case EXTENDBALL_UP:		// 伸びる円、上から下
			case EXTENDBALL_DWON:	// 伸びる円、下から上
			case EXTENDBALL_LEFT:	// 伸びる円、左から右
			case EXTENDBALL_RIGHT:	// 伸びる円、右から左
				// 寿命設定
				pEnemy->nLife = 1;
				// 画像の大きさ設定
				pEnemy->fHeight	= 10.0f;	// 高さ
				pEnemy->fWidth	= 10.0f;		// 幅
				break;
			case GOSTRAIGHT_UP:		// 直進する長方形、上から下
			case GOSTRAIGHT_DWON:	// 直進する長方形、下から上
				// 寿命設定
				pEnemy->nLife = 1;
				// 画像の大きさ設定
				pEnemy->fHeight	= 10.0f;	// 高さ
				pEnemy->fWidth	= 5.0f;		// 幅
				break;
			case GOSTRAIGHT_LEFT:	// 直進する長方形、左から右
			case GOSTRAIGHT_RIGHT:	// 直進する長方形、右から左
				// 寿命設定
				pEnemy->nLife = 1;
				// 画像の大きさ設定
				pEnemy->fHeight	= 5.0f;		// 高さ
				pEnemy->fWidth	= 10.0f;	// 幅
				break;
			case DAMEGE_WALL:		// ダメージ壁
				break;
			default:
				assert(false);		// 本来通らない場所
				break;
			}

			// 頂点座標の設定
			SetRectCenterPos(pVtx, pEnemy->pos, pEnemy->fWidth, pEnemy->fHeight);
			
			// 使用している状態にする
			pEnemy->bUse = true;

			// 敵の数をプラス
			s_nEnemyCnt++;

			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//====================================
// 敵の当たり処理
//====================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	Enemy *pEnemy = &(s_aEnemy[nCntEnemy]);		// 敵のポインタ

	pEnemy->nLife -= nDamage;

	if (pEnemy->nLife <= 0)
	{// HPが0になったら
	}

	// 敵の数を数えなおす
	s_nEnemyCnt = 0;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			s_nEnemyCnt++;
		}
	}
}

//====================================
// 敵の取得処理
//====================================
Enemy *GetEnemy(void)
{
	// 返り値
	return s_aEnemy;
}

//====================================
// 敵の数取得処理
//====================================
int GetCntEnemy(void)
{
	return s_nEnemyCnt;
}

//====================================
// ブロックの当たり判定処理
//====================================
bool CollisionEnemy(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	bool bisLanding = false;
	// 当たり判定処理
	Enemy *pEnemy;

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

							// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_BLOCK; nCntEnemy++)
	{
		pEnemy = &(s_aEnemy[nCntEnemy]);
		if (pEnemy->bUse == true)
		{// ブロックが生きてたら

			 // 上
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_UP, *pEnemy))
			{
				HitPlayer(1);
			}

			// 下
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_DWON, *pEnemy))
			{// ブロックの座標と座標が重なり合ったら
				HitPlayer(1);
			}

			// 左
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_LEFT, *pEnemy))
			{// ブロックの座標と座標が重なり合ったら
				HitPlayer(1);
			}

			// 右
			if (CrossingEnemy(&(pos1), &(pos2), POSITION_RIGHT, *pEnemy))
			{// ブロックの座標と座標が重なり合ったら
				HitPlayer(1);
			}
		}
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	return bisLanding;
}

//====================================
// 手kのの線分の交差判定処理
//====================================
bool CrossingEnemy(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos2, JUDGE_POSITION position, Enemy enemy)
{
	// 当たり判定処理
	Enemy *pEnemy = &(enemy);
	Segment vEnemy;
	Segment vTarget;

	switch (position)
	{
	case POSITION_UP:
		// ブロックのベクトルの獲得
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_DWON:
		// ブロックのベクトルの獲得
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_LEFT:
		// ブロックのベクトルの獲得
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x - pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	case POSITION_RIGHT:
		// ブロックのベクトルの獲得
		vEnemy.start  = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y - pEnemy->fHeight, pEnemy->pos.z);
		vEnemy.vector = D3DXVECTOR3(pEnemy->pos.x + pEnemy->fWidth, pEnemy->pos.y + pEnemy->fHeight, pEnemy->pos.z) - vEnemy.start;
		break;
	default:
		assert(false);
		break;
	}

	// 被対象のベクトルの獲得
	vTarget.start  = *pPos2;
	vTarget.vector = *pPos1 - *pPos2;

	// ベクトルの始点同士の距離。
	D3DXVECTOR3 v = vTarget.start - vEnemy.start;

	// ブロックのベクトルと被対象のベクトルが平行か調べる
	float Bv_Tv = D3DXVec2Cross(&(vEnemy.vector), &(vTarget.vector));
	if (Bv_Tv == 0.0f)
	{
		// 並行である。
		return false;
	}

	float v_Bv = D3DXVec2Cross(&(v), &(vEnemy.vector));
	float v_Tv = D3DXVec2Cross(&(v), &(vTarget.vector));

	float hit1 = v_Tv / Bv_Tv;
	float hit2 = v_Bv / Bv_Tv;

	float eps = 0.000001f;	// 当たり判定を緩める

	return !((hit1 + eps < 0.0f) || (hit1 - eps > 1.0f) || (hit2 + eps < 0.0f) || (hit2 - eps > 1.0f));
}