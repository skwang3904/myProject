#pragma once

#include "iStd.h"

/*
타일 상호작용
*/




void loadRoomTile();
void drawRoomTile(float dt);

//void wallCheck(Player* pc, Enemy1* enm ,iPoint mp);
void wallCheck(bool checkFall, iPoint& pos,iPoint mp, float halfOfTexW, float halfOfTexH);
bool fallCheck(Player* pc, float dt);