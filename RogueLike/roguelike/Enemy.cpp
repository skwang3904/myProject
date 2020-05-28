#include "Enemy.h"

#include "Stage.h"

#include "EnemyStruct.h"
#include "Effect.h"

void createGolem();
void freeGolem();
void drawGolem(float dt);

void createEnemy()
{
	createGolem();
}

void freeEnemy()
{
	freeGolem();
}

void drawEnemy(float dt)
{
	drawGolem(dt);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

void testmem()
{
	for (int i = 0; i < 3000; i++)
	{
		printf("%d\n", i);
		createGolemImg();
		freeGolemImg();

	}
}

void createGolem()
{
	//testmem();
	createGolemImg();
	golems = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * GOLEM_NUM);
	for (int i = 0; i < GOLEM_NUM; i++)
		golems[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * 1);

	for (int i = 0; i < GOLEM_NUM; i++)
	{
		golems[i]->enmtype = golemNomal;
		golems[i]->init();
	}

	//----------------------------------------------------------------------------------------

	golemEletes = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * GOLEM_ELETE_NUM);
	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * 1);

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
	{
		golemEletes[i]->enmtype = golemElete;
		golemEletes[i]->init();
	}

	//----------------------------------------------------------------------------------------
}

void freeGolem()
{
	freeGolemImg();

	for (int i = 0; i < GOLEM_NUM; i++)
	{
		for (int j = 0; j < GOLEM_IMG_NUM; j++)
		{
			if (golems[i]->img[j])
				delete golems[i]->img[j];
		}
		free(golems[i]->img);

		if (golems[i]->effectImg)
			delete golems[i]->effectImg;

		for (int j = 0; j < 3; j++)
			delete golems[i]->items[j];
		free(golems[i]->items);

		if (golems[i]->projectile)
		{
			for (int j = 0; j < FIREBALL_NUM; j++)
				delete golems[i]->projectile[j];
			free(golems[i]->projectile);
		}

		free(golems[i]);
	}
	free(golems);

	//----------------------------------------------------------------------------------------

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
	{
		for (int j = 0; j < GOLEM_ELETE_IMG_NUM; j++)
		{
			if (golemEletes[i]->img[j])
				delete golemEletes[i]->img[j];
		}
		free(golemEletes[i]->img);

		if (golemEletes[i]->effectImg)
			delete golemEletes[i]->effectImg;

		for (int j = 0; j < 3; j++)
			delete golemEletes[i]->items[j];
		free(golemEletes[i]->items);

		if (golemEletes[i]->projectile)
		{
			for (int j = 0; j < FIREBALL_NUM; j++)
				delete golemEletes[i]->projectile[j];
			free(golemEletes[i]->projectile);
		}

		free(golemEletes[i]);
	}
	free(golemEletes);

	//----------------------------------------------------------------------------------------
}

void drawGolem(float dt)
{
	for (int i = 0; i < GOLEM_NUM; i++)
		golems[i]->paint(dt);

	//----------------------------------------------------------------------------------------

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i]->paint(dt);

	//----------------------------------------------------------------------------------------
}
