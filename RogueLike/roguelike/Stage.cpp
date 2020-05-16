#include "Stage.h"

#include "Room.h"
#include "RgTile.h"

int stage = 0;

void createStage(int stage)
{
	loadRoomTile();

	pc->hp = pc->_hp;
	for (int i = 0; i < TILEOFF_NUM; i++)
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
}

void freePrevStage()
{
	freeRoomTile();
	freeTileSet();
}
