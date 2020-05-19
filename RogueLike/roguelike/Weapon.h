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
public:
	Weapon();
	virtual ~Weapon();

	void drawWeapon(float dt);

public:
	//iArray* weaponArray;

	iPoint wDropPos[10];
};

extern Weapon* weapon;
