//====================================
// 
// ìGè¢ä´ÇÃèàóù
// Author YudaKaito
// 
//====================================
#include "main.h"
#include "summon.h"
#include <assert.h>

//------------------------------------
// è¢ä´WAVEÇÃóÒãìå^
//------------------------------------
typedef enum
{
	WAVE_0 = 0,
	WAVE_1,
	WAVE_2,
	WAVE_3,
	WAVE_4,
	WAVE_5,
	MAX_WAVE
}SUMMON_WAVE;

//------------------------------------
// staticïœêî
//------------------------------------
static int s_wave = 0;
static int nCnt = 0;

//====================================
// ìGÇÃè¢ä´(éwíËÇ»Çµ)
//====================================
void UpdateSummon(void)
{
	nCnt++;
	D3DXVECTOR3 pos;
	switch (s_wave)
	{
	case WAVE_0:
		if (nCnt <= 500 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(330.0f + 50.0f * nCnt * 0.05f, 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_UP);
		}

		if (nCnt >= 400 && nCnt <= 900 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(330.0f + 50.0f * (nCnt - 400) * 0.05f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_DWON);
		}

		if (nCnt >= 800 && nCnt <= 1300 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(350.0f, 1000.0f - 50.0f * (nCnt - 800) * 0.05f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
		}

		if (nCnt >= 1200 && nCnt <= 1700 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 50.0f * (nCnt - 1200) * 0.05f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
		}

		if (nCnt == 1700)
		{
			SetWave(1);
		}

		break;
	case WAVE_1:
		if (nCnt <= 500 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(330.0f + 50.0f * nCnt * 0.05f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_DWON);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 50.0f * nCnt * 0.05f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
		}

		if (nCnt >= 550 && nCnt <= 1050 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(330.0f + 50.0f * (nCnt - 550) * 0.05f, 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_UP);
			pos = D3DXVECTOR3(1550.0f - 50.0f * (nCnt - 550) * 0.05f, 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_UP);
		}

		if (nCnt >= 1100 && nCnt <= 1600 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(1550.0f - 50.0f * (nCnt - 1100) * 0.05f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_DWON);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 50.0f * (nCnt - 1100) * 0.05f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
		}

		if (nCnt == 1700)
		{
			pos = D3DXVECTOR3(350.0f, 1000.0f - 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 300.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 500.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 700.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 900.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);

			pos = D3DXVECTOR3(1550.0f, 1000.0f - 200.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 400.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 600.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 800.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
		}
		if (nCnt == 1800)
		{
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 300.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 500.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 700.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 900.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);

			pos = D3DXVECTOR3(350.0f, 1000.0f - 200.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 400.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 600.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(350.0f, 1000.0f - 800.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			SetWave(2);
		}
		break;
	case WAVE_2:
		if (nCnt == 50)
		{
			pos = D3DXVECTOR3(1550.0f - 50.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(1550.0f - 100.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(1550.0f - 150.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(1550.0f - 200.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(1550.0f - 250.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);

			pos = D3DXVECTOR3(330.0f + 50.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(330.0f + 100.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(330.0f + 150.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(330.0f + 200.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(330.0f + 250.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
		}

		if (nCnt >= 300 && nCnt <= 600 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(330.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(1550.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
		}

		if (nCnt == 800)
		{
			pos = D3DXVECTOR3(350.0f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(330.0f, 900.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
			pos = D3DXVECTOR3(1550.0f, 900.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING);
		}
		break;
	case WAVE_3:
		break;
	case WAVE_4:
		break;
	case WAVE_5:
		break;
	default:
		break;
	}
}

//====================================
// ìGÇÃè¢ä´(éwíËÇ†ÇË)
//====================================
void EnemySummon(SUMMON_TYPE type)
{
	switch (type)
	{
	case SOLO_SPLITBALL:
		SetEnemy(D3DXVECTOR3(100.0f,100.0f,100.0f),SPLITBALL_FIRST);
		break;
	case GROUP_TWOSPLITBALL:
		break;
	default:
		assert(false);
		break;
	}
}

//====================================
// ï«àÍñ Ç…è¢ä´(è„)
//====================================
void OnesideTopSummon(void)
{
	int Summon = 0;
	int Cnt = 0;

	while (Summon <= 20)
	{
		Cnt++;
		if (Cnt >= 60)
		{
			Cnt = 0;
			Summon++;
			SetEnemy(D3DXVECTOR3(500.0f + Summon * 20.0f, 30.0f, 0.0f),GOSTRAIGHT_UP);
		}
	}
}

void SetWave(int wave)
{
	s_wave = wave;
	nCnt = 0;
}
