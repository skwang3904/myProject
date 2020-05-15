#include "Enemy.h"

#include "WMelee.h"

#include "Room.h"
#include "RgTile.h"
#include "Stage.h"

Enemy1** enemys;
Enemy1* enemy1;

void Enemy1::createEnemyImg()
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
void Enemy1::init(int stage)
{
	hp = _hp = 50.0f + ((stage - 1) * 20);
	attackDmg = _attackDmg = 5.0f + ((stage - 1) * 5);
	attackSpeed = _attackSpeed = 0.5f - ((stage - 1) * 0.1f);
	moveSpeed = 50.0f + ((stage - 1) * 50);
	reach = 50.0f;

	for (int i = k; i > -1; i--)
	{
		if (maps[i]->rgTile != NULL)
		{
			Enemy1Position = maps[i]->tileOff + iPointMake(RGTILE_Width * 2,
				RGTILE_Height * 2);
			drawEnemyPos = Enemy1Position + pc->camPosition + setPos;
			k = i - 1;
			if (k < 5)
				k = TILEOFF_NUM - 1;
			break;
		}
	}

	touchEnemy1 = iRectZero;

	showHp = false;
	showHpTime = 0.0f;

	takeDmg = false;
	takeDmgTime = 0.0f;

	giveDmg = false;
	giveDmgTime = 0.0f - _attackSpeed;

	hit = false;
}

void Enemy1::drawShowHp(float dt)
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

void Enemy1::takeDmgEnemy(float dt, float dmg)
{
	// 무기로 공격이 들어오면
	// 무기 히트박스에 몹 히트박스가 걸리면
	// 한번 휘두를때 한번만 공격

	if (attacking && takeDmg == false)
	{
		hp -= dmg;
		takeDmg = true;
		showHp = true;
	}
}

void Enemy1::takeDmgEffect(float dt)
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

bool Enemy1::enemysAttack(float dt)
{
	iPoint v = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
		- Enemy1Position;

	if (iPointLength(v) > reach -10 && giveDmg == false)
		return false;
	static iPoint ATV = v + Enemy1Position;

	if (giveDmg == false && giveDmgTime == 0.0f)
	{
		giveDmg = true;
		giveDmgTime -= _attackSpeed;

		ATV = v;
		float range = reach;
		ATV /= iPointLength(ATV);
		ATV = Enemy1Position + ATV * range;
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

		if (hit == false && evasion == false)
		{
			iPoint n = ATV - Enemy1Position;
			float len = iPointLength(n);
			n /= iPointLength(n);
			iPoint p = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
				- Enemy1Position;

			float dot = min(max(p.x * n.x + p.y * n.y, 0),len);
			iPoint proj = n * dot;
			float hitDis = iPointLength(p - proj);

			if (hitDis < 5.0f) 
			{
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
	enemys = (Enemy1**)malloc(sizeof(Enemy1*) * ENEMY_NUM);

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemys[i] = (Enemy1*)malloc(sizeof(Enemy1) * 1);
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
		Enemy1* enm = enemys[i];
		enm->drawEnemyPos = enm->Enemy1Position + pc->camPosition + setPos;
		if (enm->hp > 0.0f)
		{
			enm->img->paint(dt, enm->drawEnemyPos, REVERSE_NONE);
			enm->touchEnemy1 = iRectMake(enm->drawEnemyPos.x
				, enm->drawEnemyPos.y,
				enm->img->tex->width, enm->img->tex->height);

			setRGBA(0, 0, 0, 1);
			drawRect(enm->touchEnemy1);
			setRGBA(1, 1, 1, 1);

			if (enm->takeDmg)
				enm->takeDmgEffect(dt);

			if (enm->showHp)
				enm->drawShowHp(dt);

			//if (enm->enemysAttack(dt) == false)
			//	moveEnemyType1(enemys[i], dt);

			if (enm->enemysAttack(dt) == false && //수정
				((iPointZero - pc->camPosition).x  < enm->Enemy1Position.x &&
				(iPointZero - pc->camPosition).x + RGTILE_X * RGTILE_Width - 1 > enm->Enemy1Position.x + enm->img->tex->width &&
					(iPointZero - pc->camPosition).y < enm->Enemy1Position.y &&
					(iPointZero - pc->camPosition).y + RGTILE_Y * RGTILE_Height - 1 > enm->Enemy1Position.y + enm->img->tex->height ))
				moveEnemyType1(enemys[i], dt);
		}
		else
		{
			enm->Enemy1Position = iPointZero;
			enm->touchEnemy1 = iRectZero;
		}
	}
}


void moveEnemyType1(Enemy1* enm, float dt)
{// 플레이어에게 직선 이동

	iPoint v = pc->playerPosition - enm->Enemy1Position;
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
			if (enm->Enemy1Position.x > maps[i]->tileOff.x&&
				enm->Enemy1Position.y > maps[i]->tileOff.y)
			{
				if (iPointLength(enm->Enemy1Position - maps[i]->tileOff)
					< iPointLength(enm->Enemy1Position - tile->tileOff))
					tile = maps[i];
			}
		}
	}

	wallCheck(true, tile, enm->Enemy1Position, mp, 
		enm->img->tex->width / 2, enm->img->tex->height / 2);
	
}

