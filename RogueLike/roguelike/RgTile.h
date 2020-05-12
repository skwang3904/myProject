#pragma once

// 타일 모음

#include "iStd.h"

// 타일 정보--------
#define RGTILE_X		16
#define RGTILE_Y		12
#define RGTILE_Width	40
#define RGTILE_Height	40

// 타일 기믹--------
#define MOVETILE	1
#define WALLTILE	2
#define FALLTILE	3

#define WW WALLTILE
#define FF FALLTILE

#define MOVETILE_RGBA	1, 1, 1, 1
#define WALLTILE_RGBA	0.5, 1, 0, 1
#define FALLTILE_RGBA	0, 0, 0.5, 1

#define RGTILE1X	RGTILE_X * RGTILE_Width

#define RGTILE2Y	RGTILE_Y * RGTILE_Height

#define RGTILE3X	RGTILE_X * RGTILE_Width
#define RGTILE3Y	RGTILE_Y * RGTILE_Height

extern int** allTile;
extern int RgTile[RGTILE_X * RGTILE_Y];
extern int RgRoomTile1[RGTILE_X * RGTILE_Y];
extern int RgRoomTile2[RGTILE_X * RGTILE_Y];
extern int RgRoomTile3[RGTILE_X * RGTILE_Y];

void createTileSet();
void freeTileSet();