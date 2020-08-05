#include "Stage.h"

#include "Room.h"

#include "EnemyStruct.h"
#include "EnemyData.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"

uint8 stage = 0;
uint8 nextDoor = 255;
bool nextStage = false;

void createStage()
{
	int i,j;
	reSetTile();
	setRoomTile();

	// monster init
	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < GOLEM_NUM; j++)
		{
			MonsterData* eg = &golems[j][i];
			eg->init();
		}
	}

	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < GOLEM_ELETE_NUM; j++)
		{
			MonsterData* eg = &golemEletes[j][i];
			eg->init();
		}
	}


	pc->initPlayerPosition();
	setEnemyPosition();
	//setNextDoor(pcTile);

	stage++;
}

// 직접 문을 그릴때 사용
// 현재 사용X
void setNextDoor(uint8 pcTile)
{
	int i, j, num = 0;
	uint8 activeTile[MAPTILE_NUM];
	memset(activeTile, -1, sizeof(uint8) * MAPTILE_NUM);

	for (i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile == Tile1way1 || maps[i]->rgTile == Tile1way2 ||
			maps[i]->rgTile == Tile1way3 || maps[i]->rgTile == Tile1way4)
		{
			if (i == pcTile)
				continue;
			activeTile[num] = i;
			num++;
		}
	}

	if (i == TILEOFF_NUM - 1 && activeTile[0] == -1)
	{
		for (i = 0; i < TILEOFF_NUM; i++)
		{
			if (maps[i]->rgTile == Tile2way3 || maps[i]->rgTile == Tile2way4 ||
				maps[i]->rgTile == Tile2way5 || maps[i]->rgTile == Tile2way6)
			{
				if (i == pcTile)
					continue;
				activeTile[num] = i;
				num++;
			}
		}
	}

	for (i = 0; ; i++)
	{
		if (activeTile[random() % num] != pcTile)
		{
			nextDoor = activeTile[i];
			break;
		}
	}
}

//  애니메이션 개선할것
static bool stg = false;
void nextStageAni(float dt)
{
	if (pc->img[Player_imgFall]->animation == false && stg == false)
	{
		createStage();
		showRgLoading(true, NextStage);
		stg = true;
	}

	if (stg)
	{
		drawRgLoading(dt, NextStage);

		if (bShowRgLoading(NextStage) == false)
		{
			nextStage = false;

			imgNextDoor->setTexAtIndex(0);
			stg = false;
		}
	}
	else
	{
		iPoint p = pc->playerPosition + SET_DRAW_OFF
			- iPointMake(HALF_OF_TEX_WIDTH / 2, HALF_OF_TEX_HEIGHT / 2);
		pc->img[Player_imgFall]->paint(dt, p);
	}
}

void drawNextDoor(float dt)
{
	if (nextDoor != pc->tileNumber)
		return;

	iPoint p = maps[nextDoor]->tileOff + RGTILE_CENTER + SET_DRAW_OFF
		- iPointMake(imgNextDoor->tex->width / 2, imgNextDoor->tex->height / 2);

	imgNextDoor->paint(dt, p);

	if(nextStage == false)
		containDoor(pc->playerPosition + HALF_OF_TEX_POINT + SET_DRAW_OFF);
}

void containDoor(iPoint p)
{
	iPoint dp = maps[nextDoor]->tileOff + RGTILE_CENTER + SET_DRAW_OFF;
	iPoint touch = iPointMake(imgNextDoor->tex->width / 2, imgNextDoor->tex->height / 2);
	iRect drt = iRectMake(dp.x - touch.x / 2, dp.y - touch.y / 2, touch.x, touch.y);

	if (golemEletes[0]->act == Act_dead)
	{
		imgNextDoor->setTexAtIndex(1);

		if (containPoint(p, drt))
		{
			nextStage = true;
			pc->img[Player_imgFall]->startAnimation();
			audioPlay(SND_JUMP);
		}

	}
}

