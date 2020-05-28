#include "TreasureChest.h"

#include "Room.h"

#include "Weapon.h"
#include "WMelee.h"
#include "WRange.h"
#include "useitem.h"


Chest* basicChect;
Chest* CommonChect;
Chest* RareChect;
Chest* UniqueChect;

iImage* imgBasicChest;
iImage* imgCommonChest;
iImage* imgRareChest;
iImage* imgUniqueChest;

void basicChestImg()
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(100, 100);
	g->init(size);

	setRGBA(0.5f, 1, 0.5f, 1);
	g->fillRect(0,0,size.width,size.height, 10);
	
	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	imgBasicChest = img;
}

void commonChestImg()
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(100, 100);
	g->init(size);

	setRGBA(0.5f, 1, 0.5f, 1);
	g->fillRect(0, 0, size.width, size.height, 30);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	imgCommonChest = img;
}

void rareChestImg()
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(100, 100);
	g->init(size);

	setRGBA(0.5f, 1, 0.5f, 1);
	g->fillRect(0, 0, size.width, size.height, 50);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	imgRareChest = img;
}

void uniqueChestImg()
{
	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(100, 100);
	g->init(size);

	setRGBA(0.5f, 1, 0.5f, 1);
	g->fillRect(0, 0, size.width, size.height, 70);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	imgUniqueChest = img;
}

//------------------------------------------------------------------------------

void createChestImg()
{
	basicChestImg();
	commonChestImg();
	rareChestImg();
	uniqueChestImg();
}

void freeChestImg()
{
	delete imgBasicChest;
	delete imgCommonChest;
	delete imgRareChest;
	delete imgUniqueChest;
}

//------------------------------------------------------------------------------

void openChestBasic(Chest* me);

Chest::Chest(ChestType ct)
{
	for (int i = 0; i < TILEOFF_NUM; i++)
	{
		int a = random() % TILEOFF_NUM;
		if (maps[a]->rgTile != NULL && a != pc->tileNumber)
		{
			pos = maps[a]->tileOff + RGTILE_CENTER;
			drawPos = pos + pc->camPosition + setPos;
			tileNumber = a;

			break;
		}
	}
	
	items = (UseItem**)malloc(sizeof(UseItem*) * 3);	
	switch (ct)
	{
	case basic:
	{
		for (int i = 0; i < 3; i++)
		{
			img = imgBasicChest->copy();
			items[i] = new UseItem(coin);
		}
		break;
	}
	case common:
	{
		for (int i = 0; i < 3; i++)
		{
			img = imgCommonChest->copy();
			items[i] = new UseItem(healing);
		}
		break;
	}
	case rare:
	{
		for (int i = 0; i < 3; i++)
		{
			img = imgRareChest->copy();
			items[i] = new UseItem(powerUp);
		}
		break;
	}
	case unique:
	{
		for (int i = 0; i < 3; i++)
		{
			img = imgUniqueChest->copy();
			items[i] = new UseItem(coin);
		}
		break;
	}
	default:
		break;
	}

	touchRect = iRectMake(pos.x, pos.y, img->tex->width, img->tex->height);

	open = false;
	openMethod = openChestBasic;
}

Chest::~Chest()
{
	delete img;

	for (int i = 0; i < 3; i++)
		delete items[i];
	free(items);
}

bool Chest::openAni(float dt)
{
	if (open == false)
		return false;
	// 오픈 애니메이션
	// 다 열리면 드랍
	// 드랍 아이템
	return true;
}

void Chest::paint(float dt)
{
	for (int i = 0; i < 3; i++)
	{
		items[i]->paint(dt);
	}

	if (openAni(dt))
		return;

	drawPos = pos + pc->camPosition + setPos;
	img->paint(dt, drawPos, REVERSE_NONE);
	
	//if( ? ) open = true;

	if (pc->pwp->isMelee)
	{
		meleeWeapon* mw = (meleeWeapon*)pc->pwp->wp;
		if (pc->act == attacking && mw->attackSpeed > -0.1f)
		{
			if (containRect(mw->hitBox, touchRect))
			{
				open = true;
				printf("Chest Open\n");
				openMethod(this);
			}
		}
	}
	else
	{
		;
	}
}

void createChest()
{
	createChestImg();
	basicChect = new Chest(basic);
	CommonChect = new Chest(common);
	RareChect = new Chest(rare);
	UniqueChect = new Chest(unique);
}

void freeChest()
{
	freeChestImg();
	delete basicChect;
	delete CommonChect;
	delete RareChect;
	delete UniqueChect;
}

void drawChest(float dt)
{
	basicChect->paint(dt);
	CommonChect->paint(dt);
	RareChect->paint(dt);
	UniqueChect->paint(dt);
}

//------------------------------------------------------------------------------

void openChestBasic(Chest* me)
{
	for (int i = 0; i < 3; i++)
	{
		UseItem* ui = me->items[random() % 3];
		Texture* tex = ui->img->tex;

		ui->alive = true;
		ui->aniDt = 0.0f;
		ui->value = i * 10;

		iPoint cp = iPointMake(me->img->tex->width / 2, 
			me->img->tex->height / 2);
		iPoint p = iPointMake(-100 + 100 * i, 0);
		ui->sp = me->pos + cp;
		ui->itemPos = me->pos + cp + p;
		ui->drawitemPos = ui->itemPos + pc->camPosition + setPos;

		ui->touchItem = iRectMake(ui->itemPos.x + tex->width * 0.25f,
			ui->itemPos.y + tex->height * 0.25f,
			tex->width * 0.5f, tex->height * 0.5f);
	}
}