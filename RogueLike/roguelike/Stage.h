#pragma once

#include "iStd.h"

/*
 방 연결배치, 아이템 생성
*/

/*
1. 스테이지마다 타일 재생성 및 배치,
2. 플레이어 데이터 고정
3. 몬스터 초기화 (스테이지마다 능력치 조정되게)
*/

extern int stage;
extern bool nextStage;

void createStage(int stage);
void setNextDoor(int pcTile);

void drawNextDoor(float dt);
void containDoor(iRect rt);