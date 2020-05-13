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
	attackDmg = 5.0f;
	_attackDmg = 5.0f;
	attackSpeed = 2.0f;
	_attackSpeed = 2.0f;
	moveSpeed = 100.0f;
	reach = 100.0f;

	Enemy1Position = iPointMake(200 + 40 * a,80 + 20 * a);

	touchEnemy1 = iRectZero;

	takeDmg = false;
	takeDmgTime = 0.0f;

	giveDmg = false;
	giveDmgTime = 0.0f - _attackSpeed;

	hit = false;
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

void Enemy1::takeDmgEffect(float dt)
{
	setRGBA(0, 0, 0, linear(takeDmgTime / TAKE_DMG_TIME, 1.0f, 0.0f));
	drawImage(img->tex, Enemy1Position.x, Enemy1Position.y, TOP | LEFT);
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
	iPoint v = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH,HALF_OF_TEX_HEIGHT) - Enemy1Position;

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
		drawLine(Enemy1Position, ATV);
		setLineWidth(1);
		setRGBA(1, 1, 1, 1);

		if (hit == false)
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
			enm->img->paint(dt, enm->Enemy1Position, REVERSE_NONE);
			enm->touchEnemy1 = iRectMake(enm->Enemy1Position.x
				, enm->Enemy1Position.y,
				enm->img->tex->width, enm->img->tex->height);

			setRGBA(0, 0, 0, 1);
			drawRect(enm->touchEnemy1);
			setRGBA(1, 1, 1, 1);

			if (enm->takeDmg)
			{
				enm->takeDmgEffect(dt);
			}

			if (enm->enemysAttack(dt) == false)
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
	wallCheck(true, enm->Enemy1Position, mp, enm->img->tex->width / 2, enm->img->tex->height / 2);
	
}

