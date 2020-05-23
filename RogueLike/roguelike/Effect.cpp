#include "Effect.h"

#include "Room.h"

iImage* imgFireBall;
iImage* imgChargeFire;

FireBall** ball;

FireBall::FireBall()
{
	img = imgFireBall->copy();

	alive = false;
	dmg = 0.0f;
	speed = 300.0f;
	duration = 0.0f;

	tileNumber = 0;

	v = iPointZero;
	sp = iPointZero;
	posFireBall = iPointZero;
	drawFireBallPos = iPointZero;

	touchRect = iRectZero;
	limitRect = iRectZero;

}

FireBall::~FireBall()
{
	delete img;
}

void FireBall::init(uint8 num, uint8 tileNum, iPoint& v, iPoint& pos)
{
	ball[num]->alive = true;
	ball[num]->sp = pos;
	ball[num]->posFireBall = pos;
	ball[num]->v = v / iPointLength(v);
	ball[num]->tileNumber = tileNum;
	ball[num]->setlimitRect(tileNum);
	//num++;
	//if (num > FIREBALL_NUM - 1)
	//	num = 0;
}

void FireBall::paint(float dt)
{
	if (alive == false)
		return;

	duration += dt;
	if (duration > 20.0f || containPoint(posFireBall, limitRect) == false)
	{
		alive = false;
		duration = 0.0f;
	}

	iPoint p = iPointMake(img->tex->width / 2.0f, img->tex->height / 2.0f);
	iPoint mp = v * speed * dt;
	projectileReflect(tileNumber, v, posFireBall, mp);
	setAngle();
	touchRect = iRectMake(posFireBall.x - p.x, posFireBall.y - p.y, p.x * 2.0f, p.y * 2.0f);

	drawFireBallPos = (posFireBall - p) + pc->camPosition + setPos;
	img->paint(dt, drawFireBallPos, REVERSE_NONE);

}

void FireBall::setAngle()
{
	iPoint v1 = iPointMake(1, 0);
	iPoint v2 = iPointZero;
	img->angle = iPointAngle(v1, v2, v);
}

void FireBall::setlimitRect(uint8 tileNum)
{
	iPoint p = maps[tileNum]->tileOff;

	limitRect = iRectMake(p.x, p.y, RGTILE_X * RGTILE_Width, RGTILE_Y * RGTILE_Height);
}

void createEffect()
{
	iImage* imgFire = new iImage();
	Texture* texFire;
	for (int i = 0; i < 61; i++)
	{
		texFire = createImage("assets/effect/fireball/1_%d.png", i);
		imgFire->addObject(texFire);
		freeImage(texFire);
	}

	imgFire->animation = true;
	imgFire->_repeatNum = 0;
	imgFire->lockAngle = true;
	imgFireBall = imgFire;

	ball = (FireBall**)malloc(sizeof(FireBall*) * FIREBALL_NUM);
	for (int i = 0; i < FIREBALL_NUM; i++)
		ball[i] = new FireBall();

	//----------------------------------------------------------------------------------------
	
	iImage* imgCharge = new iImage();
	Texture* texCharge;
	for (int i = 0; i < 20; i++)
	{
		texCharge = createImage("assets/effect/charge fire/6800%02d.png", i);
		imgCharge->addObject(texCharge);
		freeImage(texCharge);
	}

	imgCharge->_repeatNum = 1;

	imgChargeFire = imgCharge;
}

void freeEffect()
{
	delete imgFireBall;
	delete imgChargeFire;
}

void drawEffect(float dt)
{
	for (int i = 0; i < FIREBALL_NUM; i++)
		ball[i]->paint(dt);
}

int num = 0;
void testFireBall()
{
	iPoint p = iPointMake(1,1);
	ball[num]->alive = true;
	ball[num]->sp = pc->playerPosition;
	ball[num]->posFireBall = pc->playerPosition;
	ball[num]->v = p /= iPointLength(p);
	ball[num]->tileNumber = pc->tileNumber;
	ball[num]->setlimitRect(ball[num]->tileNumber);
	num++;
	if (num > FIREBALL_NUM - 1)
		num = 0;
}


//----------------------------------------------------------------------------------------

void projectileReflect(uint8 tile, iPoint& v, iPoint& pos, iPoint& mp)
{
	MapTile* t = maps[tile];

	int i, j;
	int width = RGTILE_X * RGTILE_Width;
	int height = RGTILE_Y * RGTILE_Height;

	if (v.x < 0.0f)
	{
		int x = (pos.x - t->tileOff.x)  / RGTILE_Width;
		int y = (pos.y - t->tileOff.y) / RGTILE_Height;
		int min = t->tileOff.x - width;

		for (i = x - 1; i > -1; i--)
		{
			bool stop = false;
			if (t->rgTile[RGTILE_X * y + i] == WALLTILE)
			{
				stop = true;
				min = t->tileOff.x + RGTILE_Width * (i + 1);
				break;
			}
			if (stop)
				break;
		}

		pos.x += mp.x;
		if (pos.x < min + 1)
		{
			pos.x = min + 1;
			v.x *= -1.0f;
		}
	}
	else if (v.x > 0.0f)
	{
		int x = (pos.x - t->tileOff.x) / RGTILE_Width;
		int y = (pos.y - t->tileOff.y) / RGTILE_Height;
		int max = t->tileOff.x + width * 2;

		for (i = x + 1; i < RGTILE_X; i++)
		{
			bool stop = false;
			if (t->rgTile[RGTILE_X * y + i] == WALLTILE)
			{
				stop = true;
				max = t->tileOff.x + RGTILE_Width * i - 1;
				break;
			}
			if (stop)
				break;
		}

		pos.x += mp.x;
		if (pos.x > max - 1)
		{
			pos.x = max - 1;
			v.x *= -1.0f;
		}

	}

	if (v.y < 0.0f)
	{
		int x = (pos.x - t->tileOff.x) / RGTILE_Width;
		int y = (pos.y - t->tileOff.y) / RGTILE_Height;
		int min = t->tileOff.y - height;

		for (j = y - 1; j > -1; j--)
		{
			bool stop = false;
			if (t->rgTile[RGTILE_X * j + x] == WALLTILE)
			{
				stop = true;
				min = t->tileOff.y + RGTILE_Height * (j + 1);
				break;
			}
			if (stop)
				break;
		}

		pos.y += mp.y;
		if (pos.y < min + 1)
		{
			pos.y = min + 1;
			v.y *= -1.0f;
		}
	}
	else if (v.y > 0.0f)
	{
		int x = (pos.x - t->tileOff.x) / RGTILE_Width;
		int y = (pos.y - t->tileOff.y) / RGTILE_Height;
		int max = t->tileOff.y + height * 2 ;

		for (j = y + 1; j < RGTILE_Y; j++)
		{
			bool stop = false;
			if (t->rgTile[RGTILE_X * j + x] == WALLTILE)
			{
				stop = true;
				max = t->tileOff.y + RGTILE_Height * j - 1;
				break;
			}
			if (stop)
				break;
		}

		pos.y += mp.y;
		if (pos.y > max + 1)
		{
			pos.y = max + 1;
			v.y *= -1.0f;
		}
	}

}
