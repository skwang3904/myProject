#include "Proc.h"

#include "Map.h"

#include "PlayerChar.h"

#include "Monster.h"

void loadProc()
{
	loadMap();
	loadPlayerChar();
	loadMonster();
}

void freeProc()
{
	freeMap();
	freePlayerChar();
	freeMonster();
}

void drawProc(float dt)
{
	drawMap(dt);
	drawPlayerChar(dt);
	drawMonster(dt);
}

void keyProc(iKeyState stat, iPoint point)
{
}
