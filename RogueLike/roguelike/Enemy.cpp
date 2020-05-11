#include "Enemy.h"

#include "Player.h"
#include "WMelee.h"

Enemy1** enemys;
Enemy1* enemy1;
void Enemy1::init()
{
	img = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(50, 100);
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

	Enemy1Position = iPointMake(280,120);

	touchEnemy1 = iRectZero;
}

void Enemy1::takeDmgEnemy(float dt, float dmg)
{
	// 무기로 공격이 들어오면
	// 무기 히트박스에 몹 히트박스가 걸리면
	// 한번 휘두를때 한번만 공격
	static bool takeDmg = false;

	if (attacking && takeDmg == false)
	{
		hp -= dmg;
		takeDmg = true;
	}

	static float delta = 0.0f;

	if (takeDmg)
	{
		setRGBA(1, 0, 0, linear(delta / 0.5f, 1.0f, 0.0f));
		drawImage(img->tex, enemy1->Enemy1Position.x, enemy1->Enemy1Position.y, TOP | LEFT);
		setRGBA(1, 1, 1, 1);
	}

	delta += dt;
	if (delta > 0.5f)
	{
		delta = 0.0f;
		takeDmg = false;
	}
}

void createEnemy()
{
	enemys = (Enemy1**)malloc(sizeof(Enemy1*) * 1);

	enemy1 = (Enemy1*)malloc(sizeof(Enemy1) * 1);
	enemy1->init();

	enemys[0] = enemy1;
}

void freeEnemy()
{
	if (enemy1->img)
		delete enemy1->img;
	free(enemy1);

	free(enemys);
}

void drawEnemy(float dt)
{
	enemy1->img->paint(dt, enemy1->Enemy1Position, REVERSE_NONE);

	enemy1->touchEnemy1 = iRectMake(enemy1->Enemy1Position.x
		, enemy1->Enemy1Position.y ,
		enemy1->img->tex->width, enemy1->img->tex->height);

	setRGBA(0, 0, 0, 1);
	drawRect(enemy1->touchEnemy1);
	setRGBA(1, 1, 1, 1);
}

