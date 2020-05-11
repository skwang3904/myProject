#pragma once

#include "iStd.h"

/*
적 유닛 
( hp, 공격력, 디버프, 버프, 이동속도, 위치p, 목표위치tp, 
애니메이션, 히트판정)
- 기본유닛
- 보스 
*/

struct Enemy1{
	iImage** img;

	float hp, _hp;
	float attackDmg;
	float _attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float moveSpeed;

	iPoint Enemy1Position;

	iRect touchEnemy1;
};

class Monster
{
public:
	Monster();
	~Monster();

public:

};

