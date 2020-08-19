#pragma once

//#include "iStd.h"

class MonsterData;
class UseItem;
typedef void (*Method_Idle_Pattern)(MonsterData* enm, float dt);
typedef void (*Method_Walk_Pattern)(MonsterData* enm, float dt);
typedef void (*Method_Hurt_Pattern)(MonsterData* enm, float dt);
typedef bool (*Method_Melee_Attack_Pattern)(MonsterData* enm, float dt);
typedef bool (*Method_Range_Attack_Pattern)(MonsterData* enm, float dt);

typedef void (*Method_Item)(MonsterData* enm);

enum MonsterType {
	golemNomal = 0,
	golemElete,
	golemBoss,
};

class MonsterData
{
	//몬스터 공통사항
	//1. 이미지
	//2. 액션타입
	//3. 아이템드랍
	//4. 기본스텟
public:
	MonsterData(MonsterType mt);
	virtual ~MonsterData();

	virtual void init() {};
	virtual void paint(float dt) {};
	virtual void drawShowHP(float dt);
	virtual void takeDmgEnemy(float dt, float dmg);
	virtual void takeDmgEffect(float dt);
public:
	// common data
	int imgDir;

	float hp, _hp;
	float attackDmg;
	float meleeAtkSpeed;
	float rangeAtkSpeed;
	float moveSpeed;
	float meleeReach;
	float rangeReach;
	float rangeTime;
	float _rangeTime;

	float ratio; //  img->ratio 에 들어갈 배율

	iImage** img;

	MonsterType enmtype;
	CharAction act;
	uint8 reverse;

	int tileNumber;
	iPoint enemyPos;
	iRect touchEnemy;

	bool showHp;
	float showHpTime;
	float _showHpTime;
	bool takeDmg;
	float takeDmgTime;
	float _takeDmgTime;
	bool atkMotion;
	float atkMotionTime;
	bool hit;

	iPoint ATV;
	float ATVlength;

	UseItem** items; // 임시 공통사용

public:
	// pattern
	Method_Idle_Pattern methodIdle = 0;
	Method_Walk_Pattern methodWalk = 0;
	Method_Hurt_Pattern methodHurt = 0;
	Method_Melee_Attack_Pattern methodMelee = 0;
	Method_Range_Attack_Pattern methodRange = 0;

	Method_Item methodead = 0;
};
extern MonsterData** totalMonster;
extern int monsterNum;

//----------------------------------------------------------------------------------------

#define GOLEM_NUM 20
#define GOLEM_ELETE_NUM 1
#define ALLENEMY_NUM GOLEM_NUM + GOLEM_ELETE_NUM

#define GOLEM_IMG_NUM 7
#define GOLEM_RATIO 0.3f
#define GOLEM_MELEE_ATKTIME 1.0f
#define GOLEM_RANGE_ATKTIME 2.5f

#define GOLEM_ELETE_IMG_NUM 7
#define GOLEM_ELETE_RATIO 0.5f
#define GOLEM_ELETE_MELEE_ATKTIME 0.3f
#define GOLEM_ELETE_RANGE_ATKTIME 2.0f

#define GOLEM_ITEM_NUM 5

#define GOLEM_BOSS_IMG_NUM 5
#define GOLEM_BOSS_IMG_DIRECTION 4
#define GOLEM_BOSS_RATIO 4.0f
#define GOLEM_ELETE_MELEE_ATKTIME 1.0f
#define GOLEM_ELETE_RANGE_ATKTIME 1.0f



class EGolem : public MonsterData
{
public:
	EGolem(MonsterType mt);
	virtual ~EGolem();

	void beginEGolem();
	void init();
	void paint(float dt);
public:
};

struct golemImgInfo {
	const char* strPath;
	int imgNum;
	int repeatNum;
	bool lastFrame;
	float aniDt;
	bool animation;
};
extern EGolem** golems;
extern EGolem** golemEletes;

void createGolemImg();
void freeGolemImg();

class EGolemBoss : public MonsterData
{
public:
	EGolemBoss(MonsterType mt);
	virtual ~EGolemBoss();

	void init();
	void paint(float dt);
public:

};
extern EGolemBoss* golemBOSS;
//----------------------------------------------------------------------------------------

