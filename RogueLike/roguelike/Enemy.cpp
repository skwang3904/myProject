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

void testmem()
{
	for (int i = 0; i < 3000; i++)
	{
		printf("%d\n", i);
		createGolemImg();
		freeGolemImg();
	}
}

//----------------------------------------------------------------------------------------

void createGolem()
{
	//testmem();
	createGolemImg();
	//golems = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * GOLEM_NUM);
	//for (int i = 0; i < GOLEM_NUM; i++)
	//	golems[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * 1);

	//for (int i = 0; i < GOLEM_NUM; i++)
	//{
	//	golems[i]->enmtype = golemNomal;
	//	golems[i]->init();
	//}

	////----------------------------------------------------------------------------------------

	//golemEletes = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * GOLEM_ELETE_NUM);
	//for (int i = 0; i < GOLEM_ELETE_NUM; i++)
	//	golemEletes[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * 1);

	//for (int i = 0; i < GOLEM_ELETE_NUM; i++)
	//{
	//	golemEletes[i]->enmtype = golemElete;
	//	golemEletes[i]->init();
	//}

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
			{
					delete eg->img[k];
			}

			free(eg->img);

			if (eg->effectImg)
				delete eg->effectImg;

			for (int k = 0; k < 5; k++)
				delete eg->items[k];
			free(eg->items);

			if (eg->projectile)
			{
				for (int k = 0; k < FIREBALL_NUM; k++)
					delete eg->projectile[k];
				free(eg->projectile);
			}
			free(&golems[i][j]);
		}
		free(golems[i]);
	}
	free(golems);

	//----------------------------------------------------------------------------------------

	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < GOLEM_ELETE_NUM; j++)
		{
			for (int k = 0; k < GOLEM_IMG_NUM; k++)
			{
					delete golemEletes[i][j].img[k];
			}
			free(golemEletes[i][j].img);

			if (golemEletes[i][j].effectImg)
				delete golemEletes[i][j].effectImg;

			for (int k = 0; k < 5; k++)
				delete golemEletes[i][j].items[k];
			free(golemEletes[i][j].items);

			if (golemEletes[i][j].projectile)
			{
				for (int k = 0; k < FIREBALL_NUM; k++)
					delete golemEletes[i][j].projectile[k];
				free(golemEletes[i][j].projectile);
			}
			free(&golemEletes[i][j]);
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
