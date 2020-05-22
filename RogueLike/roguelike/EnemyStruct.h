#pragma once

#include "iStd.h"

#define GOLEM_NUM 10
#define GOLEM_ELETE_NUM 1
#define ALLENEMY_NUM GOLEM_NUM + GOLEM_ELETE_NUM

#define GOLEM_RATIO 0.3f
#define GOLEM_ATTACK_TIME 0.8f

#define GOLEM_ELETE_RATIO 0.4f
#define GOLEM_ELETE_ATTACK_TIME 0.5f


//struct EnemyNomalGolem {
//	iImage** img;
//
//	float hp, _hp;
//	float attackDmg;
//	float _attackDmg;
//	float attackSpeed;
//	float _attackSpeed;
//	float moveSpeed;
//	float reach;
//
//	CharAction act;
//
//	uint8 tileNumber;
//	iPoint golemPos;
//	iPoint drawGolemPos;
//	iRect touchGolem;
//	float texRatio;
//
//	bool showHp;
//	float showHpTime;
//	bool takeDmg;
//	float takeDmgTime;
//	bool giveDmg;
//	float giveDmgTime;
//	bool hit;
//
//	void createEnemyImg();
//	void init(int stage);
//	void paint(float dt);
//	bool enemysAttack(float dt);
//	bool attackPattern1(float dt, iPoint v, iPoint texHalf, uint8& reverse);
//	bool attackPattern2(float dt, iPoint v, iPoint texHalf, uint8& reverse);
//
//	void moveEnemyType1(float dt);
//
//	void drawShowHp(float dt);
//	void takeDmgEnemy(float dt, float dmg);
//	void takeDmgEffect(float dt);
//
//	int reverse = REVERSE_NONE;
//	iPoint ATV = iPointZero;
//};
////extern EnemyNomalGolem** golems;
//
//struct EnemyEleteGolem {
//	iImage** img;
//
//	float hp, _hp;
//	float attackDmg;
//	float _attackDmg;
//	float attackSpeed;
//	float _attackSpeed;
//	float moveSpeed;
//	float reach;
//
//	CharAction act;
//
//	uint8 tileNumber;
//	iPoint golemPos;
//	iPoint drawGolemPos;
//	iRect touchGolem;
//	float texRatio;
//
//	bool showHp;
//	float showHpTime;
//	bool takeDmg;
//	float takeDmgTime;
//	bool giveDmg;
//	float giveDmgTime;
//	bool hit;
//
//	void createEnemyImg();
//	void init(int stage);
//	void paint(float dt);
//	bool enemysAttack(float dt);
//	void moveEnemyType1(float dt);
//
//	void drawShowHp(float dt);
//	void takeDmgEnemy(float dt, float dmg);
//	void takeDmgEffect(float dt);
//
//	int reverse = REVERSE_NONE;
//	iPoint ATV = iPointZero;
//};
////extern EnemyEleteGolem** golemEletes;


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

	uint8 tileNumber;
	iPoint golemPos;
	iPoint drawGolemPos;

	EnemyType enmtype;
	CharAction act = idle;

	iRect touchGolem = iRectZero;

	bool showHp = false;
	float showHpTime = 0.0f;
	bool takeDmg = false;
	float takeDmgTime = 0.0f;
	bool giveDmg = false;
	float giveDmgTime = 0.0f;
	bool hit = false;

	void createEnemyImg();
	void init(int stage);
	void paint(float dt);

	void drawShowHp(float dt);
	void takeDmgEnemy(float dt, float dmg);
	void takeDmgEffect(float dt);

	uint8 reverse = REVERSE_NONE;
	iPoint ATV = iPointZero;

	Method_Idle_Pattern methodIdle;
	Method_Walk_Pattern methodWalk;
	Method_Attack_Pattern methodAttack;
};

void createEnemyImg();

extern EnemyGolem** golems;
extern EnemyGolem** golemEletes;