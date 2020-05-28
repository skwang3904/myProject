#include "RgGame.h"

#include "RgIntro.h"
#include "RgMenu.h"

#include "loading.h"

#include "Stage.h"
#include "PlayerUI.h"

int gamestat;

void loadRgGame()
{
	//for (int i = 0; i < 100; i++)
	//{
	//	loadRgProc();
	//	freeRgProc();
	//}

#if 0
	loadRgProc();
	gamestat = gs_proc;
#else
	loadRgIntro();
	gamestat = gs_intro;

	//loadRgProc();

#endif

	//--------------------------------------------------------
	// sound // itunes로 변환해서 사용
	AudioInfo ai[6] = {
	{"assets/snd/swing.wav", false, 0.4f},
	{"assets/snd/jump.wav", false, 0.2f},
	{"assets/snd/falling.wav", false, 0.3f},
	{"assets/snd/enemy-hit.wav", false, 1.0f},
	{"assets/snd/killEnemy.wav", false, 0.3f},
	{"assets/snd/legacy-loop.wav", true, 0.1f},
	};

	loadAudio(ai, 6);

	//audioPlay(5);
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

void curtainTile();
void drawRgGame(float dt)
{
	switch (gamestat) {
	case gs_intro:	drawRgIntro(dt);	break;
	case gs_menu:	drawRgMenu(dt);		break;
	case gs_proc:	
	{
		drawRgProc(dt);		
		curtainTile();
		drawPlayerUI(dt);

		if (nextStage)
			nextStageAni(dt);
		break;
	}
	}

	drawLoading(dt);
}

void keyRgGame(iKeyState stat, iPoint point)
{
	if (keyLoading(stat, point))
		return;
	//if (stat == iKeyStateBegan)
	//	zoomLib(point, 2.0f);

	switch (gamestat) {
	case gs_intro:	keyRgIntro(stat, point);	break;
	case gs_menu:	keyRgMenu(stat, point);		break;
	case gs_proc:	keyRgProc(stat, point);		break;
	}
}


void curtainTile() // 화면가리개
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0,
		devSize.width, (devSize.height - RGTILE_Y * RGTILE_Height) / 2.0f);
	fillRect((devSize.width + RGTILE_X * RGTILE_Width) / 2.0f, 0,
		(devSize.width - RGTILE_X * RGTILE_Width) / 2.0f, devSize.height);
	fillRect(0, (devSize.height + RGTILE_Y * RGTILE_Height) / 2.0f,
		devSize.width, (devSize.height - RGTILE_Y * RGTILE_Height) / 2.0f);
	fillRect(0, 0,
		(devSize.width - RGTILE_X * RGTILE_Width) / 2.0f, devSize.height);
	setRGBA(1, 1, 1, 1);
}