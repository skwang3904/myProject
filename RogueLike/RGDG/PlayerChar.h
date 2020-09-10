#pragma once

#include "iStd.h"

#include "Common.h"
#include "rgArray.h"

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
	PlayerChar(int index, int8 mapNum, iPoint pos);
	virtual ~PlayerChar();

	virtual void paint(float dt, iPoint off);
	virtual void drawShadow(float dt, iPoint off);
	virtual void action(Object* obj);

	void setNewStage(int index, int8 mapNum, iPoint pos);

	void addWeapon(void* weapon);
	void removeCurrWeapon();
	void removeWeapon(int index);
	void selectWeapon(int index);
	int currWeaponIndex();

	static void cbPlayerSetIdle(iImage* me);
	static void cbPlayerSetAttack(iImage* me);

public:
	uint8 headNum;
	uint8 holdNum;

	PlayerState state;
	iPoint camera;
	iPoint wpVector;
	iPoint wpPosition;

	rgArray* arrayWeapon;
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

void loadPlayerChar();
void freePlayerChar();
void drawPlayerChar(float dt);