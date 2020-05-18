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
	if (passAni && nextStage == false)
		return;

	if (pc->hp < 0.1f)
	{
		// pc dead ani
		return;
	}

	// 몬스터 draw
	drawEnemy(dt);

	// 무기 생성위치
	weapon->drawWeapon(dt);

	pc->drawPlayer(dt);
	//printf("%.2f\n", pc->hp);

	//--------------------------------------------------------
	// pop
	drawPlayerUI(dt);

	static bool stagetest = false;
	static bool test = false;
	if (nextStage)
	{
		if (test == false)
		{
			stage++;
			createStage(stage);
			popHP->show(false);

			showRgLoading(true, NextStage);

			stagetest = true;
			test = true;
		}

		if (bShowRgLoading(NextStage) == false)
		{
			popHP->show(true);
			stagetest = false;
			nextStage = false;
			test = false;
		}
	}

	if (bShowRgLoading(NextStage))
		drawRgLoading(dt, NextStage);

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
