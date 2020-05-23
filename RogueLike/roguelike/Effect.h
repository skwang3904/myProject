#pragma once

#include "iStd.h"

struct ChargeFire {
	iImage* img;

	float duration;
	float ratio;
};

class FireBall
{
public:
	FireBall();
	~FireBall();

	void paint(float dt);

	void setAngle();
	void reflectWall();
	
public:
	iImage* img;
	
	bool alive;
	float dmg;
	float speed;
	float duration;

	uint8 tileNumber;

	uint8 reverse;
	float angle;
	
	iPoint v;
	iPoint sp;
	iPoint tp;
	iPoint posFireBall;
	iRect touchRect;
	
};

void createFireBallImg();
void freeFireBallImg();

void testFireBall();

extern FireBall** ball;