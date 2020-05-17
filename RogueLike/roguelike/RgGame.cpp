#include "RgGame.h"

#include "RgIntro.h"
#include "RgMenu.h"
#include "RgProc.h"

int gamestat;

#define myTest 0

void loadRgGame()
{
#if 1
	loadRgProc();
	gamestat = gs_proc;
#else
#endif

	//--------------------------------------------------------
	// sound
	AudioInfo ai[5] = {
	{"assets/snd/swing.wav", false, 0.4f},
	{"assets/snd/jump.wav", false, 0.2f},
	{"assets/snd/falling.wav", false, 0.5f},
	{"assets/snd/enemy-hit.wav", false, 0.7f},
	{"assets/snd/killEnemy.wav", false, 0.3f},
	};

	loadAudio(ai, 5);
}

void freeRgGame()
{
	switch (gamestat)	{
	case gs_intro:	freeRgIntro();	break;
	case gs_menu:	freeRgMenu();	break;
	case gs_proc:	freeRgProc();	break;
	}

	freeAudio();
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
	if (stat == iKeyStateBegan)
		zoomLib(point, 2.0f);

	switch (gamestat) {
	case gs_intro:	keyRgIntro(stat, point);	break;
	case gs_menu:	keyRgMenu(stat, point);		break;
	case gs_proc:	keyRgProc(stat, point);		break;
	}
}