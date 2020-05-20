#include "Player.h"

#include "Room.h"

#include "Weapon.h"
#include "WMelee.h"

#include "Stage.h"

Player* pc;
Texture** texsRight;
Texture** texsDown;
Texture** texsHead;
Texture** texsFall;
Texture** texsEvasion;

void freeWeapon(void* data)
{
	PlayerMW* pmw = (PlayerMW*)data;
	freeMeleeWeapon(pmw);
}

iSort* sort;
Player::Player()
{
	sort = new iSort();
	weaponArray = new rgArray(freeWeapon);
	initPlayerStat();
}

Player::~Player()
{
	delete sort;

	for (int i = 0; i < 10; i++)
		free(texsRight[i]);
	free(texsRight);

	for (int i = 0; i < 10; i++)
		free(texsDown[i]);
	free(texsDown);

	for (int i = 0; i < 8; i++)
		free(texsHead[i]);
	free(texsHead);

	for (int i = 0; i < 4; i++)
		free(texsFall[i]);
	free(texsFall);

	for (int i = 0; i < 4; i++)
		free(texsEvasion[i]);
	free(texsEvasion);


	free(img);
}

void Player::initPlayerStat()
{
	hp =
	_hp = 100.0f;

	attackDmg =
	_attackDmg = 10.0f;

	attackSpeed =
	_attackSpeed = 0.5f;

	moveSpeed = 500.0f;

	act = idle;

	playerPosition = iPointZero;
	camPosition = iPointZero;
	drawPos = setPos;


	viewVector = iPointMake(0, 1);

	touchPlayer = iRectZero;

	weaponArray->addObject(&PMW[0]);
	pmw = &PMW[0];
	PMW[0].drop = false;
	pmwCount = 1;
}

int Player::initPlayerPosition()
{
	int pcTile = 0;
	for (int i = 15; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile != NULL)
		{
			playerPosition = maps[i]->tileOff + RGTILE_CENTER;

			camPosition = iPointZero - maps[i]->tileOff;
			drawPos = camPosition + setPos;
			pcTile = i;
			break;
		}
	}
	return pcTile;
}

void Player::createPlayerImage()
{
	iImage** imgChar = (iImage**)malloc(sizeof(iImage*) * 10);

	iImage* imgRight = new iImage();
	texsRight = createDivideImage(10, 1, "assets/char/CharBodyR.png");	// 좌우 10개

	iImage* imgDown = new iImage();
	texsDown = createDivideImage(10, 1, "assets/char/CharBodyUD.png");	// 상하 10개

	iImage** imgHead = (iImage**)malloc(sizeof(iImage*) * 4);			// 머리 상하좌우
	texsHead = createDivideImage(8, 1, "assets/char/CharHead.png");		// 머리 4방향 각 2개

	iImage* imgFall = new iImage();
	texsFall = createDivideImage(4, 1, "assets/char/CharJump.png");		// 낙하모션 4개

	iImage* imgEvasion = new iImage();
	texsEvasion = createDivideImage(4, 1, "assets/char/CharJump.png");	// 회피모션 4개

	for (int i = 0; i < 10; i++)
		imgRight->addObject(texsRight[i]);

	for (int i = 0; i < 10; i++)
		imgDown->addObject(texsDown[i]);

	for (int i = 0; i < 4; i++)
	{
		imgHead[i] = new iImage();
		for (int j = 0; j < 2; j++)
			imgHead[i]->addObject(texsHead[2 * i + j]);
	}

	for (int i = 0; i < 4; i++)
		imgFall->addObject(texsFall[i]);

	for (int i = 0; i < 4; i++)
		imgEvasion->addObject(texsEvasion[i]);

	// 0, 1 몸 정지모션
	imgChar[0] = new iImage(); imgChar[0]->addObject(imgRight->tex);
	imgChar[1] = new iImage(); imgChar[1]->addObject(imgDown->tex);

	// 2, 3 몸 애니메이션
	imgChar[2] = imgRight;
	imgChar[3] = imgDown;

	// 4 머리 애니메이션
	imgChar[4] = imgHead[3];	//left
	imgChar[5] = imgHead[1];	//right
	imgChar[6] = imgHead[2];	//up
	imgChar[7] = imgHead[0];	//down

	imgChar[8] = imgFall;

	imgChar[9] = imgEvasion;

	for (int i = 0; i < 10; i++)
	{
		if (imgChar[i] == NULL)
			printf("player imgChar[%d] error", i);
	}
	// 생성 
	//----------------------------
	// 설정

	imgRight->startAnimation();
	imgRight->_aniDt = 0.05f;
	imgRight->_repeatNum= 0;

	imgDown->startAnimation();
	imgDown->_aniDt = 0.05f;
	imgDown->_repeatNum = 0;
	
	imgFall->_aniDt = 0.15f; // img 4개
	imgFall->_repeatNum = 1;
	imgFall->_selectedDt = imgFall->_aniDt * 4.0f;
	imgFall->selectedScale = 0.5f;


	imgEvasion->_aniDt = 0.1f;
	imgEvasion->_repeatNum = 1;
	imgEvasion->_selectedDt = imgEvasion->_aniDt * 4.0f;
	imgEvasion->angle = 720.0f;

	this->img = imgChar;


	//retaincount조절
	for (int i = 0; i < 10; i++)
		freeImage(texsRight[i]);

	for (int i = 0; i < 10; i++)
		freeImage(texsDown[i]);

	for (int i = 0; i < 8; i++)
		freeImage(texsHead[i]);

	for (int i = 0; i < 4; i++)
		freeImage(texsFall[i]);

	for (int i = 0; i < 4; i++)
		freeImage(texsEvasion[i]);

	freeImage(imgChar[0]->tex);
	freeImage(imgChar[1]->tex);

}

bool Player::actionCheck(bool key)
{
	if (key == false || pc->act != idle)
		return true;
	return false;
}

void Player::drawPlayer(float dt)
{
	if (hp <= 0.0f)
	{
		//gameover
		printf("dead\n");
	}


	sort->init();
	sort->add(pc->playerPosition.y + HALF_OF_TEX_HEIGHT * 2);
	sort->add(pc->pmw->mw->combatPosition.y);
	sort->update();

	for (int i = 0; i < sort->sdNum; i++)
	{
		if (sort->get(i) == 0)
			movePlayer(dt);
		else
			combatDraw(dt);
	}
	//combatDraw(dt);
	//movePlayer(dt);
	rootCombat(getKeyDown(keyboard_i));
	dropCombat(dt, getKeyDown(keyboard_o));
	choseWeapon();

}

void Player::showHpBar(float dt) // 임시
{
	setRGBA(0, 0, 0, 1);
	fillRect(drawPos.x, drawPos.y - 30, HALF_OF_TEX_WIDTH * 2, 15);
	setRGBA(0, 1, 0, 1);
	fillRect(drawPos.x, drawPos.y - 30, HALF_OF_TEX_WIDTH * 2 * hp / _hp, 15);
	setRGBA(1, 1, 1, 1);
}

void Player::combatDraw(float dt)
{
	pmw->method(dt, iPointZero);
}

void Player::rootCombat(bool key)
{
	if (actionCheck(key))
		return;

	for (int i = 0; i < MELEE_NUM; i++)
	{
		PlayerMW* pw = &PMW[i];
		if (containRect(touchPlayer, pw->mw->hitBox))
		{
			if (pw->drop)
			{
				weaponArray->addObject(pw);
				pmw = pw;
				pw->pos = iPointZero;
				pw->drop = false;
				pmwCount++;
				break;
			}
		}
	}
}

void Player::dropCombat(float dt, bool key)
{
	if (weaponArray->count < 2)
		return;
	if (actionCheck(key))
		return;
	
	int i, j, k;
	for (i = 0; i < weaponArray->count; i++)
	{
		PlayerMW* pw = (PlayerMW*)weaponArray->objectAtIndex(i);
		if (pmw == pw)
		{
			weaponArray->remove(i);
			pw->drop = true;
			pw->pos = playerPosition;
			pmwCount--;
			if (pmwCount < 0)
				pmwCount = weaponArray->count - 1;
			pmw = (PlayerMW*)weaponArray->objectAtIndex(pmwCount-1);
			break;
		}	
	}
}

void Player::choseWeapon()
{
	if (getKeyDown(keyboard_tab))
	{
		pmwCount--;
		if (pmwCount < 0)
			pmwCount = weaponArray->count - 1;
		pmw = (PlayerMW*)weaponArray->objectAtIndex(pmwCount-1);
		printf("pmwC %d\n", pmwCount);
	}
}

MapTile* playerTileOffSet(MapTile* tile)
{
	MapTile* t = tile;
	float min = 0xffffff;

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (pc->playerPosition.x > maps[i]->tileOff.x &&
			pc->playerPosition.y > maps[i]->tileOff.y)
		{
			if (iPointLength(pc->playerPosition - maps[i]->tileOff) < min)
			{
				min = iPointLength(pc->playerPosition - maps[i]->tileOff);
				t = maps[i];
			}
		}
	}
	return t;
}

void Player::movePlayer(float dt)
{
	static iPoint vvv = iPointMake(0,1);
	iPoint v = iPointZero;
	static int ch= 7;

	if (act == idle)
	{
		if (getKeyStat(keyboard_left)) //이동방향, 머리방향
		{
			v.x = -1;
			ch = 4;

			vvv.x = -1;
			vvv.y = 0;
		}
		else if (getKeyStat(keyboard_right))
		{
			v.x = 1;
			ch = 5;

			vvv.x = 1;
			vvv.y = 0;
		}
		if (getKeyStat(keyboard_up))
		{
			v.y = -1;
			ch = 6;

			vvv.x = 0;
			vvv.y = -1;
		}
		else if (getKeyStat(keyboard_down))
		{
			v.y = 1;
			ch = 7;

			vvv.x = 0;
			vvv.y = 1;
		}
		viewVector = v;
	}

	bool ani = (v != iPointZero);
	if(ani)
		v /= iPointLength(v);
	iPoint mp = v * (moveSpeed * dt);

	static MapTile* tile = maps[12];
	float x = playerPosition.x + HALF_OF_TEX_WIDTH;
	float y = playerPosition.y + HALF_OF_TEX_HEIGHT;
	if (x < tile->tileOff.x ||
		x > tile->tileOff.x + RGTILE_X * RGTILE_Width - 1 ||
		y < tile->tileOff.y ||
		y > tile->tileOff.y + RGTILE_Y * RGTILE_Height - 1)
	{
		if (x < tile->tileOff.x)
			playerPosition.x -= HALF_OF_TEX_WIDTH * 2;
		else if (x > tile->tileOff.x + RGTILE_X * RGTILE_Width - 1)
			playerPosition.x += HALF_OF_TEX_WIDTH;
		else if (y < tile->tileOff.y)
			playerPosition.y -= HALF_OF_TEX_HEIGHT * 2;
		else if (y > tile->tileOff.y + RGTILE_Y * HALF_OF_TEX_HEIGHT - 1)
			playerPosition.y += HALF_OF_TEX_HEIGHT;

		tile = playerTileOffSet(tile);
		camPosition = iPointZero - tile->tileOff;
		return;
	}

	if (act != evasion)
		if (fallCheck(tile, dt))
			return;

	if (evasionPlayer(tile, dt))
		return;

	wallCheck(false, tile, pc->playerPosition, mp, HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);

	img[ch]->setTexAtIndex(pc->act == attacking ? true : false);
	drawPos = playerPosition + camPosition + setPos;

	if (vvv.x)
		img[2 * ani + 0]->paint(dt, drawPos, vvv.x < 0 ? REVERSE_WIDTH : REVERSE_NONE);
	else if (vvv.y)
		img[2 * ani + 1]->paint(dt, drawPos , REVERSE_NONE);

	drawImage(img[ch]->tex, drawPos.x + HALF_OF_TEX_WIDTH, drawPos.y,
		0, 0, img[ch]->tex->width, img[ch]->tex->height,
		VCENTER | HCENTER, 1.0f, 1.0f,
		img[ch]->location, img[ch]->angle, REVERSE_NONE);

	iRect rt = iRectMake(drawPos.x, drawPos.y,
		HALF_OF_TEX_WIDTH * 2.0f, HALF_OF_TEX_HEIGHT * 2.0f);

	//히트박스 표시-------------------------------
	setRGBA(0, 1, 0, 0.3f);
	//fillRect(rt);
	setRGBA(1, 1, 1, 1);

	touchPlayer = rt;

	camPosition = iPointZero - tile->tileOff;
}

bool Player::evasionPlayer(MapTile* tile, float dt)
{
	// 회피
	// 회피중 무적

	if (getKeyDown(keyboard_space) && act != evasion && act != attacking)
	{
		if (viewVector != iPointZero)
		{
			img[9]->startAnimation();
			act = evasion;

			audioPlay(SND_JUMP);
		}
	}

	if (act != evasion)
		return false;

	static iPoint v = iPointZero;

	if (img[9]->animation == true)
	{	
		if (viewVector != iPointZero && v == iPointZero)
			v = viewVector / iPointLength(viewVector);

		iPoint mp = v * (moveSpeed / 2.0f * dt);
		wallCheck(false, tile, playerPosition, mp, HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);

		drawPos = playerPosition + camPosition + setPos;
		iPoint p = iPointMake(drawPos.x - HALF_OF_TEX_WIDTH/2,
			drawPos.y - HALF_OF_TEX_HEIGHT);
		
		if (v.x < 0)
		{
			img[9]->reverseRotate = false;
			img[9]->location = 2;
		}
		else if (v.x > 0)
		{
			img[9]->reverseRotate = true;
			img[9]->location = 2;
		}
		if (v.y < 0)
		{
			img[9]->reverseRotate = false;
			img[9]->location = 1;
		}
		else if (v.y > 0)
		{
			img[9]->reverseRotate = true;
			img[9]->location = 1;
		}

		img[9]->selected = true;
		img[9]->paint(dt, p, REVERSE_NONE);
		

		return true;
	}

	else if (img[9]->animation == false)
	{
		act = idle;
		v = iPointZero;

		return false;
	}
	
	return false;
}