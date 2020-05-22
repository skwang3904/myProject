#include "EnemyComposition.h"

#include "Room.h"
#include "Stage.h"

#include "EnemyStruct.h"

int activeTile[MAPTILE_NUM];

void setEnemyPosition(uint8 pcTile)
{
	int i, j, num = 0;
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
	for (i = 0; i < 10; i++)
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

		if (exist)
			continue;

		check[i] = t;
		if (check[i] == pcTile || check[i] == nextDoor)
		{
			i--;
			continue;
		}

		for (j = 0; j < 1; j++)
		{

			EnemyGolem* enm = golems[i +j];
			enm->golemPos = maps[check[i]]->tileOff +
				iPointMake(RGTILE_Width * 3 + RGTILE_Width * 2 * (j % 3),
					RGTILE_Height * 2 + RGTILE_Height * 2 * (j / 3));
			enm->tileNumber = check[i];
		}

		if (elete == false)
		{
			for (j = 0; j < 1; j++)
			{
				EnemyGolem* enm = golemEletes[j];
				enm->golemPos = maps[check[i]]->tileOff +
					iPointMake(RGTILE_Width * 3 + RGTILE_Width * 2 * (j % 3),
						RGTILE_Height * 2 + RGTILE_Height * 2 * (j / 3));
				elete = true;
				enm->tileNumber = check[i];
			}
		}
	}
}