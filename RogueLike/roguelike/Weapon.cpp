#include "Weapon.h"

#include "Room.h"

#include "WMelee.h"

Weapon* weapon;
Weapon::Weapon()
{
	createMeleeWeapon();
	
	int i, j = 0;
	for ( i = 0; i < MELEE_NUM; i++) // 총 좌표수
	{
		for (; j < TILEOFF_NUM; j++)
		{
			if (maps[j]->rgTile != NULL)
			{
				PMW[i].pos = maps[j]->tileOff + RGTILE_CENTER;
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
	for (int i = 0; i < MELEE_NUM; i++)
	{
		if (PMW[i].drop)
			PMW[i].method(dt, PMW[i].pos);
	}
}
