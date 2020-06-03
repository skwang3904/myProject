#pragma once

#include "iStd.h"

#define gs_intro	1
#define gs_menu		2
#define gs_proc		3

extern int gamestat;

typedef void (*Loading_Method)();
void setLoading(int gamestat, Loading_Method free, Loading_Method load);
bool getLoading();
void drawLoading(float dt);
bool keyLoading(iKeyState stat, iPoint point);

