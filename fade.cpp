//====================================
// 
// 画面遷移の処理
// Author Yuda Kaito
// 
//====================================

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "fade.h"

//------------------------------------
// グローバル変数
//------------------------------------
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ
FADE s_fade;									// フェードの状態
MODE s_modeNext;								// 次の画面(モード)
D3DXCOLOR s_colorFade;							// ポリゴン(フェード)の色

//====================================
// 画面遷移の初期化処理
//====================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント

								// デバイスの取得
	pDevice = GetDevice();

	s_fade = FADE_IN;		// フェードイン状態に
	s_modeNext = modeNext;	// 次の画面(モード)を設定
	s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 黒いポリゴン(不透明)にしておく


														// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffFade,
		NULL);

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

							// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuffFade->Unlock();

	// モードの設定
	SetMode(s_modeNext);
}

//====================================
// 画面遷移の終了処理
//====================================
void UninitFade(void)
{
	// 頂点バッファの破棄
	if (s_pVtxBuffFade != NULL)
	{
		s_pVtxBuffFade->Release();
		s_pVtxBuffFade = NULL;
	}
}

//====================================
// 画面遷移の更新処理
//====================================
void UpdateFade(void)
{
	if (s_fade != FADE_NONE)
	{
		if (s_fade == FADE_IN)
		{// フェードイン状態
			s_colorFade.a -= 0.005f;	// ポリゴンを透明にしていく

			if (s_colorFade.a <= 0.0f)
			{
				s_colorFade.a = 0.0f;
				s_fade = FADE_NONE;	// 何もしていない状態に
			}

		}
		else if (s_fade == FADE_OUT)
		{// フェードアウト状態
			s_colorFade.a += 0.01f;

			if (s_colorFade.a >= 1.0f)
			{
				s_colorFade.a = 1.0f;
				s_fade = FADE_IN;	// フェードイン常置に

									// モード設定(次の画面に移行)
				SetMode(s_modeNext);
			}

		}

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

								// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定
		pVtx[0].col = s_colorFade;
		pVtx[1].col = s_colorFade;
		pVtx[2].col = s_colorFade;
		pVtx[3].col = s_colorFade;

		// 頂点バッファをアンロックする
		s_pVtxBuffFade->Unlock();


	}
}

//====================================
// 画面遷移の描画処理
//====================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポイント

								// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

void SetFade(MODE modeNext)
{
	if (s_modeNext != modeNext)
	{
		s_fade = FADE_OUT;		// フェードアウト状態に
		s_modeNext = modeNext;	// 次の画面(モード)を設定
		s_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒いポリゴン(透明)にしておく
	}
}

//====================================
// 画面遷移の取得処理
//====================================
FADE GetFade(void)
{
	return s_fade;
}