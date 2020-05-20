#include "WMelee.h"

#include "Room.h"

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
	//Texture* texSword = createImage("assets/weapon/hammer.png");
	//imgSword->addObject(texSword);
	//freeImage(texSword);
	//nomalSword->init(imgSword,true,30,0.5f,30.0f, 60.0f, -30.0f);

	//-----------------------------------------------------------

	nomalSpear = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	iImage* imgSpear = new iImage();
	//Texture* texSpear = createImage("assets/weapon/upg_spear.png");
	//imgSpear->addObject(texSpear);
	//freeImage(texSpear);
	//nomalSpear->init(imgSpear, true, 40, 0.3f, 10.0f, 70.0f,-45.0f);

	//-----------------------------------------------------------

	nomalCyclone = (meleeWeapon*)malloc(sizeof(meleeWeapon) * 1);
	iImage* imgCyclon= new iImage();
	//Texture* texCyclon = createImage("assets/weapon/upg_axeDouble.png");
	//imgCyclon->addObject(texCyclon);
	//freeImage(texCyclon);
	//nomalCyclone->init(imgCyclon, true, 30, 1.0f, 30, 50, -70);

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

	nomalSword->init(imgSword, true, 30, 0.5f, 30.0f, 60.0f, -30.0f);
	nomalSpear->init(imgSpear, true, 40, 0.3f, 10.0f, 70.0f, -45.0f);
	nomalCyclone->init(imgCyclon, true, 30, 1.0f, 30, 50, -70);
	

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

static iPoint mv = iPointZero;
static float angle = 0.0f;
void draw(meleeWeapon* mw, float dt, iPoint dropP)
{
	Texture* tex = mw->img->tex;

	if (pc->pmw->mw == mw)
	{
		static iPoint p = mw->combatPosition;

		iRect rt;
		weaponPosAndRt(mw, mw->combatPosition, p, rt);

		drawImage(tex, 
			pc->camPosition.x + setPos.x + p.x ,
			pc->camPosition.y + setPos.y + p.y ,
			0, 0,	tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, angle + mw->holdAngle, REVERSE_NONE);
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
			pc->camPosition.x + setPos.x + p.x,
			pc->camPosition.y + setPos.y + p.y,
			0, 0,	tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 90);
		setRGBA(0, 1, 0, 1);
		drawRect(mw->hitBox);
		setRGBA(1, 1, 1, 1);
	}
}

void weaponPosition(meleeWeapon* mw, float dt, iPoint& wp)
{
	//임시
	static iPoint p = iPointMake(pc->playerPosition.x + HALF_OF_TEX_WIDTH * 2.0f,
		pc->playerPosition.y + HALF_OF_TEX_HEIGHT);
	Texture* tex = mw->img->tex;

	if (mv.x < 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y;
		angle = 90.0f;
	}
	else if (mv.x > 0)
	{
		p.x = pc->playerPosition.x + HALF_OF_TEX_WIDTH;
		p.y = pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2.0f;
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
	if (pc->act != idle)
		return;
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

void weaponPosAndRt(meleeWeapon* mw, iPoint& wcp, iPoint& centerP, iRect& rt)
{
	Texture* tex = mw->img->tex;
	iSize size = iSizeMake(mw->widthReach, mw->heightReach);

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
	weaponPosition(mw, dt, mw->combatPosition);

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
	wcp += mv * rangeRate;
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
		2, attAngleRate - attAngle + angle + mw->holdAngle, REVERSE_NONE);

	if (d > 0.5f)
	{
		for (int i = 0; i < ENEMY_NUM; i++) //enemy
		{
			if (containRect(mw->hitBox, enemys[i]->touchEnemyNomal))
				enemys[i]->takeDmgEnemy(dt, mw->attackDmg);
		}
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
	weaponVector(mw, dt);

	if (getKeyDown(keyboard_j) && pc->pmw->mw == mw && mw->attackSpeed == 0 && pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= mw->_attackSpeed;
		audioPlay(SND_SWING);
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (nomalSworadAttack(mw, dt, mw->attackEnemy, 0.3f, 0.0f, 45.0f, 1.0f, 1.0f))
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

	weaponPosition(mw, dt, mw->combatPosition);

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
	wcp += mv * rangeRate;
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
		2, attAngleRate - attAngle / 2 + angle + mw->holdAngle, REVERSE_NONE);

	if (d > 0.5f)
	{
		for (int i = 0; i < ENEMY_NUM; i++) //enemy
		{
			if (containRect(mw->hitBox, enemys[i]->touchEnemyNomal))
				enemys[i]->takeDmgEnemy(dt, mw->attackDmg);
		}
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
	weaponVector(mw, dt);

	if (getKeyDown(keyboard_j) && pc->pmw->mw == mw && mw->attackSpeed == 0 && pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= mw->_attackSpeed;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (nomalSpearAttack(mw, dt, mw->attackEnemy, 1.0f, 50.0f, 0.0f, 1.0f, 1.0f))
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
		cycAngle = angle;
		cycCp = wcp;
		cycCenter = centerP;
	}

	wcp = iPointRotate(cycCenter, cycCp, attAngleRate);
	wcp += mv * rangeRate;
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

	for (int i = 0; i < ENEMY_NUM; i++) //enemy
	{
		if (containRect(mw->hitBox, enemys[i]->touchEnemyNomal))
			enemys[i]->takeDmgEnemy(dt, mw->attackDmg);
	}

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
	weaponVector(mw, dt);

	if (getKeyDown(keyboard_j) && pc->pmw->mw == mw && mw->attackSpeed == 0 && pc->act == idle)
	{
		pc->act = attacking;
		mw->attackEnemy = true;
		mw->attackSpeed -= mw->_attackSpeed;
	}

	mw->attackSpeed += dt;
	if (mw->attackSpeed > 0.0f)
		mw->attackSpeed = (int)0;

	if (nomalCycloneAttack(mw, dt, mw->attackEnemy, 0.5f, 0.0f, 2, 1.0f, 1.0f))
		return;

	draw(mw, dt, dropP);
}