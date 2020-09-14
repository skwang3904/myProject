#include "Projectile.h"

#include "Map.h"
#include "PlayerChar.h"
#include "Monster.h"
#include "Weapon.h"

Projectile*** _projectile = NULL;
Projectile** projectile = NULL;
int projectileNum;

Projectile::Projectile(int index, int8 mapNum, iPoint pos) : Object(index, mapNum, pos)
{

}

Projectile::~Projectile()
{
}

void Projectile::fire(ProjectileKinds kinds, Weapon* atk)
{
	// atk : 무기, 무기 공격력 = 무기공격력 + 플레이어 공격력
	int i;
	for (i = 0; i < PROJECTILE_NUM; i++)
	{
		Projectile* pjt = _projectile[kinds][i];
		if (pjt->alive == false)
		{
			pjt->addProjectile(atk);

			projectile[projectileNum] = pjt;
			projectileNum++;
			break;
		}
	}
}

//---------------------------------------------------------------------------
// BowGunArrow
iImage* imgProjectileBowGunArrow = NULL;
BowGunArrow::BowGunArrow(int index, int8 mapNum, iPoint pos) : Projectile(index, mapNum, pos)
{
	int i, j;
	Texture* tex, *t;
	iImage* img;
	iSize size;

	if (imgProjectileBowGunArrow == NULL)
	{
		img = new iImage();
		size = iSizeMake(32, 64);
		tex = createTexture(size.width, size.height);

		fbo->bind(tex);
		t = createImage("assets/weapon/Projectile/arrow0.png");
		drawImage(t, size.width / 2.0f, size.height / 2.0f,
			0, 0, t->width, t->height,
			VCENTER | HCENTER, size.width / t->width, size.height / t->height,
			2, 0, REVERSE_HEIGHT);
		freeImage(t);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);

		imgProjectileBowGunArrow = img;
	}
	
	img = imgProjectileBowGunArrow->copy();

	img->lockAngle = true;
	img->angle = 0.0f;
	img->anc = VCENTER | HCENTER;
	this->img = img;
}

BowGunArrow::~BowGunArrow()
{
	delete img;

	if (imgProjectileBowGunArrow)
	{
		delete imgProjectileBowGunArrow;
		imgProjectileBowGunArrow = NULL;
	}
}

void BowGunArrow::paint(float dt, iPoint off)
{
	if (alive == false || mapNumber != player->mapNumber)
		return;
	
	iPoint mp = vector * (moveSpeed * dt);
	if (wallCheck(this, mp))
	{
		alive = false;
		return;
	}

	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		if (mapNumber == m->mapNumber)
		{
			if (containPoint(position, m->touchRect))
			{
				alive = false;
				m->action(this);
				break;
			}
		}
	}

	img->paint(dt, position + off);
}

void BowGunArrow::drawShadow(float dt, iPoint off)
{
}

void BowGunArrow::action(Object* obj)
{
}

void BowGunArrow::addProjectile(Weapon* atk)
{
	alive = true;
	mapNumber = player->mapNumber;
	img->angle = atk->holdAngle;

	position = atk->position + atk->drawPos * 2.0f;
	vector = atk->vector;

	attackPoint = atk->attackPoint;
	actionDt = atk->actionDt;
	_actionDt = atk->_actionDt;

	moveSpeed = atk->moveSpeed;
}

//---------------------------------------------------------------------------
iImage* imgProjectileFireBall = NULL;
FireBall::FireBall(int index, int8 mapNum, iPoint pos) : Projectile(index, mapNum, pos)
{
	int i, j;
	Texture* tex, *t;
	iImage* img;
	iSize size;

	if (imgProjectileFireBall == NULL)
	{
		Texture** texs = createDivideImage(5, 6, "assets/weapon/Projectile/fire_24x92.png");
		img = new iImage();
		size = iSizeMake(32, 64);

		for (i = 0; i < 30; i++)
		{
			tex = createTexture(size.width, size.height);
			fbo->bind(tex);
			t = texs[i];
			drawImage(t, size.width / 2.0f, size.height / 2.0f,
				0, 0, t->width, t->height,
				VCENTER | HCENTER, size.width / t->width, size.height / t->height,
				2, 0);
			freeImage(t);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		free(texs);

		imgProjectileFireBall = img;
	}

	img = imgProjectileFireBall->copy();

	img->animation = true;
	img->lockAngle = true;
	img->angle = 0.0f;
	img->anc = VCENTER | HCENTER;
	this->img = img;

	delta = _delta = 3.0f;
}

FireBall::~FireBall()
{
	delete img;

	if (imgProjectileFireBall)
	{
		delete imgProjectileFireBall;
		imgProjectileFireBall = NULL;
	}
}

void FireBall::paint(float dt, iPoint off)
{
	if (alive == false || mapNumber != player->mapNumber)
		return;

	delta += dt;
	if (delta > _delta)
	{
		alive = false;
		return;
	}

	iPoint mp = vector * (moveSpeed * dt);
	int check = wallCheck(this, mp);
	switch (check)
	{
	case 0:
	case 1:
	{
		vector.x *= -1.0f;
		img->angle += 180.0f;
		break;
	}
	case 2:
	case 3:
	
		vector.y *= -1.0f;
		img->angle += 180.0f;
		break;

	default:
		break;
	}

	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		if (mapNumber == m->mapNumber)
		{
			if (containPoint(position, m->touchRect))
			{
				alive = false;
				m->action(this);
				break;
			}
		}
	}

	img->paint(dt, position + off);

}

void FireBall::drawShadow(float dt, iPoint off)
{
}

void FireBall::action(Object* obj)
{
}

void FireBall::addProjectile(Weapon* atk)
{
	alive = true;
	mapNumber = player->mapNumber;
	img->angle = atk->holdAngle;

	position = atk->position + atk->drawPos;
	vector = atk->vector;

	attackPoint = atk->attackPoint;
	actionDt = atk->actionDt;
	_actionDt = atk->_actionDt;

	moveSpeed = atk->moveSpeed;

	delta = 0.0f;
}

//---------------------------------------------------------------------------

void loadProjectile()
{
	int i, j, num = 0;
	if (_projectile)
		freeProjectile();

	num = Projectile_MAX;
	projectileNum = 0;
	_projectile = (Projectile***)malloc(sizeof(Projectile**) * num);
	for (i = 0; i < num; i++)
	{
		_projectile[i] = (Projectile**)malloc(sizeof(Projectile*) * PROJECTILE_NUM);
		for (j = 0; j < PROJECTILE_NUM; j++)
		{
			switch (i)
			{
			case Projectile_BowGunArrow: _projectile[i][j] = new BowGunArrow(0, 0, iPointZero); break;
			case Projectile_FireBall: _projectile[i][j] = new FireBall(0, 0, iPointZero); break;

			default: printf("Projectile Create Error\n");	break;
			}
		}
	}

	num = Projectile_MAX * PROJECTILE_NUM;
	projectile = (Projectile**)malloc(sizeof(Projectile*) * num);
}

void freeProjectile()
{
	int i, j;
	for (i = 0; i < Projectile_MAX; i++)
	{
		for (j = 0; j < PROJECTILE_NUM; j++)
			delete _projectile[i][j];
		free(_projectile[i]);
	}
	free(_projectile);
	_projectile = NULL;
	free(projectile);
	projectile = NULL;
	projectileNum = 0;
}

void drawProjectile(float dt)
{
	int i, j;
	for (i = 0; i < projectileNum; i++)
	{
		Projectile* pjt = projectile[i];
		pjt->paint(dt, DRAW_OFF);

		if (pjt->alive == false)
		{
			projectileNum--;
			projectile[i] = projectile[projectileNum];
			i--;
		}
	}
}
