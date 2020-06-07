#include "Enemy.h"

#include "Effect.h"

#include "EnemyComposition.h"
#include "EnemyStruct.h"
#include "EnemyData.h"

void createEnemy()
{
	//------------------------------------------------------------------------
	// golems
	totalMonster = (MonsterData**)malloc(sizeof(MonsterData*) * 64);

	createGolemImg();
	golems = (EGolem**)malloc(sizeof(EGolem*) * GOLEM_NUM);
	for (int i = 0; i < GOLEM_NUM; i++)
	{
		golems[i] = new EGolem(golemNomal);
		totalMonster[monsterNum] = golems[i];
		monsterNum++;
	}

	golemEletes = (EGolem**)malloc(sizeof(EGolem*) * GOLEM_ELETE_NUM);
	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
	{
		golemEletes[i] = new EGolem(golemElete);
		totalMonster[monsterNum] = golemEletes[i];
		monsterNum++;
	}
	
	setEnemyPosition();
}

void freeEnemy()
{
	free(totalMonster);

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
