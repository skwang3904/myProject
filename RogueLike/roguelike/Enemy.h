#pragma once

#include "iStd.h"
/*
적 유닛 
( hp, 공격력, 디버프, 버프, 이동속도, 위치p, 목표위치tp, 
애니메이션, 히트판정)
- 기본유닛
- 보스 
*/

void createEnemy();
void freeEnemy();
void drawEnemy(float dt);

/*
	imgNumber
	0 idle 
	1 idle Blink
	2 Walk
	3 Attacking
	4 Dying
	5 Magic
	6 Hurt
*/