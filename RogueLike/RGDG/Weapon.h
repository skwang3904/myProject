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

	virtual void paint(float dt, iPoint off);

	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj) = 0;
	virtual bool attack(float dt) = 0;

	void setPosition();
	void attackMonster();

	void getWeapon();
	void addThisWeapon();
	void dropWeapon();

	void rootWeapon(iPoint pos);

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

	iPoint rootPos;
	float rootDt, _rootDt;
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

	virtual void action(Object* obj);

	virtual bool attack(float dt);

public:

};

//--------------------------------------------------------
// Spear
class Spear : public Weapon
{
public:
	Spear(int index, int8 mapNum, iPoint pos);
	virtual ~Spear();

	virtual void action(Object* obj);

	virtual bool attack(float dt);

public:

};

//--------------------------------------------------------
// Cyclone
class Cyclone : public Weapon
{
public:
	Cyclone(int index, int8 mapNum, iPoint pos);
	virtual ~Cyclone();

	virtual void action(Object* obj);

	virtual bool attack(float dt);

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

	virtual void action(Object* obj);

	virtual bool attack(float dt);

public:

};

//--------------------------------------------------------
// MagicWand
class MagicWand : public Weapon
{
public:
	MagicWand(int index, int8 mapNum, iPoint pos);
	virtual ~MagicWand();

	virtual void action(Object* obj);

	virtual bool attack(float dt);
public:

};

/*
부메랑처럼 움직임
	float ang = m(d, 0.0f, attackAngle);
	float r = (vector.x == 0.0f ? img->tex->height : img->tex->width);
	float ran = _sin(180 * d) * r;

	iPoint rp = iPointRotate(drawPos, iPointZero, ang);

	rp += position + vector * ran * 3;
	position = rp - drawPos;
	touchRect = iRectMake(rp.x - 15, rp.y - 15, 30, 30);
	
*/