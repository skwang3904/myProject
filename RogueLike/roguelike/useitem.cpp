#include "useitem.h"

#include "Room.h"
#include "EnemyStruct.h"
#include "EnemyData.h"

iImage* imgPotion;
iImage* imgCoin;
iImage* imgPowerUp;
iImage* imgAtkSpeedUp;
iImage* imgMoveSpeedUp;

UseItem::UseItem(itemType it)
{
    type = it;
    iImage* imgs[itemTypeMax] = {
        imgCoin, imgPotion, imgPowerUp, imgAtkSpeedUp, imgMoveSpeedUp
    };
    img = imgs[it]->copy();

    alive = false;
    getItem = false;
    value = 0.0f;

    sp = iPointZero;
    dropAniDt = ANIDT;
    dropAniHeight = 50.0f;
    getAniDt = 0.0f;

    itemPos = iPointZero;
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

bool UseItem::dropAnimation(float dt)
{
    if (dropAniDt == ANIDT)
        return false;

    dropAniDt += dt;
    if (dropAniDt > ANIDT)
    {
        sp = itemPos;
        dropAniDt = ANIDT;
    }

    float h = 0.0f - dropAniHeight * _sin(180 * dropAniDt / ANIDT);
    iPoint a = easeIn(dropAniDt / ANIDT, sp, itemPos);
    a += iPointMake(0, h);
    
    iPoint drawitemPos = a + SET_DRAW_OFF;
    img->paint(dt, drawitemPos);

    return true;
}

void UseItem::getAnimation(float dt)
{
    if (getItem == false)
        return;

    float f = ANIDT;
    //float f = 10.0f;
    getAniDt += dt;
    if (getAniDt > f)
    {
        getAniDt = f;
        getItem = false;
    }

    sp = pc->playerPosition;
    iPoint p = linear(getAniDt / f, itemPos, sp);
    itemPos = p;

    if (iPointLength(sp - itemPos) < 3.0f)
    {
        getAniDt = f;
        getItem = false;
    }

    iPoint drawitemPos = p + SET_DRAW_OFF;
    img->paint(dt, drawitemPos);
}

void UseItem::paint(float dt)
{
    getAnimation(dt);
    if (dropAnimation(dt) || alive == false)
        return;

    iRect rt = touchItem;
    rt.origin += SET_DRAW_OFF;
    setRGBA(1, 0, 0, 0.5f);
    fillRect(rt);
    setRGBA(1, 1, 1, 1);

    iPoint drawPos = itemPos + SET_DRAW_OFF;
    img->paint(dt, drawPos);

    iPoint p = pc->playerPosition + HALF_OF_TEX_POINT;
    
    if (containPoint(p, touchItem))
    {
        alive = false;
        getItem = true;
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
        "assets/item/gemGreen.png",
        "assets/item/gemBlue.png"
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

void golemItems(MonsterData* enm)
{
    for (int i = 0; i < 3; i++)
    {
        UseItem* ui = enm->items[random() % 5];
        Texture* tex = ui->img->tex;

        ui->alive = true;
        ui->dropAniDt = 0.0f;
        ui->value = 10.0f;

		iPoint et = iPointMake(enm->img[0]->tex->width / 4 * enm->ratio,
			enm->img[0]->tex->height / 2  * enm->ratio);
        iPoint p = iPointMake(-100 + 50 * i, 0);
        ui->sp = enm->enemyPos + et;
        ui->itemPos = enm->enemyPos + et + p;

        ui->touchItem = iRectMake(ui->itemPos.x + tex->width / 4, 
            ui->itemPos.y + tex->height / 4,
            tex->width / 2, tex->height / 2);
    }
}