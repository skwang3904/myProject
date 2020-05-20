#include "Enemy.h"

#include "Room.h"
#include "Stage.h"

#define GOLEM_RATIO 0.2f
EnemyNomal** golems;

void EnemyNomal::createEnemyImg()
{
	//img = new iImage();
	//iGraphics* g = iGraphics::instance();
	//iSize size = iSizeMake(30, 30);
	//g->init(size);
	//setRGBA(0, 1, 1, 1);
	//g->fillRect(0, 0, size.width, size.height);
	//setRGBA(1, 1, 1, 1);
	//setStringSize(20);
	//setStringRGBA(0, 0, 0, 1);
	//setStringBorder(0);
	//g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "몹");
	//Texture* tex = g->getTexture();
	//img->addObject(tex);
	//freeImage(tex);

	iImage** imgGolem = (iImage**)malloc(sizeof(iImage*) * 5);

	iImage* imgGolemIdle = new iImage();
	Texture** texGolemIdle = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGolemIdleBlink = new iImage();
	Texture** texGolemIdleBlink = (Texture**)malloc(sizeof(Texture*) * 12);
	iImage* imgGolemAttacking = new iImage();
	Texture** texGolemAttacking = (Texture**)malloc(sizeof(Texture*) * 12);
	for (int i = 0; i < 12; i++)
	{
		texGolemIdle[i] = createImage("assets/monster/golem1/Idle/Golem_01_Idle_%d.png",i);
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
		texGolemWalk[i] = createImage("assets/monster/golem1/Idle Blink/Golem_01_Walking_%d.png", i);
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

	imgGolem[0] = imgGolemIdle;
	imgGolem[1] = imgGolemIdleBlink;
	imgGolem[2] = imgGolemWalk;
	imgGolem[3] = imgGolemAttacking;
	imgGolem[4] = imgGolemDying;

	for (int i = 0; i < 5; i++)
		imgGolem[i]->ratio = GOLEM_RATIO;

	img = imgGolem;
}

void EnemyNomal::init(int stage)
{
	hp = _hp = 50.0f + ((stage - 1) * 20);
	attackDmg = _attackDmg = 5.0f + ((stage - 1) * 5);
	attackSpeed = _attackSpeed = 0.5f - ((stage - 1) * 0.1f);
	moveSpeed = 50.0f + ((stage - 1) * 50);
	reach = 50.0f;

	touchEnemyNomal = iRectZero;

	showHp = false;
	showHpTime = 0.0f;

	takeDmg = false;
	takeDmgTime = 0.0f;

	giveDmg = false;
	giveDmgTime = 0.0f - _attackSpeed;

	hit = false;
	dead = false;
}

#define SHOW_HP_TIME 1.0f
void EnemyNomal::drawShowHp(float dt)
{
	showHpTime += dt;
	if (showHpTime > SHOW_HP_TIME)
	{
		showHp = false;
		showHpTime = 0.0f;
	}

	iRect rt = iRectMake(drawEnemyPos.x + img[0]->tex->width * GOLEM_RATIO * 0.25f, drawEnemyPos.y,
		img[0]->tex->width * GOLEM_RATIO * 0.5f, 10);
	setRGBA(0, 0, 0, 1);
	fillRect(rt);
	setRGBA(0, 1, 0, 1);
	fillRect(rt.origin.x,rt.origin.y,rt.size.width * hp/_hp,rt.size.height);
	setRGBA(1, 1, 1, 1);
}

void EnemyNomal::takeDmgEnemy(float dt, float dmg)
{
	if (pc->act == attacking && takeDmg == false)
	{
		hp -= dmg;
		takeDmg = true;
		showHp = true;

		audioPlay(SND_ENEMY_HIT);
	}
}

#define TAKE_DMG_TIME 0.5f
void EnemyNomal::takeDmgEffect(float dt)
{
	Texture* tex = img[0]->tex;
	setRGBA(0, 0, 0, linear(takeDmgTime / TAKE_DMG_TIME, 1.0f, 0.0f));
	drawImage(tex, drawEnemyPos.x, drawEnemyPos.y,
		0,0, tex->width, tex->height,
		TOP | LEFT, GOLEM_RATIO, GOLEM_RATIO, 2, 0, REVERSE_NONE);
	setRGBA(1, 1, 1, 1);

	takeDmgTime += dt;
	if (takeDmgTime > TAKE_DMG_TIME)
	{
		takeDmgTime = 0.0f;
		takeDmg = false;
	}
}

bool EnemyNomal::enemysAttack(float dt)
{
	iPoint et = iPointMake(img[0]->tex->width * GOLEM_RATIO / 2.0f,
		img[0]->tex->height * GOLEM_RATIO / 2.0f);

	iPoint v = pc->playerPosition - EnemyNomalPosition + et;

	printf("px = %.3f, py = %.3f\n", pc->playerPosition.x, pc->playerPosition.y);
	//printf("Ex = %.3f, Ey = %.3f\n", EnemyNomalPosition.x, EnemyNomalPosition.y);
	if (iPointLength(v) > reach +50 && giveDmg == false)
		return false;
	static iPoint ATV = v;


	if (giveDmg == false && giveDmgTime == 0.0f)
	{
		giveDmg = true;
		giveDmgTime -= _attackSpeed;

		ATV = v;
		float range = reach;
		ATV /= iPointLength(ATV);
		ATV = EnemyNomalPosition  + (ATV * range);
	}

	giveDmgTime += dt;
	if (giveDmgTime > 0.0f)
	{
		giveDmg = false;
		giveDmgTime = 0.0f;
		hit = false;
	}

	if (giveDmg == true )//&& giveDmgTime > 0.0f - _attackSpeed * 0.5f)
	{
		setLineWidth(10);
		setRGBA(1, 0, 0, 1);
		drawLine(drawEnemyPos + et,
			ATV + pc->camPosition + setPos);
		setLineWidth(3);
		setRGBA(1, 1, 1, 1);
		printf("ax %.3f, ay %.3f \n", ATV.x,ATV.y);

		if (hit == false && pc->act != evasion)
		{
			iPoint n = ATV - EnemyNomalPosition + et;
			float len = iPointLength(n);
			n /= iPointLength(n);
			iPoint p = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
				- EnemyNomalPosition;

			float dot = min(max(p.x * n.x + p.y * n.y, 0),len);
			iPoint proj = n * dot;
			float hitDis = iPointLength(p - proj);

			if (hitDis < 2.0f) 
			{
				if (pc->act == evasion || pc->act == falling)
					return true;
				printf("hits\n");
				//pc->hp -= attackDmg;
				hit = true;
			}
		}
	}

	if (iPointLength(v) > reach +50 && giveDmgTime > 0.0f - _attackSpeed * 0.33f)
	{
		giveDmg = false;
		giveDmgTime = 0.0f;
		hit = false;
	}

	return true;
}

void createEnemy()
{
	golems = (EnemyNomal**)malloc(sizeof(EnemyNomal*) * ENEMY_NUM);

	golems[0] = (EnemyNomal*)malloc(sizeof(EnemyNomal) * 1);
	golems[0]->createEnemyImg();
	golems[0]->init(stage);
	for (int i = 1; i < ENEMY_NUM; i++)
	{
		golems[i] = (EnemyNomal*)malloc(sizeof(EnemyNomal) * 1);
		golems[i]->init(stage);
		iImage** imgs = (iImage**)malloc(sizeof(iImage*) * 5);
		for (int j = 0; j < 5; j++)
		{
			imgs[j] = golems[0]->img[j]->copy();
		}
		golems[i]->img = imgs;
	}
}

void freeEnemy()
{
	//for (int i = 0; i < ENEMY_NUM; i++)
	//{
	//	if (golems[i]->img)
	//		delete golems[i]->img;
	//	free(golems[i]);
	//}
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		for (int j = 0; j < 49; j++)
		{
			if (golems[i]->img[j])
				delete golems[i]->img[j];
		}
		free(golems[i]->img);
	}
	free(golems);
}

void drawEnemy(float dt)
{
	static float dmgTime = 0.0f;

	for (int i = 0; i < ENEMY_NUM; i++) //monNum
	{
		EnemyNomal* enm = golems[i];
		Texture* tex = enm->img[0]->tex;
		iRect rt = iRectMake(enm->drawEnemyPos.x +tex->width * GOLEM_RATIO * 0.25, 
			enm->drawEnemyPos.y + tex->height * GOLEM_RATIO * 0.25f,
			tex->width * GOLEM_RATIO * 0.5f, tex->height * GOLEM_RATIO * 0.5f);

		enm->drawEnemyPos = enm->EnemyNomalPosition + pc->camPosition + setPos;
		if (enm->hp > 0.0f)
		{
			enm->img[0]->paint(dt, enm->drawEnemyPos, REVERSE_NONE);
			enm->touchEnemyNomal = rt;

			setRGBA(0, 0, 0, 1);
			drawRect(enm->touchEnemyNomal);
			setRGBA(1, 1, 1, 1);

			if (enm->takeDmg)
				enm->takeDmgEffect(dt);

			if (enm->showHp)
				enm->drawShowHp(dt);

			if (enm->enemysAttack(dt) == false && //수정
				((iPointZero - pc->camPosition).x  < enm->EnemyNomalPosition.x &&
				(iPointZero - pc->camPosition).x + RGTILE_X * RGTILE_Width - 1 > enm->EnemyNomalPosition.x + tex->width * GOLEM_RATIO * 0.75f &&
					(iPointZero - pc->camPosition).y < enm->EnemyNomalPosition.y &&
					(iPointZero - pc->camPosition).y + RGTILE_Y * RGTILE_Height - 1 > enm->EnemyNomalPosition.y +tex->height* GOLEM_RATIO * 0.75f))
				moveEnemyType1(golems[i], dt);
		}
		else
		{
			enm->EnemyNomalPosition = iPointZero;
			enm->touchEnemyNomal = iRectZero;

			if (enm->dead == false)
			{
				audioPlay(SND_ENEMY_DEAD);
				enm->dead = true;
			}
		}
	}
}


void moveEnemyType1(EnemyNomal* enm, float dt)
{// 플레이어에게 직선 이동
	Texture* tex = enm->img[0]->tex;
	iPoint v = (pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT))
		- (enm->EnemyNomalPosition + iPointMake(tex->width * GOLEM_RATIO /2.0f,tex->height * GOLEM_RATIO /2.0f));
	if (iPointLength(v) > 300.0f || iPointLength(v) < enm->reach -20)
		return;

	v /= iPointLength(v);
	iPoint mp = v * enm->moveSpeed * dt;

	MapTile* tile = maps[0];
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile != NULL)
		{
			if (enm->EnemyNomalPosition.x > maps[i]->tileOff.x&&
				enm->EnemyNomalPosition.y > maps[i]->tileOff.y)
			{
				if (iPointLength(enm->EnemyNomalPosition - maps[i]->tileOff)
					< iPointLength(enm->EnemyNomalPosition - tile->tileOff))
					tile = maps[i];
			}
		}
	}

	wallCheck(true, tile, enm->EnemyNomalPosition, mp,
		tex->width * GOLEM_RATIO / 2.0f, tex->height * GOLEM_RATIO / 2.0f);
	
}

