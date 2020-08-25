#pragma once

#include "iStd.h"

#include "Common.h"

#define GOLEM_NOMAL_NUM 1

enum MonsterState {
	monster_idle = 0,
	monster_move,
	monster_meleeAttack,
	monster_rangeAttack,
	monster_hurt,
	monster_death,
};

class Monster : public Object
{
public:
	Monster(int index);
	virtual ~Monster();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void actionIdle(float) = 0;
	virtual void actionMove(float) = 0;
	virtual void actionMeleeAttack(float) = 0;
	virtual void actionRangeAttack(float) = 0;
	virtual void actionHurt(float) = 0;
	virtual void actionDeath(float) = 0;
public:
	// 멤버 클래스에서 사용할때 
	// (this->*method[n])(); 형식으로 사용해야함
	// 자식클래스의 함수포인터에 접근하려면 
	// 자식클래스의 자신의 주소를 가르쳐줘야함
	void (Monster::*method[6])(float);
	
	MonsterState state;
	bool alive;
	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float moveSpeed;

	float lookDistance;
	float meleeDistance;
	float rangeDistance;
	
	float distance;
};

//---------------------------------------------------------------------------------------
// golemNomal
class GolemNomal : public Monster
{
public:
	GolemNomal(int index);
	virtual ~GolemNomal();

	virtual void paint(float dt, iPoint off);
	virtual void actionIdle(float);
	virtual void actionMove(float);
	virtual void actionMeleeAttack(float);
	virtual void actionRangeAttack(float);
	virtual void actionHurt(float);
	virtual void actionDeath(float);
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
	virtual void actionIdle(float) = 0;
	virtual void actionMove(float) = 0;
	virtual void actionMeleeAttack(float) = 0;
	virtual void actionRangeAttack(float) = 0;
	virtual void actionHurt(float) = 0;
	virtual void actionDeath(float) = 0;
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
	virtual void actionIdle(float) = 0;
	virtual void actionMove(float) = 0;
	virtual void actionMeleeAttack(float) = 0;
	virtual void actionRangeAttack(float) = 0;
	virtual void actionHurt(float) = 0;
	virtual void actionDeath(float) = 0;
public:

};

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void loadMonster();
void freeMonster();
void drawMonster(float dt);

