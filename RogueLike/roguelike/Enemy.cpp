#include "Enemy.h"

#include "Stage.h"

#include "EnemyStruct.h"
#include "Effect.h"

void createGolem();
void freeGolem();
void drawGolem(float dt);

void testeg()
{
	for (int i = 0; i < 300; i++)
	{
		printf("i = %d\n",i);
		createGolem();
		freeGolem();
	}

	for (int i = 0; i < 300; i++)
	{
		printf("i = %d\n", i);
		createGolem();
		freeGolem();
	}
}

void createEnemy()
{
	//testeg();
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

void createGolem()
{
	createGolemImg();

	//----------------------------------------------------------------------------------------
	golems = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * 1);
	for (int i = 0; i < 1; i++)
	{
		golems[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * GOLEM_NUM);
		for (int j = 0; j < GOLEM_NUM; j++)
		{
			EnemyGolem* eg = &golems[i][j];
			eg->enmtype = golemNomal;
			eg->init();
		}
	}

	//----------------------------------------------------------------------------------------

	golemEletes = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * 1);
	for (int i = 0; i < 1; i++)
	{
		golemEletes[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * GOLEM_ELETE_NUM);
		for (int j = 0; j < GOLEM_ELETE_NUM; j++)
		{
			EnemyGolem* eg = &golemEletes[i][j];
			eg->enmtype = golemElete;
			eg->init();
		}
	}
}

void freeGolem()
{
	freeGolemImg();

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < GOLEM_NUM; j++)
		{
			EnemyGolem* eg = &golems[i][j];
			for (int k = 0; k < GOLEM_IMG_NUM; k++)
				delete eg->img[k];
			free(eg->img);

			if (eg->effectImg)
				delete eg->effectImg;

			if (eg->items)
			{
				for (int k = 0; k < 5; k++)
					delete eg->items[k];
				free(eg->items);
			}

			if (eg->projectile)
			{
				for (int k = 0; k < FIREBALL_NUM; k++)
					delete eg->projectile[k];
				free(eg->projectile);
			}
		}
		free(golems[i]);
	}
	free(golems);

	//----------------------------------------------------------------------------------------

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < GOLEM_ELETE_NUM; j++)
		{
			EnemyGolem* eg = &golemEletes[i][j];
			for (int k = 0; k < GOLEM_IMG_NUM; k++)
				delete eg->img[k];
			free(eg->img);

			if (eg->effectImg)
				delete eg->effectImg;

			if (eg->items)
			{
				for (int k = 0; k < 5; k++)
					delete eg->items[k];
				free(eg->items);
			}

			if (eg->projectile)
			{
				for (int k = 0; k < FIREBALL_NUM; k++)
					delete eg->projectile[k];
				free(eg->projectile);
			}
		}
		free(golemEletes[i]);
	}
	free(golemEletes);

	//----------------------------------------------------------------------------------------
}

void drawGolem(float dt)
{
	int i, j;
	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < GOLEM_NUM; j++)
		{
			EnemyGolem* eg = &golems[i][j];
			eg->paint(dt);
		}
	}

	//----------------------------------------------------------------------------------------
	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < GOLEM_ELETE_NUM; j++)
		{
			EnemyGolem* eg = &golemEletes[i][j];
			eg->paint(dt);
		}

	}

	//----------------------------------------------------------------------------------------
}
