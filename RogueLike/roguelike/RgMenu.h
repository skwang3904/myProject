#pragma once

// 인트로 후 메뉴
// 캐릭터 선택, 옵션 게임종료 등등

#include "iStd.h"

void loadRgMenu();
void freeRgMenu();
void drawRgMenu(float dt);
void keyRgMenu(iKeyState stat, iPoint point);

//-------------------------------------------------------------------------------------

void createPopMenuBG();
void freePopMenuBG();
void showPopMenuBG(bool show);
void drawPopMenuBG(float dt);
void keyPopMenuBG(iKeyState stat, iPoint point);

//-------------------------------------------------------------------------------------
