#pragma once

// 타일 정보 모음

#include "iStd.h"

// 타일 정보--------
#define RGTILE_X		32
#define RGTILE_Y		24
#define RGTILE_Width	30
#define RGTILE_Height	30
#define RGTILE_CENTER iPointMake( RGTILE_X * RGTILE_Width / 2, \
								  RGTILE_Y * RGTILE_Height / 2)

// 타일 기믹--------
#define MOVETILE	1
#define WALLTILE	2
#define FALLTILE	3

#define WW WALLTILE
#define FF FALLTILE

#define MOVETILE_RGBA	1, 1, 1, 1
#define WALLTILE_RGBA	0.5, 0.5, 0.5, 1
#define FALLTILE_RGBA	0, 0, 0.3, 1

#define MAPTILE_NUM 10
#define TILEOFF_NUM 25 // 제곱수
#define TILEOFF_SQRT 5

struct MapTile {
	int* rgTile;
	iPoint tileOff;
};

extern iPoint* tileOffSet;

void createTileSet();
void freeTileSet();

extern int Tile0way1[RGTILE_X * RGTILE_Y]; // 0way
extern int Tile4way1[RGTILE_X * RGTILE_Y]; // 4way

extern int Tile3way1[RGTILE_X * RGTILE_Y]; // R UD
extern int Tile3way2[RGTILE_X * RGTILE_Y]; // L UD
extern int Tile3way3[RGTILE_X * RGTILE_Y]; // LR D
extern int Tile3way4[RGTILE_X * RGTILE_Y]; // LR U

extern int Tile2way1[RGTILE_X * RGTILE_Y]; // LR
extern int Tile2way2[RGTILE_X * RGTILE_Y]; // UD
extern int Tile2way3[RGTILE_X * RGTILE_Y]; // LU
extern int Tile2way4[RGTILE_X * RGTILE_Y]; // RU
extern int Tile2way5[RGTILE_X * RGTILE_Y]; // LD
extern int Tile2way6[RGTILE_X * RGTILE_Y]; // RD

extern int Tile1way1[RGTILE_X * RGTILE_Y]; // L
extern int Tile1way2[RGTILE_X * RGTILE_Y]; // R
extern int Tile1way3[RGTILE_X * RGTILE_Y]; // U
extern int Tile1way4[RGTILE_X * RGTILE_Y]; // D