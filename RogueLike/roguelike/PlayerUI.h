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

#define CombatMenu_Pos iPointMake(1600, 400)
void createPopCombatMenu();
void freePopCombatMenu();
void showPopCombatMenu(bool show);
void drawPopCombatMenu(float dt);
bool keyPopCombatMenu(iKeyState stat, iPoint point);

/////////////////////////////////////////////////////////

void createPopItem();
void freePopItem();
void showPopItem(bool show);
void drawPopItem(float dt);
bool keyPopItem(iKeyState stat, iPoint point);

/////////////////////////////////////////////////////////

void changeStageNum();
void createPopStageNum();
void freePopStageNum();
void showPopStageNum(bool show);
void drawPopStageNum(float dt);
bool keyPopStageNum(iKeyState stat, iPoint point);