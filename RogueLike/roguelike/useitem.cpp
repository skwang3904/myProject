#include "useitem.h"

#include "Room.h"
#include "EnemyStruct.h"

iImage* imgPotion;
iImage* imgCoin;
iImage* imgPowerUp;
iImage* imgAtkSpeedUp;
iImage* imgMoveSpeedUp;

#define ANIDT 2.0f
UseItem::UseItem(itemType it)
{
    type = it;
    iImage* imgs[itemTypeMax] = {
        imgCoin, imgPotion, imgPowerUp, imgAtkSpeedUp, imgMoveSpeedUp
    };
    img = imgs[it]->copy();

    alive = false;
    value = 0.0f;

    sp = iPointZero;
    aniHeight = 50.0f;
    aniDt = ANIDT;

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
    case atkSpeedUp:
    {
        pc->attackSpeed -= pc->_attackSpeed / value;
        break;
    }
    case moveSpeedUp:
    {
        pc->moveSpeed += value;
        break;
    }
	}
}

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
    
    drawitemPos = a + SET_DRAW_OFF;
    img->paint(dt, drawitemPos);

    return true;
}

void UseItem::paint(float dt)
{
    if (animation(dt) || alive == false)
        return;

    iRect rt = touchItem;
    rt.origin += SET_DRAW_OFF;
    setRGBA(1, 0, 0, 0.5f);
    fillRect(rt);
    setRGBA(1, 1, 1, 1);

    img->paint(dt, drawitemPos);

    drawitemPos = itemPos + SET_DRAW_OFF;

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
    const char* strPath[5] = {
        "assets/item/potionRed.png",
        "assets/item/coin.png",
        "assets/item/gemRed.png",
        "assets/item/gemRed.png",
        "assets/item/gemRed.png"
    };
    iImage* imgs[5];
    for (int i = 0; i < 5; i++)
    {
        iImage* img = new iImage();
        Texture* tex = createImage(strPath[i]);
        img->addObject(tex);
        freeImage(tex);
        imgs[i] = img;
    }
    imgPotion = imgs[0];
    imgCoin = imgs[1];
    imgPowerUp = imgs[2];
    imgAtkSpeedUp = imgs[3];
    imgMoveSpeedUp = imgs[4];
}

void freeItemImg()
{
    delete imgPotion;
    delete imgCoin;
    delete imgPowerUp;
    delete imgAtkSpeedUp;
    delete imgMoveSpeedUp;
}

//----------------------------------------------------------------------------
// golem drops

void golemItems(EnemyGolem* enm)
{
    for (int i = 0; i < 3; i++)
    {
        UseItem* ui = enm->items[random() % 5];
        Texture* tex = ui->img->tex;

        ui->alive = true;
        ui->aniDt = 0.0f;
        ui->value = 10.0f;

		iPoint et = iPointMake(enm->img[0]->tex->width / 4 * enm->ratio,
			enm->img[0]->tex->height / 2  * enm->ratio);
        iPoint p = iPointMake(-100 + 50 * i, 0);
        ui->sp = enm->golemPos + et;
        ui->itemPos = enm->golemPos + et + p;
        ui->drawitemPos = ui->itemPos + SET_DRAW_OFF;

        ui->touchItem = iRectMake(ui->itemPos.x + tex->width / 4, 
            ui->itemPos.y + tex->height / 4,
            tex->width / 2, tex->height / 2);
    }
}