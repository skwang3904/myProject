#include "Proc.h"

#include "Map.h"
#include "PlayerChar.h"

void loadProc()
{
	loadMap();
	loadPlayerChar();
}

void freeProc()
{
	freeMap();
	freePlayerChar();
}

void drawProc(float dt)
{
	drawMap(dt);
	drawPlayerChar(dt);
}

void keyProc(iKeyState stat, iPoint point)
{
}
