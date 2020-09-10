#include "Weapon.h"

#include "Map.h"
#include "ProcData.h"
#include "PlayerChar.h"
#include "Monster.h"

Weapon** weapon = NULL;
int weaponNum;

Weapon::Weapon(int index, int8 mapNum, iPoint pos) : Object(index, mapNum, pos)
{
	attackDelay  = _attackDelay = 0.0f;
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

void Weapon::getWeapon()
{
	Weapon* w = (Weapon*)player->arrayWeapon->objectAtIndex(player->currWeaponIndex());
	if (w->attacking)
		return;

	if (containRect(touchRect, player->touchRect))
	{
		get = true;
		player->addWeapon(this);
		index = player->currWeaponIndex();
	}
}

void Weapon::addThisWeapon()
{
	get = true;
	player->addWeapon(this);
	index = player->currWeaponIndex();
}

void Weapon::dropWeapon()
{
	if (player->arrayWeapon->count == 1)
		return;

	player->removeCurrWeapon();
	
	for (int i = 0; i < weaponNum; i++)
	{
		if (weapon[i]->index > index)
		{
			weapon[i]->index--;
			st->weaponData[i].index--;
		}
	}

	get = false;
	position = player->position;
	touchRect = iRectMake(position.x, position.y, img->tex->width, img->tex->height);
	drawPos = iPointZero;
}

//--------------------------------------------------------
// Hammer
Hammer* hammer;

Hammer::Hammer(int index, int8 mapNum, iPoint pos) : Weapon(index, mapNum, pos)
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
	get = false;
	drawPos = iPointZero;

	//player->addWeapon(this);
	//this->index = player->currWeaponIndex();
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
		if (index == player->currWeaponIndex())
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
		if (getKeyDown(keyboard_i))
			getWeapon();
	}

	img->paint(dt, position + drawPos + off);
}

void Hammer::drawShadow(float dt, iPoint off)
{
}

void Hammer::action(Object* obj)
{
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
		{
			attacking = true;
			PlayerChar::cbPlayerSetAttack(NULL);
		}
		return false;
	}

	//attack	
	float standSpeed = _attackSpeed;
	float d = attackSpeed / standSpeed;

	typedef float (*FUNC_METHOD)(float, float, float);
	FUNC_METHOD fmethod[3] = { linear, easeIn, easeOut };
	FUNC_METHOD m = fmethod[2];

	float ang = m(d, 0.0f, attackAngle * 2) - attackAngle;
	//float ang = linear(d, 0.0f, 0.0f);
	float ran = m(d, 0.0f, attackRange);

	iPoint pdp = position + drawPos;
	iPoint rp = iPointRotate(pdp, position, ang);
	touchRect = iRectMake(rp.x - 15, rp.y - 15, 30, 30);

	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		if (m->mapNumber == player->mapNumber)
		{
			if (containRect(touchRect, m->touchRect))
				m->action(this);
		}
	}

#if 1
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

	img->angle = holdAngle + ang;
	img->position = rp - pdp;

	attackSpeed += dt;
	if (attackSpeed > standSpeed)
	{
		attackSpeed = 0.0f;
		attackDelay = 0.0f;
		attacking = false;
		hit = false;
		img->angle = holdAngle;
		img->position = iPointZero;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;
}

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

//---------------------------------------------------------------------------------------------

Spear::Spear(int index, int8 mapNum, iPoint pos) : Weapon(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex;

	imgNum = 1;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);

	iSize size = iSizeMake(10, 100);
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

	position = player->position + iPointMake(50, 50);
	touchRect = iRectMake(position.x - size.width / 2.0f, position.y - size.height / 2.0f,
		size.width, size.height);

	WeaponInfo* wi = &weaponInfo[1];
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
	get = false;
	drawPos = iPointZero;

}

Spear::~Spear()
{
}

void Spear::paint(float dt, iPoint off)
{
#if 0
	if (mapNumber != player->mapNumber)
		return;
#endif

	if (get)
	{
		if (index == player->currWeaponIndex())
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
		if (getKeyDown(keyboard_i))
			getWeapon();
	}

	img->paint(dt, position + drawPos + off);
}

void Spear::drawShadow(float dt, iPoint off)
{
}

void Spear::action(Object* obj)
{
}


bool Spear::attack(float dt)
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
		{
			attacking = true;
			PlayerChar::cbPlayerSetAttack(NULL);
		}
		return false;
	}

	//attack	
	float standSpeed = _attackSpeed;
	float d = attackSpeed / standSpeed;

	typedef float (*FUNC_METHOD)(float, float, float);
	FUNC_METHOD fmethod[3] = { linear, easeIn, easeOut };
	FUNC_METHOD m = fmethod[1];

	float ang = m(d, 0.0f, attackAngle * 2) - attackAngle;
	//float ang = linear(d, 0.0f, 0.0f);
	float ran = m(d, 0.0f, attackRange);

	iPoint pdp = position + drawPos + (player->wpVector * ran) * 1.5f;
	iPoint rp = iPointRotate(pdp, position, ang);
	touchRect = iRectMake(rp.x - 15, rp.y - 15, 30, 30);

	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		if (m->mapNumber == player->mapNumber)
		{
			if (containRect(touchRect, m->touchRect))
				m->action(this);
		}
	}


#if 1
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

	img->angle = holdAngle + ang;
	img->position = rp - pdp + (player->wpVector * ran);

	attackSpeed += dt;
	if (attackSpeed > standSpeed)
	{
		attackSpeed = 0.0f;
		attackDelay = 0.0f;
		attacking = false;
		hit = false;
		img->angle = holdAngle;
		img->position = iPointZero;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;
}


void Spear::setPosition()
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

//---------------------------------------------------------------------------------------------

void loadWeapon()
{
	weaponNum = 0;
	weapon = (Weapon**)malloc(sizeof(Weapon*) * 10);

	weapon[weaponNum] = new Hammer(-1, player->mapNumber, iPointZero); weaponNum++;
	weapon[weaponNum] = new Spear(-1, player->mapNumber, iPointZero);  weaponNum++;

	int index = 0;
	for (int i = 0; i < weaponNum; i++)
	{
		if (st->weaponData[i].index == index)
		{
			weapon[i]->addThisWeapon();
			i = -1;
			index++;
		}
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
