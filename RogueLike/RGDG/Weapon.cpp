#include "Weapon.h"

#include "ProcData.h"
#include "PlayerChar.h"
#include "Map.h"
#include "Monster.h"

Weapon** weapon;
int weaponNum;

Weapon** weaponList;
int currWeapon;

Weapon::Weapon(int index) : Object(index)
{
	attackPoint	 = 0.0f;
	_attackPoint = 0.0f;
	attackSpeed  = 0.0f;
	_attackSpeed = 0.0f;
	attackDelay  = 0.0f;
	_attackDelay = 0.0f;
	attackAngle  = 0.0f;
	attackRange  = 0.0f;

	holdAngle = 0.0f;
	attacking = false;
	hit = false;
	get = false;

	drawPos = iPointZero;
}

Weapon::~Weapon()
{
}

//--------------------------------------------------------
// Hammer
Hammer* hammer;

Hammer::Hammer(int index) : Weapon(index)
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
	this->img->lockAngle = true;
	this->img->anc = VCENTER | HCENTER;
	mapNumber = 0;

	position = player->position + iPointMake(30, 0);
	touchRect = iRectMake(position.x - size.width / 2.0f, position.y - size.height / 2.0f,
		size.width, size.height);

	WeaponInfo* wi = &weaponInfo[0];
	attackPoint = wi->_attackPoint;
	_attackPoint = wi->_attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = wi->_attackSpeed;
	attackDelay = 0.0f;
	_attackDelay = wi->_attackDelay;
	attackAngle = wi->attackAngle;
	attackRange = wi->attackRange;

	holdAngle = 90.0f;
	attacking = false;
	hit = false;
	get = true;
	drawPos = iPointZero;

	player->arrayWeapon->addObject(this);
}

Hammer::~Hammer()
{
}

void Hammer::paint(float dt, iPoint off)
{
#if 0
	if (mapNumber != player->mapNumber)
		return;
#endif

	if (get)
	{
		if ((Weapon*)player->arrayWeapon->curr == this)
		{
			setPosition();
			position = player->wpPosition;
			img->angle = holdAngle;
			if (!attack(dt))
			{
				if (getKeyDown(keyboard_o))
					dropWeapon();
			}
		}
		else
		{
			position = iPointMake(-3000, -3000);
		}
	}
	else
	{
		if (getWeapon())
			printf("getWeapon\n");
	}

	img->paint(dt, position + drawPos + off);
}

bool Hammer::attack(float dt)
{
	//if (attackDelay < _attackDelay)
	if (attackDelay < 0.2)
	{
		attackDelay += dt;
		return false;
	}

	if (attacking == false)
	{
		if (getKeyStat(keyboard_j))
			attacking = true;
		return false;
	}

	//attack	
	float d = attackSpeed / _attackSpeed;
	float ang = linear(d, 0.0f, attackAngle);
	//float ang = linear(d, 0.0f, 0.0f);
	float ran = linear(d, 0.0f, attackRange);

	iPoint pdp = position + drawPos;
	iPoint rp = iPointRotate(pdp, position, ang);
	touchRect = iRectMake(rp.x - 15, rp.y - 15, 30, 30);

	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		if (m->mapNumber == player->mapNumber)
		{
			if (containRect(touchRect, m->touchRect))
				m->getDmg(100);
		}
	}

#if 0
	iPoint drawrp = rp + DRAW_OFF;
	setRGBA(0, 1, 1, 1);
	fillRect(drawrp.x, drawrp.y, 30, 30);
	setRGBA(0, 1, 0, 1);

	iPoint posp = position + DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(posp.x, posp.y, 15, 15);
	setRGBA(1, 1, 1, 1);

	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

	img->angle = holdAngle + ang;
	img->position = rp - pdp;

	attackSpeed += dt;
	if (attackSpeed > _attackSpeed)
	{
		attackSpeed = 0.0f;
		attackDelay = 0.0f;
		attacking = false;
		hit = false;
		img->angle = holdAngle;
		img->position = iPointZero;

		return false;
	}

	return true;
}

bool Hammer::getWeapon()
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

void Hammer::dropWeapon()
{
	mapNumber = player->mapNumber; // 안쓰지만 해둠
	position = player->position;
	drawPos = iPointZero;
}

#if 0
void te()
{
	attack(iPoint, iPoint, iSize)
		-> iPoint에서 iPoint 까지 선 

}
#endif

void Hammer::setPosition()
{
	iPoint v = player->wpVector;
	float angle = holdAngle;
	iPoint p = drawPos;
	iSize size = iSizeMake(img->tex->width, img->tex->height);

	if (v.x < 0.0f)
	{
		angle = 90.0f;
		p = iPointMake(-size.height * 0.5f, size.width * 0.5f);
	}
	else if (v.x > 0.0f)
	{
		angle = 270.0f;
		p = iPointMake(size.height * 0.5f, size.width * 0.5f);

	}
	if (v.y < 0.0f)
	{
		angle = 180.0f;
		p = iPointMake(size.width * 0.5f, -size.height * 0.5f);
	}
	else if (v.y > 0.0f)
	{
		angle = 0.0f;
		p = iPointMake(-size.width * 0.5f, size.height * 0.5f);
	}

	drawPos = p;
	holdAngle = angle;
}

//--------------------------------------------------------

void loadWeapon()
{
	weapon = (Weapon**)malloc(sizeof(Weapon*) * 10);

	hammer = new Hammer(0);
	weapon[0] = hammer;
	weaponNum++;
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

