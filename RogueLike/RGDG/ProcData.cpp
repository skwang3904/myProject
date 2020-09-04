#include "ProcData.h"

PlayerInfo playerInfo[2] = {
	{100.0f, 10.0f, 1.0f, 700.0f},
	{100.0f, 10.0f, 1.0f, 400.0f},
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
		200.0f, 10.0f, 1.0f, 0.6f, 0.3f, 0.5f,
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
		12, monsterInfo[GOLEM_NOMAL]._hurtDt / 12.0f, 1, false },
	{ "assets/monster/golem1/Dying/Golem_01_Dying_%d.png",
		15, 0.08f, 1, true},
};

MonsterImageInfo golemBossImage[6] = {
	{ "assets/monster/golemBoss/golemBoss - idle/golem-idle-%c-%02d.png",
		20, 0.08f, 0, false},
	{ "assets/monster/golemBoss/golemBoss - move/golem-run-%c-%02d.png",
		24, 0.08f, 0, false},
	{ "assets/monster/golemBoss/golemBoss - attack/golem-attack-%c-%d.png",
		28, monsterInfo[GOLEM_BOSS]._attackSpeed / (28 / 4), 1, true},
	{ "assets/monster/golemBoss/golemBoss - attack/golem-attack-%c-%d.png",
		28, monsterInfo[GOLEM_BOSS]._attackSpeed / (28 / 4), 1, true},
	{ "assets/monster/golemBoss/golemBoss - hurt/golem-hurt-%c-%d.png",
		12, monsterInfo[GOLEM_BOSS]._hurtDt, 1, false},
	{ "assets/monster/golemBoss/golemBoss - death/golem-death-%c-%02d.png",
		36, 0.08f, 1, true},
};

//----------------------------------------------------------------

WeaponInfo weaponInfo[1] = {
	{
		50.0f, 0.5f, 1.0f,
		90.0f, 0.0f
	},

};