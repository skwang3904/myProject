#include "RgProc.h"

#include "loading.h"
#include "RgLoading.h"

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
	numberFont = new numFont();

	createTileSet();
	loadRoomTile();
	weapon = new Weapon();
	pc = new Player();

	createEffect();
	createItemImg();// createEnemy + createChest
	createEnemy();
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

void drawRgProc(float dt)
{
	drawRoomTile(dt);
	passTileAnimation(dt);

	if (passAni())
		return;

	drawNextDoor(dt);
	if (nextStage)
	{
		nextStageAni(dt);
		return;
	}

	if (pc->hp < 0.1f)
	{
		// pc dead ani
		//return;
	}

	drawEnemy(dt);

	weapon->drawWeapon(dt);
	pc->paint(dt);

	drawChest(dt);

	//--------------------------------------------------------
	// pop

	drawPopHP(dt);
	drawPopMiniMap(dt);
	drawPopCombatMenu(dt);
	drawPopItem(dt);
	drawPopStageNum(dt);

	numberFont->drawFont(devSize.width / 2 + 200, 30, TOP | LEFT, 1, 1, 1, "%d", stage);

	//--------------------------------------------------------
	// test
}

void keyRgProc(iKeyState stat, iPoint point)
{
	if (keyPopHP(stat, point) ||
		keyPopMiniMap(stat, point) ||
		keyPopCombatMenu(stat, point) ||
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
