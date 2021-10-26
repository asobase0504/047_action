//====================================
//
//ゲームのヘッダーファイル
//Author YudaKaito
//
//====================================
#ifndef _SETUP_H_
#define _SETUP_H_

//------------------------------------
//プロトタイプ宣言
//------------------------------------
void SetRectColor(VERTEX_2D *vtx, D3DXCOLOR *inColor);
void SetRectTex(VERTEX_2D *vtx);
void SetRectPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fHeigth, float fWidth);
void SetupRectDefault(VERTEX_2D *vtx, D3DXCOLOR *inColor);
LPDIRECT3DDEVICE9 InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff);
LPDIRECT3DDEVICE9 AddSyntheticDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);
LPDIRECT3DDEVICE9 SetDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);
float D3DXVec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);												//外積の計算処理

#endif // !_SETUP_H_


