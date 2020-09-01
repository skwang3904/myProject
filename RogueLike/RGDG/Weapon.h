#pragma once

#include "iStd.h"

#include "Common.h"

class Weapon : public Object
{
public:
	Weapon(int index);
	virtual ~Weapon();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void attack(float dt) = 0;
	virtual bool getWeapon() = 0;

	virtual void setPosition() = 0;

public:
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float attackDelay, _attackDelay;
	float attackAngle;
	float attackRange;

	float holdAngle;
	bool attacking;
	bool hit;
	bool get;
	
	iPoint drawPos;
};

//--------------------------------------------------------
// Hammer
class Hammer : public Weapon
{
public:
	Hammer(int index);
	virtual ~Hammer();

	virtual void paint(float dt, iPoint off);
	virtual void attack(float dt);
	virtual bool getWeapon();

	virtual void setPosition();

public:

};
//2종류 : 망치, 창


void loadWeapon();
void freeWeapon();
void drawWeapon(float dt);