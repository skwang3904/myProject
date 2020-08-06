#include "WMelee.h"

#include "Room.h"

#include "EnemyData.h"

#include "Weapon.h"

meleeWeapon* nomalHammer;
void nomalHammerMethod(float dt, iPoint dropP);
meleeWeapon* nomalSpear;
void nomalSpearMethod(float dt, iPoint dropP);
meleeWeapon* nomalCyclone;
void nomalCycloneMethod(float dt, iPoint dropP);

void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt);

void meleeWeapon::init(const char* info, Melee* m)
{
	// asset 2가지 =  장착할때 이미지 + UI에 표시될 이미지
	iImage* img = new iImage();
	for (int j = 0; j < 2; j++)
	{
		Texture* tex = createImage(m->strImg, j);
		img->addObject(tex);
		freeImage(tex);
	}
	m->mw->img = img;

	infoImg = infoFromMW(info);
	infomation = info;

	meleeStat* ms = &m->stat;
	attackDmg = ms->attackDmg;
	attackSpeed = 0.0f;
	_attackSpeed = ms->attackSpeed;
	widthReach = ms->widthReach;
	heightReach = ms->heightReach;
	holdAngle = ms->holdAngle;

	centerPos = iPointZero;
	hitBox = iRectZero;
	attackEnemy = false;
}

void meleeWeapon::draw(float dt, iPoint dropPos)
{
	Texture* tex = img->tex;
	iPoint setp = SET_DRAW_OFF;
	if (pc->pwp->wp == this)
	{
		iPoint centerP = pc->combatPos;
		iRect rt;
		//weaponPosAndRt(this, centerPos, centerP, rt);
		setCenterPos();
		drawImage(tex,
			setp.x + centerP.x,
			setp.y + centerP.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, pc->combatAngleV + holdAngle, REVERSE_NONE);
	}
	else
	{
		iPoint p = dropPos;
		drawImage(tex,
			setp.x + p.x,
			setp.y + p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.2f, 1.2f, 2, 45);

		iPoint hit = p - iPointMake(tex->width, tex->height) * (1.5f / 2.0f);
		hitBox = iRectMake(hit.x, hit.y , tex->width * 1.5f, tex->height * 1.5f);

		iRect mt = hitBox;
		mt.origin += SET_DRAW_OFF;
		setRGBA(0, 1, 0, 1);
		drawRect(mt);
		setRGBA(1, 1, 1, 1);
	}
}

void meleeWeapon::setCenterPos()
{
	// 무기 중심점
	// 무기 히트박스

	iPoint p = pc->combatPos + (pc->combatV * iPointMake(img->tex->width, img->tex->height)) / 2.0f;

	centerPos = p;
	
	p -= (pc->combatV * iPointMake(img->tex->width, img->tex->height));
	iRect rt = iRectMake(p.x, p.y, img->tex->width, img->tex->height);
	hitBox = rt;

	rt.origin += SET_DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
}

/////////////////////////////////////////////////////////////////////////////////

void createMeleeWeapon()
{
	nomalHammer = (meleeWeapon*)malloc(sizeof(meleeWeapon));
	nomalSpear = (meleeWeapon*)malloc(sizeof(meleeWeapon));
	nomalCyclone = (meleeWeapon*)malloc(sizeof(meleeWeapon));

	// attackDmg, attackSpeed, widthReach, heightReach, holdAngle
	meleeStat ms[MELEE_NUM] = {
	{200, 0.5f, 30.0f, 60.0f, -30.0f},
	{30, 0.2f, 10.0f, 70.0f, -45.0f},
	{30, 0.5f, 30.0f, 50.0f, -70.0f},
	};

	Melee melee[MELEE_NUM] = {
	{nomalHammer,	nomalHammerMethod,	ms[0], "assets/weapon/hammer%d.png"},
	{nomalSpear,	nomalSpearMethod,	ms[1], "assets/weapon/upg_spear%d.png"},
	{nomalCyclone,	nomalCycloneMethod,	ms[2], "assets/weapon/upg_axeDouble%d.png"},
	};

	const char* meleeInfo[MELEE_NUM] = {
	"Hammer\nHammer\nHammer\nHammer",
	"Spear\nSpear\nSpear\nSpear\nSpear",
	"Cyclone",
	};

	//-----------------------------------------------------------
	// PWP

	for (int i = 0; i < MELEE_NUM; i++)
	{
		Melee* m = &melee[i];
		m->mw->init(meleeInfo[i], m);
		PWP[i].wp = m->mw;
		PWP[i].method = m->method;
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

#if 0
void draw(meleeWeapon* mw, float dt, iPoint dropP)
{
	Texture* tex = mw->img->tex;
	iPoint setp = SET_DRAW_OFF;
	if (pc->pwp->wp == mw)
	{
		iPoint centerP = pc->combatPos;
		iRect rt;
		weaponPosAndRt(mw, mw->centerPos, centerP, rt);
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
#endif

void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt)
{
	
}

//------------------------------------------------------------------------------------

void hitMonster(meleeWeapon* mw, float dt)
{
	float dmg = pc->attackDmg + mw->attackDmg;
	for (int i = 0; i < monsterNum; i++)
	{
		MonsterData* t = totalMonster[i];
		if (t->tileNumber == pc->tileNumber)
		{
			if (containRect(mw->hitBox, t->touchEnemy))
				t->takeDmgEnemy(dt, dmg);
		}
	}
}

//------------------------------------------------------------------------------------
// 무기종류, 시간, 공격하는데 걸리는시간, 찌르는 거리, 휘두르는 각도, 배율X, 배율Y,
bool nomalHammerAttack(meleeWeapon* mw, float dt, float attTime,
	float iRange, float iAngle, float iRatioX, float iRatioY)
{
	if (pc->act != Act_attacking)
		return false;

	mw->attackSpeed += dt;
	float atkSpeed = mw->attackSpeed;
	float d = 0.0f;
	if (atkSpeed / attTime < 0.6f) 			d = 0.0f;
	else if (atkSpeed / attTime < 0.65f)	d = 0.3f;
	else if (atkSpeed / attTime < 0.7f)		d = 0.6f;
	else									d = 1.0f;

	float range = iRange;
	float rangeRate = linear(d, 0, range);

	float attAngle = iAngle;
	float attAngleRate = linear(d, 0, attAngle * 2);

	float ratioX = iRatioX; // 수정필요
	float ratioRate = linear(d, 1.0, iRatioX);

	Texture* tex = mw->img->tex;
	iPoint wcp = mw->centerPos;
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
		MapTile* m = maps[pc->tileNumber];
		for (int i = 0; i < m->mapObjNum; i++)
		{
			if (m->mapObj[i]->type == MapObj_Broke)
			{
				iRect rt = m->mapObj[i]->hitBox;
				rt.origin += maps[pc->tileNumber]->tileOff;

				if (containRect(rt, mw->hitBox))
				{
					m->mapObj[i]->objImg->setTexAtIndex(1);
				}
			}
		}

	}

	if (atkSpeed > attTime)
	{
		pc->act = Act_idle;
		mw->attackEnemy = false;
		mw->attackSpeed = 0.0f;
		return false;
	}
	return true;
}

void nomalHammerMethod(float dt, iPoint dropP)
{
	meleeWeapon* mw = nomalHammer;
	if (dropP == iPointZero)
	{
		float ats = mw->_attackSpeed * pc->attackSpeed;
		if (getKeyDown(keyboard_j) && pc->pwp->wp == mw && pc->act == Act_idle)
		{
			pc->act = Act_attacking;
			audioPlay(SND_SWING);
		}

		if (nomalHammerAttack(mw, dt, ats, 0.0f, 70.0f, 1.5f, 1.0f))
			return;
	}

	mw->draw(dt, dropP);
}

//------------------------------------------------------------------------------------------

bool nomalSpearAttack(meleeWeapon* mw, float dt, float attTime,
	float iRange, float iAngle, float iRatioX, float iRatioY)
{
	if (pc->act != Act_attacking)
		return false;

	mw->attackSpeed += dt;
	float atkSpeed = mw->attackSpeed;
	float d = 0.0f;

	if (atkSpeed / attTime < 0.5f) 		d = 0.0f;
	//else if (atkSpeed / attTime < 0.6f)	d = 0.3f;
	//else if (atkSpeed / attTime < 0.9f)	d = 0.6f;
	else								d = 1.0f;

	float range = iRange;
	float rangeRate = linear(d, 0, range);

	float attAngle = iAngle;
	float attAngleRate = linear(d, 0, attAngle * 2 + attAngle / 2);

	float ratioX = iRatioX; // 수정필요
	float ratioRate = linear(d, 1.0, iRatioX);

	Texture* tex = mw->img->tex;
	iPoint centerP = mw->centerPos;
	iRect rt = iRectZero;

	weaponPosAndRt(mw, mw->centerPos, centerP, rt);
	iPoint wcp = iPointRotate(centerP, mw->centerPos, attAngleRate - attAngle / 2);
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
		hitMonster(mw, dt);

	if (atkSpeed > attTime)
	{
		pc->act = Act_idle;
		mw->attackEnemy = false;
		mw->attackSpeed = 0.0f;
		return false;
	}
	return true;
}

void nomalSpearMethod(float dt, iPoint dropP)
{
	meleeWeapon* mw = nomalSpear;
	if (dropP == iPointZero)
	{
		float ats = mw->_attackSpeed * pc->attackSpeed;
		if (getKeyDown(keyboard_j) && pc->pwp->wp == mw && pc->act == Act_idle)
		{
			pc->act = Act_attacking;
			audioPlay(SND_SWING);
		}

		if (nomalSpearAttack(mw, dt, ats, 100.0f, 0.0f, 1.5f, 1.0f))
			return;
		mw->draw(dt, dropP);
	}
	else
		mw->draw(dt, dropP);
		
}

//------------------------------------------------------------------------------------

bool nomalCycloneAttack(meleeWeapon* mw, float dt, float attTime,
	float iRange, int iCycleCount, float iRatioX, float iRatioY)
{
	if (pc->act != Act_attacking)
		return false;

	mw->attackSpeed += dt;
	float atkSpeed = mw->attackSpeed;

	float range = iRange;
	float rangeRate = linear(atkSpeed / attTime, 0, range);

	float attAngle = 360.0f;
	int cCount = iCycleCount;
	float attAngleRate = linear(atkSpeed / attTime, 0, attAngle * cCount);

	float ratioX = iRatioX; // 수정필요
	float ratioRate = linear(atkSpeed / attTime, 1.0, iRatioX);

	Texture* tex = mw->img->tex;
	iPoint centerP = mw->centerPos;
	iRect rt = iRectZero;

	static bool cycle = false;
	static float cycAngle = 0.0f;
	static float cycHoldAngle = 0.0f;

	weaponPosAndRt(mw, mw->centerPos, centerP, rt);
	if (cycle == false)
	{
		cycle = true;
		cycAngle = pc->combatAngleV;
		cycHoldAngle = mw->holdAngle;
	}

	//if (pc->combatV.x < 0.0f) attAngleRate += 90;
	//else  attAngleRate += 270;
	//if (pc->combatV.y < 0.0f)  attAngleRate += 180;
	//else  ;

	iPoint playerCenter = pc->playerPosition + HALF_OF_TEX_POINT;
	iPoint wcp = iPointRotate(centerP, playerCenter, attAngleRate);
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
		2, cycHoldAngle + attAngleRate + cycAngle, REVERSE_NONE);

	hitMonster(mw, dt);
#if 0
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
#endif
	if (atkSpeed > attTime )
	{
		pc->act = Act_idle;
		mw->attackEnemy = false;
		mw->attackSpeed = 0.0f;
#if 0
		for (int i = 0; i < 8; i++)
		{
			pc->img[i]->_repeatNum = 0;
			pc->img[i]->location = 2;
			pc->img[i]->reverseRotate = false;
			pc->img[i]->angle = 0;
			pc->img[i]->_aniDt = 0.05f;
		}
#endif

		cycle = false;
		return false;
	}

	return true;
}

void nomalCycloneMethod(float dt, iPoint dropP)
{
	meleeWeapon* mw = nomalCyclone;
	if (dropP == iPointZero)
	{
		float ats = mw->_attackSpeed * pc->attackSpeed;
		if (getKeyDown(keyboard_j) && pc->pwp->wp == mw && pc->act == Act_idle)
		{
			pc->act = Act_attacking;
			audioPlay(SND_SWING);
		}

		if (nomalCycloneAttack(mw, dt, 1.5f, 0.0f, 1, 1.0f, 1.0f))
			return;
	}

	mw->draw(dt, dropP);

}