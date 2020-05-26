#include "Player.h"

#include "Room.h"
#include "Stage.h"

#include "Weapon.h"
#include "WMelee.h"


Player* pc;
iSort* sort;

void freeWeapon(void* data)
{
	PlayerMW* pmw = (PlayerMW*)data;
	freeMeleeWeapon(pmw);
}

Player::Player()
{
	sort = new iSort();
	weaponArray = new rgArray(freeWeapon);

	createPlayerImage();
	initPlayerStat();
}

Player::~Player()
{
	delete sort;

	for (int i = 0; i < 10; i++)
	{
		if (img[i])
			delete img[i];
	}
	free(img);
	delete pc;
}

void Player::instance()
{
	if (pc)
		return;
	pc = new Player();
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

	weaponArray->addObject(&PMW[0]);
	pmw = &PMW[0];
	PMW[0].drop = false;
	pmwCount = 1;

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
	iImage** imgChar = (iImage**)malloc(sizeof(iImage*) * 10);

	iImage* imgRight = new iImage();
	Texture** texsRight = createDivideImage(10, 1, "assets/char/CharBodyR.png");	// 좌우 10개

	iImage* imgDown = new iImage();
	Texture** texsDown = createDivideImage(10, 1, "assets/char/CharBodyUD.png");	// 상하 10개

	iImage** imgHead = (iImage**)malloc(sizeof(iImage*) * 4);			// 머리 상하좌우
	Texture** texsHead = createDivideImage(8, 1, "assets/char/CharHead.png");		// 머리 4방향 각 2개

	iImage* imgFall = new iImage();
	Texture** texsFall = createDivideImage(4, 1, "assets/char/CharJump.png");		// 낙하모션 4개

	iImage* imgEvasion = new iImage();
	Texture** texsEvasion = createDivideImage(4, 1, "assets/char/CharJump.png");	// 회피모션 4개

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


	imgEvasion->_aniDt = EVASION_DURATION / 4.0f;
	imgEvasion->_repeatNum = 1;
	imgEvasion->_selectedDt = imgEvasion->_aniDt * 4.0f;
	//imgEvasion->angle = 720.0f;

	img = imgChar;

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
				pmwCount = weaponArray->count - 1;
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
			pw->pos = playerPosition + HALF_OF_TEX_POINT;
			pmwCount--;
			if (pmwCount < 0)
				pmwCount = weaponArray->count - 1;
			pmw = (PlayerMW*)weaponArray->objectAtIndex(pmwCount);
			break;
		}	
	}
}

void Player::choseWeapon(bool key)
{
	if (actionCheck(key))
		return;

	pmwCount--;
	if (pmwCount < 0)
		pmwCount = weaponArray->count - 1;
	pmw = (PlayerMW*)weaponArray->objectAtIndex(pmwCount);
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

	if (mv.x)
		img[2 * ani + 0]->paint(dt, drawPos, mv.x < 0 ? REVERSE_WIDTH : REVERSE_NONE);
	else if (mv.y)
		img[2 * ani + 1]->paint(dt, drawPos , REVERSE_NONE);

	drawImage(img[headNum]->tex, drawPos.x + HALF_OF_TEX_WIDTH, drawPos.y + 3,
		0, 0, img[headNum]->tex->width, img[headNum]->tex->height,
		VCENTER | HCENTER, 1.0f, 1.0f,
		img[headNum]->location, img[headNum]->angle, REVERSE_NONE);

	iRect rt = iRectMake(playerPosition.x,
		playerPosition.y ,
		HALF_OF_TEX_WIDTH * 2.0f, HALF_OF_TEX_HEIGHT * 2.0f);

	touchPlayer = rt;

	//히트박스 표시-------------------------------
	setRGBA(0, 1, 0, 0.1f);
	rt.origin += pc->camPosition + setPos;
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

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
			
			evasV = viewVector / iPointLength(viewVector);
		}
	}

	if (act != evasion)
		return false;


	if (img[9]->animation == true)
	{	
		iPoint mp = evasV * (EVASION_DISTANCE * dt);
		wallCheck(false, tile, playerPosition, mp, HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT);

		drawPos = playerPosition + camPosition + setPos;
		iPoint p = iPointMake(drawPos.x - HALF_OF_TEX_WIDTH/2,
			drawPos.y - HALF_OF_TEX_HEIGHT);
		
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

		img[9]->paint(dt, p, REVERSE_NONE);
		
		return true;
	}

	else if (img[9]->animation == false)
	{
		act = idle;
		evasV = iPointZero;

		return false;
	}
	
	return false;
}