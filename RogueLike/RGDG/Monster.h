#pragma once

#include "iStd.h"

#include "Common.h"

#define GOLEM_NOMAL_NUM 2

enum MonsterState {
	monster_idle = 0,
	monster_move,
	monster_meleeAttack,
	monster_rangeAttack,
	monster_hurt,
	monster_death,
};
#if 0 // 이미지 번호
[0] = Idle;
[1] = Move;
[2] = MeleeAttack;
[3] = RangeAttack;
[4] = Hurt;
[5] = Death;
#endif

class Monster : public Object
{
public:
	Monster(int index);
	virtual ~Monster();

	virtual void paint(float dt, iPoint off) = 0;

	virtual void getDmg(float dmg) = 0;

	virtual void actionIdle(float) = 0;
	virtual void actionMove(float) = 0;
	virtual void actionMeleeAttack(float) = 0;
	virtual void actionRangeAttack(float) = 0;
	virtual void actionHurt(float) = 0;
	virtual void actionDeath(float) = 0;
	virtual void initOtherAct(int index) = 0;

public:
	// 멤버 클래스에서 사용할때 
	// (this->*method[n])(); 형식으로 사용해야함
	// 자식클래스의 함수포인터에 접근하려면 
	// 자식클래스의 자신의 주소를 가르쳐줘야함
	void (Monster::*method[6])(float);
	
	MonsterState state;
	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float moveSpeed;

	float attackDt, _attackDt;
	float getDmgDt, _getDmgDt;
	float lookDistance;
	float meleeDistance;
	float rangeDistance;
	
	float distance;
};
extern Monster** monster;
extern int monsterNum;

//---------------------------------------------------------------------------------------
// golemNomal
class GolemNomal : public Monster
{
public:
	GolemNomal(int index, int mapNum, iPoint pos);
	virtual ~GolemNomal();

	virtual void paint(float dt, iPoint off);
	virtual void getDmg(float dmg);

	virtual void actionIdle(float);
	virtual void actionMove(float);
	virtual void actionMeleeAttack(float);
	virtual void actionRangeAttack(float);
	virtual void actionHurt(float);
	virtual void actionDeath(float);
	virtual void initOtherAct(int index);

public:
	
};

//---------------------------------------------------------------------------------------
// golemElete

class GolemElete : public Monster
{
public:
	GolemElete(int index, int mapNum, iPoint pos);
	virtual ~GolemElete();

	virtual void paint(float dt, iPoint off);
	virtual void getDmg(float dmg) = 0;

	virtual void actionIdle(float) = 0;
	virtual void actionMove(float) = 0;
	virtual void actionMeleeAttack(float) = 0;
	virtual void actionRangeAttack(float) = 0;
	virtual void actionHurt(float) = 0;
	virtual void actionDeath(float) = 0;
	virtual void initOtherAct(int index) = 0;
public:

};

//---------------------------------------------------------------------------------------
// golemBoss

class GolemBoss : public Monster
{
public:
	GolemBoss(int index, int mapNum, iPoint pos);
	virtual ~GolemBoss();

	virtual void paint(float dt, iPoint off);
	virtual void getDmg(float dmg) = 0;

	virtual void actionIdle(float) = 0;
	virtual void actionMove(float) = 0;
	virtual void actionMeleeAttack(float) = 0;
	virtual void actionRangeAttack(float) = 0;
	virtual void actionHurt(float) = 0;
	virtual void actionDeath(float) = 0;
	virtual void initOtherAct(int index) = 0;
public:

};

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------

void loadMonster();
void freeMonster();
void drawMonster(float dt);

