#pragma once

#include "iStd.h"

#include "Common.h"

// Player Data
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
extern PlayerImageInfo playerImageInfo[PLAYER_IMG_NUM];

//----------------------------------------------------------------
// Monster Data
// base status : _stage status
struct MonsterInfo
{
	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float moveSpeed, _moveSpeed;

	float attackDt, _attackDt; // 공격 후 이 시간 이후 데미지판정
	float attackDelay, _attackDelay;
	float lookDistance, _lookDistance;
	float meleeDistance, _meleeDistance;
	float rangeDistance, _rangeDistance;

	int itemDropNum;
	int itemTypeKindNum;
	int itemTypeKind[5];

	float setMonsterStatus(float* base, int stage);
};
extern MonsterInfo monsterInfo[MT_max];

struct MonsterImageInfo
{
	const char* strPath;
	int imgNum;
	float aniDt;
	float _aniDt; // stage aniDt
	int repeatNum;
	bool lastFrame;
	iSize size;
};
extern MonsterImageInfo golemNomalImage[MONSTER_IMG_NUM];
extern MonsterImageInfo golemBossImage[MONSTER_IMG_NUM];

//----------------------------------------------------------------
// Weapon Data
struct WeaponInfo
{
	float attackPoint;
	float attackSpeed;
	float attackDt;	// 공격속도 일정 비율이상에 hit판정
	float attackDelay;
	float actionDt;	// 피해대상 hit모션 시간

	float moveSpeed;
	float attackAngle;
	float attackRange;
};
extern WeaponInfo weaponInfo[WT_max];

struct WeaponImageInfo
{
	const char* strPath;
	iSize size;
};
extern WeaponImageInfo weaponImageInfo[WEAPON_IMG_COUNT];
//----------------------------------------------------------------
// Item Data
struct ItemInfo
{
	float value;
	float _actionDt;
};
extern ItemInfo itemInfo[ITEM_KINDS];

struct ItemImageInfo
{
	const char* strPath;
	int imgNum;
	float aniDt;
	iSize size;
};
extern ItemImageInfo itemImageInfo[ITEM_IMG_COUNT];