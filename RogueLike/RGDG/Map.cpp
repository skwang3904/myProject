#include "Map.h"

#include "PlayerChar.h"
#include "Tile.h"
#include "Monster.h"

struct passTile {
	int tileNum;
	iPoint sp;
	iPoint tp;
	float passAniDt;
};
static passTile pt;
#define _passAniDt 0.2f

iPoint displayCenterPos;

MapTile** maps;

struct ConnectTile
{
	int index; // 타일 넘버
	bool value; //
	bool visit;
	iPoint tileOff;
};
ConnectTile ct[TILE_TOTAL_NUM];

void connectCheck(ConnectTile* c, int& count);
void pathTileCheck(ConnectTile* c);

void createMap()
{
	int i, j;
	bool randomOffCheck[TILE_TOTAL_NUM];
	int m[TILE_CONNECT_NUM];
	int conectCount = 0;
	while (conectCount < TILE_CONNECT_NUM)
	{
		for (i = 0; i < TILE_TOTAL_NUM; i++)
		{
			MapTile* m = maps[i];
			m->img = NULL;
			memset(m->tile, 0x00, sizeof(int8) * TILE_NUM_X * TILE_NUM_Y);
			m->state = MapType_Nomal;

			ConnectTile* c = &ct[i];
			c->index = i;
			c->value = false;
			c->visit = false;
			c->tileOff = m->tileOff;
		}

		memset(randomOffCheck, false, sizeof(bool) * TILE_TOTAL_NUM);
		memset(m, -1, sizeof(int) * TILE_CONNECT_NUM);
		for (i = 0; i < TILE_CONNECT_NUM; i++)
		{
			if (m[i] == -1 || randomOffCheck[m[i]])
				m[i] = random() % TILE_TOTAL_NUM;

			if (randomOffCheck[m[i]] == false)
				randomOffCheck[m[i]] = true;
			else
				i--;
		}

		for (i = 0; i < TILE_CONNECT_NUM; i++)
		{
			memcpy(maps[m[i]]->tile, tileWay[1].tile, sizeof(int8) * TILE_NUM_X * TILE_NUM_Y);
			ConnectTile* c = &ct[m[i]];
			c->value = true;
		}

		for (i = 0; i < TILE_TOTAL_NUM; i++)
		{
			ConnectTile* c = &ct[i];
			connectCheck(c, conectCount);

			if (conectCount == TILE_CONNECT_NUM)
				break;
			else if (conectCount > TILE_CONNECT_NUM)
				printf("conectCount error\n");

			for (j = 0; j < TILE_TOTAL_NUM; j++)
				c->visit = false;
			conectCount = 0;
		}

	}

	for (i = 0; i < TILE_TOTAL_NUM; i++)
	{
		ConnectTile* c = &ct[i];
		pathTileCheck(c);
	}

	for (i = 0; i < TILE_TOTAL_NUM; i++) // 임시
	{
		if (maps[i]->tile[0] != 0)
		{
			maps[i]->state = MapType_Treasure;
			break;
		}
	}

	for (i = TILE_TOTAL_NUM - 1; i > -1; i--) // 임시
	{
		if (maps[i]->tile[0] != 0)
		{
			maps[i]->state = MapType_Boss;
			break;
		}
	}
}

void connectCheck(ConnectTile* c, int& count)
{
	if (c->visit || c->value == false)
		return;

	c->visit = true;
	count++;

	int index = c->index;
	int x = c->index % TILE_TOTAL_SQRT;
	int y = c->index / TILE_TOTAL_SQRT;
	if (x > 0)						connectCheck(&ct[c->index - 1], count);
	if (x < TILE_TOTAL_SQRT - 1)	connectCheck(&ct[c->index + 1], count);
	if (y > 0)						connectCheck(&ct[c->index - TILE_TOTAL_SQRT], count);
	if (y < TILE_TOTAL_SQRT - 1)	connectCheck(&ct[c->index + TILE_TOTAL_SQRT], count);
}

void pathTileCheck(ConnectTile* c)
{
	if (c->value == false)
		return;

	int index = c->index;
	int x = c->index % TILE_TOTAL_SQRT;
	int y = c->index / TILE_TOTAL_SQRT;

	bool l = false;
	bool r = false;
	bool u = false;
	bool d = false;

	if (x > 0)						l = ct[c->index - 1].value;
	if (x < TILE_TOTAL_SQRT - 1)	r = ct[c->index + 1].value;
	if (y > 0)						u = ct[c->index - TILE_TOTAL_SQRT].value;
	if (y < TILE_TOTAL_SQRT - 1)	d = ct[c->index + TILE_TOTAL_SQRT].value;
	
	int pathNum = l + r + u + d;
	switch (pathNum) {
	case 4: pathNum = 1; break;
	case 3:
	{
		if		(r && u && d) pathNum = 2;
		else if (l && u && d) pathNum = 3;
		else if (l && r && d) pathNum = 4;
		else if (l && r && u) pathNum = 5;
		break;
	}
	case 2:
	{
		if		(l && r) pathNum = 6;
		else if (u && d) pathNum = 7;
		else if (l && u) pathNum = 8;
		else if (r && u) pathNum = 9;
		else if (l && d) pathNum = 10;
		else if (r && d) pathNum = 11;
		break;
	}
	case 1:
	{
		if		(l)	pathNum = 12;
		else if (r) pathNum = 13;
		else if (u) pathNum = 14;
		else if (d) pathNum = 15;
		break;
	}
	default:
		pathNum = 0;
		break;
	}

#define TILECOPY(index, t) memcpy(maps[index]->tile, t, sizeof(int8) * TILE_NUM_X * TILE_NUM_Y)
	TILECOPY(index, tileWay[pathNum].tile);

}

//---------------------------------------------------------------------------------------------

void wallCheck(Object* obj, iPoint mp)
{
	int i, j;
	MapTile* t = maps[player->mapNumber];
	if (t->tile == NULL)
		return;

	iPoint to = t->tileOff;
	iSize size = iSizeMake(obj->img->tex->width, obj->img->tex->height) * 0.5f;
	iPoint pos = obj->position + iPointMake(size.width, size.height);

	iSize tmp = size * 2.0f * 0;
	
	if (mp.x < 0)
	{
		int LX = pos.x - to.x;						LX /= TILE_Width;
		int TLY = pos.y - to.y;						TLY /= TILE_Height;
		int BLY = pos.y + tmp.height - to.y;		BLY /= TILE_Height;
		int min = to.x - size.width * 4.0f;
	
		for (i = LX - 1; i > -1; i--)
		{
			bool stop = false;
			for (j = TLY; j < BLY + 1; j++)
			{
				if (t->tile[TILE_NUM_X * j + i] == WALLTILE)
				{
					stop = true;
					min = to.x + TILE_Width * (i + 1);
					break;
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
		int RX = pos.x + tmp.width - to.x;			RX /= TILE_Width;
		int TRY = pos.y - to.y;						TRY /= TILE_Height;
		int BRY = pos.y + tmp.height - to.y;		BRY /= TILE_Height;
		int max = to.x + TILE_NUM_X * TILE_Width - 1 + size.width * 4.0f;
	
		for (i = RX + 1; i < TILE_NUM_X; i++)
		{
			bool stop = false;
			for (j = TRY; j < BRY + 1; j++)
			{
				if (t->tile[TILE_NUM_X * j + i] == WALLTILE)
				{
					stop = true;
					max = to.x + TILE_Width * i - 1;
					break;
				}
			}
			if (stop)
				break;
		}
	
		pos.x += mp.x;
		if (pos.x > max - tmp.width - 1)
			pos.x = max - tmp.width - 1;
	}
	
	if (mp.y < 0)
	{
		int TY = pos.y - to.y;							TY /= TILE_Height;
		int TLX = pos.x - to.x;							TLX /= TILE_Width;
		int TRX = pos.x + tmp.width - to.x;				TRX /= TILE_Width;
		int min = to.y - size.height * 4.0f;
	
		for (j = TY - 1; j > -1; j--)
		{
			bool stop = false;
			for (i = TLX; i < TRX + 1; i++)
			{
				if (t->tile[TILE_NUM_X * j + i] == WALLTILE)
				{
					stop = true;
					min = to.y + TILE_Height * (j + 1);
					break;
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
		int BY = pos.y + tmp.height - to.y;			BY /= TILE_Height;
		int BLX = pos.x - to.x;						BLX /= TILE_Width;
		int BRX = pos.x + tmp.width - to.x;			BRX /= TILE_Width;
		int max = to.y + TILE_NUM_Y * TILE_Height - 1 + size.height * 4.0f;
	
		for (j = BY + 1; j < TILE_NUM_Y; j++)
		{
			bool stop = false;
			for (i = BLX; i < BRX + 1; i++)
			{
				if (t->tile[TILE_NUM_X * j + i] == WALLTILE)
				{
					stop = true;
					max = to.y + TILE_Height * j - 1;
					break;
				}
			}
			if (stop)
				break;
		}
	
		pos.y += mp.y;
		if (pos.y > max - tmp.height - 1)
			pos.y = max - tmp.height - 1;
	}

	obj->position = pos -iPointMake(size.width, size.height);
}

//-----------------------------------------------------------------------------
// MapObject
MapObject** mapObj;
int mapObjNum;

iImage* imgNextDoor = NULL;
MapObject::MapObject(int index) : Object(index)
{
	mapNumber = 0;

	alive = false;
	position = iPointZero;
	touchRect = iRectZero;

	tileNum = 0;
	value = 0;
}

MapObject::~MapObject()
{

}

//----------------------------------------------------------------------------------
MapObjectDoor** _mapObjectDoor;
int mapObjectDoorNum;

MapObjectDoor::MapObjectDoor(int index) : MapObject(index)
{
	iImage* img;
	Texture* tex;

	if (imgNextDoor == NULL)
	{
		img = new iImage();
		iSize size = iSizeMake(TILE_Width, TILE_Height);
		Texture* tex = createTexture(size.width, size.height);

		fbo->bind(tex);
		setRGBA(0, 1, 0, 1);
		fillRect(0, 0, size.width, size.height);
		setRGBA(1, 0, 1, 1);
		fillRect(size.width * 0.25f, size.height * 0.25f, size.width * 0.5f, size.height * 0.5f);
		setRGBA(1, 1, 1, 1);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);

		this->img = img;
	}
	else
	{
		this->img = imgNextDoor->copy();
	}
	mapNumber = 0;

	alive = true;
	position = maps[mapNumber]->tileOff + iPointMake(12 * TILE_Width, 5 * TILE_Height);
	touchRect = iRectMake(position.x, position.y, TILE_Width, TILE_Height);


	tileNum = 4;
	tileNumber = (int*)calloc(sizeof(int), tileNum);

	value = 1;
}

MapObjectDoor::~MapObjectDoor()
{
	delete img; // test

	free(tileNumber);
}

void MapObjectDoor::paint(float dt, iPoint off)
{
#if 1 // touch obj
	if (containRect(touchRect, player->touchRect))
	{
		printf("touch obj\n");
		//alive = false;
		action();
	}
#endif

	iRect rt = touchRect;
	rt.origin += off;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

	img->paint(dt, position + off);
}

void MapObjectDoor::action()
{
	if (alive)
	{
		alive = false;
		for (int i = 0; i < tileNum; i++)
		{
			if (tileNumber[i] == 0) continue;

			maps[mapNumber]->tile[tileNumber[i]] = 01;
		}

		return;
	}
}

//----------------------------------------------------------------------------------

void loadMap()
{
	int i, j;
	iPoint tileOffSet[TILE_TOTAL_NUM];
	for (int i = 0; i < TILE_TOTAL_NUM; i++)
	{
		tileOffSet[i] = iPointMake(TILE_NUM_X * TILE_Width * (i % TILE_TOTAL_SQRT),
			TILE_NUM_Y * TILE_Height * (i / TILE_TOTAL_SQRT));
	}

	displayCenterPos = iPointMake(devSize.width / 2.0f, devSize.height / 2.0f)
		- iPointMake(TILE_NUM_X * TILE_Width / 2.0f, TILE_NUM_Y * TILE_Height / 2.0f);

	pt.tileNum = -1;
	pt.sp = iPointZero;
	pt.tp = iPointZero;
	pt.passAniDt = _passAniDt;

	maps = (MapTile**)malloc(sizeof(MapTile*) * TILE_TOTAL_NUM);
	for (i = 0; i < TILE_TOTAL_NUM; i++)
	{
		maps[i] = (MapTile*)malloc(sizeof(MapTile));
		maps[i]->img = NULL;

		maps[i]->tile = (int8*)calloc(sizeof(int8), TILE_NUM_X * TILE_NUM_Y);
		maps[i]->tileOff = tileOffSet[i];
	}

	// create random map
	createMap();

	int num = TILE_TOTAL_NUM;
	int t = TILE_NUM_X * TILE_NUM_Y;

	iSize size = iSizeMake(TILE_NUM_X * TILE_Width, TILE_NUM_Y * TILE_Height);
	for (i = 0; i < num; i++)
	{
		if (maps[i]->tile[0] != 0)
		{
			Texture* tex = createTexture(size.width, size.height);
			iImage* img = new iImage();

			fbo->bind(tex);
			for (int j = 0; j < t; j++)
			{
				int8 n = maps[i]->tile[j];
				if (n == MOVETILE)		setRGBA(MOVETILE_RGBA);
				else if (n == WALLTILE)	setRGBA(WALLTILE_RGBA);
				else if (n == FALLTILE)	setRGBA(FALLTILE_RGBA);
				else					setRGBA(1, 1, 1, 1);

				fillRect(TILE_Width * (j % TILE_NUM_X), TILE_Height * (j / TILE_NUM_X),
					TILE_Width, TILE_Height);
			}
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);

			img->reverse = REVERSE_HEIGHT;
			maps[i]->img = img;
		}

	}

	//-----------------------------------------------------------------------------
	mapObjectDoorNum = 4;
	_mapObjectDoor = (MapObjectDoor**)malloc(sizeof(MapObjectDoor*) * mapObjectDoorNum);

	mapObjNum = mapObjectDoorNum;
	mapObj = (MapObject**)malloc(sizeof(MapObject*) * mapObjNum);
	for (i = 0; i < mapObjectDoorNum; i++)
	{
		// l r u d
		_mapObjectDoor[i] = new MapObjectDoor(0);
		mapObj[i] = _mapObjectDoor[i];
		
		MapObject* mo = mapObj[i];
		mo->position = iPointMake(TILE_Width * 4 + TILE_Width * 2 * i, TILE_Height * 3 + TILE_Height * 2 * i);
		mo->touchRect = iRectMake(mo->position.x, mo->position.y, TILE_Width, TILE_Height);

#if 1
		TileWay* tw = &tileWay[0];
		for (j = 0; j < mo->tileNum; j++)
			mo->tileNumber[j] = tw->doorNum[j][0];

		for (j = 0; j < mo->tileNum; j++)
			maps[0]->tile[mo->tileNumber[j]] = WW;
#endif
	}
}

void freeMap()
{
	int i;
	for (i = 0; i < TILE_TOTAL_NUM; i++)
	{
		if (maps[i]->img)
			delete maps[i]->img;
		free(maps[i]->tile);
		free(maps[i]);
	}
	free(maps);

	for (i = 0; i < mapObjNum; i++)
	{
		delete mapObj[i];
	}
	free(mapObj);
	free(_mapObjectDoor);
}

void drawMap(float dt)
{
	int i;
	int num = TILE_TOTAL_NUM;
	for (i = 0; i < num; i++)
	{
		MapTile* m = maps[i];
		iPoint to = m->tileOff + DRAW_OFF;
		if (m->img)
			m->img->paint(dt, to);
	}

	for (i = 0; i < mapObjNum; i++)
	{
		MapObject* mo = mapObj[i];
		mo->paint(dt, DRAW_OFF);
	}

#if 1 // room clear test
	bool clear = true;
	for (i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		if (m->alive && m->mapNumber == player->mapNumber)
		{
			clear = false;
			break;
		}
	}

	if (clear)
	{
		for (i = 0; i < mapObjNum; i++)
		{
			MapObject* mo = mapObj[i];
			if(mo->index == 0 && mo->alive)
				mo->action();
		}
	}
#endif

#if 1 // draw wall 
	num = TILE_NUM_X * TILE_NUM_Y;
	for (i = 0; i < num; i++)
	{
		uint8 a = maps[0]->tile[i];
		if (a == WW)	setRGBA(1, 0, 0, 0.3f);
		else			setRGBA(0, 0, 1, 0.3f);
		
		iPoint p = displayCenterPos;
		fillRect(p.x + TILE_Width * (i % TILE_NUM_X), p.y + TILE_Height * (i / TILE_NUM_X),
			TILE_Width, TILE_Height);
	}
	setRGBA(1, 1, 1, 1);
#endif
}

