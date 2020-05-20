#include "Weapon.h"

#include "WMelee.h"

#include "Room.h"


Weapon* weapon;
Weapon::Weapon()
{
	createMeleeWeapon();
	
	int i, j = 0;
	for ( i = 0; i < meleeNum; i++) // 총 좌표수
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
	for (int i = 0; i < meleeNum; i++)
	{
		if (PMW[i].drop)
			PMW[i].method(dt, PMW[i].pos);
	}
}
