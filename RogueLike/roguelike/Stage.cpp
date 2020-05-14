#include "Stage.h"

#include "Room.h"
#include "RgTile.h"

int stage = 0;

void createStage(int stage)
{
	createTileSet();
	loadRoomTile();
	pc->playerPosition = mapTiles[0]->tileOff +
		iPointMake(RGTILE_X * RGTILE_Width / 2, RGTILE_Y * RGTILE_Height / 2);
	// monster init
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemys[i]->init(stage);
		enemys[i]->Enemy1Position = mapTiles[1]->tileOff + iPointMake(200 + 40 * i, 80 + 20 * i);
		enemys[i]->drawEnemyPos = enemys[i]->Enemy1Position + pc->camPosition + setPos;
	}
}

void freePrevStage()
{
	freeRoomTile();
	freeTileSet();
}
