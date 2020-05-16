#include "EnemyComposition.h"

#include "RgTile.h"
#include "Room.h"
#include "Enemy.h"


int activeTile[MAPTILE_NUM];

void setEnemyPosition(int pcTile)
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

	int mn = MAPTILE_NUM /2;
	for (i = 0; i < mn; i++)
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
		{
			i--;
			continue;
		}

		check[i] = t;

		for (j = 0; j < 6; j++)
		{
			if (check[i] == pcTile)
				continue;
			EnemyNomal* enm = enemys[6 * i +j];
			enm->EnemyNomalPosition = maps[check[i]]->tileOff +
				iPointMake(RGTILE_Width * 5 + RGTILE_Width * 3 * (j % 3),
					RGTILE_Height * 3 + RGTILE_Height * 5 * (j / 3));
		}
	}
}

void resetEnemyPosition()
{

}
