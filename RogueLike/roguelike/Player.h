#pragma once

#include "iStd.h"

/*
캐릭터 
( 키보드 조작, hp, 이동속도, 보는방향 v, 장비장착, 애니메이션 )
- 캐릭터 이미지, 애니메이션 ( 이동, 낭떨어지 )
- 장비 장착 위치
- 장비 모션, 충돌체크
*/

#define HALF_OF_TEX_WIDTH	pc->img[0]->tex->width/2
#define HALF_OF_TEX_HEIGHT	pc->img[0]->tex->height/2
#define HALF_OF_TEX_POINT	iPointMake(pc->img[0]->tex->width/2,	\
										pc->img[0]->tex->height/2)

#define Player_imgFall 8
#define Player_imgEvasion 9

#define EVASION_DISTANCE 500
#define EVASION_DURATION 0.3f


struct PlayerWP;
struct MapTile;
enum CharAction;

class Player
{
public:
	Player();
	virtual ~Player();

	void initPlayerStat();
	uint8 initPlayerPosition();
	void createPlayerImage();

	bool actionCheck(bool key);
	void drawPlayer(float dt);

	void showHpBar(float dt);

	void combatDraw(float dt);
	void rootCombat(bool key);
	void dropCombat(float dt, bool key);
	void choseWeapon(bool key);

	void paint(float dt);
	bool evasionPlayer(MapTile* tile, float dt);

	void setPlayerTile();

	void drawtouchPlayer();

public:
	iImage** img;

	float hp, _hp;
	float prevHP;
	float attackDmg;
	float _attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float moveSpeed;

	CharAction act;
	int tileNumber;

	iPoint camPosition;
	iPoint playerPosition;
	iPoint drawPos;
	iPoint viewVector;
	int headNum;
	iPoint evasV;
	iPoint combatV;
	float combatAngleV;

	iRect touchPlayer;

	rgArray* weaponArray;
	PlayerWP* pwp;
	int pwpCount;

public:
	uint16 coin;
};
extern Player* pc;

/*
	// 0, 1 몸 정지모션
	img[0]
	img[1]

	// 2, 3 몸 애니메이션
	img[2]
	img[3]

	// 4 ~ 7 머리 애니메이션
	img[4]	left
	img[5]	right
	img[6]	up
	img[7]	down

	// 8 낙하 애니메이션
	img[8]	falling

	// 9 점프 애니메이션
	img[9]
*/