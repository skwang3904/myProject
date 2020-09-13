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

MonsterInfo monsterInfo[MT_max] = {
	{	//GOLEM_NOMAL
		100.0f,		20.0f,
		5.0f,		3.0f,
		1.5f,		-0.1f,
		1.0f,		0.0f,
		100.0f,		10.0f,

		0.75f,		-0.05f,
		0.5f,		-0.1f,
		300.0f,		10.0f,
		50.0f,		60.0f,
		100.0f,		10.0f,

		2, 
		2, 
		{0, 1}
	},
	{	//GOLEM_ELETE
		100.0f,		20.0f,
		5.0f,		3.0f,
		1.5f,		-0.1f,
		1.0f,		0.0f,
		100.0f,		10.0f,

		0.75f,		-0.05f,
		0.5f,		-0.1f,
		300.0f,		10.0f,
		50.0f,		60.0f,
		100.0f,		10.0f,

		2,
		2,
		{0, 1}
	},
	{	//GOLEM_BOSS
		100.0f,		20.0f,
		5.0f,		3.0f,
		1.5f,		-0.1f,
		1.0f,		0.0f,
		100.0f,		10.0f,

		0.75f,		-0.05f,
		0.5f,		-0.1f,
		300.0f,		10.0f,
		50.0f,		60.0f,
		100.0f,		10.0f,

		2,
		2,
		{0, 1}
	},
};

float MonsterInfo::setMonsterStatus(float* base, int stage)
{
	return base[0] + base[1] * stage;
}

// [ MONSTER_IMG_NUM 6 ] in "Monster.h"
MonsterImageInfo golemNomalImage[6] = {
	{ "assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png",	12,
		0.08f, 0.0f, 0, false, iSizeMake(200, 150) },
	{ "assets/monster/golem1/Walking/Golem_01_Walking_%d.png", 18, 
		0.08f, 0.0f, 0, false, iSizeMake(200, 150) },
	{ "assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png", 12,
		monsterInfo[MT_golemNomal].attackSpeed / 12, monsterInfo[MT_golemNomal]._attackSpeed, 1, false, iSizeMake(200, 150) },
	{ "assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png", 12,
		monsterInfo[MT_golemNomal].attackSpeed / 12, monsterInfo[MT_golemNomal]._attackSpeed, 1, false, iSizeMake(200, 150) },
	{ "assets/monster/golem1/Hurt/Golem_01_Hurt_%03d.png", 12,
		monsterInfo[MT_golemNomal].actionDt / 12, monsterInfo[MT_golemNomal]._actionDt, 1, false, iSizeMake(200, 150) },
	{ "assets/monster/golem1/Dying/Golem_01_Dying_%d.png", 15,
		0.08f, 0.0f, 1, true, iSizeMake(200, 150)},
};

MonsterImageInfo golemBossImage[6] = {
	{ "assets/monster/golemBoss/golemBoss - idle/golem-idle-%c-%02d.png", 20,
		0.1f, 0.0f, 0, false, iSizeMake(378, 300) },
	{ "assets/monster/golemBoss/golemBoss - move/golem-run-%c-%02d.png", 24,
		0.1f, 0.0f, 0, false, iSizeMake(378, 300) },
	{ "assets/monster/golemBoss/golemBoss - attack/golem-attack-%c-%02d.png", 28,
		monsterInfo[MT_golemBoss].attackSpeed / (28 / 4), monsterInfo[MT_golemBoss]._attackSpeed, 1, false, iSizeMake(378, 300) },
	{ "assets/monster/golemBoss/golemBoss - attack/golem-attack-%c-%02d.png", 28,
		monsterInfo[MT_golemBoss].attackSpeed / (28 / 4), monsterInfo[MT_golemBoss]._attackSpeed, 1, false, iSizeMake(378, 300) },
	{ "assets/monster/golemBoss/golemBoss - hurt/golem-hurt-%c-%02d.png", 12,
		monsterInfo[MT_golemBoss].actionDt / (12 / 4), monsterInfo[MT_golemBoss]._actionDt, 1, false, iSizeMake(378, 300) },
	{ "assets/monster/golemBoss/golemBoss - death/golem-death-%c-%02d.png", 36,
		0.15f, 0.0f, 1, true, iSizeMake(378, 300) },
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

//----------------------------------------------------------------

ItemInfo itemInfo[3] = {
	{ 5.0f, 0.5f},
	{ 10.0f, 0.8f},
	{ 20.0f, 1.0f},
};

ItemImageInfo itemImageInfo[3] = {
	{ "assets/item/PotionSmall.png",
		1, 0.08f, iSizeMake(64, 64) },
	{ "assets/item/PotionMiddle.png",
		1, 0.08f, iSizeMake(64, 64) },
	{ "assets/item/PotionLarge.png",
		1, 0.08f, iSizeMake(64, 64) },
};
