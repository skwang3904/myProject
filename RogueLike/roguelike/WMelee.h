#pragma once

#include "iStd.h"
/*
- 근접무기
*/

struct meleeWeapon;
typedef void (*Method_Combat)(float dt, iPoint p);

struct meleeStat {
	float attackDmg;
	float attackSpeed;
	float widthReach;
	float heightReach;
	float holdAngle;
};

struct Melee {
	meleeWeapon* mw;
	Method_Combat method;
	meleeStat stat;

	const char* strImg;
};

struct meleeWeapon {
	iImage* infoImg;
	const char* infomation;
	
	iImage* img;

	float attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float widthReach;
	float heightReach;

	float holdAngle;
	iPoint combatPosition;

	iRect hitBox;
	bool attackEnemy;

	void init(const char* info, Melee* m);
};


void createMeleeWeapon();
void freeMeleeWeapon();

void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt);

void draw(meleeWeapon* mw, float dt, iPoint dropP);