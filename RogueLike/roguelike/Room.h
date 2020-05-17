#pragma once

#include "iStd.h"

/*
타일 상호작용
*/

void loadRoomTile();
void freeRoomTile();
void drawRoomTile(float dt);

extern bool passAni;
#define _passAniDt 0.2f
void passTileAnimation(float dt);

struct MapTile;
extern MapTile** maps;
void wallCheck(bool checkFall, MapTile* tile, iPoint& pos,iPoint mp, 
				float halfOfTexW, float halfOfTexH);
bool fallCheck(MapTile* tile, float dt);
