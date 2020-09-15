#include "Weapon.h"

#include "Proc.h"
#include "Map.h"
#include "ProcData.h"
#include "PlayerChar.h"
#include "Monster.h"
#include "Projectile.h"

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

void Weapon::paint(float dt, iPoint off)
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

void Weapon::attackMonster()
{
	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		if (m->mapNumber == player->mapNumber)
		{
			if (containRect(touchRect, m->touchRect))
				m->action(this);
		}
	}
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

	index = -1;
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
	Texture* tex, * t;
	iSize size = iSizeMake(60, 100);

	img = new iImage();
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	t = createImage("assets/weapon/hammer_0.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);

	img->lockAngle = true;
	img->anc = VCENTER | HCENTER;
	this->img = img;


	touchRect = iRectMake(position.x - size.width / 2.0f, position.y - size.height / 2.0f,
		size.width, size.height);

	WeaponInfo* wi = &weaponInfo[WT_Hammer];
	attackPoint = wi->attackPoint;
	_attackPoint = wi->attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = wi->attackSpeed;
	attackDt = wi->attackDt;
	attackDelay = 0.0f;
	_attackDelay = wi->attackDelay;
	actionDt = 
	_actionDt = wi->actionDt;

	attackAngle = wi->attackAngle;
	attackRange = wi->attackRange;

	holdAngle = 90.0f;
	attacking = false;
	hit = false;
	get = false;
	drawPos = iPointZero;
}

Hammer::~Hammer()
{
	delete img;
}

#if 0
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
#endif

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

	if (d > attackDt)
	{
		img->angle = holdAngle + attackAngle;
		rp = iPointRotate(pdp, position, attackAngle);
		img->position = rp - pdp;

		attackMonster();
	}
	else
	{
		img->angle = holdAngle - attackAngle;
		rp = iPointRotate(pdp, position, -attackAngle);
		img->position = rp - pdp;
	}

#if 1
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

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
		p = iPointMake(-size.height * 0.5f, 0);
	}
	else if (v.x > 0.0f)
	{
		angle = 270.0f;
		p = iPointMake(size.height * 0.5f, 0);

	}
	if (v.y < 0.0f)
	{
		angle = 0.0f;
		p = iPointMake(0, -size.height * 0.5f);
	}
	else if (v.y > 0.0f)
	{
		angle = 180.0f;
		p = iPointMake(0, size.height * 0.5f);
	}

	vector = player->wpVector;
	drawPos = p;
	holdAngle = angle;
}

//---------------------------------------------------------------------------------------------

Spear::Spear(int index, int8 mapNum, iPoint pos) : Weapon(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex, *t;

	iSize size = iSizeMake(20, 100);

	img = new iImage();
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	t = createImage("assets/weapon/upg_spear.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);

	//img->angle = 180;
	img->lockAngle = true;
	img->anc = VCENTER | HCENTER;
	this->img = img;


	position = player->position + iPointMake(50, 50);
	touchRect = iRectMake(position.x - size.width / 2.0f, position.y - size.height / 2.0f,
		size.width, size.height);

	WeaponInfo* wi = &weaponInfo[WT_Spear];
	attackPoint = wi->attackPoint;
	_attackPoint = wi->attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = wi->attackSpeed;
	attackDt = wi->attackDt;
	attackDelay = 0.0f;
	_attackDelay = wi->attackDelay;
	actionDt =
	_actionDt = wi->actionDt;

	moveSpeed = wi->moveSpeed;
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
	delete img;
}

#if 0
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
#endif

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
	if(d > attackDt)
		PRINTF(d);
	typedef float (*FUNC_METHOD)(float, float, float);
	FUNC_METHOD fmethod[3] = { linear, easeIn, easeOut };
	FUNC_METHOD m = fmethod[1];

	float ang = m(d, 0.0f, attackAngle * 2) - attackAngle;
	//float ang = linear(d, 0.0f, 0.0f);
	float ran = m(d, 0.0f, attackRange);

	iPoint pdp = position + drawPos + (player->wpVector * ran) * 1.5f;
	iPoint rp = iPointRotate(pdp, position, ang);
	touchRect = iRectMake(rp.x - 15, rp.y - 15, 30, 30);

	if (d > attackDt)
	{
		img->angle = holdAngle + attackAngle;
		rp = iPointRotate(pdp, position, attackAngle);
		img->position = rp - pdp + (player->wpVector * attackRange);

		attackMonster();
	}
	else
	{
		img->angle = holdAngle - attackAngle;
		rp = iPointRotate(pdp, position, -attackAngle);
		img->position = rp - pdp + (player->wpVector * 0);
	}

#if 1
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

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
		p = iPointMake(-size.height * 0.5f, 0);
	}
	else if (v.x > 0.0f)
	{
		angle = 270.0f;
		p = iPointMake(size.height * 0.5f, 0);

	}
	if (v.y < 0.0f)
	{
		angle = 0.0f;
		p = iPointMake(0, -size.height * 0.5f);
	}
	else if (v.y > 0.0f)
	{
		angle = 180.0f;
		p = iPointMake(0, size.height * 0.5f);
	}

	vector = player->wpVector;
	drawPos = p;
	holdAngle = angle;
}

//--------------------------------------------------------
// Cyclone
Cyclone::Cyclone(int index, int8 mapNum, iPoint pos) : Weapon(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex, * t;
	iSize size = iSizeMake(60, 100);

	img = new iImage();
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	t = createImage("assets/weapon/upg_axeDouble.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);

	//img->angle = 180;
	img->lockAngle = true;
	img->anc = VCENTER | HCENTER;
	this->img = img;


	touchRect = iRectMake(position.x - size.width / 2.0f, position.y - size.height / 2.0f,
		size.width, size.height);

	WeaponInfo* wi = &weaponInfo[WT_Cyclone];
	attackPoint = wi->attackPoint;
	_attackPoint = wi->attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = wi->attackSpeed;
	attackDt = wi->attackDt;
	attackDelay = 0.0f;
	_attackDelay = wi->attackDelay;
	actionDt =
	_actionDt = wi->actionDt;

	moveSpeed = wi->moveSpeed;
	attackAngle = wi->attackAngle;
	attackRange = wi->attackRange;

	holdAngle = 90.0f;
	attacking = false;
	hit = false;
	get = false;
	drawPos = iPointZero;
}

Cyclone::~Cyclone()
{
	delete img;
}

#if 0
void Cyclone::paint(float dt, iPoint off)
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
#endif

void Cyclone::drawShadow(float dt, iPoint off)
{
}

void Cyclone::action(Object* obj)
{
}

bool Cyclone::attack(float dt)
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
	FUNC_METHOD m = fmethod[0];

	float ang = m(d, 0.0f, attackAngle);
	//float ang = linear(d, 0.0f, 0.0f);
	float ran = m(d, 0.0f, attackRange);

	iPoint pdp = position + drawPos;
	iPoint rp = iPointRotate(pdp, position, ang);
	touchRect = iRectMake(rp.x - 15, rp.y - 15, 30, 30);

	attackMonster();

	img->angle = holdAngle + ang;
	img->position = rp - pdp;

#if 1
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

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

void Cyclone::setPosition()
{
	iPoint v = player->wpVector;
	float angle = holdAngle;
	iPoint p = drawPos;
	iSize size = iSizeMake(img->tex->width, img->tex->height);

	if (v.x < 0.0f)
	{
		angle = 90.0f;
		p = iPointMake(-size.height * 0.5f, 0);
	}
	else if (v.x > 0.0f)
	{
		angle = 270.0f;
		p = iPointMake(size.height * 0.5f, 0);

	}
	if (v.y < 0.0f)
	{
		angle = 0.0f;
		p = iPointMake(0, -size.height * 0.5f);
	}
	else if (v.y > 0.0f)
	{
		angle = 180.0f;
		p = iPointMake(0, size.height * 0.5f);
	}

	vector = player->wpVector;
	drawPos = p;
	holdAngle = angle;
}

//---------------------------------------------------------------------------------------------

BowGun::BowGun(int index, int8 mapNum, iPoint pos) : Weapon(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex, * t;
	iSize size = iSizeMake(100, 100);

	img = new iImage();
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	t = createImage("assets/weapon/BowGun.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);

	//img->angle = 180;
	img->lockAngle = true;
	img->anc = VCENTER | HCENTER;
	this->img = img;


	WeaponInfo* wi = &weaponInfo[WT_BowGun];
	attackPoint = wi->attackPoint;
	_attackPoint = wi->attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = wi->attackSpeed;
	attackDt = wi->attackDt;
	attackDelay = 0.0f;
	_attackDelay = wi->attackDelay;
	actionDt =
	_actionDt = wi->actionDt;

	moveSpeed = wi->moveSpeed;// projectile Speed
	attackAngle = wi->attackAngle;
	attackRange = wi->attackRange; 

	holdAngle = 90.0f;
	attacking = false;
	hit = false;
	get = false;
	drawPos = iPointZero;
}

BowGun::~BowGun()
{
	delete img;
}

#if 0
void BowGun::paint(float dt, iPoint off)
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
#endif

void BowGun::drawShadow(float dt, iPoint off)
{
}

void BowGun::action(Object* obj)
{
}

bool BowGun::attack(float dt)
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
			Projectile::fire(Projectile_BowGunArrow, this);
			PlayerChar::cbPlayerSetAttack(NULL);
		}
		return false;
	}

	//attack	
	attackSpeed += dt;
	if (attackSpeed > _attackSpeed)
	{
		attackSpeed = 0.0f;
		attackDelay = 0.0f;
		attacking = false;
		hit = false;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;
}

void BowGun::setPosition()
{
	iPoint v = player->wpVector;
	float angle = holdAngle;
	iPoint p = drawPos;
	iSize size = iSizeMake(img->tex->width, img->tex->height);

	if (v.x < 0.0f)
	{
		angle = 90.0f;
		p = iPointMake(-size.height * 0.5f, 0);
	}
	else if (v.x > 0.0f)
	{
		angle = 270.0f;
		p = iPointMake(size.height * 0.5f, 0);

	}
	if (v.y < 0.0f)
	{
		angle = 0.0f;
		p = iPointMake(0, -size.height * 0.5f);
	}
	else if (v.y > 0.0f)
	{
		angle = 180.0f;
		p = iPointMake(0, size.height * 0.5f);
	}

	vector = player->wpVector;
	drawPos = p;
	holdAngle = angle;
}

//---------------------------------------------------------------------------------------------

MagicWand::MagicWand(int index, int8 mapNum, iPoint pos) : Weapon(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex, * t;
	iSize size = iSizeMake(100, 100);

	img = new iImage();
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	t = createImage("assets/weapon/MagicWand.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);

	//img->angle = 180;
	img->lockAngle = true;
	img->anc = VCENTER | HCENTER;
	this->img = img;


	WeaponInfo* wi = &weaponInfo[WT_BowGun];
	attackPoint = wi->attackPoint;
	_attackPoint = wi->attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = wi->attackSpeed;
	attackDt = wi->attackDt;
	attackDelay = 0.0f;
	_attackDelay = wi->attackDelay;
	actionDt =
		_actionDt = wi->actionDt;

	moveSpeed = wi->moveSpeed;// projectile Speed
	attackAngle = wi->attackAngle;
	attackRange = wi->attackRange;

	holdAngle = 90.0f;
	attacking = false;
	hit = false;
	get = false;
	drawPos = iPointZero;
}

MagicWand::~MagicWand()
{
	delete img;
}

#if 0
void MagicWand::paint(float dt, iPoint off)
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
#endif

void MagicWand::drawShadow(float dt, iPoint off)
{
}

void MagicWand::action(Object* obj)
{
}

bool MagicWand::attack(float dt)
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
			Projectile::fire(Projectile_FireBall, this);
			PlayerChar::cbPlayerSetAttack(NULL);
		}
		return false;
	}

	//attack	
	attackSpeed += dt;
	if (attackSpeed > _attackSpeed)
	{
		attackSpeed = 0.0f;
		attackDelay = 0.0f;
		attacking = false;
		hit = false;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;

}

void MagicWand::setPosition()
{
	iPoint v = player->wpVector;
	float angle = holdAngle;
	iPoint p = drawPos;
	iSize size = iSizeMake(img->tex->width, img->tex->height);

	if (v.x < 0.0f)
	{
		angle = 90.0f;
		p = iPointMake(-size.height * 0.5f, 0);
	}
	else if (v.x > 0.0f)
	{
		angle = 270.0f;
		p = iPointMake(size.height * 0.5f, 0);

	}
	if (v.y < 0.0f)
	{
		angle = 0.0f;
		p = iPointMake(0, -size.height * 0.5f);
	}
	else if (v.y > 0.0f)
	{
		angle = 180.0f;
		p = iPointMake(0, size.height * 0.5f);
	}

	vector = player->wpVector;
	drawPos = p;
	holdAngle = angle;

}

//---------------------------------------------------------------------------------------------

void loadWeapon()
{
	weaponNum = 0;
	weapon = (Weapon**)malloc(sizeof(Weapon*) * WT_max);

	iPoint p = player->position + iPointMake(30, 0);
	weapon[weaponNum] = new Hammer(-1, player->mapNumber, p); weaponNum++;
	weapon[weaponNum] = new Spear(-1, player->mapNumber, p);  weaponNum++;
	weapon[weaponNum] = new Cyclone(-1, player->mapNumber, p);  weaponNum++;
	weapon[weaponNum] = new BowGun(-1, player->mapNumber, p);  weaponNum++;
	weapon[weaponNum] = new MagicWand(-1, player->mapNumber, p);  weaponNum++;

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
	weapon = NULL;
}

void drawWeapon(float dt)
{
	for (int i = 0; i < weaponNum; i++)
		weapon[i]->paint(dt, DRAW_OFF);
}
