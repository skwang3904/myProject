#pragma once

#include "iStd.h"

#include "Common.h"

void loadProjectile();
void freeProjectile();
void drawProjectile(float dt);

#define PROJECTILE_NUM 50
enum ProjectileKinds {
	Projectile_BowGunArrow = 0,
	Projectile_FireBall,

	Projectile_MAX,
};

class Weapon;
class Projectile : public Object
{
public:
	Projectile(int index, int8 mapNum, iPoint pos);
	virtual ~Projectile();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void drawShadow(float dt, iPoint off) = 0;
	virtual void action(Object* obj) = 0;
	virtual void addProjectile(Weapon* atk) = 0;
	static void fire(ProjectileKinds kinds, Weapon* atk);

public:

};

//---------------------------------------------------------------------------
// BowGunArrow
class BowGunArrow : public Projectile
{
public:
	BowGunArrow(int index, int8 mapNum, iPoint pos);
	virtual ~BowGunArrow();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj) ;
	virtual void addProjectile(Weapon* atk);

public:

};

//---------------------------------------------------------------------------
// FireBall
class FireBall : public Projectile
{
public:
	FireBall(int index, int8 mapNum, iPoint pos);
	virtual ~FireBall();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);
	virtual void addProjectile(Weapon* atk);

public:
	float delta, _delta;
};