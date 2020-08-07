#include "WMelee.h"

#include "Room.h"

#include "EnemyData.h"

#include "Weapon.h"

void meleeWeapon::init(const char* info, Melee* m)
{
	// asset 2가지 =  장착할때 이미지 + UI에 표시될 이미지
	iImage* img = new iImage();
	for (int i = 0; i < 2; i++)
	{
		iPoint size = iPointMake(64 + 64 * i, 64 + 64 * i);
		Texture* tex = createTexture(size.x, size.y);
		fbo->bind(tex);
		fbo->setSize(size.x, size.y);

		Texture* texM = createImage(m->strImg, i);
		setRGBA(0, 0, 1, 0.5f);
		//fillRect(0, 0, size.x, size.y);
		setRGBA(1, 1, 1, 1);

		drawImage(texM, 0, 0, TOP | LEFT);
		freeImage(texM);

		fbo->backSize();
		fbo->unbind();
		
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

	atkRange = ms->atkRange;
	atkAngle = ms->atkAngle;
	atkRatio = ms->atkRatio;
	cycleCount = ms->cycleCount;

	hitBox = iRectZero;
	attackEnemy = false;

	methodMelee = m->methodMelee;
}

void meleeWeapon::draw(float dt, iPoint dropPos)
{
	Texture* tex = img->tex;
	iPoint setp = SET_DRAW_OFF;
	if (pc->pwp->wp == this)
	{
		iRect rt;
		centerPos = setCenterPos();
		drawImage(tex,
			setp.x + centerPos.x,
			setp.y + centerPos.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 
			2, pc->combatAngleV + holdAngle, REVERSE_HEIGHT);
	}
	else
	{
		iPoint p = dropPos;
		drawImage(tex,
			setp.x + p.x,
			setp.y + p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.2f, 1.2f, 
			2, 45, REVERSE_HEIGHT);

		iPoint hit = p - iPointMake(tex->width, tex->height) * (1.5f / 2.0f);
		hitBox = iRectMake(hit.x, hit.y , tex->width * 1.5f, tex->height * 1.5f);

		iRect mt = hitBox;
		mt.origin += SET_DRAW_OFF;
		setRGBA(0, 1, 0, 1);
		drawRect(mt);
		setRGBA(1, 1, 1, 1);
	}
}

iPoint meleeWeapon::setCenterPos()
{
	// 무기 중심점
	float x = pc->combatV.y != 0 ? widthReach : heightReach;
	float y = pc->combatV.y != 0 ? heightReach : widthReach;
		
	return pc->combatPos + pc->combatV * iPointMake(x, y) / 2.0f;
}

iRect meleeWeapon::setHitBox(iPoint& cp)
{
	// 무기 히트박스
	float x = pc->combatV.y != 0 ? widthReach : heightReach;
	float y = pc->combatV.y != 0 ? heightReach : widthReach;

	iPoint p = cp - iPointMake(x, y) / 2.0f;

	return iRectMake(p.x, p.y, x, y);
}

/////////////////////////////////////////////////////////////////////////////////

meleeWeapon* nomalHammer;
void nomalHammerMethod(float dt, iPoint dropP);
meleeWeapon* nomalSpear;
void nomalSpearMethod(float dt, iPoint dropP);
meleeWeapon* nomalCyclone;
void nomalCycloneMethod(float dt, iPoint dropP);

bool nomalAttack(float dt);
bool nomalCycloneAttack(float dt);


void createMeleeWeapon()
{
	nomalHammer = (meleeWeapon*)malloc(sizeof(meleeWeapon));
	nomalSpear = (meleeWeapon*)malloc(sizeof(meleeWeapon));
	nomalCyclone = (meleeWeapon*)malloc(sizeof(meleeWeapon));

	// attackDmg, attackSpeed, widthReach, heightReach, holdAngle
	// atkRange, atkAngle, atkRatio, cycleCount
	meleeStat ms[MELEE_NUM] = {
	{200, 1.0f, 40.0f, 60.0f, -30.0f,
		0.0f, 70.0f, {1.5f, 1.5f}, 0},
	{30, 0.5f, 10.0f, 70.0f, -45.0f,
		50.0f, 0.0f, {1.5f, 1.5f}, 0},
	{3, 1.5f, 30.0f, 50.0f, -70.0f,
		0.0f, 360.0f, {1.5f, 1.5f}, 1},
	};

	Melee melee[MELEE_NUM] = {
	{nomalHammer,	nomalHammerMethod, nomalAttack,	ms[0], 
	"assets/weapon/hammer%d.png"},
	{nomalSpear,	nomalSpearMethod, nomalAttack,	ms[1], 
	"assets/weapon/upg_spear%d.png"},
	{nomalCyclone,	nomalCycloneMethod, nomalCycloneAttack,	ms[2], 
	"assets/weapon/upg_axeDouble%d.png"},
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


//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// 무기종류, 시간, 찌르는 거리, 휘두르는 각도, 배율X, 배율Y,

bool nomalAttack(float dt)
{
	if (pc->act != Act_attack)
		return false;

	meleeWeapon* mw = (meleeWeapon*)pc->pwp->wp;

	mw->attackSpeed += dt;
	float atkSpeed = mw->attackSpeed * 2.0f;
	float attTime = mw->_attackSpeed * pc->attackSpeed;
	float d = atkSpeed / attTime;
#if 1
	if (d < 0.4f) 			
		d = linear(d / 0.4f, 0.0, -0.1f);
	else if (d < 0.45f)		
		d = 0.1f;
	else									
		d = 1.0f;
#else
	d = atkSpeed / attTime;
#endif

	float range = mw->atkRange;
	float attAngle = mw->atkAngle;
	iPoint ratio = mw->atkRatio;// test

	float rangeRate = linear(d, 0, range);
	float attAngleRate = linear(d, 0, attAngle * 2);
	//iPoint ratioRate = linear(d, iPointMake(1.0f, 1.0f), ratio);
	iPoint ratioRate = ratio;

	Texture* tex = mw->img->tex;
	iPoint wcp = pc->combatPos;
	iPoint centerP = mw->centerPos;

	centerP = iPointRotate(centerP, wcp, attAngleRate - attAngle);
	centerP += pc->combatV * rangeRate + iPointVector(centerP - wcp) * linear(d, 0.0, 32);
	mw->hitBox = mw->setHitBox(centerP);

	iRect rt = mw->hitBox;
	rt.origin += SET_DRAW_OFF;
	setRGBA(0, 1, 0, 0.5f);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

	centerP += SET_DRAW_OFF;
	drawImage(tex,
		centerP.x,
		centerP.y,
		0, 0, tex->width, tex->height,
		VCENTER | HCENTER, ratioRate.x, ratioRate.y, 
		2, attAngleRate - attAngle + pc->combatAngleV + mw->holdAngle, REVERSE_HEIGHT);

	if (d > 0.5f)
		meleeWeaponHit(dt, pc->attackDmg + mw->attackDmg, mw->hitBox);

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
		if (getKeyDown(keyboard_j) && 
			pc->pwp->wp == mw && 
			pc->act == Act_idle)
		{
			pc->act = Act_attack;
			audioPlay(SND_SWING);
		}

		if (mw->methodMelee(dt))
			return;
	}

	mw->draw(dt, dropP);
}

//------------------------------------------------------------------------------------------

void nomalSpearMethod(float dt, iPoint dropP)
{
	meleeWeapon* mw = nomalSpear;
	if (dropP == iPointZero)
	{
		if (getKeyDown(keyboard_j) &&
			pc->pwp->wp == mw &&
			pc->act == Act_idle)
		{
			pc->act = Act_attack;
			audioPlay(SND_SWING);
		}

		if (mw->methodMelee(dt))
			return;
	}

	mw->draw(dt, dropP);
}

//------------------------------------------------------------------------------------

bool nomalCycloneAttack(float dt)
{
	if (pc->act != Act_attack)
		return false;

	meleeWeapon* mw = (meleeWeapon*)pc->pwp->wp;

	mw->attackSpeed += dt;
	float atkSpeed = mw->attackSpeed * 2.0f;
	float attTime = mw->_attackSpeed * pc->attackSpeed;
	float d = atkSpeed / attTime;


	float range = mw->atkRange;
	float attAngle = mw->atkAngle;
	int cCount = mw->cycleCount;
	iPoint ratio = mw->atkRatio; // 수정필요


	float rangeRate = linear(d, 0, range);
	float attAngleRate = linear(d, 0, attAngle * cCount);
	//iPoint ratioRate = linear(d, iPointMake(1.0f, 1.0f), ratio);
	iPoint ratioRate = ratio;

	Texture* tex = mw->img->tex;
	iPoint wcp = pc->playerPosition + HALF_OF_TEX_POINT;
	iPoint centerP = mw->centerPos;


	centerP = iPointRotate(centerP, wcp, attAngleRate);
	centerP += pc->combatV * rangeRate + iPointVector(centerP - wcp) * linear(d, 0.0, 0);
	mw->hitBox = mw->setHitBox(centerP);

	iRect rt = mw->hitBox;
	rt.origin += SET_DRAW_OFF;
	setRGBA(0, 1, 0, 0.5f);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

	centerP += SET_DRAW_OFF;
	drawImage(tex,
		centerP.x,
		centerP.y,
		0, 0, tex->width, tex->height,
		VCENTER | HCENTER, ratioRate.x, ratioRate.y,
		2, attAngleRate + pc->combatAngleV + mw->holdAngle, REVERSE_HEIGHT);

	meleeWeaponHit(dt, mw->attackDmg, mw->hitBox);
	if (atkSpeed > attTime )
	{
		pc->act = Act_idle;
		mw->attackEnemy = false;
		mw->attackSpeed = 0.0f;

		return false;
	}

	return true;
}

void nomalCycloneMethod(float dt, iPoint dropP)
{
	meleeWeapon* mw = nomalCyclone;
	if (dropP == iPointZero)
	{
		if (getKeyDown(keyboard_j) && 
			pc->pwp->wp == mw && 
			pc->act == Act_idle)
		{
			pc->act = Act_attack;
			audioPlay(SND_SWING);
		}

		if (mw->methodMelee(dt))
			return;
	}

	mw->draw(dt, dropP);

}