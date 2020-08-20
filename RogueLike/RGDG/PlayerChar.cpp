#include "PlayerChar.h"

#include "Tile.h"
#include "Map.h"

PlayerChar* player;
PlayerChar::PlayerChar(int index) : Object(index)
{
	int i, j;
	imgs = (iImage**)malloc(sizeof(iImage*) * PLAYER_IMG_NUM);

	initState();
}

PlayerChar::~PlayerChar()
{
	for (int i = 0; i < PLAYER_IMG_NUM; i++)
		delete imgs[i];
	free(imgs);
}

void PlayerChar::paint(float dt, iPoint off)
{
	
}

void PlayerChar::initState()
{
	hp = _hp = 100.0f;
	attackPoint = _attackPoint = 10.0f;
	attackSpeed = _attackSpeed = 1.0f;
	moveSpeed = 300.0f;


	for (int i = 0; i < TILE_TOTAL_NUM; i++)
	{
		int a = random() % TILE_TOTAL_NUM;
		if (maps[a]->tile[0] != 0 && maps[a]->state == MapType_Nomal)
		{
			position = maps[a]->tileOff + TILE_CENTER;
			camera = displayCenterPos - maps[a]->tileOff;
			mapNumber = a;
			break;
		}
	}

}

//-------------------------------------------------------------------------------------

void loadPlayerChar()
{
}

void freePlayerChar()
{
}

void drawPlayerChar(float dt)
{
	player->paint(dt, player->camera);
}
