#pragma once

#include "iStd.h"

/*
- 돈 /
- 소모아이템
- 체력회복 /
*/
struct EnemyGolem;

enum itemType{
	coin = 0,
	healing,
	powerUp,
};

class UseItem 
{
public:
	UseItem(itemType it);
	~UseItem();

public:
	iImage* img;

	bool alive;
	float value;

	iPoint sp;
	float aniHeight;
	float aniDt;

	iPoint itemPos;
	iPoint drawitemPos;
	iRect touchItem;

	itemType type;

	void gainValue();
	bool animation(float dt);

	void paint(float dt);
	//습득시 애니메이션
};

 //----------------------------------------------------------------------------

void createItemImg();
void freeItemImg();

void golemItems(EnemyGolem* enm);