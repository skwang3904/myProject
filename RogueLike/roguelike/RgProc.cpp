#include "RgProc.h"

#include "Room.h"
#include "Stage.h"

#include "loading.h"

#include "Weapon.h"

#include "Enemy.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"

#include "Effect.h"

#include "useitem.h"
#include "TreasureChest.h"

//--------------------------------------------------------

void loadRgProc()
{

	createEffect();
	createItemImg();

	createTileSet();
	loadRoomTile();


	//--------------------------------------------------------
	numberFont = new numFont();
	weapon = new Weapon();
	pc = new Player();

	createEnemy();

	int pcTile = pc->initPlayerPosition();
	setEnemyPosition(pcTile);

	createChest();
	
	//--------------------------------------------------------
	// pop

	loadRgLoading();
	loadPlayerUI();

	stage++;
}

void freeRgProc()
{
	freeEffect();
	freeItemImg();

	freeTileSet();
	freeRoomTile();

	delete numberFont;
	delete weapon;
	delete pc;

	freeEnemy();

	freeChest();
	//--------------------------------------------------------
	// pop

	freeRgLoading();
	freePlayerUI();

	//--------------------------------------------------------
	// test

}

void curtainTile() // 화면가리개
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0,
		devSize.width, (devSize.height - RGTILE_Y * RGTILE_Height) / 2.0f);
	fillRect((devSize.width + RGTILE_X * RGTILE_Width) / 2.0f, 0,
		(devSize.width - RGTILE_X * RGTILE_Width) / 2.0f, devSize.height);
	fillRect(0, (devSize.height + RGTILE_Y * RGTILE_Height) / 2.0f,
		devSize.width, (devSize.height - RGTILE_Y * RGTILE_Height) / 2.0f);
	fillRect(0, 0,
		(devSize.width - RGTILE_X * RGTILE_Width) / 2.0f, devSize.height);
	setRGBA(1, 1, 1, 1);
}

void drawRgProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);

	drawRoomTile(dt);
	passTileAnimation(dt);
	if (passAni)
		return;


	drawNextDoor(dt);
	if (nextStage)
		return;

	if (pc->hp < 0.1f)
	{
		// pc dead ani
		//return;
	}

	drawEnemy(dt);

	weapon->drawWeapon(dt);
	pc->drawPlayer(dt);

	drawChest(dt);
	//--------------------------------------------------------
	// pop

	//curtainTile();
	//drawPlayerUI(dt);
	curtainTile();
	if (getLoading() == false)
	{
		drawPlayerUI(dt);

		if (nextStage)
			nextStageAni(dt);
	}
	//--------------------------------------------------------
	// test
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
