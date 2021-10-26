//=========================================
//
//タイトルの処理
//Author YudaKaito
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

//マクロ定義
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)
#define MAX_TEXTURE			(5)

//プレスエンターの状態の種類
typedef enum
{
	TYPE_NOME,	//変化なし
	TYPE_BLINK,	//点滅状態
	TYPE_MAX
}PRESS_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	bool bUse;			//使用してるかどうか
}TitleObj;

//グローバル変数
static LPDIRECT3DTEXTURE9 s_pTextureTitle[MAX_TEXTURE] = {};		//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffTitle = {};				//頂点バッファへのポインタ
static TitleObj s_Tex[MAX_TEXTURE] = {};
static LPDIRECT3DTEXTURE9 s_pTextureTitlePress = NULL;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffTitlePress = NULL;		//頂点バッファへのポインタ
static PRESS_TYPE s_presstype;
static TitleObj press;
static int s_nTypeCnt;	//縮小と拡大の間隔
static bool s_bFadeCheek;	//フェード処置に移行するかの処理
static bool s_bPreassUse;
static int s_nFadeCnt;	//フェード処理に行くまでの間隔
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
//初期化処理
//=========================================
void InitTitle(void)
{
	//int sum = 0;
	//for (int i = 0; i < 5; ++i)
	//{
	//	assert(i >= 0 && i < NUM_LIST);
	//	sum += INT_LIST[i];
	//}

	//LIST hoge = LIST_3;
	//int aaa = 0;
	//switch (hoge)
	//{
	//case LIST_0:
	//	break;
	//case LIST_1:

	//	aaa = 2;

	//	break;
	//case LIST_2:

	//	aaa = 3;

	//	break;
	//default:
	//	assert(false);
	//	break;
	//}



	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポイント
	int nTexCnt;

	//デバイスの取得
	pDevice = GetDevice();

	//音楽の再生
	PlaySound(SOUND_LABEL_BGM000);

	//テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg003.jpg",
		&s_pTextureTitle[0]);

	//テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg011.png",
		&s_pTextureTitle[1]);

	//テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg012.png",
		&s_pTextureTitle[2]);

	//テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg013.png",
		&s_pTextureTitle[3]);

	//テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg014.png",
		&s_pTextureTitle[4]);

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/press_enter01.png",
		&s_pTextureTitlePress);

	//プレスタイトルの初期化処理
	s_presstype = TYPE_NOME;						//点滅状態
	s_nTypeCnt = 0;									//拡大と収縮のタイミング
	press.pos = D3DXVECTOR3(80.0f, 575.0f, 0.0f);	//座標
	press.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	//カラー
	press.bUse = false;								//プレスが使用している否か

	s_bFadeCheek = false;							//フェード処理に移行するかの変数
	s_nFadeCnt = 0;									//フェード処理に移行するまでの間隔
	s_nTimeTitle = 0;

	//タイトル画面の初期化処理
	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		s_Tex[nTexCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_Tex[nTexCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		s_Tex[nTexCnt].bUse = true;
	}
	s_Tex[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	s_Tex[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTitle,
		NULL);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffTitlePress,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		switch (nTexCnt)
		{
		case 0:
			//頂点座標の設定
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

			s_Tex[nTexCnt].bUse = true;
			break;
		case 1:
			//頂点座標の設定
			pVtx[0].pos.x = 1.0f;
			pVtx[0].pos.y = 125.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = 1.0f + 1220.0f;
			pVtx[1].pos.y = 125.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 1.0f;
			pVtx[2].pos.y = 510.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = 1.0f + 1220.0f;
			pVtx[3].pos.y = 510.0f;
			pVtx[3].pos.z = 0.0f;
			break;
		case 2:
			//頂点座標の設定
			pVtx[0].pos.x = 510.0f;
			pVtx[0].pos.y = 130.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = SCREEN_WIDTH;
			pVtx[1].pos.y = 130.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 510.0f;
			pVtx[2].pos.y = SCREEN_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = SCREEN_WIDTH;
			pVtx[3].pos.y = SCREEN_HEIGHT;
			pVtx[3].pos.z = 0.0f;
			break;
		case 3:
			//頂点座標の設定
			pVtx[0].pos.x = 240.0f;
			pVtx[0].pos.y = 285.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = 1040.0f;
			pVtx[1].pos.y = 285.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 240.0f;
			pVtx[2].pos.y = 435.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = 1040.0f;
			pVtx[3].pos.y = 435.0f;
			pVtx[3].pos.z = 0.0f;
			break;
		case 4:
			//頂点座標の設定
			pVtx[0].pos.x = 200.0f;
			pVtx[0].pos.y = 230.0f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = 1110.0f;
			pVtx[1].pos.y = 230.0f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = 200.0f;
			pVtx[2].pos.y = 520.0f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = 1110.0f;
			pVtx[3].pos.y = 520.0f;
			pVtx[3].pos.z = 0.0f;
			break;
		default:
			break;
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = s_Tex[nTexCnt].col;
		pVtx[1].col = s_Tex[nTexCnt].col;
		pVtx[2].col = s_Tex[nTexCnt].col;
		pVtx[3].col = s_Tex[nTexCnt].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	s_pVtxBuffTitle->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffTitlePress->Lock(0, 0, (void**)&pVtx, 0);

	//長方形の初期化
	//rhwの設定
	//頂点カラーの設定
	//テクスチャ座標の設定
	InitRect(pVtx);

	//頂点座標の設定
	pVtx[0].pos.x = press.pos.x;
	pVtx[0].pos.y = press.pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = press.pos.x + TITLEPRESS_WIDTH;
	pVtx[1].pos.y = press.pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = press.pos.x;
	pVtx[2].pos.y = press.pos.y + TITLEPRESS_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = press.pos.x + TITLEPRESS_WIDTH;
	pVtx[3].pos.y = press.pos.y + TITLEPRESS_HEIGHT;
	pVtx[3].pos.z = 0.0f;


	//頂点バッファをアンロックする
	s_pVtxBuffTitlePress->Unlock();
}

//=========================================
//終了処理
//=========================================
void UninitTitle(void)
{
	int nCnt;
	//サウンドの停止
	StopSound();

	for (nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (s_pTextureTitle[nCnt] != NULL)
		{
			s_pTextureTitle[nCnt]->Release();
			s_pTextureTitle[nCnt] = NULL;
		}

	}

	//頂点バッファの破棄
	if (s_pVtxBuffTitle != NULL)
	{
		s_pVtxBuffTitle->Release();
		s_pVtxBuffTitle = NULL;
	}

	//テクスチャの破棄
	if (s_pTextureTitlePress != NULL)
	{
		s_pTextureTitlePress->Release();
		s_pTextureTitlePress = NULL;
	}

	//頂点バッファの破棄
	if (s_pVtxBuffTitlePress != NULL)
	{
		s_pVtxBuffTitlePress->Release();
		s_pVtxBuffTitlePress = NULL;
	}
}

//=========================================
//更新処理
//=========================================
void UpdateTitle(void)
{
	int nTexCnt;
	s_nTimeTitle++;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//
	//タイトルの画像
	//
	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (s_Tex[4].col.a >= 1.0f && s_Tex[1].col.a <= 1.0f)
	{//タイトルの影のフェード
		s_Tex[1].col.a += 0.001f;
	}
	if (s_Tex[4].col.a <= 1.0f)
	{//タイトルのフェード
		s_Tex[4].col.a += 0.005f;
	}

	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		SetRectColor(pVtx, &(s_Tex[nTexCnt].col));
		//頂点カラーの設定
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	s_pVtxBuffTitlePress->Unlock();

	//
	//プレスタイトル
	//
	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffTitlePress->Lock(0, 0, (void**)&pVtx, 0);

	//ゲームモードに移行
	if (s_bFadeCheek == false)
	{
		if (GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN) == true)
		{//EnterキーかパッドのBを押された時
		 //決定音の再生
			PlaySound(SOUND_LABEL_SE_ENTER);
			s_bFadeCheek = true;	//フェード処理に入る
		}

		//点滅処理
		switch (s_presstype)
		{
		case TYPE_NOME:		//点灯状態
			//頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
			if (s_nTypeCnt == 50)
			{	//消灯状態に移行
				s_presstype = TYPE_BLINK;
				s_nTypeCnt = 0;
			}
			break;
		case TYPE_BLINK:	//消灯状態
			//頂点カラーの設定
			SetRectColor(pVtx, &(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)));
			if (s_nTypeCnt == 50)
			{	//点灯状態に移行
				s_presstype = TYPE_NOME;
				s_nTypeCnt = 0;
			}
			break;
		default:
			break;
		}
	}
	else if (s_bFadeCheek == true)
	{
		//頂点カラーの設定
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
			SetFade(MODE_GAME);	//ゲームモードに移行
		}
		s_nFadeCnt++;
	}

	//頂点バッファをアンロックする
	s_pVtxBuffTitlePress->Unlock();

	//切り替えタイミング
	s_nTypeCnt++;

	if (s_nTimeTitle >= 1500)
	{
		SetFade(MODE_RANKING);
	}
}

//=========================================
//描画処理
//=========================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nTexCnt;

	//デバイスの取得
	pDevice = GetDevice();

	//背景の描写
	//頂点バッファをデータストリーム設定
	pDevice = InitDraw(pDevice, s_pVtxBuffTitle);

	for (nTexCnt = 0; nTexCnt < MAX_TEXTURE; nTexCnt++)
	{
		if (s_Tex[nTexCnt].bUse == true)
		{//弾が使用されている
			//ポリゴン描画
			//テクスチャの設定
			pDevice = SetDraw(pDevice, s_pTextureTitle[nTexCnt], nTexCnt);
		}
	}

	//PRESSENTERの描写
	//頂点バッファをデータストリーム設定
	pDevice = InitDraw(pDevice, s_pVtxBuffTitlePress);
	//ポリゴン描画
	pDevice = SetDraw(pDevice, s_pTextureTitlePress, 0);
}