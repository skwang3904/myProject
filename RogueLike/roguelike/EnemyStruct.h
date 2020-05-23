#pragma once

#include "iStd.h"

#define GOLEM_NUM 10
#define GOLEM_ELETE_NUM 1
#define ALLENEMY_NUM GOLEM_NUM + GOLEM_ELETE_NUM

#define GOLEM_RATIO 0.3f
#define GOLEM_ATTACK_TIME 0.8f

#define GOLEM_ELETE_RATIO 0.4f
#define GOLEM_ELETE_ATTACK_TIME 0.5f

//----------------------------------------------------------------------------------------

struct EnemyGolem;
typedef void (*Method_Idle_Pattern)(EnemyGolem* enm, float dt);
typedef void (*Method_Walk_Pattern)(EnemyGolem* enm, float dt);
typedef bool (*Method_Attack_Pattern)(EnemyGolem* enm, float dt);

enum EnemyType {
	golemNomal = 0,
	golemElete,
};

struct EnemyGolem {
	iImage** img;

	float hp, _hp;
	float attackDmg;
	float attackSpeed;
	float moveSpeed;
	float reach;
	float ratio;

	int tileNumber;
	iPoint golemPos;
	iPoint drawGolemPos;

	EnemyType enmtype;
	CharAction act;

	iRect touchGolem;

	bool showHp;
	float showHpTime;
	bool takeDmg;
	float takeDmgTime;
	bool giveDmg;
	float giveDmgTime;
	bool hit;

	void init(int stage);
	void paint(float dt);

	void drawShowHp(float dt);
	void takeDmgEnemy(float dt, float dmg);
	void takeDmgEffect(float dt);

	uint8 reverse;
	iPoint ATV = iPointZero;

	Method_Idle_Pattern methodIdle;
	Method_Walk_Pattern methodWalk;
	Method_Attack_Pattern methodAttack;


	float timer = 0.0f;
	bool test = false;
};

iImage** golemImg();
iImage** golemEleteImg();


extern EnemyGolem** golems;
extern EnemyGolem** golemEletes;