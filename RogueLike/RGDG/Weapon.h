#pragma once

#include "iStd.h"

#include "Common.h"

void loadWeapon();
void freeWeapon();
void drawWeapon(float dt);

class Weapon : public Object
{
public:
	Weapon(int index, int8 mapNum, iPoint pos);
	virtual ~Weapon();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void drawShadow(float dt, iPoint off) = 0;
	virtual void action(Object* obj) = 0;

	virtual bool attack(float dt) = 0;
	virtual void setPosition() = 0;

	void attackMonster();
	void getWeapon();
	void addThisWeapon();
	void dropWeapon();

public:
	float attackDt;
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

/* Melee Weapon */
//--------------------------------------------------------
// Hammer
class Hammer : public Weapon
{
public:
	Hammer(int index, int8 mapNum, iPoint pos);
	virtual ~Hammer();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
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
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

	virtual bool attack(float dt);
	virtual void setPosition();

public:

};

//--------------------------------------------------------
// Cyclone
class Cyclone : public Weapon
{
public:
	Cyclone(int index, int8 mapNum, iPoint pos);
	virtual ~Cyclone();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

	virtual bool attack(float dt);
	virtual void setPosition();

public:

};

/* Range Weapon */
//--------------------------------------------------------
// BowGun
class BowGun : public Weapon
{
public:
	BowGun(int index, int8 mapNum, iPoint pos);
	virtual ~BowGun();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

	virtual bool attack(float dt);
	virtual void setPosition();

public:

};

//--------------------------------------------------------
// MagicWand
class MagicWand : public Weapon
{
public:
	MagicWand(int index, int8 mapNum, iPoint pos);
	virtual ~MagicWand();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

	virtual bool attack(float dt);
	virtual void setPosition();

public:

};