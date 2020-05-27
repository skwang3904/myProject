#pragma once

#include "iStd.h"
/*
- 원거리 무기
*/

struct rangeWeapon {
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
		iImage* iImg,
		float iAttackDmg,
		float iAttackSpeed,
		float iWidthReach,
		float iHeightReach,
		float iHoldAngle);
};

void createRangeWeapon();

void weaponPosAndRt(rangeWeapon* rw, iPoint& wcp, iPoint& centerP, iRect& rt);

void draw(rangeWeapon* rw, float dt, iPoint dropP);