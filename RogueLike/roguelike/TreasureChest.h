#pragma once

#include "iStd.h"

class UseItem;
struct PlayerWP;

enum ChestType {
	basic = 0,
	common,
	rare,
	unique,
};

class Chest;
typedef void (*Method_OpenChest)(Chest* me);
class Chest
{
public:
	Chest(ChestType ct);
	virtual ~Chest();

	void openAni(float dt);
	void paint(float dt);

	void setChest();

public:
	iImage* img;

	bool alive;
	bool open;

	int tileNumber;
	iPoint pos;
	iRect touchRect;

	UseItem** items;
	PlayerWP* weapons;

	Method_OpenChest openMethod;
};

void createChest();
void freeChest();
void drawChest(float dt);