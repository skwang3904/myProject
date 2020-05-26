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

void loadRgProc()
{
	//memoryTest();

	createEffect();
	createItemImg();

	stage++;
	createTileSet();
	loadRoomTile();
	newRoomTile();

	weapon = new Weapon();
	pc->instance();

	createEnemy();

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
	freeEffect();
	freeItemImg();

	delete pc;
	delete weapon;

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
	//curtainTile();
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

	drawEffect(dt);

	drawEnemy(dt);

	weapon->drawWeapon(dt);
	pc->drawPlayer(dt);

	//--------------------------------------------------------
	// pop

	//curtainTile();
	drawPlayerUI(dt);

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
