#include "EnemyStruct.h"

#include "Room.h"

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
	attackSpeed = _attackSpeed = GOLEM_ATTACK_TIME - ((stage - 1) * GOLEM_ATTACK_TIME * 0.1f);
	moveSpeed = 50.0f + ((stage - 1) * 50);
	reach = 50.0f;

	act = idle;

	tileNumber = -1;

	touchGolem = iRectZero;
	texRatio = GOLEM_RATIO;

	showHp = false;
	showHpTime = 0.0f;
	takeDmg = false;
	takeDmgTime = 0.0f;
	giveDmg = false;
	giveDmgTime = 0.0f;
	hit = false;
}

void EnemyNomalGolem::drawEnemy(float dt)
{
	if (act == dead)
		return;

	static int reverse;
	Texture* tex = img[0]->tex;
	drawGolemPos = golemPos + pc->camPosition + setPos;

	iRect rt = iRectMake(drawGolemPos.x + tex->width * texRatio * 0.25,
		drawGolemPos.y + tex->height * texRatio * 0.25f,
		tex->width * texRatio * 0.5f,
		tex->height * texRatio * 0.75f);
	iRect rt1 = iRectMake(drawGolemPos.x,
		drawGolemPos.y,
		tex->width * texRatio,
		tex->height * texRatio);

	if (hp > 0.0f)
	{
		reverse = (pc->playerPosition.x < golemPos.x ? REVERSE_WIDTH : REVERSE_NONE);
		if (img[0]->animation == false && img[1]->animation == false)
			img[random() % 2]->startAnimation();

		if (act != attacking)
		{
			if (act == idle)
			{
				if (img[0]->animation)	img[0]->paint(dt, drawGolemPos, reverse);
				else	img[1]->paint(dt, drawGolemPos, reverse);
			}
			else //if (act == walking)
				img[2]->paint(dt, drawGolemPos, reverse);
		}
		touchGolem = rt;

		setRGBA(0, 0, 0, 1);
		drawRect(touchGolem);
		setRGBA(1, 0, 1, 1);
		drawRect(rt1);
		setRGBA(1, 1, 1, 1);

		if (takeDmg)
			takeDmgEffect(dt);

		if (showHp)
			drawShowHp(dt);

		iPoint mapPos = iPointZero - pc->camPosition;
		if (enemysAttack(dt) == false && //수정
			(mapPos.x  < golemPos.x &&
				mapPos.x + RGTILE_X * RGTILE_Width - 1 > golemPos.x + tex->width * texRatio * 0.75f &&
				mapPos.y < golemPos.y + tex->height * texRatio * 0.25f &&
				mapPos.y + RGTILE_Y * RGTILE_Height - 1 > golemPos.y + tex->height * texRatio))
		{
			act = walking;
			moveEnemyType1(dt);
		}
	}
	else
	{
		if (act != dying)
		{
			act = dying;
			audioPlay(SND_ENEMY_DEAD);
			img[4]->startAnimation();
		}

		img[4]->paint(dt, drawGolemPos, reverse);

		if (img[4]->animation == false)
		{
			golemPos = iPointZero;
			touchGolem = iRectZero;
			act = dead;
		}
	}
}

bool EnemyNomalGolem::enemysAttack(float dt)
{
	iPoint et = iPointMake(img[0]->tex->width * texRatio / 2.0f,
		img[0]->tex->height * texRatio / 2.0f);

	iPoint v = pc->playerPosition + HALF_OF_TEX_POINT
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
			iPoint p = pc->playerPosition + HALF_OF_TEX_POINT
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

	if (iPointLength(v) > reach + 50 && giveDmgTime > 0.0f - _attackSpeed * 0.2f)
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

void EnemyNomalGolem::moveEnemyType1(float dt)
{
	Texture* tex = img[0]->tex;
	iPoint gp = iPointMake(tex->width * texRatio / 2.0f, tex->height * texRatio / 2.0f);
	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (golemPos + gp);

	if (iPointLength(v) > 300.0f || iPointLength(v) < reach)
	{
		act = idle;
		return;
	}
	
	v /= iPointLength(v);
	iPoint mp = v * moveSpeed * dt;
	
	gp *= 0.5f;
	iPoint dummy = golemPos + gp;
	wallCheck(true, maps[tileNumber], dummy, mp, gp.x, gp.y * 1.5f);
	golemPos = dummy - gp;
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

	iRect rt = iRectMake(drawGolemPos.x + img[0]->tex->width * texRatio * 0.25f, drawGolemPos.y,
		img[0]->tex->width * texRatio * 0.5f, 10);
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
		TOP | LEFT, texRatio, texRatio, 2, 0, REVERSE_NONE);
	setRGBA(1, 1, 1, 1);

	takeDmgTime += dt;
	if (takeDmgTime > TAKE_DMG_TIME)
	{
		takeDmgTime = 0.0f;
		takeDmg = false;
	}
}

//----------------------------------------------------------------------------------------

EnemyEleteGolem** golemEletes;

void EnemyEleteGolem::createEnemyImg()
{
	iImage** imgGE = (iImage**)malloc(sizeof(iImage*) * 5);

	iImage* imgGEIdle = new iImage();
	Texture** texGEIdle = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGEIdleBlink = new iImage();
	Texture** texGEIdleBlink = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGEAttacking = new iImage();
	Texture** texGEAttacking = (Texture**)malloc(sizeof(Texture*) * 12);
	for (int i = 0; i < 12; i++)
	{
		texGEIdle[i] = createImage("assets/monster/golemElete/Idle/Golem_03_Idle_0%02d.png", i);
		texGEIdleBlink[i] = createImage("assets/monster/golemElete/Idle Blink/Golem_03_Idle Blinking_0%02d.png", i % 10);
		texGEAttacking[i] = createImage("assets/monster/golemElete/Attacking/Golem_03_Attacking_0%02d.png", i % 10);

		imgGEIdle->addObject(texGEIdle[i]);
		imgGEIdleBlink->addObject(texGEIdleBlink[i]);
		imgGEAttacking->addObject(texGEAttacking[i]);

		freeImage(texGEIdle[i]);
		freeImage(texGEIdleBlink[i]);
		freeImage(texGEAttacking[i]);
	}

	iImage* imgGEWalk = new iImage();
	Texture** texGEWalk = (Texture**)malloc(sizeof(Texture*) * 18);
	for (int i = 0; i < 18; i++)
	{
		texGEWalk[i] = createImage("assets/monster/golemElete/Walking/Golem_03_Walking_0%02d.png", i % 10);
		imgGEWalk->addObject(texGEWalk[i]);
		freeImage(texGEWalk[i]);
	}

	iImage* imgGEDying = new iImage();
	Texture** texGEDying = (Texture**)malloc(sizeof(Texture*) * 15);
	for (int i = 0; i < 15; i++)
	{
		texGEDying[i] = createImage("assets/monster/golemElete/Dying/Golem_03_Dying_0%02d.png", i % 10);
		imgGEDying->addObject(texGEDying[i]);
		freeImage(texGEDying[i]);
	}

	imgGEIdle->_repeatNum = 1;
	imgGEIdleBlink->_repeatNum = 1;

	imgGEWalk->animation = true;
	imgGEWalk->_aniDt = 0.05;


	imgGEAttacking->_aniDt = GOLEM_ELETE_ATTACK_TIME / 12.0f;
	imgGEAttacking->_repeatNum = 1;

	imgGEDying->_aniDt = 0.07f;
	imgGEDying->lastFrame = true;
	imgGEDying->_repeatNum = 1;

	imgGE[0] = imgGEIdle;
	imgGE[1] = imgGEIdleBlink;
	imgGE[2] = imgGEWalk;
	imgGE[3] = imgGEAttacking;
	imgGE[4] = imgGEDying;

	for (int i = 0; i < 5; i++)
		imgGE[i]->ratio = GOLEM_ELETE_RATIO;

	img = imgGE;
}

void EnemyEleteGolem::init(int stage)
{
	hp = _hp = 100.0f + ((stage - 1) * 40);
	attackDmg = _attackDmg = 5.0f + ((stage - 1) * 10);
	attackSpeed = _attackSpeed = GOLEM_ELETE_ATTACK_TIME - ((stage - 1) * GOLEM_ATTACK_TIME * 0.15f);
	moveSpeed = 80.0f + ((stage - 1) * 50);
	reach = 100.0f;

	act = idle;

	tileNumber = -1;

	touchGolem = iRectZero;
	texRatio = GOLEM_ELETE_RATIO;

	showHp = false;
	showHpTime = 0.0f;
	takeDmg = false;
	takeDmgTime = 0.0f;
	giveDmg = false;
	giveDmgTime = 0.0f;
	hit = false;
}

void EnemyEleteGolem::drawEnemy(float dt)
{
	if (act == dead)
		return;

	static int reverseE;
	Texture* tex = img[0]->tex;
	drawGolemPos = golemPos + pc->camPosition + setPos;

	iRect rt = iRectMake(drawGolemPos.x + tex->width * GOLEM_ELETE_RATIO * 0.25,
		drawGolemPos.y + tex->height * texRatio * 0.25f,
		tex->width * texRatio * 0.5f,
		tex->height * texRatio * 0.75f);
	iRect rt1 = iRectMake(drawGolemPos.x,
		drawGolemPos.y,
		tex->width * texRatio,
		tex->height * texRatio);


	if (hp > 0.0f)
	{
		reverseE = (pc->playerPosition.x < golemPos.x ? REVERSE_WIDTH : REVERSE_NONE);
		if (img[0]->animation == false && img[1]->animation == false)
			img[random() % 2]->startAnimation();

		if (act != attacking)
		{
			if (act == idle)
			{
				if (img[0]->animation)	img[0]->paint(dt, drawGolemPos, reverseE);
				else	img[1]->paint(dt, drawGolemPos, reverseE);
			}
			else //if (act == walking)
				img[2]->paint(dt, drawGolemPos, reverseE);
		}
		touchGolem = rt;

		setRGBA(0, 0, 0, 1);
		drawRect(touchGolem);
		setRGBA(1, 0, 1, 1);
		drawRect(rt1);
		setRGBA(1, 1, 1, 1);

		if (takeDmg)
			takeDmgEffect(dt);

		if (showHp)
			drawShowHp(dt);

		iPoint mapPos = maps[tileNumber]->tileOff;
		if (enemysAttack(dt) == false && //수정
			(mapPos.x  < golemPos.x &&
				mapPos.x + RGTILE_X * RGTILE_Width - 1 > golemPos.x + tex->width * texRatio * 0.75f &&
				mapPos.y < golemPos.y + tex->height * texRatio * 0.25f &&
				mapPos.y + RGTILE_Y * RGTILE_Height - 1 > golemPos.y + tex->height * texRatio))
		{
			act = walking;
			moveEnemyType1(dt);
		}

	}
	else
	{
		if (act != dying)
		{
			act = dying;
			audioPlay(SND_ENEMY_DEAD);
			img[4]->startAnimation();
		}

		img[4]->paint(dt, drawGolemPos, reverseE);

		if (img[4]->animation == false)
		{
			golemPos = iPointZero;
			touchGolem = iRectZero;
			act = dead;
		}
	}
}

bool EnemyEleteGolem::enemysAttack(float dt)
{
	iPoint et = iPointMake(img[0]->tex->width * texRatio / 2.0f,
		img[0]->tex->height * texRatio / 2.0f);

	iPoint v = pc->playerPosition + HALF_OF_TEX_POINT
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
			iPoint p = pc->playerPosition + HALF_OF_TEX_POINT
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

void EnemyEleteGolem::moveEnemyType1(float dt)
{
	Texture* tex = img[0]->tex;
	iPoint gp = iPointMake(tex->width * texRatio / 2.0f, tex->height * texRatio / 2.0f);
	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (golemPos + gp);
	if (iPointLength(v) > 300.0f || iPointLength(v) < reach)
	{
		act = idle;
		return;
	}

	v /= iPointLength(v);
	iPoint mp = v * moveSpeed * dt;

	gp *= 0.5f;
	iPoint dummy = golemPos + gp ;
	wallCheck(true, maps[tileNumber], dummy, mp, gp.x, gp.y * 1.5f);
	golemPos = dummy - gp ;
}

void EnemyEleteGolem::drawShowHp(float dt)
{
	showHpTime += dt;
	if (showHpTime > SHOW_HP_TIME)
	{
		showHp = false;
		showHpTime = 0.0f;
	}

	iRect rt = iRectMake(drawGolemPos.x + img[0]->tex->width * texRatio * 0.25f, drawGolemPos.y,
		img[0]->tex->width * texRatio * 0.5f, 30);
	setRGBA(0, 0, 0, 1);
	fillRect(rt);
	setRGBA(0, 1, 0, 1);
	fillRect(rt.origin.x, rt.origin.y, rt.size.width * hp / _hp, rt.size.height);
	setRGBA(1, 1, 1, 1);
}

void EnemyEleteGolem::takeDmgEnemy(float dt, float dmg)
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

void EnemyEleteGolem::takeDmgEffect(float dt)
{
	Texture* tex = img[0]->tex;
	setRGBA(0, 0, 0, linear(takeDmgTime / TAKE_DMG_TIME, 1.0f, 0.0f));
	drawImage(tex, drawGolemPos.x, drawGolemPos.y,
		0, 0, tex->width, tex->height,
		TOP | LEFT, texRatio, texRatio, 2, 0, REVERSE_NONE);
	setRGBA(1, 1, 1, 1);

	takeDmgTime += dt;
	if (takeDmgTime > TAKE_DMG_TIME)
	{
		takeDmgTime = 0.0f;
		takeDmg = false;
	}
}
