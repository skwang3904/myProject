#pragma once

#include "iStd.h"


typedef void (*Loading_Method)();
void setLoading(int gamestat, Loading_Method free, Loading_Method load);
void drawLoading(float dt);
bool keyLoading(iKeyState stat, iPoint point);