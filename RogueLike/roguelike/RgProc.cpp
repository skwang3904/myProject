#include "RgProc.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"

#include "EnemyComposition.h"
#include "Stage.h"

#include "PlayerUI.h"

void loadRgProc()
{
	stage++;
	createTileSet();
	loadRoomTile();

	weapon = Weapon::instance();

	pc = Player::instance();
	pc->createPlayerImage();
	pc->initPlayerStat();

	createEnemy();

	createStage(stage);

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
	static float delta = 0.0f;
	static bool stagetest = false;

	if (getKeyDown(keyboard_attack))
	{
		stagetest = true;
		showRgLoading(true, NextStage);
	}

	static bool test = false;
	if (delta > _nextStageloadingTime/2)
	{
		if (test == false)
		{
			stage++;
			createStage(stage);
			test = true;
		}
	}

	if (delta > _nextStageloadingTime)
	{
		popHP->show(true);
		showRgLoading(false, NextStage);
		stagetest = false;
		delta = 0.0f;
		nextStage = false;
	}

	// 맵 로딩 화면
	drawRoomTile(dt);

	passTileAnimation(dt);
	drawNextDoor(dt);
	if (passAni && nextStage == false)
		return;

	if (nextStage)
	{
		stagetest = true;
		popHP->show(false);
		//loding
	}

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
	//printf("x = %.2f, y = %.2f\n", pc->playerPosition.x, pc->playerPosition.y);


	//--------------------------------------------------------
	// pop
	drawPlayerUI(dt);

	if (stagetest)
	{
		delta += dt;
		drawRgLoading(dt, NextStage);

		return;
	}
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
