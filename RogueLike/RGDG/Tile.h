#pragma once

#include "iStd.h"

#define TILE_NUM_X	32
#define TILE_NUM_Y	32
#define TILE_Width	24
#define TILE_Height	24
#define TILE_CENTER iPointMake( TILE_NUM_X * TILE_Width / 2, \
								  TILE_NUM_Y * TILE_Height / 2)

#define TILE_CONNECT_NUM	10
#define TILE_TOTAL_NUM		25 // 제곱수
#define TILE_TOTAL_SQRT		5

#define TILE_MOVE       1
#define TILE_WALL       2
#define TILE_FALL       3
#define TILE_DOOR       4
#define TILE_BARREL     5
#define TILE_TRAPDOOR   6
#define TILE_ITEMBOX    7

#define WW TILE_WALL
#define FF TILE_FALL
#define DR TILE_DOOR
#define BA TILE_BARREL
#define TD TILE_TRAPDOOR
#define IB TILE_ITEMBOX

#define MOVETILE_RGBA	1, 1, 1, 1
#define WALLTILE_RGBA	0.5, 0.5, 0.5, 1
#define FALLTILE_RGBA	0, 0, 0.3, 1

extern int8* tileWay[16];

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