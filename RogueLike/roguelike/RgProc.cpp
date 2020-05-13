#include "RgProc.h"

#include "Player.h"
#include "Enemy.h"
#include "Room.h"
#include "Weapon.h"
#include "WMelee.h"
#include "RgTile.h"

void loadRgProc()
{
	createTileSet();
	loadRoomTile();

	createEnemy();

	pc = Player::instance();

	pc->createPlayerImage();
	pc->initPlayerStat();

	weapon = Weapon::instance();

}

void freeRgProc()
{
	delete pc;
	delete weapon;

	freeEnemy();
	freeTileSet();
	freeRoomTile();
}

void drawRgProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	drawRoomTile(dt);
	
	// 몬스터 draw

	if (pc->hp <= 0.0f)
		return;

	drawEnemy(dt);

	// 특정위치에서 (ex 상자) 등장
	for (int i = 0; i < meleeNum; i++)
	{
		if(pc->method == _method[i])
			continue;
		_method[i](dt, true, weapon->wDropPos[i]);
	}
	
	pc->drawPlayer(dt);
	//printf("%.2f\n", pc->hp);
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
