#pragma once

#include "iStd.h"

#define FIREBALL_NUM 30

#define alive_duration 10.0f
#define fire_timer 2.0f
extern iImage* imgChargeFire;

class FireBall
{
public:
	FireBall();
	~FireBall();

	void init(float d, float firetime, float speed, uint8 tileNum, iPoint& vlen, iPoint& pos);
	void paint(float dt);

	void setAngle();
	void setlimitRect(uint8 tileNum);

	bool hitFireBall(iRect& rt);

public:
	iImage* img;
	
	bool alive;
	float dmg;
	float speed;
	float duration;
	float timer;

	uint8 tileNumber;

	iPoint v;
	iPoint sp;
	iPoint posFireBall;
	iPoint drawFireBallPos;
	iRect touchRect;

	iRect limitRect;
};

void createEffect();
void freeEffect();
void drawEffect(float dt);

void projectileReflect(uint8 tile, iPoint& v, iPoint& pos, iPoint& mp);
