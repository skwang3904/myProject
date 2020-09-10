#include "Proc.h"

#include "Tile.h"
#include "Map.h"

#include "ProcData.h"
#include "PlayerChar.h"
#include "Monster.h"
#include "Weapon.h"
#include "Item.h"

void loadProc()
{
	loadNumberFont();

	loadMap();
	loadStage();
	loadPlayerChar();
	loadMonster();
	loadWeapon();
	loadItem();

	loadPassMap();

	createPopState();
	createPopProcButton();
	createPopMiniMap();
	createPopInven();
	createPopProcMenu();
	createPopGameOver();

	showPopState(true);
	showPopProcButton(true);
	showPopMiniMap(true);
	showPopInven(true);
}

void freeProc()
{
	freeNumberFont();

	freeMap();
	freeStage();
	freePlayerChar();
	freeMonster();
	freeWeapon();
	freeItem();

	freePassMap();

	freePopState();
	freePopProcButton();
	freePopMiniMap();
	freePopInven();
	freePopProcMenu();
	freePopGameOver();
}

void drawProc(float dt)
{
	// if(loading)
	// return;


	float pop_dt = dt;
	if (popProcMenu->bShow || 
		popGameOver->bShow)
		dt = 0.0f;

	float pass_dt = dt;
	if (passMap->passDt < passMap->_passDt ||
		passMap->nextDt < passMap->_nextDt)
		pass_dt = 0.0f;

	drawMap(pass_dt);
	drawPlayerChar(pass_dt);
	drawMonster(pass_dt);
	drawWeapon(pass_dt);
	drawItem(pass_dt);

	passMap->update(dt);
	
	drawPopState(dt);
	drawPopProcButton(dt);
	drawPopMiniMap(dt);
	drawPopInven(dt);
	drawPopProcMenu(pop_dt);
	drawPopGameOver(pop_dt);
	//numberFont->drawFont()

	if (passMap->nextStage(dt))
		return;
}

void keyProc(iKeyState stat, iPoint point)
{
	if (keyPopGameOver(stat, point) ||
		keyPopProcMenu(stat, point) ||
		keyPopInven(stat,point)	||
		keyPopMiniMap(stat, point) ||
		keyPopProcButton(stat, point) ||
		keyPopState(stat, point))
		return;
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
		st->setString("%d\n%d", 0, 0);

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
	int stage = atoi(line[1]);

	iString::freeStringLine(line, lineNum);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 128);
	g->init(size);

	setRGBA(0, 0, 0.2f, 1);
	g->fillRect(0, 0, size.width, size.height,3);
	setRGBA(1, 1, 1, 1);

	setStringRGBA(1, 1, 1, 1);
	setStringSize(25);
	setStringBorder(0);
	g->drawString(10, size.height * 0.33f, VCENTER | LEFT, "HP: ");
	g->drawString(10, size.height * 0.67f, VCENTER | LEFT, "Stage: ");
	g->drawString(size.width / 2.0f, size.height * 0.33f, VCENTER | LEFT, "%d", hp);
	g->drawString(size.width - 25, size.height * 0.67f, VCENTER | HCENTER, "%d", stage);

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

	stState->setString("%.0f\n%d", player->hp, stageNum + 1);
}

bool keyPopState(iKeyState stat, iPoint point)
{
	return false;
}

//-----------------------------------------------------------
// ProcButton
iPopup* popProcButton;
iImage* imgProcButtonBtn;

void createPopProcButton()
{
	int i, j;
	iImage* img;
	Texture* tex;
	iPopup* pop = new iPopup(iPopupStyleMove);

	img = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 64);


	setStringRGBA(0, 0, 0, 1);
	setStringSize(30);
	setStringBorder(0);
	for (i = 0; i < 2; i++)
	{
		g->init(size);
		
		if (i == 0) 	setRGBA(1, 1, 1, 1);
		else 	setRGBA(0, 1, 0, 1);
		g->fillRect(0, 0, size.width, size.height, 10);
		g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, "menu");

		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	setRGBA(1, 1, 1, 1);
	setStringRGBA(1, 1, 1, 1);
	setStringSize(1);
	setStringBorder(0);

	imgProcButtonBtn = img;
	pop->addObject(img);

	pop->openPosition = iPointMake(devSize.width / 2.0f - size.width / 2.0f, -100);
	pop->closePosition = iPointMake(devSize.width / 2.0f - size.width / 2.0f, 10);
	popProcButton = pop;
}

void freePopProcButton()
{
	delete popProcButton;
}

void showPopProcButton(bool show)
{
	popProcButton->show(show);
}

void drawPopProcButton(float dt)
{
	for (int i = 0; i < 1; i++)
		imgProcButtonBtn->setTexAtIndex(i == popProcButton->selected);

	popProcButton->paint(dt);
}

bool keyPopProcButton(iKeyState stat, iPoint point)
{
	if (popProcButton->bShow == false)
		return false;

	if (popProcButton->stat != iPopupStatProc)
		return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popProcButton->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			showPopProcMenu(true);
		}
		break;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < 1; i++)
		{
			if (containPoint(point, imgProcButtonBtn->touchRect(popProcButton->closePosition)))
			{
				j = i;
				break;
			}
		}
		
		if (popProcButton->selected != j)
			;

		popProcButton->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}

	return true;
}

//-----------------------------------------------------------
// MiniMap
iPopup* popMiniMap;
iImage* imgMiniMap;

void refreshMiniMap(Texture* tex)
{
	fbo->bind(tex);
	fbo->clear(0, 0, 0, 0.2f);

	setLineWidth(10);
	for (int i = 0; i < TILE_TOTAL_NUM; i++)
	{
		if (maps[i]->tileIndex == -1)
			continue;


		iPoint p = iPointMake(TILE_Width * (i % TILE_TOTAL_SQRT), TILE_Height * (i / TILE_TOTAL_SQRT));
		setRGBA(0, 0, 0, 1);
		fillRect(p.x, p.y, TILE_Width, TILE_Height);

		if (i == player->mapNumber) setRGBA(0, 1, 0, 1);
		else						setRGBA(1, 1, 1, 1);

		fillRect(p.x + 1, p.y + 1, TILE_Width - 2, TILE_Height - 2 , 10);

		if (maps[i]->state == MapType_Boss)
		{
			setRGBA(1, 0, 0, 0.7f);
			fillRect(p.x + 3, p.y + 3, TILE_Width - 6, TILE_Height - 6, 10);
		}
	}
	setRGBA(1, 1, 1, 1);
	setLineWidth(1);

	fbo->unbind();
}

void createPopMiniMap()
{
	int i, j;
	iImage* img;
	Texture* tex;
	iPopup* pop = new iPopup(iPopupStyleMove);
	
	img = new iImage();
	iSize size = iSizeMake(TILE_Width * TILE_TOTAL_SQRT, TILE_Height * TILE_TOTAL_SQRT);
	tex = createTexture(size.width, size.height);
	refreshMiniMap(tex);
	img->addObject(tex);
	freeImage(tex);

	img->reverse = REVERSE_HEIGHT;
	img->ratio = 2.0f;
	img->position = iPointMake(devSize.width - size.width * img->ratio - 50, 50);
	imgMiniMap = img;
	pop->addObject(img);

	popMiniMap = pop;
}

void freePopMiniMap()
{
	delete popMiniMap;
}

void showPopMiniMap(bool show)
{
	popMiniMap->show(show);
}

void drawPopMiniMap(float dt)
{
	refreshMiniMap(imgMiniMap->tex);
	popMiniMap->paint(dt);
}

bool keyPopMiniMap(iKeyState stat, iPoint point)
{
	return false;
}

//-----------------------------------------------------------
// Inven
iPopup* popInven;
iImage** imgInvenBtn;

Texture* texInvenWeapon;

void drawPopInvenWeapon(float dt)
{
	if (player->arrayWeapon->count == 0)
	{
		texInvenWeapon = NULL;
		return;
	}

	Weapon* w = (Weapon*)player->arrayWeapon->objectAtIndex(player->currWeaponIndex());
	
	texInvenWeapon = w->img->tex;
}

void drawPopInvenBefore(iPopup* me, iPoint p, float dt);

void createPopInven()
{
	int i, j;
	iImage* img;
	Texture* tex;
	iPopup* pop = new iPopup(iPopupStyleMove);

	imgInvenBtn = (iImage**)malloc(sizeof(iImage*) * 4);
	iSize size = iSizeMake(128, 128);
	for (i = 0; i < 1; i++)
	{
		img = new iImage();

		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.width, size.height);
			fbo->bind(tex);
			
			if (j == 0) setRGBA(1, 0, 1, 1);
			else		setRGBA(0, 1, 0, 1);
			
			fillRect(0, 0, size.width, size.height, 15);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake((size.width + 10) * (i % 2),
			(size.height + 10) * (i / 2));

		imgInvenBtn[i] = img;
		pop->addObject(img);
	}
	iPoint p = iPointMake(devSize.width - size.width * 1 - 30, devSize.height - size.height * 1 - 30);

	pop->openPosition = p + iPointMake(500, 0);
	pop->closePosition = p;
	pop->methodDrawBefore = drawPopInvenBefore;
	popInven = pop;
}

void freePopInven()
{
	delete popInven;
	free(imgInvenBtn);
}

void showPopInven(bool show)
{
	popInven->show(show);
}

void drawPopInvenBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 1; i++)
		imgInvenBtn[i]->setTexAtIndex(i == popInven->selected);

	drawPopInvenWeapon(dt);
}

void drawPopInven(float dt)
{
	popInven->paint(dt);

	iPoint p = popInven->closePosition + iPointMake(64,64);
	if(texInvenWeapon)
		drawImage(texInvenWeapon, p.x, p.y, VCENTER | HCENTER);
}

bool keyPopInven(iKeyState stat, iPoint point)
{

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popInven->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			printf("click inven\n");
		}
		else if (i == 1)
		{
		}
		return true;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < 1; i++)
		{
			if (containPoint(point, imgInvenBtn[i]->touchRect(popInven->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popInven->selected != j)
			;

		popInven->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}


	return false;
}

//-----------------------------------------------------------
// ProcMenu
iPopup* popProcMenu;
iImage** imgProcMenu;

void drawPopProcMenuBefore(iPopup* me, iPoint p, float dt);

void createPopProcMenu()
{
	int i, j;
	iImage* img;
	Texture* tex;
	iPopup* pop = new iPopup(iPopupStyleMove);
	imgProcMenu = (iImage**)malloc(sizeof(iImage*) * 2);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(128, 64);
	const char* strBtn[2] = {
		"RESUME", "EXIT"
	};
	setStringSize(30);
	setStringBorder(0);
	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
			{
				setRGBA(0, 0, 0, 1);
				setStringRGBA(1, 1, 1, 1);
			}
			else
			{
				setRGBA(0, 0.5f, 0, 1);
				setStringRGBA(0, 0, 0, 1);
			}
			g->fillRect(0, 0, size.width, size.height, 5);
			g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, strBtn[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}

		img->position = iPointMake((devSize.width - size.width) / 2.0f, devSize.height / 2.0f - 64 + 74 * i);
		imgProcMenu[i] = img;
		pop->addObject(img);
	}
	setRGBA(1, 1, 1, 1);
	setStringRGBA(1, 1, 1, 1);
	setStringSize(1);
	setStringBorder(0);

	pop->methodDrawBefore = drawPopProcMenuBefore;
	popProcMenu = pop;
}

void freePopProcMenu()
{
	delete popProcMenu;
	free(imgProcMenu);
}

void showPopProcMenu(bool show)
{
	popProcMenu->show(show);
}

void drawPopProcMenuBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 2; i++)
		imgProcMenu[i]->setTexAtIndex(i == popProcMenu->selected);
}

void drawPopProcMenu(float dt)
{
	popProcMenu->paint(dt);
}

bool keyPopProcMenu(iKeyState stat, iPoint point)
{
	if (popProcMenu->bShow == false)
		return false;

	if (popProcMenu->stat != iPopupStatProc)
		return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popProcMenu->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			// 재개
			showPopProcMenu(false);
		}
		else if (i == 1)
		{
			//나가기
			showPopProcMenu(false);
		}
		return true;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgProcMenu[i]->touchRect(popProcMenu->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popProcMenu->selected != j)
			;

		popProcMenu->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}

	return true;
}

//-----------------------------------------------------------
// GameOver
iPopup* popGameOver;
iImage** imgGameOverBtn;

void drawPopGameOverBefore(iPopup* me, iPoint p, float dt);

void createPopGameOver()
{
	int i, j;
	iImage* img;
	Texture* tex;
	iPopup* pop = new iPopup(iPopupStyleMove);
	
	img = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(256, 256);
	g->init(size);

	setRGBA(0, 1, 0, 1);
	g->fillRect(0, 0, size.width, size.height, 20);
	setRGBA(1, 1, 1, 1);

	setStringRGBA(0, 0, 0, 1);
	setStringSize(40);
	setStringBorder(0);
	g->drawString(size.width / 2.0f, 32, VCENTER | HCENTER, "Game Over");
	setStringRGBA(1, 1, 1, 1);
	setStringSize(1);
	setStringBorder(0);

	tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);


	imgGameOverBtn = (iImage**)malloc(sizeof(iImage*) * 2);
	const char* strBtn[2] = {
	"Main Menu", "Game Exit"
	};
	size = iSizeMake(240, 48);
	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
			{
				setRGBA(0, 0, 1, 1);
				setStringRGBA(1, 1, 1, 1);
			}
			else
			{
				setRGBA(1, 1, 1, 1);
				setStringRGBA(0, 0, 0, 1);
			}
			g->fillRect(0, 0, size.width, size.height, 5);
			setRGBA(1, 1, 1, 1);


			setStringSize(20);
			setStringBorder(0);
			g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, strBtn[i]);
			setStringRGBA(1, 1, 1, 1);
			setStringSize(1);
			setStringBorder(0);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(8 , 80 + 58 * i);
		imgGameOverBtn[i] = img;
		pop->addObject(img);
	}

	pop->openPosition = iPointMake(devSize.width/2.0f - 128, -128);
	pop->closePosition = iPointMake(devSize.width / 2.0f - 128, devSize.height / 2.0f - 64);
	pop->methodDrawBefore = drawPopGameOverBefore;
	popGameOver = pop;
}

void freePopGameOver()
{
	delete popGameOver;
	free(imgGameOverBtn);
}

void showPopGameOver(bool show)
{
	popGameOver->show(show);
}

void drawPopGameOverBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 2; i++)
		imgGameOverBtn[i]->setTexAtIndex(i == popGameOver->selected);
}

void drawPopGameOver(float dt)
{
	popGameOver->paint(dt);
}

bool keyPopGameOver(iKeyState stat, iPoint point)
{
	if (popGameOver->bShow == false)
		return false;

	//if(popGameOver->stat != iPopupStatProc)
	//	return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popGameOver->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			// main
			showPopGameOver(false);
		}
		else if (i == 1)
		{
			//exit
			showPopGameOver(false);
		}
		break;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgGameOverBtn[i]->touchRect(popGameOver->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popGameOver->selected != j)
			;

		popGameOver->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}

	return true;
}
