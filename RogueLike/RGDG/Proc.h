#pragma once

#include "iStd.h"

#include "Tile.h"

extern int stageNum;
void loadProc();
void freeProc();
void drawProc(float dt);
void keyProc(iKeyState stat, iPoint point);

class MapTile;
class MapObject;
class PlayerChar;
class Monster;
class Weapon;
class Item;

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

struct Stage
{
	int currStage;

	MapData mapData[TILE_TOTAL_NUM];

	// playerInfo
	PlayerData playerData;

	// monsterInfo
	int s_golemNomalNum;
	int s_golemBossNum;
	MonsterData monsterData[30];

	// weaponInfo
	WeaponData weaponData[10];

	void create();
	void setStage();

};
extern Stage* st;

bool loadStage();
void saveStage();
void freeStage();

struct PassMap
{
	float passDt, _passDt;
	int8 prevMapNumber;
	int8 mapNumber;

	void pass(int8 mapNum);
	void update(float dt);

	void nextStage();
};
extern PassMap* passMap;
#define PASS_DT 0.5f

void loadPassMap();
void freePassMap();
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

게임 종료 조건
-player life = 0
boss kill

#endif

//-----------------------------------------------------------
// UI
void createPopState();
void freePopState();
void showPopState(bool show);
void drawPopState(float dt);
bool keyPopState(iKeyState stat, iPoint point);

//-----------------------------------------------------------
// ProcButton
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