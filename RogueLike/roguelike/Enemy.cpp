#include "Enemy.h"

#include "Stage.h"

#include "EnemyStruct.h"

void createEnemy()
{
	golems = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * GOLEM_NUM);
	golemEletes = (EnemyGolem**)malloc(sizeof(EnemyGolem*) * GOLEM_ELETE_NUM);
	int i;
	for (i = 0; i < GOLEM_NUM; i++)
		golems[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * 1);
	for (i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i] = (EnemyGolem*)malloc(sizeof(EnemyGolem) * 1);

	createEnemyImg();

	for ( i = 0; i < GOLEM_NUM; i++)
		golems[i]->init(stage);

	for ( i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i]->init(stage);
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