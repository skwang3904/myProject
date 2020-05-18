#include "Stage.h"

#include "Room.h"

#include "EnemyComposition.h"

int stage = 0;
int nextDoor = -1;
bool nextStage = false;

void createStage(int stage)
{
	newRoomTile();

	// monster init
	for (int i = 0; i < ENEMY_NUM; i++)
		enemys[i]->init(stage);

	int pcTile = pc->initPlayerPosition();

	setEnemyPosition(pcTile);

	setNextDoor(pcTile);
}

void setNextDoor(int pcTile)
{
	int i, j, num = 0;
	int activeTile[MAPTILE_NUM];
	int check[MAPTILE_NUM];
	for (i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile == Tile1way1 || maps[i]->rgTile == Tile1way2 || 
			maps[i]->rgTile == Tile1way3 || maps[i]->rgTile == Tile1way4)
		{
			activeTile[num] = i;
			num++;
		}
	}

	for (i = 0; ; i++)
	{
		if (maps[activeTile[random()%num]]->tileOff + pc->camPosition != iPointZero && 
			activeTile[random() % num] != pcTile)
		{
			nextDoor = activeTile[i];
			break;
		}
	}
}

void drawNextDoor(float dt)
{
	iPoint p = pc->camPosition + maps[nextDoor]->tileOff + setPos + RGTILE_CENTER;

	setRGBA(0, 0.5, 1, 1);
	fillRect(p.x, p.y, 50, 50);
	setRGBA(1, 1, 1, 1);

	containDoor(pc->touchPlayer);
}

void containDoor(iRect rt)
{
	if (maps[nextDoor]->tileOff + pc->camPosition != iPointZero)
		return;

	iPoint p = pc->camPosition + maps[nextDoor]->tileOff + setPos + RGTILE_CENTER;
	iRect drt = iRectMake(p.x - 25, p.y - 25, 50, 50);

	if (containRect(drt, rt))
	{
		nextStage = true;
		audioPlay(SND_JUMP);
	}
}

