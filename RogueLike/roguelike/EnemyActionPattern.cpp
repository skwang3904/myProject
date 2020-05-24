#include "EnemyActionPattern.h"

#include "Room.h"

#include "EnemyStruct.h"
#include "Effect.h"

//----------------------------------------------------------------------------------------
// idle

void IdleEyeBlink(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	iImage** img = e->img;
	Texture* tex = img[0]->tex;

	iPoint gp = iPointMake(tex->width * e->ratio / 2.0f, tex->height * e->ratio / 2.0f);
	e->reverse = (pc->playerPosition.x + HALF_OF_TEX_WIDTH < e->golemPos.x + gp.x ?
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

	int i;
	for (i = 0; i < FIREBALL_NUM; i++)
	{
		e->projectile[i]->paint(dt);
		if (e->projectile[i]->hitFireBall(pc->touchPlayer))
		{
			//pc->hp -= e->attackDmg;
			printf("hit fireball\n");
		}
	}

	if (iPointLength(v) > 300 && e->giveDmg == false && e->act != attacking)
		return false;

	uint8 a = 0;
	if (e->giveDmg == false)
	{
		e->img[5]->startAnimation();

		e->giveDmg = true;
		e->giveDmgTime -= e->attackSpeed;
		e->act = attacking;

		e->ATV = v + e->golemPos + et;
		e->reverse = (v.x > 0.0f ? REVERSE_NONE : REVERSE_WIDTH);

		a = random() % FIREBALL_NUM;
		float speed = 0.0f;
		float timer = 0.0f;
		float duration = 0.0f;
		iPoint fv = e->golemPos + et + iPointMake(0, -80);
		iPoint vv = iPointZero;
		for (i = 0; i < a; i++)
		{
			duration = 5 + ((random() % 100) / 20.0f);
			timer = (random() % 100) / 100.0f;
			//speed = 200.0f + random() % 300;
			speed = 50.0f;
			vv = iPointMake(1.0f - (random() % 200) / 100.0f, 1.0f - (random() % 200) / 100.0f);

			e->projectile[i]->init(duration, timer, speed, e->tileNumber, vv, fv);
		}
		e->effectImg->startAnimation();
	}

	e->giveDmgTime += dt;
	if((e->img[5]->frame < 6))
		e->img[5]->paint(dt * 2.0f, e->drawGolemPos, e->reverse);
	else
	{

		e->effectImg->paint(dt, e->drawGolemPos + iPointMake(-20, -100), REVERSE_NONE);
		e->img[5]->paint(dt * 0.75f, e->drawGolemPos, e->reverse);
	}

	if (e->giveDmgTime > 0.0f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
	}

	if (iPointLength(v) < e->reach -50 && e->giveDmgTime > 0.0f - e->attackSpeed * 0.5f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
		e->img[5]->animation == false;
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------
