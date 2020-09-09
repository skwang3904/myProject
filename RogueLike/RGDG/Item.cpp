#include "Item.h"

#include "Tile.h"
#include "Map.h"
#include "ProcData.h"
#include "PlayerChar.h"

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
	targetPosition = iPointZero;
}

Item::~Item()
{
	if (imgItems)
		freeItemImage();
}

void Item::paint(float dt, iPoint off)
{
	if (alive == false)
		return;

	iPoint p = position;
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
				//alive = false;
				touchRect = iRectZero;
			}
			p = linear(actionDt / _actionDt, position, targetPosition);
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
				dropPosition = iPointZero;
			}
			p = linear(actionDt / _actionDt, position, dropPosition);
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
	get = true;
	actionDt = 0.0f;
	targetPosition = maps[mapNumber]->tileOff + iPointMake(0, TILE_NUM_Y * TILE_Height - img->tex->height);
}

void Item::dropItem(int* index, int typeNum, int dropNum)
{
	int tn = typeNum;
	int type;

	for (int i = 0; i < dropNum; i++)
	{
		type = index[random() % typeNum];
		for (int j = 0; j < ITEM_CREATE_NUM; j++)
		{
			Item* it = _item[type][j];
			if (it->alive == false)
			{
				;

			}

		}
	}
}

//-----------------------------------------------------------

void loadItem()
{
	int i, j;
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

#if 1
	Item* it = _item[0][0];
	it->mapNumber = player->mapNumber;
	it->alive = true;
	it->actionDt = 0.0f;
	it->position = maps[it->mapNumber]->tileOff + iPointMake(300, 300);
	it->dropPosition = it->position;
	it->touchRect = iRectMake(it->position.x, it->position.y, it->img->tex->width, it->img->tex->height);

	item[itemNum] = _item[0][0];
	itemNum++;
#endif
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