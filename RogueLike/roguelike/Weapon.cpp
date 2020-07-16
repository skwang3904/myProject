#include "Weapon.h"

#include "Room.h"

#include "WMelee.h"
#include "WRange.h"

Weapon* weapon;
PlayerWP* PWP;

Weapon::Weapon()
{
	PWP = (PlayerWP*)malloc(sizeof(Player) * TOTAL_WP_NUM);
	createMeleeWeapon();
	createRangeWeapon();
	
	for (int i = 0; i < TOTAL_WP_NUM; i++) // 총 좌표수
	{
		for (int j = TILEOFF_NUM-1; j > -1; j--)
		{
			if (maps[j]->rgTile != NULL)
			{
				PWP[i].pos = maps[j]->tileOff + RGTILE_CENTER;
				break;
			}
		}
	}
}

Weapon::~Weapon()
{
	freeMeleeWeapon();
	freeRangeWeapon();
	free(PWP);
}

void Weapon::drawWeapon(float dt)
{
	for (int i = 0; i < TOTAL_WP_NUM; i++)
	{
		if (PWP[i].drop)
			PWP[i].method(dt, PWP[i].pos);
	}
}

//----------------------------------------------------------------------------'

iImage* infoFromMW(const char* info)
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(300, 300);
	g->init(size);

	setRGBA(0.3, 0.3, 1, 1);
	g->fillRect(0, 0, size.width, size.height, 30);
	setRGBA(1, 1, 1, 1);

	setStringRGBA(0, 0, 0, 1);
	setStringSize(30);
	setStringBorder(0);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, info);

	iImage* img = new iImage();
	Texture* tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);

	return img;
}
