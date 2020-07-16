#include "Effect.h"

#include "Room.h"

#include "EnemyStruct.h"

iImage* imgFireBall;
iImage* imgChargeFire;

FireBall::FireBall()
{
	img = imgFireBall->copy();

	alive = false;
	dmg = 0.0f;
	speed = 300.0f;
	duration = alive_duration;
	timer = fire_timer;

	tileNumber = 0;

	v = iPointZero;
	sp = iPointZero;
	position = iPointZero;

	touchPos = iPointZero;
	limitRect = iRectZero;
}

FireBall::~FireBall()
{
	delete img;
}

void FireBall::init(float d, float firetime, float firespeed, int tileNum, iPoint& vlen, iPoint& pos)
{
	if (alive)
		return;

	alive = true;
	duration = d;
	timer = firetime;
	speed = firespeed;
	tileNumber = tileNum;
	setlimitRect(tileNum);
	v = vlen / iPointLength(vlen);
	sp = pos;
	position = pos;
}

void FireBall::paint(float dt)
{
	if (alive == false)
		return;

	if (timer < fire_timer)
	{
		timer += dt;
		return;
	}

	duration += dt;
	if (duration > alive_duration || containPoint(position, limitRect) == false)
		alive = false;

	iPoint p = iPointMake(img->tex->width / 2, img->tex->height / 2);
	iPoint posFB = position + p;
	iPoint mp = v * speed * dt;
	projectileReflect(tileNumber, v, posFB, mp);
	position = posFB - p;
	setAngle();

	touchPos = position + p;
	iPoint test = SET_DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(touchPos.x + test.x, touchPos.y + test.y, 10, 10);
	setRGBA(1, 1, 1, 1);

	iPoint drawFireBallPos = position + SET_DRAW_OFF;
	img->paint(dt, drawFireBallPos);
}

void FireBall::setAngle()
{
	iPoint v1 = iPointMake(1, 0);
	iPoint v2 = iPointZero;
	img->angle = iPointAngle(v1, v2, v);
}

void FireBall::setlimitRect(int tileNum)
{
	iPoint p = maps[tileNum]->tileOff;

	limitRect = iRectMake(p.x, p.y, RGTILE_X * RGTILE_Width, RGTILE_Y * RGTILE_Height);
}

bool FireBall::hitFireBall(iRect& rt)
{
	if (alive == false)
		return false;

	iPoint p = position + iPointMake(img->tex->width / 2, img->tex->height / 2);
	if (containPoint(p, rt))
	{
		alive = false;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------

iImage* imgArrow;

Arrow::Arrow()
{
	img = imgArrow->copy();

	alive = false;
	dmg = 0.0f;
	speed = 300.0f;
	duration = alive_duration;
	timer = fire_timer;

	tileNumber = 0;

	v = iPointZero;
	sp = iPointZero;
	position = iPointZero;

	touchPos = iPointZero;
	limitRect = iRectZero;
}

Arrow::~Arrow()
{
	delete img;
}

void Arrow::init(float d, float firetime, float firespeed, int tileNum, iPoint& vlen, iPoint& pos)
{
	if (alive)
		return;

	alive = true;
	duration = d;
	timer = firetime;
	speed = firespeed;
	tileNumber = tileNum;
	setlimitRect(tileNum);
	v = vlen / iPointLength(vlen);
	sp = pos;
	position = pos;
}

void Arrow::paint(float dt)
{
	if (alive == false)
		return;

	if (timer < fire_timer)
	{
		timer += dt;
		return;
	}

	duration += dt;
	if (duration > alive_duration || containPoint(position, limitRect) == false)
		alive = false;

	iPoint p = iPointMake(img->tex->width / 2, img->tex->height / 2);
	iPoint posFB = position + p;
	iPoint mp = v * speed * dt;
	projectileReflect(tileNumber, v, posFB, mp);
	position = posFB - p;
	setAngle();

	touchPos = position + p;
	iPoint test = SET_DRAW_OFF;
	setRGBA(1, 0, 0, 1);
	fillRect(touchPos.x + test.x, touchPos.y + test.y, 10, 10);
	setRGBA(1, 1, 1, 1);

	iPoint drawFireBallPos = position + SET_DRAW_OFF;
	img->paint(dt, drawFireBallPos);
}

void Arrow::setAngle()
{
	iPoint v1 = iPointMake(1, 0);
	iPoint v2 = iPointZero;
	img->angle = iPointAngle(v1, v2, v);
}

void Arrow::setlimitRect(int tileNum)
{
	iPoint p = maps[tileNum]->tileOff;

	limitRect = iRectMake(p.x, p.y, RGTILE_X * RGTILE_Width, RGTILE_Y * RGTILE_Height);
}

bool Arrow::hitArrow(iRect& rt)
{
	if (alive == false)
		return false;

	iPoint p = position + iPointMake(img->tex->width / 2, img->tex->height / 2);
	if (containPoint(p, rt))
	{
		alive = false;
		return true;
	}
	return false;
}


void createEffect()
{
	int i;
	iImage* img = new iImage();
	for (i = 0; i < 61; i++)
	{
		Texture* tex = createImage("assets/effect/fireball/3/1_%d.png", i);
		img->addObject(tex);
		freeImage(tex);
	}

	img->_aniDt = 0.04f;
	img->animation = true;
	img->_repeatNum = 0;
	img->lockAngle = true;
	imgFireBall = img;

	//----------------------------------------------------------------------------------------
	
	img = new iImage();
	for (i = 0; i < 20; i++)
	{
		Texture* tex = createImage("assets/effect/charge fire/6800%02d.png", i);
		img->addObject(tex);
		freeImage(tex);
	}

	img->_repeatNum = 1;
	img->_aniDt = 0.1f;

	imgChargeFire = img;

	//----------------------------------------------------------------------------------------

	img = new iImage();
	for (i = 0; i < 1; i++)
	{
		Texture* tex = createImage("assets/weapon/arrow.png");
		img->addObject(tex);
		freeImage(tex);
	}

	imgArrow = img;
}

void freeEffect()
{
	delete imgFireBall;
	delete imgChargeFire;
	delete imgArrow;
}

void drawEffect(float dt)
{
	
}

//----------------------------------------------------------------------------------------

void projectileReflect(int tile, iPoint& v, iPoint& pos, iPoint& mp)
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

