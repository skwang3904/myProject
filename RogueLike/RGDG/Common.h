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
	int imgNum;
	iImage** imgs;
	iImage* img;
	int8 mapNumber;
	
	iPoint position;
	iPoint vector;
};

//----------------------------------------------------------------------------
// common enum

enum State {
	state_idle = 0,
	state_move,
	state_attack,
	state_death,

	// playerState
	state_evasion,
};