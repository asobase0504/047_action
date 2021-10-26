//=========================================
//
//パーティクルの処理
//Author YudaKaito
//
//=========================================

//-----------------------------------------
//include
//-----------------------------------------
#include "main.h"
#include "setup.h"
#include "particle.h"
#include <assert.h>
#include "enemy.h"

//-----------------------------------------
//マクロ定義
//-----------------------------------------
#define MAX_PARTCLE	(1024)	//パーティクルの最大数
#define MAX_TEX		(MAX_PARTICLE)		//テクスチャの種類

//-----------------------------------------
//パーティクル構造体
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//色
	float fRaduus;		//半径
	int nMaxLife;		//最大寿命
	int nLife;			//寿命
	PARTICLE_TYPE type;	//パーティクルの種類
	bool bUse;			//使用しているかどうかe
}Particle;

//-----------------------------------------
//static変数
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_TEX] = {};		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	//頂点バッファへのポインタ
static Particle s_aParticle[MAX_PARTCLE];
static float s_fAngle;

//-----------------------------------------
//プロトコール
//-----------------------------------------
#if 0
float angle = rand() / (float)RAND_MAX * 2.0f * D3DX_PI;
float length = 50.0f + rand() / (float)RAND_MAX * 50.0f;
float diffX = sinf(angle) * length;
float diffY = cosf(angle) * length;
float speed = ? ? ? ? ;
float life = length / speed;

pParticle->pos.x = pos.x + diffX;
pParticle->pos.y = pos.y + diffY;
pParticle->move.x = sinf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.y = cosf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.z = 0.0f;
pParticle->col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
pParticle->fRaduus = 10.0f;
pParticle->nLife = (rand() % 20) + 10;
#endif

//=========================================
//パーティクルの初期化処理
//=========================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[0]);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[1]);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[2]);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_SPLITBALL_ATTACK]);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		s_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].fRaduus = 0.0f;
		s_aParticle[nCntParticle].nLife = 0;
		s_aParticle[nCntParticle].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	VERTEX_2D* vtxBase = pVtx;

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		assert(pVtx + 3 < vtxBase + (4 * MAX_PARTCLE));

		pParticle = &(s_aParticle[nCntParticle]);

		//頂点座標の設定
		SetRectPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		//rhwの設定
		//テクスチャ座標の設定
		//頂点カラーの設定
		SetupRectDefault(pVtx, &(pParticle->col));

		SetupRectDefault(pVtx, &(s_aParticle[nCntParticle].col));

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
//パーティクルの終了処理
//=========================================
void UninitParticle(void)
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		//テクスチャの破棄
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//=========================================
//パーティクルの更新処理
//=========================================
void UpdateParticle(void)
{
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = s_aParticle + nCntParticle;

		if (!pParticle->bUse)
		{	//パーティクルが使用されていない
			pVtx += 4;
			continue;
		}

		//パーティクルが使用されている
		//位置の更新
		pParticle->pos += pParticle->move;

		////体力の更新
		//pParticle->nLife--;
		//if (pParticle->nLife <= 0)
		//{
		//	pParticle->bUse = false;
		//}

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_JUMP:	//プレイヤーのジャンプパーティクル	//列挙型に変更する。
			//体力の更新
			pParticle->nLife--;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			//透明度の更新
			pParticle->col.a -= (float)1 / pParticle->nMaxLife;
			break;
		case PARTICLE_PLAYER_DEATH:
		{
			Player *pPlayer = GetPlayer();
			
			//復活時の処理
			if (pPlayer->state == PLAYERSTATE_REVIVAL)
			{
				if(pParticle->nLife == pParticle->nMaxLife)
				{	//復活時の最初にだけ通る処理
					
					//D3DXVECTOR3 v = pPlayer->pos - pParticle->pos;	//プレイヤーの死亡位置と、各破片の位置の差を求める
					D3DXVECTOR3 v = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f) - pParticle->pos;	//プレイヤーの死亡位置と、各破片の位置の差を求める
					pParticle->move.x = v.x / pParticle->nMaxLife;	//nLifeが尽きた時に中央に集めるようにする
					pParticle->move.y = v.y / pParticle->nMaxLife;	//nLifeが尽きた時に中央に集めるようにする
				}
				pParticle->nLife--;
				if (pParticle->nLife < 0)
				{
					pPlayer->RevivalInterval--;
					pParticle->bUse = false;
				}
			}
			else
			{
				//移動量を更新(減衰)
				pParticle->move.x += (0 - pParticle->move.x) * 0.015f;
				pParticle->move.y += (0 - pParticle->move.y) * 0.015f;
			}
		}
			break;
		case PARTICLE_SPLITBALL_ATTACK:
			pParticle->fRaduus--;
			if (pParticle->fRaduus <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		default:
			break;
		}

		pVtx[0].pos.x = pParticle->pos.x - pParticle->fRaduus;
		pVtx[0].pos.y = pParticle->pos.y - pParticle->fRaduus;
		pVtx[0].pos.z = pParticle->pos.z + 0.0f;

		pVtx[1].pos.x = pParticle->pos.x + pParticle->fRaduus;
		pVtx[1].pos.y = pParticle->pos.y - pParticle->fRaduus;
		pVtx[1].pos.z = pParticle->pos.z + 0.0f;

		pVtx[2].pos.x =pParticle->pos.x - pParticle->fRaduus;
		pVtx[2].pos.y =pParticle->pos.y + pParticle->fRaduus;
		pVtx[2].pos.z =pParticle->pos.z + 0.0f;

		pVtx[3].pos.x = pParticle->pos.x + pParticle->fRaduus;
		pVtx[3].pos.y = pParticle->pos.y + pParticle->fRaduus;
		pVtx[3].pos.z = pParticle->pos.z + 0.0f;

		pVtx[0].col = pParticle->col;
		pVtx[1].col = pParticle->col;
		pVtx[2].col = pParticle->col;
		pVtx[3].col = pParticle->col;

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
//パーティクルの描画処理
//=========================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポイント
	Particle *pParticle;

	//描画のための設定
	pDevice = InitDraw(pDevice, s_pVtxBuff);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (s_aParticle[nCntParticle].bUse == true)
		{//パーティクルが使用されている
			switch (pParticle->type)
			{
			case PARTICLE_PLAYER_JUMP:	//プレイヤーのジャンプ時
				//テクスチャを加算合成で貼り付けて描画する
				pDevice = AddSyntheticDraw(pDevice, s_pTexture[pParticle->type], nCntParticle);
				break;
			case PARTICLE_SPLITBALL_ATTACK:	//別れる球の攻撃時
				//テクスチャを貼り付けて描画する
				pDevice = SetDraw(pDevice, s_pTexture[pParticle->type], nCntParticle);
				break;
			case PARTICLE_PLAYER_DEATH:
				pDevice = SetDraw(pDevice, s_pTexture[pParticle->type], nCntParticle);
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//=========================================
//パーティクルの設定処理
//=========================================
void SetParticle(D3DXVECTOR3 pos, PARTICLE_TYPE type)
{
	Particle *pParticle;
	VERTEX_2D *pVtx;
	int nCntParticle;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (pParticle->bUse)
		{//パーティクルが使用されてる
			pVtx += 4;
			continue;
		}

		//パーティクルが使用されていない
		pParticle->	type = type;
		switch (type)
		{
		case PARTICLE_PLAYER_JUMP:	//プレイヤーのジャンプパーティクル
			pParticle->pos = pos;
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.1f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.1f);
			pParticle->move.z = 0.0f;
			pParticle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pParticle->fRaduus = 10.0f;
			pParticle->nMaxLife = 50;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_SPLITBALL_ATTACK:	//別れる球の攻撃パーティクル
			pParticle->pos = pos;
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->fRaduus = 40.0f;
			pParticle->nMaxLife = 50;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_PLAYER_DEATH:
			pParticle->pos = pos;
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 5.0f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 5.0f);
			pParticle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pParticle->fRaduus = 7.0f;
			pParticle->nMaxLife = 75;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		default:
			assert(false);
			break;
		}

		pParticle->bUse = true;

		SetRectPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		pVtx[0].col = pParticle->col;
		pVtx[1].col = pParticle->col;
		pVtx[2].col = pParticle->col;
		pVtx[3].col = pParticle->col;

		pVtx += 4;
		break;
	}
	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}
