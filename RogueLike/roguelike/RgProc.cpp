#include "RgProc.h"

#include "Room.h"
#include "RgTile.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"

#include "EnemyComposition.h"
#include "Stage.h"

#include "PlayerUI.h"

void loadRgProc()
{
	stage++;
	createTileSet();
	loadRoomTile();

	weapon = Weapon::instance();

	pc = Player::instance();
	pc->createPlayerImage();
	pc->initPlayerStat();

	createEnemy();

	createStage(stage);

	/////////////////////////////////////////////////////////
	// pop
	loadPlayerUI();



	/////////////////////////////////////////////////////////
	// sound
	AudioInfo ai[4] = {
	{"assets/snd/swing.wav", false, 0.4f},
	{"assets/snd/jump.wav", false, 0.2f},
	{"assets/snd/falling.wav", false, 0.5f},
	{"assets/snd/enemy-hit.wav", false, 0.7f},
	{"assets/snd/killEnemy.wav", false, 0.3f},
	};

	loadAudio(ai, 4);
}

void freeRgProc()
{
	delete pc;
	delete weapon;

	freeEnemy();
	freeTileSet();
	freeRoomTile();

	/////////////////////////////////////////////////////////
	// pop
	freePlayerUI();

	freeAudio();
}

void drawRgProc(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);
	static float delta = 0.0f;
	static bool stagetest = false;

	int test = 0;
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemys[i]->hp < 0.1f)
			test++;
		if (i == ENEMY_NUM - 1 && test == ENEMY_NUM)
		{
			// stage animation
			stage++;
			createStage(stage);

			stagetest = true;
		}
	}

	if (nextStage)
	{
		stagetest = true;
		popHP->show(false);
	}

	if (delta > 1.0f)
	{
		stage++;
		createStage(stage);
		stagetest = false;
		delta = 0.0f;
		nextStage = false;

		popHP->show(true);
	}

	drawRoomTile(dt);
	passTileAnimation(dt);

	if (stagetest)
	{
		delta += dt;
		return;
	}

	if (passAni)
		return;
	

	// 몬스터 draw

	if (pc->hp < 0.1f)
		return;

	drawEnemy(dt);

	// 특정위치에서 (ex 상자) 등장
	for (int i = 0; i < meleeNum; i++)
	{
		if(pc->method == _method[i])
			continue;
		_method[i](dt, true, weapon->wDropPos[i]);
	}
	
	pc->drawPlayer(dt);
	//printf("%.2f\n", pc->hp);
	//printf("x = %.2f, y = %.2f\n", pc->playerPosition.x, pc->playerPosition.y);

	drawNextDoor(dt);


	/////////////////////////////////////////////////////////
	// pop
	drawPlayerUI(dt);
}

void keyRgProc(iKeyState stat, iPoint point)
{
	if (keyPlayerUI(stat, point))
		return;


	switch (stat)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		break;
	case iKeyStateEnded:
		break;

	}
}
