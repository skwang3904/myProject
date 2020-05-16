#pragma once

#include "iStd.h"

void loadPlayerUI();
void freePlayerUI();
void drawPlayerUI(float dt);
bool keyPlayerUI(iKeyState stat, iPoint point);

/////////////////////////////////////////////////////////

extern iPopup* popHP;

void createPopHP();
void freePopHP();
void showPopHP(bool show);
void drawPopHP(float dt);
bool keyPopHP(iKeyState stat, iPoint point);

/////////////////////////////////////////////////////////

void createPopCombat();
void freePopCombat();
void showPopCombat(bool show);
void drawPopCombat(float dt);
bool keyPopCombat(iKeyState stat, iPoint point);