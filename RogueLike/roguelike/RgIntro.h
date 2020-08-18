#pragma once

// 인트로화면 구성

#include "iStd.h"

void loadRgIntro();
void freeRgIntro();
void drawRgIntro(float dt);
void keyRgIntro(iKeyState stat, iPoint point);

//-------------------------------------------------------------------------------------

void createPopIntroBG();
void freePopIntroBG();
void showPopIntroBG(bool show);
void drawPopIntroBG(float dt);
bool keyPopIntroBG(iKeyState stat, iPoint point);

//-------------------------------------------------------------------------------------

void createPopIntroButten();
void freePopIntroButten();
void showPopIntroButten(bool show);
void drawPopIntroButten(float dt);
bool keyPopIntroButten(iKeyState stat, iPoint point);

//-------------------------------------------------------------------------------------