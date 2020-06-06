#include "Enemy.h"

#include "Effect.h"

#include "EnemyComposition.h"
#include "EnemyStruct.h"
#include "EnemyData.h"

void createEnemy()
{
	//------------------------------------------------------------------------
	// golems
	createGolemImg();
	golems = (EGolem**)malloc(sizeof(EGolem*) * GOLEM_NUM);
	for (int i = 0; i < GOLEM_NUM; i++)
		golems[i] = new EGolem(golemNomal);

	golemEletes = (EGolem**)malloc(sizeof(EGolem*) * GOLEM_ELETE_NUM);
	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i] = new EGolem(golemElete);

	setEnemyPosition(pc->tileNumber);
}

void freeEnemy()
{
	freeGolemImg();
	for (int i = 0; i < GOLEM_NUM; i++)
		delete golems[i];
	free(golems);

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
		delete golemEletes[i];
	free(golemEletes);
}

void drawEnemy(float dt)
{
	for (int i = 0; i < GOLEM_NUM; i++)
		golems[i]->paint(dt);

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i]->paint(dt);
}

//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
