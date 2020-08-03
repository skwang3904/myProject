#include "Room.h"

#include "EnemyData.h"
#include "Stage.h"

ConnectTile ct[TILEOFF_NUM];
MapTile** maps;

void connectCheck(ConnectTile* c, int& count);
void pathTileCheck(ConnectTile* c);

struct passTile {
	int tileNum;
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

	pt.tileNum = -1;
	pt.sp = iPointZero;
	pt.tp = iPointZero;
	pt.passAniDt = _passAniDt;

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
			m->rgTile = NULL;
			m->mapImg = NULL;
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

		int k = -1;
		for (i = 0; i < TILEOFF_NUM; i++)
		{
			// test
			bool exist = false;
			for (j = 0; j < MAPTILE_NUM; j++)
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

		for (i = 0; i < TILEOFF_NUM; i++)
		{
			ConnectTile* c = &ct[i];
			if (maps[i]->rgTile != NULL)
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
		if (maps[i]->rgTile)
		{
			maps[i]->state = MapType_Treasure;
			break;
		}
	}

	for (i = TILEOFF_NUM-1; i > -1; i--) // 임시
	{
		if (maps[i]->rgTile)
		{
			maps[i]->state = MapType_Boss;
			break;
		}

	}

	// 다음스테이지 넘어갈떄 초기화 해줘야함
	for (i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile == NULL)
			continue;

		MapTile* m = &maps[i][0];

		m->mapObjNum = 7;
		m->mapObj = (MapObject**)malloc(sizeof(MapObject*) * m->mapObjNum);

		const char* strBarrel[4] = {
			"barrel_1", "barrel_2", "door_close_east", "door_open_east"
		};

		iImage* img;
		Texture* tex;
		for (j = 0; j < m->mapObjNum; j++)
		{
			m->mapObj[j] = (MapObject*)malloc(sizeof(MapObject));
			MapObject* mobj = &m->mapObj[j][0];
			if (j < 3)
			{
				img = new iImage();

				iPoint po = iPointMake(128, 157);

				for (int k = 0; k < 2; k++)
				{
					tex = createTexture(po.x, po.y);
					fbo->bind(tex);

					iSize d = devSize;
					devSize = iSizeMake(po.x, po.y);
					iRect v = viewport;
					viewport = iRectMake(0, 0, po.x, po.y);
					float mat[16];
					memcpy(mat, mProjection->d(), sizeof(float) * 16);
					mProjection->loadIdentity();
					mProjection->ortho(0, po.x, po.y, 0, -1000, 1000);

					Texture* texBar = createImage("assets/object/%s.png", strBarrel[k]);
					drawImage(texBar, 0, 0, TOP | LEFT);
					freeImage(texBar);

					memcpy(mProjection->d(), mat, sizeof(float) * 16);
					devSize = d;
					viewport = v;
					fbo->unbind();

					img->addObject(tex);
					freeImage(tex);
				}

				mobj->objPos = iPointMake((3 + j * 3) * RGTILE_Width, (3 + j * 3) * RGTILE_Height);
				mobj->objTileNum = 1;
				mobj->objTile = (int*)calloc(sizeof(int), mobj->objTileNum);
				mobj->objTile[0] = RGTILE_X * (3 + j * 3) + (3 + j * 3);

				int tmp = mobj->objTile[0];

				m->rgTile[tmp] = WALLTILE;
				m->rgTile[tmp + 1] = WALLTILE;
				m->rgTile[tmp + RGTILE_X] = WALLTILE;
				m->rgTile[tmp + RGTILE_X + 1] = WALLTILE;

				float ratio = max(RGTILE_Width / tex->width, RGTILE_Height / tex->height) * 2.0f;
				img->ratio = ratio;
				img->reverse = REVERSE_HEIGHT;
				//img->animation = true;
				//img->_aniDt = 0.5f;

				mobj->objImg = img;
				mobj->type = MapObj_Nomal;
			}
			else if (j < 7)
			{
				iPoint drp = iPointZero;
				bool check = false;
				mobj->objImg = NULL;
				int angle = 0;
				if (j == 3 && m->rgTile[RGTILE_X * (RGTILE_Y / 2)] == DR)
				//if (m->rgTile[RGTILE_X * (RGTILE_Y / 2)] == DR)
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
					drp = iPointMake(0, RGTILE_Height * (RGTILE_Y / 2 - 2));
				}
				if (j == 4 && m->rgTile[RGTILE_X * (RGTILE_Y / 2) + RGTILE_X - 1] == DR)
				//if ( m->rgTile[RGTILE_X * (RGTILE_Y / 2) + RGTILE_X - 1] == DR)
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
					drp = iPointMake(RGTILE_Width * (RGTILE_X - 1), RGTILE_Height * (RGTILE_Y / 2 - 2));

				}
				if (j == 5 && m->rgTile[RGTILE_X / 2] == DR)
				//if ( m->rgTile[RGTILE_X / 2] == DR)
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
					drp = iPointMake(RGTILE_Width * (RGTILE_X / 2 - 2), 0);

				}
				if (j == 6 && m->rgTile[RGTILE_X * (RGTILE_Y - 1) + RGTILE_X / 2] == DR)
				//if ( m->rgTile[RGTILE_X * (RGTILE_Y - 1) + RGTILE_X / 2] == DR)
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
					drp = iPointMake(RGTILE_Width * (RGTILE_X / 2 - 2), RGTILE_Height * (RGTILE_Y - 1));
				}

				if (check)
				{
					img = new iImage();
					for (int k = 0; k < 2; k++)
					{
						tex = createImage("assets/object/%s.png", strBarrel[k + 2]);
						img->addObject(tex);
						freeImage(tex);
					}
					img->ratio = RGTILE_Height * 4.0f / tex->height;
					img->reverse = REVERSE_HEIGHT;
					img->angle = angle;
					img->lockAngle = true;

					mobj->objImg = img;

					mobj->objPos = drp;
					mobj->type = MapObj_Door;
				}
			}
		}
		//--------------------------------------------------------------------

		img = new iImage();

		int kkk = 5;
		for (int k = 0; k < kkk; k++)
		{
			iPoint p = iPointMake(RGTILE_X * RGTILE_Width, RGTILE_Y * RGTILE_Height);
			Texture* tex = createTexture(p.x, p.y);
			fbo->bind(tex);

			iSize d = devSize;
			devSize = iSizeMake(p.x, p.y);
			iRect v = viewport;
			viewport = iRectMake(0, 0, p.x, p.y);
			float mat[16];
			memcpy(mat, mProjection->d(), sizeof(float) * 16);
			mProjection->loadIdentity();
			mProjection->ortho(0, p.x, p.y, 0, -1000, 1000);

			int num = RGTILE_X * RGTILE_Y;
			for (int n = 0; n < num; n++)
			{
				int* tile = m->rgTile;

				float a = fabsf((float)k / kkk - 0.5f);
				if (tile[n] == WALLTILE || tile[n] == DOORTILE)		setRGBA(a, a, a, 1);
				else if (tile[n] == FALLTILE)	setRGBA(FALLTILE_RGBA);
				else							setRGBA(a + 0.5f, a + 0.5f, a + 0.5f, 1);

				p = iPointMake(RGTILE_Width * (n % RGTILE_X),
					RGTILE_Height * (n / RGTILE_X));
				fillRect(p.x, p.y, RGTILE_Width, RGTILE_Height);
			}

			memcpy(mProjection->d(), mat, sizeof(float) * 16);
			devSize = d;
			viewport = v;
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		
		img->reverse = REVERSE_HEIGHT;
		img->animation = true;
		img->_aniDt = 0.5f;

		m->mapImg = img;
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
	{
		if (maps[i]->mapImg)
		{
			delete(maps[i]->mapImg);

			int num = maps[i]->mapObjNum;
			for (int j = 0; j < num; j++)
			{
				delete(maps[i]->mapObj[j]->objImg);

				free(maps[i]->mapObj[j]->objTile);

				free(maps[i]->mapObj[j]);
			}
			free(maps[i]->mapObj);
		}
		free(maps[i]);
	}
	free(maps);
}

void drawObject(float dt)
{
	bool check = true;
	MapTile* m = maps[pc->tileNumber];
	for (int i = 0; i < monsterNum; i++)
	{
		if (totalMonster[i]->tileNumber == pc->tileNumber)
		{
			//닫힘
			check = false;
			break;
		}
	}

	int tmp = check ? 01 : WALLTILE;
		for (int j = 0; j < 3; j++) // 나무통
		{
			MapObject* mobj = m->mapObj[j];
			m->rgTile[mobj->objTile[0]] = tmp;
			m->rgTile[mobj->objTile[0] + 1] = tmp;
			m->rgTile[mobj->objTile[0] + RGTILE_X] = tmp;
			m->rgTile[mobj->objTile[0] + RGTILE_X + 1] = tmp;
		}

		for (int j = 0; j < 4; j++)
		{
			MapObject* mobj = m->mapObj[j + 3];
			for (int k = 0; k < mobj->objTileNum; k++)
			{
				m->rgTile[mobj->objTile[k]] = tmp;
			}
		}

	

	for (int j = 0; j < 7; j++)
	{
		if (m->mapObj[j]->objImg)
		{
			MapObject* mobj = m->mapObj[j];
			iPoint p = m->tileOff + mobj->objPos + SET_DRAW_OFF;
			iImage* img = mobj->objImg;
			img->setTexAtIndex(check);
			img->paint(dt, p);
		}
	}
}

void drawRoomTile(float dt)
{
	MapTile* m = maps[pc->tileNumber];
	iPoint p = m->tileOff + SET_DRAW_OFF;

	m->mapImg->paint(dt, p);

	drawObject(dt);
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