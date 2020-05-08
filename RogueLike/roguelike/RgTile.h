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
#define WALLTILE_RGBA	1, 1, 0, 1
#define FALLTILE_RGBA	0, 0, 0.5, 1

extern int RgTile[RGTILE_X * RGTILE_Y];