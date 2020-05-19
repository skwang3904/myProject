#include "PlayerUI.h"

void loadPlayerUI()
{
	createPopHP();

	showPopHP(true);
}

void freePlayerUI()
{
	freePopHP();
}

void drawPlayerUI(float dt)
{
	drawPopHP(dt);
}

bool keyPlayerUI(iKeyState stat, iPoint point)
{
	if (keyPopHP(stat, point) ||
		keyPopCombat(stat,point))
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

	pop->openPosition = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePosition = iPointZero;

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
	delete imgHPgageGreen;
	delete imgHPgageRed;
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
}

/////////////////////////////////////////////////////////

iPopup* popCombat;

void createPopCombat()
{
}

void freePopCombat()
{
}

void showPopCombat(bool show)
{
}

void drawPopCombat(float dt)
{
}

bool keyPopCombat(iKeyState stat, iPoint point)
{
	return false;
	if (popCombat->bShow == false)
		return false;

	if (popCombat->stat != iPopupStatProc)
		return true;
}