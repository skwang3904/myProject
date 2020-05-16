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

iPopup* popHP;

void createPopUI()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	
	


}

void freePopUI()
{
}

void showPopUI(bool show)
{
	popHP->show(show);
}

void drawPopUI(float dt)
{
}

bool keyPopUI(iKeyState stat, iPoint point)
{
	if (popHP->bShow == false)
		return false;

	if (popHP->stat != iPopupStatProc)
		return true;
}
