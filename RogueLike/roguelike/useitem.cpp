#include "useitem.h"

#include "Room.h"
#include "EnemyStruct.h"

iImage* imgPotion;
iImage* imgCoin;

UseItem::UseItem(itemType it)
{
    switch (it)
    {
    case coin:
    {
        img = imgCoin->copy();
        break;
    }
    case healing:
    {
        img = imgPotion->copy();
        break;
    }
    case dmgUp:
    {
        break;
    }
    }

    value = 0.0f;
    type = it;

    itemPos = iPointZero;
    drawitemPos = iPointZero;
    touchItem = iRectZero;
}

UseItem::~UseItem()
{
    delete img;
}

void UseItem::gainValue()
{
	switch (type)
	{
    case coin:
    {
        pc->coin += value;
        break;
    }
    case healing:
    {
        pc->hp += value;
        break;
    }
    case dmgUp:
    {

        break;
    }
	}
}

void UseItem::paint(float dt)
{
    img->paint(dt, drawitemPos, REVERSE_NONE);

    setRGBA(1, 1, 0, 1);
    fillRect(touchItem.origin.x, touchItem.origin.y, touchItem.size.width, touchItem.size.height);
    setRGBA(1, 1, 1, 1);

    
    if (containRect(pc->touchPlayer, touchItem))
    {
        gainValue();
    }
}

 //----------------------------------------------------------------------------

void createItemImg()
{
   iImage* imgHP = new iImage();
   Texture* texHP = createImage("assets/item/heart.png");
   imgHP->addObject(texHP);
   freeImage(texHP);

   imgPotion = imgHP;

   //----------------------------------------------------------------------------

   iImage* imgMoney = new iImage();
   Texture* texMoney = createImage("assets/item/coin.png");
   imgMoney->addObject(texMoney);
   freeImage(texMoney);

   imgCoin = imgMoney;
}

void freeItemImg()
{
    delete imgPotion;
    delete imgCoin;
}


void golemItems(EnemyGolem* enm)
{
    for (int i = 0; i < 2; i++)
    {
        UseItem* ui = enm->items[i];
        Texture* tex = ui->img->tex;
        ui->value = i * 10;
        ui->itemPos = pc->playerPosition;

        iPoint p = iPointMake(-100 + 200 * i, 0);
        ui->drawitemPos = ui->itemPos + p + (pc->camPosition + maps[enm->tileNumber]->tileOff);

        printf("%.2f, %.2f\n", ui->drawitemPos.x, ui->drawitemPos.y);

        ui->touchItem = iRectMake(ui->itemPos.x, 
            ui->itemPos.y,
            tex->width, tex->height);

        iPoint pp = iPointMake(ui->touchItem.origin.x, ui->touchItem.origin.y);

        printf("%.2f, %.2f\n", pp.x, pp.y);

    }
}