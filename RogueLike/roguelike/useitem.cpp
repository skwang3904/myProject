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

    alive = false;
    value = 0.0f;
    type = it;

    sp = iPointZero;
    aniHeight = 100.0f;
    aniDt = 0.0f;

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
        if (pc->hp > 100.0f)
            pc->hp = 100.0f;
        break;
    }
    case dmgUp:
    {

        break;
    }
	}
}

#define ANIDT 3.0f
bool UseItem::animation(float dt)
{
    if (sp == itemPos)
        return false;

    aniDt += dt;
    if (aniDt > ANIDT)
    {
        sp = itemPos;
        aniDt = ANIDT;
    }
    // 수정
    float h = aniHeight - aniHeight * (fabsf((aniDt / ANIDT) * 2.0f - 1))* -1.0f + 1;
    printf("%.2f\n", h);
    iPoint a = linear(aniDt / ANIDT, sp, itemPos);
    iPoint b = a + iPointMake(0, h);
    
    drawitemPos = b + pc->camPosition + setPos;
    img->paint(dt, drawitemPos, REVERSE_NONE);

    return true;
}

void UseItem::paint(float dt)
{
    if (animation(dt))
        return;

    if (alive == false)
        return;

    iRect rt = touchItem;
    rt.origin += pc->camPosition + setPos;
    setRGBA(1, 0, 0, 0.5f);
    fillRect(rt);
    setRGBA(1, 1, 1, 1);

    img->paint(dt, drawitemPos, REVERSE_NONE);

    drawitemPos = itemPos + pc->camPosition + setPos;

    iPoint p = pc->playerPosition + HALF_OF_TEX_POINT;
    
    if (containPoint(p, touchItem))
    {
        alive = false;
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

        ui->alive = true;
        ui->value = 1 + i * 9;

        iPoint p = iPointMake(-100 + 200 * i, 0);
        ui->sp = enm->golemPos;
        ui->itemPos = enm->golemPos + p;
        ui->drawitemPos = ui->itemPos + (pc->camPosition + setPos);

        ui->touchItem = iRectMake(ui->itemPos.x + tex->width * 0.25f, 
            ui->itemPos.y + tex->height * 0.25f,
            tex->width * 0.5f, tex->height * 0.5f);

        iPoint pp = iPointMake(ui->touchItem.origin.x, ui->touchItem.origin.y);
    }
}