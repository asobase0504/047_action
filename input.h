//=========================================
//
//キーボードのヘッダーファイル
//Author YudaKaito
//
//=========================================
#ifndef  _INPUT_H_
#define	_INPUT_H_

//キーの処理
typedef enum
{
	JOYKEY_UP = 0,		//十字キー（上）
	JOYKEY_DOWN,		//十字キー（下）
	JOYKEY_LEFT,		//十字キー（左）
	JOYKEY_RIGHT,		//十字キー（右）
	JOYKEY_START,		//STARTボタン
	JOYKEY_BACK,		//BACKボタン
	JOYKEY_PUSHLSTICK,	//左スティック押し込み
	JOYKEY_PUSHRSTICK,	//右スティック押し込み
	JOYKEY_LBUTTON5,	//左ボタン上
	JOYKEY_RBUTTON5,	//右ボタン上
	JOYKEY_10,			//不明
	JOYKEY_11,			//不明
	JOYKEY_A,			//Aボタン
	JOYKEY_B,			//Bボタン
	JOYKEY_X,			//Xボタン
	JOYKEY_Y,			//Yボタン
	MAX_JOYKEY,			//最大数
}JOYKEY;

//プロトタイプ宣言
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