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
			maps[i]->state = MapType_ItemBox;
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
				if (t->tile[TILE_NUM_X * j + i] == TILE_WALL)
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
				if (t->tile[TILE_NUM_X * j + i] == TILE_WALL)
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
				if (t->tile[TILE_NUM_X * j + i] == TILE_WALL)
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
				if (t->tile[TILE_NUM_X * j + i] == TILE_WALL)
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

MapObject::MapObject(int index, int8 mapNum, iPoint pos) : Object(index, mapNum, pos)
{
	alive = false;
	touchRect = iRectZero;

	value = 0;

	tileNumX = 0;
	tileNumY = 0;
	tileNumber = NULL;
}

MapObject::~MapObject()
{
	if (tileNumber)
		free(tileNumber);
}

//----------------------------------------------------------------------------------

iImage* imgMapObjDoor = NULL;
MapObjectDoor::MapObjectDoor(int index, int8 mapNum, iPoint pos, int tileNumber) : MapObject(index, mapNum, pos)
{
	tileNumX = 1;
	tileNumY = 4;
	int num = tileNumX * tileNumY;
	this->tileNumber = (int*)calloc(sizeof(int), num);

	iSize size = iSizeMake(TILE_Width * tileNumX, TILE_Height * tileNumY);
	int tmp = 0;
	switch (index)
	{
	case 0: 
	case 1: 
	{
		tmp = TILE_NUM_X;
		//size
		break;
	}
	case 2: 
	case 3:
	{
		tmp = 1;
		size = iSizeMake(size.height, size.width);
		break;
	}
	default:
		printf("Door index Error\n");
		break;
	}

	for (int i = 0; i < num; i++)
	{
		int tn = tileNumber + i * tmp;
		this->tileNumber[i] = tn;
		//maps[mapNumber]->tile[tn] = WW;
	}

	//-----------------------------------------------------------------

	int i, j;
	iImage* img;
	Texture* tex;

	if (imgMapObjDoor == NULL)
	{
		setStringSize(TILE_Height - 5);
		setStringRGBA(0, 0, 0, 1);
		setStringBorder(0);
		img = new iImage();

		iGraphics* g = iGraphics::instance();
		for (i = 0; i < 2; i++)
		{
			g->init(size);

			if (i == 0) setRGBA(0, 0, 1, 1);
			else		setRGBA(0, 1, 0, 1);
			g->fillRect(0, 0, size.width, size.height, 10);

			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "문");

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}

		setRGBA(1, 1, 1, 1);
		setStringSize(1);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(0);
	}
	else
	{
		img = imgMapObjDoor->copy();
	}

	img->animation = true;
	img->_aniDt = 1.0f;
	this->img = img;
	

	touchRect = iRectMake(position, size);
}

MapObjectDoor::~MapObjectDoor()
{
	delete img; // test
}

void MapObjectDoor::paint(float dt, iPoint off)
{
	if (mapNumber != player->mapNumber)
		return;

	if (containRect(touchRect, player->touchRect))
	{
		printf("touch obj\n");
		action(player);
	}

#if 0
	iRect rt = touchRect;
	rt.origin += off;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

	img->paint(dt, position + off);
}

void MapObjectDoor::action(Object* obj)
{
	PlayerChar* p = (PlayerChar*)obj;
	int8 pm = p->mapNumber;
	int d = 2;
	iSize size = iSizeMake(p->img->tex->width, p->img->tex->height);
	switch (index)
	{
	case 0:
		pm--;
		p->position.x = maps[pm]->tileOff.x + (TILE_NUM_X - d) * TILE_Width - size.width;
		break;
	case 1:
		pm++;
		p->position.x = maps[pm]->tileOff.x + d * TILE_Width;
		break;
	case 2:
		pm -= TILE_TOTAL_SQRT;
		p->position.y = maps[pm]->tileOff.y + (TILE_NUM_Y - d) * TILE_Height - size.height;
		break;
	case 3:
		pm += TILE_TOTAL_SQRT;
		p->position.y = maps[pm]->tileOff.y + d * TILE_Height;
		break;

	}

	p->camera = iPointZero - maps[pm]->tileOff;
	p->mapNumber = pm;
}

//----------------------------------------------------------------------------------

iImage* imgMapObjBarrel = NULL;
MapObjectBarrel::MapObjectBarrel(int index, int8 mapNum, iPoint pos, int tileNumber) : MapObject(index, mapNum, pos)
{
	tileNumX = 2;
	tileNumY = 2;
	int num = tileNumX * tileNumY;
	this->tileNumber = (int*)calloc(sizeof(int), num);

	iSize size = iSizeMake(TILE_Width * tileNumX, TILE_Height * tileNumY);

	for (int i = 0; i < num; i++)
	{
		int tn = tileNumber + TILE_NUM_X * (i / tileNumX) + (i % tileNumX);
		this->tileNumber[i] = tn;
		maps[mapNumber]->tile[tn] = WW;
	}

	//-----------------------------------------------------------------

	int i, j;
	iImage* img;
	Texture* tex;

	if (imgMapObjBarrel == NULL)
	{
		setStringSize(TILE_Height);
		setStringRGBA(0, 0, 0, 1);
		setStringBorder(0);
		img = new iImage();

		iGraphics* g = iGraphics::instance();
		for (i = 0; i < 2; i++)
		{
			g->init(size);

			if (i == 0) setRGBA(1, 1, 0, 1);
			else		setRGBA(1, 0, 1, 1);
			g->fillRect(0, 0, size.width, size.height, 20);

			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "나무");

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		setRGBA(1, 1, 1, 1);
		setStringSize(1);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(0);
	}
	else
	{
		img = imgMapObjBarrel->copy();
	}

	img->animation = true;
	img->_aniDt = 1.0f;
	this->img = img;

	touchRect = iRectMake(position, size);
}

MapObjectBarrel::~MapObjectBarrel()
{
	delete img;
}

void MapObjectBarrel::paint(float dt, iPoint off)
{

	img->paint(dt, position + off);
}

void MapObjectBarrel::action(Object* obj)
{
}

//----------------------------------------------------------------------------------

iImage* imgMapObjItemBox = NULL;
MapObjectItemBox::MapObjectItemBox(int index, int8 mapNum, iPoint pos, int tileNumber) : MapObject(index, mapNum, pos)
{
	tileNumX = 6;
	tileNumY = 4;
	int num = tileNumX * tileNumY;
	this->tileNumber = (int*)calloc(sizeof(int), num);

	iSize size = iSizeMake(TILE_Width * tileNumX, TILE_Height * tileNumY);

	for (int i = 0; i < num; i++)
	{
		int tn = tileNumber + TILE_NUM_X * (i / tileNumX) + (i % tileNumX);
		this->tileNumber[i] = tn;
		maps[mapNumber]->tile[tn] = WW;
	}

	//-----------------------------------------------------------------

	int i, j;
	iImage* img;
	Texture* tex;

	if (imgMapObjItemBox == NULL)
	{
		setStringSize(TILE_Height);
		setStringRGBA(0, 0, 0, 1);
		setStringBorder(0);
		img = new iImage();

		iGraphics* g = iGraphics::instance();
		for (i = 0; i < 2; i++)
		{
			g->init(size);

			if (i == 0) setRGBA(0, 1, 1, 1);
			else		setRGBA(0, 0, 1, 1);
			g->fillRect(0, 0, size.width, size.height, 20);

			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "상자");

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		setRGBA(1, 1, 1, 1);
		setStringSize(1);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(0);
	}
	else
	{
		img = imgMapObjItemBox->copy();
	}

	img->animation = true;
	img->_aniDt = 1.0f;
	this->img = img;

	touchRect = iRectMake(position, size);

}

MapObjectItemBox::~MapObjectItemBox()
{
	delete img;
}

void MapObjectItemBox::paint(float dt, iPoint off)
{
	img->paint(dt, position + off);
}

void MapObjectItemBox::action(Object* obj)
{
	img->startAnimation();
}


//----------------------------------------------------------------------------------

void loadMap()
{
	int i, j, k, num = TILE_TOTAL_NUM;

	iPoint tileOffSet[TILE_TOTAL_NUM];
	for (int i = 0; i < num; i++)
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

	maps = (MapTile**)malloc(sizeof(MapTile*) * num);
	for (i = 0; i < num; i++)
	{
		maps[i] = (MapTile*)malloc(sizeof(MapTile));
		maps[i]->img = NULL;

		maps[i]->tile = (int8*)calloc(sizeof(int8), TILE_NUM_X * TILE_NUM_Y);
		maps[i]->tileOff = tileOffSet[i];
	}

	// create random map
	createMap();

	int t = TILE_NUM_X * TILE_NUM_Y;
	iSize size = iSizeMake(TILE_NUM_X * TILE_Width, TILE_NUM_Y * TILE_Height);
	for (i = 0; i < num; i++)
	{
		if (maps[i]->tile[0] != 0)
		{
			Texture* tex = createTexture(size.width, size.height);
			iImage* img = new iImage();

			fbo->bind(tex);
			for (j = 0; j < t; j++)
			{
				int8 n = maps[i]->tile[j];
				if (n == TILE_MOVE)			setRGBA(MOVETILE_RGBA);
				else if (n == TILE_WALL)	setRGBA(WALLTILE_RGBA);
				else if (n == TILE_FALL)	setRGBA(FALLTILE_RGBA);
				else						setRGBA(1, 1, 1, 1);

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

	mapObj = (MapObject**)malloc(sizeof(MapObject*) * 100);
	mapObjNum = 0;
	bool check[4] = { true, true, true, true };
	
	for (i = 0; i < TILE_TOTAL_NUM; i++)
	{
		if (maps[i]->state == MapType_ItemBox)
			maps[i]->tile[TILE_NUM_X * 24 + TILE_NUM_X / 2] = IB;
	}

	for (k = 0; k < num; k++)
	{
		for (i = 0; i < TILE_NUM_X; i++)
		{
			for (j = 0; j < TILE_NUM_Y; j++)
			{
				int tn = TILE_NUM_X * j + i;
				iPoint p = maps[k]->tileOff + iPointMake(TILE_Width * i, TILE_Height * j);
				
				switch (maps[k]->tile[tn])
				{
				case 01:
				{
					break;
				}
				case TILE_WALL:
				{
					break;
				}
				case TILE_FALL:
				{
					break;
				}
				case TILE_DOOR:
				{
					bool exist = true;
					int dir = 0;
					if		(check[0] && i == 0)				dir = 0, check[0] = false, exist = false;
					else if (check[1] && i == TILE_NUM_X - 1)	dir = 1, check[1] = false, exist = false;
					else if (check[2] && j == 0)				dir = 2, check[2] = false, exist = false;
					else if (check[3] && j == TILE_NUM_Y - 1)	dir = 3, check[3] = false, exist = false;

					if (exist)
						break;

					mapObj[mapObjNum] = new MapObjectDoor(dir, k, p, tn);
					mapObjNum++;
					break;
				}
				case TILE_BARREL:
				{
					mapObj[mapObjNum] = new MapObjectBarrel(0, k, p, tn);
					mapObjNum++;
					break;
				}
				case TILE_TRAPDOOR:
				{
					break;
				}
				case TILE_ITEMBOX:
				{
					mapObj[mapObjNum] = new MapObjectItemBox(0, k, p, tn);
					mapObjNum++;
					break;
				}
				default: printf("tile read error\n");	break;
				}
			}
		}

		memset(check, true, sizeof(bool) * 4);
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


#if 1 // draw wall 
	num = TILE_NUM_X * TILE_NUM_Y;
	for (i = 0; i < num; i++)
	{
		uint8 a = maps[player->mapNumber]->tile[i];
		if (a == WW)	setRGBA(1, 0, 0, 0.7f);
		else			setRGBA(0, 0, 1, 0.7f);
		
		iPoint p = displayCenterPos;
		fillRect(p.x + TILE_Width * (i % TILE_NUM_X), p.y + TILE_Height * (i / TILE_NUM_X),
			TILE_Width, TILE_Height);
	}
	setRGBA(1, 1, 1, 1);
#endif
}
