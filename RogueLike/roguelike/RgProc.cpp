#include "RgProc.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"

#include "Enemy.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"

#include "Effect.h"

#include "useitem.h"

void memoryTest()
{
	stage++;
	for (int i = 0; i < 300; i++)
	{
		printf("i = %d\n", i);
		createEnemy();
		//int pcTile = pc->initPlayerPosition();
		//setNextDoor(pcTile);
		//setEnemyPosition(pcTile);
		//loadRgLoading();
		//loadPlayerUI();



		freeEnemy();

	
		//freeRgLoading();
		//freePlayerUI();
	}
}
//--------------------------------------------------------
//--------------------------------------------------------

void loadRgProc()
{
	//memoryTest();


	createEffect();
	createItemImg();

	createTileSet();
	loadRoomTile();
	newRoomTile();

	numberFont = new numFont();
	weapon = new Weapon();
	pc->instance();

	createEnemy();

	int pcTile = pc->initPlayerPosition();
	setEnemyPosition(pcTile);

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

	delete numberFont;
	delete weapon;
	delete pc;

	freeEnemy();
	freeTileSet();
	freeRoomTile();

	//--------------------------------------------------------
	// pop

	freeRgLoading();
	freePlayerUI();

	//--------------------------------------------------------
	// test

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

	//--------------------------------------------------------
	// pop

	//curtainTile();
	//drawPlayerUI(dt);

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
