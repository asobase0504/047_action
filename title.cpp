//=========================================
// 
// タイトルの処理
// Author YudaKaito
// 
//=========================================
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "particle.h"
#include "ranking.h"
#include "setup.h"

#include <assert.h>

// マクロ定義
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)
#define MAX_TEXTURE			(5)

#define SELECTBG			"data/TEXTURE/enemy000.png"
#define TITLE				"data/TEXTURE/タイトル.png"
#define GAMESTART			"data/TEXTURE/GAMESTART.png"
#define TUTORIAL			"data/TEXTURE/TUTORIAL.png"
#define EXIT				"data/TEXTURE/EXIT.png"

// プレスエンターの状態の種類
typedef enum
{
	TYPE_NOME,	// 変化なし
	TYPE_BLINK,	// 点滅状態
	TYPE_MAX
}PRESS_TYPE;

// 配置するオブジェクトの種類
typedef enum
{
	OBJ_BG,
	OBJ_SELECTBG,
	OBJ_TITLE,
	OBJ_GAMESTART,
	OBJ_TUTORIAL,
	OBJ_EXIT,
	OBJ_MAX
}OBJ_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	bool bUse;			// 使用してるかどうか
}TitleObj;

// グローバル変数
static LPDIRECT3DTEXTURE9 s_pTexture[OBJ_MAX] = {};		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};			// 頂点バッファへのポインタ
static TitleObj s_Tex[MAX_TEXTURE] = {};
static PRESS_TYPE s_presstype;
static TitleObj press;
static int s_nTypeCnt;	// 縮小と拡大の間隔
static bool s_bFadeCheek;	// フェード処置に移行するかの処理
static bool s_bPreassUse;
static int s_nFadeCnt;	// フェード処理に行くまでの間隔
static int s_nTimeTitle;

typedef enum {
	LIST_0,
	LIST_1,
	LIST_2,
	LIST_3,
	NUM_LIST
} LIST;
static const int INT_LIST[NUM_LIST] = {
	1,2,3,4
};

//=========================================
// 初期化処理
//=========================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	int nTexCnt;

	// 音楽の再生
	PlaySound(SOUND_LABEL_BGM000);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&s_pTexture[OBJ_BG]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTBG,
		&s_pTexture[OBJ_SELECTBG]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		TITLE,
		&s_pTexture[OBJ_TITLE]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		GAMESTART,
		&s_pTexture[OBJ_GAMESTART]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL,
		&s_pTexture[OBJ_TUTORIAL]);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		EXIT,
		&s_pTexture[OBJ_EXIT]);

	// プレスタイトルの初期化処理
	s_presstype = TYPE_NOME;						// 点滅状態
	s_nTypeCnt = 0;									// 拡大と収縮のタイミング
	press.pos = D3DXVECTOR3(80.0f, 575.0f, 0.0f);	// 座標
	press.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラー
	press.bUse = false;								// プレスが使用している否か

	s_bFadeCheek = false;							// フェード処理に移行するかの変数
	s_nFadeCnt = 0;									// フェード処理に移行するまでの間隔
	s_nTimeTitle = 0;

	// タイトル画面の初期化処理
	for (nTexCnt = 0; nTexCnt < OBJ_MAX; nTexCnt++)
	{
		s_Tex[nTexCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Tex[nTexCnt].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		s_Tex[nTexCnt].bUse = true;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * OBJ_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < OBJ_MAX; i++)
	{

		switch (i)
		{
		case OBJ_BG:
			// 頂点座標の設定
			pVtx[0].pos.x = 0.0f;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SCREEN_WIDTH;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 0.0f;
			pVtx[2].pos.y = SCREEN_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SCREEN_WIDTH;
			pVtx[3].pos.y = SCREEN_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			s_Tex[i].bUse = true;
			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 0.9f, 0.8f, 1.0f)));
			break;
		case OBJ_SELECTBG:
			// 頂点座標の設定
			SetRectCenterPos(pVtx, D3DXVECTOR3(SCREEN_WIDTH - 350.0f, SCREEN_HEIGHT, 0.0f), 1000.0f, 1000.0f);

			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f)));
			break;
		case OBJ_TITLE:
			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(40.0f, 30.0f, 0.0f), 1380.0f, 415.0f);

			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f)));
			break;
		case OBJ_GAMESTART:
			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(1000.0f, 560.0f, 0.0f), 780.0f, 140.0f);
			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)));
			break;
		case OBJ_TUTORIAL:
			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(1000.0f, 720.0f, 0.0f), 615.0f, 140.0f);
			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)));
			break;
		case OBJ_EXIT:
			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(1000.0f, 880.0f, 0.0f), 270.0f, 140.0f);
			// 頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)));
			break;
		case OBJ_MAX:
		default:
			assert(false);
			break;
		}

		// rhwの設定
		InitRectRhw(pVtx);


		// テクスチャ座標の設定
		InitRectTex(pVtx);

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
// 終了処理
//=========================================
void UninitTitle(void)
{
	int nCnt;
	// サウンドの停止
	StopSound();

	for (nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
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
// 更新処理
//=========================================
void UpdateTitle(void)
{
	int nTexCnt;
	s_nTimeTitle++;

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	//// 
	//// タイトルの画像
	//// 
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//if (s_Tex[4].col.a >= 1.0f && s_Tex[1].col.a <= 1.0f)
	//{// タイトルの影のフェード
	//	s_Tex[1].col.a += 0.001f;
	//}
	//if (s_Tex[4].col.a <= 1.0f)
	//{// タイトルのフェード
	//	s_Tex[4].col.a += 0.005f;
	//}

	//for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	//{
	//	SetRectColor(pVtx, &(s_Tex[nTexCnt].col));
	//	// 頂点カラーの設定
	//	pVtx += 4;
	//}

	// ゲームモードに移行
	if (s_bFadeCheek == false)
	{
		if (GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN) == true)
		{// EnterキーかパッドのBを押された時
		 // 決定音の再生
			PlaySound(SOUND_LABEL_SE_ENTER);
			s_bFadeCheek = true;	// フェード処理に入る
		}

		//// 点滅処理
		//switch (s_presstype)
		//{
		//case TYPE_NOME:		// 点灯状態
		//	// 頂点カラーの設定
		//	SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		//	if (s_nTypeCnt == 50)
		//	{	// 消灯状態に移行
		//		s_presstype = TYPE_BLINK;
		//		s_nTypeCnt = 0;
		//	}
		//	break;
		//case TYPE_BLINK:	// 消灯状態
		//	// 頂点カラーの設定
		//	SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));
		//	if (s_nTypeCnt == 50)
		//	{	// 点灯状態に移行
		//		s_presstype = TYPE_NOME;
		//		s_nTypeCnt = 0;
		//	}
		//	break;
		//default:
		//	break;
		//}
	}
	else if (s_bFadeCheek == true)
	{
		// 頂点カラーの設定
		SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));

		if (s_nFadeCnt % 5 == 0)
		{
			pVtx[0].pos.x -= 7.0f;
			pVtx[0].pos.y -= 1.0f;
			pVtx[0].pos.z += 0.0f;

			pVtx[1].pos.x += 7.0f;
			pVtx[1].pos.y -= 1.0f;
			pVtx[1].pos.z += 0.0f;

			pVtx[2].pos.x -= 7.0f;
			pVtx[2].pos.y += 1.0f;
			pVtx[2].pos.z += 0.0f;

			pVtx[3].pos.x += 7.0f;
			pVtx[3].pos.y += 1.0f;
			pVtx[3].pos.z += 0.0f;
		}
		if (s_nFadeCnt == 20)
		{
			s_presstype = TYPE_BLINK;
			s_nFadeCnt = 0;
			SetFade(MODE_GAME);	// ゲームモードに移行
		}
		s_nFadeCnt++;
	}
	// 切り替えタイミング
	s_nTypeCnt++;

	//if (s_nTimeTitle >= 1500)
	//{
	//	SetFade(MODE_RANKING);
	//}
}

//=========================================
// 描画処理
//=========================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nTexCnt;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景の描写
	// 頂点バッファをデータストリーム設定
	pDevice = InitDraw(pDevice, s_pVtxBuff);

	for (nTexCnt = 0; nTexCnt < OBJ_MAX; nTexCnt++)
	{
		if (s_Tex[nTexCnt].bUse == true)
		{// 弾が使用されている
			// ポリゴン描画
			// テクスチャの設定
			SetDraw(pDevice, s_pTexture[nTexCnt], nTexCnt * 4);
		}
	}
}