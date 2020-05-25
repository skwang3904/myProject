#include "RgProc.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"

#include "Enemy.h"
#include "EnemyComposition.h"

#include "PlayerUI.h"

#include "Effect.h"

#include "useitem.h"


iImage* coinimgtest;
void cointest()
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(500, 500);
	g->init(size);

	setStringSize(50);
	setStringRGBA(1, 0, 0, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, TOP | LEFT, "%d", pc->coin);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	coinimgtest = img;
}

void freecointest()
{
	delete coinimgtest;
}
//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------

void loadRgProc()
{
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
	freecointest();
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
		//return;
	}

	drawEffect(dt);

	drawEnemy(dt);

	weapon->drawWeapon(dt);
	pc->drawPlayer(dt);

	//--------------------------------------------------------
	// pop
	drawPlayerUI(dt);

	//--------------------------------------------------------
	// test

	cointest();
	coinimgtest->paint(dt, iPointZero, REVERSE_NONE);
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
