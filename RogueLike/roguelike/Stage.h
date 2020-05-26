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

extern uint8 stage;
extern uint8 nextDoor;
extern bool nextStage;

void createStage(uint8 stage);
void setNextDoor(uint8 pcTile);

void drawNextDoor(float dt);
void containDoor(iPoint p);

void nextStageAni(float dt);