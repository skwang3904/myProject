#pragma once

#include "iType.h"
#include "RgTile.h"

/*
타일 상호작용
*/

void loadRoomTile();
void setRoomTile();
void freeRoomTile();
void drawRoomTile(float dt);

enum MapState {
	MapState_Nomal = 0,
	MapState_Boss,
	MapState_Treasure,
	MapState_Shop,
};

struct MapTile {
	int* rgTile;
	Texture* tileTex;
	iPoint tileOff;
	
	MapState state;
};
extern MapTile** maps;

struct ConnectTile {
	int index; // 타일 넘버
	bool value; //
	bool visit;
	iPoint tileOff;
};
extern ConnectTile ct[TILEOFF_NUM];

void passTileAnimation(float dt);
bool passAni();

void wallCheck(bool checkFall, MapTile* tile, iPoint& pos,iPoint mp, 
				float halfOfTexW, float halfOfTexH);