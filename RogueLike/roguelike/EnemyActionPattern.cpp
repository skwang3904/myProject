#include "EnemyActionPattern.h"

#include "Room.h"

#include "EnemyStruct.h"

//----------------------------------------------------------------------------------------
// idle

void IdleEyeBlink(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	iImage** img = e->img;

	e->reverse = (pc->playerPosition.x < e->golemPos.x ?
		REVERSE_WIDTH : REVERSE_NONE);
	uint8 r = e->reverse;
	iPoint p = e->drawGolemPos;

	if (img[0]->animation == false && img[1]->animation == false)
		img[random() % 2]->startAnimation();

	if (e->act != attacking)
	{
		if (e->act == idle)
		{
			if (img[0]->animation)	img[0]->paint(dt, p, r);
			else	img[1]->paint(dt, p, r);
		}
		else //if (act == walking)
			img[2]->paint(dt, p, r);
	}
}

//----------------------------------------------------------------------------------------
// walk

void WalkToPlayer(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	Texture* tex = e->img[0]->tex;
	iPoint gp = iPointMake(tex->width * e->ratio / 2.0f, tex->height * e->ratio / 2.0f);
	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (e->golemPos + gp);

	if (iPointLength(v) > 300.0f || iPointLength(v) < e->reach)
	{
		e->act = idle;
		return;
	}

	v /= iPointLength(v);
	iPoint mp = v * e->moveSpeed * dt;

	gp *= 0.5f;
	iPoint dummy = e->golemPos + gp;
	wallCheck(true, maps[e->tileNumber], dummy, mp, gp.x, gp.y * 1.5f);
	e->golemPos = dummy - gp;
}

//----------------------------------------------------------------------------------------
// attack

bool commonAttack(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	iPoint et = iPointMake(e->img[0]->tex->width * e->ratio / 2.0f,
		e->img[0]->tex->height * e->ratio / 2.0f);

	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (e->golemPos + et);

	if (iPointLength(v) > e->reach&& e->giveDmg == false && e->act != attacking)
		return false;

	if (e->giveDmg == false)
	{
		e->img[3]->startAnimation();

		e->giveDmg = true;
		e->giveDmgTime -= e->attackSpeed;
		e->act = attacking;

		e->ATV = v;
		float range = e->reach;
		e->ATV /= iPointLength(e->ATV);
		e->ATV = e->golemPos + et + (e->ATV * range);
		e->reverse = (v.x > 0.0f ? REVERSE_NONE : REVERSE_WIDTH);
	}

	e->giveDmgTime += dt;
	e->img[3]->paint(dt, e->drawGolemPos, e->reverse);

	if (e->giveDmgTime > 0.0f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
	}

	if (e->giveDmg == true && e->giveDmgTime > 0.0f - e->attackSpeed * 0.33f)
	{
		setLineWidth(10);
		setRGBA(1, 0, 0, 1);
		drawLine(e->drawGolemPos + et,
			e->ATV + pc->camPosition + setPos);
		setLineWidth(1);
		setRGBA(1, 1, 1, 1);

		if (e->hit == false && pc->act != evasion)
		{
			iPoint n = e->ATV - e->golemPos + et;
			float len = iPointLength(n);
			n /= iPointLength(n);
			iPoint p = pc->playerPosition + HALF_OF_TEX_POINT
				- e->golemPos + et;

			float dot = min(max(p.x * n.x + p.y * n.y, 0), len);
			iPoint proj = n * dot;
			float hitDis = iPointLength(p - proj);

			if (hitDis < HALF_OF_TEX_WIDTH / 2)
			{
				if (pc->act == evasion || pc->act == falling)
					return true;
				printf("hits\n");
				//pc->hp -= attackDmg;
				e->hit = true;
			}
		}
	}

	if (iPointLength(v) > e->reach + 50 && e->giveDmgTime > 0.0f - e->attackSpeed * 0.1f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
		e->img[3]->animation == false;
		return false;
	}

	return true;
}

bool rangeAttack(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	iPoint et = iPointMake(e->img[0]->tex->width * e->ratio / 2.0f,
		e->img[0]->tex->height * e->ratio / 2.0f);

	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (e->golemPos + et);

	if (iPointLength(v) > 300 && e->giveDmg == false && e->act != attacking)
		return false;

	if (e->giveDmg == false)
	{
		e->img[5]->startAnimation();

		e->giveDmg = true;
		e->giveDmgTime -= e->attackSpeed;
		e->act = attacking;

		e->ATV = v + e->golemPos + et;
		e->reverse = (v.x > 0.0f ? REVERSE_NONE : REVERSE_WIDTH);
	}

	e->giveDmgTime += dt;
	if((e->img[5]->frame < 6))
		e->img[5]->paint(dt, e->drawGolemPos, e->reverse);
	else
		e->img[5]->paint(dt/2.0f, e->drawGolemPos, e->reverse);
		


	iPoint tp = e->ATV + pc->camPosition + setPos;

	iPoint ranPos1 = tp + iPointMake(50, 50);
	iPoint ranPos2 = tp + iPointMake(-50, -50);
	iPoint ranPos3 = tp + iPointMake(50, -50);
	iPoint ranPos4 = tp + iPointMake(-50, 50);

	setRGBA(1, 0, 0, 1);

	fillRect(ranPos1.x - 20, ranPos1.y - 20, 40, 40);
	fillRect(ranPos2.x - 20, ranPos2.y - 20, 40, 40);
	fillRect(ranPos3.x - 20, ranPos3.y - 20, 40, 40);
	fillRect(ranPos4.x - 20, ranPos4.y - 20, 40, 40);

	fillRect(tp.x - 20, tp.y - 20, 40, 40);
	setRGBA(1, 1, 1, 1);

	if (e->giveDmgTime > 0.0f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
	}

	if (e->giveDmg == true && e->giveDmgTime > 0.0f - e->attackSpeed * 0.33f)
	{
		//setLineWidth(10);
		//setRGBA(1, 0, 0, 1);
		//drawLine(e->drawGolemPos + et,
		//	e->ATV + pc->camPosition + setPos);
		//setLineWidth(1);
		//setRGBA(1, 1, 1, 1);

		if (e->hit == false && pc->act != evasion)
		{
			iPoint n = e->ATV - e->golemPos + et;
			float len = iPointLength(n);
			n /= iPointLength(n);
			iPoint p = pc->playerPosition + HALF_OF_TEX_POINT
				- e->golemPos + et;

			float dot = min(max(p.x * n.x + p.y * n.y, 0), len);
			iPoint proj = n * dot;
			float hitDis = iPointLength(p - proj);

			if (hitDis < HALF_OF_TEX_WIDTH / 2)
			{
				if (pc->act == evasion || pc->act == falling)
					return true;
				printf("hits\n");
				//pc->hp -= attackDmg;
				e->hit = true;
			}
		}
	}

	if (iPointLength(v) > e->reach + 50 && e->giveDmgTime > 0.0f - e->attackSpeed * 0.1f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
		e->img[3]->animation == false;
		return false;
	}

	return true;
}
