#pragma once

#include "iStd.h"

/*
타일 상호작용
*/

void loadRoomTile();
void freeRoomTile();
void drawRoomTile(float dt);

struct MapTile;
void wallCheck(bool checkFall, MapTile* tile, iPoint& pos,iPoint mp, 
				float halfOfTexW, float halfOfTexH);
bool fallCheck(Player* pc, MapTile* tile, float dt);
