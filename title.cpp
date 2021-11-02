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
#include "score.h"

#include "time.h"

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
	LPDIRECT3DTEXTURE9 tex;
	LPDIRECT3DVERTEXBUFFER9 VtxBuff;
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	bool bUse;			// 使用してるかどうか
}OBJECT;

// グローバル変数
static OBJECT SelectBg;
static OBJECT ScreenBg;
static OBJECT Title;
static OBJECT GameStart;
static OBJECT Tutorial;
static OBJECT Exit;
static LPDIRECT3DTEXTURE9 s_pTexture[OBJ_MAX] = {};		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};			// 頂点バッファへのポインタ
//static OBJECT s_Tex[MAX_TEXTURE] = {};
static PRESS_TYPE s_presstype;
static int s_nTypeCnt;		// 縮小と拡大の間隔
static bool s_bFadeCheek;	// フェード処置に移行するかの処理
static bool s_bPreassUse;
static int s_nFadeCnt;		// フェード処理に行くまでの間隔
static int s_nTimeTitle;

//=========================================
// 初期化処理
//=========================================
void InitTitle(void)
{
	VERTEX_2D *pVtx;							// 頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント
	int nTexCnt;

	// 音楽の再生
	PlaySound(SOUND_LABEL_BGM000);

	/*							*/
	/*	スクリーン背景の初期化	*/
	/*							*/
	// 変数の初期化
	SelectBg.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SelectBg.col = D3DXCOLOR(1.0f, 0.9f, 0.8f, 1.0f);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice, NULL, &SelectBg.tex);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&SelectBg.VtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	SelectBg.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetRectUpLeftPos(pVtx, SelectBg.pos, SCREEN_WIDTH, SCREEN_HEIGHT);

	// 頂点カラーの設定
	SetRectColor(pVtx, &(SelectBg.col));

	// rhwの設定
	InitRectRhw(pVtx);

	// テクスチャ座標の設定
	InitRectTex(pVtx);

	// 頂点バッファをアンロックする
	SelectBg.VtxBuff->Unlock();


	/*							*/
	/*	選択画面背景の初期化	*/
	/*							*/
	// 変数の初期化
	SelectBg.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SelectBg.col = D3DXCOLOR(1.0f, 0.9f, 0.8f, 1.0f);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,SELECTBG,&ScreenBg.tex);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&ScreenBg.VtxBuff,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	SelectBg.VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	SetRectUpLeftPos(pVtx, SelectBg.pos, SCREEN_WIDTH, SCREEN_HEIGHT);

	// 頂点カラーの設定
	SetRectColor(pVtx, &(SelectBg.col));

	// rhwの設定
	InitRectRhw(pVtx);

	// テクスチャ座標の設定
	InitRectTex(pVtx);

	// 頂点バッファをアンロックする
	SelectBg.VtxBuff->Unlock();

	/*							*/
	/*	選択画面背景の初期化	*/
	/*							*/
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,TITLE,&Title.tex);

	/*							*/
	/*	選択画面背景の初期化	*/
	/*							*/
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,GAMESTART,&GameStart.tex);

	/*							*/
	/*	選択画面背景の初期化	*/
	/*							*/
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,TUTORIAL,&Tutorial.tex);

	/*							*/
	/*	選択画面背景の初期化	*/
	/*							*/
	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,EXIT,&Exit.tex);

	// プレスタイトルの初期化処理
	s_presstype = TYPE_NOME;						// 点滅状態
	s_nTypeCnt = 0;									// 拡大と収縮のタイミング

	s_bFadeCheek = false;							// フェード処理に移行するかの変数
	s_nFadeCnt = 0;									// フェード処理に移行するまでの間隔
	s_nTimeTitle = 0;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * OBJ_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < OBJ_MAX; i++)
	{

		switch (i)
		{
		case OBJ_BG:
			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	
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

	InitTime();
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
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// タイトルの画像
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	// ゲームモードに移行
	if (!(s_bFadeCheek))
	{
		if (GetJoypadTrigger(JOYKEY_A) || GetKeyboardTrigger(DIK_RETURN))
		{// EnterキーかパッドのAを押された時
		 // 決定音の再生
			PlaySound(SOUND_LABEL_SE_ENTER);
			s_bFadeCheek = true;	// フェード処理に入る
		}
	}
	else if (s_bFadeCheek)
	{
		s_presstype = TYPE_BLINK;
		s_nFadeCnt = 0;
		SetFade(MODE_GAME);	// ゲームモードに移行
	}
}

//=========================================
// 描画処理
//=========================================
void DrawTitle(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 背景の描写
	// 頂点バッファをデータストリーム設定
	InitDraw(pDevice, s_pVtxBuff);

	for (int i = 0; i < OBJ_MAX; i++)
	{
		if (s_Tex[i].bUse == true)
		{
			// ポリゴン描画
			// テクスチャの設定
			SetDraw(pDevice, s_pTexture[i], i * 4);
		}
	}

}