#pragma once

struct TileInfo
{
	int value;
	bool visit;
	int path[1024];
	int pathNum;
};

class iShortestPath
{
public:
	iShortestPath();
	virtual ~iShortestPath();

	void init(int* tiles, int tileX, int tileY);
	void dijkstra(int start, int end, int* path, int& pathNum);
	void removeDuplicate(int* path, int& pathNum);
public:

	int* tiles;
	int tileX, tileY, tileXY;

	TileInfo* ti;
	int tiNum;
};

#define sp_inf 99


extern int tiles[70];
#define tile_num_w 10
#define tile_num_h 7
#define tile_size_w 60
#define tile_size_h 60

#include "iType.h"
void initShortestPath();
void freeShortestPath();
void drawShortestPath(float dt);
void keyShortestPath(iKeyState stat, iPoint point);
