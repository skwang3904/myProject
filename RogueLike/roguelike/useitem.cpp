#include "useitem.h"

#include "Room.h"
#include "EnemyStruct.h"

iImage* imgPotion;
iImage* imgCoin;
iImage* imgPowerUp;

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
    case powerUp:
    {
        img = imgPowerUp->copy();
        break;
    }
    default:
    {
        img = imgPotion->copy();
        break;
    }
    }

    alive = false;
    value = 0.0f;
    type = it;

    sp = iPointZero;
    aniHeight = 50.0f;
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
        //pc->coin += value;
        pc->coin++;
        break;
    }
    case healing:
    {
        pc->hp += value;
        if (pc->hp > pc->_hp)
            pc->hp = pc->_hp;
        break;
    }
    case powerUp:
    {
        pc->attackDmg += value;
        break;
    }
	}
}

#define ANIDT 0.5f
bool UseItem::animation(float dt)
{
    if (aniDt == ANIDT)
        return false;

    aniDt += dt;
    if (aniDt > ANIDT)
    {
        sp = itemPos;
        aniDt = ANIDT;
    }

    float h = 0.0f - aniHeight * _sin(180 * aniDt / ANIDT);
    iPoint a = easeIn(aniDt / ANIDT, sp, itemPos);
    a += iPointMake(0, h);
    
    drawitemPos = a + pc->camPosition + setPos;
    img->paint(dt, drawitemPos, REVERSE_NONE);

    return true;
}

void UseItem::paint(float dt)
{
    if (animation(dt) || alive == false)
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
   iImage* imgpotion = new iImage();
   Texture* texpotion = createImage("assets/item/potionRed.png");
   imgpotion->addObject(texpotion);
   freeImage(texpotion);

   imgPotion = imgpotion;

   //----------------------------------------------------------------------------

   iImage* imgMoney = new iImage();
   Texture* texMoney = createImage("assets/item/coin.png");
   imgMoney->addObject(texMoney);
   freeImage(texMoney);

   imgCoin = imgMoney;

   //----------------------------------------------------------------------------

   iImage* imgPower = new iImage();
   Texture* texPower = createImage("assets/item/gemRed.png");
   imgPower->addObject(texPower);
   freeImage(texPower);

   imgPowerUp = imgPower;
}

void freeItemImg()
{
    delete imgPotion;
    delete imgCoin;
    delete imgPowerUp;
}

void golemItems(EnemyGolem* enm)
{
    for (int i = 0; i < 2; i++)
    {
        UseItem* ui = enm->items[random()%3];
        Texture* tex = ui->img->tex;

        ui->alive = true;
        ui->value =  i * 10;

		iPoint et = iPointMake(enm->img[0]->tex->width / 4.0f * enm->ratio, enm->img[0]->tex->height / 2.0f  * enm->ratio);
        iPoint p = iPointMake(-100 + 100 * i, 0);
        ui->sp = enm->golemPos + et;
        ui->itemPos = enm->golemPos + et + p;
        ui->drawitemPos = ui->itemPos + (pc->camPosition + setPos);

        ui->touchItem = iRectMake(ui->itemPos.x + tex->width * 0.25f, 
            ui->itemPos.y + tex->height * 0.25f,
            tex->width * 0.5f, tex->height * 0.5f);
    }
}