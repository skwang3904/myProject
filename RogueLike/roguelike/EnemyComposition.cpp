#include "EnemyComposition.h"

#include "Room.h"
#include "Stage.h"

#include "EnemyStruct.h"


void setEnemyPosition(uint8 pcTile)
{
	int i, j, num = 0;
	int activeTile[MAPTILE_NUM];
	int check[MAPTILE_NUM];

	for (i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile != NULL)
		{
			activeTile[num] = i;
			num++;
		} 
	}

	bool elete = false;
	int mn = MAPTILE_NUM /2;
	//for (i = 0; i < mn; i++)
	int n = num - 1;
	for (i = 0; i < n; i++)
	{
		int t = activeTile[random() % num];
		bool exist = false;
		for (int k = 0; k < i; k++)
		{
			if (check[k] == t)
			{
				exist = true;
				break;
			}
		}

		check[i] = t;
		if (exist || check[i] == pcTile)
		{
			i--;
			continue;
		}

		for (j = 0; j < 2; j++)
		{
			EnemyGolem* enm = golems[2 * i + j];
			enm->golemPos = maps[check[i]]->tileOff +
				iPointMake(RGTILE_Width * 5 + RGTILE_Width * 5 * (j % 3),
					RGTILE_Height * 5 + RGTILE_Height * 5 * (j / 3));
			enm->tileNumber = check[i];
		}
	}

	if (elete == false)
	{
		for (j = 0; j < n; j++)
		{
			if (maps[check[j]]->rgTile== Tile1way1 ||
				maps[check[j]]->rgTile== Tile1way2 ||
				maps[check[j]]->rgTile== Tile1way3 ||
				maps[check[j]]->rgTile== Tile1way4)
			{
				EnemyGolem* enm = golemEletes[0];
				enm->golemPos = maps[check[j]]->tileOff +
					RGTILE_CENTER;
				elete = true;
				enm->tileNumber = check[j];
			}
		}
	}
}