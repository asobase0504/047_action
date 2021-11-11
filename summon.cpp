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

		if (nCnt >= 750 && nCnt <= 1250 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 50.0f * (nCnt - 750) * 0.05f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
		}

		if (nCnt >= 1050 && nCnt <= 1550 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(350.0f, 1000.0f - 50.0f * (nCnt - 1050) * 0.05f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
		}

		if (nCnt == 1550)
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
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f - 100.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f - 150.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f - 200.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f - 250.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);

			pos = D3DXVECTOR3(330.0f + 50.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f + 100.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f + 150.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f + 200.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f + 250.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt >= 300 && nCnt <= 600 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(330.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt == 800)
		{
			pos = D3DXVECTOR3(350.0f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(330.0f, 900.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f, 900.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt == 1000)
		{
			for (int i = 0; i < 14; i++)
			{
				pos = D3DXVECTOR3(1550.0f - 100.0f * i, 100.0f, 0.0f);
				SetEnemy(pos, BALL_HOMING00);
			}
		}

		if (nCnt >= 1200 && nCnt <= 1400 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(1550.0f - 100.0f * (nCnt - 1200) * 0.05f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(350.0f + 100.0f * (nCnt - 1200) * 0.05f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt == 1600)
		{
			pos = D3DXVECTOR3(330.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt == 1700)
		{
			pos = D3DXVECTOR3(330.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);

			pos = D3DXVECTOR3(330.0f, 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(1550.0f, 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
			pos = D3DXVECTOR3(330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_LEFT);
			pos = D3DXVECTOR3(1550.0f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
		}

		if (nCnt >= 1750 && nCnt <= 2000 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(900.0f + 50.0f * (nCnt - 1750) * 0.05f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_DWON);
			pos = D3DXVECTOR3(900.0f - 50.0f * (nCnt - 1750) * 0.05f, 1000.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_DWON);
		}
		if (nCnt == 2000)
		{
			SetWave(3);
		}
		break;
	case WAVE_3:
		if (nCnt == 100)
		{
			for (int i = 0; i < 14; i++)
			{
				pos = D3DXVECTOR3(1550.0f - 100.0f * i, 100.0f, 0.0f);
				SetEnemy(pos, BALL_HOMING01);
			}
		}

		if (nCnt >= 200 && nCnt <= 700 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(330.0f + 50.0f * (nCnt - 200) * 0.05f, 100.0f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_UP);
			pos = D3DXVECTOR3(330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt >= 750 && nCnt <= 1250 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(1550.0f, 1000.0f - 50.0f * (nCnt - 750) * 0.05f, 0.0f);
			SetEnemy(pos, GOSTRAIGHT_RIGHT);
		}

		if (nCnt == 1250)
		{
			pos = D3DXVECTOR3(900.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(900.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
		}
		if (nCnt == 1350)
		{
			for (int i = 0; i < 10; i++)
			{
				pos = D3DXVECTOR3(350.0f, 1000.0f - 100.0f * i, 0.0f);
				SetEnemy(pos, GOSTRAIGHT_LEFT);
				pos = D3DXVECTOR3(1550.0f - 100.0f * i, 100.0f, 0.0f);
				SetEnemy(pos, GOSTRAIGHT_UP);
			}
		}

		if (nCnt == 1400)
		{
			pos = D3DXVECTOR3(900.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(900.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1000.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt == 1500)
		{
			pos = D3DXVECTOR3(800.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(800.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1000.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1000.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(800.0f, 500.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(800.0f, 600.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1000.0f, 500.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1000.0f, 600.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt >= 1550 && nCnt <= 2050 && nCnt % 20 == 0)
		{
			pos = D3DXVECTOR3(1550.0f, 100.0f + 50.0f * (nCnt - 1550) * 0.05f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}

		if (nCnt == 1700)
		{
			pos = D3DXVECTOR3(330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1550.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			SetWave(4);
		}

		break;
	case WAVE_4:
		if (nCnt == 50)
		{
			pos = D3DXVECTOR3(330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(530.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(730.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(930.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1130.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1550.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
		}
		if (nCnt == 70)
		{
			pos = D3DXVECTOR3(330.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f, 300.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(330.0f, 500.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f, 700.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(330.0f, 800.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(330.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
		}
		if (nCnt == 90)
		{
			pos = D3DXVECTOR3(330.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(530.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(730.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(930.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1130.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1330.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1530.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
		}
		if (nCnt == 110)
		{
			pos = D3DXVECTOR3(1530.0f, 100.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1530.0f, 300.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1530.0f, 500.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1530.0f, 700.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			pos = D3DXVECTOR3(1530.0f, 800.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING00);
			pos = D3DXVECTOR3(1530.0f, 1000.0f, 0.0f);
			SetEnemy(pos, BALL_HOMING01);
			SetWave(3);
		}
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

//====================================
// WAVEÇÃê›íË
//====================================
void SetWave(int wave)
{
	s_wave = wave;
	nCnt = 0;
}
