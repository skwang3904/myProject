#include "Stage.h"

#include "Room.h"

#include "Enemy.h"
#include "EnemyStruct.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"
#include "RgLoading.h"

#include "RgProc.h"

uint8 stage = 0;
uint8 nextDoor = -1;
bool nextStage = false;

void createStage(uint8 stage)
{
	newRoomTile();

	// monster init
	for (int i = 0; i < GOLEM_NUM; i++)
		golems[i]->init(stage);

	uint8 pcTile = pc->initPlayerPosition();

	setEnemyPosition(pcTile);
	setNextDoor(pcTile);
}

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
	static bool stagetest = false;
	static bool stageAni = false;
	if (stageAni == false)
	{
		pc->img[8]->startAnimation();
		stageAni = true;
	}
	
	if (pc->img[8]->animation == true)
	{
		iPoint p = pc->drawPos - iPointMake(HALF_OF_TEX_WIDTH / 2, HALF_OF_TEX_HEIGHT / 2);
		pc->img[8]->paint(dt,p,REVERSE_NONE);
		return;
	}
	
	if (stagetest == false)
	{
		stage++;
		createStage(stage);

		showRgLoading(true, NextStage);
		stagetest = true;
	}

	drawRgLoading(dt, NextStage);

	if (bShowRgLoading(NextStage) == false)
	{
		nextStage = false;
		stagetest = false;
		stageAni = false;
	}

}

void drawNextDoor(float dt)
{
	if (nextStage)
	{
		nextStageAni(dt);
		return;
	}

	iPoint p = pc->camPosition + maps[nextDoor]->tileOff + setPos + RGTILE_CENTER;

	setRGBA(0, 0.5, 1, 1);
	fillRect(p.x-25, p.y-25, 50, 50); // 문 디자인
	setRGBA(1, 1, 1, 1);

	containDoor(pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT));
}

void containDoor(iPoint p)
{
	if (maps[nextDoor]->tileOff + pc->camPosition != iPointZero)
		return;

	iPoint dp = maps[nextDoor]->tileOff + RGTILE_CENTER;
	iRect drt = iRectMake(dp.x - 25, dp.y - 25, 50, 50);

	if (containPoint(p, drt))
	{
		nextStage = true;
		audioPlay(SND_JUMP);
	}
}

