#pragma once

#include "iStd.h"

#include "Common.h"

extern iPoint displayCenterPos;
#define DRAW_OFF displayCenterPos + player->camera

struct MapTile
{
	MapType state;
	iImage* img;

	int tileIndex;
	int8* tile; // 타일 설정
	iPoint tileOff;
};
extern MapTile** maps;
int wallCheck(Object* obj, iPoint mp);

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
	MapObject(int index, int8 mapNum, iPoint pos);
	virtual ~MapObject();

	virtual void paint(float dt, iPoint off) = 0;
	virtual void drawShadow(float dt, iPoint off) = 0;
	virtual void action(Object* obj) = 0;

public:
	//type = index로 구분
	int value;

	int tileNumX;
	int tileNumY;
	int* tileNumber;
};
extern MapObject** _mapObj;
extern MapObject** mapObj;
extern int mapObjNum;
extern MapObject** mapObjBroken;
extern int mapObjBrokenNum;
//-----------------------------------------------------------------------------

class MapObjectDoor : public MapObject
{
public:
	MapObjectDoor(int index, int8 mapNum, iPoint pos, int tileNumber);
	virtual ~MapObjectDoor();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

public:

};

//-----------------------------------------------------------------------------

class MapObjectNextDoor : public MapObject
{
public:
	MapObjectNextDoor(int index, int8 mapNum, iPoint pos, int tileNumber);
	virtual ~MapObjectNextDoor();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

	static void setNextDoor(Object* obj);
public:

};

//-----------------------------------------------------------------------------

class MapObjectBarrel : public MapObject
{
public:
	MapObjectBarrel(int index, int8 mapNum, iPoint pos, int tileNumber);
	virtual ~MapObjectBarrel();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

public:

};

//-----------------------------------------------------------------------------

class MapObjectItemBox : public MapObject
{
public:
	MapObjectItemBox(int index, int8 mapNum, iPoint pos, int tileNumber);
	virtual ~MapObjectItemBox();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

public:

};


void loadMap();
void freeMap();

void createMap(bool readFile = false); //true : read, false : newCreate
void createMapImage();
void deleteMapImage();

void drawMap(float dt);