#include "PlayerChar.h"

#include "Proc.h"
#include "ProcData.h"
#include "Tile.h"
#include "Map.h"
#include "Weapon.h"

PlayerChar* player;
PlayerChar::PlayerChar(int index, int8 mapNum, iPoint pos) : Object(index, mapNum, pos)
{
	int i, j, k, n, num, count = 0;

	iImage* img;
	Texture* tex;
	Texture** texs;
	iImage** imgs = (iImage**)malloc(sizeof(iImage*) * 8);
	iSize size;

	for (i = 0; i < 5; i++)
	{
		PlayerImageInfo* pii = &playerImageInfo[i];
		size = pii->size;
		num = pii->dirNum * pii->kindsNum;
		texs = createDivideImage(num, 1, pii->strPath, index);
		for (j = 0; j < pii->dirNum; j++)
		{
			img = new iImage();
			for (k = 0; k < pii->kindsNum; k++)
			{
				Texture* t = texs[pii->kindsNum * j + k];
				tex = createTexture(size.width, size.height);

				fbo->bind(tex);
				drawImage(t, 0, 0,
					0, 0, t->width, t->height,
					TOP | LEFT, size.width / t->width, size.height / t->height,
					2, 0, REVERSE_HEIGHT);
				fbo->unbind();

				img->addObject(tex);
				freeImage(tex);
			}
			img->_aniDt = pii->aniDt;
			img->animation = pii->animation;
			
			imgs[count] = img;
			count++;
		}

		for (n = 0; n < num; n++)
			freeImage(texs[n]);
		free(texs);
	}

	imgNum = 10;
	this->imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);

	//head
	this->imgs[0] = imgs[3];
	this->imgs[1] = imgs[1];
	this->imgs[2] = imgs[2];
	this->imgs[3] = imgs[0];
	//body
	this->imgs[4] = new iImage(); this->imgs[4]->addObject(imgs[4]->tex);
	this->imgs[5] = new iImage(); this->imgs[5]->addObject(imgs[5]->tex);
	this->imgs[6] = imgs[4];
	this->imgs[7] = imgs[5];
	// good
	this->imgs[8] = imgs[6];
	// jump
	imgs[7]->_repeatNum = 1;
	imgs[7]->_aniDt = 0.1f;
	imgs[7]->lastFrame = true;
	imgs[7]->position = iPointMake(-imgs[7]->tex->width * 0.22f, -imgs[7]->tex->height);
	this->imgs[9] = imgs[7];

	//common data
	this->img = this->imgs[5];


	vector = iPointZero;
	touchRect = iRectZero;

	//------------------------------------------------------------------------------------------
	// init data

	headNum = 0;
	holdNum = 4;

	state = player_idle;
	camera = iPointZero - maps[mapNumber]->tileOff;
	wpVector = iPointMake(0, 1);
	wpPosition = iPointZero;

	PlayerInfo* pi = &playerInfo[index];
	hp = _hp = pi->_hp;
	attackPoint = _attackPoint = pi->_attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = pi->_attackSpeed;
	moveSpeed = pi->moveSpeed;

#if 0
	for (int i = 0; i < TILE_TOTAL_NUM; i++)
	{
		int a = random() % TILE_TOTAL_NUM;
		if (maps[a]->tile[0] != 0 && maps[a]->state == MapType_Nomal)
		{
			position = maps[a]->tileOff + TILE_CENTER;
			camera = displayCenterPos - maps[a]->tileOff;
			mapNumber = a;
			break;
		}
	}
#endif

	arrayWeapon = new rgArray();
}

PlayerChar::~PlayerChar()
{
	delete arrayWeapon;
}

void PlayerChar::paint(float dt, iPoint off)
{
	uint8 reverse = img->reverse;
	iPoint mp = iPointZero;
	uint32 key = getKeyStat();

	if (key && state < player_attack)
	{
		state = player_move;
		if (key & keyboard_left)
		{
			headNum = 0;
			img = imgs[6];
			reverse = REVERSE_WIDTH;
			mp.x -= 1.0f;
			holdNum = 4;

			wpVector.x = -1.0f;
			wpVector.y = 0.0f;
		}
		else if (key & keyboard_right)
		{
			headNum = 1;
			img = imgs[6];
			reverse = REVERSE_NONE;
			mp.x += 1.0f;
			holdNum = 4;

			wpVector.x = 1.0f;
			wpVector.y = 0.0f;
		}
		if (key & keyboard_up)
		{
			headNum = 2;
			img = imgs[7];
			reverse = REVERSE_WIDTH;
			mp.y -= 1.0f;
			holdNum = 5;

			wpVector.x = 0.0f;
			wpVector.y = -1.0f;
		}
		else if (key & keyboard_down)
		{
			headNum = 3;
			img = imgs[7];
			reverse = REVERSE_NONE;
			mp.y += 1.0f;
			holdNum = 5;

			wpVector.x = 0.0f;
			wpVector.y = 1.0f;
		}

		if (getKeyDown(keyboard_space))
		{
			state = player_jump;
			img = imgs[9];
			img->startAnimation(cbPlayerSetIdle);
		}

		vector = iPointVector(mp);
	}
	else if (key == 0 && state < player_attack)
	{
		state = player_idle;
		img = imgs[holdNum];

		vector = mp;	
	}

	iPoint half = iPointMake(img->tex->width / 2.0f, img->tex->height / 2.0f);
	iPoint sp = position + half;

	wpPosition = sp + iPointMake(-wpVector.y * 32, wpVector.x * 24);

	mp = vector * (moveSpeed * dt);
	wallCheck(this, mp);
	
	iPoint rp = position + half * 0.5f;
	touchRect = iRectMake(rp.x, rp.y, half.x, half.y);

#if 0 //draw touchRect
	setRGBA(1, 0, 0, 1);
	iRect rt = touchRect;
	rt.origin += DRAW_OFF + img->position;
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

	//evasion
	iPoint p = position + DRAW_OFF;
	img->reverse = reverse;
	img->paint(dt, p);
	if (state != player_jump)
		imgs[headNum]->paint(dt, p + iPointMake(4, -43));

	if (getKeyDown(keyboard_i))
	{
		for (int i = 0; i < weaponNum; i++)
		{
			if (weapon[i]->attacking)
				return;
		}
		selectWeapon(arrayWeapon->currIndex - 1);
	}
}

void PlayerChar::action(Object* obj)
{
	hp -= obj->attackPoint;
	if (hp <= 0.0f)
		showPopGameOver(true);
}

void PlayerChar::initData()
{
	// 스테이지 넘어갈때 or 죽었을때
}

void PlayerChar::addWeapon(void* weapon)
{
	arrayWeapon->addObject(weapon);
}

void PlayerChar::removeCurrWeapon()
{
	arrayWeapon->remove(arrayWeapon->currIndex);
}

void PlayerChar::removeWeapon(int index)
{
	arrayWeapon->remove(index);
}

void PlayerChar::selectWeapon(int index)
{
	arrayWeapon->objectAtIndex(index);
}

int PlayerChar::currWeaponIndex()
{
	return arrayWeapon->currIndex;
}

void PlayerChar::cbPlayerSetIdle(iImage* me)
{
	player->state = player_idle;
}

//-------------------------------------------------------------------------------------

void loadPlayerChar()
{
	iPoint p = iPointMake(TILE_NUM_X * TILE_Width / 2.0f, TILE_NUM_Y * TILE_Height / 2.0f);
	player = new PlayerChar(0, 0, p);
}

void freePlayerChar()
{
	delete player;
}

void drawPlayerChar(float dt)
{
	player->paint(dt, DRAW_OFF);
}
