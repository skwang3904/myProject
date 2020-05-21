#include "Enemy.h"

#include "Room.h"
#include "Stage.h"

#include "EnemyStruct.h"
#include "EnemyComposition.h"

#include "RgProc.h"

void createEnemy()
{
	golems = (EnemyNomalGolem**)malloc(sizeof(EnemyNomalGolem*) * GOLEM_NUM);

	golems[0] = (EnemyNomalGolem*)malloc(sizeof(EnemyNomalGolem) * 1);
	golems[0]->createEnemyImg();
	golems[0]->init(stage);
	for (int i = 1; i < GOLEM_NUM; i++)
	{
		golems[i] = (EnemyNomalGolem*)malloc(sizeof(EnemyNomalGolem) * 1);
		golems[i]->init(stage);
		iImage** imgs = (iImage**)malloc(sizeof(iImage*) * 5);
		for (int j = 0; j < 5; j++)
		{
			imgs[j] = golems[0]->img[j]->copy();
		}
		golems[i]->img = imgs;
	}

	//----------------------------------------------------------------------------------------

	golemEletes = (EnemyEleteGolem**)malloc(sizeof(EnemyEleteGolem*) * GOLEM_NUM);

	golemEletes[0] = (EnemyEleteGolem*)malloc(sizeof(EnemyEleteGolem) * 1);
	golemEletes[0]->createEnemyImg();
	golemEletes[0]->init(stage);
	for (int i = 1; i < GOLEM_NUM; i++)
	{
		golemEletes[i] = (EnemyEleteGolem*)malloc(sizeof(EnemyEleteGolem) * 1);
		golemEletes[i]->init(stage);
		iImage** imgs = (iImage**)malloc(sizeof(iImage*) * 5);
		for (int j = 0; j < 5; j++)
		{
			imgs[j] = golemEletes[0]->img[j]->copy();
		}
		golemEletes[i]->img = imgs;
	}
}

void freeEnemy()
{
	for (int i = 0; i < GOLEM_NUM; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (golems[i]->img[j])
				delete golems[i]->img[j];
		}
		free(golems[i]->img);
	}
	free(golems);

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (golemEletes[i]->img[j])
				delete golemEletes[i]->img[j];
		}
		free(golemEletes[i]->img);
	}
	free(golemEletes);
}

void drawEnemy(float dt)
{
	static int reverse;
	for (int i = 0; i < GOLEM_NUM; i++) //monNum
	{
		EnemyNomalGolem* enm = golems[i];
		Texture* tex = enm->img[0]->tex;

		enm->drawGolemPos = enm->golemPos + pc->camPosition + setPos;
		if (enm->act == dead)
			continue;

		iRect rt = iRectMake(enm->drawGolemPos.x + tex->width * GOLEM_RATIO * 0.25, 
			enm->drawGolemPos.y + tex->height * GOLEM_RATIO * 0.25f,
			tex->width * GOLEM_RATIO * 0.5f,
			tex->height * GOLEM_RATIO * 0.75f);
		iRect rt1 = iRectMake(enm->drawGolemPos.x,
			enm->drawGolemPos.y,
			tex->width * GOLEM_RATIO ,
			tex->height * GOLEM_RATIO);

		if (enm->hp > 0.0f)
		{
			reverse = (pc->playerPosition.x < enm->golemPos.x ? REVERSE_WIDTH : REVERSE_NONE);
			if (enm->img[0]->animation == false && enm->img[1]->animation == false)
				enm->img[random() % 2]->startAnimation();

			if (enm->act != attacking)
			{
				if (enm->act == idle)
				{
					if (enm->img[0]->animation)	enm->img[0]->paint(dt, enm->drawGolemPos, reverse);
					else	enm->img[1]->paint(dt, enm->drawGolemPos, reverse);
				}
				else //if (enm->act == walking)
					enm->img[2]->paint(dt, enm->drawGolemPos, reverse);
			}
			enm->touchGolem = rt;

			setRGBA(0, 0, 0, 1);
			drawRect(enm->touchGolem);
			setRGBA(1, 0, 1, 1);
			drawRect(rt1);
			setRGBA(1, 1, 1, 1);

			if (enm->takeDmg)
				enm->takeDmgEffect(dt);

			if (enm->showHp)
				enm->drawShowHp(dt);

			if (enm->enemysAttack(dt) == false && //수정
				((iPointZero - pc->camPosition).x  < enm->golemPos.x &&
				(iPointZero - pc->camPosition).x + RGTILE_X * RGTILE_Width - 1 > enm->golemPos.x + tex->width * GOLEM_RATIO * 0.75f &&
					(iPointZero - pc->camPosition).y < enm->golemPos.y &&
					(iPointZero - pc->camPosition).y + RGTILE_Y * RGTILE_Height - 1 > enm->golemPos.y + tex->height * GOLEM_RATIO * 0.75f))
			{
				enm->act = walking;
				enm->moveEnemyType1(dt);
			}
		}
		else
		{
			if (enm->act !=dying)
			{
				enm->act = dying;
				audioPlay(SND_ENEMY_DEAD);
				enm->img[4]->startAnimation();
			}

			enm->img[4]->paint(dt, enm->drawGolemPos, reverse);

			if (enm->img[4]->animation == false)
			{
				enm->golemPos = iPointZero;
				enm->touchGolem = iRectZero;
				enm->act = dead;
			}
		}
	}

	//---------------------------------------------------------------------------------
	static int reverseE;
	for (int i = 0; i < GOLEM_ELETE_NUM; i++) //monNum
	{
		EnemyEleteGolem* enm = golemEletes[i];
		Texture* tex = enm->img[0]->tex;

		enm->drawGolemPos = enm->golemPos + pc->camPosition + setPos;
		if (enm->act == dead)
			continue;

		iRect rt = iRectMake(enm->drawGolemPos.x + tex->width * GOLEM_ELETE_RATIO * 0.25,
			enm->drawGolemPos.y + tex->height * GOLEM_ELETE_RATIO * 0.25f,
			tex->width * GOLEM_ELETE_RATIO * 0.5f,
			tex->height * GOLEM_ELETE_RATIO * 0.75f);
		iRect rt1 = iRectMake(enm->drawGolemPos.x,
			enm->drawGolemPos.y,
			tex->width * GOLEM_ELETE_RATIO,
			tex->height * GOLEM_ELETE_RATIO);


		if (enm->hp > 0.0f)
		{
			reverse = (pc->playerPosition.x < enm->golemPos.x ? REVERSE_WIDTH : REVERSE_NONE);
			if (enm->img[0]->animation == false && enm->img[1]->animation == false)
				enm->img[random() % 2]->startAnimation();

			if (enm->act != attacking)
			{
				if (enm->act == idle)
				{
					if (enm->img[0]->animation)	enm->img[0]->paint(dt, enm->drawGolemPos, reverseE);
					else	enm->img[1]->paint(dt, enm->drawGolemPos, reverseE);
				}
				else //if (enm->act == walking)
					enm->img[2]->paint(dt, enm->drawGolemPos, reverseE);
			}
			enm->touchGolem = rt;

			setRGBA(0, 0, 0, 1);
			drawRect(enm->touchGolem);
			setRGBA(1, 0, 1, 1);
			drawRect(rt1);
			setRGBA(1, 1, 1, 1);

			if (enm->takeDmg)
				enm->takeDmgEffect(dt);

			if (enm->showHp)
				enm->drawShowHp(dt);

			if (enm->enemysAttack(dt) == false && //수정
				((iPointZero - pc->camPosition).x  < enm->golemPos.x &&
				(iPointZero - pc->camPosition).x + RGTILE_X * RGTILE_Width - 1 > enm->golemPos.x + tex->width * GOLEM_ELETE_RATIO * 0.75f &&
					(iPointZero - pc->camPosition).y < enm->golemPos.y &&
					(iPointZero - pc->camPosition).y + RGTILE_Y * RGTILE_Height - 1 > enm->golemPos.y + tex->height * GOLEM_ELETE_RATIO * 0.75f))
			{
				enm->act = walking;
				enm->moveEnemyType1(dt);
			}
		}
		else
		{
			if (enm->act != dying)
			{
				enm->act = dying;
				audioPlay(SND_ENEMY_DEAD);
				enm->img[4]->startAnimation();
			}

			enm->img[4]->paint(dt, enm->drawGolemPos, reverseE);

			if (enm->img[4]->animation == false)
			{
				enm->golemPos = iPointZero;
				enm->touchGolem = iRectZero;
				enm->act = dead;
			}
		}
	}
}