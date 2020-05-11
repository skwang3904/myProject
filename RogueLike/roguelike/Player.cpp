#include "Player.h"

#include "Room.h"
#include "RgTile.h"

#include "Weapon.h"
#include "WMelee.h"

Player* pc;
Texture** texsRight;
Texture** texsDown;
Texture** texsHead;
Texture** texsFall;
Texture** texsEvasion;

Method_Combat _mw[MELEE_NUM] = { nomalSwordMethod, nomalSpearMethod };


bool evasion = false;
bool falling = false;
bool attacking = false;

bool actionCheck(bool key)
{
	if (key == false || evasion == true || falling == true || attacking == true)
		return true;
	return false;
}

Player::Player()
{
	initPlayerStat();
}

Player::~Player()
{
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

	for (int i = 0; i < 10; i++)
	{
		if (img[i])
			delete img[i];
	}
	free(img);
}

Player* Player::instance()
{
	static Player* p = new Player();
	return p;
}


void Player::initPlayerStat()
{
	hp =
	_hp = 100.0f;

	attackDmg =
	_attackDmg = 10.0f;

	attackSpeed =
	_attackSpeed = 0.5f;

	moveSpeed = 200.0f;

	playerPosition = iPointMake(RGTILE_X * RGTILE_Width /2, RGTILE_Y * RGTILE_Height/2);
	weaponVector = iPointMake(0, 1);

	touchPlayer = iRectZero;

	mw = NULL;

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

	// 생성 
	//----------------------------
	// 설정

	imgRight->startAnimation();
	imgRight->_aniDt = 0.05f;
	imgRight->_repeatNum= 0;

	imgDown->startAnimation();
	imgDown->_aniDt = 0.05f;
	imgDown->_repeatNum = 0;
	
	imgFall->_aniDt = 0.15f;
	imgFall->_repeatNum = 1;

	imgEvasion->_aniDt = 0.1f;
	imgEvasion->_repeatNum = 1;

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

	freeImage(imgRight->tex);
	freeImage(imgDown->tex);
}

void Player::combatDraw(float dt)
{
	if(mw)
		mw(dt, false, iPointZero);
}

void Player::rootCombat(bool key)
{
	if (actionCheck(key))
		return;

	int num = meleeNum;
	if (mw)
	{
		for (int i = 0; i < num; i++)
		{
			if (mw == _mw[i])
			{
				weapon->wDropPos[i] = iPointMake(playerPosition.x + HALF_OF_TEX_WIDTH,
					playerPosition.y + HALF_OF_TEX_HEIGHT);

				mw = NULL;
				break;
			}
		}
	}

	for (int i = 0; i < num; i++)
	{
		if (mw != _mw[i])
		{
			if (containRect(touchPlayer, _meleeWP[i]->hitBox))
			{
				//drop weapon				
				mw = _mw[i];
				weapon->wDropPos[i] = iPointZero;
				break;
			}
		}
	}
}

void Player::dropCombat(float dt,bool key)
{
	if (actionCheck(key))
		return;
	
	int num = meleeNum;
	if (mw)
	{
		//임시
		for (int i = 0; i < num; i++)
		{
			if (mw == _mw[i])
			{
				weapon->wDropPos[i] = iPointMake(playerPosition.x + HALF_OF_TEX_WIDTH,
					playerPosition.y + HALF_OF_TEX_HEIGHT);
			}
		}

		mw = NULL;
	}
	
}

void Player::movePlayer(float dt)
{
	iPoint v = iPointZero;
	static iPoint weaponV = iPointMake(0,1);
	static int ch= 7;

	if (getKeyStat(keyboard_left)) //이동방향, 머리방향
	{
		v.x = -1;
		ch = 4;
	}
	else if (getKeyStat(keyboard_right))
	{
		v.x = 1;
		ch = 5;
	}
	if (getKeyStat(keyboard_up)) 
	{ 
		v.y = -1;
		ch = 6;
	}
	else if (getKeyStat(keyboard_down))
	{
		v.y = 1;
		ch = 7;
	}
	weaponV = v;
	weaponVector = weaponV;

	bool ani = (v != iPointZero);
	if(ani)
		v /= iPointLength(v);
	iPoint mp = v * (moveSpeed * dt);


	if (evasion == false)
		if (falling = fallCheck(pc, dt))
			return;

	wallCheck(pc, mp);
	if (evasionPlayer(dt))
		return;


	//방향에 따라 캐릭터 애니메이션
	if (v.x)
	{
		img[2 * ani + 0]->paint(dt, playerPosition, v.x < 0 ? REVERSE_WIDTH : REVERSE_NONE);
		drawImage(img[ch]->tex , 
			playerPosition.x + HALF_OF_TEX_WIDTH,
			playerPosition.y  + HALF_OF_TEX_HEIGHT - 12,
			VCENTER | HCENTER);
	}
	else
	{
		img[2 * ani + 1]->paint(dt, playerPosition, REVERSE_NONE);
		drawImage(img[ch]->tex, 
			playerPosition.x + HALF_OF_TEX_WIDTH,
			playerPosition.y + HALF_OF_TEX_HEIGHT - 12,
			VCENTER | HCENTER);
	}

	iRect rt = iRectMake(playerPosition.x, playerPosition.y,
		HALF_OF_TEX_WIDTH *2.0f,
		HALF_OF_TEX_HEIGHT * 2.0f);

	//히트박스 표시-------------------------------
	setRGBA(0, 1, 0, 0.3f);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

	touchPlayer = rt;

}

bool Player::evasionPlayer(float dt)
{
	// 회피
	// 전방 이동 
	// 회피중 무적
	if (getKeyDown(keyboard_space))
	{
		if (weaponVector != iPointZero)
		{
			if (evasion == false)
				img[9]->startAnimation();
			evasion = true;
		}
	}

	if (img[9]->animation == true)
	{	
		iPoint v = iPointZero;
		if (weaponVector != iPointZero)
			v = weaponVector / iPointLength(weaponVector);

		if (playerPosition.x < 0.0f)
			playerPosition.x = 0.0f;
		else if (playerPosition.x > RGTILE_X* RGTILE_Width - HALF_OF_TEX_WIDTH * 2.0f)
				 playerPosition.x = RGTILE_X * RGTILE_Width - HALF_OF_TEX_WIDTH * 2.0f;
		if (playerPosition.y < 0.0f)
			playerPosition.y = 0.0f;
		else if (playerPosition.y > RGTILE_Y* RGTILE_Height - HALF_OF_TEX_HEIGHT * 2.0f)
				 playerPosition.y = RGTILE_Y * RGTILE_Height - HALF_OF_TEX_HEIGHT * 2.0f;

		iPoint p = iPointMake(	playerPosition.x - HALF_OF_TEX_WIDTH,
								playerPosition.y - HALF_OF_TEX_HEIGHT - 30);

		img[9]->paint(dt, p, REVERSE_NONE);

		return true;
	}

	else if (img[9]->animation == false)
	{
		evasion = false;
		return false;
	}
	
	return false;
}

