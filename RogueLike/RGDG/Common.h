#pragma once

#include "iStd.h"

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
