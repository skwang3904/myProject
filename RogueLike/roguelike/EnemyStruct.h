#pragma once

#include "iStd.h"

#define GOLEM_NUM 10
#define GOLEM_ELETE_NUM 1
#define ALLENEMY_NUM GOLEM_NUM + GOLEM_ELETE_NUM

#define GOLEM_RATIO 0.2f
#define GOLEM_ATTACK_TIME 0.8f

#define GOLEM_ELETE_RATIO 0.4f
#define GOLEM_ELETE_ATTACK_TIME 0.5f

enum CharAction;
struct EnemyNomalGolem {
	iImage** img;

	float hp, _hp;
	float attackDmg;
	float _attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float moveSpeed;
	float reach;

	CharAction act;

	uint8 tileNumber;
	iPoint golemPos;
	iPoint drawGolemPos;
	iRect touchGolem;
	float texRatio;

	bool showHp;
	float showHpTime;
	bool takeDmg;
	float takeDmgTime;
	bool giveDmg;
	float giveDmgTime;
	bool hit;

	void createEnemyImg();
	void init(int stage);
	void drawEnemy(float dt);
	bool enemysAttack(float dt);
	void moveEnemyType1(float dt);

	void drawShowHp(float dt);
	void takeDmgEnemy(float dt, float dmg);
	void takeDmgEffect(float dt);
};
extern EnemyNomalGolem** golems;

struct EnemyEleteGolem {
	iImage** img;

	float hp, _hp;
	float attackDmg;
	float _attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float moveSpeed;
	float reach;

	CharAction act;

	uint8 tileNumber;
	iPoint golemPos;
	iPoint drawGolemPos;
	iRect touchGolem;
	float texRatio;

	bool showHp;
	float showHpTime;
	bool takeDmg;
	float takeDmgTime;
	bool giveDmg;
	float giveDmgTime;
	bool hit;

	void createEnemyImg();
	void init(int stage);
	void drawEnemy(float dt);
	bool enemysAttack(float dt);
	void moveEnemyType1(float dt);

	void drawShowHp(float dt);
	void takeDmgEnemy(float dt, float dmg);
	void takeDmgEffect(float dt);
};
extern EnemyEleteGolem** golemEletes;