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

struct PlayerImageInfo
{
	const char* strPath;
	int dirNum;
	int kindsNum;
	bool animation;
	float aniDt;
	iSize size;
};
extern PlayerImageInfo playerImageInfo[5];

//----------------------------------------------------------------

struct MonsterInfo
{
	float _hp;
	float _attackPoint;
	float _attackSpeed;
	float _attackDt; // 공격 후 이 시간 이후 데미지판정
	float _attackDelay;
	float _actionDt;

	float moveSpeed;
	float lookDistance;
	float meleeDistance;
	float rangeDistance;
};
extern MonsterInfo monsterInfo[3];
#define GOLEM_NOMAL 0
#define GOLEM_ELETE 1
#define GOLEM_BOSS	2

struct MonsterImageInfo
{
	const char* strPath;
	int imgNum;
	float aniDt;
	int repeatNum;
	bool lastFrame;
};
extern MonsterImageInfo golemNomalImage[6];
extern MonsterImageInfo golemBossImage[6];

//----------------------------------------------------------------

struct WeaponInfo
{
	float _attackPoint;
	float _attackSpeed;
	float _attackDelay;
	float attackAngle;
	float attackRange;
};
extern WeaponInfo weaponInfo[2];