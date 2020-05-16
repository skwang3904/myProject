#include "WMelee.h"

#include "Enemy.h"
#include "RgTile.h"
#include "Room.h"

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

	_meleeWP = (meleeWeapon**)malloc(sizeof(meleeWeapon*) * 2);

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

	nomalSpear->init(imgSpear, true, 40, 0.3f, 0.3f, 70.0f, 0.0f,iPointZero, iRectZero,false);
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
}

static iPoint mv = iPointZero;
void weaponPosition(meleeWeapon* mw, float dt, iPoint& wp)
{
	//임시
	static iPoint p = iPointMake(pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f,
		pc->playerPosition.y + HALF_OF_TEX_HEIGHT);

	if (mv.x < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y;
		angle = 90.0f;
	}
	else if (mv.x > 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f ;
		angle = 270.0f;
	}

	if (mv.y < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT;
		angle = 0.0f;
	}
	else if (mv.y > 0)
	{
		p.x = pc->playerPosition.x;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT; 
		angle = 180.0f;
	}

	wp = p;
}

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
	if (mw->attackEnemy == false && pc->act != attacking)
		return false;

	weaponPosition(mw, dt, mw->combatPosition);

	static float delta = 0.0f;

	float range = iRange;	
	float rangeRate = easeIn(delta / attTime, 0, range);

	float attAngle = iAngle;		
	float attAngleRate = easeIn(delta / attTime, 0, attAngle);

	float ratio = iRatio;
	float ratioRate = linear(delta / attTime, 1.0f, ratio);

	mw->combatPosition += mv * rangeRate;

	int vcen = VCENTER;
	int hcen = HCENTER;

	Texture* tex = mw->img->tex;

	iPoint wcp = mw->combatPosition;
	iPoint centerP = wcp;

	iRect rt = iRectZero;
	iSize size = iSizeMake(pc->img[0]->tex->width/2, mw->reach);
	iPoint pcPos = pc->playerPosition + pc->camPosition + setPos;

	if (mv.x < 0)
	{
		vcen = VCENTER;
		hcen = RIGHT;

		centerP.x = wcp.x - tex->height  / 2;

		rt.origin.x = pcPos.x + HALF_OF_TEX_WIDTH - size.height;
		rt.origin.y = pcPos.y;
		rt.size.width = size.height;
		rt.size.height = size.width;
	}
	else if (mv.x > 0)
	{
		vcen = VCENTER;
		hcen = LEFT;

		centerP.x = wcp.x + tex->height / 2;

		rt.origin.x = pcPos.x + HALF_OF_TEX_WIDTH;
		rt.origin.y = pcPos.y + HALF_OF_TEX_HEIGHT * 2 - size.width;
		rt.size.width = size.height;
		rt.size.height = size.width;
	}

	if (mv.y < 0)
	{
		vcen = BOTTOM;
		hcen = HCENTER;

		centerP.x = wcp.x;
		centerP.y = wcp.y - tex->height / 2;

		rt.origin.x = pcPos.x + HALF_OF_TEX_WIDTH * 2 - size.width;
		rt.origin.y = pcPos.y + HALF_OF_TEX_HEIGHT - size.height;
		rt.size.width = size.width;
		rt.size.height = size.height;
	}
	else if (mv.y > 0)
	{
		vcen = TOP;
		hcen = HCENTER;

		centerP.x = wcp.x;
		centerP.y = wcp.y + tex->height / 2;

		rt.origin.x = pcPos.x;
		rt.origin.y = pcPos.y + HALF_OF_TEX_HEIGHT;
		rt.size.width = size.width;
		rt.size.height = size.height;
	}

	setRGBA(0, 1, 0, 1);
	mw->hitBox = rt;
	fillRect(rt);

	setRGBA(1, 1, 1, 1);
	wcp = iPointRotate(centerP, wcp, attAngleRate);
	wcp += pc->camPosition + setPos;

	drawImage(tex,
		wcp.x,
		wcp.y,
		0, 0, tex->width, tex->height,
		VCENTER | HCENTER, ratioRate, ratioRate, 
		2, attAngleRate + angle + mw->holdAngle, REVERSE_NONE);

	for (int i = 0; i < ENEMY_NUM; i++) //enemy
	{
		if (containRect(mw->hitBox, enemys[i]->touchEnemyNomal))
		{
			enemys[i]->takeDmgEnemy(dt, mw->attackDmg);
		}
	}

	delta += dt;
	if (delta > attTime)
	{
		delta = 0.0f;
		pc->act = idle;
		mw->attackEnemy = false;

		draw(mw, dt, false, iPointZero);
		return false;
	}

	return true;
}

void draw(meleeWeapon* melee, float dt, bool drop, iPoint dropP)
{
	Texture* tex = melee->img->tex;

	if (drop == false)
	{
		weaponPosition(melee, dt, melee->combatPosition);
		static iPoint p = melee->combatPosition;

		if (mv.x < 0.0f)
		{
			p.x = melee->combatPosition.x - tex->height / 2;
			p.y = melee->combatPosition.y;
		}
		else if (mv.x > 0.0f)
		{
			p.x = melee->combatPosition.x + tex->height / 2;
			p.y = melee->combatPosition.y;
		}
		if (mv.y < 0.0f)
		{
			p.x = melee->combatPosition.x;
			p.y = melee->combatPosition.y - tex->height / 2;
		}
		else if (mv.y > 0.0f)
		{
			p.x = melee->combatPosition.x;
			p.y = melee->combatPosition.y + tex->height / 2;
		}

		drawImage(tex, 
			pc->camPosition.x + setPos.x + p.x ,
			pc->camPosition.y + setPos.y + p.y ,
			0, 0,	tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, angle + melee->holdAngle, REVERSE_NONE);
	}
	else
	{
		iPoint p = dropP;
		drawImage(tex, 
			pc->camPosition.x + setPos.x + p.x, 
			pc->camPosition.y + setPos.y + p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.2f, 1.2f, 2, 90, REVERSE_NONE);

		melee->hitBox = getHitBoxRect(tex,
			pc->camPosition.x + setPos.x + p.x,
			pc->camPosition.y + setPos.y + p.y,
			0, 0,	tex->width, tex->height,
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
	iRect iHitBox,
	bool iAttackEnemy)
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
	attackEnemy = iAttackEnemy;
}

void meleeWeapon::init()
{
	img = new iImage();
	melee = true;
	attackDmg = 30.0f;
	attackSpeed = 0.0f;
	_attackSpeed = 0.2f;
	reach = 50.0f;
	holdAngle = -10.0f;

	combatPosition = iPointZero;
	hitBox = iRectZero;
	attackEnemy = false;

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
	if (pc->act == evasion || pc->act == falling)
		return;

	meleeWeapon* mw = nomalSword;
	weaponVector(mw, dt);

	if (getKeyStat(keyboard_attack)  && mw->attackSpeed == 0 && pc->mw == mw &&
		pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= mw->_attackSpeed;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (attackMelee(mw, dt, mw->attackEnemy, 0.2f, 0.0f, 90.0f, 1.0f))
		return;

	draw(mw, dt, drop, dropP);
}

void nomalSpearMethod(float dt, bool drop, iPoint dropP)
{
	if (pc->act == evasion || pc->act == falling)
		return;

	meleeWeapon* mw = nomalSpear;
	weaponVector(mw, dt);

	if (getKeyStat(keyboard_attack) && mw->attackSpeed == 0 && pc->mw == mw &&
		pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= mw->_attackSpeed;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;


	if (attackMelee(mw, dt, mw->attackEnemy, 0.1f, 50.0f, 0.0f, 1.0f))
		return;

	draw(mw, dt, drop, dropP);
}