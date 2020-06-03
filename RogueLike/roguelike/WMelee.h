#pragma once

#include "iStd.h"
/*
- 근접무기
*/

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

	void init(
		const char* info,
		float iAttackDmg,
		float iAttackSpeed,
		float iWidthReach,
		float iHeightReach,
		float iHoldAngle);
};

void createMeleeWeapon();
void freeMeleeWeapon();

void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt);

void draw(meleeWeapon* mw, float dt, iPoint dropP); // weapon으로 이사할것