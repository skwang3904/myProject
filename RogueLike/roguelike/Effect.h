#pragma once

#include "iStd.h"

#define FIREBALL_NUM 30

extern iImage* imgChargeFire;

class FireBall
{
public:
	FireBall();
	~FireBall();

	void init(uint8 num, uint8 tileNum, iPoint& v, iPoint& pos);
	void paint(float dt);

	void setAngle();
	void setlimitRect(uint8 tileNum);
public:
	iImage* img;
	
	bool alive;
	float dmg;
	float speed;
	float duration;

	uint8 tileNumber;

	iPoint v;
	iPoint sp;
	iPoint posFireBall;
	iPoint drawFireBallPos;
	iRect touchRect;

	iRect limitRect;
};
extern FireBall** ball;


void createEffect();
void freeEffect();
void drawEffect(float dt);

void testFireBall();

void projectileReflect(uint8 tile, iPoint& v, iPoint& pos, iPoint& mp);
