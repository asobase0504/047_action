//=========================================
//
//プレイヤーの処理
//Author YudaKaito
//
//=========================================

//-----------------------------------------
//include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "setup.h"
#include <assert.h>
#include "player.h"
#include "particle.h"
#include "effect.h"
#include "block.h"

//-----------------------------------------
//スタティック変数
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	//頂点バッファへのポインタ
static Player s_player;
static D3DXVECTOR3 OldPos[4];

//=========================================
//プレイヤーの初期化処理
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポイント
	Player *pPlayer;

	pPlayer = &(s_player);
	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEX,
		&s_pTexture);

	pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT-50.0f, 0.0f);	//位置を初期化
	pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//移動量を初期化
	pPlayer->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向きを初期化
	pPlayer->nLife = 3;												//体力の
	pPlayer->fMove = PLAYER_MOVE;									//移動速度
	pPlayer->fWidth = PLAYER_WIDTH;									//プレイヤーの幅
	pPlayer->fHeigth = PLAYER_HEIGTH;								//プレイヤーの高さ
	pPlayer->bUse = true;											//プレイヤーの表示の有無
	pPlayer->state = PLAYERSTATE_APPEAR;							//プレイヤーのステータス
	pPlayer->RevivalInterval = 0;									//復活のインターバル

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//中心座標から上の長さを算出する。
	pPlayer->fLength = sqrtf(pPlayer->fWidth  * pPlayer->fWidth + pPlayer->fHeigth * pPlayer->fHeigth);

	//中心座標から上の頂点の角度を算出する
	pPlayer->fAngle = atan2f(pPlayer->fWidth, pPlayer->fHeigth);


	//頂点座標の設定
	pVtx[0].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[0].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[1].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z - (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[2].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z - (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[3].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI / 2.0f)) * pPlayer->fWidth / 2.0f;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	//頂点カラーの設定
	//テクスチャ座標の設定
	SetupRectDefault(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
//プレイヤーの終了処理
//=========================================
void UninitPlayer(void)
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

//=========================================
//プレイヤーの更新処理
//=========================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//int nAnimationTime;
	Player *pPlayer;
	pPlayer = &(s_player);

	switch (pPlayer->state)
	{
	case PLAYERSTATE_APPEAR:	//プレイヤーが出現中
		pPlayer->state = PLAYERSTATE_NORMAL;
		pPlayer->bUse = true;
		pPlayer->move.y = 0.0f;
		pPlayer->rot.z = 0.0f;
		pPlayer->pos = D3DXVECTOR3(600.0f, SCREEN_HEIGHT - 50.0f, 0.0f);	//位置を初期化
		break;
	case PLAYERSTATE_NORMAL:	//プレイヤーが活動中
		//重力の加算
		pPlayer->move.y += WOARD_GRAVITY;

		//前回の座標を更新
		pPlayer->posOld = pPlayer->pos;

		//前回の回転を更新
		pPlayer->rotOld = pPlayer->rot;

		//移動処理
		MovePlayer();

		//ジャンプ処理
		if (pPlayer->nJumpCnt < PLAYER_JUMPMAX)
		{//ジャンプ回数
			if (GetKeyboardTrigger(DIK_SPACE))
			{
				pPlayer->jumpstate = JUMP_NOW;
				pPlayer->nJumpCnt++;
				//			pPlayer->pos.y -= 12.0f;
				pPlayer->move.y = 0.0f;
				pPlayer->move.y += -PLAYER_JUMPMOVE;

				for (int i = 0; i < 40; i++)
				{
					SetParticle(pPlayer->pos, PARTICLE_PLAYER_JUMP);
				}

			}
		}

		//回転処理
		if (GetKeyboardPress(DIK_Q))
		{
			pPlayer->rot.z += 0.1f;

			//中心座標の移行
			if (pPlayer->rot.z >= 0.0f && pPlayer->Centerpos == PLAYER_POS_RIGHT)
			{
				pPlayer->pos.x = pPlayer->pos.x - sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->pos.y = pPlayer->pos.y - cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->Centerpos = PLAYER_POS_LEFT;
			}
		}
		if (GetKeyboardPress(DIK_E))
		{
			pPlayer->rot.z -= 0.1f;

			//中心座標の移行
			if (pPlayer->rot.z <= 0.0f && pPlayer->Centerpos == PLAYER_POS_LEFT)
			{
				pPlayer->pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
				pPlayer->Centerpos = PLAYER_POS_RIGHT;
			}
		}

		//位置を更新
		pPlayer->pos.x += pPlayer->move.x;
		pPlayer->pos.y += pPlayer->move.y;

		//移動量を更新(減衰)
		pPlayer->move.x += (0 - pPlayer->move.x) * 0.025f;

		//回転して床に辺が面したとき
		if (-D3DX_PI / 2 >= pPlayer->rot.z)
		{
			pPlayer->rot.z = 0;

			pPlayer->pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.z = 0.0f;

		}
		else if (D3DX_PI / 2 <= pPlayer->rot.z)
		{
			pPlayer->rot.z = 0;

			pPlayer->pos.x = pPlayer->pos.x - sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.y = pPlayer->pos.y - cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
			pPlayer->pos.z = 0.0f;

		}

		//マップ端にいった場合反対のマップ端に出る
		if (pPlayer->pos.x - pPlayer->fWidth >= SCREEN_WIDTH)
		{
			pPlayer->pos.x = -pPlayer->fWidth;
		}
		else if (pPlayer->pos.x + pPlayer->fWidth <= 0)
		{
			pPlayer->pos.x = SCREEN_WIDTH + pPlayer->fWidth;
		}

		//床に着いたらジャンプ制限のリセット
		if (pPlayer->pos.y > SCREEN_HEIGHT)
		{
			pPlayer->nJumpCnt = 0;
			pPlayer->jumpstate = JUMP_NONE;
			pPlayer->move.y = 0.0f;
			pPlayer->pos.y = SCREEN_HEIGHT;
		}
		break;
	case PLAYERSTATE_DEATH:	//プレイヤーが死んだ場合
		pPlayer->bUse = false;

		if (GetKeyboardPress(DIK_R))
		{
			pPlayer->state = PLAYERSTATE_REVIVAL;
		}
		break;
	case PLAYERSTATE_REVIVAL:
		if (pPlayer->RevivalInterval <= 0)
		{
			pPlayer->state = PLAYERSTATE_APPEAR;
		}
		break;
	default:
		break;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	RectPlayer(pVtx,pPlayer->Centerpos);

	bool bisLanding;

	//それぞれの頂点座標の当たり判定
	bisLanding = CollisionBlock(pPlayer, pVtx[0].pos, pVtx[1].pos);
	bisLanding = CollisionBlock(pPlayer, pVtx[1].pos, pVtx[2].pos);
	bisLanding = CollisionBlock(pPlayer, pVtx[2].pos, pVtx[3].pos);
	bisLanding = CollisionBlock(pPlayer, pVtx[3].pos, pVtx[0].pos);

	//敵との当たり判定
	CollisionEnemy(pPlayer, pVtx[0].pos, pVtx[1].pos);
	CollisionEnemy(pPlayer, pVtx[1].pos, pVtx[2].pos);
	CollisionEnemy(pPlayer, pVtx[2].pos, pVtx[3].pos);
	CollisionEnemy(pPlayer, pVtx[3].pos, pVtx[0].pos);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
//プレイヤーの描画処理
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポイント
	Player *pPlayer = GetPlayer();

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (pPlayer->bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=========================================
//プレイヤーの移動処理
//=========================================
void MovePlayer(void)
{
	Player *pPlayer;
	pPlayer = &(s_player);

	//キーボードの移動処理
	switch (pPlayer->jumpstate)
	{
	case JUMP_NOW:
		if (GetJoypadPress(JOYKEY_LEFT) || GetKeyboardPress(DIK_A))
		{//Aキーが押された
			pPlayer->move.x += sinf(D3DX_PI * -0.5f) * (pPlayer->fMove * 0.5f);
		}
		else if (GetJoypadPress(JOYKEY_RIGHT) || GetKeyboardPress(DIK_D))
		{//Dキーが押された
			pPlayer->move.x += sinf(D3DX_PI * 0.5f) * (pPlayer->fMove * 0.5f);
		}
		break;
	case JUMP_NONE:
		if (GetJoypadPress(JOYKEY_LEFT) || GetKeyboardPress(DIK_A))
		{//Aキーが押された
			pPlayer->move.x += sinf(D3DX_PI * -0.5f) * pPlayer->fMove;
		}
		else if (GetJoypadPress(JOYKEY_RIGHT) || GetKeyboardPress(DIK_D))
		{//Dキーが押された
			pPlayer->move.x += sinf(D3DX_PI * 0.5f) * pPlayer->fMove;
		}
		break;
	default:
		assert(false);
		break;
	}
}

//=========================================
//プレイヤーのダメージ処理
//=========================================
void HitPlayer(int nDamege)
{
	Player *pPlayer = &(s_player);
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{
		for (int i = 0; i < 32; i++)
		{
			pPlayer->RevivalInterval++;
			SetParticle(pPlayer->pos, PARTICLE_PLAYER_DEATH);
		}
		pPlayer->state = PLAYERSTATE_DEATH;
		pPlayer->nLife--;
	}
}

//=========================================
//プレイヤーの頂点座標の設定
//=========================================
void RectPlayer(VERTEX_2D *pVtx, PLAYER_CENTERPOS Centerpos)
{
	Player *pPlayer = &(s_player);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (Centerpos)
	{
	case PLAYER_POS_RIGHT:
		//頂点座標の設定
		pVtx[0].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[0].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (-D3DX_PI + pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[1].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z - D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[2].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z - D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pPlayer->pos.x;
		pVtx[3].pos.y = pPlayer->pos.y;
		pVtx[3].pos.z = 0.0f;

		break;
	case PLAYER_POS_LEFT:
		//頂点座標の設定
		pVtx[0].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[0].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI)) * pPlayer->fHeigth;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[1].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pPlayer->pos.x;
		pVtx[2].pos.y = pPlayer->pos.y;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pPlayer->pos.x + sinf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[3].pos.y = pPlayer->pos.y + cosf(pPlayer->rot.z + D3DX_PI / 2.0f) * pPlayer->fWidth;
		pVtx[3].pos.z = 0.0f;
		break;
	default:
		assert(false);
		break;
	}

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
//プレイヤーの取得処理
//=========================================
Player* GetPlayer(void)
{
	return &(s_player);
}
