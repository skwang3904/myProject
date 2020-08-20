#pragma once

#include "iStd.h"

extern int8 gamestat;
#define gamestat_intro	0
#define gamestat_menu	1
#define gamestat_proc	2
#define gamestat_else	3


void loadGame();
void freeGame();
void drawGame(float dt);
void keyGame(iKeyState stat, iPoint point);

