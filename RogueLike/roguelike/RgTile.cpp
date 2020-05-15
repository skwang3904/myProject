#include "RgTile.h"

iPoint* tileOffSet;
MapTile** mapTiles;
iPoint setPos;
void createTileSet()
{
	setPos = SETPOS_POSITION;

	tileOffSet = (iPoint*)malloc(sizeof(iPoint) * TILEOFF_NUM);
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		tileOffSet[i] = iPointMake(RGTILE_X * RGTILE_Width * (i % TILEOFF_SQRT) ,
			RGTILE_Y * RGTILE_Height * (i / TILEOFF_SQRT));
	}
}

void freeTileSet()
{
	free(tileOffSet);
}

int Tile0way1[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile4way1[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, FF, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, FF, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};

int Tile3way1[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};

int Tile3way2[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, FF, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, FF, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, FF, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, FF, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};

int Tile3way3[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, FF, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};

int Tile3way4[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, FF, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, FF, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile2way1[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile2way2[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};

int Tile2way3[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile2way4[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile2way5[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};

int Tile2way6[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};


int Tile1way1[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile1way2[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, 01,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile1way3[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
};

int Tile1way4[RGTILE_X * RGTILE_Y] = {
	WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, WW, WW, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, WW, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, 01, WW,
	WW, WW, WW, WW, WW, WW, 01, 01, 01, 01, WW, WW, WW, WW, WW, WW,
};


