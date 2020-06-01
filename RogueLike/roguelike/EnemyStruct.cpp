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
		if (stage == 0)
		{
			img = (iImage**)malloc(sizeof(iImage*) * GOLEM_IMG_NUM);
			for (int i = 0; i < GOLEM_IMG_NUM; i++)
				img[i] = imgGolem[i]->copy();

			effectImg = NULL;
			projectile = NULL;

			//effectImg = imgChargeFire->copy();
			//projectile = (FireBall**)malloc(sizeof(FireBall*) * FIREBALL_NUM);
			//for (int i = 0; i < FIREBALL_NUM; i++)
			//	projectile[i] = new FireBall();
		}

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

		break;
	}
	case golemElete:
	{
		if (stage == 0)
		{
			img = (iImage**)malloc(sizeof(iImage*) * GOLEM_ELETE_IMG_NUM);
			for (int i = 0; i < GOLEM_ELETE_IMG_NUM; i++)
				img[i] = imgGolemElete[i]->copy();

			effectImg = imgChargeFire->copy();

			projectile = (FireBall**)malloc(sizeof(FireBall*) * FIREBALL_NUM);
			for (int i = 0; i < FIREBALL_NUM; i++)
				projectile[i] = new FireBall();
		}

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

	items = NULL;
	if (stage == 0)
	{
		//items = (UseItem**)malloc(sizeof(UseItem*) * GOLEM_ITEM_NUM);
		//items[0] = new UseItem(coin);
		//items[1] = new UseItem(healing);
		//items[2] = new UseItem(powerUp);
		//items[3] = new UseItem(atkSpeedUp);
		//items[4] = new UseItem(moveSpeedUp);
	}
	methodead = golemItems;

}

void EnemyGolem::paint(float dt)
{
	if (act == dying || act == dead)
	{
		for (int i = 0; i < GOLEM_ITEM_NUM; i++)
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

		img[4]->paint(dt, drawGolemPos);

		if (img[4]->animation == false)
		{
			golemPos = iPointZero;
			touchGolem = iRectZero;
			act = dead;
			if (this == &golemEletes[0][0])
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
	imgGolem = (iImage**)malloc(sizeof(iImage*) * GOLEM_IMG_NUM);

	iImage* imgGolemIdle = new iImage();
	iImage* imgGolemIdleBlink = new iImage();
	iImage* imgGolemAttacking = new iImage();
	iImage* imgGolemHurt = new iImage();
	for (int i = 0; i < 12; i++)
	{
		Texture* texGolemIdle = createImage("assets/monster/golem1/Idle/Golem_01_Idle_%d.png", i);
		imgGolemIdle->addObject(texGolemIdle);
		freeImage(texGolemIdle);

		Texture* texGolemIdleBlink = createImage("assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png", i);
		imgGolemIdleBlink->addObject(texGolemIdleBlink);
		freeImage(texGolemIdleBlink);

		Texture* texGolemAttacking = createImage("assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png", i);
		imgGolemAttacking->addObject(texGolemAttacking);
		freeImage(texGolemAttacking);

		Texture* texGolemHurt = createImage("assets/monster/golem1/Hurt/Golem_01_Hurt_%03d.png", i);
		imgGolemHurt->addObject(texGolemHurt);
		freeImage(texGolemHurt);
	}
	imgGolemIdle->_repeatNum = 1;
	imgGolemIdleBlink->_repeatNum = 1;

	imgGolemAttacking->_aniDt = GOLEM_MELEE_ATKTIME / 12.0f;
	imgGolemAttacking->_repeatNum = 1;

	imgGolemHurt->_repeatNum = 1;

	iImage* imgGolemWalk = new iImage();
	iImage* imgGolemMagic = new iImage();
	for (int i = 0; i < 18; i++)
	{
		Texture* texGolemWalk = createImage("assets/monster/golem1/Walking/Golem_01_Walking_%d.png", i);
		imgGolemWalk->addObject(texGolemWalk);
		freeImage(texGolemWalk);

		Texture* texGolemMagic = createImage("assets/monster/golem1/Taunt/Golem_01_Taunt_%03d.png", i);
		imgGolemMagic->addObject(texGolemMagic);
		freeImage(texGolemMagic);
	}
	imgGolemWalk->animation = true;
	imgGolemWalk->_aniDt = 0.05f;

	imgGolemMagic->_aniDt = GOLEM_RANGE_ATKTIME / 18.0f;
	imgGolemMagic->_repeatNum = 1;

	iImage* imgGolemDying = new iImage();
	Texture* texGolemDying;
	for (int i = 0; i < 15; i++)
	{
		texGolemDying = createImage("assets/monster/golem1/Dying/Golem_01_Dying_%d.png", i);
		imgGolemDying->addObject(texGolemDying);
		freeImage(texGolemDying);
	}
	imgGolemDying->_aniDt = 0.07f;
	imgGolemDying->lastFrame = true;
	imgGolemDying->_repeatNum = 1;

	imgGolem[0] = imgGolemIdle;
	imgGolem[1] = imgGolemIdleBlink;
	imgGolem[2] = imgGolemWalk;
	imgGolem[3] = imgGolemAttacking;
	imgGolem[4] = imgGolemDying;
	imgGolem[5] = imgGolemMagic;
	imgGolem[6] = imgGolemHurt;

	for (int i = 0; i < GOLEM_IMG_NUM; i++)
		imgGolem[i]->ratio = GOLEM_RATIO;
}

void golemEleteImg()
{
	imgGolemElete = (iImage**)malloc(sizeof(iImage*) * GOLEM_ELETE_IMG_NUM);

	iImage* imgGEIdle = new iImage();
	iImage* imgGEIdleBlink = new iImage();
	iImage* imgGEAttacking = new iImage();
	iImage* imgGEHurt = new iImage();
	for (int i = 0; i < 12; i++)
	{
		Texture* texGEIdle = createImage("assets/monster/golemElete/Idle/Golem_03_Idle_0%02d.png", i);
		imgGEIdle->addObject(texGEIdle);
		freeImage(texGEIdle);

		Texture* texGEIdleBlink = createImage("assets/monster/golemElete/Idle Blink/Golem_03_Idle Blinking_0%02d.png", i);
		imgGEIdleBlink->addObject(texGEIdleBlink);
		freeImage(texGEIdleBlink);

		Texture* texGEAttacking = createImage("assets/monster/golemElete/Attacking/Golem_03_Attacking_0%02d.png", i);
		imgGEAttacking->addObject(texGEAttacking);
		freeImage(texGEAttacking);

		Texture* texGEHurt = createImage("assets/monster/golemElete/Hurt/Golem_03_Hurt_%03d.png", i);
		imgGEHurt->addObject(texGEHurt);
		freeImage(texGEHurt);
	}
	imgGEIdle->_repeatNum = 1;
	imgGEIdleBlink->_repeatNum = 1;

	imgGEAttacking->_aniDt = GOLEM_ELETE_MELEE_ATKTIME / 12.0f;
	imgGEAttacking->_repeatNum = 1;

	imgGEHurt->_repeatNum = 1;

	iImage* imgGEWalk = new iImage();
	iImage* imgGEMagic = new iImage();
	for (int i = 0; i < 18; i++)
	{
		Texture* texGEWalk = createImage("assets/monster/golemElete/Walking/Golem_03_Walking_0%02d.png", i);
		imgGEWalk->addObject(texGEWalk);
		freeImage(texGEWalk);

		Texture* texGEMagic = createImage("assets/monster/golemElete/Taunt/Golem_03_Taunt_0%02d.png", i);
		imgGEMagic->addObject(texGEMagic);
		freeImage(texGEMagic);
	}
	imgGEWalk->animation = true;
	imgGEWalk->_aniDt = 0.05;

	imgGEMagic->_aniDt = GOLEM_ELETE_RANGE_ATKTIME / 18.0f;
	imgGEMagic->_repeatNum = 1;

	iImage* imgGEDying = new iImage();
	for (int i = 0; i < 15; i++)
	{
		Texture* texGEDying = createImage("assets/monster/golemElete/Dying/Golem_03_Dying_0%02d.png", i);
		imgGEDying->addObject(texGEDying);
		freeImage(texGEDying);
	}

	imgGEDying->_aniDt = 0.07f;
	imgGEDying->lastFrame = true;
	imgGEDying->_repeatNum = 1;

	imgGolemElete[0] = imgGEIdle;
	imgGolemElete[1] = imgGEIdleBlink;
	imgGolemElete[2] = imgGEWalk;
	imgGolemElete[3] = imgGEAttacking;
	imgGolemElete[4] = imgGEDying;
	imgGolemElete[5] = imgGEMagic;
	imgGolemElete[6] = imgGEHurt;

	for (int i = 0; i < GOLEM_ELETE_IMG_NUM; i++)
		imgGolemElete[i]->ratio = GOLEM_ELETE_RATIO;
}
