#pragma once

#include "iStd.h"

void loadProc();
void freeProc();
void drawProc(float dt);
void keyProc(iKeyState stat, iPoint point);

#if 0
0. 세이브 방법
- 매 스테이지 시작 시 저장
- 맵 정보 : maps 데이터
공통 저장변수 : index, imgNum, mapNumber, position, vector, touchRect
- 플레이어캐릭터 : imgNum, imgs, camera, status
- 몬스터 : monsterNum, golemNomalNum, _golemNomal, ...
- state, status

1. 맵생성 : 타일 > 방
- 맵 오브젝트->타일에 속함->공격 or 상호작용시 동작
함정요소(밟으면 데미지 입는 발판)
- 방 랜덤구성
- 맵오브젝트, 캐릭터, 몬스터 sorting
2. 캐릭터 조작 : 움직임, 점프 ... 무기들고있을때 공격
3. 몬스터 : 패턴 ... 간단한 이동만 가능
4. 무기 :
	5. 아이템 :
#endif

//-----------------------------------------------------------
// UI
void createPopState();
void freePopState();
void showPopState(bool show);
void drawPopState(float dt);
bool keyPopState(iKeyState stat, iPoint point);