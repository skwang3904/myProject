#include "Map.h"

#include "Tile.h"

struct ConnectTile
{
	int index; // 타일 넘버
	bool value; //
	bool visit;
	iPoint tileOff;
};

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
void createMap();

void loadMap()
{
	int i;
	iPoint tileOffSet[TILE_TOTAL_NUM];
	for (int i = 0; i < TILE_TOTAL_NUM; i++)
	{
		tileOffSet[i] = iPointMake(TILE_NUM_X * TILE_Width * (i % TILE_TOTAL_SQRT),
			TILE_NUM_Y * TILE_Height * (i / TILE_TOTAL_SQRT));
	}

	displayCenterPos = iPointMake(devSize.width / 2 - TILE_NUM_X * TILE_Width / 2.0f,
		devSize.height / 2 - TILE_NUM_Y * TILE_Height / 2.0f);

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

	
	createMap();
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
}

void drawMap(float dt)
{
	
	int num = TILE_TOTAL_NUM;
	int t = TILE_NUM_X * TILE_NUM_Y;
	
	for (int i = 0; i < num; i++)
	{
		iPoint p = maps[i]->tileOff;
		for (int j = 0; j < t; j++)
		{
			int8 n = maps[i]->tile[j];
			if (n == MOVETILE)		setRGBA(MOVETILE_RGBA);
			else if (n == WALLTILE)	setRGBA(WALLTILE_RGBA);
			else if (n == FALLTILE)	setRGBA(FALLTILE_RGBA);
			else					setRGBA(1, 1, 1, 1);

			
			fillRect(p.x + TILE_Width * (j % TILE_NUM_X), p.y + TILE_Height * (j / TILE_NUM_X),
				TILE_Width, TILE_Height);
		}
	}
	setRGBA(1, 1, 1, 1);
}

//----------------------------------------------------------------------------------
void connectCheck(ConnectTile* c, int& count);
void pathTileCheck(ConnectTile* c);

void createMap()
{
	int i, j;
	ConnectTile ct[TILE_TOTAL_NUM];
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
			m->tileOff = tileOffSet[i];
			m->state = MapType_Nomal;

			ConnectTile* c = &ct[i];
			c->index = i;
			c->value = false;
			c->visit = false;
			c->tileOff = tileOffSet[i];
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
			memcpy(maps[m[i]]->tile, Tile4Way1, sizeof(int8) * TILE_NUM_X * TILE_NUM_Y);
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
	if (x > 0)						connectCheck(&c[c->index - 1], count);
	if (x < TILE_TOTAL_SQRT - 1)	connectCheck(&c[c->index + 1], count);
	if (y > 0)						connectCheck(&c[c->index - TILE_TOTAL_SQRT], count);
	if (y < TILE_TOTAL_SQRT - 1)	connectCheck(&c[c->index + TILE_TOTAL_SQRT], count);
}

#define MAPSIZE(index, t) memcpy(maps[index]->tile, t, sizeof(int8) * TILE_NUM_X * TILE_NUM_Y)
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

	if (x > 0)						l = c[c->index - 1].value;
	if (x < TILE_TOTAL_SQRT - 1)	r = c[c->index + 1].value;
	if (y > 0)						u = c[c->index - TILE_TOTAL_SQRT].value;
	if (y < TILE_TOTAL_SQRT - 1)	d = c[c->index + TILE_TOTAL_SQRT].value;

	int pathNum = l + r + u + d;
	switch (pathNum) {
	case 4: MAPSIZE(index, Tile4Way1); break;
	case 3:
	{
		if (r && u && d)	  MAPSIZE(index, Tile3Way1);
		else if (l && u && d) MAPSIZE(index, Tile3Way2);
		else if (l && r && d) MAPSIZE(index, Tile3Way3);
		else if (l && r && u) MAPSIZE(index, Tile3Way4);
		break;
	}
	case 2:
	{
		if (l && r) MAPSIZE(index, Tile2Way1);
		else if (u && d) MAPSIZE(index, Tile2Way2);
		else if (l && u) MAPSIZE(index, Tile2Way3);
		else if (r && u) MAPSIZE(index, Tile2Way4);
		else if (l && d) MAPSIZE(index, Tile2Way5);
		else if (r && d) MAPSIZE(index, Tile2Way6);
		break;
	}
	case 1:
	{
		if (l) MAPSIZE(index, Tile1Way1);
		else if (r) MAPSIZE(index, Tile1Way2);
		else if (u) MAPSIZE(index, Tile1Way3);
		else if (d) MAPSIZE(index, Tile1Way4);
		break;
	}
	default:
		MAPSIZE(index, Tile0Way1);
		break;
	}
}

//---------------------------------------------------------------------------------------------

void wallCheck(iPoint& pos, iPoint mp, float halfOfTexW, float halfOfTexH)
{
	int i, j;
	MapTile* t = maps[0];
	if (t->tile == NULL)
		return;

	if (mp.x < 0)
	{
		int LX = pos.x - t->tileOff.x;							LX /= TILE_Width;
		int TLY = pos.y - t->tileOff.y;						TLY /= TILE_Height;
		int BLY = pos.y + halfOfTexH * 2.0f - t->tileOff.y;	BLY /= TILE_Height;
		int min = t->tileOff.x - HALF_OF_TEX_WIDTH * 3;

		for (i = LX - 1; i > -1; i--)
		{
			bool stop = false;
			for (j = TLY; j < BLY + 1; j++)
			{
				if (t->tile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					min = t->tileOff.x + TILE_Width * (i + 1);
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
		int RX = pos.x + halfOfTexW * 2.0f - t->tileOff.x;		RX /= TILE_Width;
		int TRY = pos.y - t->tileOff.y;							TRY /= TILE_Height;
		int BRY = pos.y + halfOfTexH * 2.0f - t->tileOff.y;		BRY /= TILE_Height;
		int max = t->tileOff.x + RGTILE_X * TILE_Width - 1 + HALF_OF_TEX_WIDTH * 3;

		for (i = RX + 1; i < RGTILE_X; i++)
		{
			bool stop = false;
			for (j = TRY; j < BRY + 1; j++)
			{
				if (t->tile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					max = t->tileOff.x + TILE_Width * i - 1;
					break;
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
		int TY = pos.y - t->tileOff.y;							TY /= TILE_Height;
		int TLX = pos.x - t->tileOff.x;							TLX /= TILE_Width;
		int TRX = pos.x + halfOfTexW * 2.0f - t->tileOff.x;		TRX /= TILE_Width;
		int min = t->tileOff.y - HALF_OF_TEX_HEIGHT * 3;

		for (j = TY - 1; j > -1; j--)
		{
			bool stop = false;
			for (i = TLX; i < TRX + 1; i++)
			{
				if (t->tile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					min = t->tileOff.y + TILE_Height * (j + 1);
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
		int BY = pos.y + halfOfTexH * 2.0f - t->tileOff.y;		BY /= TILE_Height;
		int BLX = pos.x - t->tileOff.x;							BLX /= TILE_Width;
		int BRX = pos.x + halfOfTexW * 2.0f - t->tileOff.x;		BRX /= TILE_Width;
		int max = t->tileOff.y + RGTILE_Y * TILE_Height - 1 + HALF_OF_TEX_HEIGHT * 3;

		for (j = BY + 1; j < RGTILE_Y; j++)
		{
			bool stop = false;
			for (i = BLX; i < BRX + 1; i++)
			{
				if (t->tile[RGTILE_X * j + i] == WALLTILE)
				{
					stop = true;
					max = t->tileOff.y + TILE_Height * j - 1;
					break;
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
