#pragma once

#include "iStd.h"

void loadIntro();
void freeIntro();
void drawIntro(float dt);
void keyIntro(iKeyState stat, iPoint point);

/*
맨처음 시작시 나올 화면

게임제목
간단한 제작자 소개글

버튼)
-게임시작 
->새로시작
->이어하기

-옵션 -> 기본 사운드설정,
-크레딧
-종료
*/

void loadIntroShader();
void freeIntroShader();
void drawIntroShader(float dt);

//------------------------------------------------------------------------
// PressAnyKey
void createPopPressAnyKey();
void freePopPressAnyKey();
void showPopPressAnyKey(bool show);
void drawPopPressAnyKey(float dt);
bool keyPopPressAnyKey(iKeyState stat, iPoint point);

//------------------------------------------------------------------------
// IntroButton
void createPopIntroButton();
void freePopIntroButton();
void showPopIntroButton(bool show);
void drawPopIntroButton(float dt);
bool keyPopIntroButton(iKeyState stat, iPoint point);

//------------------------------------------------------------------------
// SelectStart
void createPopIntroSelectStart();
void freePopIntroSelectStart();
void showPopIntroSelectStart(bool show);
void drawPopIntroSelectStart(float dt);
bool keyPopIntroSelectStart(iKeyState stat, iPoint point);