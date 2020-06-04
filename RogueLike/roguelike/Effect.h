#pragma once

#include "iStd.h"

#define FIREBALL_NUM 30

#define alive_duration 10.0f
#define fire_timer 2.0f
extern iImage* imgChargeFire;

struct MapTile;

class FireBall
{
public:
	FireBall();
	~FireBall();

	void init(float d, float firetime, float speed, int tileNum, iPoint& vlen, iPoint& pos);
	void paint(float dt);

	void setAngle();
	void setlimitRect(int tileNum);

	bool hitFireBall(iRect& rt);

public:
	iImage* img;
	
	bool alive;
	float dmg;
	float speed;
	float duration;
	float timer;

	int tileNumber;

	iPoint v;
	iPoint sp;
	iPoint posFireBall;
	iPoint drawFireBallPos;
	iPoint touchPos;

	iRect limitRect;
};

void createEffect();
void freeEffect();
void drawEffect(float dt);

void projectileReflect(int tile, iPoint& v, iPoint& pos, iPoint& mp);
