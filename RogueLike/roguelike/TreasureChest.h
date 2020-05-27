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

class Chest
{
public:
	Chest(ChestType ct);
	~Chest();

	void openAni();
	void paint(float dt);

public:
	bool open;

	iPoint pos;
	int tileNumber;

	UseItem** items;
	PlayerWP* weapons;
};

