#pragma once

#include "iStd.h"

void loadGame();
void freeGame();
void drawGame(float dt);
void keyGame(iKeyState stat, iPoint point);



/*
	AudioInfo ai[11] = {
	{"assets/snd/sfx_HammerAttack.wav", false, 1.0f},
	{"assets/snd/sfx_SpearAttack.wav", false, 1.0f},
	{"assets/snd/sfx_CyclonAttack.wav", true, 1.0f},
	{"assets/snd/sfx_EnemyHit.wav", false, 1.0f},
	{"assets/snd/sfx_EnemyDeath.wav", false, 1.0f},
	{"assets/snd/sfx_NextStageMove.wav", false, 1.0f},
	{"assets/snd/sfx_ItemPickUP.wav", false, 1.0f},
	{"assets/snd/sfx_Menu MouseOver.wav", false, 1.0f},
	{"assets/snd/sfx_Menu Selection.wav", false, 1.0f},
	{"assets/snd/bgm_gameMusic.wav", true, 1.0f},
	{"assets/snd/bgm_IntroMusic.wav", true, 1.0f},
	};
*/