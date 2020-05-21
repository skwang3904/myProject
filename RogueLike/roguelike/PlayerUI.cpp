#include "PlayerUI.h"

#include "Room.h"
#include "Stage.h"
#include "WMelee.h"

#include "EnemyStruct.h"

void loadPlayerUI()
{
	createPopHP();
	createPopCombatMenu();
	createPopMiniMap();

	showPopHP(true);
	showPopCombatMenu(true);
	showPopMiniMap(true);
}

void freePlayerUI()
{
	freePopHP();
	freePopCombatMenu();
	freePopMiniMap();
}

void drawPlayerUI(float dt)
{
	drawPopHP(dt);
	drawPopCombatMenu(dt);
	drawPopMiniMap(dt);
}

bool keyPlayerUI(iKeyState stat, iPoint point)
{
	if (keyPopHP(stat, point) ||
		keyPopCombatMenu(stat,point)||
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
iImage* imgMiniMap;

#define MINIMAPTILE 40
static int minitile = MINIMAPTILE;
Texture* refreshMiniMap()
{
	if (imgMiniMap->tex)
		freeImage(imgMiniMap->tex);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(minitile * TILEOFF_SQRT, minitile * TILEOFF_SQRT);
	g->init(size);

	for (int i = 0; i < TILEOFF_NUM; i++)
	{

		if (ct[i].value)
		{
			if (ct[i].tileOff + pc->camPosition == iPointZero) setRGBA(0, 1, 0, 1);
			else setRGBA(0.8, 0.8, 0.8, 1);

			g->fillRect(minitile * (i % TILEOFF_SQRT), minitile * (i / TILEOFF_SQRT), minitile, minitile);
			setRGBA(0, 0, 0, 1);
			g->drawRect(minitile * (i % TILEOFF_SQRT), minitile * (i / TILEOFF_SQRT), minitile, minitile);

			if (ct[i].tileOff == ct[nextDoor].tileOff)
			{
				setRGBA(0, 0, 1, 1);
				g->fillRect(minitile * (i % TILEOFF_SQRT) + 10,  minitile * (i / TILEOFF_SQRT) + 10,
					minitile - 20, minitile - 20);
			}

			for (int j = 0; j < GOLEM_NUM; j++) // 몬스터표시
			{
				EnemyNomalGolem* enm = golems[j];
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
			if (a == i)
			{
				setRGBA(1, 0, 0, 1);
				g->fillRect(minitile * (i % TILEOFF_SQRT) + 10, minitile * (i / TILEOFF_SQRT) + 10,
					minitile - 20, minitile - 20);
			}
		}
	}
	setRGBA(1, 1, 1, 1);

	return  g->getTexture();
}

void createPopMiniMap()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popMiniMap = pop;
	
	imgMiniMap = new iImage();

	Texture* tex = refreshMiniMap();
	imgMiniMap->addObject(tex);
	//freeImage(tex); // 리테인카운터 감소시키면 삭제에러뜸

	pop->addObject(imgMiniMap);
	pop->openPosition = iPointMake(devSize.width - minitile * TILEOFF_SQRT, 100);
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
	if (getKeyStat(keyboard_r))
	{
		popMiniMap->closePosition = iPointMake(500, 100);
		minitile = 150;
	}
	else
	{
		popMiniMap->closePosition = iPointMake(devSize.width - minitile * TILEOFF_SQRT, 100);
		minitile = MINIMAPTILE;
	}

	imgMiniMap->tex = refreshMiniMap();
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

void drawCombat(float dt)
{
	iPoint p ;
	
	for (int i = 0; i < pc->weaponArray->count; i++)
	{
		p = iPointMake(1600 + 150 * (i % 2), 400 + +150 * (i / 2));

		PlayerMW* pw = (PlayerMW*)pc->weaponArray->objectAtIndex(i);
		pw->mw->img->setTexAtIndex(1);
		if (pw == pc->pmw)
		{
			setRGBA(0, 1, 0, 1);
			fillRect(p.x, p.y, pw->mw->img->tex->width, pw->mw->img->tex->height);
			setRGBA(1, 1, 1, 1);
		}
		drawImage(pw->mw->img->tex, p.x, p.y, TOP | LEFT);
		pw->mw->img->setTexAtIndex(0);
	}

}

void createPopCombatMenu()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	popCombatMenu = pop;

	iImage* imgCombatMenu = new iImage();
	Texture* texCombatMenu = createImage("assets/PlayerUI/inven button1.png");
	imgCombatMenu->addObject(texCombatMenu);
	//freeImage(texCombatMenu);

	imgCombatMenu->position = iPointMake(1600, 400);
	pop->addObject(imgCombatMenu);

	for (int i = 1; i < 8; i++)
	{
		iImage* imgCombatMenu1 = imgCombatMenu->copy();
		imgCombatMenu1->position = iPointMake(1600 + 150 * (i % 2), 400 + 150 * (i / 2));
		pop->addObject(imgCombatMenu1);
	}
}

void freePopCombatMenu()
{
	delete popCombatMenu;
}

void showPopCombatMenu(bool show)
{
	popCombatMenu->show(show);
}

void drawPopCombatMenu(float dt)
{
	//무기 그리기
	popCombatMenu->paint(dt);
	drawCombat(dt);
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

	}
	case iKeyStateMoved:
	{
		break;
	}
	case iKeyStateEnded:

		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////

void createPopItem()
{
}

void freePopItem()
{
}

void showPopItem(bool show)
{
}

void drawPopItem(float dt)
{
}

bool keyPopItem(iKeyState stat, iPoint point)
{
	return false;
}
