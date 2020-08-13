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

	golemBOSS = new EGolemBoss(golemBoss);
	totalMonster[monsterNum] = golemBOSS;
	monsterNum++;
	
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

	delete golemBOSS;
}

void drawEnemy(float dt)
{
	for (int i = 0; i < GOLEM_NUM; i++) // 몬스터 그림자 테스트용
	{
		EGolem* e = golems[i];
		Texture* tex = e->img[0]->tex;
		iPoint p = iPointMake(e->enemyPos.x + tex->width * e->ratio * 0.5f,
			e->enemyPos.y + tex->height * e->ratio * 0.95f) + SET_DRAW_OFF;

		setRGBA(0, 0, 0, 0.1f);
		fillEllipse(p.x, p.y, tex->width * e->ratio * 0.5f, tex->height * e->ratio * 0.25f);
		setRGBA(1, 1, 1, 1);
	}

	for (int i = 0; i < GOLEM_NUM; i++)
		golems[i]->paint(dt);

	for (int i = 0; i < GOLEM_ELETE_NUM; i++)
		golemEletes[i]->paint(dt);

	golemBOSS->paint(dt);
}

//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
