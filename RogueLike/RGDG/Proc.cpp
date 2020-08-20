#include "Proc.h"

#include "Map.h"
#include "PlayerChar.h"

void loadProc()
{
	loadMap();
}

void freeProc()
{
	freeMap();
}

void drawProc(float dt)
{
	drawMap(dt);
}

void keyProc(iKeyState stat, iPoint point)
{
}
