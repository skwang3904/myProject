#include "Proc.h"

#include "Map.h"

#include "PlayerChar.h"
#include "Monster.h"
#include "Weapon.h"

void loadProc()
{
	loadMap();
	player = new PlayerChar(0);
	loadMonster();
	loadWeapon();


	createPopState();
	showPopState(true);
}

void freeProc()
{
	freeMap();
	delete player;
	freeMonster();
	freeWeapon();


	freePopState();
}

void drawProc(float dt)
{
	drawMap(dt);
	player->paint(dt, DRAW_OFF);
	drawMonster(dt);
	drawWeapon(dt);


	drawPopState(dt);

}

void keyProc(iKeyState stat, iPoint point)
{
}


//-----------------------------------------------------------
// UI
iPopup* popState;
iStrTex* stState;

Texture* methodStState(const char* str);

void createPopState()
{
	int i, j;
	iImage* img;
	Texture* tex;

	iPopup* pop = new iPopup(iPopupStyleNone);

	for (i = 0; i < 1; i++)
	{
		img = new iImage();
		iStrTex* st = new iStrTex(methodStState);
		st->setString("%d\n%d\n%d", 0, 0, 0);

		img->addObject(st->tex);
		img->position = iPointMake(10, 100);
		stState = st;
		pop->addObject(img);
	}

	popState = pop;
}

Texture* methodStState(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum);
	int hp = atoi(line[0]);
	int ap = atoi(line[1]);
	int ms = atoi(line[2]);
	iString::freeStringLine(line, lineNum);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 512);
	g->init(size);

	setRGBA(0, 0, 0.5f, 1);
	g->fillRect(0, 0, size.width, size.height);
	setRGBA(1, 1, 1, 1);

	setStringRGBA(1, 1, 1, 1);
	setStringSize(30);
	setStringBorder(0);
	g->drawString(10, 100, TOP | LEFT, "%d", hp);
	g->drawString(10, 200, TOP | LEFT, "%d", ap);
	g->drawString(10, 300, TOP | LEFT, "%d", ms);

	setStringRGBA(1, 1, 1, 1);
	setStringSize(1);
	setStringBorder(0);

	return g->getTexture();
}


void freePopState()
{
	delete popState;
	delete stState;
}

void showPopState(bool show)
{
	popState->show(show);
}

void drawPopState(float dt)
{
	popState->paint(dt);

	stState->setString("%d\n%d\n%d", (int)player->hp, 0, 0);
}

bool keyPopState(iKeyState stat, iPoint point)
{
	return false;
}
