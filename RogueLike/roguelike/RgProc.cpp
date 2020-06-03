#include "RgProc.h"

#include "loading.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"

#include "Enemy.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"

#include "Effect.h"

#include "useitem.h"
#include "TreasureChest.h"

void loadRgProc()
{
	createTileSet();
	loadRoomTile();
	numberFont = new numFont();
	weapon = new Weapon();
	pc = new Player();

	createEffect();
	createItemImg();// createEnemy + createChest
	createEnemy();

	int pcTile = pc->initPlayerPosition();
	setEnemyPosition(pcTile);
	createChest();

	//--------------------------------------------------------
	// pop
	loadRgLoading();
	createPopHP();
	createPopMiniMap();
	createPopCombatMenu();
	createPopItem();
	createPopStageNum();

	showPopHP(true);
	showPopCombatMenu(true);
	showPopMiniMap(true);
	showPopItem(true);
	showPopStageNum(true);

	stage++;
}

void freeRgProc()
{
	freeTileSet();
	freeRoomTile();

	delete numberFont;
	delete weapon;
	delete pc;

	freeEffect();
	freeItemImg();
	freeEnemy();
	freeChest();

	//--------------------------------------------------------
	// pop
	freeRgLoading();
	freePopHP();
	freePopMiniMap();
	freePopCombatMenu();
	freePopItem();
	freePopStageNum();
}

void curtainTile() // 화면가리개
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0,
		devSize.width, (devSize.height - RGTILE_Y * RGTILE_Height) / 2);
	fillRect((devSize.width + RGTILE_X * RGTILE_Width) / 2, 0,
		(devSize.width - RGTILE_X * RGTILE_Width) / 2, devSize.height);
	fillRect(0, (devSize.height + RGTILE_Y * RGTILE_Height) / 2,
		devSize.width, (devSize.height - RGTILE_Y * RGTILE_Height) / 2);
	fillRect(0, 0,
		(devSize.width - RGTILE_X * RGTILE_Width) / 2, devSize.height);
	setRGBA(1, 1, 1, 1);
}

void drawRgProc(float dt)
{
	drawRoomTile(dt);
	passTileAnimation(dt);
	if (passAni)
		return;


	drawNextDoor(dt);
	//if (nextStage)
	//	return;

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

	curtainTile();
	if(getLoading()==false)
	{
		//drawPlayerUI(dt);
		drawPopHP(dt);
		drawPopMiniMap(dt);
		drawPopCombatMenu(dt);
		drawPopItem(dt);
		drawPopStageNum(dt);

		numberFont->drawFont(devSize.width / 2 + 200, 30, TOP | LEFT, 1, 1, 1, "%d", stage);

		if (nextStage)
			nextStageAni(dt);
	}

	//--------------------------------------------------------
	// test
}

void keyRgProc(iKeyState stat, iPoint point)
{
	if (keyPopHP(stat, point) ||
		keyPopCombatMenu(stat, point) ||
		keyPopMiniMap(stat, point) ||
		keyPopItem(stat, point) ||
		keyPopStageNum(stat, point))
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
