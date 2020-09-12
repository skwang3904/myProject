#include "Map.h"

#include "Proc.h"
#include "Tile.h"
#include "PlayerChar.h"
#include "Monster.h"

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

MapTile** maps = NULL;
iPoint displayCenterPos;

void createMap(bool readFile)
{
	int i, j, num = TILE_TOTAL_NUM;

	if (readFile)
	{
		for (i = 0; i < num; i++)
		{
			maps[i]->state = (MapType)st->mapData[i].state;
			int index = st->mapData[i].tileIndex;
			if (index != -1)
				memcpy(maps[i]->tile, tileWay[index], sizeof(int8) * TILE_NUM_X * TILE_NUM_Y);
			maps[i]->tileIndex = index;
		}
	}
	else
	{
		bool randomOffCheck[TILE_TOTAL_NUM];
		int m[TILE_CONNECT_NUM];
		int conectCount = 0;
		while (conectCount < TILE_CONNECT_NUM)
		{
			for (i = 0; i < TILE_TOTAL_NUM; i++)
			{
				MapTile* m = maps[i];
				memset(m->tile, 0x00, sizeof(int8) * TILE_NUM_X * TILE_NUM_Y);
				m->state = MapType_Nomal;
				m->tileIndex = -1;

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
				memcpy(maps[m[i]]->tile, tileWay[1], sizeof(int8) * TILE_NUM_X * TILE_NUM_Y);
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
			MapTile* m = maps[i];
			if (m->tileIndex != -1)
			{
				m->state = MapType_ItemBox;
				break;
			}
		}

		for (i = TILE_TOTAL_NUM - 1; i > -1; i--) // 임시
		{
			MapTile* m = maps[i];
			if (m->tileIndex != -1)
			{
				m->state = MapType_Boss;
				break;
			}
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

	pathNum = pathNum * 2 + random() % 2;
#define TILECOPY(index, t) memcpy(maps[index]->tile, t, sizeof(int8) * TILE_NUM_X * TILE_NUM_Y)
	TILECOPY(index, tileWay[pathNum]);

	maps[index]->tileIndex = pathNum;
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
MapObject** _mapObj;
MapObject** mapObj;
int mapObjNum, _mapObjNum;

MapObject::MapObject(int index, int8 mapNum, iPoint pos) : Object(index, mapNum, pos)
{
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
	float angle = 0.0f;
	iSize size = iSizeMake(TILE_Width * tileNumX, TILE_Height * tileNumY);
	int tmp = 0;
	switch (index)
	{
	case 0: 
	case 1: 
	{
		tmp = TILE_NUM_X;
		//size
		angle = 90.0f + 180.0f * index;
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
		imgMapObjDoor = img;
	}

	img = imgMapObjDoor->copy();

	//img->animation = true;
	img->_aniDt = 1.0f;
	img->angle = angle;
	img->lockAngle = true;
	if(angle)
		img->position = iPointZero + iPointMake(-img->tex->width + img->tex->height, img->tex->width - img->tex->height) / 2.0f;
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

void MapObjectDoor::drawShadow(float dt, iPoint off)
{
}

void MapObjectDoor::action(Object* obj)
{
	PlayerChar* p = (PlayerChar*)obj;
	int8 pm = p->mapNumber;

	int d = 2; // 맵 이동 시 타일 간격 d개
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

	passMap->pass(pm);
}

//----------------------------------------------------------------------------------

MapObjectNextDoor* mapObjNextDoor = NULL;
MapObjectNextDoor::MapObjectNextDoor(int index, int8 mapNum, iPoint pos, int tileNumber) : MapObject(index, mapNum, pos)
{
	//-----------------------------------------------------------------

	int i, j;
	iImage* img;
	Texture* tex;

	setStringSize(TILE_Height);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	img = new iImage();
	iSize size = iSizeMake(TILE_Width * 3, TILE_Height * 3);
	iGraphics* g = iGraphics::instance();
	for (i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0) setRGBA(1, 0, 1, 1);
		else		setRGBA(0, 1, 0, 1);
		g->fillRect(0, 0, size.width, size.height, 20);

		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "다음\n지역");

		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}

	//img->animation = true;
	img->_aniDt = 1.0f;
	this->img = img;

	mapObjNextDoor = this;
}

MapObjectNextDoor::~MapObjectNextDoor()
{
	delete img;
}

void MapObjectNextDoor::paint(float dt, iPoint off)
{
	if (alive == false)
		return;

	if (containRect(touchRect, player->touchRect))
	{
		// next stage
		alive = false;
		printf("next stage\n");
		passMap->startNextStage();
	}
	
	img->paint(dt, position + off);
}

void MapObjectNextDoor::drawShadow(float dt, iPoint off)
{
}

void MapObjectNextDoor::action(Object* obj)
{
	// 보스 죽엇을때 생성
	mapNumber = obj->mapNumber;

	alive = true;
	position = obj->position;
	touchRect = iRectMake(position.x, position.y, img->tex->width, img->tex->height);
}

void MapObjectNextDoor::setNextDoor(Object* obj)
{
	mapObjNextDoor->action(obj);
}

//----------------------------------------------------------------------------------

iImage* imgMapObjBarrel = NULL;
MapObjectBarrel::MapObjectBarrel(int index, int8 mapNum, iPoint pos, int tileNumber) : MapObject(index, mapNum, pos)
{
	tileNumX = 2;
	tileNumY = 2;
	int num = tileNumX * tileNumY;
	this->tileNumber = (int*)calloc(sizeof(int), num);

	for (int i = 0; i < num; i++)
	{
		int tn = tileNumber + TILE_NUM_X * (i / tileNumX) + (i % tileNumX);
		this->tileNumber[i] = tn;
		maps[mapNumber]->tile[tn] = TILE_FALL;
	}

	//-----------------------------------------------------------------

	int i, j;
	iImage* img;
	Texture* tex;
	iSize size = iSizeMake(TILE_Width * tileNumX, TILE_Height * tileNumY);

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

		imgMapObjBarrel = img;
	}

	img = imgMapObjBarrel->copy();

	//img->animation = true;
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

void MapObjectBarrel::drawShadow(float dt, iPoint off)
{
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

		imgMapObjItemBox = img;
	}

	img = imgMapObjItemBox->copy();

	//img->animation = true;
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

void MapObjectItemBox::drawShadow(float dt, iPoint off)
{
}

void MapObjectItemBox::action(Object* obj)
{
	img->startAnimation();
}


//----------------------------------------------------------------------------------

void loadMap()
{
	int i, j, k, num = TILE_TOTAL_NUM;

	displayCenterPos = iPointMake(devSize.width / 2.0f, devSize.height / 2.0f)
		- iPointMake(TILE_NUM_X * TILE_Width / 2.0f, TILE_NUM_Y * TILE_Height / 2.0f);

	iPoint tileOffSet[TILE_TOTAL_NUM];
	for (int i = 0; i < num; i++)
	{
		tileOffSet[i] = iPointMake(TILE_NUM_X * TILE_Width * (i % TILE_TOTAL_SQRT),
			TILE_NUM_Y * TILE_Height * (i / TILE_TOTAL_SQRT));
	}

	maps = (MapTile**)calloc(sizeof(MapTile*), num);
	for (i = 0; i < num; i++)
	{
		maps[i] = (MapTile*)malloc(sizeof(MapTile));
		maps[i]->img = NULL;

		maps[i]->tileIndex = -1;
		maps[i]->tile = (int8*)calloc(sizeof(int8), TILE_NUM_X * TILE_NUM_Y);
		maps[i]->tileOff = tileOffSet[i];
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

	for (i = 0; i < _mapObjNum; i++)
		delete _mapObj[i];
	free(_mapObj);
	free(mapObj);

	deleteMapImage();
}

void drawMap(float dt)
{
	int i, j;
	int num = TILE_TOTAL_NUM;
#if 1
	int tileNum = TILE_NUM_X * TILE_NUM_Y;
	iPoint tmp = iPointMake(devSize.width - TILE_Width * TILE_NUM_X, devSize.height - TILE_Height * TILE_NUM_Y) / 2.0f;
	for (j = 0; j < tileNum; j++)
	{
		int8 n = maps[player->mapNumber]->tile[j];
		if (n == TILE_MOVE)			setRGBA(MOVETILE_RGBA);
		else if (n == TILE_WALL)	setRGBA(WALLTILE_RGBA);
		else if (n == TILE_FALL)	setRGBA(FALLTILE_RGBA);
		else						setRGBA(1, 1, 1, 1);

		fillRect(tmp.x + TILE_Width * (j % TILE_NUM_X), tmp.y + TILE_Height * (j / TILE_NUM_X),
			TILE_Width, TILE_Height);
	}
#endif
	setRGBA(1, 1, 1, 0.6f);
	for (i = 0; i < num; i++)
	{
		if (i != player->mapNumber) 
			continue;

		MapTile* m = maps[i];
		iPoint to = m->tileOff + DRAW_OFF;
		if (m->img)
			m->img->paint(dt, to);
	}
	setRGBA(1, 1, 1, 1);

	for (i = 0; i < mapObjNum; i++)
	{
		MapObject* mo = mapObj[i];
		if(mo->mapNumber == player->mapNumber)
			mo->paint(dt, DRAW_OFF);

#if 0 // 아직 alive 사용안함
		if (mo->alive == false)
		{
			mapObjNum--;
			mapObj[i] = mapObj[mapObjNum];
			i--;
		}
#endif
	}


#if 0 // draw wall 
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

iImage** imgMaps = NULL;
void createMapImage()
{
	int i, j, k, num = TILE_TOTAL_NUM;
	Texture* tex, *t;
	iImage* img;
	iSize size;
	// map img
#if 0 // 테스트용
	size = iSizeMake(TILE_NUM_X * TILE_Width, TILE_NUM_Y * TILE_Height);
	int tileNum = TILE_NUM_X * TILE_NUM_Y;
	for (i = 0; i < num; i++)
	{
		if (maps[i]->tileIndex == -1) continue;

		tex = createTexture(size.width, size.height);
		img = new iImage();

		fbo->bind(tex);
		for (j = 0; j < tileNum; j++)
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

		if (maps[i]->img)
			delete maps[i]->img;
		maps[i]->img = img;
	}
#else
	// from tile.cpp
	const char* strPath[TILE_IMAGE_NUM] = {
		"assets/maps/room4_0.png",		"assets/maps/room4_0.png",
		"assets/maps/room4_0.png",		"assets/maps/room4_1.png",

		"assets/maps/room3_RUD_0.png",	"assets/maps/room3_RUD_0.png",
		"assets/maps/room3_LUD_0.png",	"assets/maps/room3_LUD_0.png",
		"assets/maps/room3_LRD_0.png",	"assets/maps/room3_LRD_0.png",
		"assets/maps/room3_LRU_0.png",	"assets/maps/room3_LRU_0.png",

		"assets/maps/room2_LR_0.png",	"assets/maps/room2_LR_0.png",
		"assets/maps/room2_UD_0.png",	"assets/maps/room2_UD_0.png",
		"assets/maps/room2_LU_0.png",	"assets/maps/room2_LU_1.png",
		"assets/maps/room2_RU_0.png",	"assets/maps/room2_RU_1.png",
		"assets/maps/room2_LD_0.png",	"assets/maps/room2_LD_1.png",
		"assets/maps/room2_RD_0.png",	"assets/maps/room2_RD_1.png",

		"assets/maps/room1_L_0.png",	"assets/maps/room1_L_0.png",
		"assets/maps/room1_R_0.png",	"assets/maps/room1_R_0.png",
		"assets/maps/room1_U_0.png",	"assets/maps/room1_U_0.png",
		"assets/maps/room1_D_0.png",	"assets/maps/room1_D_0.png",
	};

	float ratio = 1.5f;
	iSize baseSize = iSizeMake(TILE_NUM_X * TILE_Width, TILE_NUM_Y * TILE_Height);
	size = baseSize * ratio;

	if (imgMaps == NULL)
	{
		imgMaps = (iImage**)calloc(sizeof(iImage*), TILE_IMAGE_NUM);
		num = TILE_IMAGE_NUM;
		for (i = 0; i < num; i++)
		{
			img = new iImage();
			tex = createTexture(size.width, size.height);

			fbo->bind(tex);
			setRGBA(0.7f, 0.7f, 0.7f, 1);
			t = createImage(strPath[i]);
			drawImage(t, size.width / 2.0f, size.height / 2.0f,
				0, 0, t->width, t->height,
				VCENTER | HCENTER, size.width / t->width, size.height / t->height,
				2, 0, REVERSE_HEIGHT);
			freeImage(t);
			setRGBA(1, 1, 1, 1);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);

			img->position = iPointZero - iPointMake(size.width - baseSize.width, size.height - baseSize.height) * 0.5f;
			imgMaps[i] = img;
		}
	}

	num = TILE_TOTAL_NUM;
	for (i = 0; i < num; i++)
	{
		int index = maps[i]->tileIndex;
		if (index == -1) continue;

		if (maps[i]->img)
			delete maps[i]->img;
		maps[i]->img = imgMaps[index]->copy();
	}
#endif
	setRGBA(1, 1, 1, 1);

	//-----------------------------------------------------------------------------
	
	if (_mapObj)
	{
		for (i = 0; i < _mapObjNum; i++)
			delete _mapObj[i];
		free(_mapObj);
		free(mapObj);
		_mapObj = NULL;
	}

	_mapObj = (MapObject**)malloc(sizeof(MapObject*) * 200);
	mapObj = (MapObject**)malloc(sizeof(MapObject*) * 200);
	mapObjNum = _mapObjNum = 0;

	for (i = 0; i < TILE_TOTAL_NUM; i++)
	{
		if (maps[i]->state == MapType_ItemBox)
			maps[i]->tile[TILE_NUM_X * 24 + TILE_NUM_X / 2] = IB;
	}

	// mapObj img
	bool check[4] = { true, true, true, true };
	for (k = 0; k < num; k++)
	{
		MapTile* m = maps[k];
		if (m->tileIndex == -1) continue;
		for (i = 0; i < TILE_NUM_X; i++)
		{
			for (j = 0; j < TILE_NUM_Y; j++)
			{
				int tn = TILE_NUM_X * j + i;
				iPoint p = m->tileOff + iPointMake(TILE_Width * i, TILE_Height * j);

				switch (m->tile[tn])
				{
				case 01: // TILE_MOVE
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
					if (check[0] && i == 0)						dir = 0, check[0] = false, exist = false;
					else if (check[1] && i == TILE_NUM_X - 1)	dir = 1, check[1] = false, exist = false;
					else if (check[2] && j == 0)				dir = 2, check[2] = false, exist = false;
					else if (check[3] && j == TILE_NUM_Y - 1)	dir = 3, check[3] = false, exist = false;

					if (exist)
						break;

					_mapObj[mapObjNum] = new MapObjectDoor(dir, k, p, tn);
					mapObj[mapObjNum] = _mapObj[mapObjNum];
					mapObjNum++;
					break;
				}
				case TILE_BARREL:
				{
					_mapObj[mapObjNum] = new MapObjectBarrel(0, k, p, tn);
					mapObj[mapObjNum] = _mapObj[mapObjNum];
					mapObjNum++;
					break;
				}
				case TILE_TRAPDOOR:
				{
					break;
				}
				case TILE_ITEMBOX:
				{
					_mapObj[mapObjNum] = new MapObjectItemBox(0, k, p, tn);
					mapObj[mapObjNum] = _mapObj[mapObjNum];
					mapObjNum++;
					break;
				}
				case TILE_MONSTER_SPAWN: 
					break;
				default:
					printf("tile read error\n");
					break;
				}
			}
		}

		memset(check, true, sizeof(bool) * 4);
	}

	_mapObj[mapObjNum] = new MapObjectNextDoor(0, 0, iPointZero, 0);
	mapObj[mapObjNum] = _mapObj[mapObjNum];
	mapObjNum++;

	_mapObjNum = mapObjNum;
}

void deleteMapImage()
{
	int i;
	// maps
	for (i = 0; i < TILE_IMAGE_NUM; i++)
		delete imgMaps[i];
	free(imgMaps);

	// mapObj
	free(mapObjNextDoor);
	delete imgMapObjDoor;
	delete imgMapObjBarrel;
	delete imgMapObjItemBox;
}