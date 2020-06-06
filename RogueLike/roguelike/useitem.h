#pragma once

#include "iStd.h"

/*
- 돈 /
- 소모아이템
- 체력회복 /
*/


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
	bool animation(float dt);

	void paint(float dt);

	//습득시 애니메이션

public:
	itemType type;
	iImage* img;

	bool alive;
	float value;

	iPoint sp;
	float aniHeight;
	float aniDt;

	iPoint itemPos;
	//iPoint drawitemPos;
	iRect touchItem;

};

 //----------------------------------------------------------------------------

void createItemImg();
void freeItemImg();

class MonsterData;
void golemItems(MonsterData* enm);