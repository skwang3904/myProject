#include "Player.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"


Player* pc;
iSort* sort;

Player::Player()
{
	sort = new iSort();
	weaponArray = new rgArray();

	createPlayerImage();
	initPlayerStat();
}

Player::~Player()
{
	delete sort;
	delete weaponArray;

	for (int i = 0; i < 10; i++)
	{
		if (img[i])
			delete img[i];
	}
	free(img);
}

void Player::initPlayerStat()
{
	hp =
	_hp = 
	prevHP = 100.0f;

	attackDmg =
	_attackDmg = 10.0f;

	attackSpeed =
	_attackSpeed = 1.0f;

	moveSpeed = 800.0f;

	act = idle;
	tileNumber = -1;

	playerPosition = iPointZero;
	camPosition = iPointZero;
	drawPos = setPos;

	viewVector = iPointMake(0, 1);
	headNum = 7;
	evasV = iPointZero;
	combatV = iPointMake(0, 1);
	combatAngleV = 270.0f;

	touchPlayer = iRectZero;

	weaponArray->addObject(&PWP[0]);
	pwp = &PWP[0];
	PWP[0].drop = false;
	pwpCount = 1;

	coin = 0;
}

uint8 Player::initPlayerPosition()
{
	uint8 pcTile = 0;
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		int a = random() % TILEOFF_NUM;
		if (maps[a]->rgTile != NULL)
		{
			playerPosition = maps[a]->tileOff + RGTILE_CENTER;

			camPosition = iPointZero - maps[a]->tileOff;
			drawPos = playerPosition + camPosition + setPos;
			tileNumber = a;
			break;
		}
	}
	return tileNumber;
}

void Player::createPlayerImage()
{
	iImage* imgRight = new iImage();
	Texture** texs = createDivideImage(10, 1, "assets/char/CharBodyR.png");	// 좌우 10개
	for (int i = 0; i < 10; i++)
	{
		imgRight->addObject(texs[i]);
		freeImage(texs[i]);
	}
	free(texs);
	imgRight->startAnimation();
	imgRight->_aniDt = 0.05f;
	imgRight->_repeatNum = 0;

	iImage* imgDown = new iImage();
	texs = createDivideImage(10, 1, "assets/char/CharBodyUD.png");	// 상하 10개
	for (int i = 0; i < 10; i++)
	{
		imgDown->addObject(texs[i]);
		freeImage(texs[i]);
	}
	free(texs);
	imgDown->startAnimation();
	imgDown->_aniDt = 0.05f;
	imgDown->_repeatNum = 0;

	iImage** imgHead = (iImage**)malloc(sizeof(iImage*) * 4);			// 머리 상하좌우
	texs = createDivideImage(8, 1, "assets/char/CharHead.png");		// 머리 4방향 각 2개
	for (int i = 0; i < 4; i++)
	{
		imgHead[i] = new iImage();
		for (int j = 0; j < 2; j++)
		{
			imgHead[i]->addObject(texs[2 * i + j]);
			freeImage(texs[2 * i + j]);
		}
	}
	free(texs);

	iImage* imgFall = new iImage();
	texs = createDivideImage(4, 1, "assets/char/CharJump.png");		// 낙하모션 4개
	for (int i = 0; i < 4; i++)
	{
		imgFall->addObject(texs[i]);
		freeImage(texs[i]);
	}
	free(texs);
	imgFall->_aniDt = 0.15f; // img 4개
	imgFall->_repeatNum = 1;
	imgFall->_selectedDt = imgFall->_aniDt * 4.0f;
	imgFall->selectedScale = 0.5f;

	iImage* imgEvasion = new iImage();
	texs = createDivideImage(4, 1, "assets/char/CharJump.png");	// 회피모션 4개
	for (int i = 0; i < 4; i++)
	{
		imgEvasion->addObject(texs[i]);
		freeImage(texs[i]);
	}
	free(texs);
	imgEvasion->_aniDt = EVASION_DURATION / 4.0f;
	imgEvasion->_repeatNum = 1;
	//imgEvasion->_selectedDt = imgEvasion->_aniDt * 4.0f;
	//imgEvasion->angle = 720.0f;

	img = (iImage**)malloc(sizeof(iImage*) * 10);
	// 0, 1 몸 정지모션
	img[0] = new iImage(); img[0]->addObject(imgRight->tex);
	img[1] = new iImage(); img[1]->addObject(imgDown->tex);

	// 2, 3 몸 애니메이션
	img[2] = imgRight;
	img[3] = imgDown;

	// 4 머리 애니메이션
	img[4] = imgHead[3];	//left
	img[5] = imgHead[1];	//right
	img[6] = imgHead[2];	//up
	img[7] = imgHead[0];	//down
	free(imgHead);

	img[8] = imgFall;

	img[9] = imgEvasion;
#if _DEBUG
	for (int i = 0; i < 10; i++)
	{
		if (img[i] == NULL)
			printf("player imgChar[%d] error", i);
	}
#endif
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
		//printf("dead\n");
	}

	sort->init();
	sort->add(pc->playerPosition.y + HALF_OF_TEX_HEIGHT/2.0f);
	if (pc->pwp->isMelee)
	{
		meleeWeapon* mw = (meleeWeapon*)pc->pwp->wp;
		sort->add(mw->combatPosition.y);
	}
	else
	{	
		//range
		//sort->add(pc->pwp->wp->combatPosition.y);
	}
	sort->update();

	for (int i = 0; i < sort->sdNum; i++) // 수정필요
	{
		if (sort->get(i) == 0)
			paint(dt);
		else
			combatDraw(dt);
	}

	choseWeapon(getKeyDown(keyboard_tab));
	rootCombat(getKeyDown(keyboard_i));
	dropCombat(dt, getKeyDown(keyboard_o));
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
	pwp->method(dt, iPointZero);
}

void Player::rootCombat(bool key)
{
	if (actionCheck(key))
		return;

	for (int i = 0; i < TOTAL_WP_NUM; i++)
	{
		PlayerWP* pwa = &PWP[i];
		if (pwa->isMelee)
		{
			meleeWeapon* mw = (meleeWeapon*)pwa->wp;
			if (pwa->drop)
			{
				if (containRect(touchPlayer, mw->hitBox))
				{
					weaponArray->addObject(pwa);
					pwp = pwa;
					pwa->pos = iPointZero;
					pwa->drop = false;
					pwpCount = weaponArray->count - 1;
					break;
				}
			}
		}
		else //range
		{
			;
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
		PlayerWP* pw = (PlayerWP*)weaponArray->objectAtIndex(i);
		if (pwp == pw)
		{
			weaponArray->remove(i);
			pw->drop = true;
			pw->pos = playerPosition + HALF_OF_TEX_POINT;
			pwpCount--;
			if (pwpCount < 0)
				pwpCount = weaponArray->count - 1;
			pwp = (PlayerWP*)weaponArray->objectAtIndex(pwpCount);
			break;
		}	
	}
}

void Player::choseWeapon(bool key)
{
	if (actionCheck(key))
		return;

	pwpCount--;
	if (pwpCount < 0)
		pwpCount = weaponArray->count - 1;
	pwp = (PlayerWP*)weaponArray->objectAtIndex(pwpCount);
}

void Player::setPlayerTile()
{
	int n = tileNumber;
	if (playerPosition.x + HALF_OF_TEX_WIDTH < maps[n]->tileOff.x)
	{
		tileNumber = n - 1;
		playerPosition.x -= HALF_OF_TEX_WIDTH;
	}
	else if (playerPosition.x + HALF_OF_TEX_WIDTH > maps[n]->tileOff.x + RGTILE_X * RGTILE_Width - 1)
	{
		tileNumber = n + 1;
		playerPosition.x += HALF_OF_TEX_WIDTH;
	}
	else if (playerPosition.y + HALF_OF_TEX_HEIGHT < maps[n]->tileOff.y)
	{
		tileNumber = n - TILEOFF_SQRT;
		playerPosition.y -= HALF_OF_TEX_HEIGHT;
	}
	else if (playerPosition.y + HALF_OF_TEX_HEIGHT > maps[n]->tileOff.y + RGTILE_Y * RGTILE_Height - 1)
	{
		tileNumber = n + TILEOFF_SQRT;
		playerPosition.y += HALF_OF_TEX_HEIGHT;
	}
	else
		printf("player setTile error\n");

	camPosition = iPointZero - maps[tileNumber]->tileOff;
}

void Player::drawtouchPlayer()
{
	iRect rt = iRectMake(playerPosition.x,
		playerPosition.y,
		HALF_OF_TEX_WIDTH * 2.0f, HALF_OF_TEX_HEIGHT * 2.0f);

	touchPlayer = rt;

	setRGBA(0, 0, 0, 0.5f);
	rt.origin += pc->camPosition + setPos;
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

}

void Player::paint(float dt)
{
	iPoint v = iPointZero;
	iPoint mv = pc->combatV;

	if (act == idle)
	{
		if (getKeyStat(keyboard_left)) //이동방향, 머리방향
		{
			v.x = -1;
			headNum = 4;

			mv.x = -1;
			mv.y = 0;
		}
		else if (getKeyStat(keyboard_right))
		{
			v.x = 1;
			headNum = 5;

			mv.x = 1;
			mv.y = 0;
		}
		if (getKeyStat(keyboard_up))
		{
			v.y = -1;
			headNum = 6;

			mv.x = 0;
			mv.y = -1;
		}
		else if (getKeyStat(keyboard_down))
		{
			v.y = 1;
			headNum = 7;

			mv.x = 0;
			mv.y = 1;
		}
		viewVector = v;

		if (mv != iPointZero);
		pc->combatV = mv;
	}

	bool ani = (v != iPointZero);
	if(ani)
		v /= iPointLength(v);
	iPoint mp = v * (moveSpeed * dt);

	iPoint t = maps[tileNumber]->tileOff;
	float x = playerPosition.x + HALF_OF_TEX_WIDTH;
	float y = playerPosition.y + HALF_OF_TEX_HEIGHT;
	if (x < t.x ||
		x > t.x + RGTILE_X * RGTILE_Width - 1 ||
		y < t.y ||
		y > t.y + RGTILE_Y * RGTILE_Height - 1)
	{
		setPlayerTile();
		return;
	}
	
	MapTile* tile = maps[tileNumber];
	if (act != evasion)
		if (fallCheck(tile, dt))
			return;

	if (evasionPlayer(tile, dt))
		return;

	wallCheck(false, tile, pc->playerPosition, mp, HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);

	img[headNum]->setTexAtIndex(pc->act == attacking ? true : false);
	drawPos = playerPosition + camPosition + setPos;

	//히트박스 표시-------------------------------
	drawtouchPlayer();

	if (mv.x)
	{
		img[2 * ani + 0]->reverse = (mv.x < 0 ? REVERSE_WIDTH : REVERSE_NONE);
		img[2 * ani + 0]->paint(dt, drawPos);
	}
	else if (mv.y)
		img[2 * ani + 1]->paint(dt, drawPos);

	drawImage(img[headNum]->tex, drawPos.x + HALF_OF_TEX_WIDTH, drawPos.y + 3,
		0, 0, img[headNum]->tex->width, img[headNum]->tex->height,
		VCENTER | HCENTER, 1.0f, 1.0f,
		img[headNum]->location, img[headNum]->angle, REVERSE_NONE);



}

bool Player::evasionPlayer(MapTile* tile, float dt)
{
	// 회피
	// 회피중 무적

	if (getKeyDown(keyboard_space) && act != evasion && act != attacking)
	{
		if (viewVector != iPointZero)
		{
			img[Player_imgEvasion]->startAnimation();
			act = evasion;

			audioPlay(SND_JUMP);
			
			evasV = viewVector / iPointLength(viewVector);
		}
	}

	if (act != evasion)
		return false;


	if (img[Player_imgEvasion]->animation == true)
	{	
		iPoint mp = evasV * (EVASION_DISTANCE * dt);
		wallCheck(false, tile, playerPosition, mp, HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);

		//drawPos = playerPosition + camPosition + setPos;
		iPoint dp = playerPosition + iPointMake(0,-30) + camPosition + setPos;
		iPoint p = iPointMake(dp.x - HALF_OF_TEX_WIDTH/2,
			dp.y - HALF_OF_TEX_HEIGHT);
		
		//if (evasV.x < 0)
		//{
		//	img[9]->reverseRotate = false;
		//	img[9]->location = 2;
		//}
		//else if (evasV.x > 0)
		//{
		//	img[9]->reverseRotate = true;
		//	img[9]->location = 2;
		//}
		//if (evasV.y < 0)
		//{
		//	img[9]->reverseRotate = false;
		//	img[9]->location = 1;
		//}
		//else if (evasV.y > 0)
		//{
		//	img[9]->reverseRotate = true;
		//	img[9]->location = 1;
		//}
		//img[9]->selected = true;

		//히트박스 표시-------------------------------
		drawtouchPlayer();

		img[Player_imgEvasion]->paint(dt, p);
		
		return true;
	}

	else if (img[Player_imgEvasion]->animation == false)
	{
		act = idle;
		evasV = iPointZero;

		return false;
	}
	
	return false;
}