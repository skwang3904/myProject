#include "RgProc.h"

#include "Player.h"
#include "Enemy.h"
#include "Room.h"
#include "Weapon.h"
#include "WMelee.h"
#include "RgTile.h"

void loadRgProc()
{
	pc = Player::instance();

	pc->createPlayerImage();
	pc->initPlayerStat();

	weapon = Weapon::instance();

	createEnemy();
}

void freeRgProc()
{
	delete pc;
	delete weapon;

	freeEnemy();
}

void drawRgProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	drawRoomTile(dt);
	
	// 몬스터 draw

	drawEnemy(dt);




	//떨어진 무기 줍기
	// 특정위치에서 (ex 상자) 등장
	// 손에서 버릴때 현재위치에 버림
	for (int i = 0; i < meleeNum; i++)
	{
		if(pc->method == _method[i])
			continue;
		_method[i](dt, true, weapon->wDropPos[i]);
	}
	
	pc->combatDraw(dt);
	pc->movePlayer(dt);
	pc->rootCombat(getKeyDown(keyboard_pickup));
	pc->dropCombat(dt,getKeyDown(keyboard_drop));
}

void keyRgProc(iKeyState stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;
	case iKeyStateEnded:
		break;

	}
}
