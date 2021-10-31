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
// staticïœêî
//------------------------------------
static int nCnt = 0;

//====================================
// ìGÇÃè¢ä´(éwíËÇ»Çµ)
//====================================
void UpdateSummon(void)
{
	nCnt++;

	if (nCnt >= 150)
	{
		nCnt = 0;
		int r = rand() % SUMMON_TYPE_MAX;

		switch (r)
		{
		case SOLO_SPLITBALL:
			SetEnemy({ 1100.0f,350.0f,0.0f }, SPLITBALL_FIRST);
			break;
		case SOLO_EXTENDBALL_UP:
//			SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_UP);
			break;
		case SOLO_EXTENDBALL_DWON:
//			SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_DWON);
			break;
		case SOLO_EXTENDBALL_LEFT:
//			SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_LEFT);
			break;
		case SOLO_EXTENDBALL_RIGHT:
//			SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_RIGHT);
			break;
		case GROUP_TWOSPLITBALL:
			SetEnemy({ 1100.0f,350.0f,0.0f }, SPLITBALL_FIRST);
			SetEnemy({ 1600.0f,350.0f,0.0f }, SPLITBALL_FIRST);
			break;
		default:
			assert(false);
			break;
		}
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
	case SOLO_EXTENDBALL_UP:
		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_UP);
		break;
	case SOLO_EXTENDBALL_DWON:
		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_DWON);
		break;
	case SOLO_EXTENDBALL_LEFT:
		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_LEFT);
		break;
	case SOLO_EXTENDBALL_RIGHT:
		SetEnemy(D3DXVECTOR3(100.0f, 100.0f, 100.0f), EXTENDBALL_RIGHT);
		break;
	case GROUP_TWOSPLITBALL:
		break;
	default:
		assert(false);
		break;
	}
}
