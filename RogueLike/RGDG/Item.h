#pragma once

#include "iStd.h"

#include "Common.h"

#define ITEM_CREATE_NUM 20

class Monster;
class MapObject;
class Item : public Object
{
public:
	Item(int index, int8 mapNum, iPoint pos);
	virtual ~Item();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);
	virtual void aliveItem(Object* obj); // 수정

	void addItem(Object* obj, iPoint dropPos);

	static void dropMonsterItem(Monster* m);
	static void dropMapObjectItem(MapObject* mo);
public:
	bool get;
	float value;

	iPoint dropPosition;
	float dropHeight;
	iPoint targetPosition;
};
extern Item** item;
extern int itemNum;


void loadItem();
void freeItem();
void drawItem(float dt);
