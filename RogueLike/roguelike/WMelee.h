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

	float attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float reach;

	float holdAngle;

	iPoint combatPosition;
	iRect hitBox;
	bool attackEnemy;

	void init(); // 임시
	void init(
		iImage* iImg,
		bool isMelee,
		float iAttackDmg,
		float iAttackSpeed,
		float _iAttackSpeed,
		float iReach,
		float holdAngle,
		iPoint iCombatPosition,
		iRect iHitBox,
		bool iAttackEnemy);

};

void createMeleeWeapon();
void freeMeleeWeapon();
void weaponPosition(meleeWeapon* mw, float dt, iPoint& wp);
void weaponVector(meleeWeapon* mw, float dt);
// 사거리, 각도, 크기배율 
bool attackMelee(meleeWeapon* mw, float dt, bool att, float attTime,
	float iRange, float iAngle, float iRatio);

//void attackMelee(meleeWeapon* wMelee, 'data...');

void draw(meleeWeapon* melee, float dt, float holdAngle, bool drop, iPoint dropP);

extern meleeWeapon** _meleeWP;


extern meleeWeapon* nomalSword;
void nomalSwordMethod(float dt, bool drop, iPoint dropP);

extern meleeWeapon* nomalSpear;
void nomalSpearMethod(float dt, bool drop, iPoint dropP);