#include "Player.h"

#include "Room.h"

#include "Weapon.h"
#include "WMelee.h"
#include "WRange.h"

Player* pc;
iSort* sort;
// 캐릭터 이미지 수정
Player::Player()
{
	sort = new iSort();
	weaponArray = new rgArray();

	createPlayerImage();
	initPlayerStat();
	initPlayerPosition();
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

	act = Act_idle;
	tileNumber = -1;

	playerPosition = iPointZero;
	camPosition = iPointZero;

	viewVector = iPointMake(0, 1);
	headNum = 7;
	evasV = iPointZero;
	combatV = iPointMake(0, 1);
	combatPos = iPointZero;
	combatAngleV = 180.0f;

	touchPlayer = iRectZero;

#define STARTWEAPON 0
	weaponArray->addObject(&PWP[STARTWEAPON]);
	pwp = &PWP[STARTWEAPON];
	PWP[STARTWEAPON].drop = false;
	pwpCount = 1;

	coin = 0;
}

void Player::initPlayerPosition()
{
	uint8 pcTile = 0;
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		int a = random() % TILEOFF_NUM;
		if (maps[a]->rgTile[0] != 0 && maps[a]->state == MapType_Nomal)
		{
			playerPosition = maps[a]->tileOff + RGTILE_CENTER;
			camPosition = iPointZero - maps[a]->tileOff;
			tileNumber = a;
			break;
		}
	}
	combatPos = playerPosition + iPointMake(img[0]->tex->width / 2.0f * img[0]->ratio * 0.5f,
											img[0]->tex->height / 2.0f * img[0]->ratio * 1.5f);
}

extern int playerCharNumber;
void Player::createPlayerImage()
{
	int i, j;

	const char* strChar[10] = {
	"assets/char/CharBodyR.png",
	"assets/char/CharBodyUD.png",
	"assets/char/CharHead.png",
	"assets/char/CharJump.png",
	"assets/char/CharJump.png",

	"assets/char/CharBodyR2.png",
	"assets/char/CharBodyUD2.png",
	"assets/char/CharHead2.png",
	"assets/char/CharJump2.png",
	"assets/char/CharJump2.png",
	};

	int s = playerCharNumber;

	iPoint size = iPointMake(64, 40) * 2.0f;
	Texture* tex;

	iImage* imgRight = new iImage();
	Texture** texs = createDivideImage(10, 1, strChar[5 * s + 0]);	// 좌우 10개
	for (i = 0; i < 10; i++)
	{
		tex = createTexture(size.x, size.y);
		fbo->bind(tex);
		fbo->setSize(size);

		Texture* t = texs[i];
		drawImage(t, 0, 0,
			0, 0, t->width, t->height,
			TOP | LEFT, size.x / t->width, size.y / t->height,
			2, 0, REVERSE_HEIGHT);
		freeImage(t);

		fbo->backSize();
		fbo->unbind();

		imgRight->addObject(tex);
		freeImage(tex);
	}
	free(texs);
	imgRight->startAnimation();
	imgRight->_aniDt = 0.05f;
	imgRight->_repeatNum = 0;

	iImage* imgDown = new iImage();
	texs = createDivideImage(10, 1, strChar[5 * s + 1]);	// 상하 10개
	for (i = 0; i < 10; i++)
	{
		tex = createTexture(size.x, size.y);
		fbo->bind(tex);
		fbo->setSize(size);

		Texture* t = texs[i];
		drawImage(t, 0, 0,
			0, 0, t->width, t->height,
			TOP | LEFT, size.x / t->width, size.y / t->height,
			2, 0, REVERSE_HEIGHT);
		freeImage(t);

		fbo->backSize();
		fbo->unbind();

		imgDown->addObject(tex);
		freeImage(tex);
	}
	free(texs);
	imgDown->startAnimation();
	imgDown->_aniDt = 0.05f;
	imgDown->_repeatNum = 0;

	size = iPointMake(60, 45) * 2.0f;

	iImage** imgHead = (iImage**)malloc(sizeof(iImage*) * 4);			// 머리 상하좌우
	texs = createDivideImage(8, 1, strChar[5 * s + 2]);		// 머리 4방향 각 2개
	for (i = 0; i < 4; i++)
	{
		imgHead[i] = new iImage();
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.x, size.y);
			fbo->bind(tex);
			fbo->setSize(size);

			Texture* t = texs[2 * i + j];
			drawImage(t, 0, 0,
				0, 0, t->width, t->height,
				TOP | LEFT, size.x / t->width, size.y / t->height,
				2, 0, REVERSE_HEIGHT);
			freeImage(t);

			fbo->backSize();
			fbo->unbind();

			imgHead[i]->addObject(tex);
			freeImage(tex);
		}
	}
	free(texs);

	size = iPointMake(97, 60) * 2.0f;

	iImage* imgFall = new iImage();
	texs = createDivideImage(4, 1, strChar[5 * s + 3]);		// 낙하모션 4개
	for (i = 0; i < 4; i++)
	{
		tex = createTexture(size.x, size.y);
		fbo->bind(tex);
		fbo->setSize(size);

		Texture* t = texs[i];
		drawImage(t, 0, 0,
			0, 0, t->width, t->height,
			TOP | LEFT, size.x / t->width, size.y / t->height,
			2, 0, REVERSE_HEIGHT);
		freeImage(t);

		fbo->backSize();
		fbo->unbind();

		imgFall->addObject(tex);
		freeImage(tex);
	}
	free(texs);
	imgFall->_aniDt = 0.15f; // img 4개
	imgFall->_repeatNum = 1;
	imgFall->_selectedDt = imgFall->_aniDt * 4.0f;
	imgFall->selectedScale = 0.5f;

	iImage* imgEvasion = new iImage();
	texs = createDivideImage(4, 1, strChar[5 * s + 4]);	// 회피모션 4개
	for (i = 0; i < 4; i++)
	{
		tex = createTexture(size.x, size.y);
		fbo->bind(tex);
		fbo->setSize(size);

		Texture* t = texs[i];
		drawImage(t, 0, 0,
			0, 0, t->width, t->height,
			TOP | LEFT, size.x / t->width, size.y / t->height,
			2, 0, REVERSE_HEIGHT);
		freeImage(t);

		fbo->backSize();
		fbo->unbind();

		imgEvasion->addObject(tex);
		freeImage(tex);
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

	//for (i = 0; i < 10; i++)
	//	img[i]->ratio = 2.0f;
}

bool Player::actionCheck(bool key)
{
	if (key == false || act != Act_idle)
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
	sort->add(playerPosition.y + HALF_OF_TEX_HEIGHT / 2.0f);
	if (pwp->isMelee)
	{
		meleeWeapon* mw = (meleeWeapon*)pwp->wp;
		sort->add(mw->centerPos.y);
	}
	else
	{	
		//range
		rangeWeapon* rw = (rangeWeapon*)pwp->wp;
		sort->add(rw->centerPos.y);
	}
	sort->update();

	//for (int i = 0; i < sort->sdNum; i++) // 수정필요
	//{
	//	if (sort->get(i) == 0)
	//		drawPlayer(dt);
	//	else
	//		combatDraw(dt);
	//}

	drawPlayer(dt);
	combatDraw(dt);

	choseWeapon(getKeyDown(keyboard_tab));
	rootCombat(getKeyDown(keyboard_i));
	dropCombat(dt, getKeyDown(keyboard_o));
}

void Player::combatDraw(float dt)
{
	if (act == Act_evasion || act == Act_falling)
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
	iPoint size = HALF_OF_TEX_POINT * 2.0f;
	iPoint p = playerPosition;
	iPoint tile = maps[tileNumber]->tileOff;
	if (p.x < tile.x)
	{
		tileNumber--;
		playerPosition.x -= size.x;
	}
	else if (p.x + size.x > tile.x + RGTILE_X * RGTILE_Width - 1)
	{
		tileNumber++;
		playerPosition.x += size.x;
	}
	else if (p.y < tile.y)
	{
		tileNumber -= TILEOFF_SQRT;
		playerPosition.y -= size.y;
	}
	else if (p.y + size.y > tile.y + RGTILE_Y * RGTILE_Height - 1)
	{
		tileNumber += TILEOFF_SQRT;
		playerPosition.y += size.y;
	}
	else
		printf("player setTile error\n");
		//printf("%d\n ", tileNumber);


	camPosition = iPointZero - maps[tileNumber]->tileOff;
}

void Player::drawtouchPlayer()
{
	iRect rt = iRectMake(playerPosition.x + img[0]->tex->width * 0.25f, 
		playerPosition.y + img[0]->tex->height / 2.0f,
		HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);

	touchPlayer = rt;

	setRGBA(0, 1, 0, 0.5f);
	rt.origin += SET_DRAW_OFF;
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
}

void Player::drawPlayer(float dt)
{
	iPoint v = iPointZero;
	iPoint comp = iPointZero;
	int angle = 0;
	if (act == Act_idle) 
	{
		if (getKeyStat(keyboard_left)) //이동방향, 머리방향
		{
			v.x = -1;
			headNum = 4;
			comp = iPointMake(HALF_OF_TEX_WIDTH * 0.5f, HALF_OF_TEX_HEIGHT * 0.5f);
			angle = 90;
		}
		else if (getKeyStat(keyboard_right))
		{
			v.x = 1;
			headNum = 5;
			comp = iPointMake(HALF_OF_TEX_WIDTH * 1.5f, HALF_OF_TEX_HEIGHT * 1.5f);
			angle = 270;
		}
		if (getKeyStat(keyboard_up))
		{
			v.y = -1;
			headNum = 6;
			comp = iPointMake(HALF_OF_TEX_WIDTH * 1.5f , HALF_OF_TEX_HEIGHT * 0.5f);
			angle = 0;

		}
		else if (getKeyStat(keyboard_down))
		{
			v.y = 1;
			headNum = 7;
			comp = iPointMake(HALF_OF_TEX_WIDTH * 0.5f, HALF_OF_TEX_HEIGHT * 1.5f);
			angle = 180;
		}
	}
	viewVector = v;

	if (v != iPointZero)
	{
		combatV = v;
		if (combatV.y != 0)
			combatV.x = 0;
		combatAngleV = angle;
		combatPos = playerPosition + comp;
	}

	bool ani = (v != iPointZero);
	if (ani)
		v /= iPointLength(v);
	iPoint mp = v * (moveSpeed * dt) * (act == Act_attack ? 0.1f : 1.0f);

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
	if (act != Act_evasion)
		if (fallCheck(tile, dt))
			return;

	if (evasionPlayer(tile, dt))
		return;

	iPoint size = iPointMake(HALF_OF_TEX_WIDTH * 0.5f, HALF_OF_TEX_HEIGHT);
	iPoint playPos = playerPosition + size;
	wallCheck(false, tile, playPos, mp, size.x, size.y * 0.5f);
	playerPosition = playPos - (size );

	img[headNum]->setTexAtIndex(act == Act_attack ? true : false);
	iPoint drawPos = playerPosition + SET_DRAW_OFF;

	//히트박스 표시-------------------------------
	drawtouchPlayer();

	if (combatV.x)
	{
		img[2 * ani + 0]->reverse = (combatV.x < 0 ? REVERSE_WIDTH : REVERSE_NONE);
		img[2 * ani + 0]->paint(dt, drawPos);
	}
	else if (combatV.y)
		img[2 * ani + 1]->paint(dt, drawPos);

	float dis = (img[2 * ani + 0]->tex->width - img[headNum]->tex->width) / 2;
	drawPos += iPointMake(dis * img[headNum]->ratio, -img[headNum]->tex->height / 2 * img[headNum]->ratio);
	img[headNum]->paint(dt, drawPos);
}

void endEvasion(iImage* me)
{
	pc->act = Act_idle;
	pc->evasV = iPointZero;
}

bool Player::evasionPlayer(MapTile* tile, float dt)
{
	if (getKeyDown(keyboard_space) && act == Act_idle)
	{
		img[Player_imgEvasion]->startAnimation(endEvasion);
		act = Act_evasion;

		audioPlay(SND_JUMP);

		evasV = viewVector != iPointZero ? viewVector / iPointLength(viewVector) : iPointZero;
	}

	if (act != Act_evasion)
		return false;

	if (img[Player_imgEvasion]->animation)
	{	
		iPoint mp = evasV * (EVASION_DISTANCE * dt);
		iPoint size = iPointMake(HALF_OF_TEX_WIDTH * 0.5f, HALF_OF_TEX_HEIGHT);

		iPoint playPos = playerPosition + size;
		wallCheck(false, tile, playPos, mp, size.x, size.y * 0.5f);
		playerPosition = playPos - size;

		iPoint dp = playerPosition + iPointMake(0, -30) + SET_DRAW_OFF;
		iPoint p = iPointMake(dp.x - size.x, 
			dp.y - (size.y * 2.0));
		
		//히트박스 표시-------------------------------
		drawtouchPlayer();

		img[Player_imgEvasion]->paint(dt, p);
		
		return true;
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
		(pc->playerPosition + HALF_OF_TEX_POINT)
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
		if (pc->act != Act_falling)
		{
			pc->img[Player_imgFall]->startAnimation();
			pc->img[Player_imgFall]->selected = true;
			findMoveTile(t, moveTileNum);

			audioPlay(SND_FALL);
		}

		if (pc->img[Player_imgFall]->animation == false)
		{
			pc->img[Player_imgFall]->selected = false;
			pc->act = Act_idle;
			pc->playerPosition = moveTileNum;

			return false;
		}

		pc->act = Act_falling;
		iPoint p = pc->playerPosition - t->tileOff + setPos
			- HALF_OF_TEX_POINT / 2.0f;

		pc->img[Player_imgFall]->selected = true;
		pc->img[Player_imgFall]->paint(dt, p);

		return true;
	}

	return false;
}