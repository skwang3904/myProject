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
	dmgUp,
};

class UseItem 
{
public:
	UseItem(itemType it);
	~UseItem();

public:
	iImage* img;

	float value;
	
	iPoint itemPos;
	iPoint drawitemPos;
	iRect touchItem;

	itemType type;

	void gainValue();

	void paint(float dt);
	//습득시 애니메이션
};

 //----------------------------------------------------------------------------

void createItemImg();
void freeItemImg();

void golemItems(EnemyGolem* enm);