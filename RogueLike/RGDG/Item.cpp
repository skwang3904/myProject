#include "Item.h"

#include "Proc.h"
#include "ProcData.h"
#include "Tile.h"
#include "Map.h"
#include "PlayerChar.h"
#include "Monster.h"

#define ITEM_DROP_DISTANCE 70
static iPoint itemDropPosition[10];

Item*** _item = NULL;
Item** item = NULL;
int itemNum;

iImage** imgItems = NULL;
void createItemImage();
void freeItemImage();

Item::Item(int index, int8 mapNum, iPoint pos) : Object(index, mapNum, pos)
{
	if (imgItems == NULL)
		createItemImage();

	this->img = imgItems[index]->copy();

	get = false;
	ItemInfo* it = &itemInfo[index];
	value = it->value;
	actionDt = _actionDt = it->_actionDt;


	touchSize = iSizeMake(this->img->tex->width, this->img->tex->height);
	dropPosition = iPointZero;
	dropHeight = 50;

	targetPosition = iPointZero;
}

Item::~Item()
{
	delete img;

	if (imgItems)
		freeItemImage();
}

void Item::paint(float dt, iPoint off)
{
	if (alive == false)
		return;

	iPoint p = position;
	float delta = actionDt / _actionDt;
	if (get)
	{
		targetPosition = player->position;
		if (actionDt == _actionDt)
		{
			p = targetPosition;
		}
		else if (actionDt < _actionDt)
		{
			actionDt += dt;
			if (actionDt > _actionDt)
			{
				actionDt = _actionDt;
				alive = false;
				touchRect = iRectZero;
			}
			p = linear(delta, position, targetPosition);
		}
	}
	else
	{
		if (actionDt == _actionDt)
		{
			if (containRect(touchRect, player->touchRect))
			{
				audioPlay(AUDIO_ItemPickUp);
				action(player);
			}
		}
		else if (actionDt < _actionDt)
		{
			actionDt += dt;
			if (actionDt > _actionDt)
			{
				actionDt = _actionDt;
				position = dropPosition;	
				touchRect = iRectMake(position.x, position.y, img->tex->width, img->tex->height);
			}
			p = linear(delta, position, dropPosition);
			p.y += -dropHeight * _sin(180 * delta) * _sin(180 * delta);
		}
	}

#if SHOW_TOUCHRECT
	iRect rt = touchRect;
	rt.origin += off;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);
#endif

	img->paint(dt, p + off);
}

void Item::drawShadow(float dt, iPoint off)
{
}

void Item::action(Object* obj)
{
	// get item
	get = true;
	actionDt = 0.0f;
	//targetPosition = obj->position;

	PlayerChar* p = player;
	switch (index)
	{
	case 0:
	case 1:
	case 2: 
	{
		p->hp += value;
		if (p->hp > 100.0f)
			p->hp = 100.0f;
		break;
	}
	default:
		break;
	}
	
}

void Item::aliveItem(Object* obj)
{

}

void Item::addItem(Object* obj, iPoint dropPos)
{
	mapNumber = obj->mapNumber;
	position = obj->position;
	touchRect = iRectZero;

	alive = true;
	get = false;
	actionDt = 0.0f;
	dropPosition = position + dropPos;
}

//-----------------------------------------------------------

void loadItem()
{
	int i, j;
	if (item == NULL)
	{
		itemNum = 0;
		item = (Item**)malloc(sizeof(Item*) * 300);

		_item = (Item***)malloc(sizeof(Item**) * item_max);
		for (i = 0; i < item_max; i++)
		{
			_item[i] = (Item**)malloc(sizeof(Item*) * ITEM_CREATE_NUM);

			for (j = 0; j < ITEM_CREATE_NUM; j++)
			{
				_item[i][j] = new Item(i, 0, iPointZero);
			}
		}
	}
	else
	{
		for (i = 0; i < itemNum; i++)
			item[i]->alive = false;
		itemNum = 0;
	}
}

void freeItem()
{
	int i, j;
	
	for (i = 0; i < item_max; i++)
	{
		for (j = 0; j < ITEM_CREATE_NUM; j++)
			delete _item[i][j];
		free(_item[i]);
	}
	free(_item);
	_item = NULL;
	free(item);
	item = NULL;
}

void drawItem(float dt)
{
#if SORTING
	for (int i = 0; i < itemNum; i++)
	{
		Item* it = item[i];
		if (it->alive == false)
		{
			itemNum--;
			item[i] = item[itemNum];
			i--;
			continue;
		}
		objects[procSort->sdNum] = it;
		procSort->add(it->position.y + it->img->tex->height);
	}
#else
	for (int i = 0; i < itemNum; i++)
	{
		Item* it = item[i];
		it->paint(dt, DRAW_OFF);

		if (it->alive == false)
		{
			itemNum--;
			item[i] = item[itemNum];
			i--;
		}
	}
#endif
}

//-----------------------------------------------------------

void createItemImage()
{
	int i, j, k;
	Texture* tex;
	iImage* img;
	iSize size;
	imgItems = (iImage**)malloc(sizeof(iImage*) * item_max);

	for (i = 0; i < item_max; i++)
	{
		img = new iImage();

		ItemImageInfo* iti = &itemImageInfo[i];
		size = iti->size;
		for (j = 0; j < iti->imgNum; j++)
		{
			tex = createTexture(size.width, size.height);

			fbo->bind(tex);
			//Texture* t = createImage(iti->strPath, j);
			Texture* t = createImage(iti->strPath);
			DRAWIMAGE(t, size);
			freeImage(t);
			fbo->unbind();
			
			img->addObject(tex);
			freeImage(tex);
		}

		img->_aniDt = iti->aniDt;
		imgItems[i] = img;
	}
}

void freeItemImage()
{
	int i;

	for (i = 0; i < item_max; i++)
		delete imgItems[i];
	free(imgItems);
	imgItems = NULL;
}

//-----------------------------------------------------------

void setItemDropPosition(int dropNum)
{
	float distance = -1.0f * ((float)dropNum / 2.0f) * ITEM_DROP_DISTANCE;
	for (int i = 0; i < dropNum; i++)
	{
		itemDropPosition[i] = iPointMake(distance + ITEM_DROP_DISTANCE * i, 0);
	}
}

void dropMonsterItem(Monster* m)
{
	int i, j, num = m->itemDropNum;
	setItemDropPosition(num);

	int kind;
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < ITEM_CREATE_NUM; j++)
		{
			kind = m->itemTypeKind[random() % m->itemTypeKindNum];
			Item* it = _item[kind][j];
			if (it->alive == false)
			{
				it->addItem(m, itemDropPosition[i]);

				item[itemNum] = it;
				itemNum++;
				break;
			}
		}
	}
}

