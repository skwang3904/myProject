#pragma once

// 게임화면

#include "iStd.h"


#define SND_SWING 0
#define SND_JUMP 1
#define SND_FALL 2
#define SND_ENEMY_HIT 3
#define SND_ENEMY_DEAD 4

#define _nextStageloadingTime 3.0f

void loadRgProc();
void freeRgProc();
void drawRgProc(float dt);
void keyRgProc(iKeyState stat, iPoint point);