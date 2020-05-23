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

	//createFireBallImg();
	//testFireBall();
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

	//freeFireBallImg();
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

	drawEnemy(dt);

	weapon->drawWeapon(dt);
	pc->drawPlayer(dt);

	//for(int i=0; i<50;i++)
	//ball[i]->paint(dt);
	//if (getKeyDown(keyboard_j))
	//{
	//	ball[ballNum]->alive = true;
	//	ball[ballNum]->sp = pc->drawPos;
	//	ball[ballNum]->posFireBall = pc->drawPos;

	//	ball[ballNum]->v =iPointMake(1,1);
	//	ball[ballNum]->v = ball[ballNum]->v / iPointLength(ball[ballNum]->v);
	//	ball[ballNum]->tileNumber = pc->tileNumber;

	//	ballNum++;
	//}

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
