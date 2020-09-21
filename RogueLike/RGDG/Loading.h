#pragma once

#include "iStd.h"

#define gamestat_intro	0
#define gamestat_menu	1
#define gamestat_proc	2
#define gamestat_else	3

extern int8 gamestat;
typedef void (*METHOD_LOADING)();
void setLoading(int8 newGameState, METHOD_LOADING free, METHOD_LOADING load);

void loadGameLoading();
void freeGameLoading();
void drawLoading(float dt);
bool nowLoading();
bool keyLoading(iKeyState stat, iPoint point);

//------------------------------------------------------------------------------------------
extern iPopup* popGameLoading;
void createPopGameLoading();
void freePopGameLoading();
void showPopGameLoading(bool show);
void drawPopGameLoading(float dt);
