#include "EnemyData.h"

#include "Room.h"
#include "Stage.h"

#include "EnemyActionPattern.h"

#include "Effect.h"
#include "useitem.h"

//golems
iImage** imgGolem;
iImage** imgGolemElete;
iImage** imgGolemBoss;

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
	case golemBoss:
	{
		tmp = imgGolemBoss;
		num = GOLEM_BOSS_IMG_NUM * GOLEM_BOSS_IMG_DIRECTION;
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

	if (items)
	{
		for (int i = 0; i < itemTypeMax; i++)
			delete items[i];
		free(items);
	}
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

float test = 0.1f;
void MonsterData::takeDmgEnemy(float dt, float dmg)
{
	if (takeDmg == false)
	{
		hp -= dmg;
		if (hp < 0.0f)
		{
			act = Act_dying;
			// death audio
			return;
		}

		{
			if (dmg < 5)	test = 0.1f;
			else			test = 1.0f;
		}

		takeDmg = true;
		showHp = true;
		if (act != Act_meleeAtk && act != Act_rangeAtk)
			act = Act_hurt;


		audioPlay(SND_ENEMY_HIT);
	}
}

void MonsterData::takeDmgEffect(float dt)
{
	takeDmgTime += dt;
	if (takeDmgTime > _takeDmgTime * test)
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
	beginEGolem();
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

	act = Act_idle;
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
}

void EGolem::paint(float dt)
{
	bool mapPos = (tileNumber == pc->tileNumber);

	if (act == Act_dying || act == Act_dead)
	{
		for (int i = 0; i < GOLEM_ITEM_NUM; i++)
			items[i]->paint(dt);

		if (act == Act_dead)
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
			act = Act_idle;
	}
	else
	{
		if (img[4]->animation == false)
		{
			audioPlay(SND_ENEMY_DEAD);
			img[4]->startAnimation();
			methodead(this);
			img[4]->reverse = reverse;
		}

		img[4]->paint(dt, enemyPos + SET_DRAW_OFF);

		if (img[4]->animation == false)
		{
			enemyPos = iPointZero;
			touchEnemy = iRectZero;
			act = Act_dead;
			tileNumber = 0;
		}
	}

}

//----------------------------------------------------------------------------------------

EGolemBoss* golemBOSS;
EGolemBoss::EGolemBoss(MonsterType mt) : MonsterData(mt)
{
	init();
}

EGolemBoss::~EGolemBoss()
{
}

void EGolemBoss::init()
{
	uint8 s = stage;

	hp = _hp = 300 + s * 50;
	attackDmg = 10 + s * 5;
	meleeAtkSpeed = GOLEM_ELETE_MELEE_ATKTIME - s * GOLEM_ELETE_MELEE_ATKTIME * 0.5f;
	rangeAtkSpeed = GOLEM_ELETE_RANGE_ATKTIME - s * GOLEM_ELETE_RANGE_ATKTIME * 0.5f;
	moveSpeed = 150 + s * 20;

	meleeReach = 150;
	rangeReach = 300;
	rangeTime = 0;
	_rangeTime = 3.0f;

	ratio = GOLEM_BOSS_RATIO;

	methodIdle = IdleBoss;
	methodWalk = WalkBossToPlayer;
	methodHurt = commonHurtBoss;
	methodMelee = commonAttackBoss;
	methodRange = MethodFalse;

	//methodead = golemItems;

	act = Act_idle;
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

	imgDir = 0;
}

void EGolemBoss::paint(float dt)
{
	bool mapPos = (tileNumber == pc->tileNumber);
	if (mapPos)
	{
		Texture* tex = img[0]->tex;
		iRect rt = iRectMake(enemyPos.x + tex->width * ratio * 0.25f,
			enemyPos.y + tex->height * ratio * 0.25f,
			tex->width * ratio * 0.5f,
			tex->height * ratio * 0.75f);
		iRect rt1 = iRectMake(enemyPos.x,
			enemyPos.y,
			tex->width * ratio,
			tex->height * ratio);

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


		if (act == Act_idle || act == Act_walking)
		{
			iImage* img = this->img[0];
			iPoint dir = iPointVector((pc->playerPosition + HALF_OF_TEX_POINT)
				- (enemyPos + iPointMake(img->tex->width * ratio * 0.5f, img->tex->height * ratio * 0.5f)));

			float angle = iPointAngle(iPointMake(1, 0), iPointZero, dir);
			if (angle < 45 || angle > 315) imgDir = 2;
			else if (angle > 45 && angle < 135) imgDir = 0;
			else if (angle > 135 && angle < 225) imgDir = 1;
			else if (angle > 225 && angle < 315) imgDir = 3;

			iPoint et = iPointMake(img->tex->width * ratio / 2,
				img->tex->height * ratio / 2);

			ATV = (pc->playerPosition + HALF_OF_TEX_POINT)
				- (enemyPos + et);
			ATVlength = iPointLength(ATV);

			if (ATVlength < meleeReach)
				act = Act_meleeAtk;
			else if (ATVlength < rangeReach)
				//act = Act_rangeAtk;
				act = Act_walking;
				
			else if (ATVlength < 600)
				act = Act_walking;
			else
				act = Act_idle;

		}

		switch (act)
		{
		case Act_idle:
		{
			methodIdle(this, dt);
			break;
		}
		case Act_attack:
			break;
		case Act_meleeAtk:
		{
			methodMelee(this, dt);
			break;
		}
		case Act_rangeAtk:
		{
			methodRange(this, dt);
			break;
		}
		case Act_falling:
			break;
		case Act_evasion:
			break;
		case Act_walking:
		{
			methodWalk(this, dt);
			break;
		}
		case Act_hurt:
		{
			methodHurt(this, dt);
			break;
		}
		case Act_dying:
		{
			int imgD = GOLEM_BOSS_IMG_DIRECTION * 3 + imgDir;
			iImage* img = this->img[imgD];
			if (img->animation == false)
			{
				img->startAnimation();
			}

			img->paint(dt, enemyPos + SET_DRAW_OFF);

			if (img->animation == false)
			{
				enemyPos = iPointZero;
				touchEnemy = iRectZero;
				act = Act_dead;
				tileNumber = 0;
			}
			break;
		}
		case Act_dead:
		{
			break;
		}
		default:
			break;
		}
	}

	if (takeDmg)
		takeDmgEffect(dt);

	if (showHp)
		drawShowHP(dt);
}


//----------------------------------------------------------------------------------------

golemImgInfo eii[GOLEM_IMG_NUM + GOLEM_ELETE_IMG_NUM] = {
	{ "assets/monster/golem1/Idle/Golem_01_Idle_%d.png",
		12, 1, false, 0.08f, false },
	{ "assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png",
		12, 1, false, 0.08f, false },
	{ "assets/monster/golem1/Walking/Golem_01_Walking_%d.png",
		18, 0, false, 0.05f, true },
	{ "assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png",
		12, 1, false, GOLEM_MELEE_ATKTIME / 12.0f, false },
	{ "assets/monster/golem1/Dying/Golem_01_Dying_%d.png",
		15, 1, true, 0.07f, false },
	{ "assets/monster/golem1/Taunt/Golem_01_Taunt_%03d.png",
		18, 1, false, GOLEM_RANGE_ATKTIME / 18.0f, false },
	{ "assets/monster/golem1/Hurt/Golem_01_Hurt_%03d.png",
		12, 1, false, 0.08f, false },
	////////////////////////////////////////////////////
	{ "assets/monster/golemElete/Idle/Golem_03_Idle_%03d.png",
		12, 1, false, 0.08f, false },
	{ "assets/monster/golemElete/Idle Blink/Golem_03_Idle Blinking_%03d.png",
		12, 1, false, 0.08f, false },
	{ "assets/monster/golemElete/Walking/Golem_03_Walking_%03d.png",
		18, 0, false, 0.05f, true },
	{ "assets/monster/golemElete/Attacking/Golem_03_Attacking_%03d.png",
		12, 1, false, GOLEM_ELETE_MELEE_ATKTIME / 12.0f, false },
	{ "assets/monster/golemElete/Dying/Golem_03_Dying_%03d.png",
		15, 1, true, 0.07f, false },
	{ "assets/monster/golemElete/Taunt/Golem_03_Taunt_%03d.png",
		18, 1, false, GOLEM_ELETE_RANGE_ATKTIME / 18.0f, false },
	{ "assets/monster/golemElete/Hurt/Golem_03_Hurt_%03d.png",
		12, 1, false, 0.08f, false },
};

golemImgInfo egb[GOLEM_BOSS_IMG_NUM] = { // %c = d l r t
	{"assets/monster/golemBoss/golemBoss - idle/golem-idle-%c-%02d.png",
	5,0,false, 0.2f, true },

	{"assets/monster/golemBoss/golemBoss - move/golem-run-%c-%02d.png",
	6,0,false,0.2f, true },

	{"assets/monster/golemBoss/golemBoss - attack/golem-attack-%c-%02d.png",
	7,1,false, GOLEM_ELETE_MELEE_ATKTIME / 7.0f, false },

	{"assets/monster/golemBoss/golemBoss - death/golem-death-%c-%02d.png",
	9,1,true,0.2f, false },

	{"assets/monster/golemBoss/golemBoss - hurt/golem-hurt-%c-%02d.png",
	3,1,false,0.2f, false },
};

void createGolemImg()
{
	int i, j, k;
	int total = GOLEM_IMG_NUM + GOLEM_ELETE_IMG_NUM;
	iImage** imgG = (iImage**)malloc(sizeof(iImage*) * total);
	for (i = 0; i < total; i++)
	{
		golemImgInfo* e = &eii[i];

		iImage* img = new iImage();
		for (j = 0; j < e->imgNum; j++)
		{
			Texture* tex = createImage(e->strPath, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->_repeatNum = e->repeatNum;
		img->lastFrame = e->lastFrame;
		img->_aniDt = e->aniDt;
		img->animation = e->animation;
		img->ratio = (i < GOLEM_IMG_NUM ? GOLEM_RATIO : GOLEM_ELETE_RATIO);

		imgG[i] = img;
	}

	imgGolem = imgG;
	imgGolemElete = &imgG[GOLEM_IMG_NUM];



	total = GOLEM_BOSS_IMG_NUM * GOLEM_BOSS_IMG_DIRECTION;
	imgG = (iImage**)malloc(sizeof(iImage*) * total);
	char c[4] = { 'd' ,'l' ,'r' ,'t' };
	for (i = 0; i < total; i++)
	{
		golemImgInfo* e = &egb[i / 4];
		iImage* img = new iImage();
		for (j = 0; j < e->imgNum; j++)
		{
			Texture* tex = createImage(e->strPath, c[i % 4], j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->_repeatNum = e->repeatNum;
		img->lastFrame = e->lastFrame;
		img->_aniDt = e->aniDt;
		img->animation = e->animation;
		img->ratio = GOLEM_BOSS_RATIO;
		imgG[i] = img;
	}
	imgGolemBoss = imgG;
}

void freeGolemImg()
{
	int total = GOLEM_IMG_NUM + GOLEM_ELETE_IMG_NUM;
	for (int i = 0; i < total; i++)
		delete imgGolem[i];
	free(imgGolem);
	//free(imgGolemElete);

	total = GOLEM_BOSS_IMG_NUM * GOLEM_BOSS_IMG_DIRECTION;
	for (int i = 0; i < total; i++)
		delete imgGolemBoss[i];
	free(imgGolemBoss);
	
}
