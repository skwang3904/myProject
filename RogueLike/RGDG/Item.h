#pragma once

#include "iStd.h"

#include "Common.h"

class Item : public Object
{
public:
	Item(int index, int8 mapNum, iPoint pos);
	virtual ~Item();

	virtual void paint(float dt, iPoint off);
	virtual void action(Object* obj);

public:
	float value;

	bool get;
};


void loadItem();
void freeItem();
void drawItem(float dt);