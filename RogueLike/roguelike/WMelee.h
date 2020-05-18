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
	float widthReach;
	float heightReach;

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
		float iWidthReach,
		float iHeightReach,
		float holdAngle,
		iPoint iCombatPosition,
		iRect iHitBox,
		bool iAttackEnemy);

};

void createMeleeWeapon();
void freeMeleeWeapon();
void weaponPosition(meleeWeapon* mw, float dt, iPoint& wp);
void weaponVector(meleeWeapon* mw, float dt);

void draw(meleeWeapon* mw, float dt, iPoint dropP);

extern meleeWeapon** _meleeWP;

extern meleeWeapon* nomalSword;
void nomalSwordMethod(float dt, iPoint dropP);

extern meleeWeapon* nomalSpear;
void nomalSpearMethod(float dt, iPoint dropP);

extern meleeWeapon* nomalCyclone;
void nomalCycloneMethod(float dt, iPoint dropP);