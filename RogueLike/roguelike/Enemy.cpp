#include "Enemy.h"

#include "WMelee.h"

#include "Room.h"
#include "RgTile.h"
#include "Stage.h"

EnemyNomal** enemys;
EnemyNomal* enemy1;

void EnemyNomal::createEnemyImg()
{
	img = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(30, 30);
	g->init(size);

	setRGBA(0, 1, 1, 1);
	g->fillRect(0, 0, size.width, size.height);
	setRGBA(1, 1, 1, 1);

	setStringSize(20);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "몹");

	Texture* tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);
}

int k = TILEOFF_NUM - 1;
void EnemyNomal::init(int stage)
{
	hp = _hp = 50.0f + ((stage - 1) * 20);
	attackDmg = _attackDmg = 5.0f + ((stage - 1) * 5);
	attackSpeed = _attackSpeed = 0.5f - ((stage - 1) * 0.1f);
	moveSpeed = 50.0f + ((stage - 1) * 50);
	reach = 50.0f;

	//for (int i = k; i > -1; i--)
	//{
	//	if (maps[i]->rgTile != NULL)
	//	{
	//		EnemyNomalPosition = maps[i]->tileOff + iPointMake(RGTILE_Width * 2,
	//			RGTILE_Height * 2);
	//		drawEnemyPos = EnemyNomalPosition + pc->camPosition + setPos;
	//		k = i - 1;
	//		if (k < 5)
	//			k = TILEOFF_NUM - 1;
	//		break;
	//	}
	//}

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

void EnemyNomal::drawShowHp(float dt)
{
	showHpTime += dt;
	if (showHpTime > SHOW_HP_TIME)
	{
		showHp = false;
		showHpTime = 0.0f;
	}

	setRGBA(0, 0, 0, 1);
	fillRect(drawEnemyPos.x, drawEnemyPos.y - 20, img->tex->width, 5);
	setRGBA(0, 1, 0, 1);
	fillRect(drawEnemyPos.x, drawEnemyPos.y - 20, img->tex->width * hp / _hp, 5);
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

void EnemyNomal::takeDmgEffect(float dt)
{
	setRGBA(0, 0, 0, linear(takeDmgTime / TAKE_DMG_TIME, 1.0f, 0.0f));
	drawImage(img->tex, drawEnemyPos.x, drawEnemyPos.y, TOP | LEFT);
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
	iPoint v = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
		- EnemyNomalPosition;

	if (iPointLength(v) > reach -10 && giveDmg == false)
		return false;
	static iPoint ATV = v + EnemyNomalPosition;

	if (giveDmg == false && giveDmgTime == 0.0f)
	{
		giveDmg = true;
		giveDmgTime -= _attackSpeed;

		ATV = v;
		float range = reach;
		ATV /= iPointLength(ATV);
		ATV = EnemyNomalPosition + ATV * range;
	}

	giveDmgTime += dt;
	if (giveDmgTime > 0.0f)
	{
		giveDmg = false;
		giveDmgTime = 0.0f;
		hit = false;
	}

	if (giveDmg == true && giveDmgTime > 0.0f - _attackSpeed * 0.5f)
	{
		setLineWidth(10);
		setRGBA(1, 0, 0, 1);
		drawLine(drawEnemyPos, ATV + pc->camPosition + setPos);
		setLineWidth(1);
		setRGBA(1, 1, 1, 1);

		if (hit == false && pc->act != evasion)
		{
			iPoint n = ATV - EnemyNomalPosition;
			float len = iPointLength(n);
			n /= iPointLength(n);
			iPoint p = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
				- EnemyNomalPosition;

			float dot = min(max(p.x * n.x + p.y * n.y, 0),len);
			iPoint proj = n * dot;
			float hitDis = iPointLength(p - proj);

			if (hitDis < 5.0f) 
			{
				if (pc->act == evasion || pc->act == falling)
					return true;
				pc->hp -= attackDmg;
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
	enemys = (EnemyNomal**)malloc(sizeof(EnemyNomal*) * ENEMY_NUM);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemys[i] = (EnemyNomal*)malloc(sizeof(EnemyNomal) * 1);
		enemys[i]->createEnemyImg();
		enemys[i]->init(stage);
	}
}

void freeEnemy()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemys[i]->img)
			delete enemys[i]->img;
		free(enemys[i]);
	}
	free(enemys);
}

void drawEnemy(float dt)
{
	static float dmgTime = 0.0f;

	for (int i = 0; i < ENEMY_NUM; i++) //monNum
	{
		EnemyNomal* enm = enemys[i];
		enm->drawEnemyPos = enm->EnemyNomalPosition + pc->camPosition + setPos;
		if (enm->hp > 0.0f)
		{
			enm->img->paint(dt, enm->drawEnemyPos, REVERSE_NONE);
			enm->touchEnemyNomal = iRectMake(enm->drawEnemyPos.x
				, enm->drawEnemyPos.y,
				enm->img->tex->width, enm->img->tex->height);

			setRGBA(0, 0, 0, 1);
			drawRect(enm->touchEnemyNomal);
			setRGBA(1, 1, 1, 1);

			if (enm->takeDmg)
				enm->takeDmgEffect(dt);

			if (enm->showHp)
				enm->drawShowHp(dt);

			//if (enm->enemysAttack(dt) == false)
			//	moveEnemyType1(enemys[i], dt);

			if (enm->enemysAttack(dt) == false && //수정
				((iPointZero - pc->camPosition).x  < enm->EnemyNomalPosition.x &&
				(iPointZero - pc->camPosition).x + RGTILE_X * RGTILE_Width - 1 > enm->EnemyNomalPosition.x + enm->img->tex->width &&
					(iPointZero - pc->camPosition).y < enm->EnemyNomalPosition.y &&
					(iPointZero - pc->camPosition).y + RGTILE_Y * RGTILE_Height - 1 > enm->EnemyNomalPosition.y + enm->img->tex->height ))
				moveEnemyType1(enemys[i], dt);
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

	iPoint v = pc->playerPosition - enm->EnemyNomalPosition;
	if (iPointLength(v) > 200.0f)
		return;
	else if (iPointLength(v) < 40.0f)
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
		enm->img->tex->width / 2, enm->img->tex->height / 2);
	
}

