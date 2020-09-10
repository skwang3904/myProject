#include "Item.h"

#include "Tile.h"
#include "Map.h"
#include "ProcData.h"
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
	int i, j;
	iImage* img;
	Texture* tex;
	iSize size;

	imgNum = 1;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);

	for (i = 0; i < imgNum; i++)
	{
		imgs[i] = imgItems[index]->copy();
	}

	this->img = imgs[0];

	get = false;
	ItemInfo* it = &itemInfo[index];
	value = it->value;
	actionDt = _actionDt = it->_actionDt;

	dropPosition = iPointZero;
	dropHeight = 50;

	targetPosition = iPointZero;
}

Item::~Item()
{
	if (itemNum == 0)
	{
		if (imgItems)
			freeItemImage();
	}
}

void Item::paint(float dt, iPoint off)
{
	if (alive == false)
		return;

	iPoint p = position;
	float delta = actionDt / _actionDt;
	if (get)
	{
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
				printf("touch item\n");
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

	iRect rt = touchRect;
	rt.origin += off;
	setRGBA(1, 0, 0, 1);
	fillRect(rt);
	setRGBA(1, 1, 1, 1);

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
	targetPosition = maps[mapNumber]->tileOff + iPointMake(0, TILE_NUM_Y * TILE_Height - img->tex->height);
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
	free(item);
}

void drawItem(float dt)
{
	int i;
	for (i = 0; i < itemNum; i++)
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
			drawImage(t, size.width / 2.0f, size.height / 2.0f,
				0, 0, t->width, t->height,
				VCENTER | HCENTER, size.width / t->width, size.height / t->height,
				2, 0, REVERSE_HEIGHT);
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

