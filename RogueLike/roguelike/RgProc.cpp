#include "RgProc.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"

#include "Enemy.h"
//#include "EnemyStruct.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"


void loadRgProc()
{
	stage++;
	createTileSet();
	loadRoomTile();
	newRoomTile();

	createEnemy();

	weapon = new Weapon();
	pc = new Player();

	int pcTile = pc->initPlayerPosition();
	setNextDoor(pcTile);
	setEnemyPosition(pcTile);

	//--------------------------------------------------------
	// pop

	loadRgLoading();
	loadPlayerUI();
}

void freeRgProc()
{
	delete pc;
	delete weapon;

	freeEnemy();
	freeTileSet();
	freeRoomTile();

	//--------------------------------------------------------
	// pop

	freeRgLoading();
	freePlayerUI();
}

void drawRgProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);

	passTileAnimation(dt);
	if (passAni)
		return;

	drawRoomTile(dt);

	drawNextDoor(dt);
	if (nextStage)
		return;

	if (pc->hp < 0.1f)
	{
		// pc dead ani
		return;
	}

	drawEnemy(dt);

	weapon->drawWeapon(dt);
	pc->drawPlayer(dt);
}

void keyRgProc(iKeyState stat, iPoint point)
{
	if (keyPlayerUI(stat, point))
		return;


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
