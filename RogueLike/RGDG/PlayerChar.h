#pragma once

#include "iStd.h"

#include "Common.h"

enum PlayerState {
	player_idle = 0,
	player_move,
	player_attack,
	player_jump,
	player_hurt,
	player_death,
};

class PlayerChar : public Object
{
public:
	PlayerChar(int index);
	virtual ~PlayerChar();

	void initData();
	virtual void paint(float dt, iPoint off);


public:
	uint8 headNum;
	uint8 holdNum;

	PlayerState state;
	iPoint camera;

	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float moveSpeed;

	iArray* arrayWeapon;

};
extern PlayerChar* player;

/*
imgs
0 : head left	<- down
1 : head right	<- right
2 : head up		<- up
3 : head down	<- left

4 : hold body right 
5 : hold body up,down 
6 : ani body right
7 : ani body up,down 
8 : char Good
9 : char Jump

*/
