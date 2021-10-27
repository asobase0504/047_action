//=========================================
// 
// �l�X�ȏ�������ݒ菈�����܂Ƃ߂�B
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "setup.h"

//=========================================
// �����`�̏�����
//=========================================
void InitRect(VERTEX_2D * vtx)
{
	// ���_���W�̐ݒ�
	InitRectPos(vtx);

	// ���_�J���[�̐ݒ�
	InitRectColor(vtx);

	// �e�N�X�`�����W�̐ݒ�
	InitRectTex(vtx);

	// rhw �̐ݒ�
	InitRectRhw(vtx);
}

//=========================================
// ���_�o�b�t�@��POS��������
//=========================================
void InitRectPos(VERTEX_2D * vtx)
{
	vtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// ���_�o�b�t�@��POS��ݒ�(���S���W)
//=========================================
void SetRectCenterPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fHeigth, float fWidth)
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

void SetRectUpLeftPos(VERTEX_2D * vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	vtx[0].pos.x = pos.x;
	vtx[0].pos.y = pos.y;
	vtx[0].pos.z = pos.z;

	vtx[1].pos.x = pos.x + fWidth;
	vtx[1].pos.y = pos.y;
	vtx[1].pos.z = pos.z + 0.0f;

	vtx[2].pos.x = pos.x;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z + 0.0f;

	vtx[3].pos.x = pos.x + fWidth;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z + 0.0f;
}

//=========================================
// ���_�o�b�t�@�̃J���[��������
//=========================================
void InitRectColor(VERTEX_2D * vtx)
{
	for (int i = 0; i < 4; ++i)
	{
		vtx->col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		vtx++;
	}
}

//=========================================
// ���_�o�b�t�@�̃J���[��ݒ�
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
// ���_�o�b�t�@�̃e�N�X�`�����W�̏�����
//=========================================
void InitRectTex(VERTEX_2D * vtx)
{
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=========================================
// ���_�o�b�t�@�̃e�N�X�`�����W�̐ݒ�
//=========================================
void SetRextTex(VERTEX_2D * vtx, float top, float bottom, float left, float right)
{
	vtx[0].tex = D3DXVECTOR2(left, top);
	vtx[1].tex = D3DXVECTOR2(right, top);
	vtx[2].tex = D3DXVECTOR2(left, bottom);
	vtx[3].tex = D3DXVECTOR2(right, bottom);

}

//=========================================
// rhw �̏�����
//=========================================
void InitRectRhw(VERTEX_2D * vtx)
{
	// rhw �̐ݒ�
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;
}

//=========================================
// �`�ʏ����ɑO��Ƃ��ĕK�v�ȕ���
//=========================================
LPDIRECT3DDEVICE9 InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	return pDevice;
}

//=========================================
// ���Z�����Ȃ��̕`��
//=========================================
LPDIRECT3DDEVICE9 SetDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture);
	
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt, 2);

	return pDevice;
}

//=========================================
// ���Z�����L��̕`��
//=========================================
LPDIRECT3DDEVICE9 AddSetDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice = SetDraw(pDevice, Texture, nCnt);

	// a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return pDevice;
}

//====================================
// �����̊O�Ϗ���
//====================================
float D3DXVec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return v1->x * v2->y - v1->y * v2->x;
}