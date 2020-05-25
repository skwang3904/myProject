#pragma once

#include "iStd.h"
/*
- 근접무기
*/

//typedef void (*Method_Combat)(float dt, iPoint dropP);
struct meleeWeapon {
	iImage* infoImg;
	const char* infomation;
	
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
		const char* info,
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

	iPoint pos;
	bool drop;
};
extern PlayerMW PMW[MELEE_NUM];

void createMeleeWeapon();
void freeMeleeWeapon(PlayerMW* pmw);

void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt);

void draw(meleeWeapon* mw, float dt, iPoint dropP);

void nomalSwordMethod(float dt, iPoint dropP);
void nomalSpearMethod(float dt, iPoint dropP);
void nomalCycloneMethod(float dt, iPoint dropP);
