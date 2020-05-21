#pragma once

#include "iStd.h"

#define GOLEM_RATIO 0.2f
#define GOLEM_ATTACK_TIME 0.5f

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

	iPoint golemPos;
	iPoint drawGolemPos;
	iRect touchGolem;

	bool showHp;
	float showHpTime;
	bool takeDmg;
	float takeDmgTime;
	bool giveDmg;
	float giveDmgTime;
	bool hit;

	void createEnemyImg();
	void init(int stage);
	void drawShowHp(float dt);
	void takeDmgEnemy(float dt, float dmg);
	void takeDmgEffect(float dt);
	bool enemysAttack(float dt);

	//move func pointer
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

	iPoint golemPos;
	iPoint drawGolemPos;
	iRect touchGolem;

	bool showHp;
	float showHpTime;
	bool takeDmg;
	float takeDmgTime;
	bool giveDmg;
	float giveDmgTime;
	bool hit;

	void createEnemyImg();
	void init(int stage);
	void drawShowHp(float dt);
	void takeDmgEnemy(float dt, float dmg);
	void takeDmgEffect(float dt);
	bool enemysAttack(float dt);

	//move func pointer
};
extern EnemyEleteGolem** golemEletes;