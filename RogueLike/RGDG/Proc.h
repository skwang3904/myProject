#pragma once

#include "iStd.h"

#include "Common.h"
#include "Tile.h"

#define SORTING 1
extern iSort* procSort;

void loadProc();
void freeProc();
void drawProc(float dt);
void keyProc(iKeyState stat, iPoint point);

/* 4방향 순서 : LRUD (반드시 지킬것)*/
#if 0
0. 세이브 방법
- 매 스테이지 시작 시 저장
- 맵 정보 : maps 데이터
공통 저장변수 : index, imgNum, mapNumber, position, vector, touchRect...
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
6. 보상 상자 :

game ui
화면 하단에 소모item 목록
function key or mouse or number key

게임 종료 조건
-player life = 0
boss kill

#endif

//----------------------------------------------------------------------------
// Stage data

struct MapData
{
	int state;

	int tileIndex;
};

struct PlayerData
{
	int index;
	int8 mapNum;
	iPoint position;

	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float moveSpeed;
};

struct MonsterData
{
	int index;
	int8 mapNum;
	iPoint position;
};

struct WeaponData
{
	int index;
};

#define FILE_PATH "save.sav"
#define END_STAGE 3
struct Stage
{
	int stageNum;

	MapData mapData[TILE_TOTAL_NUM];

	// playerInfo
	PlayerData playerData;

	// monsterInfo
	int actMonsterNum[MT_max];
	MonsterData monsterData[30];

	// weaponInfo
	WeaponData weaponData[10];

	void create();
	void setStageData();

	void setPlayerData(int* actMap, int connectNum);
	void setMonsterData(int* actMap, int connectNum);
};
extern Stage* st;

extern bool isNewGame;
void loadStage();
void saveStage();
void freeStage();

//----------------------------------------------------------------------------
// passMap

struct PassMap
{
	float nextDt, _nextDt;
	iPoint center;

	float popDt, _popDt;

	float passDt, _passDt;
	int8 prevMapNumber;
	int8 mapNumber;

	void init();
	void pass(int8 mapNum);
	void update(float dt);

	void startNextStage();
	bool nextStage(float dt);
};
extern PassMap* passMap;
#define PASS_STAGE_DT 2.0f
#define PASS_POP_DT 2.0f
#define PASS_MAP_DT 0.3f

void loadPassMap();
void freePassMap();

//-----------------------------------------------------------
// stageLoading
extern iPopup* popStageLoading;
void createPopStageLoading();
void freePopStageLoading();
void showPopStageLoading(bool show);
void drawPopStageLoading(float dt);
bool keyPopStageLoading(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// stageNum
void createPopStageNum();
void freePopStageNum();
void showPopStageNum(bool show);
void drawPopStageNum(float dt);
bool keyPopStageNum(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// UI
void createPopState();
void freePopState();
void showPopState(bool show);
void drawPopState(float dt);
bool keyPopState(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// ProcButton
extern iImage* imgInvenWeaponBtn;
extern iImage** imgProcButtonBtn;
void createPopProcButton();
void freePopProcButton();
void showPopProcButton(bool show);
void drawPopProcButton(float dt);
bool keyPopProcButton(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// MiniMap
void createPopMiniMap();
void freePopMiniMap();
void showPopMiniMap(bool show);
void drawPopMiniMap(float dt);
bool keyPopMiniMap(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// Inven
void createPopInven();
void freePopInven();
void showPopInven(bool show);
void drawPopInven(float dt);
bool keyPopInven(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// ProcMenu
extern iPopup* popProcMenu;
void createPopProcMenu();
void freePopProcMenu();
void showPopProcMenu(bool show);
void drawPopProcMenu(float dt);
bool keyPopProcMenu(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// GameOver
extern iPopup* popGameOver;
void createPopGameOver();
void freePopGameOver();
void showPopGameOver(bool show);
void drawPopGameOver(float dt);
bool keyPopGameOver(iKeyState stat, iPoint point);