#pragma once

#include "iStd.h"

//----------------------------------------------------------------------------
// macro


//----------------------------------------------------------------------------
// class

class Object
{
public:
	Object(int index);
	virtual ~Object();

	virtual void paint(float dt, iPoint off) = 0;
public:
	int index;
	bool alive;

	int imgNum;
	iImage** imgs;
	iImage* img;
	int8 mapNumber;
	
	iPoint position;
	iPoint vector;

	iRect touchRect;
};
