#include "RgProc.h"

#include "Room.h"
#include "RgTile.h"
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
	freePlayerUI();

}

void drawRgProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);
	static float delta = 0.0f;
	static bool stagetest = false;


	if (delta > 1.0f)
	{
		stage++;
		createStage(stage);

		popHP->show(true);

		stagetest = false;
		delta = 0.0f;
		nextStage = false;
	}

	// 맵 로딩 화면
	if (stagetest)
	{
		delta += dt;
		return;
	}

	drawRoomTile(dt);
	passTileAnimation(dt);
	drawNextDoor(dt);
	if (passAni)
		return;

	if (nextStage)
	{
		stagetest = true;
		popHP->show(false);
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
