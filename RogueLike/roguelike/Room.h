#pragma once

#include "iStd.h"

/*
타일 상호작용
*/

void loadRoomTile();
void setRoomTile();
void freeRoomTile();
void drawRoomTile(float dt);

struct ConnectTile {
	int index; // 타일 넘버
	bool value; //
	bool visit;
	iPoint tileOff;
};
extern ConnectTile ct[TILEOFF_NUM];

struct MapTile;
extern MapTile** maps;

void passTileAnimation(float dt);
bool passAni();

void wallCheck(bool checkFall, MapTile* tile, iPoint& pos,iPoint mp, 
				float halfOfTexW, float halfOfTexH);
bool fallCheck(MapTile* tile, float dt);
