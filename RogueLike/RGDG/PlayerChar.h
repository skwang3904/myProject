#pragma once

#include "iStd.h"

#include "Common.h"

#define PLAYER_IMG_NUM 8 
class PlayerChar : public Object
{
public:
	PlayerChar(int index);
	virtual ~PlayerChar();

	virtual void paint(float dt, iPoint off);
	void initState();
public:
	iImage** imgs;

	float hp, _hp;
	float attackPoint, _attackPoint;
	float attackSpeed, _attackSpeed;
	float moveSpeed;

	iPoint camera;
};
extern PlayerChar* player;


void loadPlayerChar();
void freePlayerChar();
void drawPlayerChar(float dt);
