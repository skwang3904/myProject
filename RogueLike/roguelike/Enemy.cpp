#include "Enemy.h"

#include "Player.h"
#include "WMelee.h"

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
	attackDmg = 0.0f;
	_attackDmg = 0.0f;
	attackSpeed = 0.0f;
	_attackSpeed = 0.0f;
	moveSpeed = 100.0f;

	Enemy1Position = iPointMake(200 + 40 * a,80 + 20 * a);

	touchEnemy1 = iRectZero;
	takeDmg = false;
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
	enemys = (Enemy1**)malloc(sizeof(Enemy1*) * 5);
	for (int i = 0; i < 5; i++)
	{
		enemys[i] = (Enemy1*)malloc(sizeof(Enemy1) * 1);
		enemys[i]->init(i);
	}


	//enemy1 = (Enemy1*)malloc(sizeof(Enemy1) * 1);
	//enemy1->init();
	//enemys[0] = enemy1;


	enemysNum = 5;
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
	static float moveDelta = 0.0f;
	moveDelta += dt;
	if (moveDelta > 2.0f)
		moveDelta = 0.0f;

	float moveDis = 2.5f - fabs(linear(moveDelta / 2.0f, -5.0f, 5.0f));

	for (int i = 0; i < num; i++) //monNum
	{
		Enemy1* enm = enemys[i];	
		enm->Enemy1Position.x += moveDis;
		enm->img->paint(dt, enm->Enemy1Position, REVERSE_NONE);
		enm->touchEnemy1 = iRectMake(enm->Enemy1Position.x
			, enm->Enemy1Position.y,
			enm->img->tex->width, enm->img->tex->height);

		setRGBA(0, 0, 0, 1);
		drawRect(enm->touchEnemy1);
		setRGBA(1, 1, 1, 1);
	}

	static float takeDmgDelta = 0.0f;
	for (int i = 0; i < num; i++) //monNum
	{
		Enemy1* enm = enemys[i];
		if (enm->takeDmg)
		{
			setRGBA(0, 0, 0, linear(takeDmgDelta / TAKE_DMG_TIME, 1.0f, 0.0f));
			drawImage(enm->img->tex, enm->Enemy1Position.x, enm->Enemy1Position.y, TOP | LEFT);
			setRGBA(1, 1, 1, 1);

			takeDmgDelta += dt;
			if (takeDmgDelta > TAKE_DMG_TIME)
			{
				takeDmgDelta = 0.0f;
				enm->takeDmg = false;
			}
		}
	}
}

