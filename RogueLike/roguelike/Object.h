#pragma once

#include "iStd.h"

class Object
{
public:
	Object();
	virtual ~Object();

public:
	iImage* img;
	iPoint position;
	int index;
	int mapNumber;
};



enum MapObjType {
	Map_Obj_Nomal = 0,
	Map_Obj_Door,
	Map_Obj_Broke,
};

struct MapObject2
{
	iImage* objImg; // 오브젝트 애니메이션
	iPoint objPos;

	int objTileNum; //오브젝트가 사용하는 타일숫자
	int* objTile; // 오브젝트가 사용하는 타일

	MapObjType type;
	iRect hitBox;
};

class MapObject : public Object
{
public:
	MapObject();
	virtual ~MapObject();

public:

};
