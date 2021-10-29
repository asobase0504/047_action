//====================================
// 
// ゲームのヘッダーファイル
// Author YudaKaito
// 
//====================================
#ifndef _SETUP_H_
#define _SETUP_H_

//------------------------------------
// プロトタイプ宣言
//------------------------------------

// 				// 
// 長方形処理	// 
// 				// 
void InitRect			(VERTEX_2D *vtx);					// 	--------------------	// 長方形の初期化
void InitRectPos		(VERTEX_2D *vtx);					// 	--------------------	// 頂点座標初期化
void SetRectCenterPos	(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);	// 中心座標が中心にあった場合の頂点バッファの座標設定
void SetRectUpLeftPos	(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);	// 中心座標が左上にあった場合の頂点バッファの座標設定
void SetRectUpRightPos	(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);	// 中心座標が右上にあった場合の頂点バッファの座標設定
void InitRectColor		(VERTEX_2D *vtx);					// 	--------------------	// 頂点バッファの色の初期化
void SetRectColor		(VERTEX_2D *vtx, D3DXCOLOR *inColor);							// 頂点バッファの色設定
void InitRectTex		(VERTEX_2D *vtx);					// 	--------------------	// 頂点バッファのテクスチャ座標の初期化
void SetRectTex			(VERTEX_2D *vtx,float top,float bottom,float left,float right);	// 頂点バッファのテクスチャ座標の設定
void InitRectRhw		(VERTEX_2D *vtx);

// 				// 
// 終了処理		// 
// 				// 
void UnInitTexture	(LPDIRECT3DTEXTURE9 tex);
void UnInitBuffer	(LPDIRECT3DVERTEXBUFFER9 buff);

// 				// 
// 描写処理		// 
// 				// 
void InitDraw	(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff);		// 描写前の処理
LPDIRECT3DDEVICE9 SetDraw	(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 通常の描写
void AddSetDraw	(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 加算処理の描写

// 				// 
// 計算処理		// 
// 				// 
float D3DXVec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);	// 外積の計算処理

#endif // !_SETUP_H_


