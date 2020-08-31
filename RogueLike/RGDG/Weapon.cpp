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
	attackAngle = 90.0f;
	attackRange = 90.0f;

	holdAngle = 90.0f;
	anc = TOP | LEFT;

	hit = false;
	get = true;

}

Weapon::~Weapon()
{
}

void Weapon::paint(float dt, iPoint off)
{
#if 0
	if (mapNumber != player->mapNumber)
		return;
#endif
	if (get)
	{
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
#endif
	}
	else
	{
		getWeapon();
	}

	setPosition();
	attack(dt);

	position = player->wpPosition;
	img->anc = anc;
	img->lockAngle = true;

	img->paint(dt, position + off);
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
	float ang = holdAngle + linear(d, 0.0f, attackAngle);
	float ran = linear(d, 0.0f, attackRange);

	iPoint pp = player->position + iPointMake(player->img->tex->width / 2.0f, player->img->tex->height / 2.0f);
	iPoint rp = iPointRotate(position, pp, ang);

	setRGBA(0, 1, 0, 1);
	fillRect(position.x + (DRAW_OFF).x, position.y + (DRAW_OFF).y, 10, 10);
	setRGBA(1, 1, 1, 1);

	img->angle = ang;

	attackSpeed += dt;
	if (attackSpeed > _attackSpeed)
	{
		attackSpeed = 0.0f;
		attackDelay = 0.0f;
	}
}

bool Weapon::getWeapon()
{
	if (getKeyDown(keyboard_i))
	{
		if (containRect(touchRect, player->touchRect))
		{
			get = true;
			return true;
		}	
	}

	return false;
}

void Weapon::setPosition()
{
	iPoint v = player->wpVector;
	float angle = 0.0f;
	int anc = TOP | LEFT;

	if (v.x < 0.0f)
	{
		angle = 90.0f;
		anc = BOTTOM | RIGHT;
	}
	else if (v.x > 0.0f)
	{
		angle = 90.0f;
		anc = TOP | LEFT;
	}
	if (v.y < 0.0f)
	{
		angle = 180.0f;
		anc = BOTTOM | LEFT;
	}
	else if (v.y > 0.0f)
	{
		angle = 0.0f;
		anc = TOP | RIGHT;
	}

	this->anc = anc;
	holdAngle = angle;
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