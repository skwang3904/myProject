#include "WMelee.h"

#include "Player.h"
#include "RgTile.h"

meleeWeapon** _meleeWP;

meleeWeapon* nomalSword; //검
meleeWeapon* nomalSpear; //창

int meleeNum;

void createMeleeWeapon()
{
	meleeNum = 0;

	_meleeWP = (meleeWeapon**)malloc(sizeof(meleeWeapon*) * 1);

	nomalSword = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	nomalSword->init();
	meleeNum++;

	nomalSpear = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	//-----------------------------------------------------------

	iImage* imgSpear = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(10, 70);
	g->init(size);

	setRGBA(0.5, 0, 0.5, 1);
	g->fillRect(0, 0, size.width, size.height);
	setRGBA(1, 1, 1, 1);

	setStringSize(10);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "창");

	Texture* tex = g->getTexture();
	imgSpear->addObject(tex);
	freeImage(tex);
	//-----------------------------------------------------------

	char name= 'a';
	nomalSpear->init(imgSpear, true, name, 10, 0.3f, 0.3f, 70.0f, iPointZero, iRectZero);
	meleeNum++;

	_meleeWP[0] = nomalSword;
	_meleeWP[1] = nomalSpear;
}

void freeMeleeWeapon()
{
	for (int i = 0; i < meleeNum; i++)
		delete _meleeWP[i]->img;
	free(_meleeWP);

	meleeNum = 0;
}

static int vertical = TOP;
static int horizon = LEFT;
static float angle = 0.0f;

iPoint weaponPosition(float dt, iPoint& wp)
{
	//임시
	static iPoint p = iPointMake(pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f,
		pc->playerPosition.y + HALF_OF_TEX_HEIGHT);

	if (pc->weaponVector.x < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y + 15;
		vertical = BOTTOM;
		horizon = RIGHT;
		angle = 90.0f;
	}
	else if (pc->weaponVector.x > 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f - 15;
		vertical = TOP;
		horizon = LEFT;
		angle = 270.0f;
	}

	if (pc->weaponVector.y < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT;
		vertical = BOTTOM;
		horizon = LEFT;
		angle = 0.0f;
	}
	else if (pc->weaponVector.y > 0)
	{
		p.x = pc->playerPosition.x;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT;
		vertical = TOP;
		horizon = RIGHT;
		angle = 180.0f;
	}

	wp = p;
	return p;
}

void attackMelee(meleeWeapon* mw ,float dt, bool drop, iPoint dropP)
{
	// 공격할때 움직이는 거리, 각도, 속도 등,,
	static iPoint index = pc->weaponVector;

	mw->attackSpeed = 0.0f;
	mw->attackSpeed += dt;
	if (mw->attackSpeed < mw->_attackSpeed)
	{
		iPoint p = pc->weaponVector;
		float range = 100.0f;

		mw->combatPosition += p * (range * dt);
		index += p * (range * dt);
	}
	else
	{
		mw->combatPosition -= index;
		mw->attackSpeed -= mw->_attackSpeed;
		index = pc->weaponVector;
	}
}

//void attackMelee(meleeWeapon* wMelee, 'data...');

void draw(meleeWeapon* melee, float dt, bool drop, iPoint dropP)
{
	Texture* tex = melee->img->tex;

	if (drop == false)
	{
		iPoint p = weaponPosition(dt, melee->combatPosition);

		drawImage(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			vertical | horizon, 1.0f, 1.0f, 2, angle, REVERSE_NONE);
		setRGBA(0, 1, 0, 1);

		melee->hitBox = drawHitBox(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			vertical | horizon, 1.0f, 1.0f, 2, angle);
		drawRect(melee->hitBox);

		setRGBA(1, 1, 1, 1);
	}
	else
	{
		iPoint p = dropP;
		drawImage(tex, p.x, p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 0, REVERSE_NONE);

		setRGBA(0, 1, 0, 1);
		melee->hitBox = drawHitBox(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 0);
		drawRect(melee->hitBox);

		setRGBA(1, 1, 1, 1);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void meleeWeapon::init(
	iImage* iImg,
	bool isMelee,
	char iName,
	float iAttackDmg,
	float iAttackSpeed,
	float _iAttackSpeed,
	float iReach,
	iPoint iCombatPosition,
	iRect iHitBox)
{
	img = iImg;
	melee = isMelee;
	name[64] = iName;

	 attackDmg = iAttackDmg;
	 attackSpeed = iAttackSpeed;
	 _attackSpeed = _iAttackSpeed;
	 reach = iReach;

	 combatPosition = iCombatPosition;
	 hitBox = iHitBox;

	
}

void meleeWeapon::init()
{
	img = new iImage();
	melee = true;
	strcpy(name, "검");
	attackDmg = 10.0f;
	attackSpeed = 0.5f;
	_attackSpeed = 0.5f;
	reach = 10.0f;

	combatPosition = iPointZero;
	hitBox = iRectZero;

	//-----------------------------------------------------------
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(20, 50);
	g->init(size);


	setRGBA(1, 0, 0, 1);
	g->fillRect(0, 0, size.width, size.height);
	setRGBA(1, 1, 1, 1);

	setStringSize(10);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "%s", name);

	Texture* tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);
	//-----------------------------------------------------------

}
/////////////////////////////////////////////////////////////////////////////////

void nomalSwordMethod(float dt,bool drop, iPoint dropP)
{
	if(drop == false && getKeyDown(keyboard_attack))
		attackMelee(nomalSword, dt, drop, dropP);
	draw(nomalSword, dt, drop, dropP);

}

void nomalSpearMethod(float dt, bool drop, iPoint dropP)
{
	draw(nomalSpear, dt, drop, dropP);

}