#pragma once

#include "iStd.h"

#include "Common.h"

class Monster : public Object
{
public:
	Monster(int index);
	virtual ~Monster();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void actionIdle() = 0;
	virtual void actionMove() = 0;
	virtual void actionMeleeAttack() = 0;
	virtual void actionRangeAttack() = 0;
	virtual void actionHurt() = 0;
	virtual void actionDeath() = 0;
public:

};

//---------------------------------------------------------------------------------------
// golemNomal

class GolemNomal : public Monster
{
public:
	GolemNomal(int index);
	virtual ~GolemNomal();

	virtual void paint(float dt, iPoint off);
	virtual void actionIdle() = 0;
	virtual void actionMove() = 0;
	virtual void actionMeleeAttack() = 0;
	virtual void actionRangeAttack() = 0;
	virtual void actionHurt() = 0;
	virtual void actionDeath() = 0;
public:
	
};

//---------------------------------------------------------------------------------------
// golemElete

class GolemElete : public Monster
{
public:
	GolemElete(int index);
	virtual ~GolemElete();

	virtual void paint(float dt, iPoint off);
	virtual void actionIdle() = 0;
	virtual void actionMove() = 0;
	virtual void actionMeleeAttack() = 0;
	virtual void actionRangeAttack() = 0;
	virtual void actionHurt() = 0;
	virtual void actionDeath() = 0;
public:

};

//---------------------------------------------------------------------------------------
// golemBoss

class GolemBoss : public Monster
{
public:
	GolemBoss(int index);
	virtual ~GolemBoss();

	virtual void paint(float dt, iPoint off);
	virtual void actionIdle() = 0;
	virtual void actionMove() = 0;
	virtual void actionMeleeAttack() = 0;
	virtual void actionRangeAttack() = 0;
	virtual void actionHurt() = 0;
	virtual void actionDeath() = 0;
public:

};