#include "ProcData.h"

PlayerInfo playerInfo[2] = {
	{100.0f, 10.0f, 1.0f, 700.0f},
	{100.0f, 10.0f, 1.0f, 400.0f},
};

PlayerImageInfo playerImageInfo[5] = {
	{"assets/char/CharHead%d.png",	
	4, 2, false, 0.08f, iSizeMake(60, 45) * 2.0f},
	{"assets/char/CharBodyR%d.png",	
	1, 10, true, 0.1f, iSizeMake(64, 40) * 2.0f},
	{"assets/char/CharBodyUD%d.png",
	1, 10, true, 0.1f, iSizeMake(64, 40) * 2.0f},
	{"assets/char/CharGood%d.png",	
	1, 4, false, 0.3f, iSizeMake(97, 60) * 2.0f},
	{"assets/char/CharJump%d.png",	
	1, 4, false, 0.3f, iSizeMake(97, 60) * 2.0f},
};

//----------------------------------------------------------------

MonsterInfo monsterInfo[3] = {
	{	//GOLEM_NOMAL
		100.0f, 5.0f, 1.5f, 0.8f, 0.5f, 1.0f,
		100.0f, 300.0f, 50.0f, 100.0f 
	},
	{	//GOLEM_ELETE
		150.0f, 7.0f, 1.5f, 0.8f, 0.5f, 1.0f,
		150.0f, 300.0f, 50.0f, 100.0f
	},
	{	//GOLEM_BOSS
		200.0f, 10.0f, 1.0f, 0.6f, 0.3f, 1.0f,
		200.0f, 300.0f, 100.0f, 150.0f
	},
};

/*
struct MonsterImageInfo
{
	const char* strPath;
	int imgNum;
	float aniDt;
	int repeatNum;
	bool lastFrame;
};
*/

// [ MONSTER_IMG_NUM 6 ] in "Monster.h"
MonsterImageInfo golemNomalImage[6] = {
	{ "assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png",
		12, 0.08f, 0, false },
	{ "assets/monster/golem1/Walking/Golem_01_Walking_%d.png",
		18, 0.08f, 0, false },
	{ "assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png",
		12, monsterInfo[GOLEM_NOMAL]._attackSpeed / 12.0f, 1, false },
	{ "assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png",
		1, 0.08f, 1, false },
	{ "assets/monster/golem1/Hurt/Golem_01_Hurt_%03d.png",
		12, monsterInfo[GOLEM_NOMAL]._actionDt / 12.0f, 1, false },
	{ "assets/monster/golem1/Dying/Golem_01_Dying_%d.png",
		15, 0.08f, 1, true},
};

MonsterImageInfo golemBossImage[6] = {
	{ "assets/monster/golemBoss/golemBoss - idle/golem-idle-%c-%02d.png",
		20, 0.1f, 0, false},
	{ "assets/monster/golemBoss/golemBoss - move/golem-run-%c-%02d.png",
		24, 0.1f, 0, false},
	{ "assets/monster/golemBoss/golemBoss - attack/golem-attack-%c-%02d.png",
		28, monsterInfo[GOLEM_BOSS]._attackSpeed / (28 / 4), 1, false},
	{ "assets/monster/golemBoss/golemBoss - attack/golem-attack-%c-%02d.png",
		28, monsterInfo[GOLEM_BOSS]._attackSpeed / (28 / 4), 1, false},
	{ "assets/monster/golemBoss/golemBoss - hurt/golem-hurt-%c-%02d.png",
		12, monsterInfo[GOLEM_BOSS]._actionDt / (12 / 4), 1, false},
	{ "assets/monster/golemBoss/golemBoss - death/golem-death-%c-%02d.png",
		36, 0.15f, 1, true},
};

//----------------------------------------------------------------

WeaponInfo weaponInfo[2] = {
	{
		20.0f, 0.5f, 1.0f,
		90.0f, 0.0f
	},
	{
		50.0f, 0.5f, 1.0f,
		0.0f, 100.0f
	},
};