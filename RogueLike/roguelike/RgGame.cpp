#include "RgGame.h"

#include "RgIntro.h"
#include "RgMenu.h"
#include "RgProc.h"

int gamestat;

void loadRgGame()
{
#if 1
	loadRgProc();
	gamestat = gs_proc;
#else
#endif

	iPoint p = iPointRotate(iPointMake(5, 0), iPointMake(3,0), 90);
	printf("x = %.2f, y = %.2f\n",p.x,p.y );
}

void freeRgGame()
{
	switch (gamestat)	{
	case gs_intro:	freeRgIntro();	break;
	case gs_menu:	freeRgMenu();	break;
	case gs_proc:	freeRgProc();	break;
	}
}

void drawRgGame(float dt)
{
	switch (gamestat) {
	case gs_intro:	drawRgIntro(dt);	break;
	case gs_menu:	drawRgMenu(dt);		break;
	case gs_proc:	drawRgProc(dt);		break;
	}
}

void keyRgGame(iKeyState stat, iPoint point)
{
	switch (gamestat) {
	case gs_intro:	keyRgIntro(stat, point);	break;
	case gs_menu:	keyRgMenu(stat, point);		break;
	case gs_proc:	keyRgProc(stat, point);		break;
	}
}
