#pragma once

#include "iStd.h"

#include "Common.h"
extern iPoint displayCenterPos;
#define DRAW_OFF displayCenterPos + player->camera

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

void wallCheck(Object* obj, iPoint mp);