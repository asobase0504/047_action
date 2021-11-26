//=========================================
// 
// リザルト画面の処理
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

// グローバル変数
LPDIRECT3DTEXTURE9 s_pTexture[GAME_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
RESULT s_result;
GAME_CASE s_GameCase;

//=========================================
// 初期化処理
//=========================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント

								// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg004.jpg",
		&s_pTexture[0]);

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bg016.jpg",
		&s_pTexture[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

							// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

}

//=========================================
// 終了処理
//=========================================
void UninitResult(void)
{
	int nCntResult;
	// 音楽の停止
	StopSound();

	for (nCntResult = 0; nCntResult < GAME_MAX; nCntResult++)
	{
		// テクスチャの破棄
		if (s_pTexture[nCntResult] != NULL)
		{
			s_pTexture[nCntResult]->Release();
			s_pTexture[nCntResult] = NULL;
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
void UpdateResult(void)
{
	if (GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		// 決定音の再生
		PlaySound(SOUND_LABEL_SE_ENTER);

		SetFade(MODE_TITLE);
	}

}

//=========================================
// 描画処理
//=========================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture[s_GameCase]);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=========================================
// 描画処理
//=========================================
void GetResult(GAME_CASE Game_Case)
{
	s_GameCase = Game_Case;
}