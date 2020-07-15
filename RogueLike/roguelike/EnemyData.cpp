#include "EnemyData.h"

#include "Room.h"
#include "Stage.h"

#include "EnemyActionPattern.h"

#include "Effect.h"
#include "useitem.h"

//golems
iImage** imgGolem;
iImage** imgGolemElete;

MonsterData** totalMonster = NULL;
int monsterNum = 0;

MonsterData::MonsterData(MonsterType mt)
{
	iImage** tmp = NULL;
	int num = 0;
	enmtype = mt;
	switch (enmtype)
	{
	case golemNomal:
	{
		tmp = imgGolem;
		num = GOLEM_IMG_NUM;
		break;
	}
	case golemElete:
	{
		tmp = imgGolemElete;
		num = GOLEM_ELETE_IMG_NUM;
		break;
	}
	default:
		printf("monster create error\n");
		break;
	}

	img = (iImage**)malloc(sizeof(iImage*) * num);
	for (int i = 0; i < num; i++)
		img[i] = tmp[i]->copy();
}

MonsterData::~MonsterData()
{
	int num = -1;
	switch (enmtype)
	{
	case golemNomal: num = GOLEM_IMG_NUM; break;
	case golemElete: num = GOLEM_ELETE_IMG_NUM; break;
	default: printf("monster delete error\n"); break;
	}

	for (int i = 0; i < num; i++)
		delete img[i];
	free(img);

	for (int i = 0; i < itemTypeMax; i++)
		delete items[i];
	free(items);
}

void MonsterData::drawShowHP(float dt)
{
	showHpTime += dt;
	if (showHpTime > _showHpTime)
	{
		showHp = false;
		showHpTime = 0.0f;
	}
	iPoint drawPos = enemyPos + SET_DRAW_OFF;
	iRect rt = iRectMake(drawPos.x + img[0]->tex->width * ratio * 0.25f, drawPos.y,
		img[0]->tex->width * ratio * 0.5f, 30);
	setRGBA(0, 0, 0, 1);
	fillRect(rt);
	setRGBA(0, 1, 0, 1);
	fillRect(rt.origin.x, rt.origin.y, rt.size.width * hp / _hp, rt.size.height);
	setRGBA(1, 1, 1, 1);

}

void MonsterData::takeDmgEnemy(float dt, float dmg)
{
	if (takeDmg == false)
	{
		hp -= dmg;
		takeDmg = true;
		showHp = true;
		if (act != meleeAtk && act != rangeAtk)
			act = hurt;

		audioPlay(SND_ENEMY_HIT);
	}
}

void MonsterData::takeDmgEffect(float dt)
{
	//Texture* tex = img[0]->tex;
	//iPoint drawPos = enemyPos + SET_DRAW_OFF;
	//setRGBA(0, 0, 0, linear(takeDmgTime / _takeDmgTime, 1.0f, 0.0f));
	//drawImage(tex, drawPos.x, drawPos.y,
	//	0, 0, tex->width, tex->height,
	//	TOP | LEFT, ratio, ratio, 2, 0, reverse);
	//setRGBA(1, 1, 1, 1);

	takeDmgTime += dt;
	if (takeDmgTime > _takeDmgTime)
	{
		takeDmgTime = 0.0f;
		takeDmg = false;
	}
}

//--------------------------------------------------------------------------
// golems

EGolem** golems;
EGolem** golemEletes;

EGolem::EGolem(MonsterType mt) : MonsterData(mt)
{
	init();
}

EGolem::~EGolem()
{
	// 임시 공통 사용

}

void EGolem::beginEGolem()
{
	// 임시 공통사용 아이템
	// 사용안하는 함수
	init();

	itemType it[itemTypeMax] = { coin, healing, powerUp, atkSpeedUp, moveSpeedUp };
	items = (UseItem**)malloc(sizeof(UseItem*) * itemTypeMax);
	for (int i = 0; i < itemTypeMax; i++)
		items[i] = new UseItem(it[i]);
}

void EGolem::init()
{
	uint8 s = stage;
	switch (enmtype)
	{
	case golemNomal:
	{
		hp = _hp = 50 + s * 30;
		attackDmg = 5 + s * 3;
		meleeAtkSpeed = GOLEM_MELEE_ATKTIME - s * GOLEM_MELEE_ATKTIME * 0.05f;
		rangeAtkSpeed = GOLEM_RANGE_ATKTIME - s * GOLEM_RANGE_ATKTIME * 0.05f;
		moveSpeed = 100 + s * 20;

		meleeReach = 100;
		rangeReach = 200;
		rangeTime = 0;
		_rangeTime = 0;

		ratio = GOLEM_RATIO;

		methodIdle = IdleEyeBlink;
		methodWalk = WalkToPlayer;
		methodHurt = commonHurt;
		methodMelee = commonAttack;
		methodRange = MethodFalse;

		methodead = golemItems;

		break;
	}
	case golemElete:
	{
		hp = _hp = 100 + s * 50;
		attackDmg = 10 + s * 5;
		meleeAtkSpeed = GOLEM_ELETE_MELEE_ATKTIME - s * GOLEM_ELETE_MELEE_ATKTIME * 0.05f;
		rangeAtkSpeed = GOLEM_ELETE_RANGE_ATKTIME - s * GOLEM_ELETE_RANGE_ATKTIME * 0.05f;
		moveSpeed = 150 + s * 20;

		meleeReach = 150;
		rangeReach = 300;
		rangeTime = 0;
		_rangeTime = 3.0f;

		ratio = GOLEM_ELETE_RATIO;

		methodIdle = IdleEyeBlink;
		methodWalk = WalkToPlayer;
		methodHurt = commonHurt;
		methodMelee = commonAttack;
		methodRange = MethodFalse;

		methodead = golemItems;

		break;
	}
	default:
		printf("golem init error\n");
		break;
	}

	act = idle;
	reverse = REVERSE_NONE;

	tileNumber = 0;
	enemyPos = iPointZero;
	touchEnemy = iRectZero;

	showHp = false;
	showHpTime = 0.0f;
	_showHpTime = 1.0f;
	takeDmg = false;
	takeDmgTime = 0.0f;
	_takeDmgTime = 0.3f;
	atkMotion = false;
	atkMotionTime = 0.0f;

	hit = false;
	ATV = iPointZero;

	itemType it[itemTypeMax] = { coin, healing, powerUp, atkSpeedUp, moveSpeedUp };
	items = (UseItem**)malloc(sizeof(UseItem*) * itemTypeMax);
	for (int i = 0; i < itemTypeMax; i++)
		items[i] = new UseItem(it[i]);
}

void EGolem::paint(float dt)
{
	bool mapPos = (tileNumber == pc->tileNumber);

	if (act == dying || act == dead)
	{
		for (int i = 0; i < GOLEM_ITEM_NUM; i++)
			items[i]->paint(dt);

		if (act == dead)
			return;
	}

	Texture* tex = img[0]->tex;
	iRect rt = iRectMake(enemyPos.x + tex->width * ratio * 0.25f,
		enemyPos.y + tex->height * ratio * 0.25f,
		tex->width * ratio * 0.5f,
		tex->height * ratio * 0.75f);
	iRect rt1 = iRectMake(enemyPos.x,
		enemyPos.y,
		tex->width * ratio,
		tex->height * ratio);

	iPoint drawPos = enemyPos + SET_DRAW_OFF;
	touchEnemy = rt;

	rt.origin += SET_DRAW_OFF;
	rt1.origin += SET_DRAW_OFF;
	setRGBA(0, 0, 0, 1);
	setLineWidth(2);
	drawRect(rt);
	setRGBA(1, 0, 1, 1);
	drawRect(rt1);
	setRGBA(1, 1, 1, 1);
	setLineWidth(1);

	if (hp > 0.0f)
	{
		methodIdle(this, dt);
		methodHurt(this, dt);

		if (takeDmg)
			takeDmgEffect(dt);

		if (showHp)
			drawShowHP(dt);
		if (mapPos)
		{
			if ((methodRange(this, dt) == false) &&
				(methodMelee(this, dt) == false))
			{
				methodWalk(this, dt);
			}
		}
		else
			act = idle;
	}
	else
	{
		if (act != dying)
		{
			act = dying;
			audioPlay(SND_ENEMY_DEAD);
			img[4]->startAnimation();
			methodead(this);
			img[4]->reverse = reverse;
		}

		img[4]->paint(dt, drawPos);

		if (img[4]->animation == false)
		{
			enemyPos = iPointZero;
			touchEnemy = iRectZero;
			act = dead;
			if (this == golemEletes[0])
				nextDoor = tileNumber;
			tileNumber = 0;
		}
	}

}

//----------------------------------------------------------------------------------------

golemImgInfo eii[GOLEM_IMG_NUM + GOLEM_ELETE_IMG_NUM] = {
	{ "assets/monster/golem1/Idle/Golem_01_Idle_%d.png",
		12, 1, false, 0.08f },
	{ "assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png",
		12, 1, false, 0.08f },
	{ "assets/monster/golem1/Walking/Golem_01_Walking_%d.png",
		18, 0, false, 0.05f },
	{ "assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png",
		12, 1, false, GOLEM_MELEE_ATKTIME / 12.0f},
	{ "assets/monster/golem1/Dying/Golem_01_Dying_%d.png",
		15, 1, true, 0.07f},
	{ "assets/monster/golem1/Taunt/Golem_01_Taunt_%03d.png",
		18, 1, false, GOLEM_RANGE_ATKTIME / 18.0f},
	{ "assets/monster/golem1/Hurt/Golem_01_Hurt_%03d.png",
		12, 1, false, 0.08f},
	////////////////////////////////////////////////////
	{ "assets/monster/golemElete/Idle/Golem_03_Idle_%03d.png",
		12, 1, false, 0.08f},
	{ "assets/monster/golemElete/Idle Blink/Golem_03_Idle Blinking_%03d.png",
		12, 1, false, 0.08f},
	{ "assets/monster/golemElete/Walking/Golem_03_Walking_%03d.png",
		18, 0, false, 0.05f},
	{ "assets/monster/golemElete/Attacking/Golem_03_Attacking_%03d.png",
		12, 1, false, GOLEM_ELETE_MELEE_ATKTIME / 12.0f},
	{ "assets/monster/golemElete/Dying/Golem_03_Dying_%03d.png",
		15, 1, true, 0.07f},
	{ "assets/monster/golemElete/Taunt/Golem_03_Taunt_%03d.png",
		18, 1, false, GOLEM_ELETE_RANGE_ATKTIME / 18.0f},
	{ "assets/monster/golemElete/Hurt/Golem_03_Hurt_%03d.png",
		12, 1, false, 0.08f},
};

void createGolemImg()
{
	int total = GOLEM_IMG_NUM + GOLEM_ELETE_IMG_NUM;
	iImage** imgG = (iImage**)malloc(sizeof(iImage*) * total);
	for (int i = 0; i < total; i++)
	{
		golemImgInfo* e = &eii[i];

		iImage* img = new iImage();
		for (int j = 0; j < e->imgNum; j++)
		{
			Texture* tex = createImage(e->strPath, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->_repeatNum = e->repeatNum;
		img->_aniDt = e->aniDt;
		img->lastFrame = e->lastFrame;
		img->ratio = (i < GOLEM_IMG_NUM ? GOLEM_RATIO : GOLEM_ELETE_RATIO);

		imgG[i] = img;
	}
	imgG[2]->animation = true;
	imgG[GOLEM_IMG_NUM + 2]->animation = true;

	imgGolem = imgG;
	imgGolemElete = &imgG[GOLEM_IMG_NUM];
}

void freeGolemImg()
{
	int total = GOLEM_IMG_NUM + GOLEM_ELETE_IMG_NUM;
	for (int i = 0; i < total; i++)
		delete imgGolem[i];
	free(imgGolem);
	//free(imgGolemElete);
}

