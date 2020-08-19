#pragma once

#include "iStd.h"




//---------------------------------------------------------------------
// Option

void createPopOption();
void freePopOption();
void showPopOption(bool show);
void drawPopOption(float dt);
bool keyPopOption(iKeyState stat, iPoint point);