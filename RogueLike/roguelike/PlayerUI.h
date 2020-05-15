#pragma once

#include "iStd.h"

void loadPlayerUI();
void freePlayerUI();
void drawPlayerUI(float dt);
void keyPlayerUI(iKeyState stat, iPoint point);

/////////////////////////////////////////////////////////

void createPopUI();
void freePopUI();
void showPopUI(bool show);
void drawPopUI(float dt);
bool keyPopUI(iKeyState stat, iPoint point);