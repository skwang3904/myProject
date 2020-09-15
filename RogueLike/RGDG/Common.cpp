#include "Common.h"

//----------------------------------------------------------------------------

Object::Object(int index, int8 mapNum, iPoint pos)
{
	this->index = index;
	alive = false;

	imgNum = 0;
	imgs = NULL;
	img = NULL;
	this->mapNumber = mapNum;

	this->position = pos;
	vector = iPointZero;
	touchRect = iRectZero;

	prevHp = 0.0f;
	hp = _hp = 0.0f;
	attackPoint = _attackPoint = 0.0f;
	attackSpeed = _attackSpeed = 0.0f;
	actionDt = _actionDt = 0.0f;

	moveSpeed = 0.0f;
}

Object::~Object()
{
	for (int i = 0; i < imgNum; i++)
		delete imgs[i];
	free(imgs);
}

//----------------------------------------------------------------------------
// Pop Option
iPopup* popOption;
iImage** imgOption;
void createPopOption()
{
	int i, j;
	iImage* img;
	Texture* tex, *t;
	iPopup* pop = new iPopup(iPopupStyleMove);
	iSize size;

	// bgm, sfx, 적용, 닫기
	imgOption = (iImage**)malloc(sizeof(iImage*) * 3);

	size = iSizeMake(devSize.width * 0.5f, devSize.height * 0.7f);
	for (i = 0; i < 3; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.width, size.height);
			fbo->bind(tex);

			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		imgOption[i] = img;
	}

	pop->openPosition;
	pop->closePosition;
	pop->methodDrawBefore;
	pop->_showDt;
	popOption = pop;
}

void freePopOption()
{
	delete popOption;
}

void showPopOption(bool show)
{
	popOption->show(show);
}

void drawPopOption(float dt)
{
	popOption->paint(dt);
}

bool keyPopOption(iKeyState stat, iPoint point)
{
	if (popOption->bShow == false)
		return false;

	if (popOption->stat != iPopupStatProc)
		return false;

	return true;
}
