#include "Enemy.h"

#include "Stage.h"

#include "EnemyStruct.h"

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
	for (int i = 0; i < GOLEM_NUM; i++)
		golems[i]->paint(dt);

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i]->paint(dt);
}