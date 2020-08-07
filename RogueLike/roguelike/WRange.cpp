#include "WRange.h"

#include "Weapon.h"
#include "Effect.h"

rangeWeapon* nomalBow;
void nomalBowMethod(float dt, iPoint dropP)
{

}

void rangeWeapon::init(const char* info, Range* r)
{
	// asset 2가지 =  장착할때 이미지 + UI에 표시될 이미지
	iImage* img = new iImage();
	for (int j = 0; j < 2; j++)
	{
		Texture* tex = createImage(r->strImg, j);
		img->addObject(tex);
		freeImage(tex);
	}
	r->rw->img = img;

	infoImg = infoFromMW(info);
	infomation = info;

	rangeStat* rs = &r->stat;
	fireDmg = rs->fireDmg;
	fireSpeed = 
	_fireSpeed = rs->fireSpeed;
	widthReach = rs->widthReach;
	heightReach = rs->heightReach;
	holdAngle = rs->holdAngle;

	centerPos = iPointZero;
	hitBox = iRectZero;
	attackEnemy = false;
}

void createRangeWeapon()
{
	nomalBow = (rangeWeapon*)malloc(sizeof(rangeWeapon));

	Range range[RANGE_NUM] = {
		{nomalBow, nomalBowMethod, {1,1, 1, 1, -45}, "assets/weapon/bow%d.png"},
	};

	const char* rangeInfo[RANGE_NUM] = { 
		"BOW",
	};

	//-----------------------------------------------------------
	// PWP

	for (int i = 0; i < RANGE_NUM; i++)
	{
		Range* r = &range[i];

		r->rw->init(rangeInfo[i], r);
		PWP[MELEE_NUM + i].wp = r->rw;
		PWP[MELEE_NUM + i].method = r->method;
		PWP[MELEE_NUM + i].isMelee = false;
		PWP[MELEE_NUM + i].pos = iPointZero;
		PWP[MELEE_NUM + i].drop = true;
	}
}

void freeRangeWeapon()
{
	delete nomalBow->img;
	delete nomalBow->infoImg;
	free(nomalBow);
}

