#include "Weapon.h"

#include "WMelee.h"


Weapon* weapon;
Weapon::Weapon()
{
	createMeleeWeapon();
	

	for (int i = 0; i < 4; i++) // 초기 드랍좌표
		//wDropPos[i] = iPointMake(120 + 240 * i % 2, 80 + 320 * i / 2);
		wDropPos[i] = iPointMake(120 + 240 * i / 2, 80 + 320 * i % 2);
	wDropPos[0] = iPointMake(360, 240);
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
