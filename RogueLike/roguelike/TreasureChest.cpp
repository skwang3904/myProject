#include "TreasureChest.h"

#include "Room.h"

Chest::Chest(ChestType ct)
{
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		int a = random() % TILEOFF_NUM;
		if (maps[a]->rgTile != NULL && a != pc->tileNumber)
		{
			pos = maps[a]->tileOff + RGTILE_CENTER;
			tileNumber = a;
		}
	}

	switch (ct)
	{
	case basic:
	{

		break;
	}
	case common:
	{

		break;
	}
	case rare:
	{

		break;
	}
	case unique:
	{

		break;
	}
	default:
		break;
	}
}

Chest::~Chest()
{

}

void Chest::openAni()
{
	if (open == false)
		return;
}

void Chest::paint(float dt)
{

	//if( ? ) open = true;
}
