#pragma once

#include "iStd.h"

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyState stat, iPoint point);

/*
게임 시작전 캐릭터 선택화면

캐릭터 아이콘
캐릭터 이미지
캐릭터 설명 -> 팝업으로?

버튼)
-게임시작
-옵션
-돌아가기

*/