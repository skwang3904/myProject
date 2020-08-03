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

enum MapObjType {
	MapObj_Nomal = 0,
	MapObj_Broke,
};

struct MapObject
{
	Texture* objTex = NULL; // 오브젝트 애니메이션
	iPoint objPos;
	MapObjType type;
};

struct MapTile 
{
	MapState state;
	int* rgTile; // 타일 설정
	Texture* tileTex; // 맵 애니메이션용

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