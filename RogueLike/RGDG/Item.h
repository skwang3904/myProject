#pragma once

#include "iStd.h"

#include "Common.h"

class Item : public Object
{
public:
	Item(int index);
	virtual ~Item();

	virtual void paint(float dt, iPoint off);
	void getItem();
public:
	float value;

	bool get;
	float getItemDt, _getItemDt;
};


void loadItem();
void freeItem();
void drawItem(float dt);