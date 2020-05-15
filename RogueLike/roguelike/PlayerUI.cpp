#include "PlayerUI.h"

void loadPlayerUI()
{
	createPopUI();

	showPopUI(true);
}

void freePlayerUI()
{
	freePopUI();
}

void drawPlayerUI(float dt)
{
	drawPopUI(dt);
}

void keyPlayerUI(iKeyState stat, iPoint point)
{
	if (keyPopUI(stat, point))
		return;
}

/////////////////////////////////////////////////////////

iPopup* popUI;

void createPopUI()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	
	


}

void freePopUI()
{
}

void showPopUI(bool show)
{
	popUI->show(show);
}

void drawPopUI(float dt)
{
}

bool keyPopUI(iKeyState stat, iPoint point)
{
	if (popUI->bShow == false)
		return false;

	if (popUI->stat != iPopupStatProc)
		return true;
}
