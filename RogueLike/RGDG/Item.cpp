#include "Item.h"

#include "Tile.h"
#include "Map.h"
#include "PlayerChar.h"

Item* item;
Item::Item(int index) : Object(index)
{
	iImage* img;
	Texture* tex;

	img = new iImage();
	iSize size = iSizeMake(128, 128);
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	setRGBA(0, 1, 0, 1);
	fillRect(0, 0, size.width, size.height);
	setRGBA(1, 0, 1, 1);
	fillRect(size.width * 0.25f, size.height * 0.25f, size.width * 0.5f, size.height * 0.5f);
	setRGBA(1, 1, 1, 1);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);

	alive = true;
	this->img = img;
	mapNumber = 0;
	position = maps[mapNumber]->tileOff + iPointMake(24 * TILE_Width, 12 * TILE_Height);
	touchRect = iRectMake(position.x, position.y, size.width, size.height);


	value = 5.0f;
	get = false;
	getItemDt = 0.0f;
	_getItemDt = 2.0f;
}

Item::~Item()
{
	delete img; // test
}

void Item::paint(float dt, iPoint off)
{
	if (alive == false)
		return;

	iPoint p = position;
	if (get)
	{
		getItemDt += dt;
		if (getItemDt > _getItemDt)
		{
			getItemDt = _getItemDt;
			alive = false;
		}
		iPoint pp = player->position;// +iPointMake(player->img->tex->width / 2.0f, player->img->tex->height / 2.0f);
		p = linear(getItemDt / _getItemDt, position, pp);
	}
	else
	{
		if (containRect(touchRect, player->touchRect))
		{
			printf("touch item\n");
			getItem();
		}
	}

	
	img->paint(dt, p + off);
}

void Item::getItem()
{
	if (get)
		return;

	get = true;
	switch (index)
	{
		
	default:
		break;
	}
}

void loadItem()
{
	item = new Item(0);
}

void freeItem()
{
	delete item;
}

void drawItem(float dt)
{
	item->paint(dt, DRAW_OFF);
}