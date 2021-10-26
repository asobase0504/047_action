#ifndef _TITLE_H_			// ���̃}�N����`������ĂȂ�������
#define _TITLE_H_			// 2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

// �v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 rot;	// ����
	float fWidth;		// �摜�̕�
	float fHeigth;		// �摜�̍���
	float fLength;		// �Ίp���̒���
	float fAngle;		// �Ίp���̊p�x

}TITLE;

// �v���g�^�C�v�錾
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif // !_POLYGON_H_

