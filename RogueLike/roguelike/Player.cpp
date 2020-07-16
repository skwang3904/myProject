#include "Player.h"

#include "Room.h"

#include "Weapon.h"
#include "WMelee.h"
#include "WRange.h"


Player* pc;
iSort* sort;
iPoint setPos;

Player::Player()
{
	sort = new iSort();
	weaponArray = new rgArray();

	createPlayerImage();
	initPlayerStat();
	initPlayerPosition();

	setPos = iPointMake(devSize.width / 2 - RGTILE_X * RGTILE_Width / 2.0f,
	devSize.height / 2 - RGTILE_Y * RGTILE_Height / 2.0f);
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

	viewVector = iPointMake(0, 1);
	headNum = 7;
	evasV = iPointZero;
	combatV = iPointMake(0, 1);
	combatAngleV = 270.0f;

	touchPlayer = iRectZero;

	weaponArray->addObject(&PWP[MELEE_NUM]);
	pwp = &PWP[MELEE_NUM];
	PWP[MELEE_NUM].drop = false;
	pwpCount = 1;

	coin = 0;
}

void Player::initPlayerPosition()
{
	uint8 pcTile = 0;
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		int a = random() % TILEOFF_NUM;
		if (maps[a]->rgTile != NULL)
		{
			playerPosition = maps[a]->tileOff + RGTILE_CENTER;
			camPosition = iPointZero - maps[a]->tileOff;
			tileNumber = a;
			break;
		}
	}
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
}

bool Player::actionCheck(bool key)
{
	if (key == false || pc->act != idle)
		return true;
	return false;
}

void Player::paint(float dt)
{
	if (hp <= 0.0f)
	{
		//gameover
		//printf("dead\n");
	}

	sort->init();
	sort->add(pc->playerPosition.y + HALF_OF_TEX_HEIGHT / 2.0f);
	if (pc->pwp->isMelee)
	{
		meleeWeapon* mw = (meleeWeapon*)pc->pwp->wp;
		sort->add(mw->combatPosition.y);
	}
	else
	{	
		//range
		rangeWeapon* rw = (rangeWeapon*)pc->pwp->wp;
		sort->add(rw->combatPosition.y);
	}
	sort->update();

	for (int i = 0; i < sort->sdNum; i++) // 수정필요
	{
		if (sort->get(i) == 0)
			drawPlayer(dt);
		else
			combatDraw(dt);
	}

	choseWeapon(getKeyDown(keyboard_tab));
	rootCombat(getKeyDown(keyboard_i));
	dropCombat(dt, getKeyDown(keyboard_o));
}

void Player::combatDraw(float dt)
{
	if (act == evasion || act == falling)
		return;
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
			rangeWeapon* rw = (rangeWeapon*)pwa->wp;
			if (pwa->drop)
			{
				if (containRect(touchPlayer, rw->hitBox))
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
	}
}

void Player::dropCombat(float dt, bool key)
{
	if (weaponArray->count < 2 || actionCheck(key))
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
	rt.origin += SET_DRAW_OFF;
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
}

void Player::drawPlayer(float dt)
{
	iPoint v = iPointZero;
	iPoint mv = pc->combatV;

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

	bool ani = (v != iPointZero);
	if (ani)
		v /= iPointLength(v);
	iPoint mp = v * (moveSpeed * dt) * (act == attacking ? 0.1f : 1.0f);

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
	iPoint drawPos = playerPosition + SET_DRAW_OFF;

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
	if (getKeyDown(keyboard_space) && act == idle)
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

	if (img[Player_imgEvasion]->animation)
	{	
		iPoint mp = evasV * (EVASION_DISTANCE * dt);
		wallCheck(false, tile, playerPosition, mp, HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);

		iPoint dp = playerPosition + iPointMake(0,-30) + SET_DRAW_OFF;
		iPoint p = iPointMake(dp.x - HALF_OF_TEX_WIDTH/2,
			dp.y - HALF_OF_TEX_HEIGHT);
		
		//히트박스 표시-------------------------------
		drawtouchPlayer();

		img[Player_imgEvasion]->paint(dt, p);
		
		return true;
	}
	else //if (img[Player_imgEvasion]->animation == false)
	{
		act = idle;
		evasV = iPointZero;

		return false;
	}
	
	return false;
}

float findMoveTile(MapTile* tile, int x, int y)
{	// 수정 필요
	float min = 0xffff;
	MapTile* t = tile;
	if (t->rgTile[RGTILE_X * y + x] != MOVETILE)
		return min;

	float distance = iPointLength(
		(pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT))
		- t->tileOff + iPointMake(RGTILE_Width * x + RGTILE_Width / 2,
			RGTILE_Height * y + RGTILE_Height / 2));

	if (min > distance)
		return distance;
	return min;
}

void findMoveTile(MapTile* tile, iPoint& moveTileNum)
{
	MapTile* t = tile;
	int x = (int)(pc->playerPosition.x - t->tileOff.x + HALF_OF_TEX_WIDTH) / RGTILE_Width;
	int y = (int)(pc->playerPosition.y - t->tileOff.y + HALF_OF_TEX_HEIGHT) / RGTILE_Height;

	float minD = 0xffff;
	float dis;

	int pcX = x;
	int pcY = y;

	for (int i = 0; ; i++)
	{
		// x-1, y-1,	x, y-1,		x+1, y-1
		// x-1, y					x+1, y
		// x-1, y+1		x, y+1		x+1, y+1

		dis = findMoveTile(tile, x - i, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y - i;
		}
		dis = findMoveTile(tile, x - i, y);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y;
		}
		dis = findMoveTile(tile, x - i, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y + i;
		}

		dis = findMoveTile(tile, x, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x;
			pcY = y - i;
		}
		dis = findMoveTile(tile, x, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x;
			pcY = y + i;
		}

		dis = findMoveTile(tile, x + i, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y - i;
		}
		dis = findMoveTile(tile, x + i, y);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y;
		}

		dis = findMoveTile(tile, x + i, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y + i;
		}

		if (minD != 0xffff)
			break;
	}

	iPoint p = iPointMake(t->tileOff.x + RGTILE_Width * pcX,
		t->tileOff.y + RGTILE_Height * pcY);

	if (p.x < pc->playerPosition.x + HALF_OF_TEX_WIDTH)
		p -= iPointMake(HALF_OF_TEX_WIDTH, 0);
	else
		p += iPointMake(HALF_OF_TEX_WIDTH, 0);
	if (p.y < pc->playerPosition.y + HALF_OF_TEX_HEIGHT)
		p -= iPointMake(0, HALF_OF_TEX_HEIGHT);
	else
		p += iPointMake(HALF_OF_TEX_HEIGHT, 0);

	moveTileNum = p;
}

bool Player::fallCheck(MapTile* tile, float dt)
{
	// 임시 - 낭떨어지에 진입시 가장 가까이있는 타일로 이동  - 어색함
	// 라이프 감소
	// 잠시 무적
	if (tile->rgTile == NULL)
		return false;

	MapTile* t = tile;

	int x = (int)(pc->playerPosition.x - t->tileOff.x + HALF_OF_TEX_WIDTH) / RGTILE_Width;
	int y = (int)(pc->playerPosition.y - t->tileOff.y + HALF_OF_TEX_HEIGHT) / RGTILE_Height;
	iPoint moveTileNum = iPointZero;
	if (t->rgTile[RGTILE_X * y + x] == FALLTILE)
	{
		if (pc->act != falling)
		{
			pc->img[Player_imgFall]->startAnimation();
			pc->img[Player_imgFall]->selected = true;
			findMoveTile(t, moveTileNum);

			audioPlay(SND_FALL);
		}

		if (pc->img[Player_imgFall]->animation == false)
		{
			pc->img[Player_imgFall]->selected = false;
			pc->act = idle;
			pc->playerPosition = moveTileNum;

			return false;
		}

		pc->act = falling;
		iPoint p = pc->playerPosition - t->tileOff + setPos
			- iPointMake(HALF_OF_TEX_WIDTH / 2, HALF_OF_TEX_HEIGHT / 2);

		pc->img[Player_imgFall]->selected = true;
		pc->img[Player_imgFall]->paint(dt, p);

		return true;
	}

	return false;
}