#include "iShortestPath.h"

#include "iStd.h"

iShortestPath::iShortestPath()
{
	tiles = NULL;
	tileX = 0;
	tileY = 0;

	ti = NULL;
	tiNum = 0;
}

iShortestPath::~iShortestPath()
{
	if (ti)
		free(ti);
}

void iShortestPath::init(int* tiles, int tileX, int tileY)
{
	this->tiles = tiles;
	this->tileX = tileX;
	this->tileY = tileY;
	tileXY = tileX * tileY;

	if (tiNum < tileXY)
	{
		if (ti)
			free(ti);
		ti = (TileInfo*)malloc(sizeof(TileInfo) * tileXY);
		tiNum = tileXY;
	}
}

void iShortestPath::dijkstra(int start, int end, int* path, int& pathNum)
{
	for (int i = 0; i < tileXY; i++)
	{
		TileInfo* t = &ti[i];
		t->value = sp_inf;
		t->visit = false;
		t->pathNum = 0;
	}

	TileInfo* t = &ti[start];
	t->value = 0;
	t->path[0] = start;
	t->pathNum = 1;

	while (1)
	{
		int curr = -1, next;
		int min = sp_inf;
		for (int i = 0; i < tileXY; i++)
		{
			if (ti[i].value < min && ti[i].visit==false)
			{
				curr = i;
				min = ti[i].value;
			}
		}
		if (curr == -1)
			break;

		TileInfo *tiCurr = &ti[curr], *tiNext;
		tiCurr->visit = true;

		for (int i = 0; i < 4; i++)
		{
			if (i == 0) {// left
				if (curr % tileX == 0) continue;
				next = curr - 1;
			}
			else if (i == 1) {// right
				if (curr % tileX == tileX - 1) continue;
				next = curr + 1;
			}
			else if (i == 2) {// up
				if (curr / tileX == 0) continue;
				next = curr - tileX;
			}
			else if (i == 3) {// down
				if (curr / tileX == tileY - 1) continue;
				next = curr + tileX;
			}
			if (tiles[next] == sp_inf) continue;
			tiNext = &ti[next];
			int value = tiCurr->value + tiles[next];
			if (value < tiNext->value)
			{
				tiNext->value = value;
				tiNext->pathNum = tiCurr->pathNum;
				memcpy(tiNext->path, tiCurr->path, sizeof(int) * tiCurr->pathNum);
				tiNext->path[tiCurr->pathNum] = next;
				tiNext->pathNum++;
			}
		}
	}

	pathNum = ti[end].pathNum;
	memcpy(path, ti[end].path, sizeof(int) * pathNum);
}

void iShortestPath::removeDuplicate(int* path, int& pathNum)
{
	for (int i = 0; i < pathNum - 2; i++)
	{
		int cp = path[i];
		int cx = cp % tileX;
		int cy = cp / tileX;

		int np = path[1 + i];
		int nx = np % tileX;
		int ny = np / tileX;

		if (cx == nx)
		{
			int n = pathNum;
			for (int j = 2 + i; j < n; j++)
			{
				int p = path[j];
				int x = p % tileX;
				if (cx == x)
				{
					memcpy(&path[1 + i], &path[j], sizeof(int) * (n - j));
					pathNum--;
				}
				else break;
			}
		}
		else if (cy == ny)
		{
			int n = pathNum;
			for (int j = 2 + i; j < n; j++)
			{
				int p = path[j];
				int y = p / tileX;
				if (cy == y)
				{
					memcpy(&path[1 + i], &path[j], sizeof(int) * (n - j));
					pathNum--;
				}
				else break;
			}
		}
	}
}


int tiles[70] = {
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 99, 01, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 99, 01, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 99, 01, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 99, 01, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 99, 01, 01, 01, 01,
	01, 01, 01, 01, 01, 01, 01, 01, 01, 01,
};

class Me
{
public:
	Me()
	{
		p = tp = iPointZero;
		memset(path, 0x00, sizeof(int) * tile_num_w * tile_num_h);
		pathIndex = pathNum;
		speed = 100;
	}

	virtual ~Me()
	{

	}

	void move(float dt)
	{
		if (p != tp)
		{
			if (p.x < tp.x)
			{
				p.x += speed * dt;
				if (p.x > tp.x)
					p.x = tp.x;
			}
			else if (p.x > tp.x)
			{
				p.x -= speed * dt;
				if (p.x < tp.x)
					p.x = tp.x;
			}
			if (p.y < tp.y)
			{
				p.y += speed * dt;
				if (p.y > tp.y)
					p.y = tp.y;
			}
			else if (p.y > tp.y)
			{
				p.y -= speed * dt;
				if (p.y < tp.y)
					p.y = tp.y;
			}
		}
		else
		{
			if (pathIndex < pathNum)
			{
				int index = path[pathIndex];
				tp.x = tile_size_w * (index % tile_num_w) + tile_size_w / 2;
				tp.y = tile_size_h * (index / tile_num_w) + tile_size_h / 2;
				pathIndex++;
			}
		}
	}
public:
	int path[tile_num_w * tile_num_h], pathNum;
	int pathIndex;

	iPoint p, tp;
	float speed;
};

iShortestPath* sp;
Me* me;

struct TOTAL
{
	Texture* tex;	// 전체 이미지
	uint8* rgba;	// 전체 rgba
	bool* gone;
	iRect rt;		// 선택된 영역
	iRect rtDrag;
	iImage* img;	// 선택된 영역의 이미지의 리스트
};

TOTAL* total;

void initShortestPath()
{
	sp = new iShortestPath();
	sp->init(tiles, 10, 7);

	me = new Me();
	me->p = me->tp = iPointMake(tile_size_w / 2, tile_size_h / 2);

	total = (TOTAL*)malloc(sizeof(TOTAL));
	const char* path = "assets/atlas0.png";
	total->tex = createImage(path);
	{
		wchar_t* ws = utf8_to_utf16(path);
		Bitmap* bmp = new Bitmap(ws);
		free(ws);
	
		int width, height;
		total->rgba = bmp2rgba(bmp, width, height);
		delete bmp;

		total->gone = (bool*)calloc(sizeof(bool), nextPOT(width) * nextPOT(height));
	}
	total->rt = iRectZero;
	total->rtDrag = iRectZero;
	total->img = NULL;
}

void freeShortestPath()
{
	delete sp;
	delete me;
}

void drawShortestPath(float dt)
{
	iPoint off = iPointZero;

	for (int i = 0; i < 70; i++)
	{
		switch (tiles[i]) {
		case 1:		setRGBA(1, 1, 1, 1); break;
		case sp_inf:setRGBA(1, 0, 0, 1); break;
		}
		fillRect(off.x + tile_size_w * (i % tile_num_w),
				off.y + tile_size_h * (i / tile_num_w),
				tile_size_w, tile_size_h);
		setRGBA(1, 0, 1, 1);
		drawRect(off.x + tile_size_w * (i % tile_num_w) + 2,
				off.y + tile_size_h * (i / tile_num_w) + 2,
				tile_size_w - 4, tile_size_h - 4);
	}
	
	setRGBA(0, 0, 1, 1);
	fillRect(me->p.x - 20, me->p.y - 20, 40, 40);
	me->move(dt);

	setRGBA(1, 1, 1, 1);
	fillRect(0, 0, total->tex->width, total->tex->height);
	drawImage(total->tex, 0, 0, TOP | LEFT);

	if (total->rt.origin != iPointZero &&
		total->rt.size != iSizeZero)
	{
		setRGBA(1, 0, 0, 1);
		drawRect(total->rt.origin.x, total->rt.origin.y,
			total->rt.size.width, total->rt.size.height);
	}
	if (total->rtDrag.origin != iPointZero &&
		total->rtDrag.size != iSizeZero)
	{
		setRGBA(1, 0, 0, 1);
		drawRect(total->rtDrag.origin.x, total->rtDrag.origin.y,
			total->rtDrag.size.width, total->rtDrag.size.height);
	}
	setRGBA(1, 1, 1, 1);
}

void clickTotal(iPoint point);
void dragTotal(iRect drag);

iPoint p0Drag, p1Drag;
bool dragging = false;
void keyShortestPath(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		dragging = true;
		p0Drag = point;
	}
	else if (stat == iKeyStateMoved)
	{
		if (dragging)
		{
			p1Drag = point;
			if (p0Drag.x < p1Drag.x)
			{
				total->rtDrag.origin.x = p0Drag.x;
				total->rtDrag.size.width = p1Drag.x - p0Drag.x;
			}
			else
			{
				total->rtDrag.origin.x = p1Drag.x;
				total->rtDrag.size.width = p0Drag.x - p1Drag.x;
			}
			if (p0Drag.y < p1Drag.y)
			{
				total->rtDrag.origin.y = p0Drag.y;
				total->rtDrag.size.height = p1Drag.y - p0Drag.y;
			}
			else
			{
				total->rtDrag.origin.y = p1Drag.y;
				total->rtDrag.size.height = p0Drag.y - p1Drag.y;
			}
		}
	}
	else if (stat == iKeyStateEnded)
	{
		dragging = false;
		dragTotal(total->rtDrag);
		total->rtDrag = iRectZero;
	}
	return;

	if (stat == iKeyStateBegan)
	{
		iRect rt = iRectMake(0, 0, total->tex->width, total->tex->height);
		if (containPoint(point, rt))
		{
			clickTotal(point);
		}
		return;

		iPoint off = iPointZero;
		int sx = me->p.x - off.x;
		sx /= tile_size_w;

		int sy = me->p.y - off.y;
		sy /= tile_size_h;

		int ex = point.x - off.x;
		ex /= tile_size_w;

		int ey = point.y - off.y;
		ey /= tile_size_h;

		sp->dijkstra(
			sy * tile_num_w + sx,
			ey * tile_num_w + ex,
			me->path, me->pathNum);
		sp->removeDuplicate(me->path, me->pathNum);
		me->pathIndex = 1;

		for (int i = 0; i < me->pathNum; i++)
		{
			printf("%d, %d\n", i, me->path[i]);
		}
	}
}

int fact(int n)
{
	if( n > 1 )
		return n * fact(n - 1);
	return 1;
}

int _left, _right, _up, _down;
void findRect(int x, int y)
{
	int index = (int)total->tex->width * y + x;
	if (total->gone[index])
		return;
	total->gone[index] = true;

	index = (int)total->tex->potWidth * 4 * y + 4 * x;
	if (total->rgba[index + 3] == 0)
		return;

	if (x < _left)
		_left = x;
	if (x > _right)
		_right = x;
	if (y < _up)
		_up = y;
	if (y > _down)
		_down = y;

	findRect(x - 1, y);
	findRect(x + 1, y);
	findRect(x, y - 1);
	findRect(x, y + 1);
}

void clickTotal(iPoint point)
{
	// n! = n * (n-1) * (n-2) * ... * 1
	// 6! = 6 * 5 * 4 * 3 * 2 * 1
#if 0
	printf("fact = %d\n", fact(3));
#else
	Texture* tex = total->tex;
	//tex->width, tex->height, tex->potWidth, tex->potHeight;
	uint8* rgba = total->rgba;
	memset(total->gone, 0x00, (int)tex->potWidth * tex->potHeight);

	_left = 100000000;
	_right = -1000000000;
	_up = 10000000000;
	_down = -100000000;
	findRect(point.x, point.y);
	_left -= 2;
	_right += 2;
	_up -= 2;
	_down += 2;

	// left
	//for (int i = point.x; i >= _left - 1; i--)
	for (int i = point.x; i > _left; i--)
	{
		bool exist = false;
		for (int j = _up; j < _down; j++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		if (exist == false)
		{
			total->rt.origin.x = i + 1;
			break;
		}
	}
	// right
	for (int i = point.x; i < _right; i++)
	{
		bool exist = false;
		for (int j = _up; j < _down; j++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		if (exist == false)
		{
			total->rt.size.width = i - 1 - total->rt.origin.x;
			break;
		}
	}

	// top
	for (int j = point.y; j > _up; j--)
	{
		bool exist = false;
		for (int i = _left; i <= _right; i++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		if (exist == false)
		{
			total->rt.origin.y = j + 1;
			break;
		}
	}

	// bottom
	for (int j = point.y; j < _down; j++)
	{
		bool exist = false;
		for (int i = _left; i <= _right; i++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		if (exist == false)
		{
			total->rt.size.height = j - 1 - total->rt.origin.y;
			break;
		}
	}

#endif
}

void dragTotal(iRect drag)
{
	Texture* tex = total->tex;
	//tex->width, tex->height, tex->potWidth, tex->potHeight;
	uint8* rgba = total->rgba;

	for (int i = drag.origin.x; i < drag.origin.x + drag.size.width; i++)
	{
		bool exist = false;
		for (int j = drag.origin.y; j < drag.origin.y + drag.size.height; j++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		total->rt.origin.x = i;
	}

	for (int i = drag.origin.x + drag.size.width-1; i > drag.origin.x-1; i--)
	{
		bool exist = false;
		for (int j = drag.origin.y; j < drag.origin.y + drag.size.height; j++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		total->rt.size.width = i - total->rt.origin.x;
	}

	for (int j = drag.origin.y; j < drag.origin.y + drag.size.height; j++)
	{
		bool exist = false;
		for (int i = drag.origin.x; i < drag.origin.x + drag.size.width; i++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		total->rt.origin.y = j;
	}

	for (int j = drag.origin.y + drag.size.height-1; j > drag.origin.y-1; j--)
	{
		bool exist = false;
		for (int i = drag.origin.x; i < drag.origin.x + drag.size.width; i++)
		{
			if (rgba[(int)tex->potWidth * 4 * j + 4 * i + 3])
			{
				exist = true;
				break;
			}
		}
		total->rt.size.height = j - total->rt.origin.y;
	}
}




