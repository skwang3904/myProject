#include "WRange.h"

#include "Weapon.h"
#include "Effect.h"

rangeWeapon* nomalBow;
void nomalBowMethod(float dt, iPoint dropP);

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

	combatPosition = iPointZero;
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

void weaponPosAndRt(rangeWeapon* rw, iPoint& wcp, iPoint& centerP, iRect& rt)
{
	Texture* tex = rw->img->tex;
	iSize size = iSizeMake(rw->widthReach, rw->heightReach);

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

void draw(rangeWeapon* rw, float dt, iPoint dropP)
{
	Texture* tex = rw->img->tex;
	iPoint setp = SET_DRAW_OFF;
	if (pc->pwp->wp == rw)
	{
		iPoint centerP = rw->combatPosition;
		iRect rt;
		weaponPosAndRt(rw, rw->combatPosition, centerP, rt);

		drawImage(tex,
			setp.x + centerP.x,
			setp.y + centerP.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.0f, 1.0f, 2, pc->combatAngleV + rw->holdAngle, REVERSE_NONE);
	}
	else
	{
		iPoint p = dropP;
		drawImage(tex,
			setp.x + p.x,
			setp.y + p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.2f, 1.2f, 2, 90, REVERSE_NONE);

		rw->hitBox = getHitBoxRect(tex,
			p.x,
			p.y,
			0, 0, tex->width, tex->height,
			VCENTER | HCENTER, 1.5f, 1.5f, 2, 90);

		iRect mt = rw->hitBox;
		mt.origin += SET_DRAW_OFF;
		setRGBA(0, 1, 0, 1);
		drawRect(mt);
		setRGBA(1, 1, 1, 1);
	}

}

//------------------------------------------------------------------------------------------

void nomalBowFire() //발사간격, 총알속도, 
{
	printf("fire\n");
}

void nomalBowMethod(float dt, iPoint dropP)
{
	rangeWeapon* rw = nomalBow;
	if (dropP == iPointZero)
	{
		float fireSpd = rw->_fireSpeed;
		if (getKeyDown(keyboard_j) && pc->pwp->wp == rw && pc->act == idle)
		{
			// fire
			nomalBowFire();
		}
		draw(rw, dt, dropP);
	}
	else
		draw(rw, dt, dropP);
}