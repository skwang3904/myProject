#pragma once

#include "iStd.h"

#include "Common.h"

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
	float _actionDt;
	float moveSpeed;

	float _attackDt; // 공격 후 이 시간 이후 데미지판정
	float _attackDelay;
	float lookDistance;
	float meleeDistance;
	float rangeDistance;

	int itemDropNum;
	int itemTypeKindNum;
	int itemTypeKind[5];

};
extern MonsterInfo monsterInfo[3];

struct MonsterImageInfo
{
	const char* strPath;
	int imgNum;
	float aniDt;
	int repeatNum;
	bool lastFrame;
	iSize size;
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

//----------------------------------------------------------------

struct ItemInfo
{
	float value;
	float _actionDt;
};
extern ItemInfo itemInfo[3];

struct ItemImageInfo
{
	const char* strPath;
	int imgNum;
	float aniDt;
	iSize size;
};
extern ItemImageInfo itemImageInfo[3];