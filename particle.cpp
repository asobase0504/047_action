//=========================================
// 
// パーティクルの処理
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "setup.h"
#include "particle.h"
#include <assert.h>
#include "enemy.h"

//-----------------------------------------
// マクロ定義
//-----------------------------------------
#define MAX_PARTCLE	(1024)	// パーティクルの最大数
#define MAX_TEX		(MAX_PARTICLE)		// テクスチャの種類
#define JUMP_TEX	"data/TEXTURE/jump.png"

//-----------------------------------------
// パーティクル構造体
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	D3DXVECTOR3 rot;	// 角度
	D3DXCOLOR col;		// 色
	float fRaduus;		// 半径
	float fLength;		// 中心座標からの半径
	float fAngle;		// 角度の算出
	int nMaxLife;		// 最大寿命
	int nLife;			// 寿命
	PARTICLE_TYPE type;	// パーティクルの種類
	bool bUse;			// 使用しているかどうかe
}Particle;

//-----------------------------------------
// static変数
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_PARTICLE] = {};		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static Particle s_aParticle[MAX_PARTCLE];
static float s_fAngle;

//-----------------------------------------
// プロトコール
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
// パーティクルの初期化処理
//=========================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		JUMP_TEX,
		&s_pTexture[PARTICLE_PLAYER_JUMP]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_SPLITBALL_ATTACK]);
	
	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING00_ATTACK]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING01_ATTACK]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING00_DIE]);

	D3DXCreateTextureFromFile(pDevice,
		SPLITBALL_TEX,
		&s_pTexture[PARTICLE_BALL_HOMING01_DIE]);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		s_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aParticle[nCntParticle].fRaduus = 0.0f;
		s_aParticle[nCntParticle].nLife = 0;
		s_aParticle[nCntParticle].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	VERTEX_2D* vtxBase = pVtx;

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		assert(pVtx + 3 < vtxBase + (4 * MAX_PARTCLE));

		pParticle = &(s_aParticle[nCntParticle]);

		// 頂点座標の設定
		SetRectCenterPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		// 頂点カラーの設定
		SetRectColor(pVtx, &(pParticle->col));

		// テクスチャ座標の設定
		InitRectTex(pVtx);
		
		// rhwの設定
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
// パーティクルの終了処理
//=========================================
void UninitParticle(void)
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		// テクスチャの破棄
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//=========================================
// パーティクルの更新処理
//=========================================
void UpdateParticle(void)
{
	Particle *pParticle;

	for (int i = 0; i < MAX_PARTCLE; i++)
	{
		pParticle = s_aParticle + i;

		if (!pParticle->bUse)
		{	// パーティクルが使用されていない
			continue;
		}

		// パーティクルが使用されている
		// 位置の更新
		pParticle->pos += pParticle->move;

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_JUMP:	// プレイヤーのジャンプパーティクル	// 列挙型に変更する。
			pParticle->nLife--;			// 体力の更新
			pParticle->col.a -= (float)1.0f / pParticle->nMaxLife;			// 透明度の更新
			pParticle->fRaduus += 1.5f;	// 半径の拡大
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		case  PARTICLE_PLAYER_WALK:		// プレイヤーの移動
			pParticle->nLife -= 1;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_PLAYER_AIR:			// プレイヤーの空中軌道
			pParticle->nLife--;
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_PLAYER_DEATH:
		{
			Player *pPlayer = GetPlayer();
			
			// 復活時の処理
			if (pPlayer->state == PLAYERSTATE_REVIVAL)
			{
				if(pParticle->nLife == pParticle->nMaxLife)
				{	// 復活時の最初にだけ通る処理
					
					// D3DXVECTOR3 v = pPlayer->pos - pParticle->pos;	// プレイヤーの死亡位置と、各破片の位置の差を求める
					D3DXVECTOR3 v = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f) - pParticle->pos;	// プレイヤーの死亡位置と、各破片の位置の差を求める
					pParticle->move.x = v.x / pParticle->nMaxLife;	// nLifeが尽きた時に中央に集めるようにする
					pParticle->move.y = v.y / pParticle->nMaxLife;	// nLifeが尽きた時に中央に集めるようにする
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
				// 移動量を更新(減衰)
				pParticle->move.x += (0 - pParticle->move.x) * 0.03f;
				pParticle->move.y += (0 - pParticle->move.y) * 0.03f;
			}
		}
			break;
		case PARTICLE_SPLITBALL_ATTACK:
			pParticle->fRaduus -= 2.0f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_BALL_HOMING00_ATTACK:	// 甘い追従をする円の攻撃 
			// 体力の更新
			pParticle->nLife--;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			// 透明度の更新
			pParticle->col.g -= (float)1 / pParticle->nMaxLife;
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING01_ATTACK:	// 追従をする円の攻撃 
			// 体力の更新
			pParticle->nLife--;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			// 透明度の更新
			pParticle->col.g -= (float)1 / pParticle->nMaxLife;
			pParticle->col.r -= (float)1 / pParticle->nMaxLife;
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			break;
		case PARTICLE_GOSTRAIGHT_DIE:	// 直進する長方形死亡時
			pParticle->fRaduus -= 0.15f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_BALL_HOMING00_DIE:		// 甘い追従をする円の死亡時
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			pParticle->fRaduus -= 0.05f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		case PARTICLE_BALL_HOMING01_DIE:		// 追従をする円の死亡時
			pParticle->col.a -= (float)0.5f / pParticle->nMaxLife;
			pParticle->fRaduus -= 0.05f;
			if (pParticle->fRaduus <= 0.0f)
			{
				pParticle->bUse = false;
			}
			break;
		default:
			break;
		}

		// 中心座標から上の長さを算出する。
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// 中心座標から上の頂点の角度を算出する
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		VERTEX_2D *pVtx;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		// 頂点座標の設定
		SetRectCenterRotPos(pVtx, pParticle->pos, pParticle->rot, pParticle->fAngle, pParticle->fLength);
		// 頂点カラーの設定
		SetRectColor(pVtx, &(pParticle->col));

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

	}
}

//=========================================
// パーティクルの描画処理
//=========================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	Particle *pParticle;

	// 描画のための設定
	InitDraw(pDevice, s_pVtxBuff);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (s_aParticle[nCntParticle].bUse == true)
		{// パーティクルが使用されている
			switch (pParticle->type)
			{
			case PARTICLE_PLAYER_JUMP:	// プレイヤーのジャンプ時
			case PARTICLE_BALL_HOMING00_ATTACK:	// 甘い追従をする円の攻撃 
			case PARTICLE_SPLITBALL_ATTACK:	// 別れる球の攻撃時
			case PARTICLE_PLAYER_AIR:			// プレイヤーの空中軌道
			case PARTICLE_PLAYER_DEATH:		// プレイヤーの死亡時
			case PARTICLE_GOSTRAIGHT_DIE:// 直進する長方形死亡時
			case PARTICLE_PLAYER_WALK:		// プレイヤーの移動
			case PARTICLE_BALL_HOMING01_ATTACK:	// 追従をする円の攻撃 
			case PARTICLE_BALL_HOMING00_DIE:		// 甘い追従をする円の死亡時
			case PARTICLE_BALL_HOMING01_DIE:		// 追従をする円の死亡時
				// テクスチャを貼り付けて描画する
				RectDraw(pDevice, s_pTexture[pParticle->type], nCntParticle * 4);
				break;
				//// テクスチャを加算合成で貼り付けて描画する
				//RectAddDraw(pDevice, s_pTexture[pParticle->type], nCntParticle * 4);
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//=========================================
// パーティクルの設定処理
//=========================================
void SetParticle(D3DXVECTOR3 pos, PARTICLE_TYPE type)
{
	Particle *pParticle;
	VERTEX_2D *pVtx;
	int nCntParticle;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (pParticle->bUse)
		{// パーティクルが使用されてる
			pVtx += 4;
			continue;
		}

		// パーティクルが使用されていない
		pParticle->	type = type;
		switch (type)
		{
		case PARTICLE_PLAYER_JUMP:	// プレイヤーのジャンプパーティクル
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->move.z = 0.0f;
			pParticle->col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
			pParticle->fRaduus = 10.0f;
			pParticle->nMaxLife = 25;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case  PARTICLE_PLAYER_WALK:		// プレイヤーの移動
			pParticle->pos = pos;
			pParticle->col = D3DXCOLOR(0.5f, 0.35f, 0.25f, 1.0f);
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->fRaduus = 3.0f;
			pParticle->nMaxLife = 25;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_PLAYER_AIR:			// プレイヤーの空中軌道
		{
			Player *pPlayer = GetPlayer();
			pParticle->pos = pos;
			pParticle->rot = pPlayer->rot;
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
			pParticle->fRaduus = 3.5f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
		}
			break;
		case PARTICLE_PLAYER_DEATH:
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 10.0f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 10.0f);
			pParticle->col = D3DXCOLOR(0.4f, 0.71f, 0.63f, 1.0f);
			pParticle->fRaduus = 7.0f;
			pParticle->nMaxLife = 40;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_SPLITBALL_ATTACK:	// 別れる球の攻撃パーティクル
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->fRaduus = 40.0f;
			pParticle->nMaxLife = 50;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING00_ATTACK:	// 甘い追従をする円の攻撃 
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 0.8f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->fRaduus = 7.5f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING01_ATTACK:	// 追従をする円の攻撃 
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.3f);
			pParticle->fRaduus = 8.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_GOSTRAIGHT_DIE:// 直進する長方形死亡時
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->fRaduus = 5.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING00_DIE:		// 甘い追従をする円の死亡時
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->fRaduus = 5.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		case PARTICLE_BALL_HOMING01_DIE:		// 追従をする円の死亡時
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			pParticle->move.x = cosf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->move.y = sinf((float)(rand() % 629 - 314) / 100) * ((float)(rand() % 10) / 10 + 0.5f);
			pParticle->fRaduus = 5.0f;
			pParticle->nMaxLife = 10;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		default:
			assert(false);
			break;
		}

		// 中心座標から上の長さを算出する。
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// 中心座標から上の頂点の角度を算出する
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		pParticle->bUse = true;

		SetRectCenterPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		SetRectColor(pVtx, &(pParticle->col));

		pVtx += 4;
		break;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}
