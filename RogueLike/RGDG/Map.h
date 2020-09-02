#pragma once

#include "iStd.h"

#include "Common.h"
extern iPoint displayCenterPos;
#define DRAW_OFF displayCenterPos + player->camera

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

//-----------------------------------------------------------------------------
// MapObject
// 문, 밟으면 피해입는 함정, 부숴지고 아이템나오는 상자
#if 0
index
0 : Door
1 : 
2 :
#endif
class MapObject : public Object
{
public:
	MapObject(int index);
	virtual ~MapObject();

	virtual void paint(float dt, iPoint off) = 0; //

	// 상호작용 함수,
	virtual void action() = 0;
public:
	//type = index로 구분
	int tileNum;
	int* tileNumber;
	int value;
};

//-----------------------------------------------------------------------------

class MapObjectDoor : public MapObject
{
public:
	MapObjectDoor(int index);
	virtual ~MapObjectDoor();

	virtual void paint(float dt, iPoint off);

	virtual void action();
public:

};
//-----------------------------------------------------------------------------

void loadMap();
void freeMap();
void drawMap(float dt);