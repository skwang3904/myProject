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
	setRoomTile();

	// monster init
	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < GOLEM_NUM; j++)
		{
			MonsterData* eg = &golems[i][j];
			eg->init();
		}
	}

	for (i = 0; i < 1; i++)
	{
		for (j = 0; j < GOLEM_ELETE_NUM; j++)
		{
			MonsterData* eg = &golemEletes[i][j];
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

void nextStageAni(float dt)
{
	if (pc->img[Player_imgFall]->animation == false && nextDoor != 255)
	{
		pc->img[Player_imgFall]->startAnimation();
		pc->img[Player_imgFall]->selected = true;
		nextDoor = 255;
	}
	
	if (pc->img[Player_imgFall]->animation)
	{
		iPoint p = pc->playerPosition + SET_DRAW_OFF - iPointMake(HALF_OF_TEX_WIDTH / 2, HALF_OF_TEX_HEIGHT / 2);
		pc->img[Player_imgFall]->paint(dt, p);
		return;
	}
	
	static bool stage = false;
	if (stage == false)
	{
		createStage();
		showRgLoading(true, NextStage);

		stage = true;
	}

	drawRgLoading(dt, NextStage);

	if (bShowRgLoading(NextStage) == false)
	{
		nextStage = false;

		stage = false;
	}
}

void drawNextDoor(float dt)
{
	if (nextStage || (nextDoor != pc->tileNumber))
		return;

	iPoint p = maps[nextDoor]->tileOff + SET_DRAW_OFF + RGTILE_CENTER;

	setRGBA(0, 0.5, 1, 1);
	fillRect(p.x-25, p.y-25, 50, 50); // 문 디자인
	setRGBA(1, 1, 1, 1);

	containDoor(pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT));
}

void containDoor(iPoint p)
{
	iPoint dp = maps[nextDoor]->tileOff + RGTILE_CENTER;
	iRect drt = iRectMake(dp.x - 25, dp.y - 25, 50, 50);

	if (containPoint(p, drt))
	{
		nextStage = true;
		audioPlay(SND_JUMP);
	}
}

