#include "TreasureChest.h"

#include "Room.h"

Chest::Chest(ChestType ct)
{
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		if (maps[i]->rgTile != NULL && i != pc->tileNumber)
		{

		}
	}
}

Chest::~Chest()
{

}