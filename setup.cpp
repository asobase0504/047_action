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
#include "setup.h"

//=========================================
//頂点バッファのPOSを初期化(Defolt)
//=========================================
void SetRectPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fHeigth, float fWidth)
{
	vtx[0].pos.x = pos.x - fWidth;
	vtx[0].pos.y = pos.y - fHeigth;
	vtx[0].pos.z = pos.z + 0.0f;

	vtx[1].pos.x = pos.x + fWidth;
	vtx[1].pos.y = pos.y - fHeigth;
	vtx[1].pos.z = pos.z + 0.0f;

	vtx[2].pos.x = pos.x - fWidth;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z + 0.0f;

	vtx[3].pos.x = pos.x + fWidth;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z + 0.0f;
}

//=========================================
//頂点バッファのカラーを初期化
//=========================================
void SetRectColor(VERTEX_2D *vtx, D3DXCOLOR *inColor)
{
	for (int i = 0; i < 4; ++i)
	{
		vtx->col = *inColor;
		vtx++;
	}
}

//=========================================
//頂点バッファのテクスチャを初期化
//=========================================
void SetRectTex(VERTEX_2D *vtx)
{
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=========================================
//頂点バッファの基本情報を初期化
//=========================================
void SetupRectDefault(VERTEX_2D *vtx, D3DXCOLOR *inColor)
{
	SetRectColor(vtx, inColor);
	SetRectTex(vtx);

	for (int i = 0; i < 4; ++i)
	{
		vtx->rhw = 1.0f;
		++vtx;
	}
}


//=========================================
//描写処理に前提として必要な部分
//=========================================
LPDIRECT3DDEVICE9 InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	//頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	return pDevice;
}

//=========================================
//加算合成有りの描写
//=========================================
LPDIRECT3DDEVICE9 AddSyntheticDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice = SetDraw(pDevice, Texture, nCnt);

	//aブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return pDevice;
}

//=========================================
//加算合成なしの描写
//=========================================
LPDIRECT3DDEVICE9 SetDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	//テクスチャの設定
	pDevice->SetTexture(0, Texture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);

	return pDevice;
}

//====================================
//線分の外積処理
//====================================
float D3DXVec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return v1->x * v2->y - v1->y * v2->x;
}