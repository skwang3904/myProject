#include "Stage.h"

#include "Room.h"
#include "RgTile.h"

#include "EnemyComposition.h"

int stage = 0;
int nextDoor = -1;
bool nextStage = false;
void createStage(int stage)
{
	loadRoomTile();

	pc->hp = pc->_hp;
	for (int i = TILEOFF_NUM/2 + 3; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile)
		{
			pc->playerPosition = maps[i]->tileOff +
			iPointMake(RGTILE_X * RGTILE_Width / 2, RGTILE_Y * RGTILE_Height / 2);
			pc->camPosition = iPointZero - maps[i]->tileOff;
			break;
		}
	}

	// monster init
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemys[i]->init(stage);
		//enemys[i]->EnemyPosition = iPointMake(200 + 40 * i, 80 + 20 * i) - pc->camPosition;
		//enemys[i]->drawEnemyPos = enemys[i]->EnemyPosition + pc->camPosition + setPos;
	}

	setEnemyPosition();

	int i, j, num = 0;
	int activeTile[MAPTILE_NUM];
	int check[MAPTILE_NUM];
	for (i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile != NULL)
		{
			activeTile[num] = i;
			num++;
		}
	}

	for (i = 0; i < MAPTILE_NUM; i++)
	{
		if (maps[activeTile[i]]->tileOff + pc->camPosition != iPointZero)
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
	
}

void containDoor(iRect rt)
{
	if (maps[nextDoor]->tileOff + pc->camPosition != iPointZero)
		return;

	iPoint p = pc->camPosition + maps[nextDoor]->tileOff + setPos + RGTILE_CENTER;
	iRect drt = iRectMake(p.x - 25, p.y - 25, 50, 50);

	if (containRect(drt, rt))
		nextStage = true;
}

