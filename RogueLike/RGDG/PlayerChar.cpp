#include "PlayerChar.h"

#include "Tile.h"
#include "Map.h"
#include "ProcData.h"


struct charPath 
{ 
	const char* str[5]; 
};



PlayerChar* player;
PlayerChar::PlayerChar(int index) : Object(index)
{
	int i, j, k;

	const char* cp[5] = {
	"assets/char/CharHead%d.png",
	"assets/char/CharBodyR%d.png",
	"assets/char/CharBodyUD%d.png",
	"assets/char/CharGood%d.png",
	"assets/char/CharJump%d.png",
	};
	
	iImage* img;
	Texture* tex;
	iImage** imgs = (iImage**)malloc(sizeof(iImage*) * 8);

	Texture** texs = createDivideImage(8, 1, cp[0], index);
	iSize size = iSizeMake(60, 45) * 2.0f;
	for (i = 0; i < 4; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			Texture* t = texs[2 * i + j];
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
		imgs[i] = img;
	}
	for (k = 0; k < 8; k++)
		freeImage(texs[k]);
	free(texs);

	size = iSizeMake(64, 40) * 2.0f;
	for (i = 0; i < 2; i++)
	{
		texs = createDivideImage(10, 1, cp[1 + i], index);
		img = new iImage();
		for (j = 0; j < 10; j++)
		{
			tex = createTexture(size.width, size.height);
			Texture* t = texs[j];

			fbo->bind(tex);
			drawImage(t, 0, 0,
				0, 0, t->width, t->height,
				TOP | LEFT, size.width / t->width, size.height / t->height,
				2, 0, REVERSE_HEIGHT);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		img->animation = true;
		img->_aniDt = 0.1f;
		imgs[4 + i] = img;

		for (k = 0; k < 10; k++)
			freeImage(texs[k]);
		free(texs);
	}

	size = iSizeMake(97, 60) * 2.0f;
	for (i = 0; i < 2; i++)
	{
		texs = createDivideImage(4, 1, cp[3 + i], index);
		img = new iImage();
		for (j = 0; j < 4; j++)
		{
			tex = createTexture(size.width, size.height);
			Texture* t = texs[j];

			fbo->bind(tex);
			drawImage(t, 0, 0,
				0, 0, t->width, t->height,
				TOP | LEFT, size.width / t->width, size.height / t->height,
				2, 0, REVERSE_HEIGHT);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		img->_aniDt = 0.3f;
		imgs[6 + i] = img;

		for (k = 0; k < 4; k++)
			freeImage(texs[k]);
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
	imgs[7]->position = iPointMake(-imgs[7]->tex->width * 0.27f, -imgs[7]->tex->height);
	this->imgs[9] = imgs[7];

	//common data
	this->img = this->imgs[5];

	mapNumber = 0;
	position = iPointMake(TILE_NUM_X * TILE_Width / 2.0f, TILE_NUM_Y * TILE_Height / 2.0f);
	vector = iPointZero;
	touchRect = iRectZero;

	//------------------------------------------------------------------------------------------
	// init data

	headNum = 0;
	holdNum = 4;

	state = player_idle;
	camera = iPointZero - maps[0]->tileOff;

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

}

PlayerChar::~PlayerChar()
{
	//
}

void PlayerChar::initData()
{
	// 스테이지 넘어갈때 or 죽었을때
}

void callBackIdle(iImage* me)
{
	player->state = player_idle;
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

		}
		else if (key & keyboard_right)
		{
			headNum = 1;
			img = imgs[6];
			reverse = REVERSE_NONE;
			mp.x += 1.0f;
			holdNum = 4;
		}
		if (key & keyboard_up)
		{
			headNum = 2;
			img = imgs[7];
			reverse = REVERSE_WIDTH;
			mp.y -= 1.0f;
			holdNum = 5;
		}
		else if (key & keyboard_down)
		{
			headNum = 3;
			img = imgs[7];
			reverse = REVERSE_NONE;
			mp.y += 1.0f;
			holdNum = 5;
		}

		if (getKeyDown(keyboard_space))
		{
			state = player_jump;
			img = imgs[9];
			img->startAnimation(callBackIdle);
		}

		vector = iPointVector(mp);
	}
	else if (key == 0 && state < player_attack)
	{
		state = player_idle;
		img = imgs[holdNum];

		vector = mp;	
	}

	mp = vector * (moveSpeed * dt);
	wallCheck(this, mp);
	
	iPoint rp = position + iPointMake(img->tex->width * 0.25f, img->tex->height * 0.25);
	touchRect = iRectMake(rp.x, rp.y, img->tex->width * 0.5f, img->tex->height * 0.5f);
	setRGBA(1, 0, 0, 1);
	iRect rt = touchRect;
	rt.origin += DRAW_OFF + img->position;
	fillRect(rt);
	//rect
	setRGBA(1, 1, 1, 1);

	//camera -= mp;
	iPoint sp = position + iPointMake(img->tex->width / 2.0f, img->tex->height / 2.0f);
	if (sp.x < maps[mapNumber]->tileOff.x )
	{
		mapNumber--;
	}
	else if (sp.x > maps[mapNumber]->tileOff.x + TILE_NUM_X * TILE_Width - 1)
	{
		mapNumber++;
	}
	if (sp.y < maps[mapNumber]->tileOff.y )
	{
		mapNumber -= TILE_TOTAL_SQRT;
	}
	else if (sp.y > maps[mapNumber]->tileOff.y + TILE_NUM_Y * TILE_Height - 1)
	{
		mapNumber += TILE_TOTAL_SQRT;
	}
	camera = iPointZero - maps[mapNumber]->tileOff;

	//evasion


	iPoint p = position + DRAW_OFF;
	img->reverse = reverse;
	img->paint(dt, p);
	if (state != player_jump)
		imgs[headNum]->paint(dt, p + iPointMake(4, -43));
}


//-------------------------------------------------------------------------------------