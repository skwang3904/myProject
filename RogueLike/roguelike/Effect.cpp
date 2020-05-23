#include "Effect.h"

#include "Room.h"

iImage* imgFireBall;
//FireBall** _ball;
FireBall** ball;

FireBall::FireBall()
{
	img = imgFireBall->copy();

	alive = false;
	dmg = 5.0f;
	speed = 300.0f;
	duration = 0.0f;

	reverse = REVERSE_NONE;
	angle = 0.0f;

	v = iPointZero;
	sp = iPointZero;
	tp = iPointZero;
	posFireBall = iPointZero;
	touchRect = iRectZero;
}

FireBall::~FireBall()
{
	delete img;
}

void FireBall::paint(float dt)
{
	if (alive == false)
		return;

	reflectWall();

	duration += dt;
	if (duration > 10.0f)
	{
		alive = false;
		duration = 0.0f;
		touchRect = iRectZero;
	}

	iPoint p = iPointMake(img->tex->width / 2, img->tex->height / 2);

	touchRect = iRectMake(posFireBall.x - p.x, posFireBall.y - p.y, p.x * 2.0f, p.y * 2.0f);
	setRGBA(0, 1, 0, 0.5);
	fillRect(touchRect);
	setRGBA(1, 1, 1, 1);

	posFireBall += v * speed * dt;

	iPoint texp = posFireBall - p;
	img->paint(dt, texp, REVERSE_NONE);


}

void FireBall::setAngle()
{
	iPoint p = iPointMake(1, 0);
	iPoint tileP = maps[tileNumber]->tileOff;
	angle = 180 - angle;
	//sp ~ posFireBall
	//if (v == iPointMake(1, 0) || v == iPointMake(-1, 0) ||
	//	v == iPointMake(0, 1) || v == iPointMake(0, -1))
	//{
	//	sp = posFireBall;
	//	v *= -1.0f;
	//}
	//else
	//{
	//	sp = posFireBall;
	//}
	//v = (tp - sp) / iPointLength(tp - sp);

	sp = posFireBall;
	v *= -1.0f;

	//reverse = (sp.x > tp.x ? REVERSE_WIDTH : REVERSE_NONE);
}

void FireBall::reflectWall()
{
	for (int i = 0; i < RGTILE_X * RGTILE_Y; i++)
	{
		if (maps[tileNumber]->rgTile[i] == WALLTILE)
		{
			iRect rt = iRectMake(tileOffSet[tileNumber].x+ (RGTILE_X+ 1) * (i % RGTILE_X),
				tileOffSet[tileNumber].y + (RGTILE_Y + 1) * (i / RGTILE_Y),
				RGTILE_Width, RGTILE_Height);
			if (containPoint(posFireBall, rt))
			{
				setAngle();
			}

		}

	}
}

void createFireBallImg()
{
	iImage* img = new iImage();
	Texture* tex;
	for (int i = 0; i < 61; i++)
	{
		tex = createImage("assets/effect/fireball/1_%d.png", i);
		img->addObject(tex);
		freeImage(tex);
	}

	img->animation = true;
	img->_repeatNum = 0;
	imgFireBall = img;

	//_ball = (FireBall**)malloc(sizeof(FireBall*) * 50);
	ball = (FireBall**)malloc(sizeof(FireBall*) * 50);
	for (int i = 0; i < 50; i++)
	{
		ball[i] = new FireBall();
	}
}

void freeFireBallImg()
{
	delete imgFireBall;
}

void testFireBall()
{
	for (int i = 0; i < 50; i++)
	{
		iPoint p = iPointMake(((random() % 300) / 300.0f) - 0.5f, ((random() % 300) / 300.f) - 0.5f);
		ball[i]->alive = true;
		ball[i]->sp = pc->drawPos;
		ball[i]->posFireBall = pc->drawPos;
		ball[i]->v = p;
		ball[i]->tileNumber = pc->tileNumber;
	}
}
