#include "PlayerUI.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"

#include "EnemyStruct.h"

void testui() // test
{
	for (int i = 0; i < 1000; i++)
	{
		printf("i  = %d\n", i);
		createPopMiniMap();
		freePopMiniMap();
	}

}
//------------------------

void loadPlayerUI()
{
	createPopHP();
	//testui();
	createPopMiniMap();
	createPopCombatMenu();
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
	freePopMiniMap();
	freePopCombatMenu();
	freePopItem();
	freePopStageNum();
}

void drawPlayerUI(float dt)
{
	drawPopHP(dt);
	drawPopMiniMap(dt);
	drawPopCombatMenu(dt);
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

	const char* strPath[6] = {
		"assets/PlayerUI/Icon Bar.png",
		"assets/PlayerUI/heart.png",
		"assets/PlayerUI/Bar_BG.png",
		"assets/PlayerUI/Bar.png",
		"assets/PlayerUI/Bar_Fill_Green.png",
		"assets/PlayerUI/Bar_Fill_Red.png",
	};

	iPoint iconPoint = iPointMake(100, 30);
	iPoint barPoint = iconPoint + iPointMake(140, 0);
	iImage* imgs[6];
	for (int i = 0; i < 6; i++)
	{
		imgs[i] = new iImage();
		Texture* tex = createImage(strPath[i]);
		imgs[i]->addObject(tex);
		freeImage(tex);

		if (i == 0)
		{
			imgs[i]->position = iconPoint;
		}
		else if (i == 1)
		{
			imgs[i]->position = imgs[0]->position + iPointMake((imgs[0]->tex->width - imgs[1]->tex->width) / 2,
				(imgs[0]->tex->height - imgs[1]->tex->height) / 2 + 5);
		}
		else if (i == 2)
		{
			imgs[i]->position = barPoint;
		}
		else if (i == 3)
		{
			imgs[i]->position = imgs[2]->position + iPointMake((imgs[2]->tex->width - imgs[3]->tex->width) / 2,
				(imgs[2]->tex->height - imgs[3]->tex->height) / 2);
		}
		else if (i == 4)
		{
			imgs[i]->position = imgs[3]->position;
			imgHPgageGreen = imgs[i];
		}
		else if (i == 5)
		{
			imgs[i]->position = imgs[3]->position;
			imgHPgageRed = imgs[i];
		}

		pop->addObject(imgs[i]);
	}
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
Texture* texMiniMap;
iImage* imgMiniMap;

#define MINIMAPTILE 40
static int minitile = MINIMAPTILE;
static uint8 prevTileNum = 0;
void refreshMiniMap()
{
	fbo->bind(texMiniMap);
	fbo->clear(0, 0, 0, 1);

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (ct[i].value)
		{
			if (i == pc->tileNumber) setRGBA(0, 1, 0, 1); // 플레이어 표시
			else setRGBA(0.8, 0.8, 0.8, 1);

			fillRect(minitile * (i % TILEOFF_SQRT), minitile * (i / TILEOFF_SQRT), minitile, minitile);
			setRGBA(0, 0, 0, 1);
			drawRect(minitile * (i % TILEOFF_SQRT), minitile * (i / TILEOFF_SQRT), minitile, minitile);

			if (i == nextDoor) // 다음스테이지 표시
			{
				setRGBA(0, 0, 1, 1);
				fillRect(minitile * (i % TILEOFF_SQRT) + 10,  minitile * (i / TILEOFF_SQRT) + 10,
					minitile - 20, minitile - 20);
			}

			for (int k = 0; k < 1; k++)
			{
				for (int j = 0; j < GOLEM_NUM; j++) // 몬스터표시
				{
					EnemyGolem* eg = &golems[k][j];
					if (eg->tileNumber == i)
					{
						iPoint p = iPointMake((fabsf(eg->golemPos.x - tileOffSet[i].x) / (RGTILE_X * RGTILE_Width)),
							(fabsf(eg->golemPos.y - tileOffSet[i].y) / (RGTILE_Y * RGTILE_Height)));
						setRGBA(1, 0.5, 0, 1);
						fillRect(minitile * (i % TILEOFF_SQRT) + minitile * p.x,
							minitile * (i / TILEOFF_SQRT) + minitile * p.y,
							5, 5);
					}
				}
			}

			int a = golemEletes[0][0].tileNumber;
			if (a == i && golemEletes[0][0].hp > 0.0f)
			{
				setRGBA(1, 0, 0, 1);
				fillRect(minitile * (i % TILEOFF_SQRT) + 10, minitile * (i / TILEOFF_SQRT) + 10,
					minitile - 20, minitile - 20);
			}
		}
	}

	fbo->unbind();
	setRGBA(1, 1, 1, 1);
}

void createPopMiniMap()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popMiniMap = pop;

	texMiniMap = createTexture(devSize.width, devSize.height);
	iImage* img = new iImage();
	imgMiniMap = img;
	img->addObject(texMiniMap);
	freeImage(texMiniMap);

	img->reverse = REVERSE_HEIGHT;

	pop->addObject(img);

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
	}

	refreshMiniMap();
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

	iPoint p = iPointMake(1600, 400);
	imgCombatMenu->position = p;
	pop->addObject(imgCombatMenu);

	for (int i = 1; i < 8; i++)
	{
		iImage* imgCombatMenu1 = imgCombatMenu->copy();
		imgCombatMenu1->position = p + iPointMake(150 * (i % 2), 150 * (i / 2));
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
	imgCombatInfo->tex = setCombatInfo(0);
	imgCombatInfo->position = p + iPointMake(-350, 0);

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
		{
			showPopCombatInfo(false);
			break;
		}

		if (i == 100) // 닫기버튼
		{
			showPopCombatInfo(false);
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
			iPoint	p = iPointMake(1600 + 150 * (i % 2),400 + 150 * (i / 2));
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

void createPopItem()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popItem = pop;

	const char* strPath[5] = {
		"assets/icons/hp.png",
		"assets/icons/Money.png",
		"assets/icons/attackDmg.png",
		"assets/icons/attackSpeed.png",
		"assets/icons/moveSpeed.png",
	};

	for (int i = 0; i < 5; i++)
	{
		iImage* img = new iImage();
		Texture* tex = createImage(strPath[i]);
		img->addObject(tex);
		freeImage(tex);

		img->position = iPointMake(20, 170 + 100 * i);
		img->ratio = 0.25f;

		pop->addObject(img);
	}
}

void freePopItem()
{
	delete popItem;
}

void showPopItem(bool show)
{
	popItem->show(show);
}

void cointest()
{
	numberFont->drawFont(150, 200, TOP | LEFT, 1, 1, 1, "%d", pc->hp);
	numberFont->drawFont(150, 300, TOP | LEFT, 1, 1, 1, "%d", pc->coin);
	numberFont->drawFont(150, 400, TOP | LEFT, 1, 1, 1, "%.0f", pc->attackDmg);
	numberFont->drawFont(150, 500, TOP | LEFT, 1, 1, 1, "%.1f", pc->attackSpeed);
	numberFont->drawFont(150, 600, TOP | LEFT, 1, 1, 1, "%.1f", pc->moveSpeed);
}

void drawPopItem(float dt)
{
	popItem->paint(dt);

	cointest();
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
