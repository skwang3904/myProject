#pragma once

#include "iStd.h"
/*
- 근접무기
*/

struct meleeWeapon;
typedef void (*Method_Combat)(float dt, iPoint p);
typedef bool (*Method_MeleeAttack)(float dt);

struct meleeStat {
	float attackDmg;
	float attackSpeed;
	float widthReach;
	float heightReach;
	float holdAngle;

	float atkRange;
	float atkAngle;
	iPoint atkRatio;
	int cycleCount;
};

struct Melee {
	meleeWeapon* mw;
	Method_Combat method;
	Method_MeleeAttack methodMelee;
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
	iPoint centerPos;

	float atkRange;
	float atkAngle;
	iPoint atkRatio;
	int cycleCount;

	iPoint sp;
	iPoint ep;

	iRect hitBox;
	bool attackEnemy;

	void init(const char* info, Melee* m);
	void draw(float dt, iPoint dropPos);

	iPoint setCenterPos();
	iRect setHitBox(iPoint& cp);

	Method_MeleeAttack methodMelee;
};


void createMeleeWeapon();
void freeMeleeWeapon();

//void draw(meleeWeapon* mw, float dt, iPoint dropP);