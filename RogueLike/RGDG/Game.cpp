#include "Game.h"

#include "Common.h"
#include "Loading.h"
#include "Intro.h"
#include "Proc.h"


void loadGame()
{

	//--------------------------------------------------------
	// sound // itunes로 변환해서 사용
	AudioInfo ai[11] = {
	{"assets/snd/sfx_HammerAttack.wav", false, 1.0f},
	{"assets/snd/sfx_SpearAttack.wav", false, 1.0f},
	{"assets/snd/sfx_CyclonAttack.wav", true, 1.0f},
	{"assets/snd/sfx_EnemyHit.wav", false, 1.0f},
	{"assets/snd/sfx_EnemyDeath.wav", false, 1.0f},
	{"assets/snd/sfx_NextStageMove.wav", false, 0.3f},
	{"assets/snd/sfx_ItemPickUP.wav", false, 1.0f},
	{"assets/snd/sfx_Menu MouseOver.wav", false, 0.7f},
	{"assets/snd/sfx_Menu Selection.wav", false, 0.7f},
	{"assets/snd/bgm_gameMusic.wav", true, 0.3f},
	{"assets/snd/bgm_IntroMusic.wav", true, 0.3f},
	};

	loadAudio(ai, 11);

#if 1
	loadIntro();
	gamestat = gamestat_intro;
	audioPlay(AUDIO_INTRO);
#else
	loadProc();
	gamestat = gamestat_proc;
	audioPlay(AUDIO_GAMEMUSIC);
#endif

	createPopOption();
}

void freeGame()
{
	switch (gamestat) {
	case gamestat_intro:	freeIntro();	break;
	//case gamestat_menu:	freeMenu();		break;
	case gamestat_proc:		freeProc();		break;
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


