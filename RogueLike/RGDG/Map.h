#pragma once

#include "iStd.h"

extern iPoint displayCenterPos;

void loadMap();
void freeMap();
void drawMap(float dt);

enum MapType {
	MapType_Nomal = 0,
	MapType_Boss,
	MapType_Treasure,
	MapType_Shop,
};

struct MapTile
{
	MapType state;
	iImage* img;

	int8* tile; // 타일 설정
	iPoint tileOff;
};
extern MapTile** maps;