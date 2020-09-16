#pragma once

#include "iStd.h"

//----------------------------------------------------------------------------
// macro
#define SHOW_TOUCHRECT 0

#define SFX_NUM 9

#define AUDIO_HammerAttack	0
#define AUDIO_SpearAttack	1
#define AUDIO_CyclonAttack	2
#define AUDIO_EnemyHit		3
#define AUDIO_EnemyDeath	4
#define AUDIO_NextStageMove	5
#define AUDIO_ItemPickUp	6
#define AUDIO_MenuMouseOver	7
#define AUDIO_MenuSelected	8
#define AUDIO_GameMusic 9
#define AUDIO_INTRO		10
//----------------------------------------------------------------------------
// enum
/*MapType*/
enum MapType {
	MapType_Nomal = 0,
	MapType_Boss,
	MapType_ItemBox,
	MapType_Shop,
};

/*MonsterType*/
enum MonsterType {
	MT_golemNomal = 0,
	MT_golemElete,
	MT_golemBoss,

	MT_max
};

/*Weapon Type*/
enum WeaponType {
	// melee
	WT_Hammer = 0,
	WT_Spear,
	WT_Cyclone,

	//range
	WT_BowGun,
	WT_MagicWand,

	WT_max,
};

//----------------------------------------------------------------------------
// class

class Object
{
public:
	Object(int index, int8 mapNum, iPoint pos);
	virtual ~Object();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void drawShadow(float dt, iPoint off) = 0;
	virtual void action(Object* obj) = 0;

public:
	int index;
	bool alive;

	int imgNum;
	iImage** imgs;
	iImage* img;
	int8 mapNumber;

	iPoint position;
	iPoint vector;
	iSize touchSize;
	iRect touchRect;

	float prevHp;
	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float actionDt, _actionDt;

	float moveSpeed;
};

//----------------------------------------------------------------------------
// Pop Option
void createPopOption();
void freePopOption();
void showPopOption(bool show);
void drawPopOption(float dt);
bool keyPopOption(iKeyState stat, iPoint point);
