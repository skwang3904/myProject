#include "EnemyStruct.h"

#include "Room.h"
#include "Stage.h"

#include "EnemyActionPattern.h"

#include "Effect.h"
#include "useitem.h"

//----------------------------------------------------------------------------------------

EnemyGolem** golems;
EnemyGolem** golemEletes;

iImage** imgGolem;
iImage** imgGolemElete;

void EnemyGolem::resetActAtAttack()
{
	img[0]->frame = 0;
	img[1]->frame = 0;
	img[2]->frame = 0;

	img[4]->frame = 0;

	img[6]->frame = 0;
}

void EnemyGolem::init(int stage)
{
	switch (enmtype)
	{
	case golemNomal:
	{
		img = (iImage**)malloc(sizeof(iImage*) * GOLEM_IMG_NUM);
		for (int i = 0; i < GOLEM_IMG_NUM; i++)
			img[i] = imgGolem[i]->copy();

		hp = _hp = 50.0f + ((stage) * 20);
		attackDmg = 5.0f + ((stage) * 5);
		meleeAtkSpeed = GOLEM_MELEE_ATKTIME - ((stage) * GOLEM_MELEE_ATKTIME * 0.1f);
		rangeAtkSpeed = GOLEM_RANGE_ATKTIME;
		moveSpeed = 50.0f + ((stage) * 50);
		meleeReach = 100.0f;
		rangeReach = 300.0f;
		ratio = GOLEM_RATIO;

		methodIdle = IdleEyeBlink;
		methodWalk = WalkToPlayer;
		methodHurt = commonHurt;
		methodMelee = commonAttack;
		methodRange = MethodFalse;

		rangeTime = 0.0f;
		_rangeTime = 10.0f;

		effectImg = NULL;
		projectile = NULL;

		if (stage == 0)
		{
			//effectImg = imgChargeFire->copy();
			//projectile = (FireBall**)malloc(sizeof(FireBall*) * FIREBALL_NUM);
			//for (int i = 0; i < FIREBALL_NUM; i++)
			//	projectile[i] = new FireBall();
		}
		break;
	}
	case golemElete:
	{
		img = (iImage**)malloc(sizeof(iImage*) * GOLEM_ELETE_IMG_NUM);
		for (int i = 0; i < GOLEM_ELETE_IMG_NUM; i++)
			img[i] = imgGolemElete[i]->copy();

		hp = _hp = 100.0f + ((stage) * 40);
		attackDmg = 5.0f + ((stage) * 10);
		meleeAtkSpeed = GOLEM_ELETE_MELEE_ATKTIME - ((stage) * GOLEM_ELETE_MELEE_ATKTIME * 0.15f);
		rangeAtkSpeed = GOLEM_ELETE_RANGE_ATKTIME;
		moveSpeed = 80.0f + ((stage) * 50);
		meleeReach = 150.0f;
		rangeReach = 500.0f;
		ratio = GOLEM_ELETE_RATIO;

		methodIdle = IdleEyeBlink;
		methodWalk = WalkToPlayer;
		methodHurt = commonHurt;
		methodMelee = commonAttack;
		methodRange = rangeAttack;

		rangeTime = 0.0f;
		_rangeTime = 3.0f;

		if (stage == 0)
		{
			effectImg = imgChargeFire->copy();

			projectile = (FireBall**)malloc(sizeof(FireBall*) * FIREBALL_NUM);
			for (int i = 0; i < FIREBALL_NUM; i++)
				projectile[i] = new FireBall();
		}
		break;
	}
	default:
		break;
	}


	tileNumber = 0;
	act = idle;

	touchGolem = iRectZero;

	showHp = false;
	showHpTime = 0.0f;
	takeDmg = false;
	takeDmgTime = 0.0f;
	giveDmg = false;
	giveDmgTime = 0.0f;
	hit = false;

	reverse = REVERSE_NONE;
	ATV = iPointZero;

	if (stage == 0)
	{
		items = (UseItem**)malloc(sizeof(UseItem*) * 3);
		items[0] = new UseItem(coin);
		items[1] = new UseItem(healing);
		items[2] = new UseItem(powerUp);
	}
	methodead = golemItems;

}

void EnemyGolem::paint(float dt)
{
	if (act == dying || act == dead)
	{
		for (int i = 0; i < 3; i++)
			items[i]->paint(dt);

		if (act == dead)
			return;
	}

	Texture* tex = img[0]->tex;
	iRect rt = iRectMake(golemPos.x + tex->width * ratio * 0.25,
		golemPos.y + tex->height * ratio * 0.25f,
		tex->width * ratio * 0.5f,
		tex->height * ratio * 0.75f);
	iRect rt1 = iRectMake(golemPos.x,
		golemPos.y,
		tex->width * ratio,
		tex->height * ratio);

	drawGolemPos = golemPos + pc->camPosition + setPos;
	touchGolem = rt;

	rt.origin += pc->camPosition + setPos;
	rt1.origin += pc->camPosition + setPos;
	setRGBA(0, 0, 0, 1);
	drawRect(rt);
	setRGBA(1, 0, 1, 1);
	drawRect(rt1);
	setRGBA(1, 1, 1, 1);

	if (hp > 0.0f)
	{
		methodIdle(this, dt);
		methodHurt(this, dt);

		if (takeDmg)
			takeDmgEffect(dt);

		if (showHp)
			drawShowHp(dt);

		bool mapPos = (tileNumber == pc->tileNumber);
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
		}

		img[4]->paint(dt, drawGolemPos, reverse);

		if (img[4]->animation == false)
		{
			golemPos = iPointZero;
			touchGolem = iRectZero;
			act = dead;
			if (this == golemEletes[0])
				nextDoor = tileNumber;
			tileNumber = 0;
		}
	}
}

#define SHOW_HP_TIME 1.0f
#define TAKE_DMG_TIME 0.5f
void EnemyGolem::drawShowHp(float dt)
{
	showHpTime += dt;
	if (showHpTime > SHOW_HP_TIME)
	{
		showHp = false;
		showHpTime = 0.0f;
	}

	iRect rt = iRectMake(drawGolemPos.x + img[0]->tex->width * ratio * 0.25f, drawGolemPos.y,
		img[0]->tex->width * ratio * 0.5f, 30);
	setRGBA(0, 0, 0, 1);
	fillRect(rt);
	setRGBA(0, 1, 0, 1);
	fillRect(rt.origin.x, rt.origin.y, rt.size.width * hp / _hp, rt.size.height);
	setRGBA(1, 1, 1, 1);
}

void EnemyGolem::takeDmgEnemy(float dt, float dmg)
{
	if (pc->act == attacking && takeDmg == false)
	{
		hp -= dmg;
		takeDmg = true;
		showHp = true;
		if (act != meleeAtk && act != rangeAtk)
			act = hurt;

		audioPlay(SND_ENEMY_HIT);
	}
}

void EnemyGolem::takeDmgEffect(float dt)
{
	//Texture* tex = img[0]->tex;
	//setRGBA(0, 0, 0, linear(takeDmgTime / TAKE_DMG_TIME, 1.0f, 0.0f));
	//drawImage(tex, drawGolemPos.x, drawGolemPos.y,
	//	0, 0, tex->width, tex->height,
	//	TOP | LEFT, ratio, ratio, 2, 0, reverse);
	//setRGBA(1, 1, 1, 1);

	takeDmgTime += dt;
	if (takeDmgTime > TAKE_DMG_TIME)
	{
		takeDmgTime = 0.0f;
		takeDmg = false;
	}
}

//----------------------------------------------------------------------------------------

void createGolemImg()
{
	golemImg();
	golemEleteImg();
}

void freeGolemImg()
{
	for (int i = 0; i < GOLEM_IMG_NUM; i++)
		delete imgGolem[i];
	free(imgGolem);

	for (int i = 0; i < GOLEM_ELETE_IMG_NUM; i++)
		delete imgGolemElete[i];
	free(imgGolemElete);
}

void golemImg()
{
	iImage** imgG = (iImage**)malloc(sizeof(iImage*) * GOLEM_IMG_NUM);

	//iImage* imgGolemIdle = new iImage();
	//Texture** texGolemIdle = (Texture**)malloc(sizeof(Texture*) * 12);
	//iImage* imgGolemIdleBlink = new iImage();
	//Texture** texGolemIdleBlink = (Texture**)malloc(sizeof(Texture*) * 12);
	//iImage* imgGolemAttacking = new iImage();
	//Texture** texGolemAttacking = (Texture**)malloc(sizeof(Texture*) * 12);
	//iImage* imgGolemHurt = new iImage();
	//Texture** texGolemHurt = (Texture**)malloc(sizeof(Texture*) * 12);
	//for (int i = 0; i < 12; i++)
	//{
	//	texGolemIdle[i] = createImage("assets/monster/golem1/Idle/Golem_01_Idle_%d.png", i);
	//	texGolemIdleBlink[i] = createImage("assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png", i);
	//	texGolemAttacking[i] = createImage("assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png", i);
	//	texGolemHurt[i] = createImage("assets/monster/golem1/Hurt/Golem_01_Hurt_%03d.png", i);


	//	imgGolemIdle->addObject(texGolemIdle[i]);
	//	imgGolemIdleBlink->addObject(texGolemIdleBlink[i]);
	//	imgGolemAttacking->addObject(texGolemAttacking[i]);
	//	imgGolemHurt->addObject(texGolemHurt[i]);

	//	freeImage(texGolemIdle[i]);
	//	freeImage(texGolemIdleBlink[i]);
	//	freeImage(texGolemAttacking[i]);
	//	freeImage(texGolemHurt[i]);
	//}

	//iImage* imgGolemWalk = new iImage();
	//Texture** texGolemWalk = (Texture**)malloc(sizeof(Texture*) * 18);
	//iImage* imgGolemMagic = new iImage();
	//Texture** texGolemMagic = (Texture**)malloc(sizeof(Texture*) * 18);
	//for (int i = 0; i < 18; i++)
	//{
	//	texGolemWalk[i] = createImage("assets/monster/golem1/Walking/Golem_01_Walking_%d.png", i);
	//	texGolemMagic[i] = createImage("assets/monster/golem1/Taunt/Golem_01_Taunt_%03d.png", i);

	//	imgGolemWalk->addObject(texGolemWalk[i]);
	//	imgGolemMagic->addObject(texGolemMagic[i]);

	//	freeImage(texGolemWalk[i]);
	//	freeImage(texGolemMagic[i]);
	//}

	//iImage* imgGolemDying = new iImage();
	//Texture** texGolemDying = (Texture**)malloc(sizeof(Texture*) * 15);
	//for (int i = 0; i < 15; i++)
	//{
	//	texGolemDying[i] = createImage("assets/monster/golem1/Dying/Golem_01_Dying_%d.png", i);
	//	imgGolemDying->addObject(texGolemDying[i]);
	//	freeImage(texGolemDying[i]);
	//}

	iImage* imgGolemIdle = new iImage();
	Texture* texGolemIdle;
	iImage* imgGolemIdleBlink = new iImage();
	Texture* texGolemIdleBlink;
	iImage* imgGolemAttacking = new iImage();
	Texture* texGolemAttacking;
	iImage* imgGolemHurt = new iImage();
	Texture* texGolemHurt;
	for (int i = 0; i < 12; i++)
	{
		texGolemIdle = createImage("assets/monster/golem1/Idle/Golem_01_Idle_%d.png", i);
		texGolemIdleBlink = createImage("assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png", i);
		texGolemAttacking = createImage("assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png", i);
		texGolemHurt = createImage("assets/monster/golem1/Hurt/Golem_01_Hurt_%03d.png", i);


		imgGolemIdle->addObject(texGolemIdle);
		imgGolemIdleBlink->addObject(texGolemIdleBlink);
		imgGolemAttacking->addObject(texGolemAttacking);
		imgGolemHurt->addObject(texGolemHurt);

		freeImage(texGolemIdle);
		freeImage(texGolemIdleBlink);
		freeImage(texGolemAttacking);
		freeImage(texGolemHurt);
	}

	iImage* imgGolemWalk = new iImage();
	Texture* texGolemWalk;
	iImage* imgGolemMagic = new iImage();
	Texture* texGolemMagic;
	for (int i = 0; i < 18; i++)
	{
		texGolemWalk = createImage("assets/monster/golem1/Walking/Golem_01_Walking_%d.png", i);
		texGolemMagic = createImage("assets/monster/golem1/Taunt/Golem_01_Taunt_%03d.png", i);

		imgGolemWalk->addObject(texGolemWalk);
		imgGolemMagic->addObject(texGolemMagic);

		freeImage(texGolemWalk);
		freeImage(texGolemMagic);
	}

	iImage* imgGolemDying = new iImage();
	Texture* texGolemDying;
	for (int i = 0; i < 15; i++)
	{
		texGolemDying = createImage("assets/monster/golem1/Dying/Golem_01_Dying_%d.png", i);
		imgGolemDying->addObject(texGolemDying);
		freeImage(texGolemDying);
	}

	imgGolemIdle->_repeatNum = 1;
	imgGolemIdleBlink->_repeatNum = 1;

	imgGolemWalk->animation = true;
	imgGolemWalk->_aniDt = 0.05f;

	imgGolemAttacking->_aniDt = GOLEM_MELEE_ATKTIME / 12.0f;
	imgGolemAttacking->_repeatNum = 1;

	imgGolemDying->_aniDt = 0.07f;
	imgGolemDying->lastFrame = true;
	imgGolemDying->_repeatNum = 1;

	imgGolemMagic->_aniDt = GOLEM_RANGE_ATKTIME / 18.0f;
	imgGolemMagic->_repeatNum = 1;

	imgGolemHurt->_repeatNum = 1;

	imgG[0] = imgGolemIdle;
	imgG[1] = imgGolemIdleBlink;
	imgG[2] = imgGolemWalk;
	imgG[3] = imgGolemAttacking;
	imgG[4] = imgGolemDying;
	imgG[5] = imgGolemMagic;
	imgG[6] = imgGolemHurt;

	for (int i = 0; i < GOLEM_IMG_NUM; i++)
		imgG[i]->ratio = GOLEM_RATIO;

	imgGolem = imgG;
}

void golemEleteImg()
{
	iImage** imgGE = (iImage**)malloc(sizeof(iImage*) * GOLEM_ELETE_IMG_NUM);

	//iImage* imgGEIdle = new iImage();
	//Texture** texGEIdle = (Texture**)malloc(sizeof(Texture*) * 12);
	//iImage* imgGEIdleBlink = new iImage();
	//Texture** texGEIdleBlink = (Texture**)malloc(sizeof(Texture*) * 12);
	//iImage* imgGEAttacking = new iImage();
	//Texture** texGEAttacking = (Texture**)malloc(sizeof(Texture*) * 12);
	//iImage* imgGEHurt = new iImage();
	//Texture** texGEHurt = (Texture**)malloc(sizeof(Texture*) * 12);
	//for (int i = 0; i < 12; i++)
	//{
	//	texGEIdle[i] = createImage("assets/monster/golemElete/Idle/Golem_03_Idle_0%02d.png", i);
	//	texGEIdleBlink[i] = createImage("assets/monster/golemElete/Idle Blink/Golem_03_Idle Blinking_0%02d.png", i);
	//	texGEAttacking[i] = createImage("assets/monster/golemElete/Attacking/Golem_03_Attacking_0%02d.png",i);
	//	texGEHurt[i] = createImage("assets/monster/golemElete/Hurt/Golem_03_Hurt_%03d.png", i);

	//	imgGEIdle->addObject(texGEIdle[i]);
	//	imgGEIdleBlink->addObject(texGEIdleBlink[i]);
	//	imgGEAttacking->addObject(texGEAttacking[i]);
	//	imgGEHurt->addObject(texGEHurt[i]);

	//	freeImage(texGEIdle[i]);
	//	freeImage(texGEIdleBlink[i]);
	//	freeImage(texGEAttacking[i]);
	//	freeImage(texGEHurt[i]);
	//}

	//iImage* imgGEWalk = new iImage();
	//Texture** texGEWalk = (Texture**)malloc(sizeof(Texture*) * 18);
	//iImage* imgGEMagic = new iImage();
	//Texture** texGEMagic = (Texture**)malloc(sizeof(Texture*) * 18);
	//for (int i = 0; i < 18; i++)
	//{
	//	texGEWalk[i] = createImage("assets/monster/golemElete/Walking/Golem_03_Walking_0%02d.png", i);
	//	texGEMagic[i] = createImage("assets/monster/golemElete/Taunt/Golem_03_Taunt_0%02d.png", i);

	//	imgGEWalk->addObject(texGEWalk[i]);
	//	imgGEMagic->addObject(texGEMagic[i]);

	//	freeImage(texGEWalk[i]);
	//	freeImage(texGEMagic[i]);
	//}

	//iImage* imgGEDying = new iImage();
	//Texture** texGEDying = (Texture**)malloc(sizeof(Texture*) * 15);
	//for (int i = 0; i < 15; i++)
	//{
	//	texGEDying[i] = createImage("assets/monster/golemElete/Dying/Golem_03_Dying_0%02d.png", i );
	//	imgGEDying->addObject(texGEDying[i]);
	//	freeImage(texGEDying[i]);
	//}

	iImage* imgGEIdle = new iImage();
	Texture* texGEIdle;// = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGEIdleBlink = new iImage();
	Texture* texGEIdleBlink;// = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGEAttacking = new iImage();
	Texture* texGEAttacking;// = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGEHurt = new iImage();
	Texture* texGEHurt;// = (Texture**)malloc(sizeof(Texture*) * 12);
	for (int i = 0; i < 12; i++)
	{
		texGEIdle = createImage("assets/monster/golemElete/Idle/Golem_03_Idle_0%02d.png", i);
		texGEIdleBlink = createImage("assets/monster/golemElete/Idle Blink/Golem_03_Idle Blinking_0%02d.png", i);
		texGEAttacking = createImage("assets/monster/golemElete/Attacking/Golem_03_Attacking_0%02d.png", i);
		texGEHurt = createImage("assets/monster/golemElete/Hurt/Golem_03_Hurt_%03d.png", i);

		imgGEIdle->addObject(texGEIdle);
		imgGEIdleBlink->addObject(texGEIdleBlink);
		imgGEAttacking->addObject(texGEAttacking);
		imgGEHurt->addObject(texGEHurt);

		freeImage(texGEIdle);
		freeImage(texGEIdleBlink);
		freeImage(texGEAttacking);
		freeImage(texGEHurt);
	}

	iImage* imgGEWalk = new iImage();
	Texture* texGEWalk;// = (Texture**)malloc(sizeof(Texture*) * 18);
	iImage* imgGEMagic = new iImage();
	Texture* texGEMagic;// = (Texture**)malloc(sizeof(Texture*) * 18);
	for (int i = 0; i < 18; i++)
	{
		texGEWalk = createImage("assets/monster/golemElete/Walking/Golem_03_Walking_0%02d.png", i);
		texGEMagic = createImage("assets/monster/golemElete/Taunt/Golem_03_Taunt_0%02d.png", i);

		imgGEWalk->addObject(texGEWalk);
		imgGEMagic->addObject(texGEMagic);

		freeImage(texGEWalk);
		freeImage(texGEMagic);
	}

	iImage* imgGEDying = new iImage();
	Texture* texGEDying;// = (Texture**)malloc(sizeof(Texture*) * 15);
	for (int i = 0; i < 15; i++)
	{
		texGEDying = createImage("assets/monster/golemElete/Dying/Golem_03_Dying_0%02d.png", i);
		imgGEDying->addObject(texGEDying);
		freeImage(texGEDying);
	}
	imgGEIdle->_repeatNum = 1;
	imgGEIdleBlink->_repeatNum = 1;

	imgGEWalk->animation = true;
	imgGEWalk->_aniDt = 0.05;

	imgGEAttacking->_aniDt = GOLEM_ELETE_MELEE_ATKTIME / 12.0f;
	imgGEAttacking->_repeatNum = 1;

	imgGEDying->_aniDt = 0.07f;
	imgGEDying->lastFrame = true;
	imgGEDying->_repeatNum = 1;

	imgGEMagic->_aniDt = GOLEM_ELETE_RANGE_ATKTIME / 18.0f;
	imgGEMagic->_repeatNum = 1;

	imgGEHurt->_repeatNum = 1;

	imgGE[0] = imgGEIdle;
	imgGE[1] = imgGEIdleBlink;
	imgGE[2] = imgGEWalk;
	imgGE[3] = imgGEAttacking;
	imgGE[4] = imgGEDying;
	imgGE[5] = imgGEMagic;
	imgGE[6] = imgGEHurt;

	for (int i = 0; i < GOLEM_ELETE_IMG_NUM; i++)
		imgGE[i]->ratio = GOLEM_ELETE_RATIO;

	imgGolemElete = imgGE;
}
