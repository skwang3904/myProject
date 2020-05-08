#pragma once

#include "iStd.h"

// 실행되는 코드 모음

#define gs_intro	1
#define gs_menu		2
#define gs_proc		3



void loadRgGame();
void freeRgGame();
void drawRgGame(float dt);
void keyRgGame(iKeyState stat, iPoint point);