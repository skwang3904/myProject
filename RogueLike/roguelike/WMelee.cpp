#include "WMelee.h"

#include "Room.h"

#include "Enemy.h"
#include "EnemyStruct.h"

#include "Weapon.h"

meleeWeapon* nomalSword;
meleeWeapon* nomalSpear;
meleeWeapon* nomalCyclone;

PlayerMW PMW[MELEE_NUM];

void meleeWeapon::init(
	iImage* iImg,
	bool isMelee,
	float iAttackDmg,
	float iAttackSpeed,
	float iWidthReach,
	float iHeightReach,
	float iHoldAngle)
{
	img = iImg;
	melee = isMelee;

	attackDmg = iAttackDmg;
	attackSpeed = _attackSpeed = iAttackSpeed;
	widthReach = iWidthReach;
	heightReach = iHeightReach;
	holdAngle = iHoldAngle;

	combatPosition = iPointZero;
	hitBox = iRectZero;
	attackEnemy = false;
}

/////////////////////////////////////////////////////////////////////////////////

void createMeleeWeapon()
{
	nomalSword = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	iImage* imgSword = new iImage();

	//-----------------------------------------------------------

	nomalSpear = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	iImage* imgSpear = new iImage();

	//-----------------------------------------------------------

	nomalCyclone = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	iImage* imgCyclon= new iImage();

	//-----------------------------------------------------------

	for (int i = 0; i < 2; i++)
	{
		Texture* texSword = createImage("assets/weapon/hammer%d.png",i);
		Texture* texSpear = createImage("assets/weapon/upg_spear%d.png",i);
		Texture* texCyclon = createImage("assets/weapon/upg_axeDouble%d.png",i);
		
		imgSword->addObject(texSword);
		imgSpear->addObject(texSpear);
		imgCyclon->addObject(texCyclon);

		freeImage(texSword);
		freeImage(texSpear);
		freeImage(texCyclon);
	}

	// img, ismelee, attackDmg, attackSpeed, widthReach, heightReach, holeAngle
	nomalSword->init(imgSword, true, 30, 0.3f, 30.0f, 60.0f, -30.0f);
	nomalSpear->init(imgSpear, true, 20, 0.2f, 10.0f, 70.0f, -45.0f);
	nomalCyclone->init(imgCyclon, true, 30, 0.5f, 30.0f, 50.0f, -70.0f);

	PMW[0] = { nomalSword,nomalSwordMethod };
	PMW[1] = { nomalSpear,nomalSpearMethod };
	PMW[2] = { nomalCyclone,nomalCycloneMethod };

	for (int i = 0; i < MELEE_NUM; i++)
	{
		PMW[i].pos = iPointZero;
		PMW[i].drop = true;
	}
}

void freeMeleeWeapon(PlayerMW* pmw)
{
	if (pmw->mw->img)
		delete pmw->mw->img;
	free(pmw->mw);
}

void draw(meleeWeapon* mw, float dt, iPoint dropP)
{
	Texture* tex = mw->img->tex;
	if (pc->pmw->mw == mw)
	{
		iPoint centerP = mw->combatPosition;

		iRect rt;
		weaponPosAndRt(mw, mw->combatPosition, centerP, rt);

		drawImage(tex, 
			pc->camPosition.x + setPos.x + centerP.x ,
			pc->camPosition.y + setPos.y + centerP.y ,
			0, 0,	tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, pc->combatAngleV + mw->holdAngle, REVERSE_NONE);
	}
	else
	{
		iPoint p = dropP;
		drawImage(tex, 
			pc->camPosition.x + setPos.x + p.x, 
			pc->camPosition.y + setPos.y + p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.2f, 1.2f, 2, 90, REVERSE_NONE);

		mw->hitBox = getHitBoxRect(tex,
			 p.x,
			 p.y,
			0, 0,	tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 90);

		printf("mw %.2f  // %.2f\n", mw->hitBox.origin.x, mw->hitBox.origin.y);
		printf("pc %.2f  // %.2f\n", pc->playerPosition.x, pc->playerPosition.y);
		iRect mt = mw->hitBox;
		mt.origin += pc->camPosition + setPos;
		setRGBA(0, 1, 0, 1);
		drawRect(mt);
		setRGBA(1, 1, 1, 1);
	}
}

void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt)
{
	Texture* tex = mw->img->tex;
	iSize size = iSizeMake(mw->widthReach, mw->heightReach);

	iPoint mv = pc->combatV;
	float angle = pc->combatAngleV;
	if (mv.x < 0)
	{
		wcp.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH + tex->height / 10.0f;
		wcp.y = pc->playerPosition.y + tex->width / 10.0f;
		centerP.x = wcp.x - tex->height / 2;
		centerP.y = wcp.y;

		rt.size.width = size.height;
		rt.size.height = size.width;
		angle = 90.0f;
	}
	else if (mv.x > 0)
	{
		wcp.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH - tex->height / 10.0f;
		wcp.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f - tex->width / 10.0f;
		centerP.x = wcp.x + tex->height / 2;
		centerP.y = wcp.y;

		rt.size.width = size.height;
		rt.size.height = size.width;
		angle = 270.0f;
	}

	if (mv.y < 0)
	{
		wcp.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f - tex->width / 5.0f;
		wcp.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT + tex->height / 10.0f;
		centerP.x = wcp.x;
		centerP.y = wcp.y - tex->height / 2;

		rt.size.width = size.width;
		rt.size.height = size.height;
		angle = 0.0f;
	}
	else if (mv.y > 0)
	{
		wcp.x = pc->playerPosition.x + tex->width / 5.0f;
		wcp.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT - tex->height / 10.0f;
		centerP.x = wcp.x;
		centerP.y = wcp.y + tex->height / 2;

		rt.size.width = size.width;
		rt.size.height = size.height;
		angle = 180.0f;
	}
	pc->combatAngleV = angle;
}

//------------------------------------------------------------------------------------

void hitMonster(meleeWeapon* mw, float dt)
{
	for (int i = 0; i < ALLENEMY_NUM; i++) //enemy
	{
		if (i < GOLEM_NUM)
		{
			if (containRect(mw->hitBox, golems[i]->touchGolem))
				golems[i]->takeDmgEnemy(dt, mw->attackDmg);
		}
		else
		{
			if (containRect(mw->hitBox, golemEletes[0]->touchGolem))
				golemEletes[0]->takeDmgEnemy(dt, mw->attackDmg);
		}
	}
}

//------------------------------------------------------------------------------------
// 무기종류, 시간, 공격중인지,공격하는데 걸리는시간, 찌르는 거리, 휘두르는 각도, 배율X, 배율Y,
bool nomalSworadAttack(meleeWeapon* mw, float dt, bool att, float attTime,
	float iRange, float iAngle, float iRatioX, float iRatioY)
{
	if (pc->pmw->mw != mw)
		return false;

	if (mw->attackEnemy == false && pc->act != attacking)
		return false;
	//weaponPosition(mw, dt, mw->combatPosition);

	static float delta = 0.0f;
	float d = 0.0f;

	if (delta / attTime < 0.6f) 		d = 0.0f;
	else if (delta / attTime < 0.65f)	d = 0.3f;
	else if (delta / attTime < 0.7f)	d = 0.6f;
	else								d = 1.0f;

	float range = iRange;
	float rangeRate = linear(d, 0, range);

	float attAngle = iAngle;
	float attAngleRate = linear(d, 0, attAngle * 2);

	float ratioX = iRatioX; // 수정필요
	float ratioRate = linear(d, 1.0, iRatioX);

	Texture* tex = mw->img->tex;
	iPoint wcp = mw->combatPosition;
	iPoint centerP = wcp;
	iRect rt = iRectZero;

	weaponPosAndRt(mw, wcp,centerP, rt);

	wcp = iPointRotate(centerP, wcp, attAngleRate - attAngle);
	wcp += pc->combatV * rangeRate;
	wcp += pc->camPosition + setPos;

	rt.origin = wcp - iPointMake(rt.size.width/2.0f, rt.size.height/2.0f);
	mw->hitBox = rt;

	setRGBA(0, 1, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

	drawImage(tex,
		wcp.x,
		wcp.y,
		0, 0, tex->width, tex->height,
		VCENTER | HCENTER, ratioRate, ratioRate,
		2, attAngleRate - attAngle + pc->combatAngleV + mw->holdAngle, REVERSE_NONE);

	if (d > 0.5f)
	{
		hitMonster(mw, dt);
	}

	delta += dt;
	if (delta > attTime)
	{
		delta = 0.0f;
		pc->act = idle;
		mw->attackEnemy = false;

		draw(mw, dt, iPointZero);
		return false;
	}

	return true;
}

void nomalSwordMethod(float dt, iPoint dropP)
{
	if (pc->act == evasion || pc->act == falling)
		return;

	meleeWeapon* mw = nomalSword;

	float ats = mw->_attackSpeed * pc->attackSpeed;
	if (getKeyDown(keyboard_j) && pc->pmw->mw == mw && mw->attackSpeed == 0 && pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= ats;
		audioPlay(SND_SWING);
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (nomalSworadAttack(mw, dt, mw->attackEnemy, ats, 0.0f, 45.0f, 1.0f, 1.0f))
		return;

	draw(mw, dt, dropP);
}

//------------------------------------------------------------------------------------------

bool nomalSpearAttack(meleeWeapon* mw, float dt, bool att, float attTime,
	float iRange, float iAngle, float iRatioX, float iRatioY)
{
	if (pc->pmw->mw != mw)
		return false;

	if (mw->attackEnemy == false && pc->act != attacking)
		return false;

	//weaponPosition(mw, dt, mw->combatPosition);

	static float delta = 0.0f;
	float d = 0.0f;

	if (delta / attTime < 0.5f) 		d = 0.0f;
	//else if (delta / attTime < 0.6f)	d = 0.3f;
	//else if (delta / attTime < 0.9f)	d = 0.6f;
	else								d = 1.0f;

	float range = iRange;
	float rangeRate = linear(d, 0, range);

	float attAngle = iAngle;
	float attAngleRate = linear(d, 0, attAngle * 2 + attAngle / 2);

	float ratioX = iRatioX; // 수정필요
	float ratioRate = linear(d, 1.0, iRatioX);

	Texture* tex = mw->img->tex;
	iPoint centerP = mw->combatPosition;
	iRect rt = iRectZero;

	weaponPosAndRt(mw, mw->combatPosition, centerP, rt);

	iPoint wcp = iPointRotate(centerP, mw->combatPosition, attAngleRate - attAngle / 2);
	wcp += pc->combatV * rangeRate;
	wcp += pc->camPosition + setPos;

	rt.origin = wcp - iPointMake(rt.size.width / 2.0f, rt.size.height / 2.0f);
	mw->hitBox = rt;

	setRGBA(0, 1, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

	drawImage(tex,
		wcp.x,
		wcp.y,
		0, 0, tex->width, tex->height,
		VCENTER | HCENTER, ratioRate, ratioRate,
		2, attAngleRate - attAngle / 2 + pc->combatAngleV + mw->holdAngle, REVERSE_NONE);

	if (d > 0.5f)
	{
		hitMonster(mw, dt);
	}

	delta += dt;
	if (delta > attTime)
	{
		delta = 0.0f;
		pc->act = idle;
		mw->attackEnemy = false;

		draw(mw, dt, iPointZero);
		return false;
	}

	return true;
}

void nomalSpearMethod(float dt, iPoint dropP)
{
	if (pc->act == evasion || pc->act == falling)
		return;

	meleeWeapon* mw = nomalSpear;

	float ats = mw->_attackSpeed * pc->attackSpeed;
	if (getKeyDown(keyboard_j) && pc->pmw->mw == mw && mw->attackSpeed == 0 && pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= ats;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (nomalSpearAttack(mw, dt, mw->attackEnemy, ats, 50.0f, 0.0f, 1.0f, 1.0f))
		return;

	draw(mw, dt, dropP);
}

//------------------------------------------------------------------------------------

bool nomalCycloneAttack(meleeWeapon* mw, float dt, bool att, float attTime,
	float iRange, int iCycleCount, float iRatioX, float iRatioY)
{
	if (pc->pmw->mw != mw )
		return false;

	if (mw->attackEnemy == false && pc->act != attacking)
		return false;

	//weaponPosition(mw, dt, mw->combatPosition);

	static float delta = 0.0f;

	float range = iRange;
	float rangeRate = linear(delta / attTime, 0, range);

	float attAngle = 360.0f;
	int cCount = iCycleCount;
	float attAngleRate = linear(delta / attTime, 0, attAngle * cCount);

	float ratioX = iRatioX; // 수정필요
	float ratioRate = linear(delta / attTime, 1.0, iRatioX);

	Texture* tex = mw->img->tex;
	iPoint wcp = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);
	iPoint centerP = wcp;
	iRect rt = iRectZero;

	static bool cycle = false;
	static iPoint cycCp = iPointZero;
	static iPoint cycCenter = iPointZero;
	static float cycAngle = 0.0f;

	weaponPosAndRt(mw, mw->combatPosition, centerP, rt);
	if (cycle == false)
	{
		cycle = true;
		cycAngle = pc->combatAngleV;
		cycCp = wcp;
		cycCenter = centerP;
	}

	wcp = iPointRotate(cycCenter, cycCp, attAngleRate);
	wcp += pc->combatV * rangeRate;
	wcp += pc->camPosition + setPos;

	rt.origin = wcp - iPointMake(rt.size.width / 2.0f, rt.size.height / 2.0f);
	mw->hitBox = rt;

	setRGBA(0, 1, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

	drawImage(tex,
		wcp.x,
		wcp.y,
		0, 0, tex->width, tex->height,
		VCENTER | HCENTER, ratioRate, ratioRate,
		2, mw->holdAngle + attAngleRate + cycAngle, REVERSE_NONE);

	hitMonster(mw, dt);

	for (int i = 0; i < 8; i++)
	{
		pc->img[i]->selected = true;
		pc->img[i]->_selectedDt = attTime;
		pc->img[i]->location = 1;
		pc->img[i]->reverseRotate = true;
		pc->img[i]->angle = attAngleRate;
		pc->img[i]->_repeatNum = 1;
		pc->img[i]->_aniDt = attAngle * cCount / attTime;
	}
	pc->img[4]->angle += -45;
	pc->img[5]->angle += -45;
	pc->img[6]->angle += -45;
	pc->img[7]->angle += -45;

	delta += dt;
	if (delta > attTime )
	{
		delta = 0.0f;
		pc->act = idle;
		mw->attackEnemy = false;

		draw(mw, dt, iPointZero);

		for (int i = 0; i < 8; i++)
		{
			pc->img[i]->_repeatNum = 0;
			pc->img[i]->location = 2;
			pc->img[i]->reverseRotate = false;
			pc->img[i]->angle = 0;
			pc->img[i]->_aniDt = 0.05f;
		}

		cycle = false;
		return false;
	}

	return true;
}

void nomalCycloneMethod(float dt, iPoint dropP)
{
	if (pc->act == evasion || pc->act == falling)
		return;

	meleeWeapon* mw = nomalCyclone;

	float ats = mw->_attackSpeed * pc->attackSpeed;
	if (getKeyStat(keyboard_j) && pc->pmw->mw == mw && mw->attackSpeed == 0 && pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= ats;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (nomalCycloneAttack(mw, dt, mw->attackEnemy, ats, 0.0f, 2, 1.0f, 1.0f))
		return;

	draw(mw, dt, dropP);
}