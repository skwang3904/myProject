#include "RgGame.h"

#include "RgIntro.h"
#include "RgMenu.h"
#include "RgProc.h"

int gamestat;

#define myTest 0

void loadRgGame()
{
#if 1
	loadRgProc();
	gamestat = gs_proc;
#else
#endif

#if myTest
	void testCreateTile();
	testCreateTile();
#endif
}

void freeRgGame()
{
	switch (gamestat)	{
	case gs_intro:	freeRgIntro();	break;
	case gs_menu:	freeRgMenu();	break;
	case gs_proc:	freeRgProc();	break;
	}
}

void drawRgGame(float dt)
{
#if myTest
	return;
#endif
	switch (gamestat) {
	case gs_intro:	drawRgIntro(dt);	break;
	case gs_menu:	drawRgMenu(dt);		break;
	case gs_proc:	drawRgProc(dt);		break;
	}
}

void keyRgGame(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
		zoomLib(point, 2.0f);

	switch (gamestat) {
	case gs_intro:	keyRgIntro(stat, point);	break;
	case gs_menu:	keyRgMenu(stat, point);		break;
	case gs_proc:	keyRgProc(stat, point);		break;
	}
}

#if myTest
#include "RgTile.h"

struct LinkCheck
{
	int index;
	int tile;
	bool visit;
};

LinkCheck lc[TILEOFF_NUM];
int visitNum;

void visit(LinkCheck* l)
{
	if (l->visit || l->tile == -1)
		return;
	l->visit = true;
	visitNum++;

	int index = l->index;
	int x = index % 5;
	int y = index / 5;
	if (x > 0) visit(&lc[index - 1]);
	if (x < 4) visit(&lc[index + 1]);
	if (y > 0) visit(&lc[index - 5]);
	if (y < 4) visit(&lc[index + 5]);
}

bool allLink(int* tile, int total, int num)
{
	int i, n;
	for (i = 0; i < TILEOFF_NUM; i++)
	{
		LinkCheck* l = &lc[i];
		l->index = i;
		l->tile = tile[i];
		l->visit = false;
	}

	visitNum = 0;
	for (i = 0; i < total; i++)
	{
		LinkCheck* l = &lc[i];
		if (l->tile != -1)
		{
			visit(l);
			break;
		}
	}

	return visitNum == num;
}

void testCreateTile()
{
	int i, tile[TILEOFF_NUM];

	int tt = 0;
	while (1)
	{
		for (i = 0; i < TILEOFF_NUM; i++)
			tile[i] = -1;

		int n = 0;
		while (n< MAPTILE_NUM)
		{
			int r = random() % TILEOFF_NUM;
			if (tile[r] == -1)
			{
				tile[r] = 0;
				n++;
			}
		}

		tt++;
		if (allLink(tile, TILEOFF_NUM, MAPTILE_NUM))
			break;
	}

	printf("total = %d\n", tt);
	for (i = 0; i < TILEOFF_NUM; i++)
		printf("%d%s", tile[i], i % 5 < 4 ? "\t" : "\n");

	for (i = 0; i < TILEOFF_NUM; i++)
	{
		if (tile[i] == 0)
		{
			
		}
	}
}

#endif