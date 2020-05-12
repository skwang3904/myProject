#include "Enemy.h"

#include "WMelee.h"
#include "Room.h"

Enemy1** enemys;
Enemy1* enemy1;

int enemysNum;

void Enemy1::init(int a)
{
	img = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(30, 20 + 20 * a);
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
	
	hp = _hp = 50.0f;
	attackDmg = 10.0f;
	_attackDmg = 10.0f;
	attackSpeed = 2.0f;
	_attackSpeed = 2.0f;
	moveSpeed = 100.0f;

	Enemy1Position = iPointMake(200 + 40 * a,80 + 20 * a);

	touchEnemy1 = iRectZero;

	takeDmg = false;
	takeDmgTime = 0.0f;

	giveDmg = false;
	giveDmgTime = 0.0f;
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
	}
}

void createEnemy()
{
	enemysNum = 5;
	enemys = (Enemy1**)malloc(sizeof(Enemy1*) * enemysNum);

	for (int i = 0; i < 5; i++)
	{
		enemys[i] = (Enemy1*)malloc(sizeof(Enemy1) * 1);
		enemys[i]->init(i);
	}
}

void freeEnemy()
{

	for (int i = 0; i < enemysNum; i++)
	{
		if (enemys[i]->img)
			delete enemys[i]->img;
		free(enemys[i]);
	}

	free(enemys);
}

void drawEnemy(float dt)
{
	int num = enemysNum;

	static float dmgTime = 0.0f;

	for (int i = 0; i < num; i++) //monNum
	{
		Enemy1* enm = enemys[i];
		if (enm->hp > 0.0f)
		{
			// 가까우면 공격
			if (enemysAttack(enm, dt) == false)
				moveEnemyType1(enemys[i], dt);

			enm->img->paint(dt, enm->Enemy1Position, REVERSE_NONE);
			enm->touchEnemy1 = iRectMake(enm->Enemy1Position.x
				, enm->Enemy1Position.y,
				enm->img->tex->width, enm->img->tex->height);

			setRGBA(0, 0, 0, 1);
			drawRect(enm->touchEnemy1);
			setRGBA(1, 1, 1, 1);

			if (enm->takeDmg)
			{
				setRGBA(0, 0, 0, linear(enm->takeDmgTime / TAKE_DMG_TIME, 1.0f, 0.0f));
				drawImage(enm->img->tex, enm->Enemy1Position.x, enm->Enemy1Position.y, TOP | LEFT);
				setRGBA(1, 1, 1, 1);

				enm->takeDmgTime += dt;
				if (enm->takeDmgTime > TAKE_DMG_TIME)
				{
					enm->takeDmgTime = 0.0f;
					enm->takeDmg = false;
				}
			}
		}

		else
		{
			enm->Enemy1Position = iPointZero;
			enm->touchEnemy1 = iRectZero;
		}
	}
}

bool enemysAttack(Enemy1* enm, float dt)
{
	static float attDelta = 0.0f;
	iPoint v = pc->playerPosition - enm->Enemy1Position;
	if(iPointLength(v) > 50.0f)
		return false;

	if (enm->giveDmg == false && enm->giveDmgTime == 0.0f)
	{
		enm->giveDmg = true;
		enm->giveDmgTime -= enm->_attackSpeed;

	}

	enm->giveDmgTime += dt;

	if (enm->giveDmgTime > 0.0f)
	{
		enm->giveDmg = false;
		enm->giveDmgTime = 0.0f;
	}
	
	if (enm->giveDmgTime < 0.0f - enm->_attackSpeed / 2.0f)
	{
		setLineWidth(10);
		setRGBA(1, 0, 0, 1);
		float range = 50;
		v /= iPointLength(v);
		v = enm->Enemy1Position + v * range;
		drawLine(enm->Enemy1Position, v);

		setLineWidth(1);
		setRGBA(1, 1, 1, 1);
	}

	return true;
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
	wallCheck(true, enm->Enemy1Position, mp, enm->img->tex->width / 2, enm->img->tex->height / 2);
	
}

