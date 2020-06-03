#include "WMelee.h"

#include "Room.h"

#include "Enemy.h"
#include "EnemyStruct.h"

#include "Weapon.h"

meleeWeapon* nomalHammer;
meleeWeapon* nomalSpear;
meleeWeapon* nomalCyclone;

void nomalHammerMethod(float dt, iPoint dropP);
void nomalSpearMethod(float dt, iPoint dropP);
void nomalCycloneMethod(float dt, iPoint dropP);

void meleeWeapon::init(
	const char* info,
	float iAttackDmg,
	float iAttackSpeed,
	float iWidthReach,
	float iHeightReach,
	float iHoldAngle)
{
	infoImg = infoFromMW(info);
	infomation = info;

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
	nomalHammer = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	nomalSpear = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	nomalCyclone = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);

	meleeWeapon* mw[3] = {
		nomalHammer,
		nomalSpear,
		nomalCyclone,
	};

	// asset 2가지 =  장착할때 이미지 + UI에 표시될 이미지
	const char* strPath[3] = {
		"assets/weapon/hammer%d.png",
		"assets/weapon/upg_spear%d.png",
		"assets/weapon/upg_axeDouble%d.png",
	};

	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			Texture* tex = createImage(strPath[i], j);
			img->addObject(tex);
			freeImage(tex);
		}
		mw[i]->img = img;
	}

	//info, ismelee, attackDmg, attackSpeed, widthReach, heightReach, holeAngle
	char swordInfo[32] = "Hammer\nHammer\nHammer\nHammer";
	mw[0]->init(swordInfo, 30, 0.3f, 30.0f, 60.0f, -30.0f);

	char spearInfo[64] = "Spear\nSpear\nSpear\nSpear\nSpear";
	mw[1]->init(spearInfo, 30, 0.2f, 10.0f, 70.0f, -45.0f);

	char cycleonInfo[32] = "Cyclone";
	mw[2]->init(cycleonInfo, 30, 0.5f, 30.0f, 50.0f, -70.0f);

	//-----------------------------------------------------------
	// PWP

	PWP[0] = { mw[0],nomalHammerMethod };
	PWP[1] = { mw[1],nomalSpearMethod };
	PWP[2] = { mw[2],nomalCycloneMethod };

	for (int i = 0; i < MELEE_NUM; i++)
	{
		PWP[i].isMelee = true;
		PWP[i].pos = iPointZero;
		PWP[i].drop = true;
	}
}

void freeMeleeWeapon()
{
	delete nomalHammer->img;
	delete nomalHammer->infoImg;
	free(nomalHammer);

	delete nomalSpear->img;
	delete nomalSpear->infoImg;
	free(nomalSpear);

	delete nomalCyclone->img;
	delete nomalCyclone->infoImg;
	free(nomalCyclone);
}

void draw(meleeWeapon* mw, float dt, iPoint dropP)
{
	Texture* tex = mw->img->tex;
	iPoint setp = SET_DRAW_OFF;
	if (pc->pwp->wp == mw)
	{
		iPoint centerP = mw->combatPosition;

		iRect rt;
		weaponPosAndRt(mw, mw->combatPosition, centerP, rt);


		drawImage(tex,
			setp.x + centerP.x,
			setp.y + centerP.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, pc->combatAngleV + mw->holdAngle, REVERSE_NONE);
	}
	else
	{
		iPoint p = dropP;
		drawImage(tex,
			setp.x + p.x,
			setp.y + p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.2f, 1.2f, 2, 90, REVERSE_NONE);

		mw->hitBox = getHitBoxRect(tex,
			p.x,
			p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 90);

		iRect mt = mw->hitBox;
		mt.origin += SET_DRAW_OFF;
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
		wcp.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH + tex->height / 10;
		wcp.y = pc->playerPosition.y + tex->width / 10;
		centerP.x = wcp.x - tex->height / 2;
		centerP.y = wcp.y;

		rt.size.width = size.height;
		rt.size.height = size.width;
		angle = 90;
	}
	else if (mv.x > 0)
	{
		wcp.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH - tex->height / 10;
		wcp.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2 - tex->width / 10;
		centerP.x = wcp.x + tex->height / 2;
		centerP.y = wcp.y;

		rt.size.width = size.height;
		rt.size.height = size.width;
		angle = 270;
	}

	if (mv.y < 0)
	{
		wcp.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2 - tex->width / 5;
		wcp.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT + tex->height / 10;
		centerP.x = wcp.x;
		centerP.y = wcp.y - tex->height / 2;

		rt.size.width = size.width;
		rt.size.height = size.height;
		angle = 0;
	}
	else if (mv.y > 0)
	{
		wcp.x = pc->playerPosition.x + tex->width / 5;
		wcp.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT - tex->height / 10;
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
	float dmg = pc->attackDmg + mw->attackDmg;
	for (int i = 0; i < 1; i++) //enemy
	{
		for (int j = 0; j < GOLEM_NUM; j++)
		{
			EnemyGolem* eg = &golems[i][j];
			if (containRect(mw->hitBox, eg->touchGolem))
				eg->takeDmgEnemy(dt, dmg);
		}
	}

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < GOLEM_ELETE_NUM; j++)
		{
			EnemyGolem* eg = &golemEletes[i][j];
			if (containRect(mw->hitBox, eg->touchGolem))
				eg->takeDmgEnemy(dt, dmg);
		}
	}
}

//------------------------------------------------------------------------------------
// 무기종류, 시간, 공격중인지,공격하는데 걸리는시간, 찌르는 거리, 휘두르는 각도, 배율X, 배율Y,
bool nomalSworadAttack(meleeWeapon* mw, float dt, bool att, float attTime,
	float iRange, float iAngle, float iRatioX, float iRatioY)
{
	if (pc->pwp->wp != mw)
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
	rt.origin = wcp - iPointMake(rt.size.width/2.0f, rt.size.height/2.0f);
	wcp += SET_DRAW_OFF;
	mw->hitBox = rt;

	rt.origin += SET_DRAW_OFF;
	setRGBA(0, 1, 0, 0.5f);
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

void nomalHammerMethod(float dt, iPoint dropP)
{
	if (pc->act == evasion || pc->act == falling)
		return;

	meleeWeapon* mw = nomalHammer;

	float ats = mw->_attackSpeed * pc->attackSpeed;
	if (getKeyDown(keyboard_j) && pc->pwp->wp == mw && mw->attackSpeed == 0 && pc->act == idle)
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
	if (pc->pwp->wp != mw)
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
	rt.origin = wcp - iPointMake(rt.size.width / 2, rt.size.height / 2);
	wcp += SET_DRAW_OFF;
	mw->hitBox = rt;

	rt.origin += SET_DRAW_OFF;
	setRGBA(0, 1, 0, 0.5f);
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
	if (getKeyDown(keyboard_j) && pc->pwp->wp == mw && mw->attackSpeed == 0 && pc->act == idle)
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
	if (pc->pwp->wp != mw )
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
	rt.origin = wcp - iPointMake(rt.size.width / 2, rt.size.height / 2);
	wcp += SET_DRAW_OFF;
	mw->hitBox = rt;

	rt.origin += SET_DRAW_OFF;
	setRGBA(0, 1, 0, 0.5f);
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
	if (getKeyStat(keyboard_j) && pc->pwp->wp == mw && mw->attackSpeed == 0 && pc->act == idle)
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