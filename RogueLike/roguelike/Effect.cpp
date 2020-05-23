#include "Effect.h"

#include "Room.h"

iImage* imgFireBall;

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

	duration += dt;
	if (duration > 20.0f)
	{
		alive = false;
		duration = 0.0f;
		touchRect = iRectZero;
	}

	iPoint p = iPointMake(img->tex->width / 2, img->tex->height / 2);

	iPoint mp = v * speed * dt;
	projectileReflect(tileNumber, v, posFireBall, mp);
	setAngle();
	touchRect = iRectMake(posFireBall.x - p.x, posFireBall.y - p.y, p.x * 2.0f, p.y * 2.0f);
	//setAngle();
	iPoint texp = (posFireBall - p) + pc->camPosition + setPos;
	img->paint(dt, texp, REVERSE_NONE);

}

void FireBall::setAngle()
{
	iPoint v1 = iPointMake(1, 0);
	iPoint v2 = iPointZero;
	img->angle = iPointAngle(v1, v2, v) ;
	printf("angle %.2f\n", img->angle);
	printf("vx=  %.2f, vy = %.2f\n", v.x,v.y);
	//reverse = (sp.x > tp.x ? REVERSE_WIDTH : REVERSE_NONE);
}

void createEffect()
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
	img->lockAngle = true;
	imgFireBall = img;

	ball = (FireBall**)malloc(sizeof(FireBall*) * 50);
	for (int i = 0; i < 50; i++)
		ball[i] = new FireBall();
}

void freeEffect()
{
	delete imgFireBall;
}

void drawEffect(float dt)
{
	for (int i = 0; i < 50; i++)
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
	num++;
	if (num > 49)
		num = 0;
}


//----------------------------------------------------------------------------------------

void projectileReflect(uint8 tile, iPoint& v, iPoint& pos, iPoint& mp)
{
	MapTile* t = maps[tile];

	int i, j;
	if (v.x < 0.0f)
	{
		int x = (pos.x - t->tileOff.x)  / RGTILE_Width;
		int y = (pos.y - t->tileOff.y) / RGTILE_Height;
		int min = t->tileOff.x - (RGTILE_Y * RGTILE_Height);

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
		int max = t->tileOff.x + (RGTILE_Y * RGTILE_Height) * 2;

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
		int min = t->tileOff.y - (RGTILE_Y * RGTILE_Height);

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
		int max = t->tileOff.y + (RGTILE_Y * RGTILE_Height) * 2 ;

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
