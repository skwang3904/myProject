#include "PlayerChar.h"

#include "Proc.h"
#include "ProcData.h"
#include "Tile.h"
#include "Map.h"
#include "Weapon.h"

#include "Projectile.h"

PlayerChar* player = NULL;
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
	imgs[7]->position = iPointMake(0, -imgs[7]->tex->height);
	//imgs[7]->position = iPointMake(-imgs[7]->tex->width * 0.22f, -imgs[7]->tex->height);
	this->imgs[9] = imgs[7];

	//common data
	this->img = this->imgs[5];

	position = maps[mapNumber]->tileOff + position;
	vector = iPointZero;

	size = iSizeMake(this->img->tex->width, this->img->tex->height);
	touchSize = size * 0.33f;
	iPoint p = position + iPointMake(size.width, size.height) * 0.33f;
	touchRect = iRectMake(p, touchSize);

	//------------------------------------------------------------------------------------------
	// init data

	headNum = 0;
	holdNum = 4;

	state = player_idle;
	camera = iPointZero - maps[mapNumber]->tileOff;
	wpVector = iPointMake(0, 1);
	wpPosition = iPointZero;

	PlayerInfo* pi = &playerInfo[index];
	PlayerData* pd = &st->playerData;
	hp = pd->hp;
	_hp = pd->_hp;
	attackPoint = pd->attackPoint;
	_attackPoint = pd->_attackPoint;
	attackSpeed = pd->attackSpeed;
	_attackSpeed = pd->_attackSpeed;
	moveSpeed = pd->moveSpeed;

	arrayWeapon = new rgArray();

	coin = 0;
	gem = 0;
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

	if (dt > 0.0f)
	{
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
		else
		{
			vector = mp;
		}
	}

	iPoint half = iPointMake(img->tex->width * 0.5f, img->tex->height * 0.5f);
	iPoint sp = position + half;

	wpPosition = sp + iPointMake(-wpVector.y * 32, wpVector.x * 24);
	mp = vector * (moveSpeed * dt);
	wallCheck(this, mp);
	
	drawShadow(dt, off);

	//evasion
	iPoint p = position + DRAW_OFF;
	img->reverse = reverse;
	img->paint(dt, p);
	if (state != player_jump)
		imgs[headNum]->paint(dt, p + iPointMake(4, -img->tex->height / 1.8f));


	// change weapon
	int i;
	if (getKeyDown(keyboard_i) && state < player_attack)
	{
		for (i = 0; i < weaponNum; i++)
		{
			if (weapon[i]->attacking)
				return;
		}
		selectWeapon(arrayWeapon->currIndex - 1);
	}

	if (getKeyDown(keyboard_u) && state < player_attack)
	{
		for (int i = 0; i < weaponNum; i++)
		{
			Weapon* w = weapon[i];
			if (w->get || w->rootDt < w->_rootDt)
				continue;

			if (containRect(touchRect, w->touchRect))
			{
				w->getWeapon();
				break;
			}
		}
	}

#if SHOW_TOUCHRECT
	setRGBA(1, 0, 0, 0.5f);
	iRect rt = touchRect;
	rt.origin += DRAW_OFF;
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif
}

void PlayerChar::drawShadow(float dt, iPoint off)
{
	//
	iSize size = iSizeMake(img->tex->width, img->tex->height);
	iPoint p = position + iPointMake(size.width / 2.0f, size.height * 0.75f) + off;
	setRGBA(0, 0, 0, 0.3f);
	fillEllipse(p.x, p.y, size.width, size.height * 0.5f);
	setRGBA(1, 1, 1, 1);
}

void PlayerChar::action(Object* obj)
{
	hp -= obj->attackPoint;
#if 1	// game over
	if (hp <= 0.0f)
		showPopGameOver(true);
#endif
}

void PlayerChar::setNewStage(int index, int8 mapNum, iPoint pos)
{
	this->index = index;
	mapNumber = mapNum;
	position = maps[mapNumber]->tileOff + pos;

	headNum = 0;
	holdNum = 4;

	state = player_idle;
	camera = iPointZero - maps[mapNumber]->tileOff;
	wpVector = iPointMake(0, 1);
	wpPosition = iPointZero;
}

void PlayerChar::addWeapon(void* weapon)
{
	arrayWeapon->addObject(weapon);

	drawPopInvenRefreshWeapon();
}

void PlayerChar::removeCurrWeapon()
{
	arrayWeapon->remove(arrayWeapon->currIndex);

	drawPopInvenRefreshWeapon();
}

void PlayerChar::removeWeapon(int index)
{
	arrayWeapon->remove(index);
}

void PlayerChar::selectWeapon(int index)
{
	arrayWeapon->objectAtIndex(index);

	drawPopInvenRefreshWeapon();
}

int PlayerChar::currWeaponIndex()
{
	return arrayWeapon->currIndex;
}

void PlayerChar::cbPlayerSetIdle(iImage* me)
{
	player->state = player_idle;
}

void PlayerChar::cbPlayerSetAttack(iImage* me)
{
	player->state = player_attack;
}

//-------------------------------------------------------------------------------------

void loadPlayerChar()
{
	PlayerData* pd = &st->playerData;
	if (player)
		player->setNewStage(pd->index, pd->mapNum, pd->position);
	else
		player = new PlayerChar(pd->index, pd->mapNum, pd->position);
}

void freePlayerChar()
{
	delete player;
	player = NULL;
}

void drawPlayerChar(float dt)
{
#if SORTING
	objects[procSort->sdNum] = player;
	procSort->add(player->position.y + player->img->tex->height * 0.75f);
#else
	player->paint(dt, DRAW_OFF);
#endif
}
