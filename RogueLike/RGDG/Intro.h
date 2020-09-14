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

