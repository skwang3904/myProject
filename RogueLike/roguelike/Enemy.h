#pragma once

#include "iStd.h"
/*
적 유닛 
( hp, 공격력, 디버프, 버프, 이동속도, 위치p, 목표위치tp, 
애니메이션, 히트판정)
- 기본유닛
- 보스 
*/

#define GOLEM_IMG_NUM 6
#define GOLEM_ELETE_IMG_NUM 6


void createEnemy();
void freeEnemy();
void drawEnemy(float dt);

/*
	// 0, 1 idle pattern 2
	img[0] idle
	img[1] idle eye Blink

	// 2 Walk
	img[2] 

	// 3 Attacking
	img[3] 

	// 4 Dying
	img[4]

*/