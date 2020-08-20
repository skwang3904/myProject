#include "Room.h"

#include "EnemyData.h"
#include "Stage.h"

ConnectTile ct[TILEOFF_NUM];
MapTile** maps;

#if CHECK_CODE
MapObject*** mapObjs;
MapObject*** mapBossObjs;
MapObject*** mapTreasureObjs;
#endif 

void createObjImage();
void freeObjImage();

void createObject();
void freeObject();

void connectCheck(ConnectTile* c, int& count);
void pathTileCheck(ConnectTile* c);

struct passTile {
	int tileNum;
	iPoint sp;
	iPoint tp;
	float passAniDt;
};
static passTile pt;


iImage* imgRoom;
#if CHECK_CODE
iImage* imgNextDoor;

iImage* imgBarrel;
iImage* imgDoorNormal;
#endif 

void loadRoomTile()
{
	int i;
	maps = (MapTile**)malloc(sizeof(MapTile*) * TILEOFF_NUM);

	for (i = 0; i < TILEOFF_NUM; i++)
	{
		maps[i] = (MapTile*)malloc(sizeof(MapTile) * 1);
		maps[i]->rgTile = (int*)calloc(sizeof(int), RGTILE_X * RGTILE_Y);
	}

	pt.tileNum = -1;
	pt.sp = iPointZero;
	pt.tp = iPointZero;
	pt.passAniDt = _passAniDt;

	createObjImage();
	createObject();
	setRoomTile();
}

void setRoomTile()
{
	int i, j;
	bool randomOffCheck[TILEOFF_NUM];

	int m[MAPTILE_NUM];
	int conectCount = 0;
	while (conectCount < MAPTILE_NUM)
	{
		for (i = 0; i < TILEOFF_NUM; i++)
		{
			MapTile* m = maps[i];
			m->mapImg = NULL;
			memset(m->rgTile, 0x00, sizeof(int) * RGTILE_X * RGTILE_Y);
			m->tileOff = tileOffSet[i];
			m->state = MapType_Nomal;

			ConnectTile* c = &ct[i];
			c->index = i;
			c->value = false;
			c->visit = false;
			c->tileOff = tileOffSet[i];
		}

		memset(randomOffCheck, false, sizeof(bool) * TILEOFF_NUM);
		memset(m, -1, sizeof(int) * MAPTILE_NUM);
		for (i = 0; i < MAPTILE_NUM; i++)
		{
			if (m[i] == -1 || randomOffCheck[m[i]])
				m[i] = random() % TILEOFF_NUM;

			if (randomOffCheck[m[i]] == false)
				randomOffCheck[m[i]] = true;
			else
				i--;
		}

		for (i = 0; i < MAPTILE_NUM; i++)
		{
			memcpy(maps[m[i]]->rgTile, Tile4way1, sizeof(int) * RGTILE_X * RGTILE_Y);
			ConnectTile* c = &ct[m[i]];
			c->value = true;
		}

		for (i = 0; i < TILEOFF_NUM; i++)
		{
			ConnectTile* c = &ct[i];
			connectCheck(c, conectCount);

			if (conectCount == MAPTILE_NUM)
				break;
			else if (conectCount > MAPTILE_NUM)
				printf("conectCount error\n");

			for (j = 0; j < TILEOFF_NUM; j++)
				c->visit = false;
			conectCount = 0;
		}
		
	}

	for (i = 0; i < TILEOFF_NUM; i++)
	{
		ConnectTile* c = &ct[i];
		pathTileCheck(c);
	}

	for (i = 0; i < TILEOFF_NUM; i++) // 임시
	{
		if (maps[i]->rgTile[0] != 0)
		{
			maps[i]->state = MapType_Treasure;
			break;
		}
	}

	for (i = TILEOFF_NUM-1; i > -1; i--) // 임시
	{
		if (maps[i]->rgTile[0] != 0)
		{
			maps[i]->state = MapType_Boss;
			break;
		}
	}

#if CHECK_CODE
	for (i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile[0] == 0)
			continue;

		MapTile* m = maps[i];
		iImage* img;

		MapObject** mo;
		m->mapObjNum = 3;
		m->mapObj = (MapObject**)malloc(sizeof(MapObject*) * m->mapObjNum);
		for (j = 0; j < m->mapObjNum; j++)
		{
			if (m->state == MapType_Boss)
			{
				mo = mapBossObjs[0];
			}
			else if (m->state == MapType_Treasure)
			{
				mo = mapTreasureObjs[0];
			}
			else
			{
				mo = mapObjs[random() % 5];
			}
			m->mapObj[j] = (MapObject*)malloc(sizeof(MapObject));
			MapObject* mobj = m->mapObj[j];

			mobj->objImg = mo[j]->objImg->copy();
			mobj->objPos = mo[j]->objPos;
			mobj->objTileNum = mo[j]->objTileNum;
			mobj->objTile = (int*)calloc(sizeof(int), m->mapObj[j]->objTileNum);
			memcpy(mobj->objTile, mo[j]->objTile, sizeof(int) * m->mapObj[j]->objTileNum);
			mobj->type = mo[j]->type;
			mobj->hitBox = mo[j]->hitBox;

			img = mobj->objImg;
			img->ratio = max(RGTILE_Width / img->tex->width, RGTILE_Height / img->tex->height) * 2.0f;
			img->reverse = REVERSE_HEIGHT;
			//item
		}

		//------------------------------------------------------------

		m->mapDoor = (MapObject**)malloc(sizeof(MapObject*) * 4);

		for (j = 0; j < 4; j++)
		{
			m->mapDoor[j] = (MapObject*)malloc(sizeof(MapObject));
			MapObject* mobj = m->mapDoor[j];
			iPoint drp = iPointZero;
			bool check = false;
			mobj->objImg = NULL;
			mobj->objTile = NULL;
			int angle = 0;
			if (j == 0 && m->rgTile[RGTILE_X * (RGTILE_Y / 2)] == DR)
			{
				check = true;
				angle = 180;

				mobj->objTileNum = 4;
				mobj->objTile = (int*)calloc(sizeof(int), mobj->objTileNum);
				for (int k = 0; k < mobj->objTileNum; k++)
				{
					mobj->objTile[k] = RGTILE_X * (RGTILE_Y / 2 - 2 + k);
					m->rgTile[mobj->objTile[k]] = WALLTILE;
				}
				drp = iPointMake(0, RGTILE_Height * (RGTILE_Y / 2 - 2))
					- iPointMake(RGTILE_Width * 4, RGTILE_Height);
			}
			if (j == 1 && m->rgTile[RGTILE_X * (RGTILE_Y / 2) + RGTILE_X - 1] == DR)
			{
				check = true;
				angle = 0;

				mobj->objTileNum = 4;
				mobj->objTile = (int*)calloc(sizeof(int), mobj->objTileNum);
				for (int k = 0; k < mobj->objTileNum; k++)
				{
					mobj->objTile[k] = RGTILE_X * (RGTILE_Y / 2 - 2 + k) + RGTILE_X - 1;
					m->rgTile[mobj->objTile[k]] = WALLTILE;
				}
				drp = iPointMake(RGTILE_Width * (RGTILE_X - 1), RGTILE_Height * (RGTILE_Y / 2 - 2))
					- iPointMake(0, RGTILE_Height);

			}
			if (j == 2 && m->rgTile[RGTILE_X / 2] == DR)
			{
				check = true;
				angle = 90;

				mobj->objTileNum = 4;
				mobj->objTile = (int*)calloc(sizeof(int), mobj->objTileNum);
				for (int k = 0; k < mobj->objTileNum; k++)
				{
					mobj->objTile[k] = RGTILE_X / 2 - 2 + k;
					m->rgTile[mobj->objTile[k]] = WALLTILE;
				}
				drp = iPointMake(RGTILE_Width * (RGTILE_X / 2 - 2), 0)
					- iPointMake(RGTILE_Width, RGTILE_Height * 4);

			}
			if (j == 3 && m->rgTile[RGTILE_X * (RGTILE_Y - 1) + RGTILE_X / 2] == DR)
			{
				check = true;
				angle = 270;

				mobj->objTileNum = 4;
				mobj->objTile = (int*)calloc(sizeof(int), mobj->objTileNum);
				for (int k = 0; k < mobj->objTileNum; k++)
				{
					mobj->objTile[k] = RGTILE_X * (RGTILE_Y - 1) + RGTILE_X / 2 - 2 + k;
					m->rgTile[mobj->objTile[k]] = WALLTILE;
				}
				drp = iPointMake(RGTILE_Width * (RGTILE_X / 2 - 2), RGTILE_Height * (RGTILE_Y - 1))
					- iPointMake(RGTILE_Width, 0);
			}

			if (check)
			{
				mobj->objImg = imgDoorNormal->copy();
				img = mobj->objImg;

				img->ratio = RGTILE_Height * 4.0f / img->tex->height * 1.5f;
				img->reverse = REVERSE_HEIGHT;
				img->angle = angle;
				img->lockAngle = true;

				mobj->objPos = drp;
				mobj->type = MapObj_Door;
			}
		}
		//--------------------------------------------------------------------
		// 맵 이미지 애니메이션

		m->mapImg = imgRoom->copy();
		img = m->mapImg;	

		img->reverse = REVERSE_HEIGHT;
		//img->animation = true;
		//img->_aniDt = 0.5f;
	}
#endif
}

void reSetTile()
{
#if CHECK_CODE
	int i, j;
	// 타일 초기화
	for (i = 0; i < TILEOFF_NUM; i++)
	{
		MapTile* m = maps[i];
		if (m->rgTile[0] == 0) continue;

		delete m->mapImg;

		for (j = 0; j < m->mapObjNum; j++)
		{
			m->mapObj[j]->type = MapObj_Nomal;
			delete m->mapObj[j]->objImg;

			if(m->mapObj[j]->objTile)
				free(m->mapObj[j]->objTile);

			free(m->mapObj[j]);
		}

		free(m->mapObj);

		for (j = 0; j < 4; j++)
		{
			m->mapDoor[j]->type = MapObj_Nomal;

			if (m->mapDoor[j]->objImg)
			{
				delete m->mapDoor[j]->objImg;
				free(m->mapDoor[j]->objTile);
			}
			free(m->mapDoor[j]);
		}
		free(m->mapDoor);
	}
#endif
}

void connectCheck(ConnectTile* c, int& count)
{
	if (c->visit || c->value == false)
		return;

	c->visit = true;
	count++;

	int index = c->index;
	int x = c->index % TILEOFF_SQRT;
	int y = c->index / TILEOFF_SQRT;
	if (x > 0)					connectCheck(&ct[c->index- 1], count);
	if (x < TILEOFF_SQRT - 1)	connectCheck(&ct[c->index+ 1], count);
	if (y > 0)					connectCheck(&ct[c->index- TILEOFF_SQRT], count);
	if (y < TILEOFF_SQRT - 1)	connectCheck(&ct[c->index+ TILEOFF_SQRT], count);
}

#define MAPSIZE(index, tile) memcpy(maps[index]->rgTile, tile, sizeof(int) * RGTILE_X * RGTILE_Y)
void pathTileCheck(ConnectTile* c)
{
	if (c->value == false)
		return;

	int index = c->index;
	int x = c->index % TILEOFF_SQRT;
	int y = c->index / TILEOFF_SQRT;

	bool l = false;
	bool r = false;
	bool u = false;
	bool d = false;

	if (x > 0)					l = ct[c->index - 1].value;
	if (x < TILEOFF_SQRT - 1)	r = ct[c->index + 1].value;
	if (y > 0)					u = ct[c->index - TILEOFF_SQRT].value;
	if (y < TILEOFF_SQRT - 1)	d = ct[c->index + TILEOFF_SQRT].value;

	int pathNum = l + r + u + d;
	switch (pathNum) {
	case 4: MAPSIZE(index, Tile4way1); break;
	case 3:
	{
		if (r && u && d)	  MAPSIZE(index, Tile3way1);
		else if (l && u && d) MAPSIZE(index, Tile3way2);
		else if (l && r && d) MAPSIZE(index, Tile3way3);
		else if (l && r && u) MAPSIZE(index, Tile3way4);
		break;
	}
	case 2:
	{
		if (l && r) MAPSIZE(index, Tile2way1);
		else if (u && d) MAPSIZE(index, Tile2way2);
		else if (l && u) MAPSIZE(index, Tile2way3);
		else if (r && u) MAPSIZE(index, Tile2way4);
		else if (l && d) MAPSIZE(index, Tile2way5);
		else if (r && d) MAPSIZE(index, Tile2way6);
		break;
	}
	case 1:
	{
		if (l) MAPSIZE(index, Tile1way1);
		else if (r) MAPSIZE(index, Tile1way2);
		else if (u) MAPSIZE(index, Tile1way3);
		else if (d) MAPSIZE(index, Tile1way4);
		break;
	}
	default:
		MAPSIZE(index, Tile0way1);
		break;
	}
}

void freeRoomTile()
{
#if CHECK_CODE
	freeObjImage();
	freeObject();

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile[0] != 0)
		{
			delete maps[i]->mapImg;

			int num = maps[i]->mapObjNum;
			if (num > 0)
			{
				for (int j = 0; j < num; j++)
				{
					delete maps[i]->mapObj[j]->objImg;
					free(maps[i]->mapObj[j]->objTile);
					free(maps[i]->mapObj[j]);
				}
				free(maps[i]->mapObj);
			}

			for (int j = 0; j < 4; j++)
			{
				if (maps[i]->mapDoor[j]->objImg)
				{
					delete maps[i]->mapDoor[j]->objImg;
					free(maps[i]->mapDoor[j]->objTile);
				}
			}
			free(maps[i]->mapDoor);
		}

		free(maps[i]->rgTile);
		free(maps[i]);
	}
	free(maps);
#endif
}

void drawObject(float dt)
{
#if CHECK_CODE
	int i, j;
	bool check = true;
	MapTile* m = maps[pc->tileNumber];
	for (i = 0; i < monsterNum; i++)
	{
		if (totalMonster[i]->tileNumber == pc->tileNumber)
		{
			//닫힘
			check = false;
			break;
		}
	}

	{
#if 0
		//int tmp = check ? 01 : WALLTILE;
#else
		int tmp = check ? 01 : 01;
#endif
		for (j = 0; j < 3; j++) // 나무통
		{
			MapObject* mobj = m->mapObj[j];

			for (int k = 0; k < mobj->objTileNum; k++)
			{
				m->rgTile[mobj->objTile[k]] = tmp;
			}
		}

		for (j = 0; j < 4; j++)
		{
			MapObject* mobj = m->mapDoor[j];
			for (int k = 0; k < mobj->objTileNum; k++)
			{
				m->rgTile[mobj->objTile[k]] = tmp;
			}
		}
	}

	for (i = 0; i < 3; i++)
	{
		if (m->mapObj[i]->objImg)
		{
			MapObject* mobj = m->mapObj[i];
			iPoint p = m->tileOff + mobj->objPos + SET_DRAW_OFF;
			iImage* img = mobj->objImg;
			//img->setTexAtIndex(check);
			img->paint(dt, p);
		}
	}

	for (i = 0; i < 4; i++)
	{
		if (m->mapDoor[i]->objImg)
		{
			MapObject* mobj = m->mapDoor[i];
			iPoint p = m->tileOff + mobj->objPos + SET_DRAW_OFF;
			iImage* img = mobj->objImg;
			img->setTexAtIndex(check);
			img->paint(dt, p);
		}
	}
#endif
}

void drawRoomTile(float dt)
{
	MapTile* m = maps[pc->tileNumber];
	iPoint p = m->tileOff + SET_DRAW_OFF - iPointMake(RGTILE_Width * 6, RGTILE_Height * 4.5);

	m->mapImg->paint(dt, p);

	drawObject(dt);

	{ 
		int num = RGTILE_X * RGTILE_Y;
		iPoint tmp = m->tileOff + SET_DRAW_OFF;
		setRGBA(0, 1, 0, 0.1f);
		for (int i = 0; i < num; i++)
		{
			if (m->rgTile[i] == WALLTILE)
			{
				fillRect(tmp.x + RGTILE_Width * (i % RGTILE_X), tmp.y + RGTILE_Height * (i / RGTILE_X),
					RGTILE_Width, RGTILE_Height);
			}
		}
	}

	setRGBA(1, 1, 1, 1);
}

void passTileAnimation(float dt)
{
	passTile* t = &pt;
	if (t->tileNum == -1)
		t->tileNum = pc->tileNumber;

	if (t->tileNum == pc->tileNumber && t->passAniDt == _passAniDt)
		return;

	if (nextStage)
	{
		t->tileNum = pc->tileNumber;
		return;
	}

	if (t->passAniDt == _passAniDt)
	{
		t->sp = iPointZero - maps[t->tileNum]->tileOff;
		t->tp = iPointZero - maps[pc->tileNumber]->tileOff;
		t->passAniDt = 0.0f;
	}

	pc->camPosition = linear(t->passAniDt / _passAniDt, t->sp, t->tp);

	for (int i = 0; i < 2; i++)
	{
		int tn = (i == 0 ? t->tileNum : pc->tileNumber);

		iPoint p = maps[tn]->tileOff + SET_DRAW_OFF;

		maps[tn]->mapImg->paint(dt, p);
	}

	t->passAniDt += dt;
	if (t->passAniDt > _passAniDt)
	{
		t->passAniDt = _passAniDt;
		t->tileNum = pc->tileNumber;
		pc->camPosition = t->tp;
		return;
	}
}

bool passAni()
{
	return pt.passAniDt < _passAniDt;
}

void wallCheck(bool checkFall, MapTile* tile, iPoint& pos, iPoint mp, float halfOfTexW, float halfOfTexH)
{
	int i, j;
	MapTile* t = tile;
	if (tile->rgTile == NULL)
		return;

	bool evase = (pc->act == Act_evasion);
	if (mp.x < 0)
	{
		int LX = pos.x - t->tileOff.x ;							LX /= RGTILE_Width;
		int TLY = pos.y - t->tileOff.y ;						TLY /= RGTILE_Height;
		int BLY = pos.y + halfOfTexH * 2.0f - t->tileOff.y ;	BLY /= RGTILE_Height;
		int min = t->tileOff.x - HALF_OF_TEX_WIDTH * 3;
		if(evase) 
			min = t->tileOff.x;
		for (i = LX - 1; i > -1; i--)
		{
			bool stop = false;
			for (j = TLY; j < BLY + 1; j++)
			{
				if (t->rgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					min = t->tileOff.x + RGTILE_Width * (i + 1);
					break;
				}

				if (checkFall)
				{
					if (tile->rgTile[RGTILE_X * j + i] == FALLTILE)
					{
						stop = true;
						min = t->tileOff.x + RGTILE_Width * (i + 1);
						break;
					}
				}
			}
			if (stop)
				break;
		}
		pos.x += mp.x;
		if (pos.x < min + 1)
			pos.x = min + 1;
	}
	else if (mp.x > 0)
	{
		int RX = pos.x + halfOfTexW * 2.0f - t->tileOff.x;		RX /= RGTILE_Width;
		int TRY = pos.y - t->tileOff.y;							TRY /= RGTILE_Height;
		int BRY = pos.y + halfOfTexH * 2.0f - t->tileOff.y;		BRY /= RGTILE_Height;
		int max = t->tileOff.x + RGTILE_X * RGTILE_Width - 1 + HALF_OF_TEX_WIDTH * 3;
		if (evase)
			 max = t->tileOff.x + RGTILE_X * RGTILE_Width - 1;
		for (i = RX + 1; i < RGTILE_X; i++)
		{
			bool stop = false;
			for (j = TRY; j < BRY + 1; j++)
			{
				if (t->rgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					max = t->tileOff.x + RGTILE_Width * i - 1;
					break;
				}

				if (checkFall)
				{
					if (t->rgTile[RGTILE_X * j + i] == FALLTILE)
					{
						stop = true;
						max = t->tileOff.x + RGTILE_Width * i - 1;
						break;
					}
				}
			}
			if (stop)
				break;
		}

		pos.x += mp.x;
		if (pos.x > max - halfOfTexW * 2.0f - 1)
			pos.x = max - halfOfTexW * 2.0f - 1;
	}

	if (mp.y < 0)
	{
		int TY = pos.y - t->tileOff.y;							TY /= RGTILE_Height;
		int TLX = pos.x - t->tileOff.x;							TLX /= RGTILE_Width;
		int TRX = pos.x + halfOfTexW * 2.0f - t->tileOff.x;		TRX /= RGTILE_Width;
		int min = t->tileOff.y - HALF_OF_TEX_HEIGHT * 3;
		if (evase)
		  min = t->tileOff.y;
		for (j = TY - 1; j > -1; j--)
		{
			bool stop = false;
			for (i = TLX; i < TRX + 1; i++)
			{
				if (t->rgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					min = t->tileOff.y + RGTILE_Height * (j + 1);
					break;
				}

				if (checkFall)
				{
					if (t->rgTile[RGTILE_X * j + i] == FALLTILE)
					{
						stop = true;
						min = t->tileOff.y + RGTILE_Height * (j + 1);
						break;
					}
				}
			}
			if (stop)
				break;
		}
		pos.y += mp.y;
		if (pos.y < min + 1)
			pos.y = min + 1;
	}
	else if (mp.y > 0)
	{
		int BY =  pos.y + halfOfTexH * 2.0f - t->tileOff.y;		BY /= RGTILE_Height;
		int BLX = pos.x - t->tileOff.x;							BLX /= RGTILE_Width;
		int BRX = pos.x + halfOfTexW * 2.0f - t->tileOff.x;		BRX /= RGTILE_Width;
		int max = t->tileOff.y + RGTILE_Y * RGTILE_Height - 1 + HALF_OF_TEX_HEIGHT * 3;
		if (evase)
		  max = t->tileOff.y + RGTILE_Y * RGTILE_Height - 1;

		for (j = BY + 1; j < RGTILE_Y; j++)
		{
			bool stop = false;
			for (i = BLX; i < BRX + 1; i++)
			{
				if (t->rgTile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					max = t->tileOff.y + RGTILE_Height * j - 1;
					break;
				}

				if (checkFall)
				{
					if (t->rgTile[RGTILE_X * j + i] == FALLTILE)
					{
						stop = true;
						max = t->tileOff.y + RGTILE_Height * j - 1;
						break;
					}
				}
			}
			if (stop)
				break;
		}

		pos.y += mp.y;
		if (pos.y > max - halfOfTexH * 2.0f - 1)
			pos.y = max - halfOfTexH * 2.0f - 1;
	}
}


//--------------------------------------------------------------------------------------------
// createObject

void createObjImage()
{
#if CHECK_CODE
	int i, j;

	iImage* img = new iImage();
	Texture* tex;
	//-----------------------------------------------------------------------
	// 맵 이미지

	iPoint p = iPointMake((RGTILE_X + 12) * RGTILE_Width,
		(RGTILE_Y + 9) * RGTILE_Height);
	tex = createTexture(p.x, p.y);
	fbo->bind(tex);
	fbo->setSize(p.x, p.y);

	Texture* texRoom = createImage("assets/object/room.png");
	setRGBA(1, 1, 1, 0.8f);
	drawImage(texRoom, 0, 0,
		0, 0, texRoom->width, texRoom->height,
		TOP | LEFT, p.x / texRoom->width, p.y / texRoom->height,
		2, 0);
	freeImage(texRoom);
	setRGBA(1, 1, 1, 1);

	fbo->backSize();
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	imgRoom = img;

	//----------------------------------------------------------------------
	// 다음스테이지 문

	const char* strNextDoor[2] = {
		"trapdoor_1", "trapdoor_2"
	};

	img = new iImage();
	p = iPointMake(RGTILE_Width * 4, RGTILE_Height * 4);
	for (i = 0; i < 2; i++)
	{
		tex = createTexture(p.x, p.y);
		fbo->bind(tex);
		fbo->setSize(p.x, p.y);

		Texture* texND = createImage("assets/object/%s.png", strNextDoor[i]);
		drawImage(texND, 0, 0,
			0, 0, texND->width, texND->height,
			TOP | LEFT, p.x / texND->width, p.y / texND->height,
			2, 0);
		freeImage(texND);

		fbo->backSize();
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);
	}

	imgNextDoor = img;

	//-----------------------------------------------------------------------
	// 오브젝트 이미지 
	const char* strBarrel[4] = {
		"barrel_1", "barrel_2", "door_close_east", "door_open_east"
	};

	img = new iImage();
	p = iPointMake(128, 157);
	for (i = 0; i < 2; i++)
	{
		tex = createTexture(p.x, p.y);
		fbo->bind(tex);
		fbo->setSize(p.x, p.y);

		Texture* texBar = createImage("assets/object/%s.png", strBarrel[i]);
		drawImage(texBar, 0, 0, TOP | LEFT);
		freeImage(texBar);

		fbo->backSize();
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);
	}
	imgBarrel = img;

	img = new iImage();
	for (i = 0; i < 2; i++)
	{
		tex = createImage("assets/object/%s.png", strBarrel[i + 2]);
		img->addObject(tex);
		freeImage(tex);
	}
	imgDoorNormal = img;
#endif
}

void freeObjImage()
{
	delete imgRoom;
#if CHECK_CODE
	delete imgNextDoor;

	delete imgBarrel;
	delete imgDoorNormal;
#endif
}

void createObject()
{
#if CHECK_CODE
	int i, j, k;
	iImage* img;

	int numList = 5;
	int numObj = 3;

	mapObjs = (MapObject***)malloc(sizeof(MapObject**) * numList);

	iPoint p[3] = {
		{8,  6},
		{24, 12},
		{16, 18}
	};

	for (i = 0; i < numList; i++)
	{
		mapObjs[i] = (MapObject**)malloc(sizeof(MapObject*) * numObj); // mapobjNum
		p[0] += iPointMake(1, 1);
		p[1] += iPointMake(-2, 0);
		p[2] += iPointMake(-1, -1);

		for (j = 0; j < numObj; j++)
		{
			mapObjs[i][j] = (MapObject*)malloc(sizeof(MapObject));
			MapObject* mo = mapObjs[i][j];

			mo->objPos = iPointMake(RGTILE_Width, RGTILE_Height) * p[j];
			mo->objTileNum = 4;
			mo->objTile = (int*)calloc(sizeof(int), mo->objTileNum);
			for (k = 0; k < mo->objTileNum; k++)
				mo->objTile[k] = RGTILE_X * (p[j].y + k / 2) + p[j].x + k % 2;

			mo->type = MapObj_Broke;
			mo->hitBox = iRectMake(mo->objPos.x, mo->objPos.y,
				RGTILE_Width * 2, RGTILE_Height * 2);

			mo->objImg = imgBarrel->copy();
			img = mo->objImg;

			img->ratio = max(RGTILE_Width / img->tex->width, RGTILE_Height / img->tex->height) * 2.0f;
			img->reverse = REVERSE_HEIGHT;
		}
	}

	numList = 1;
	numObj = 3;
	mapBossObjs = (MapObject***)malloc(sizeof(MapObject**) * numList);
	p[0] = { 16,  6 };
	p[1] = { 16,  12 };
	p[2] = { 16,  18 };

	for (i = 0; i < numList; i++)
	{
		mapBossObjs[i] = (MapObject**)malloc(sizeof(MapObject*) * numObj); // mapobjNum

		for (j = 0; j < numObj; j++)
		{
			mapBossObjs[i][j] = (MapObject*)malloc(sizeof(MapObject));
			MapObject* mo = mapBossObjs[i][j];

			mo->objPos = iPointMake(RGTILE_Width, RGTILE_Height) * p[j];
			mo->objTileNum = 4;
			mo->objTile = (int*)calloc(sizeof(int), mo->objTileNum);
			for (k = 0; k < mo->objTileNum; k++)
				mo->objTile[k] = RGTILE_X * (p[j].y + k / 2) + p[j].x + k % 2;

			mo->type = MapObj_Broke;
			mo->hitBox = iRectMake(mo->objPos.x, mo->objPos.y,
				RGTILE_Width * 2, RGTILE_Height * 2);

			mo->objImg = imgBarrel->copy();
			img = mo->objImg;

			float ratio = max(RGTILE_Width / img->tex->width, RGTILE_Height / img->tex->height) * 2.0f;
			img->ratio = ratio;
			img->reverse = REVERSE_HEIGHT;
		}
	}


	numList = 1;
	numObj = 3;
	mapTreasureObjs = (MapObject***)malloc(sizeof(MapObject**) * numList);
	p[0] = { 8,  12 };
	p[1] = { 16,  12 };
	p[2] = { 24,  12 };

	for (i = 0; i < numList; i++)
	{
		mapTreasureObjs[i] = (MapObject**)malloc(sizeof(MapObject*) * numObj); // mapobjNum

		for (j = 0; j < numObj; j++)
		{
			mapTreasureObjs[i][j] = (MapObject*)malloc(sizeof(MapObject));
			MapObject* mo = mapTreasureObjs[i][j];

			mo->objPos = iPointMake(RGTILE_Width, RGTILE_Height) * p[j];
			mo->objTileNum = 4;
			mo->objTile = (int*)calloc(sizeof(int), mo->objTileNum);
			for (k = 0; k < mo->objTileNum; k++)
				mo->objTile[k] = RGTILE_X * (p[j].y + k / 2) + p[j].x + k % 2;

			mo->type = MapObj_Broke;
			mo->hitBox = iRectMake(mo->objPos.x, mo->objPos.y,
				RGTILE_Width * 2, RGTILE_Height * 2);

			mo->objImg = imgBarrel->copy();
			img = mo->objImg;

			float ratio = max(RGTILE_Width / img->tex->width, RGTILE_Height / img->tex->height) * 2.0f;
			img->ratio = ratio;
			img->reverse = REVERSE_HEIGHT;
		}
	}
#endif
}

void freeObject()
{
#if CHECK_CODE
	int i, j;
	int numList = 5;
	int numObj = 3;

	for (i = 0; i< numList; i++)
	{
		for (j = 0; j < numObj; j++)
		{
			delete mapObjs[i][j]->objImg;
			free(mapObjs[i][j]->objTile);
			free(mapObjs[i][j]);
		}
		free(mapObjs[i]);
	}
	free(mapObjs);

	numList = 1;
	numObj = 3;
	for (i = 0; i < numList; i++)
	{
		for (j = 0; j < numObj; j++)
		{
			delete mapBossObjs[i][j]->objImg;
			free(mapBossObjs[i][j]->objTile);
			free(mapBossObjs[i][j]);
		}
		free(mapBossObjs[i]);
	}
	free(mapBossObjs);

	numList = 1;
	numObj = 3;
	for (i = 0; i < numList; i++)
	{
		for (j = 0; j < numObj; j++)
		{
			delete mapTreasureObjs[i][j]->objImg;
			free(mapTreasureObjs[i][j]->objTile);
			free(mapTreasureObjs[i][j]);
		}
		free(mapTreasureObjs[i]);
	}
	free(mapTreasureObjs);
#endif
}