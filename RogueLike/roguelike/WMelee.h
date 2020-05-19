#pragma once

#include "iStd.h"
/*
- 근접무기
*/

extern int meleeNum;

struct meleeWeapon {
	iImage* img;

	bool melee;

	float attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float widthReach;
	float heightReach;

	float holdAngle;

	iPoint combatPosition;
	iRect hitBox;
	bool attackEnemy;

	void init(
		iImage* iImg,
		bool isMelee,
		float iAttackDmg,
		float iAttackSpeed,
		float iWidthReach,
		float iHeightReach,
		float holdAngle);
};

struct PlayerMW {
	meleeWeapon* mw;
	Method_Combat method;
};
extern PlayerMW PMW[MELEE_NUM];

void createMeleeWeapon();
void freeMeleeWeapon(PlayerMW* pmw);
void weaponPosition(meleeWeapon* mw, float dt, iPoint& wp);
void weaponVector(meleeWeapon* mw, float dt);
void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt);

void draw(meleeWeapon* mw, float dt, iPoint dropP);

//extern meleeWeapon** _meleeWP;

//extern meleeWeapon* nomalSword;
void nomalSwordMethod(float dt, iPoint dropP);

//extern meleeWeapon* nomalSpear;
void nomalSpearMethod(float dt, iPoint dropP);

//extern meleeWeapon* nomalCyclone;
void nomalCycloneMethod(float dt, iPoint dropP);