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
	drawShadow(dt, off);
#if 0
	if (mapNumber != player->mapNumber)
		return;
#endif

	if (get)
	{
		if (index == player->currWeaponIndex())
		{
			setPosition();
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

void Weapon::drawShadow(float dt, iPoint off)
{
	iPoint p = position + drawPos + off;
	setRGBA(0, 0, 0, 0.3f);
	iPoint v = iPointMake(fabs(vector.x), fabs(vector.y));
	fillEllipse(p.x, p.y, 25 + v.x * 25, 25 + v.y * 25);
	setRGBA(1, 1, 1, 1);
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

void Weapon::setPosition()
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

	position = player->wpPosition;
	vector = player->wpVector;
	drawPos = p;
	holdAngle = angle;
	img->angle = angle;
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
		if (getKeyDown(keyboard_j))
		{
			audioPlay(AUDIO_HammerAttack);
			attacking = true;
			img->ratio = 1.3f;
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

	float ang = m(d * d, 0.0f, attackAngle * 2) - attackAngle;
	//float ang = linear(d, 0.0f, 0.0f);
	float ran = m(d, 0.0f, attackRange);

	iPoint rp = iPointRotate(drawPos, iPointZero, ang);
	rp += position;
	position = rp - drawPos;
	touchRect = iRectMake(rp.x- 30, rp.y - 30, 60, 60);

	if (d > attackDt)
	{
		attackMonster();
		
		for (int i = 0; i < mapObjBrokenNum; i++)
		{
			if (containRect(touchRect, mapObjBroken[i]->touchRect))
			{
				mapObjBroken[i]->action(this);
			}
		}
	}

	img->angle = holdAngle + ang;

#if SHOW_TOUCHRECT
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
		img->ratio = 1.0f;
		img->angle = holdAngle;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;
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
		if (getKeyDown(keyboard_j))
		{
			audioPlay(AUDIO_SpearAttack);
			attacking = true;
			img->ratio = 1.2f;
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

	float ran = m(d, attackRange, 0.0f);

	iPoint rp = vector * ran + position + drawPos;
	position = rp - drawPos;

	float x = (vector.x == 0.0f ? img->tex->width : img->tex->height) * img->ratio * 0.5f;
	float y = (vector.y == 0.0f ? img->tex->width : img->tex->height) * img->ratio * 0.5f;
	touchRect = iRectMake(rp.x - x, rp.y - y, x * 2.0f, y * 2.0f);

	if (d < 1.0f - attackDt)
	{
		attackMonster();

		for (int i = 0; i < mapObjBrokenNum; i++)
		{
			if (containRect(touchRect, mapObjBroken[i]->touchRect))
			{
				mapObjBroken[i]->action(this);
			}
		}
	}

#if SHOW_TOUCHRECT
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 0.7f);
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
		img->ratio = 1.0f;
		img->angle = holdAngle;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;
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
	position = iPointZero;
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
		if (getKeyDown(keyboard_j))
		{
			audioPlay(AUDIO_CyclonAttack);
			attacking = true;
			img->ratio = 1.2f;
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
	float ran = _sin(180 * d) * 50;

	iPoint v = iPointVector(drawPos);
	iPoint rp = iPointRotate(drawPos + v * 20, iPointZero, ang);
	rp += position;
	position = rp - drawPos;

	img->angle = holdAngle + ang;
	touchRect = iRectMake(rp.x - 30, rp.y - 30, 60, 60);

	attackMonster();
	for (int i = 0; i < mapObjBrokenNum; i++)
	{
		if (containRect(touchRect, mapObjBroken[i]->touchRect))
		{
			mapObjBroken[i]->action(this);
		}
	}

#if SHOW_TOUCHRECT
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

	attackSpeed += dt;
	if (attackSpeed > standSpeed)
	{
		audioStop(AUDIO_CyclonAttack);
		attackSpeed = 0.0f;
		attackDelay = 0.0f;
		attacking = false;
		hit = false;
		img->ratio = 1.0f;
		img->angle = holdAngle;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;
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
			PlayerChar::cbPlayerSetAttack(NULL);
		}
		return false;
	}

	float d = attackSpeed / _attackSpeed;
	iPoint v = vector * -1.0f;
	position += v * d * 50;


	//attack	
	attackSpeed += dt;
	if (attackSpeed > _attackSpeed)
	{
		Projectile::fire(Projectile_BowGunArrow, this);

		attackSpeed = 0.0f;
		attackDelay = 0.0f;
		attacking = false;
		hit = false;
		PlayerChar::cbPlayerSetIdle(NULL);

		return false;
	}

	return true;
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
	position = iPointZero;
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

//---------------------------------------------------------------------------------------------

void loadWeapon()
{
	weaponNum = 0;
	weapon = (Weapon**)malloc(sizeof(Weapon*) * WT_max);

	iPoint p = iPointMake(-3000, -3000);
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
