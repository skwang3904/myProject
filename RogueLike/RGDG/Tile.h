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

extern int8 Tile0Way1[TILE_NUM_X * TILE_NUM_Y]; // 0way
extern int8 Tile4Way1[TILE_NUM_X * TILE_NUM_Y]; // 4way
	   		  
extern int8 Tile3Way1[TILE_NUM_X * TILE_NUM_Y]; // R UD
extern int8 Tile3Way2[TILE_NUM_X * TILE_NUM_Y]; // L UD
extern int8 Tile3Way3[TILE_NUM_X * TILE_NUM_Y]; // LR D
extern int8 Tile3Way4[TILE_NUM_X * TILE_NUM_Y]; // LR U
	   		 
extern int8 Tile2Way1[TILE_NUM_X * TILE_NUM_Y]; // LR
extern int8 Tile2Way2[TILE_NUM_X * TILE_NUM_Y]; // UD
extern int8 Tile2Way3[TILE_NUM_X * TILE_NUM_Y]; // LU
extern int8 Tile2Way4[TILE_NUM_X * TILE_NUM_Y]; // RU
extern int8 Tile2Way5[TILE_NUM_X * TILE_NUM_Y]; // LD
extern int8 Tile2Way6[TILE_NUM_X * TILE_NUM_Y]; // RD
	   		  
extern int8 Tile1Way1[TILE_NUM_X * TILE_NUM_Y]; // L
extern int8 Tile1Way2[TILE_NUM_X * TILE_NUM_Y]; // R
extern int8 Tile1Way3[TILE_NUM_X * TILE_NUM_Y]; // U
extern int8 Tile1Way4[TILE_NUM_X * TILE_NUM_Y]; // D