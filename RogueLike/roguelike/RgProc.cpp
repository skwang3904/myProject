#include "RgProc.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"

#include "EnemyComposition.h"

#include "PlayerUI.h"

void loadRgProc()
{
	stage++;
	createTileSet();
	loadRoomTile();
	newRoomTile();

	createEnemy();
	//createStage(stage);

	weapon = Weapon::instance();
	pc = Player::instance();
	pc->createPlayerImage();

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

	// 맵 로딩 화면
	drawRoomTile(dt);
	passTileAnimation(dt);
	drawNextDoor(dt);

	if (nextStage)
		return;

	if (passAni)
		return;



	// 몬스터 draw
	drawEnemy(dt);

	if (pc->hp < 0.1f)
	{
		// pc dead ani
		return;
	}


	// 무기 생성위치
	weapon->drawWeapon(dt);

	pc->drawPlayer(dt);
	//printf("%.2f\n", pc->hp);

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
