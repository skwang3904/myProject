#pragma once

#include "iStd.h"

/*
적 유닛 
( hp, 공격력, 디버프, 버프, 이동속도, 위치p, 목표위치tp, 
애니메이션, 히트판정)
- 기본유닛
- 보스 
*/

#define ENEMY_NUM 5

struct Enemy1{
	iImage* img;

	float hp, _hp;
	float attackDmg;
	float _attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float moveSpeed;
	float reach;

	iPoint Enemy1Position;
	iPoint drawEnemyPos;

	iRect touchEnemy1;
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
extern Enemy1** enemys;
#define TAKE_DMG_TIME 0.3f
#define SHOW_HP_TIME 1.0f

void createEnemy();
void freeEnemy();
void drawEnemy(float dt);

void moveEnemyType1(Enemy1* enm,float dt);
