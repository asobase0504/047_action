//====================================
// 
// �Q�[���̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//====================================
#ifndef _SETUP_H_
#define _SETUP_H_

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------

// 				// 
// �����`����	// 
// 				// 
void InitRect			(VERTEX_2D *vtx);					// 	--------------------	// �����`�̏�����
void InitRectPos		(VERTEX_2D *vtx);					// 	--------------------	// ���_���W������
void SetRectCenterPos	(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);	// ���S���W�����S�ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�
void SetRectUpLeftPos	(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);	// ���S���W������ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�
void SetRectUpRightPos	(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);	// ���S���W���E��ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�
void InitRectColor		(VERTEX_2D *vtx);					// 	--------------------	// ���_�o�b�t�@�̐F�̏�����
void SetRectColor		(VERTEX_2D *vtx, D3DXCOLOR *inColor);							// ���_�o�b�t�@�̐F�ݒ�
void InitRectTex		(VERTEX_2D *vtx);					// 	--------------------	// ���_�o�b�t�@�̃e�N�X�`�����W�̏�����
void SetRectTex			(VERTEX_2D *vtx,float top,float bottom,float left,float right);	// ���_�o�b�t�@�̃e�N�X�`�����W�̐ݒ�
void InitRectRhw		(VERTEX_2D *vtx);

// 				// 
// �I������		// 
// 				// 
void UnInitTexture	(LPDIRECT3DTEXTURE9 tex);
void UnInitBuffer	(LPDIRECT3DVERTEXBUFFER9 buff);

// 				// 
// �`�ʏ���		// 
// 				// 
void InitDraw	(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff);		// �`�ʑO�̏���
LPDIRECT3DDEVICE9 SetDraw	(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// �ʏ�̕`��
void AddSetDraw	(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// ���Z�����̕`��

// 				// 
// �v�Z����		// 
// 				// 
float D3DXVec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2);	// �O�ς̌v�Z����

#endif // !_SETUP_H_


