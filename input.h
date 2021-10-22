//=========================================
//
//�L�[�{�[�h�̃w�b�_�[�t�@�C��
//Author YudaKaito
//
//=========================================
#ifndef  _INPUT_H_
#define	_INPUT_H_

//�L�[�̏���
typedef enum
{
	JOYKEY_UP = 0,		//�\���L�[�i��j
	JOYKEY_DOWN,		//�\���L�[�i���j
	JOYKEY_LEFT,		//�\���L�[�i���j
	JOYKEY_RIGHT,		//�\���L�[�i�E�j
	JOYKEY_START,		//START�{�^��
	JOYKEY_BACK,		//BACK�{�^��
	JOYKEY_PUSHLSTICK,	//���X�e�B�b�N��������
	JOYKEY_PUSHRSTICK,	//�E�X�e�B�b�N��������
	JOYKEY_LBUTTON5,	//���{�^����
	JOYKEY_RBUTTON5,	//�E�{�^����
	JOYKEY_10,			//�s��
	JOYKEY_11,			//�s��
	JOYKEY_A,			//A�{�^��
	JOYKEY_B,			//B�{�^��
	JOYKEY_X,			//X�{�^��
	JOYKEY_Y,			//Y�{�^��
	MAX_JOYKEY,			//�ő吔
}JOYKEY;

//�v���g�^�C�v�錾
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelese(int nKey);

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelese(JOYKEY key);

#endif //  _INPUT_H_