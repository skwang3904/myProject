#pragma once

#include "iStd.h"

/*
타일 상호작용
*/




void loadRoomTile();
void drawRoomTile(float dt);

void wallCheck(Player* pc, iPoint mp);
bool fallCheck(Player* pc, float dt);