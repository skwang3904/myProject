#include "RgProc.h"

#include "Player.h"
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
}

void freeRgProc()
{
	delete pc;
	delete weapon;
}

void drawRgProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	drawRoomTile(dt);
	
	// 몬스터 draw
	//weapon->drawWeapon(dt);


	//계속
	//떨어진 무기 줍기
	// 특정위치에서 (ex 상자) 등장
	// 손에서 버릴때 현재위치에 등장

	
	for (int i = 0; i < meleeNum; i++)
	{
		if(pc->mw == _mw[i])
			continue;
		_mw[i](dt, true, weapon->wDropPos[i]);
	}
	

	pc->movePlayer(dt);
	pc->rootCombat(getKeyDown(keyboard_pickup));
	pc->dropCombat(dt,getKeyDown(keyboard_drop));
	pc->combatDraw(dt);
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
