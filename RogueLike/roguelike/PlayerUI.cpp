#include "PlayerUI.h"

#include "Room.h"

void loadPlayerUI()
{
	createPopHP();
	createPopCombatMenu();
	createPopInven1();
	createPopMiniMap();

	showPopHP(true);
	showPopCombatMenu(true);
	showPopMiniMap(true);
}

void freePlayerUI()
{
	freePopHP();
	freePopCombatMenu();
	freePopInven1();
	freePopMiniMap();
}

void drawPlayerUI(float dt)
{
	drawPopHP(dt);
	drawPopCombatMenu(dt);
	drawPopInven1(dt);
	drawPopMiniMap(dt);
}

bool keyPlayerUI(iKeyState stat, iPoint point)
{
	if (keyPopHP(stat, point) ||
		keyPopCombatMenu(stat,point)||
		keyPopInven1(stat,point)||
		keyPopMiniMap(stat,point))
		return true;
}

/////////////////////////////////////////////////////////

iPopup* popHP;
iImage* imgHPgageGreen;
iImage* imgHPgageRed;

void createPopHP()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popHP = pop;

	iImage* imgIconBar = new iImage();
	iImage* imgheart = new iImage();
	iImage* imgBarBG = new iImage();
	iImage* imgBar = new iImage();
	iImage* imgBarFillGreen = new iImage();
	iImage* imgBarFillRed = new iImage();
	imgHPgageGreen = imgBarFillGreen;
	imgHPgageRed = imgBarFillRed;

	Texture* texIconBar = createImage("assets/PlayerUI/Icon Bar.png");
	Texture* texheart = createImage("assets/PlayerUI/heart.png");
	Texture* texBarBG = createImage("assets/PlayerUI/Bar_BG.png");
	Texture* texBar = createImage("assets/PlayerUI/Bar.png");
	Texture* texBarFillGreen = createImage("assets/PlayerUI/Bar_Fill_Green.png");
	Texture* texBarFillRed = createImage("assets/PlayerUI/Bar_Fill_Red.png");
	
	imgIconBar->addObject(texIconBar);
	imgheart->addObject(texheart);

	iPoint iconPoint = iPointMake(100, 30);
	imgIconBar->position = iconPoint;
	imgheart->position = imgIconBar->position + iPointMake((imgIconBar->tex->width - imgheart->tex->width) / 2,
		(imgIconBar->tex->height - imgheart->tex->height) / 2 + 5);

	imgBarBG->addObject(texBarBG);
	imgBar->addObject(texBar);
	imgBarFillGreen->addObject(texBarFillGreen);
	imgBarFillRed->addObject(texBarFillRed);

	iPoint barPoint = iconPoint + iPointMake(140, 0);
	imgBarBG->position = barPoint;
	imgBar->position = imgBarBG->position + iPointMake((imgBarBG->tex->width - imgBar->tex->width)/2,
		(imgBarBG->tex->height - imgBar->tex->height) / 2);
	imgBarFillGreen->position = imgBar->position;
	imgBarFillRed->position = imgBar->position;

	pop->addObject(imgIconBar);
	pop->addObject(imgheart);
	pop->addObject(imgBarBG);
	pop->addObject(imgBar);
	pop->addObject(imgBarFillGreen);
	pop->addObject(imgBarFillRed);

	freeImage(texIconBar);
	freeImage(texheart);
	freeImage(texBarBG);
	freeImage(texBar);
	freeImage(texBarFillGreen);
	freeImage(texBarFillRed);
}

void freePopHP()
{
	delete popHP;
}

void showPopHP(bool show)
{
	popHP->show(show);
}

void drawPopHP(float dt)
{
	static float prevHP = pc->_hp;
	static float currentHP = pc->_hp;
	static float delta = 0.0f;
	if (pc->hp < 0.0f)
		pc->hp = 0.0f;
	currentHP = pc->hp;
	if (prevHP != currentHP)
	{
		delta += dt;
		imgHPgageGreen->imgRatioX = linear(delta / 2.0f, prevHP, currentHP) / pc->_hp;
		
		if (delta > 2.0f)
		{
			delta = 0.0f;
			prevHP = currentHP;
		}
	}
		
	imgHPgageRed->imgRatioX = pc->hp / pc->_hp;
	if (imgHPgageRed->imgRatioX > 1.0f)
		imgHPgageRed->imgRatioX = 1.0f;
	popHP->paint(dt);
}

bool keyPopHP(iKeyState stat, iPoint point)
{
	if (popHP->bShow == false)
		return false;

	if (popHP->stat != iPopupStatProc)
		return true;

	return false;
}

/////////////////////////////////////////////////////////

iPopup* popMiniMap;

void createPopMiniMap()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popMiniMap = pop;
	
	iImage* imgMiniMap = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(20 * TILEOFF_SQRT, 20 * TILEOFF_SQRT);
	g->init(size);

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		setRGBA(1, 0, 0, 1);
		if (ct[i].value)
		{// 미니맵 작업
			g->fillRect(20 * (i % TILEOFF_SQRT), 20 * (i / TILEOFF_SQRT), 20, 20);
		}
	}
	setRGBA(1, 1, 1, 1);
	Texture* tex = g->getTexture();
	imgMiniMap->addObject(tex);
	freeImage(tex);

	pop->addObject(imgMiniMap);
	pop->openPosition = iPointMake(500, 500);
	pop->closePosition = pop->openPosition;
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
	popMiniMap->paint(dt);
}

bool keyPopMiniMap(iKeyState stat, iPoint point)
{
	if (popMiniMap->bShow == false)
		return false;

	if (popMiniMap->stat != iPopupStatProc)
		return true;

	return false;
}

/////////////////////////////////////////////////////////

iPopup* popCombatMenu;
iImage** imgInvenButton;

void drawPopCombatMenuBefore(iPopup* me, iPoint p, float dt);

// 인벤토리 무기 양방향 링크리스트로 만들기
void createPopCombatMenu()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popCombatMenu = pop;
	
	imgInvenButton = (iImage**)malloc(sizeof(iImage*) * 2);

	iImage* imgInvenMenu = new iImage();
	Texture* texInvenMenu = createImage("assets/PlayerUI/inven menu.png");
	imgInvenMenu->addObject(texInvenMenu);
	freeImage(texInvenMenu);
	pop->addObject(imgInvenMenu);

	int i;
	for (i = 0; i < 2; i++)
		imgInvenButton[i] = new iImage();

	for (i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			Texture* texInvenButton = createImage("assets/PlayerUI/inven button%d.png", 2 - j);
			imgInvenButton[i]->addObject(texInvenButton);
			freeImage(texInvenButton);
		}
		imgInvenButton[i]->position = iPointMake(50, 50 + 150 * i);
		pop->addObject(imgInvenButton[i]);
	}


	pop->openPosition = iPointMake(50, 200);
	pop->closePosition = pop->openPosition;

	pop->methodDrawBefore = drawPopCombatMenuBefore;
}

void freePopCombatMenu()
{
	delete popCombatMenu;
	free(imgInvenButton);
}

void showPopCombatMenu(bool show)
{
	popCombatMenu->show(show);
}

//오픈메소드
void drawPopCombatMenuBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 2; i++)
		imgInvenButton[i]->setTexAtIndex(i == popCombatMenu->selected);
}

void drawPopCombatMenu(float dt)
{
	popCombatMenu->paint(dt);
}

bool keyPopCombatMenu(iKeyState stat, iPoint point)
{
	if (popCombatMenu->bShow == false)
		return false;

	if (popCombatMenu->stat != iPopupStatProc)
		return true;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popCombatMenu->selected;
		if (i == -1)
		{
			showPopInven1(false);

		}
		else if (i == 0)
		{
			showPopInven1(true);
		}
		else 
		{
			showPopInven1(false);
		}
		break;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgInvenButton[i]->touchRect(popCombatMenu->closePosition)))
			{
				j = i;
				break;
			}
		}

		popCombatMenu->selected = j;
		break;
	}
	case iKeyStateEnded:

		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////
//들고있는 무기 그리기
iPopup* popInven1;

void createPopInven1()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popInven1 = pop;

	iImage* imgWeapon = new iImage();
	Texture* texWeapon = createImage("assets/weapon/axe.png");
	imgWeapon->addObject(texWeapon);
	freeImage(texWeapon);

	imgWeapon->position = imgInvenButton[0]->position + iPointMake(150, 200);

	pop->addObject(imgWeapon);
}

void freePopInven1()
{
	delete popInven1;
}

void showPopInven1(bool show)
{
	popInven1->show(show);
}

void drawPopInven1(float dt)
{
	popInven1->paint(dt);
}

bool keyPopInven1(iKeyState stat, iPoint point)
{
	if (popInven1->bShow == false)
		return false;

	if (popInven1->stat != iPopupStatProc)
		return true;

	return false;
}
