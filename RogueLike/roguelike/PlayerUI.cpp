#include "PlayerUI.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"

#include "EnemyStruct.h"
#include "EnemyData.h"

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
		"assets/PlayerUI/Bar_Fill_Red.png"
	};
	iImage** imgs = (iImage**)malloc(sizeof(iImage*) * 6);
	iImage* img;
	Texture* tex;
	for (int i = 0; i < 6; i++)
	{
		tex = createImage(strPath[i]);
		img = new iImage();
		img->addObject(tex);
		freeImage(tex);
		if (i == 0)			img->position = iPointMake(100, 30);
		else if (i == 1)	img->position = imgs[0]->center() - iPointMake(tex->width / 2, tex->height / 2 + 5);
		else if (i == 2)	img->position = imgs[0]->position + iPointMake(140, 30);
		else if (i == 3)	img->position = imgs[2]->center() - iPointMake(tex->width / 2, tex->height / 2 + 5);
		else				img->position = imgs[3]->position;
		pop->addObject(img);
		imgs[i] = img;
	}
	imgHPgageGreen = imgs[4];
	imgHPgageRed = imgs[5];

	free(imgs);
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
		imgHPgageGreen->imgRatioX = linear(delta / 2, prevHP, pc->hp) / pc->_hp;
		
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

//---------------------------------------------------------------------------

iPopup* popMiniMap;
Texture* texMiniMap;
iImage* imgMiniMap;

#define MINIMAPTILE 40
static int minitile = MINIMAPTILE;
void refreshMiniMap()
{
	fbo->bind(texMiniMap);
	fbo->clear(0, 0, 0, 0);

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (ct[i].value)
		{
			if (i == pc->tileNumber) setRGBA(0, 1, 0, 1); // 플레이어 표시
			else setRGBA(0.8, 0.8, 0.8, 1);

			iPoint mp = iPointMake(minitile * (i % TILEOFF_SQRT), minitile * (i / TILEOFF_SQRT));
			fillRect(mp.x, mp.y, minitile, minitile);
			setRGBA(0, 0, 0, 1);
			drawRect(mp.x, mp.y, minitile, minitile);

			if (i == nextDoor) // 다음스테이지 표시
			{
				setRGBA(0, 0, 1, 1);
				fillRect(mp.x + 10, mp.y + 10,
					minitile - 20, minitile - 20);
			}

			for (int k = 0; k < 1; k++)
			{
				for (int j = 0; j < GOLEM_NUM; j++) // 몬스터표시
				{
					MonsterData* eg = golems[j];
					if (eg->tileNumber == i)
					{
						iPoint p = iPointMake((fabsf(eg->enemyPos.x - tileOffSet[i].x) / (RGTILE_X * RGTILE_Width)),
							(fabsf(eg->enemyPos.y - tileOffSet[i].y) / (RGTILE_Y * RGTILE_Height)));
						setRGBA(1, 0.5, 0, 1);
						fillRect(mp.x + minitile * p.x,
							mp.y + minitile * p.y,
							5, 5);
					}
				}
			}

			int a = golemEletes[0]->tileNumber;
			if (a == i && golemEletes[0]->hp > 0.0f)
			{
				setRGBA(1, 0, 0, 1);
				fillRect(mp.x + 10, mp.y + 10,
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
			imgMiniMap->position = iPointMake(-1.0f * (devSize.width / 2) - minitile * TILEOFF_SQRT, 100);
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
	iPoint p = iPointMake(1600, 400);
	//---------------------------------------------------------------------------
	// menu

	iPopup* pop = new iPopup(iPopupStyleNone);
	popCombatMenu = pop;

	iImage* img = new iImage();
	Texture* tex = createImage("assets/PlayerUI/inven button1.png");
	img->addObject(tex);
	freeImage(tex);
	img->position = p;
	pop->addObject(img);

	for (int i = 1; i < 8; i++)
	{
		iImage* tmp = img->copy();
		tmp->position = p +iPointMake(150 * (i % 2), 150 * (i / 2));
		pop->addObject(tmp);
	}

	//---------------------------------------------------------------------------
	// info
	pop = new iPopup(iPopupStyleNone);
	popCombatInfo = pop;

	img = new iImage();
	img->addObject(setCombatInfo(0));
	img->position = p + iPointMake(-350, 0);
	pop->addObject(img);
	imgCombatInfo = img;

	img = new iImage();
	tex = createImage("assets/PlayerUI/check button.png");
	img->addObject(tex);
	freeImage(tex);
	img->position = imgCombatInfo->position + iPointMake(250, 0);
	pop->addObject(img);
	imgInfoButten = img;

	pop->_showDt = 0.01f;
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
			iPoint	p = iPointMake(1600 + 150 * (i % 2), 400 + 150 * (i / 2));
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
		"assets/icons/moveSpeed.png"
	};
	iPoint pos[5] = { {20, 170}, {20, 270}, {20, 370}, {20, 470}, {20, 570}, };
	float ratio[5] = { 0.25f,0.25f,0.25f,0.25f,0.25f, };

	for (int i = 0; i < 5; i++)
	{
		iImage* img = new iImage();
		Texture* tex = createImage(strPath[i]);
		img->addObject(tex);
		freeImage(tex);
		img->position = pos[i];
		img->ratio = ratio[i];
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
	numberFont->drawFont(150, 200, TOP | LEFT, 1, 1, 1, "%.0f", pc->hp);
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
	iImage* img = new iImage();
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
