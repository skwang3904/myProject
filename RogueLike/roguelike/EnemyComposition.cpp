#include "EnemyComposition.h"

#include "Room.h"
#include "Stage.h"

#include "EnemyData.h"
#include "EnemyStruct.h"


iPoint enemyPos[16] = {
{RGTILE_Width * 3,  RGTILE_Height * 3},
{RGTILE_Width * 3,  RGTILE_Height * 6},
{RGTILE_Width * 3,  RGTILE_Height * 9},
{RGTILE_Width * 8,  RGTILE_Height * 3},
{RGTILE_Width * 8,  RGTILE_Height * 6},
{RGTILE_Width * 8,  RGTILE_Height * 9},
{RGTILE_Width * 13, RGTILE_Height * 3},
{RGTILE_Width * 13, RGTILE_Height * 6},
{RGTILE_Width * 13, RGTILE_Height * 9},
{RGTILE_Width * 18, RGTILE_Height * 3},
{RGTILE_Width * 18, RGTILE_Height * 6},
{RGTILE_Width * 18, RGTILE_Height * 9},
{RGTILE_Width * 23, RGTILE_Height * 3},
{RGTILE_Width * 23, RGTILE_Height * 6},
{RGTILE_Width * 23, RGTILE_Height * 9},
{RGTILE_Width * 25, RGTILE_Height * 18}, 
};

void setEnemyPosition(int pcTile)
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

		int index = -1;
		for (j = 0; j < 4; j++)
		{
			if (index == -1)
				index = random() % 16;

			MonsterData* enm = golems[4 * i + j];
			enm->enemyPos = maps[check[i]]->tileOff + 
				enemyPos[index];

			index -= 1 + (random() % 3);
			if (index < 0)
				index = 15;
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
				MonsterData* enm = golemEletes[0];
				enm->enemyPos = maps[check[j]]->tileOff +
					RGTILE_CENTER;
				elete = true;
				enm->tileNumber = check[j];
				break;
			}
		}
	}
}
