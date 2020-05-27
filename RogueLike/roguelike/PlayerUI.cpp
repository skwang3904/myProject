#include "PlayerUI.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"

#include "EnemyStruct.h"

void loadPlayerUI()
{
	createPopHP();
	createPopCombatMenu();
	createPopMiniMap();
	createPopItem();
	createPopStageNum();

	showPopHP(true);
	showPopCombatMenu(true);
	showPopMiniMap(true);
	showPopItem(true);
	showPopStageNum(true);
}

void freePlayerUI()
{
	freePopHP();
	freePopCombatMenu();
	freePopMiniMap();
	freePopItem();
	freePopStageNum();
}

void drawPlayerUI(float dt)
{
	drawPopHP(dt);
	drawPopCombatMenu(dt);
	drawPopMiniMap(dt);
	drawPopItem(dt);
	drawPopStageNum(dt);

	numberFont->drawFont(devSize.width / 2 + 200, 30, TOP | LEFT, 1, 1, 1, "%d", stage);
}

bool keyPlayerUI(iKeyState stat, iPoint point)
{
	if (keyPopHP(stat, point) ||
		keyPopCombatMenu(stat, point) ||
		keyPopMiniMap(stat, point) ||
		keyPopItem(stat, point) ||
		keyPopStageNum(stat, point))
		return true;
	return false;
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
	static float delta = 0.0f;
	if (pc->hp < 0.0f)
		pc->hp = 0.0f;

	if (prevHP != pc->hp)
	{
		delta += dt;
		imgHPgageGreen->imgRatioX = linear(delta / 2.0f, prevHP, pc->hp) / pc->_hp;
		
		if (delta > 2.0f)
		{
			delta = 0.0f;
			prevHP = pc->hp;
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
iImage* imgMiniMap;

#define MINIMAPTILE 40
static int minitile = MINIMAPTILE;
static uint8 prevTileNum = 0;
void refreshMiniMap()
{
	if (imgMiniMap->tex)
		freeImage(imgMiniMap->tex);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(devSize.width,devSize.height);
	g->init(size);

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (ct[i].value)
		{
			if (i == pc->tileNumber) setRGBA(0, 1, 0, 1); // 플레이어 표시
			else setRGBA(0.8, 0.8, 0.8, 1);

			g->fillRect(minitile * (i % TILEOFF_SQRT), minitile * (i / TILEOFF_SQRT), minitile, minitile);
			setRGBA(0, 0, 0, 1);
			g->drawRect(minitile * (i % TILEOFF_SQRT), minitile * (i / TILEOFF_SQRT), minitile, minitile);

			if (i == nextDoor) // 다음스테이지 표시
			{
				setRGBA(0, 0, 1, 1);
				g->fillRect(minitile * (i % TILEOFF_SQRT) + 10,  minitile * (i / TILEOFF_SQRT) + 10,
					minitile - 20, minitile - 20);
			}

			for (int j = 0; j < GOLEM_NUM; j++) // 몬스터표시
			{
				EnemyGolem* enm = golems[j];
				if (enm->tileNumber == i)
				{				
					iPoint p = iPointMake((fabsf(enm->golemPos.x - tileOffSet[i].x) / (RGTILE_X * RGTILE_Width)),
						(fabsf(enm->golemPos.y - tileOffSet[i].y) / (RGTILE_Y * RGTILE_Height)));
					setRGBA(1, 0.5, 0, 1);
					g->fillRect(minitile * (i % TILEOFF_SQRT) + minitile * p.x,
						minitile * (i / TILEOFF_SQRT) + minitile * p.y,
						5, 5);
				}
			}

			int a = golemEletes[0]->tileNumber;
			if (a == i && golemEletes[0]->hp > 0.0f)
			{
				setRGBA(1, 0, 0, 1);
				g->fillRect(minitile * (i % TILEOFF_SQRT) + 10, minitile * (i / TILEOFF_SQRT) + 10,
					minitile - 20, minitile - 20);
			}
		}
	}
	setRGBA(1, 1, 1, 1);

	Texture* tex = g->getTexture();
	imgMiniMap->addObject(tex);
	freeImage(tex);
}

void createPopMiniMap()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popMiniMap = pop;
	imgMiniMap = new iImage();
	refreshMiniMap();

	pop->addObject(imgMiniMap);
	pop->openPosition = iPointMake(devSize.width - minitile * (TILEOFF_SQRT + 5), 100);
	pop->closePosition = pop->openPosition;

	prevTileNum = pc->tileNumber;
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
	if (getKeyDown(keyboard_r))
	{
		if (minitile == MINIMAPTILE)
		{
			imgMiniMap->position = iPointMake(-1.0f * (devSize.width / 2.0f) - minitile * TILEOFF_SQRT, 100);
			minitile = 150;
		}
		else if (minitile == 150)
		{
			imgMiniMap->position = iPointZero;
			minitile = MINIMAPTILE;
		}
		refreshMiniMap();
	}

	if (prevTileNum != pc->tileNumber)
	{
		refreshMiniMap();
		prevTileNum = pc->tileNumber;
	}

	//refreshMiniMap();
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

//---------------------------------------------------------------------------

iPopup* popCombatMenu;
iPopup* popCombatInfo;
iImage* imgCombatInfo;
iImage* imgInfoButten;

void drawCombat(float dt)
{
	iPoint p;

	PlayerWP* pw;

	for (int i = 0; i < pc->weaponArray->count; i++)
	{
		p = iPointMake(1600 + 150 * (i % 2), 400 + +150 * (i / 2));

		pw = (PlayerWP*)pc->weaponArray->objectAtIndex(i);
		if (pw->isMelee)
		{
			meleeWeapon* mw = (meleeWeapon*)pw->wp;
			mw->img->setTexAtIndex(1);
			if (pw == pc->pwp)
			{
				if (popCombatMenu->selected == i) setRGBA(0, 0, 1, 1);
				else setRGBA(0, 1, 0, 1);
				fillRect(p.x, p.y, mw->img->tex->width, mw->img->tex->height);
				setRGBA(1, 1, 1, 1);
			}

			drawImage(mw->img->tex, p.x, p.y, TOP | LEFT);
			mw->img->setTexAtIndex(0);
		}
		else //range
		{ 
			//if (pw == pc->pwp)
			//{
			//	if (popCombatMenu->selected == i) setRGBA(0, 0, 1, 1);
			//	else setRGBA(0, 1, 0, 1);
			//	fillRect(p.x, p.y, mw->img->tex->width, pw->wp->img->tex->height);
			//	setRGBA(1, 1, 1, 1);
			//}

			//drawImage(pw->mw->img->tex, p.x, p.y, TOP | LEFT);
			//pw->mw->img->setTexAtIndex(0);
		}
	}
}

Texture* setCombatInfo(int i)
{
	PlayerWP* pw = (PlayerWP*)pc->weaponArray->objectAtIndex(i);
	if (pw->isMelee)
	{
		meleeWeapon* mw = (meleeWeapon*)pw->wp;
		return mw->infoImg->tex;
	}
	else // range
	{
		return NULL;
	}
}

void createPopCombatMenu()
{
	//---------------------------------------------------------------------------
	// menu
	iPopup* pop = new iPopup(iPopupStyleNone);
	popCombatMenu = pop;

	iImage* imgCombatMenu = new iImage();
	Texture* texCombatMenu = createImage("assets/PlayerUI/inven button1.png");
	imgCombatMenu->addObject(texCombatMenu);
	freeImage(texCombatMenu);

	imgCombatMenu->position = CombatMenu_Pos;
	pop->addObject(imgCombatMenu);

	for (int i = 1; i < 8; i++)
	{
		iImage* imgCombatMenu1 = imgCombatMenu->copy();
		imgCombatMenu1->position = CombatMenu_Pos +iPointMake(150 * (i % 2), 150 * (i / 2));
		pop->addObject(imgCombatMenu1);
	}

	//---------------------------------------------------------------------------
	// info
	iPopup* popInfo = new iPopup(iPopupStyleNone);
	popCombatInfo = popInfo;

	iImage* imgButten = new iImage();
	imgInfoButten = imgButten;

	Texture* texButten = createImage("assets/PlayerUI/check button.png");
	imgButten->addObject(texButten);
	freeImage(texButten);

	imgCombatInfo = new iImage();
	//imgCombatInfo->tex = setCombatInfo(0);
	imgCombatInfo->position = CombatMenu_Pos + iPointMake(-350, 0);

	imgButten->position = imgCombatInfo->position + iPointMake(250, 0);

	popInfo->addObject(imgCombatInfo);
	popInfo->addObject(imgButten);

	popInfo->_showDt = 0.01f;
}

void freePopCombatMenu()
{
	delete popCombatMenu;
	delete popCombatInfo;
}

void showPopCombatMenu(bool show)
{
	popCombatMenu->show(show);
}

void showPopCombatInfo(bool show)
{
	popCombatInfo->show(show);
}

void drawPopCombatMenu(float dt)
{
	popCombatMenu->paint(dt);
	drawCombat(dt);

	popCombatInfo->paint(dt);
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
			break;
		if (i == 100)
		{
			popCombatInfo->show(false);
			break;
		}
		imgCombatInfo->tex = setCombatInfo(i);
		showPopCombatInfo(true);
		break;
	}
	case iKeyStateMoved:
	{
		for (int i = 0; i < pc->weaponArray->count; i++)
		{
			iPoint	p = CombatMenu_Pos + iPointMake(150 * (i % 2), 150 * (i / 2));
			iRect rt = iRectMake(p.x, p.y, 128, 128);
			if (containPoint(point, rt))
			{
				j = i;
				break;
			}
		}

		if (containPoint(point, imgInfoButten->touchRect(popCombatInfo->closePosition)))
			j = 100;

		popCombatMenu->selected = j;
		break;
	}
	case iKeyStateEnded:

		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------------

iPopup* popItem;
iImage* coinimgtest;

void createPopItem()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popItem = pop;

	iImage* imgHP = new iImage();
	Texture* texHP = createImage("assets/item/heart.png");

	iImage* imgCoin = new iImage();
	Texture* texCoin = createImage("assets/item/coin.png");

	iImage* imgGemRed = new iImage();
	Texture* texGemRed = createImage("assets/item/gemRed.png");

	iImage* imgGemBlue = new iImage();
	Texture* texGemBlue = createImage("assets/item/gemBlue.png");

	iImage* imgGemGreen = new iImage();
	Texture* texGemGreen = createImage("assets/item/gemGreen.png");

	imgHP->addObject(texHP);
	imgCoin->addObject(texCoin);
	imgGemRed->addObject(texGemRed);
	imgGemBlue->addObject(texGemBlue);
	imgGemGreen->addObject(texGemGreen);

	freeImage(texHP);
	freeImage(texCoin);
	freeImage(texGemRed);
	freeImage(texGemBlue);
	freeImage(texGemGreen);

	imgHP->position =		iPointMake(50, 150);
	imgCoin->position =		iPointMake(50, 250);
	imgGemRed->position =	iPointMake(50, 350);
	imgGemBlue->position =	iPointMake(50, 450);
	imgGemGreen->position = iPointMake(50, 550);

	pop->addObject(imgHP);
	pop->addObject(imgCoin);
	pop->addObject(imgGemRed);
	pop->addObject(imgGemBlue);
	pop->addObject(imgGemGreen);
}

void freePopItem()
{
	delete popItem;
	delete coinimgtest;
}

void showPopItem(bool show)
{
	popItem->show(show);
}

void cointest()
{
	if (coinimgtest)
		delete coinimgtest;

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(1024, 1024);
	g->init(size);

	setStringSize(50);
	setStringRGBA(0, 1, 0, 1);
	setStringBorder(1);

	g->drawString(200, 200, TOP | LEFT, "%.1f", pc->hp);

	g->drawString(200, 300, TOP | LEFT, "%d", pc->coin);

	g->drawString(200, 400, TOP | LEFT, "%.1f", pc->attackDmg);

	g->drawString(200, 500, TOP | LEFT, "%.1f", pc->attackSpeed);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	coinimgtest = img;
}

void drawPopItem(float dt)
{
	popItem->paint(dt);

	cointest();
	coinimgtest->paint(dt, iPointZero, REVERSE_NONE);
}

bool keyPopItem(iKeyState stat, iPoint point)
{
	if (popItem->bShow == false)
		return false;

	if (popItem->stat != iPopupStatProc)
		return true;

	return false;
}

/////////////////////////////////////////////////////////

iPopup* popStageNum;

void createPopStageNum()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popStageNum = pop;

	iImage* img = new iImage();

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(400, 100);
	g->init(size);

	setRGBA(1, 1, 1, 1);
	g->fillRect(0, 0, size.width, size.height, 30);
	setRGBA(1, 1, 1, 1);

	setStringSize(50);
	setStringRGBA(0, 1, 1, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "STAGE");

	Texture* tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);

	img->position = iPointMake(devSize.width / 2 -100, 0);
	pop->addObject(img);
}

void freePopStageNum()
{
	delete popStageNum;
}

void showPopStageNum(bool show)
{
	popStageNum->show(show);
}

void drawPopStageNum(float dt)
{
	popStageNum->paint(dt);
}

bool keyPopStageNum(iKeyState stat, iPoint point)
{
	if (popStageNum->bShow == false)
		return false;

	if (popStageNum->stat != iPopupStatProc)
		return true;

	return false;
}
