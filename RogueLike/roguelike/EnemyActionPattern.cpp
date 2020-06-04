#include "EnemyActionPattern.h"

#include "Room.h"

#include "EnemyStruct.h"
#include "Effect.h"

bool MethodFalse(EnemyGolem* enm, float dt)
{
	return false;
}

//----------------------------------------------------------------------------------------
// idle

void IdleEyeBlink(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;

	if (e->act != idle)
		return;

	iImage** img = e->img;
	Texture* tex = img[0]->tex;

	iPoint gp = iPointMake(tex->width * e->ratio / 2, tex->height * e->ratio / 2);

	e->reverse = (pc->playerPosition.x + HALF_OF_TEX_WIDTH < e->golemPos.x + gp.x ?
		REVERSE_WIDTH : REVERSE_NONE);

	img[0]->reverse = e->reverse;
	img[1]->reverse = e->reverse;
	iPoint egdp = e->golemPos + SET_DRAW_OFF;

	if (img[0]->animation == false && img[1]->animation == false)
		img[random() % 2]->startAnimation();

	if (img[0]->animation)	img[0]->paint(dt, egdp);
	else					img[1]->paint(dt, egdp);
}

//----------------------------------------------------------------------------------------
// hurt

void commonHurt(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	if (e->act != hurt)
		return;

	iImage** img = e->img;
	Texture* tex = img[0]->tex;

	iPoint gp = iPointMake(tex->width * e->ratio / 2, tex->height * e->ratio / 2);

	e->reverse = (pc->playerPosition.x + HALF_OF_TEX_WIDTH < e->golemPos.x + gp.x ?
		REVERSE_WIDTH : REVERSE_NONE);

	iPoint egdp = e->golemPos + SET_DRAW_OFF;

	if (img[6]->animation == false)
		img[6]->startAnimation();

	img[6]->reverse = e->reverse;
	img[6]->paint(dt, egdp);
	if (img[6]->animation == false)
		e->act = idle;
}

//----------------------------------------------------------------------------------------
// walk

void WalkToPlayer(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	iImage** img = e->img;
	Texture* tex = img[0]->tex;
	iPoint gp = iPointMake(tex->width * e->ratio / 2, tex->height * e->ratio / 2);
	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (e->golemPos + gp);

	if (iPointLength(v) > 500.0f)
	{
		e->act = idle;
		return;
	}

	if (e->act == hurt)
		return;

	e->act = walking;
	v /= iPointLength(v);
	iPoint mp = v * e->moveSpeed * dt;

	gp *= 0.5f;
	iPoint dummy = e->golemPos + gp;
	wallCheck(true, maps[e->tileNumber], dummy, mp, gp.x, gp.y * 1.5f);
	e->golemPos = dummy - gp;

	iPoint egdp = e->golemPos + SET_DRAW_OFF;
	img[2]->reverse = e->reverse;
	img[2]->paint(dt, egdp);
}

//----------------------------------------------------------------------------------------
// attack

bool commonAttack(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;

	if (e->act == rangeAtk)
		return false;

	iPoint et = iPointMake(e->img[0]->tex->width * e->ratio / 2,
		e->img[0]->tex->height * e->ratio / 2);

	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (e->golemPos + et);

	if (iPointLength(v) > e->meleeReach && e->act != meleeAtk)
		return false;

	e->resetActAtAttack();
	if (e->giveDmg == false)
	{
		e->img[3]->startAnimation();

		e->giveDmg = true;
		e->giveDmgTime -= e->meleeAtkSpeed;
		e->act = meleeAtk;

		e->ATV = v;
		//float range = e->meleeReach;
		//e->ATV /= iPointLength(e->ATV);
		//e->ATV = e->golemPos + et + (e->ATV * range);
		e->ATV += e->golemPos + et;
		e->reverse = (v.x > 0.0f ? REVERSE_NONE : REVERSE_WIDTH);
	}

	e->giveDmgTime += dt;
	iPoint egdp = e->golemPos + SET_DRAW_OFF;
	e->img[3]->reverse = e->reverse;
	e->img[3]->paint(dt, egdp);

	if (e->giveDmgTime > 0.0f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
	}

	if (e->giveDmg == true && e->giveDmgTime
		> 0.0f - e->meleeAtkSpeed * 0.4f && e->giveDmgTime < 0.0f - e->meleeAtkSpeed * 0.3f)
	{
		setLineWidth(10);
		setRGBA(1, 0, 0, 1);
		drawLine(egdp + et,
			e->ATV + SET_DRAW_OFF);
		setLineWidth(1);
		setRGBA(1, 1, 1, 1);

		if (e->hit == false && pc->act != evasion)
		{
			iPoint n = e->ATV - e->golemPos + et;
			float len = iPointLength(n);
			n /= len;
			iPoint p = pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH,HALF_OF_TEX_HEIGHT) - e->golemPos + et;
			float dot = min(max(p.x * n.x + p.y * n.y, 0.0f), len);
			iPoint proj =  n * dot;
			float hitDis = iPointLength(p - proj);
			//printf("dis %.2f\n", hitDis);

			if (hitDis < e->meleeReach * 0.33f)
			{
				if (pc->act == evasion || pc->act == falling)
					return true;

				pc->hp -= e->attackDmg;
				e->hit = true;
			}
		}
	}

	//if (iPointLength(v) > e->meleeReach + 50 && e->giveDmgTime > 0.0f - e->meleeAtkSpeed * 0.1f)
	//{
	//	e->giveDmg = false;
	//	e->giveDmgTime = 0.0f;
	//	e->hit = false;
	//	e->act = idle;
	//	e->img[3]->animation == false;
	//	return false;
	//}

	return true;
}

bool rangeAttack(EnemyGolem* enm, float dt)
{
	EnemyGolem* e = enm;
	Texture* tex = e->img[0]->tex;
	iPoint et = iPointMake(tex->width * e->ratio / 2,
		tex->height * e->ratio / 2);

	iPoint v = (pc->playerPosition + HALF_OF_TEX_POINT)
		- (e->golemPos + et);

	int i;
	for (i = 0; i < FIREBALL_NUM; i++)
	{
		FireBall* fb = e->projectile[i];
		fb->paint(dt);
		if (fb->hitFireBall(pc->touchPlayer))
		{
			if (pc->act != evasion && pc->act != falling)
			{
				pc->hp -= e->attackDmg;
				fb->alive = false;
				audioPlay(SND_ENEMY_HIT);
			}
		}
	}

	if (e->tileNumber == pc->tileNumber)
		e->rangeTime += dt;

	if (e->act == meleeAtk || e->rangeTime < e->_rangeTime)
		return false;
	if ((iPointLength(v) > e->rangeReach || iPointLength(v) < e->rangeReach -50 )&&
		 e->act != rangeAtk)
		return false;

	e->resetActAtAttack();
	uint8 a = 0;
	iPoint fbp = iPointMake(e->projectile[0]->img->tex->width, e->projectile[0]->img->tex->height);
	iPoint cfp = iPointMake(e->effectImg->tex->width, e->effectImg->tex->height);
	if (e->giveDmg == false)
	{
		e->img[5]->startAnimation();

		e->giveDmg = true;
		e->giveDmgTime -= e->rangeAtkSpeed;
		e->act = rangeAtk;
		e->reverse = (v.x > 0.0f ? REVERSE_NONE : REVERSE_WIDTH);

		a = 5 + (random() % (FIREBALL_NUM - 5));
		float speed = 0.0f;
		float timer = 0.0f;
		float duration = 0.0f;
		iPoint fv = e->golemPos - fbp / 2 + iPointMake(et.x, 0);
		iPoint vv = iPointZero;
		for (i = 0; i < a; i++)
		{
			//duration = 5 + ((random() % 100) / 20.0f);
			duration = 0.0f;
			//timer = (random() % 100) / 100.0f;
			timer = 0;
			speed = 200.0f + random() % 300;
			//speed = 100.0f;
			vv = iPointMake(1.0f - (random() % 200) / 100, 1.0f - (random() % 200) / 100);

			if (e->projectile[i]->alive == false)
				e->projectile[i]->init(duration, timer, speed, e->tileNumber, vv, fv);
			else
			{
				a++;
				if (a > FIREBALL_NUM - 1)
					break;
			}
		}
		e->effectImg->startAnimation();
	}

	iPoint egdp = e->golemPos + SET_DRAW_OFF;
	e->giveDmgTime += dt;
	e->img[5]->reverse = e->reverse;
	if ((e->img[5]->frame < 6))
		e->img[5]->paint(dt * 2.0f, egdp);
	else
	{
		e->effectImg->paint(dt, egdp - cfp/2.0f + iPointMake(et.x, 0));
		e->img[5]->paint(dt * 0.75f, egdp);
	}

	if (e->giveDmgTime > 0.0f)
	{
		e->giveDmg = false;
		e->giveDmgTime = 0.0f;
		e->hit = false;
		e->act = idle;
		e->rangeTime = 0.0f;
	}

	return true;
}

//----------------------------------------------------------------------------------------
