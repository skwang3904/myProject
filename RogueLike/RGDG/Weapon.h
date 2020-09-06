#pragma once

#include "iStd.h"

#include "Common.h"

class Weapon : public Object
{
public:
	Weapon(int index, int8 mapNum, iPoint pos);
	virtual ~Weapon();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void action(Object* obj) = 0;
	virtual bool attack(float dt) = 0;
	virtual void setPosition() = 0;

	static void getWeapon(Weapon* me);
	static void dropWeapon(Weapon* me);

public:
	float attackDelay, _attackDelay;
	float attackAngle;
	float attackRange;

	float holdAngle;
	bool attacking;
	bool hit;
	bool get;
	
	iPoint drawPos;
};
extern Weapon** weapon;
extern int weaponNum;
//--------------------------------------------------------
// Hammer
class Hammer : public Weapon
{
public:
	Hammer(int index, int8 mapNum, iPoint pos);
	virtual ~Hammer();

	virtual void paint(float dt, iPoint off);
	virtual void action(Object* obj);

	virtual bool attack(float dt);
	virtual void setPosition();

public:

};

//--------------------------------------------------------
// Spear
class Spear : public Weapon
{
public:
	Spear(int index, int8 mapNum, iPoint pos);
	virtual ~Spear();

	virtual void paint(float dt, iPoint off);
	virtual void action(Object* obj);

	virtual bool attack(float dt);
	virtual void setPosition();

public:

};


void loadWeapon();
void freeWeapon();
void drawWeapon(float dt);