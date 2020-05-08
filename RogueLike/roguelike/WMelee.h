#pragma once

#include "iStd.h"
/*
- 근접무기
*/

/*  Melee Weapon Info

-	Texture* tex;

	bool melee; // 1: 근접, 2: 원거리,
-	char name[64];
-
-	float attackDmg;
-	float attackSpeed;
-	float _attackSpeed;
-	float reach;
-
-	iPoint combatPosition;
-	iRect hitBox;

*/

extern int meleeNum;

struct meleeWeapon {
	iImage* img;

	bool melee;
	char name[64];

	float attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float reach;

	iPoint combatPosition;
	iRect hitBox;

	void init();
	void init(
		iImage* iImg,
		bool isMelee,
		char iName,
		float iAttackDmg,
		float iAttackSpeed,
		float _iAttackSpeed,
		float iReach,
		iPoint iCombatPosition,
		iRect iHitBox);

};

void createMeleeWeapon();
void freeMeleeWeapon();
iPoint weaponPosition(float dt, iPoint& wp);
void attackMelee(meleeWeapon* mw, float dt, bool drop, iPoint dropP);
//void attackMelee(meleeWeapon* wMelee, 'data...');

void draw(meleeWeapon* melee, float dt, bool drop, iPoint dropP);

extern meleeWeapon** _meleeWP;


extern meleeWeapon* nomalSword;
void nomalSwordMethod(float dt, bool drop, iPoint dropP);

extern meleeWeapon* nomalSpear;
void nomalSpearMethod(float dt, bool drop, iPoint dropP);