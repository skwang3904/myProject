#pragma once

#include "iStd.h"
/*
- 원거리 무기
*/

class Arrow;
struct Range;
struct rangeWeapon {
	iImage* infoImg;
	const char* infomation;

	iImage* img;

	float fireDmg;
	float fireSpeed;
	float _fireSpeed;
	float widthReach;
	float heightReach;

	float holdAngle;
	iPoint combatPosition;

	iRect hitBox;
	bool attackEnemy;

	Arrow** arrow;
	void init(const char* info, Range* r);
};

struct rangeStat {
	float fireDmg;
	float fireSpeed;
	float widthReach;
	float heightReach;
	float holdAngle;
};

typedef void (*Method_Combat)(float dt, iPoint p);
struct Range {
	rangeWeapon* rw;
	Method_Combat method;
	rangeStat stat;

	const char* strImg;
};

void createRangeWeapon();
void freeRangeWeapon();

void weaponPosAndRt(rangeWeapon* rw, iPoint& wcp, iPoint& centerP, iRect& rt);

void draw(rangeWeapon* rw, float dt, iPoint dropP);