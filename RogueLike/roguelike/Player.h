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

#define EVASION_DISTANCE 500
#define EVASION_DURATION 0.4f

#define MELEE_NUM 2

typedef void (*Method_Combat)(float dt, iPoint dropP);
extern Method_Combat _method[MELEE_NUM];

enum PlayerAction {
	idle = 0,
	attacking,
	falling,
	evasion
};

struct meleeWeapon;
struct MapTile;

class Player
{
private:
	Player();
public:
	virtual ~Player();

	static Player* instance();

	void initPlayerStat();
	void createPlayerImage();

	bool actionCheck(bool key);
	void drawPlayer(float dt);

	void showHpBar(float dt);

	void combatDraw(float dt);
	void rootCombat(bool key);
	void dropCombat(bool key);

	void movePlayer(float dt);
	bool evasionPlayer(MapTile* tile, float dt);

public:
	//iArray* weaponArray; 무기 인벤토리
	iImage** img;

	float hp, _hp;
	float attackDmg;
	float _attackDmg;
	float attackSpeed;
	float _attackSpeed;
	float moveSpeed;

	PlayerAction act;

	iPoint camPosition;
	iPoint playerPosition;
	iPoint drawPos;

	iPoint viewVector;

	iRect touchPlayer;

	Method_Combat method;
	meleeWeapon* mw;
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