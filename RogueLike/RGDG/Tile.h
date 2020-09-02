#pragma once

#include "iStd.h"

#define TILE_NUM_X	32
#define TILE_NUM_Y	24
#define TILE_Width	30
#define TILE_Height	30
#define TILE_CENTER iPointMake( TILE_NUM_X * TILE_Width / 2, \
								  TILE_NUM_Y * TILE_Height / 2)

#define TILE_CONNECT_NUM	9
#define TILE_TOTAL_NUM		9 // 제곱수
#define TILE_TOTAL_SQRT		3

#define MOVETILE	1
#define WALLTILE	2
#define FALLTILE	3
#define DOORTILE	4

#define WW WALLTILE
#define FF FALLTILE
#define DR DOORTILE

#define MOVETILE_RGBA	1, 1, 1, 1
#define WALLTILE_RGBA	0.5, 0.5, 0.5, 1
#define FALLTILE_RGBA	0, 0, 0.3, 1

struct TileWay
{
	int8* tile;
	int** doorNum; // doorNum[4][4]
};
extern TileWay* tileWay;

void loadTile();
void freeTile();

/*
     tileWay
     0 : 0way
     1 : 4way
     
     2 : R UD
     3 : L UD
     4 : LR D
     5 : LR U
     
     6 : LR
     7 : UD
     8 : LU
     9 : RU
    10 : LD
    11 : RD
    
    12 : L
    13 : R
    14 : U
    15 : D
*/