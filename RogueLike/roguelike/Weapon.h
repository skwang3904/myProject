#pragma once

#include "iStd.h"
/*
- 장비
- 근접무기
- 원거리무기
*/

// 무기능력 외에 무기관련 관리하는 클래스
// 무기 생성, 파괴, 무기 드랍포지션 등,

#define MELEE_NUM 3
#define RANGE_NUM 1
#define TOTAL_WP_NUM MELEE_NUM + RANGE_NUM

typedef void (*Method_Combat)(float dt, iPoint dropP);
struct PlayerWP {
	void* wp;
	Method_Combat method;

	bool isMelee;
	iPoint pos;
	bool drop;
};
extern PlayerWP* PWP;

//------------------------------------------------------------------------

class Weapon
{
public:
	Weapon();
	virtual ~Weapon();

	void drawWeapon(float dt);

public:
	iPoint wDropPos[10];
};
extern Weapon* weapon;

iImage* infoFromMW(const char* info);

void meleeWeaponHit(float dt, float dmg, iRect& rt);
void hitMonster(float dt, float dmg, iRect& rt);
void hitObject(float dt, float dmg, iRect& rt);

