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
#define OPTION_BUTTON_NUM 4
iPopup* popOption;
iImage** imgOption;

void drawPopOptionBefore(iPopup* me, iPoint p, float dt);

void createPopOption()
{
	int i, j;
	iImage* img;
	Texture* tex, *t;
	iPopup* pop = new iPopup(iPopupStyleMove);
	iSize size;

	// 적용, 닫기, bgm, sfx
	imgOption = (iImage**)malloc(sizeof(iImage*) * OPTION_BUTTON_NUM);

	img = new iImage();
	iSize frameSize = iSizeMake(devSize.width * 0.5f, devSize.height * 0.7f);
	tex = createTexture(frameSize.width, frameSize.height);

	fbo->bind(tex);
	//t = createImage();
	fbo->clear(0.6f, 0.4f, 0, 1);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	size = iSizeMake(devSize.width * 0.1f, devSize.width * 0.1f);
	for (i = 0; i < OPTION_BUTTON_NUM; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.width, size.height);

			fbo->bind(tex);
			//if (j == 0) setRGBA(1, 1, 1, 1);
			//else setRGBA(0, 1, 0, 1);			
			if (j == 0) fbo->clear(1, 1, 1, 1);
			else fbo->clear(0, 1, 0, 1);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		if (i < 2) // button
			img->position = iPointMake(devSize.width * 0.1f + size.width * 2 * i, devSize.height * 0.55f);
		else	//bgm, sfx
			img->position = iPointMake(devSize.width * 0.2f, devSize.height * 0.2f + size.height * 1.2 * (i - 2));

		imgOption[i] = img;
		pop->addObject(img);
	}

	pop->openPosition = iPointMake((devSize.width - frameSize.width) / 2.0f, -frameSize.height);
	pop->closePosition = iPointMake((devSize.width - frameSize.width) / 2.0f, (devSize.height - frameSize.height) / 2.0f);;
	pop->methodDrawBefore;
	pop->_showDt = 0.5f;
	popOption = pop;
}

void freePopOption()
{
	delete popOption;
	free(imgOption);
}

void showPopOption(bool show)
{
	popOption->show(show);
}

void drawPopOptionBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < OPTION_BUTTON_NUM; i++)
		imgOption[i]->setTexAtIndex(i == popOption->selected);
}

void drawPopOption(float dt)
{
	popOption->paint(dt);
}


typedef void (*Method_Option)(iPoint point);
Method_Option methodOption = NULL;
iPoint prevOptionPoint = iPointZero;

void optionBgm(iPoint point)
{
	imgOption[2]->position.x += point.x - prevOptionPoint.x;
	prevOptionPoint = point;
}

void optionSfx(iPoint point)
{
	imgOption[3]->position.x += point.x - prevOptionPoint.x;
	prevOptionPoint = point;
}

void setOptionSound()
{
	//float bgm = imgOption[2]->position;
	//float sfx = imgOption[3]->position;

	//audioVolume(bgm, sfx, SFX_NUM);
}

bool keyPopOption(iKeyState stat, iPoint point)
{
	if (popOption->bShow == false)
		return false;

	if (popOption->stat != iPopupStatProc)
		return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popOption->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			// apply
			//setOptionSound();
			showPopOption(false);
		}
		else if (i == 1)
		{
			// close
			showPopOption(false);
		}
		else if (i == 2)
		{
			// bgm
			methodOption = optionBgm;
			prevOptionPoint = point;
		}
		else if (i == 3)
		{
			// sfx
			methodOption = optionSfx;
			prevOptionPoint = point;
		}

		popOption->selected = -1;
		break;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < OPTION_BUTTON_NUM; i++)
		{
			if (containPoint(point, imgOption[i]->touchRect(popOption->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popOption->selected != j)
			;

		popOption->selected = j;

		if (methodOption)
			methodOption(point);

		break;
	}
	case iKeyStateEnded:
	{
		methodOption = NULL;
		break;
	}
	}

	return true;

}
