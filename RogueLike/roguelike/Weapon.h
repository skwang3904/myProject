#pragma once

#include "iStd.h"
/*
- 장비
- 근접무기
- 원거리무기
*/

// 무기능력 외에 무기관련 관리하는 클래스
// 무기 생성, 파괴, 무기 드랍포지션 등,

class Weapon
{
private:
	Weapon();

public:
	virtual ~Weapon();

	static Weapon* instance();

	//void drawWeapon(float dt);
	//void attactWeapon(Player* pc);

public:

	iPoint wDropPos[4];

	//int meleeNum;
	//int rangeNum;
	//int totalWeaponNum;

};

extern Weapon* weapon;
