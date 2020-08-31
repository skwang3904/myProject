#pragma once

#include "iStd.h"

#include "Common.h"

class Weapon : public Object
{
public:
	Weapon(int index);
	virtual ~Weapon();

	virtual void paint(float dt, iPoint off);
	virtual void attack(float dt);
	bool getWeapon();


	void setPosition();
public:
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float attackDelay, _attackDelay;
	float attackAngle;
	float attackRange;

	float holdAngle;
	int anc;
	bool hit;
	bool get;
	// 공격력, 속도,
	// bool 데미지를 주었나
	// 플레이어가 주웠는지,
	// 공격할때 움직이는 각도, 거리
	// 
};

//2종류 : 망치, 창


void loadWeapon();
void freeWeapon();
void drawWeapon(float dt);