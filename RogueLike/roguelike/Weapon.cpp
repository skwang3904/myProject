#include "Weapon.h"

#include "WMelee.h"

#include "Room.h"


Weapon* weapon;
Weapon::Weapon()
{
	createMeleeWeapon();
	
	int i, j = 0;
	for ( i = 0; i < 10; i++) // 총 좌표수
	{
		for (; j < TILEOFF_NUM; j++)
		{
			if (maps[j]->rgTile != NULL)
			{
				wDropPos[i] = maps[j]->tileOff + RGTILE_CENTER;
				j++;
				break;
			}
		}
	}
}

Weapon::~Weapon()
{
	//freeMeleeWeapon();
}

void Weapon::drawWeapon(float dt)
{
	for (int i = 0; i < meleeNum; i++)
	{
		bool exist = false;
		for (int j = 0; j < pc->weaponArray->count; j++)
		{
			if (pc->weaponArray->objectAtIndex(j) == &PMW[i])
			{
				exist = true;
				break;
			}
		}

		if (exist)
			continue;
		else
			PMW[i].method(dt, weapon->wDropPos[i]);
	}
}
