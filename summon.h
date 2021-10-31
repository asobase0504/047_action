//====================================
// 
// 敵召喚のヘッダーファイル
// Author YudaKaito
// 
//====================================
#ifndef _SUMMON_H_
#define _SUMMON_H_

//------------------------------------
// include
//------------------------------------
#include "enemy.h"

//------------------------------------
// 
//------------------------------------
typedef enum
{
	SOLO_SPLITBALL = 0,
	SOLO_EXTENDBALL_UP,
	SOLO_EXTENDBALL_DWON,
	SOLO_EXTENDBALL_LEFT,
	SOLO_EXTENDBALL_RIGHT,
	GROUP_TWOSPLITBALL,
	SUMMON_TYPE_MAX,
}SUMMON_TYPE;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void UpdateSummon(void);
void EnemySummon(SUMMON_TYPE type);

#endif // !_SUMMON_H_
