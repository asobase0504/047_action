//=========================================
//
//�v���C���[�̏���
//Author YudaKaito
//
//=========================================

//-----------------------------------------
//include
//-----------------------------------------
#include "main.h"
#include "setup.h"

//=========================================
//���_�o�b�t�@��POS��������(Defolt)
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
//���_�o�b�t�@�̃J���[��������
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
//���_�o�b�t�@�̃e�N�X�`����������
//=========================================
void SetRectTex(VERTEX_2D *vtx)
{
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=========================================
//���_�o�b�t�@�̊�{����������
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
//�`�ʏ����ɑO��Ƃ��ĕK�v�ȕ���
//=========================================
LPDIRECT3DDEVICE9 InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	//���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	return pDevice;
}

//=========================================
//���Z�����L��̕`��
//=========================================
LPDIRECT3DDEVICE9 AddSyntheticDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice = SetDraw(pDevice, Texture, nCnt);

	//a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return pDevice;
}

//=========================================
//���Z�����Ȃ��̕`��
//=========================================
LPDIRECT3DDEVICE9 SetDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);

	return pDevice;
}

//====================================
//�����̊O�Ϗ���
//====================================
float D3DXVec2Cross(D3DXVECTOR3 *v1, D3DXVECTOR3 *v2)
{
	return v1->x * v2->y - v1->y * v2->x;
}