#include "Room.h"

#include "Stage.h"

ConnectTile ct[TILEOFF_NUM];
MapTile** maps;

void connectCheck(ConnectTile* c, int& count);
void pathTileCheck(ConnectTile* c);

struct passTile {
	iPoint prevCamOff;
	iPoint sp;
	iPoint tp;
	float passAniDt;
};
static passTile pt;
#define _passAniDt 0.2f

void loadRoomTile()
{
	maps = (MapTile**)malloc(sizeof(MapTile*) * TILEOFF_NUM);

	for (int i = 0; i < TILEOFF_NUM; i++)
		maps[i] = (MapTile*)malloc(sizeof(MapTile) * 1);

	pt.prevCamOff = iPointMake(-1, -1);
	pt.sp = iPointZero;
	pt.tp = iPointZero;
	pt.passAniDt = _passAniDt;

	setRoomTile();
}

void setRoomTile()
{
	bool randomOffCheck[TILEOFF_NUM];

	int m[MAPTILE_NUM];
	int conectCount = 0;
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
			c->tileOff = tileOffSet[i];
		}

		memset(randomOffCheck, false, sizeof(bool) * TILEOFF_NUM);
		memset(m, -1, sizeof(int) * MAPTILE_NUM);
		for (int i = 0; i < MAPTILE_NUM; i++)
		{
			if (m[i] == -1 || randomOffCheck[m[i]])
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
			if (exist)
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
			connectCheck(c, conectCount);

			if (conectCount == MAPTILE_NUM)
				break;
			else if (conectCount > MAPTILE_NUM)
				printf("conectCount error\n");

			for (int j = 0; j < TILEOFF_NUM; j++)
				c->visit = false;
			conectCount = 0;
		}
	}

	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		ConnectTile* c = &ct[i];
		pathTileCheck(c);
	}
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

bool path(ConnectTile* c)
{
	if (c->value)
		return true;
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
		break;
	}
}

void freeRoomTile()
{
	for (int i = 0; i < TILEOFF_NUM; i++)
		free(maps[i]);
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
				int* tile = maps[i]->rgTile;
				if (tile[j] == MOVETILE) setRGBA(MOVETILE_RGBA);
				else if (tile[j] == WALLTILE)setRGBA(WALLTILE_RGBA);
				else if (tile[j] == FALLTILE)setRGBA(FALLTILE_RGBA);

				iPoint p = maps[i]->tileOff + SET_DRAW_OFF +
							iPointMake(	RGTILE_Width * (j % RGTILE_X), 
										RGTILE_Height * (j / RGTILE_X));
				fillRect(p.x, p.y, RGTILE_Width, RGTILE_Height);

				//setRGBA(1, 0, 0, 1);
				//drawRect(maps[i]->tileOff.x + pc->camPosition.x + setPos.x + RGTILE_Width * (j % RGTILE_X) + 2,
				//	maps[i]->tileOff.y + pc->camPosition.y + setPos.y + RGTILE_Height * (j / RGTILE_X) + 2,
				//	RGTILE_Width - 4, RGTILE_Height - 4);
			}
		}
	}
	setRGBA(1, 1, 1, 1);
}

void passTileAnimation(float dt)
{
	passTile* t = &pt;
	if (t->prevCamOff == iPointMake(-1, -1))
		t->prevCamOff = pc->camPosition;

	if (t->prevCamOff == pc->camPosition && t->passAniDt == _passAniDt)
		return;

	if (nextStage)
	{
		t->prevCamOff = pc->camPosition;
		return;
	}

	if (t->passAniDt == _passAniDt)
	{
		t->sp = t->prevCamOff;
		t->tp = pc->camPosition;
		t->passAniDt = 0.0f;
	}

	pc->camPosition = t->sp + (t->tp - t->sp) * t->passAniDt / _passAniDt;

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
				setRGBA(0, 0, 0, 0.7f);
				iPoint p = maps[i]->tileOff + SET_DRAW_OFF +
							iPointMake(	RGTILE_Width * (j % RGTILE_X),
										RGTILE_Height * (j / RGTILE_X));
				fillRect(p.x, p.y, RGTILE_Width, RGTILE_Height);
			}
		}
	}
	setRGBA(1, 1, 1, 1);
	t->passAniDt += dt;
	if (t->passAniDt > _passAniDt)
	{
		t->passAniDt = _passAniDt;
		t->prevCamOff = t->tp;
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
	bool evase = (pc->act == evasion);
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