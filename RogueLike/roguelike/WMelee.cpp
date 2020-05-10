#include "WMelee.h"

#include "Player.h"
#include "RgTile.h"

meleeWeapon** _meleeWP;

meleeWeapon* nomalSword; //검
meleeWeapon* nomalSpear; //창

int meleeNum;

static int vertical = TOP;
static int horizon = LEFT;
static float angle = 0.0f;

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

	nomalSpear->init(imgSpear, true, 10, 0.3f, 0.3f, 70.0f, iPointZero, iRectZero);
	meleeNum++;

	_meleeWP[0] = nomalSword;
	_meleeWP[1] = nomalSpear;

}

void freeMeleeWeapon()
{
	for (int i = 0; i < meleeNum; i++)
	{
		if(_meleeWP[i]->img)
			delete _meleeWP[i]->img;
		free(_meleeWP[i]);
	}
	free(_meleeWP);

	meleeNum = 0;
}

void weaponPosition(meleeWeapon* mw, float dt, iPoint& wp)
{
	//임시
	static iPoint p = iPointMake(pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f,
		pc->playerPosition.y + HALF_OF_TEX_HEIGHT);

	if (pc->weaponVector.x < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH - mw->img->tex->height/2;
		p.y = pc->playerPosition.y;
		vertical = BOTTOM;
		horizon = RIGHT;
		angle = 90.0f;
	}
	else if (pc->weaponVector.x > 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH + mw->img->tex->height / 2;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f ;
		vertical = TOP;
		horizon = LEFT;
		angle = 270.0f;
	}

	if (pc->weaponVector.y < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT - mw->img->tex->height / 2;
		vertical = BOTTOM;
		horizon = LEFT;
		angle = 0.0f;
	}
	else if (pc->weaponVector.y > 0)
	{
		p.x = pc->playerPosition.x;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT + mw->img->tex->height / 2;
		vertical = TOP;
		horizon = RIGHT;
		angle = 180.0f;
	}

	wp = p;
}

static bool att = false;
static iPoint saveCP = iPointZero;
static iPoint mv = iPointZero;

void weaponVector(meleeWeapon* mw, float dt)
{
	if (getKeyStat(keyboard_left)) // 무기방향
	{
		mv.x = -1;
		mv.y = 0;

	}
	else if (getKeyStat(keyboard_right))
	{
		mv.x = 1;
		mv.y = 0;

	}
	if (getKeyStat(keyboard_up))
	{
		mv.x = 0;
		mv.y = -1;

	}
	else if (getKeyStat(keyboard_down))
	{
		mv.x = 0;
		mv.y = 1;
	}
}

bool attackMelee(meleeWeapon* mw ,float dt,bool attacking, bool drop, iPoint dropP)
{
	// 공격할때 움직이는 거리, 각도, 속도 등,,
	if (attacking == false || drop == true)
		return false;

	weaponPosition(mw, dt, mw->combatPosition);
	if (saveCP == iPointZero)
		saveCP = mw->combatPosition;

	static float delta = 0.0f;
	delta += dt;
	if (delta > 0.2f)
	{
		delta = 0.0f;
		mw->combatPosition = saveCP;
		saveCP = iPointZero;
		att = false;
		return false;
	}

	float range = 50.0f;
	float test = easeIn(delta / 0.1f, 0, range);

	iPoint p = mv * test;
	mw->combatPosition += p;

	iPoint cp = mw->combatPosition;
	Texture* tex = mw->img->tex;

	drawImage(tex, cp.x, cp.y, 0, 0,
		tex->width, tex->height,
		VCENTER | HCENTER, 1.0f, 1.0f, 2, angle, REVERSE_NONE);

	mw->hitBox = getHitBoxRect(tex, cp.x, cp.y, 0, 0,
		tex->width, tex->height,
		VCENTER | HCENTER, 1.0f, 1.0f, 2, angle);
	setRGBA(0, 1, 0, 1);
	drawRect(mw->hitBox);
	setRGBA(1, 1, 1, 1);

	return true;
}

//void attackMelee(meleeWeapon* wMelee, 'data...');

// 히트박스
// 이미지가 히트박스에 곂치면


void draw(meleeWeapon* melee, float dt, bool drop, iPoint dropP)
{
	Texture* tex = melee->img->tex;

	if (drop == false)
	{
		weaponPosition(melee,dt, melee->combatPosition);
		iPoint p = melee->combatPosition;

		drawImage(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, angle, REVERSE_NONE);

		melee->hitBox = getHitBoxRect(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, angle);
		setRGBA(0, 1, 0, 1);
		drawRect(melee->hitBox);
		setRGBA(1, 1, 1, 1);
	}
	else
	{
		iPoint p = dropP;
		drawImage(tex, p.x, p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 0, REVERSE_NONE);

		melee->hitBox = getHitBoxRect(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 0);
		setRGBA(0, 1, 0, 1);
		drawRect(melee->hitBox);
		setRGBA(1, 1, 1, 1);
	}
}

/////////////////////////////////////////////////////////////////////////////////

void meleeWeapon::init(
	iImage* iImg,
	bool isMelee,
	float iAttackDmg,
	float iAttackSpeed,
	float _iAttackSpeed,
	float iReach,
	iPoint iCombatPosition,
	iRect iHitBox)
{
	img = iImg;
	melee = isMelee;

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
	attackDmg = 10.0f;
	attackSpeed = 0.0f;
	_attackSpeed = 1.0f;
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
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "검");

	Texture* tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);
	//-----------------------------------------------------------

}
/////////////////////////////////////////////////////////////////////////////////

void nomalSwordMethod(float dt, bool drop, iPoint dropP)
{
	meleeWeapon* mw = nomalSword;
	weaponVector(mw, dt);

	if (getKeyDown(keyboard_attack)  && mw->attackSpeed == 0)
	{
		att = true;
		mw->attackSpeed -= mw->_attackSpeed;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed =(int)0;

	if (attackMelee(mw, dt, att, drop, dropP))
		return;

	draw(mw, dt, drop, dropP);

}

void nomalSpearMethod(float dt, bool drop, iPoint dropP)
{
	meleeWeapon* mw = nomalSpear;
	weaponVector(mw, dt);

	if (getKeyDown(keyboard_attack)&& mw->attackSpeed == 0 )
	{
		att = true;
		mw->attackSpeed -= mw->_attackSpeed;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (attackMelee(mw, dt, att, drop, dropP))
		return;

	draw(mw, dt, drop, dropP);
}