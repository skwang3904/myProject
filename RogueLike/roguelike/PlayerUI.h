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

void createPopMiniMap();
void freePopMiniMap();
void showPopMiniMap(bool show);
void drawPopMiniMap(float dt);
bool keyPopMiniMap(iKeyState stat, iPoint point);

/////////////////////////////////////////////////////////

void createPopCombatMenu();
void freePopCombatMenu();
void showPopCombatMenu(bool show);
void drawPopCombatMenu(float dt);
bool keyPopCombatMenu(iKeyState stat, iPoint point);
