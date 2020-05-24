#include "RgProc.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"

#include "Enemy.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"

#include "Effect.h"


void loadRgProc()
{
	createEffect();

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

int ballNum = 0;
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

	drawEffect(dt);

	drawEnemy(dt);

	weapon->drawWeapon(dt);
	pc->drawPlayer(dt);




	//--------------------------------------------------------
	// pop
	drawPlayerUI(dt);

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
