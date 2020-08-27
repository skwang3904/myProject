#include "Weapon.h"

#include "PlayerChar.h"
#include "Map.h"

Weapon** weapon;
int weaponNum;

Weapon::Weapon(int index) : Object(index)
{
	int i, j;
	iImage* img;
	Texture* tex;


	imgNum = 1;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);

	iSize size = iSizeMake(20, 50);
	for (i = 0; i < imgNum; i++)
	{
		img = new iImage();

		tex = createTexture(size.width, size.height);
		fbo->bind(tex);
		fbo->clear(0, 0.5f, 1, 1);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);

		imgs[i] = img;
	}
	this->img = imgs[0];
	mapNumber = 0;

	position = player->position + iPointMake(30, 0);
	touchRect = iRectMake(position.x - size.width / 2.0f, position.y - size.height / 2.0f,
		size.width, size.height);

	attackPoint = 10.0f;
	_attackPoint = 10.0f;
	attackSpeed = 1.0f;
	_attackSpeed = 1.0f;
	attackDelay = 2.0f;
	_attackDelay = 2.0f;
	angle = 90.0f;
	range = 90.0f;

	hit = false;
	get = false;

}

Weapon::~Weapon()
{
}

void Weapon::paint(float dt, iPoint off)
{
	if (mapNumber != player->mapNumber)
		return;

	if (get)
		return;

	getWeapon();
	//attack(dt);
#if 0
	float d = attackSpeed / _attackSpeed;
	float ang = linear(d, 0.0f, angle);
	iPoint pp = player->position + iPointMake(player->img->tex->width / 2, player->img->tex->height / 2);
	position = player->position + iPointMake(100, 0);
	iPoint rp = iPointRotate(position, player->position, ang);
	img->angle = 45;
	img->lockAngle = true;
	img->paint(dt, rp + off);
#else
	position = player->position + iPointMake(30, 0);
	img->paint(dt, position + off);
#endif
}

void Weapon::attack(float dt)
{
	if (attackDelay < _attackDelay)
	{
		attackDelay += dt;
		return;
	}
	
	//attack
	float d = attackSpeed / _attackSpeed;
	float ang = linear(d, 0.0f, angle);
	float ran = linear(d, 0.0f, range);

	iPoint pp = player->position + iPointMake(player->img->tex->width, player->img->tex->height);
	iPoint rp = iPointRotate(position, player->position, ang);

	attackSpeed += dt;
	if (attackSpeed > _attackSpeed)
	{
		attackSpeed = 0.0f;
		attackDelay -= _attackDelay;
	}
}

void Weapon::getWeapon()
{
	
	if (getKeyStat(keyboard_i))
	{
		if (containRect(touchRect, player->touchRect))
		{
		}	
	}
}


void loadWeapon()
{
	weaponNum = 1;
	weapon = (Weapon**)malloc(sizeof(Weapon*) * weaponNum);

	for (int i = 0; i < weaponNum; i++)
	{
		weapon[i] = new Weapon(0);
	}
}

void freeWeapon()
{
	for (int i = 0; i < weaponNum; i++)
		delete weapon[i];
	free(weapon);

}

void drawWeapon(float dt)
{
	for (int i = 0; i < weaponNum; i++)
		weapon[i]->paint(dt, DRAW_OFF);
}