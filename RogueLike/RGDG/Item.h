#pragma once

#include "iStd.h"

#include "Common.h"

#define ITEM_CREATE_NUM 5

enum ItemList
{
	item_PotionSmall = 0,
	item_PotionMiddle,
	item_PotionLarge,

	item_max
};

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

public:
	bool get;
	float value;

	iPoint dropPosition;
	float dropHeight;
	iPoint targetPosition;
};

void loadItem();
void freeItem();
void drawItem(float dt);

class Monster;
void dropMonsterItem(Monster* m);