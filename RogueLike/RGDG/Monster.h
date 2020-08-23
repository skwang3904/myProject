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
	// 멤버 클래스에서 사용할때 
	// (this->*method[n])(); 형식으로 사용해야함
	// 자식클래스의 함수포인터에 접근하려면 
	// 자식클래스의 자신의 주소를 가르쳐줘야함
	void (Monster::*method[6])();
	
};

//---------------------------------------------------------------------------------------
// golemNomal
#define GOLEM_NOMAL_NUM 1
class GolemNomal : public Monster
{
public:
	GolemNomal(int index);
	virtual ~GolemNomal();

	virtual void paint(float dt, iPoint off);
	virtual void actionIdle();
	virtual void actionMove();
	virtual void actionMeleeAttack();
	virtual void actionRangeAttack();
	virtual void actionHurt();
	virtual void actionDeath();
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

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void loadMonster();
void freeMonster();
void drawMonster(float dt);

