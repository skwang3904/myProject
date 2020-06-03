#pragma once

// 게임화면

#include "iStd.h"

#define SND_SWING 0
#define SND_JUMP 1
#define SND_FALL 2
#define SND_ENEMY_HIT 3
#define SND_ENEMY_DEAD 4

#define SET_DRAW_OFF pc->camPosition + setPos

enum CharAction {
	idle = 0,
	attacking,
	meleeAtk,
	rangeAtk,

	falling,
	evasion,
	walking,
	hurt,
	dying,
	dead,
};

void loadRgProc();
void freeRgProc();
void drawRgProc(float dt);
void keyRgProc(iKeyState stat, iPoint point);


/* 
	수정 & 추가 할 것
	구글 문서 참조 
*/