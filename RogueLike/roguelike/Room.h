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

enum MapType {
	MapType_Nomal = 0,
	MapType_Boss,
	MapType_Treasure,
	MapType_Shop,
};

enum MapObjType {
	MapObj_Nomal = 0,
	MapObj_Door,
	MapObj_Broke,
};

struct MapObject
{
	iImage* objImg = NULL; // 오브젝트 애니메이션
	iPoint objPos;
	int objTileNum;
	int* objTile;
	MapObjType type;
};

struct MapTile 
{
	MapType state;

	int* rgTile; // 타일 설정
	iImage* mapImg; // 맵 애니메이션용
	iPoint tileOff;
	
	int mapObjNum;
	MapObject** mapObj;
};
extern MapTile** maps;

struct ConnectTile
{
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