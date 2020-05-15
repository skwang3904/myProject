#include "Weapon.h"

#include "WMelee.h"

#include "RgTile.h"
#include "Room.h"


Weapon* weapon;
Weapon::Weapon()
{
	createMeleeWeapon();
	
	for (int i = 0; i < 10; i++) // 총 좌표수
	{
		for (int j = 5; j < TILEOFF_NUM; j++)
		{
			if(maps[j]->rgTile != NULL)
				wDropPos[i] = maps[j]->tileOff + RGTILE_CENTER;
			
			if (j > TILEOFF_NUM - 1)
				j = 6;
		}
	}
}

Weapon::~Weapon()
{
	freeMeleeWeapon();
}

Weapon* Weapon::instance()
{
	static Weapon* wp = new Weapon();
	return wp;
}
