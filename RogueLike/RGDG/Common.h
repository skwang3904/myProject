#pragma once

#include "iStd.h"

//----------------------------------------------------------------------------
// enum
/*MapType*/
enum MapType {
	MapType_Nomal = 0,
	MapType_Boss,
	MapType_ItemBox,
	MapType_Shop,
};

/*MonsterType*/
enum MonsterType {
	MT_golemNomal = 0,
	MT_golemElete,
	MT_golemBoss,

	MT_max
};

/*Weapon Type*/
enum WeaponType {
	// melee
	WT_Hammer = 0,
	WT_Spear,
	WT_Cyclone,

	//range
	WT_BowGun,
	WT_MagicWand,

	WT_max,
};

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
// Pop Option
void createPopOption();
void freePopOption();
void showPopOption(bool show);
void drawPopOption(float dt);
bool keyPopOption(iKeyState stat, iPoint point);
