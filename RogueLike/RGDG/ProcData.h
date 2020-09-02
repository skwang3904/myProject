#pragma once

#include "iStd.h"

struct PlayerInfo
{
	float _hp;
	float _attackPoint;
	float _attackSpeed;
	float moveSpeed;
};
extern PlayerInfo playerInfo[2];

//----------------------------------------------------------------

struct MonsterInfo
{
	float _hp;
	float _attackPoint;
	float _attackSpeed;
	float moveSpeed;

	float _attackDt;
	float _getDmgDt;
	float lookDistance;
	float meleeDistance;
	float rangeDistance;
};
extern MonsterInfo monsterInfo[2];

struct WeaponInfo
{
	float _attackPoint;
	float _attackSpeed;
	float _attackDelay;
	float attackAngle;
	float attackRange;
};
extern WeaponInfo weaponInfo[1];