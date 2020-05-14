#pragma once

// 타일 모음

#include "iStd.h"

// 타일 정보--------
#define RGTILE_X		16
#define RGTILE_Y		12
#define RGTILE_Width	20
#define RGTILE_Height	20

// 타일 기믹--------
#define MOVETILE	1
#define WALLTILE	2
#define FALLTILE	3

#define WW WALLTILE
#define FF FALLTILE

#define MOVETILE_RGBA	1, 1, 1, 1
#define WALLTILE_RGBA	0.5, 1, 0, 1
#define FALLTILE_RGBA	0, 0, 0.5, 1

#define MAPTILE_NUM 15
#define TILEOFF_NUM 16 // 제곱수
#define TILEOFF_SQRT 4

struct MapTile {
	int* rgTile;
	iPoint tileOff;
};
extern iPoint* tileOffSet;
extern MapTile** mapTiles;
extern iPoint setPos;

void createTileSet();
void freeTileSet();

extern int Tile4way1[RGTILE_X * RGTILE_Y]; //4way

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