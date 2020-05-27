#pragma once

#include "iStd.h"

#define GOLEM_NUM 50
#define GOLEM_ELETE_NUM 1
#define ALLENEMY_NUM GOLEM_NUM + GOLEM_ELETE_NUM

#define GOLEM_IMG_NUM 6
#define GOLEM_RATIO 0.3f
#define GOLEM_MELEE_ATKTIME 1.0f
#define GOLEM_RANGE_ATKTIME 2.5f

#define GOLEM_ELETE_IMG_NUM 6
#define GOLEM_ELETE_RATIO 0.5f
#define GOLEM_ELETE_MELEE_ATKTIME 0.3f
#define GOLEM_ELETE_RANGE_ATKTIME 2.0f

//----------------------------------------------------------------------------------------

struct EnemyGolem;
typedef void (*Method_Idle_Pattern)(EnemyGolem* enm, float dt);
typedef void (*Method_Walk_Pattern)(EnemyGolem* enm, float dt);
typedef bool (*Method_Melee_Attack_Pattern)(EnemyGolem* enm, float dt);
typedef bool (*Method_Range_Attack_Pattern)(EnemyGolem* enm, float dt);

typedef void (*Method_Item)(EnemyGolem* enm);

enum EnemyType {
	golemNomal = 0,
	golemElete,
};

class UseItem;
struct FireBall;
struct EnemyGolem {
	iImage** img;

	float hp, _hp;
	float attackDmg;
	float meleeAtkSpeed;
	float rangeAtkSpeed;
	float moveSpeed;

	float meleeReach;
	float rangeReach;
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

	void init(int stage = 0);
	void paint(float dt);

	void drawShowHp(float dt);
	void takeDmgEnemy(float dt, float dmg);
	void takeDmgEffect(float dt);

	uint8 reverse;
	iPoint ATV = iPointZero;

	Method_Idle_Pattern methodIdle;
	Method_Walk_Pattern methodWalk;
	Method_Melee_Attack_Pattern methodMelee;
	Method_Range_Attack_Pattern methodRange;

	iImage* effectImg;
	FireBall** projectile;
	float rangeTime;
	float _rangeTime;

	UseItem** items;
	Method_Item methodead;
};

iImage** golemImg();
iImage** golemEleteImg();

extern EnemyGolem** golems;
extern EnemyGolem** golemEletes;