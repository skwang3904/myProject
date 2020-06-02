#pragma once

#include "iStd.h"

/*
타일 상호작용
*/

void loadRoomTile();
void newRoomTile();
void freeRoomTile();
void drawRoomTile(float dt);

struct ConnectTile {
	int index; // 타일 넘버
	bool value; //
	bool visit;
	iPoint tileOff;
};
extern ConnectTile ct[TILEOFF_NUM];

extern bool passAni;
#define _passAniDt 0.2f
void passTileAnimation(float dt);

struct MapTile;
extern MapTile** maps;
void wallCheck(bool checkFall, MapTile* tile, iPoint& pos,iPoint mp, 
				float halfOfTexW, float halfOfTexH);
bool fallCheck(MapTile* tile, float dt);
