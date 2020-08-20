#pragma once

#include "iStd.h"


class Object
{
public:
	Object(int index);
	virtual ~Object();

	virtual void paint(float dt, iPoint off) = 0;
public:
	int index;
	iImage* img;
	int8 mapNumber;
	
	iPoint position;
	iPoint vector;
};

