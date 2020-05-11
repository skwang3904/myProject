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

	nomalSpear->init(imgSpear, true, 10, 0.3f, 0.3f, 70.0f, 0.0f,iPointZero, iRectZero);
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
		//p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH - mw->img->tex->height/2;
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y;
		angle = 90.0f;
	}
	else if (pc->weaponVector.x > 0)
	{
		//p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH + mw->img->tex->height / 2;
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f ;
		angle = 270.0f;
	}

	if (pc->weaponVector.y < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f;
		//p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT - mw->img->tex->height / 2;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT;
		angle = 0.0f;
	}
	else if (pc->weaponVector.y > 0)
	{
		p.x = pc->playerPosition.x;
		//p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT +mw->img->tex->height / 2;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT; 
		angle = 180.0f;
	}

	wp = p;

}


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

bool attackMelee(meleeWeapon* mw ,float dt, bool att, float attTime,
	float iRange, float iAngle, float iRatio)
{
	// 공격할때 움직이는 거리, 각도, 속도 등,,
	if (attacking == false)
		return false;

	weaponPosition(mw, dt, mw->combatPosition);

	static float delta = 0.0f;

	float range = iRange;	
	float rangeRate = linear(delta / attTime, 0, range);

	float attAngle = iAngle;		
	float attAngleRate = linear(delta / attTime, 0, attAngle);

	float ratio = iRatio;
	float ratioRate = linear(delta / attTime, 1.0f, ratio);

	mw->combatPosition += mv * rangeRate;

	int vcen = VCENTER;
	int hcen = HCENTER;

	iPoint wcp = mw->combatPosition;
	iPoint centerP = wcp;

	Texture* tex = mw->img->tex;
	if (mv.x < 0)
	{
		vcen = VCENTER;
		hcen = RIGHT;

		centerP.x = wcp.x - tex->height  / 2;
	}
	else if (mv.x > 0)
	{
		vcen = VCENTER;
		hcen = LEFT;

		centerP.x = wcp.x + tex->height / 2;
	}
	if (mv.y <0)
	{ 
		vcen = BOTTOM;
		hcen = HCENTER;

		centerP.x = wcp.x;
		centerP.y = wcp.y - tex->height / 2;
	}
	else if (mv.y >0)
	{
		vcen = TOP;
		hcen = HCENTER;

		centerP.x = wcp.x;
		centerP.y = wcp.y +tex->height / 2;
	}

	wcp = iPointRotate(centerP, wcp, attAngleRate);
	drawImage(tex,wcp.x, wcp.y, 0, 0,
		tex->width, tex->height,
		VCENTER | HCENTER, ratioRate, ratioRate, 2, attAngleRate + angle + mw->holdAngle, REVERSE_NONE);

	delta += dt;
	if (delta > attTime)
	{
		delta = 0.0f;
		attacking = false;

		draw(mw, dt,mw->holdAngle, false, iPointZero);
		return false;
	}
	return true;
}

//void attackMelee(meleeWeapon* wMelee, 'data...');

// 히트박스
// 이미지가 히트박스에 곂치면


void draw(meleeWeapon* melee, float dt, float holdAngle, bool drop, iPoint dropP)
{
	Texture* tex = melee->img->tex;

	if (drop == false)
	{
		weaponPosition(melee, dt, melee->combatPosition);
		static iPoint p = melee->combatPosition;

		if (pc->weaponVector.x < 0.0f)
		{
			p.x = melee->combatPosition.x - tex->width;
			p.y = melee->combatPosition.y;
		}
		else if (pc->weaponVector.x > 0.0f)
		{
			p.x = melee->combatPosition.x + tex->width;
			p.y = melee->combatPosition.y;
		}
		if (pc->weaponVector.y < 0.0f)
		{
			p.x = melee->combatPosition.x;
			p.y = melee->combatPosition.y - tex->height / 2;
		}
		else if (pc->weaponVector.y > 0.0f)
		{
			p.x = melee->combatPosition.x;
			p.y = melee->combatPosition.y + tex->height / 2;
		}

		drawImage(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, angle + holdAngle, REVERSE_NONE);
	}
	else
	{
		iPoint p = dropP;
		drawImage(tex, p.x, p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 90, REVERSE_NONE);

		melee->hitBox = getHitBoxRect(tex, p.x, p.y, 0, 0,
			tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 90);
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
	float iHoldAngle,
	iPoint iCombatPosition,
	iRect iHitBox)
{
	img = iImg;
	melee = isMelee;

	 attackDmg = iAttackDmg;
	 attackSpeed = iAttackSpeed;
	 _attackSpeed = _iAttackSpeed;
	 reach = iReach;
	 holdAngle = iHoldAngle;

	 combatPosition = iCombatPosition;
	 hitBox = iHitBox;

	
}

void meleeWeapon::init()
{
	img = new iImage();
	melee = true;
	attackDmg = 10.0f;
	attackSpeed = 0.0f;
	_attackSpeed = 0.3f;
	reach = 10.0f;
	holdAngle = -10.0f;

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

	if (getKeyDown(keyboard_attack)  && mw->attackSpeed == 0 && pc->mw && 
		evasion == false && falling == false)
	{
		attacking = true;
		mw->attackSpeed -= mw->_attackSpeed;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (attackMelee(mw, dt, attacking, 0.1f, 0.0f, 90.0f, 1.0f))
		return;

	draw(mw, dt, mw->holdAngle, drop, dropP);

}

void nomalSpearMethod(float dt, bool drop, iPoint dropP)
{
	meleeWeapon* mw = nomalSpear;
}