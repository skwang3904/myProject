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
extern FireBall** ball;

void createEffect();
void freeEffect();
void drawEffect(float dt);

void testFireBall();

void projectileReflect(uint8 tile, iPoint& v, iPoint& pos, iPoint& mp);
