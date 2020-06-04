#pragma once

// 인트로화면 구성

#include "iStd.h"


enum loadingType {
	PlayerDead = 0,
	NextStage,
};

void loadRgLoading();
void freeRgLoading();
void showRgLoading(bool show, loadingType type);
bool bShowRgLoading(loadingType type);
void drawRgLoading(float dt, loadingType type);
void keyRgLoading(iKeyState stat, iPoint point);

//-------------------------------------------------------------------

void createPopPlayerDead();
void freePopPlayerDead();
void showPopPlayerDead(bool show);
void drawPopPlayerDead(float dt);
bool keyPopPlayerDead(iKeyState stat, iPoint point);

//-------------------------------------------------------------------

void createPopNextStage();
void freePopNextStage();
void showPopNextStage(bool show);
void drawPopNextStage(float dt);
bool keyPopNextStage(iKeyState stat, iPoint point);