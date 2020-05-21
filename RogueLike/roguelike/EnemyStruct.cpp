#include "EnemyStruct.h"

#include "RgProc.h"

EnemyNomalGolem** golems;

void EnemyNomalGolem::createEnemyImg()
{
	iImage** imgGolem = (iImage**)malloc(sizeof(iImage*) * 5);

	iImage* imgGolemIdle = new iImage();
	Texture** texGolemIdle = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGolemIdleBlink = new iImage();
	Texture** texGolemIdleBlink = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGolemAttacking = new iImage();
	Texture** texGolemAttacking = (Texture**)malloc(sizeof(Texture*) * 12);
	for (int i = 0; i < 12; i++)
	{
		texGolemIdle[i] = createImage("assets/monster/golem1/Idle/Golem_01_Idle_%d.png", i);
		texGolemIdleBlink[i] = createImage("assets/monster/golem1/Idle Blink/Golem_01_Idle Blinking_%d.png", i);
		texGolemAttacking[i] = createImage("assets/monster/golem1/Attacking/Golem_01_Attacking_%d.png", i);

		imgGolemIdle->addObject(texGolemIdle[i]);
		imgGolemIdleBlink->addObject(texGolemIdleBlink[i]);
		imgGolemAttacking->addObject(texGolemAttacking[i]);

		freeImage(texGolemIdle[i]);
		freeImage(texGolemIdleBlink[i]);
		freeImage(texGolemAttacking[i]);
	}

	iImage* imgGolemWalk = new iImage();
	Texture** texGolemWalk = (Texture**)malloc(sizeof(Texture*) * 18);
	for (int i = 0; i < 18; i++)
	{
		texGolemWalk[i] = createImage("assets/monster/golem1/Walking/Golem_01_Walking_%d.png", i);
		imgGolemWalk->addObject(texGolemWalk[i]);
		freeImage(texGolemWalk[i]);
	}

	iImage* imgGolemDying = new iImage();
	Texture** texGolemDying = (Texture**)malloc(sizeof(Texture*) * 15);
	for (int i = 0; i < 15; i++)
	{
		texGolemDying[i] = createImage("assets/monster/golem1/Dying/Golem_01_Dying_%d.png", i);
		imgGolemDying->addObject(texGolemDying[i]);
		freeImage(texGolemDying[i]);
	}

	imgGolemIdle->_repeatNum = 1;
	imgGolemIdleBlink->_repeatNum = 1;

	imgGolemWalk->animation = true;
	imgGolemWalk->_aniDt = 0.05;


	imgGolemAttacking->_aniDt = GOLEM_ATTACK_TIME / 12.0f;
	imgGolemAttacking->_repeatNum = 1;

	imgGolemDying->_aniDt = 0.07f;
	imgGolemDying->lastFrame = true;
	imgGolemDying->_repeatNum = 1;

	imgGolem[0] = imgGolemIdle;
	imgGolem[1] = imgGolemIdleBlink;
	imgGolem[2] = imgGolemWalk;
	imgGolem[3] = imgGolemAttacking;
	imgGolem[4] = imgGolemDying;

	for (int i = 0; i < 5; i++)
		imgGolem[i]->ratio = GOLEM_RATIO;

	img = imgGolem;
}

void EnemyNomalGolem::init(int stage)
{
	hp = _hp = 50.0f + ((stage - 1) * 20);
	attackDmg = _attackDmg = 5.0f + ((stage - 1) * 5);
	attackSpeed = _attackSpeed = GOLEM_ATTACK_TIME - ((stage - 1) * 0.1f);
	moveSpeed = 50.0f + ((stage - 1) * 50);
	reach = 50.0f;

	act = idle;

	touchGolem = iRectZero;

	showHp = false;
	showHpTime = 0.0f;
	takeDmg = false;
	takeDmgTime = 0.0f;
	giveDmg = false;
	giveDmgTime = 0.0f;
	hit = false;
}

#define SHOW_HP_TIME 1.0f
void EnemyNomalGolem::drawShowHp(float dt)
{
	showHpTime += dt;
	if (showHpTime > SHOW_HP_TIME)
	{
		showHp = false;
		showHpTime = 0.0f;
	}

	iRect rt = iRectMake(drawGolemPos.x + img[0]->tex->width * GOLEM_RATIO * 0.25f, drawGolemPos.y,
		img[0]->tex->width * GOLEM_RATIO * 0.5f, 10);
	setRGBA(0, 0, 0, 1);
	fillRect(rt);
	setRGBA(0, 1, 0, 1);
	fillRect(rt.origin.x, rt.origin.y, rt.size.width * hp / _hp, rt.size.height);
	setRGBA(1, 1, 1, 1);
}

void EnemyNomalGolem::takeDmgEnemy(float dt, float dmg)
{
	if (pc->act == attacking && takeDmg == false)
	{
		hp -= dmg;
		takeDmg = true;
		showHp = true;
		if (act != attacking)
			act = pain;

		audioPlay(SND_ENEMY_HIT);
	}
}

#define TAKE_DMG_TIME 0.5f
void EnemyNomalGolem::takeDmgEffect(float dt)
{
	Texture* tex = img[0]->tex;
	setRGBA(0, 0, 0, linear(takeDmgTime / TAKE_DMG_TIME, 1.0f, 0.0f));
	drawImage(tex, drawGolemPos.x, drawGolemPos.y,
		0, 0, tex->width, tex->height,
		TOP | LEFT, GOLEM_RATIO, GOLEM_RATIO, 2, 0, REVERSE_NONE);
	setRGBA(1, 1, 1, 1);

	takeDmgTime += dt;
	if (takeDmgTime > TAKE_DMG_TIME)
	{
		takeDmgTime = 0.0f;
		takeDmg = false;
	}
}

bool EnemyNomalGolem::enemysAttack(float dt)
{
	iPoint et = iPointMake(img[0]->tex->width * GOLEM_RATIO / 2.0f,
		img[0]->tex->height * GOLEM_RATIO / 2.0f);

	iPoint v = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
		- golemPos - et;

	if (iPointLength(v) > reach&& giveDmg == false && act != attacking)
		return false;

	static iPoint ATV = v;
	static int reverse = REVERSE_NONE;
	if (giveDmg == false)
	{
		img[3]->startAnimation();

		giveDmg = true;
		giveDmgTime -= _attackSpeed;
		act = attacking;

		ATV = v;
		float range = reach;
		ATV /= iPointLength(ATV);
		ATV = golemPos + et + (ATV * range);
		reverse = (v.x > 0.0f ? REVERSE_NONE : REVERSE_WIDTH);
	}

	giveDmgTime += dt;
	img[3]->paint(dt, drawGolemPos, reverse);

	if (giveDmgTime > 0.0f)
	{
		giveDmg = false;
		giveDmgTime = 0.0f;
		hit = false;
		act = idle;
	}

	if (giveDmg == true && giveDmgTime > 0.0f - _attackSpeed * 0.33f)
	{
		setLineWidth(10);
		setRGBA(1, 0, 0, 1);
		drawLine(drawGolemPos + et,
			ATV + pc->camPosition + setPos);
		setLineWidth(1);
		setRGBA(1, 1, 1, 1);

		if (hit == false && pc->act != evasion)
		{
			iPoint n = ATV - golemPos + et;
			float len = iPointLength(n);
			n /= iPointLength(n);
			iPoint p = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
				- golemPos + et;

			float dot = min(max(p.x * n.x + p.y * n.y, 0), len);
			iPoint proj = n * dot;
			float hitDis = iPointLength(p - proj);

			if (hitDis < HALF_OF_TEX_WIDTH / 2)
			{
				if (pc->act == evasion || pc->act == falling)
					return true;
				printf("hits\n");
				//pc->hp -= attackDmg;
				hit = true;
			}
		}
	}

	if (iPointLength(v) > reach + 50 && giveDmgTime > 0.0f - _attackSpeed * 0.1f)
	{
		giveDmg = false;
		giveDmgTime = 0.0f;
		hit = false;
		act = idle;
		img[3]->animation == false;
		return false;
	}

	return true;
}

void EnemyEleteGolem::createEnemyImg()
{
}

void EnemyEleteGolem::init(int stage)
{
}

void EnemyEleteGolem::drawShowHp(float dt)
{
}

void EnemyEleteGolem::takeDmgEnemy(float dt, float dmg)
{
}

void EnemyEleteGolem::takeDmgEffect(float dt)
{
}

bool EnemyEleteGolem::enemysAttack(float dt)
{
	return false;
}
