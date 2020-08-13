#pragma once

#include "iStd.h"

/*
- 돈 /
- 소모아이템
- 체력회복 /
*/

#define ANIDT 0.5f

enum itemType{
	coin = 0,
	healing,
	powerUp,
	atkSpeedUp,
	moveSpeedUp,

	itemTypeMax
};

class UseItem 
{
public:
	UseItem(itemType it);
	~UseItem();

	void gainValue();
	bool dropAnimation(float dt);
	void getAnimation(float dt);

	void paint(float dt);

	//습득시 애니메이션

public:
	itemType type;
	iImage* img;

	bool alive;
	bool getItem;
	float value;

	iPoint sp;
	float dropAniDt;
	float dropAniHeight;
	float getAniDt;

	iPoint itemPos;
	iRect touchItem;
};

 //----------------------------------------------------------------------------

void createItemImg();
void freeItemImg();

class MonsterData;
void golemItems(MonsterData* enm);