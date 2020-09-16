#include "Game.h"

#include "Common.h"
#include "Loading.h"
#include "Intro.h"
#include "Proc.h"


void loadGame()
{
#if 1
	loadIntro();
	gamestat = gamestat_intro;
#else
	loadProc();
	gamestat = gamestat_proc;
#endif

	//--------------------------------------------------------
	// sound // itunes로 변환해서 사용
	AudioInfo ai[6] = {
	{"assets/snd/swing.wav", false, 0.4f},
	{"assets/snd/jump.wav", false, 0.1f},
	{"assets/snd/falling.wav", false, 0.3f},
	{"assets/snd/enemy-hit.wav", false, 1.0f},
	{"assets/snd/killEnemy.wav", false, 0.3f},
	{"assets/snd/legacy-loop.wav", true, 0.15f},
	};

	loadAudio(ai, 6);

	createPopOption();
}

void freeGame()
{
	switch (gamestat) {
	case gamestat_intro:freeIntro();	break;
	//case gamestat_menu:	freeMenu();	break;
	case gamestat_proc:	freeProc();	break;
	}

	freeAudio();

	freePopOption();
}

void drawGame(float dt)
{
	float _dt = dt;
	if (nowLoading())
		dt = 0.0f;

	switch (gamestat) {
	case gamestat_intro:	drawIntro(dt);	break;
	//case gamestat_menu:	drawMenu(dt);		break;
	case gamestat_proc:		drawProc(dt);		break;
	}

	drawPopOption(dt);
	drawLoading(_dt);
}

void keyGame(iKeyState stat, iPoint point)
{
	if (keyLoading(stat, point) ||
		keyPopOption(stat, point))
		return;

	//if (stat == iKeyStateBegan)
	//	zoomLib(point, 2.0f);

	switch (gamestat) {
	case gamestat_intro:	keyIntro(stat, point);	break;
	//case gamestat_menu:	keyMenu(stat, point);	break;
	case gamestat_proc:		keyProc(stat, point);	break;
	}

}


