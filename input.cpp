//=========================================
//
//キーボードの処置
//Author YudaKaito
//
//=========================================
#include "main.h"
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX	(256)	//キーの最大数

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;					//DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX];					//キーボードのプレス情報
BYTE g_akeyStateTrigger[NUM_KEY_MAX];			//キーボードのトリガー情報
BYTE g_akeyStateRelese[NUM_KEY_MAX];			//キーボードのリリース情報
XINPUT_STATE g_joyKeyState;						//ジョイパッドのプレス情報
int g_ajoykeyStateTrigger;						//ジョイパッドのトリガー情報
int g_ajoykeyStateRelese;						//ジョイパッドのリリース情報

//====================================
//キーボードの初期化処理
//====================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//Directオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//====================================
//キーボードの終了処理
//====================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();	//キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInuptオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
	}
}

//====================================
//キーボードの更新処理
//====================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_akeyStateTrigger[nCntKey] = ~g_aKeyState[nCntKey] & aKeyState[nCntKey];
			g_akeyStateRelese[nCntKey] = g_aKeyState[nCntKey] & ~aKeyState[nCntKey];
			g_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	//キーボードのアクセス権を獲得
	}
}

//====================================
//キーボードのプレス情報を取得
//====================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//====================================
//キーボードのトリガー情報を取得
//====================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_akeyStateTrigger[nKey] & 0x80) ? true : false;
}

//====================================
//キーボードのリリース情報を取得
//====================================
bool GetKeyboardRelese(int nKey)
{
	return (g_akeyStateRelese[nKey] & 0x80) ? true : false;
}

//====================================
//ジョイパットの初期化処理
//====================================
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効にする)
	XInputEnable(true);

	return S_OK;
}

//====================================
//ジョイパットの終了処理
//====================================
void UninitJoypad(void)
{
	//XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//====================================
//ジョイパットの更新処理
//====================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;	//ジョイパッドの入力処理

	//ジョイパッドの状態の取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{

		g_ajoykeyStateTrigger = ~g_joyKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;
		g_ajoykeyStateRelese = g_joyKeyState.Gamepad.wButtons & ~joykeyState.Gamepad.wButtons;
		g_joyKeyState = joykeyState;	//ジョイパッドのプレス情報を保存
	}

}

//====================================
//ジョイパットのプレス情報を取得
//====================================
bool GetJoypadPress(JOYKEY key)
{
	return(g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================
//キーボードのトリガー情報を取得
//====================================
bool GetJoypadTrigger(JOYKEY key)
{
	return(g_ajoykeyStateTrigger & (0x01 << key)) ? true : false;
}