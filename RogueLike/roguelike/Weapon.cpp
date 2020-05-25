#include "Weapon.h"

#include "Room.h"

#include "WMelee.h"

Weapon* weapon;
Weapon::Weapon()
{
	createMeleeWeapon();
	
	for (int i = 0; i < MELEE_NUM; i++) // 총 좌표수
	{
		for (int j = TILEOFF_NUM-1; j > -1; j--)
		{
			if (maps[j]->rgTile != NULL)
			{
				PMW[i].pos = maps[j]->tileOff + RGTILE_CENTER;
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
