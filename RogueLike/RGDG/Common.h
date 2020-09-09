#pragma once

#include "iStd.h"

#include "Tile.h"

extern int stageNum;
//----------------------------------------------------------------------------
// class

class Object
{
public:
	Object(int index, int8 mapNum, iPoint pos);
	virtual ~Object();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void drawShadow(float dt, iPoint off) = 0;
	virtual void action(Object* obj) = 0;

public:
	int index;
	bool alive;

	int imgNum;
	iImage** imgs;
	iImage* img;
	int8 mapNumber;

	iPoint position;
	iPoint vector;
	iRect touchRect;

	float prevHp;
	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float actionDt, _actionDt;

	float moveSpeed;
};

//----------------------------------------------------------------------------
// Stage data

struct MapData
{
	int state;

	int tileIndex;
};

struct PlayerData
{
	int index;
	int8 mapNum;
	iPoint position;

	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float moveSpeed;
};

enum MonsterType {
	MT_golemNomal = 0,
	MT_golemBoss,

	MT_max
};

struct MonsterData
{
	int index;
	int8 mapNum;
	iPoint position;
};

struct WeaponData
{
	int index;
};

struct Stage
{
	int currStage;

	MapData mapData[TILE_TOTAL_NUM];

	// playerInfo
	PlayerData playerData;

	// monsterInfo
	int actMonsterNum[MT_max];
	MonsterData monsterData[30];

	// weaponInfo
	WeaponData weaponData[10];

	void create();
	void setStageData();

	void setPlayerData(int* actMap, int connectNum);
	void setMonsterData(int* actMap, int connectNum);
};
extern Stage* st;

void loadStage();
void saveStage();
void freeStage();

//----------------------------------------------------------------------------
// passMap

struct PassMap
{
	float passDt, _passDt;
	int8 prevMapNumber;
	int8 mapNumber;

	void pass(int8 mapNum);
	void update(float dt);

	void nextStage();
};
extern PassMap* passMap;
#define PASS_DT 0.5f

void loadPassMap();
void freePassMap();