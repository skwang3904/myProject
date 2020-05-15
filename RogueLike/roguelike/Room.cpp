#include "Room.h"

#include "RgTile.h"

#include <math.h>


struct ConnectTile {
	int index; // 타일 넘버
	bool value; //
	bool visit;
};
ConnectTile ct[TILEOFF_NUM];

void connectCheck(ConnectTile* c);
void pathTileCheck(ConnectTile* c);
int conectCount = 0;

MapTile** maps;
bool randomOffCheck[TILEOFF_NUM];
static bool nextStage = false;
void loadRoomTile()
{
	
	int m[MAPTILE_NUM];
	if (nextStage == false)
	{
		maps = (MapTile**)malloc(sizeof(MapTile*) * TILEOFF_NUM);

		for (int i = 0; i < TILEOFF_NUM; i++)
			maps[i] = (MapTile*)malloc(sizeof(MapTile) * 1);
	}

	conectCount = 0;
	while (conectCount < MAPTILE_NUM)
	{

		for (int i = 0; i < TILEOFF_NUM; i++)
		{
			maps[i]->rgTile = NULL;
			maps[i]->tileOff = tileOffSet[i];

			ConnectTile* c = &ct[i];
			c->index = i;
			c->value = false;
			c->visit = false;
		}

		for (int i = 0; i < TILEOFF_NUM; i++)
			randomOffCheck[i] = false;

		memset(m, -1, sizeof(int) * MAPTILE_NUM);
		for (int i = 0; i < MAPTILE_NUM; i++)
		{
			if (m[i] == -1 || randomOffCheck[m[i]] == true)
				m[i] = random() % TILEOFF_NUM;

			if (randomOffCheck[m[i]] == false)
				randomOffCheck[m[i]] = true;
			else
				i--;
		}

		int k = -1;
		for (int i = 0; i < TILEOFF_NUM; i++)
		{
			// test
			bool exist = false;
			for (int j = 0; j < MAPTILE_NUM; j++)
			{
				if (i == m[j])
				{
					k = j;
					exist = true;
					break;
				}
			}
			if (exist == true)
				maps[m[k]]->rgTile = Tile4way1;
		}

		for (int i = 0; i < TILEOFF_NUM; i++)
		{
			ConnectTile* c = &ct[i];
			if (maps[i]->rgTile != NULL)
				c->value = true;
		}

		for (int i = 0; i < TILEOFF_NUM; i++)
		{
			ConnectTile* c = &ct[i];
				connectCheck(c);

			if (conectCount == MAPTILE_NUM)
				break;
			else if (conectCount > MAPTILE_NUM)
				printf("conectCount error\n");

			for (int j = 0; j < TILEOFF_NUM; j++)
				c->visit = false;

			//printf("conectCount %d\n", conectCount);
			conectCount = 0;
		}
	}

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		ConnectTile* c = &ct[i];
		pathTileCheck(c);
	}

	nextStage = true;
}

void connectCheck(ConnectTile* c)
{
	if (c->visit == true || c->value == false)
		return;

	c->visit = true;
	conectCount++;

	int index = c->index;
	int x = c->index % TILEOFF_SQRT;
	int y = c->index / TILEOFF_SQRT;
	if (x > 0)					connectCheck(&ct[c->index- 1]);
	if (x < TILEOFF_SQRT - 1)	connectCheck(&ct[c->index+ 1]);
	if (y > 0)					connectCheck(&ct[c->index- TILEOFF_SQRT]);
	if (y < TILEOFF_SQRT - 1)	connectCheck(&ct[c->index+ TILEOFF_SQRT]);
}

bool path(ConnectTile* c)
{
	if (c->value == true)
		return true;
	else
		return false;
}
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

	if (x > 0)					l = path(&ct[c->index - 1]);
	if (x < TILEOFF_SQRT - 1)	r = path(&ct[c->index + 1]);
	if (y > 0)					u = path(&ct[c->index - TILEOFF_SQRT]);
	if (y < TILEOFF_SQRT - 1)	d = path(&ct[c->index + TILEOFF_SQRT]);

	int pathNum = l + r + u + d;
	switch (pathNum) {
	case 4: maps[index]->rgTile = Tile4way1; break;
	case 3:
	{
		if (r && u && d) maps[index]->rgTile = Tile3way1;
		else if (l && u && d) maps[index]->rgTile = Tile3way2;
		else if (l && r && d) maps[index]->rgTile = Tile3way3;
		else if (l && r && u) maps[index]->rgTile = Tile3way4;
		break;
	}
	case 2:
	{
		if (l && r) maps[index]->rgTile = Tile2way1;
		else if (u && d) maps[index]->rgTile = Tile2way2;
		else if (l && u) maps[index]->rgTile = Tile2way3;
		else if (r && u) maps[index]->rgTile = Tile2way4;
		else if (l && d) maps[index]->rgTile = Tile2way5;
		else if (r && d) maps[index]->rgTile = Tile2way6;
		break;
	}
	case 1:
	{
		if (l) maps[index]->rgTile = Tile1way1;
		else if (r) maps[index]->rgTile = Tile1way2;
		else if (u) maps[index]->rgTile = Tile1way3;
		else if (d) maps[index]->rgTile = Tile1way4;
		break;
	}
	default:
		maps[index]->rgTile = Tile0way1;
	}
}


void freeRoomTile()
{
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		free(maps[i]);
	}
	free(maps);
}

void drawRoomTile(float dt)
{
	int num = RGTILE_X * RGTILE_Y;

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile != NULL)
		{
			for (int j = 0; j < num; j++)
			{
				if (maps[i]->rgTile[j] == MOVETILE) setRGBA(MOVETILE_RGBA);
				else if (maps[i]->rgTile[j] == WALLTILE)setRGBA(WALLTILE_RGBA);
				else if (maps[i]->rgTile[j] == FALLTILE)setRGBA(FALLTILE_RGBA);
				fillRect(maps[i]->tileOff.x + pc->camPosition.x + setPos.x + RGTILE_Width * (j % RGTILE_X),
					maps[i]->tileOff.y + pc->camPosition.y + setPos.y + RGTILE_Height * (j / RGTILE_X),
					RGTILE_Width, RGTILE_Height);
			}
		}
	}
	setRGBA(1, 1, 1, 1);
}

static iPoint prevCamOff = iPointMake(-1, -1);
bool passAni = false;
float passAniDt = _passAniDt;
static iPoint sp = iPointZero;
static iPoint ep = iPointZero;
void passTileAnimation(float dt)
{
	if (prevCamOff == iPointMake(-1, -1))
		prevCamOff = pc->camPosition;

	if (prevCamOff == pc->camPosition && passAniDt == _passAniDt)
		return;

	if (passAni == false)
	{
		sp = prevCamOff;
		ep = pc->camPosition;
		passAniDt = 0.0f;
		passAni = true;
	}

	pc->camPosition = sp + (ep - sp) * passAniDt / _passAniDt;

	passAniDt += dt;
	if (passAniDt > _passAniDt)
	{
		passAni = false;
		passAniDt = _passAniDt;
		prevCamOff = ep;
		pc->camPosition = ep;
		return;
	}
}

void wallCheck2(bool checkFall, MapTile* tile, iPoint& pos, iPoint mp, float halfOfTexW, float halfOfTexH)
{
	// 이동시 벽 감지
	// 계속
	int i, j;
	MapTile* t = tile;
	if (tile->rgTile == NULL)
		return;

	if (mp.x < 0)
	{
		int LX = pos.x - t->tileOff.x ;							LX /= RGTILE_Width;
		int TLY = pos.y - t->tileOff.y ;						TLY /= RGTILE_Height;
		int BLY = pos.y + halfOfTexH * 2.0f - t->tileOff.y ;	BLY /= RGTILE_Height;
		int min = t->tileOff.x- 100;
		//int min = t->tileOff.x;
		for (i = LX - 1; i > -1; i--)
		{
			bool stop = false;
			for (j = TLY; j < BLY + 1; j++)
			{
				if (tile->rgTile[RGTILE_X * j + i] == WALLTILE)
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
		int max = t->tileOff.x + RGTILE_X * RGTILE_Width * TILEOFF_SQRT  - 1;
		//int max = t->tileOff.x + RGTILE_X * RGTILE_Width - 1;
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
		int min = t->tileOff.y-100;
		//int min = t->tileOff.y;
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
		int max = t->tileOff.y + RGTILE_Y * RGTILE_Height * TILEOFF_SQRT - 1;
		//int max = t->tileOff.y + RGTILE_Y * RGTILE_Height - 1;

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

void wallCheck(bool checkFall, MapTile* tile, iPoint& pos, iPoint mp, float halfOfTexW, float halfOfTexH)
{
	wallCheck2(checkFall, tile, pos, mp, halfOfTexW, halfOfTexH);
}

void findMoveTile(Player* pc, MapTile* tile);
bool fallCheck(Player* pc, MapTile* tile, float dt)
{
	// 임시 - 낭떨어지에 진입시 가장 가까이있는 타일로 이동  - 어색함
	// 라이프 감소
	// 잠시 무적
	if (tile->rgTile == NULL)
		return false;

	MapTile* t = tile;

	int x = (int)(pc->playerPosition.x - t->tileOff.x + HALF_OF_TEX_WIDTH) / RGTILE_Width;
	int y = (int)(pc->playerPosition.y - t->tileOff.y + HALF_OF_TEX_HEIGHT) / RGTILE_Height;

	static bool falling = false;

	if (t->rgTile[RGTILE_X * y + x] == FALLTILE)
	{
		if (falling == false)
			pc->img[8]->startAnimation();

		if (pc->img[8]->animation == false)
		{
			falling = false;
			//이동
			findMoveTile(pc,tile);
			return false;
		}

		falling = true;
		iPoint p = iPointMake( pc->playerPosition.x - t->tileOff.x + setPos.x- HALF_OF_TEX_WIDTH,
			pc->playerPosition.y - t->tileOff.y + setPos.y - HALF_OF_TEX_HEIGHT);

		pc->img[8]->paint(dt, p, REVERSE_NONE);

		return true;
	}
	else
	{
		falling = false;
		return false;
	}
}

float findMoveTile(Player* pc, MapTile* tile, int x, int y)
{
	float min = 0xffff;
	MapTile* t = tile;
	if (t->rgTile[RGTILE_X * y + x] != MOVETILE)
		return min;

	float distance = iPointLength(
		pc->playerPosition + iPointMake(HALF_OF_TEX_WIDTH, HALF_OF_TEX_HEIGHT)
		- iPointMake(t->tileOff.x + RGTILE_Width * x + RGTILE_Width / 2,
				t->tileOff.y + RGTILE_Height * y + RGTILE_Height / 2));

	if (min > distance)
		return distance;
	return min;
}

void findMoveTile(Player* pc, MapTile* tile)
{
	MapTile* t = tile;
	int x = (int)(pc->playerPosition.x - t->tileOff.x + HALF_OF_TEX_WIDTH) / RGTILE_Width;
	int y = (int)(pc->playerPosition.y - t->tileOff.y + HALF_OF_TEX_HEIGHT) / RGTILE_Height;

	float minD = 0xffff;
	float dis;

	int pcX = x;
	int pcY = y;

	for (int i = 0; ; i++)
	{
		// x-1, y-1,	x, y-1,		x+1, y-1
		// x-1, y					x+1, y
		// x-1, y+1		x, y+1		x+1, y+1

		dis = findMoveTile(pc,tile, x - i, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y - i;
		}
		dis = findMoveTile(pc, tile, x - i, y);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y;
		}
		dis = findMoveTile(pc, tile, x - i, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x - i;
			pcY = y + i;
		}

		dis = findMoveTile(pc, tile, x, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x;
			pcY = y - i;
		}
		dis = findMoveTile(pc, tile, x, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x;
			pcY = y + i;
		}

		dis = findMoveTile(pc, tile, x + i, y - i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y - i;
		}
		dis = findMoveTile(pc,tile, x + i, y);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y;
		}

		dis = findMoveTile(pc, tile, x + i, y + i);
		if (dis < minD)
		{
			minD = dis;
			pcX = x + i;
			pcY = y + i;
		}

		if (minD != 0xffff)
			break;
	}

	iPoint p = iPointMake(t->tileOff.x + RGTILE_Width * pcX,
		t->tileOff.y + RGTILE_Height * pcY);

	if (p.x < pc->playerPosition.x)	p -= iPointMake(HALF_OF_TEX_WIDTH, 0);
	else							p += iPointMake(HALF_OF_TEX_WIDTH, 0);
	if (p.y < pc->playerPosition.y)	p -= iPointMake(0, HALF_OF_TEX_HEIGHT);
	else							p += iPointMake(0, HALF_OF_TEX_HEIGHT);
	
	pc->playerPosition = p;
}
