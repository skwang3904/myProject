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
	touchSize = iSizeZero;
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

void drawPopOptionClose(iPopup* me);
void drawPopOptionBefore(iPopup* me, iPoint p, float dt);

void createPopOption()
{
	int i, j;
	iImage* img;
	Texture* tex, *t, *text;
	iPopup* pop = new iPopup(iPopupStyleMove);
	iSize size;

	// 적용, 닫기, bgm, sfx
	imgOption = (iImage**)malloc(sizeof(iImage*) * OPTION_BUTTON_NUM);

	img = new iImage();
	iSize frameSize = iSizeMake(devSize.width * 0.5f, devSize.height * 0.7f);

	size = iSizeMake(frameSize.width * 0.6f, frameSize.height * 0.2f);
	iGraphics* g = iGraphics::instance();
	g->init(size);
	setStringSize(size.height * 0.5f);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, "Option");
	text = g->getTexture();

	size = frameSize;
	tex = createTexture(size.width, size.height);
	fbo->bind(tex);
	fbo->clear(0.5, 0.2, 0.1, 1);
	setRGBA(0, 1, 0, 1);
	fillRect(devSize.width * 0.2f, devSize.height * 0.6f, devSize.width * 0.6f, 10);
	fillRect(devSize.width * 0.2f, devSize.height * 0.4f, devSize.width * 0.6f, 10);
	setRGBA(1, 1, 1, 1);
	drawImage(text, size.width / 2.0f, devSize.height - text->height,
		0, 0, text->width, text->height,
		VCENTER | HCENTER, 1.0f, 1.0f,
		2, 0, REVERSE_HEIGHT);
	freeImage(text);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	const char* strBtn[4] = {
		"Apply",
		"Close",
		"BGM",
		"SFX",
	};

	size = iSizeMake(devSize.width * 0.1f, devSize.width * 0.1f);
	setStringSize(size.height * 0.3f);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	for (i = 0; i < OPTION_BUTTON_NUM; i++) // apply, close, bgm, sfx
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.width, size.height);

			g->init(size);
			g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, strBtn[i]);
			text = g->getTexture();

			fbo->bind(tex);
			if (j == 0) fbo->clear(1, 0, 1, 1);
			else fbo->clear(0, 1, 0, 1);
			DRAWIMAGE(text, size);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		if (i < 2) // button
			img->position = iPointMake(devSize.width * 0.1f + size.width * 2 * i, devSize.height * 0.55f);
		else	//bgm, sfx
			img->position = iPointMake(400, devSize.height * 0.2f + size.height * 1.2 * (i - 2));

		imgOption[i] = img;
		pop->addObject(img);
	}

	pop->openPosition = iPointMake((devSize.width - frameSize.width) / 2.0f, -frameSize.height);
	pop->closePosition = iPointMake((devSize.width - frameSize.width) / 2.0f, (devSize.height - frameSize.height) / 2.0f);;
	pop->methodClose = drawPopOptionClose;
	pop->methodDrawBefore = drawPopOptionBefore;
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

void drawPopOptionClose(iPopup* me)
{
	float bgm, sfx;
	audioGetVolume(bgm, sfx);
	imgOption[2]->position.x = 100.0f + bgm * 300.0f;
	imgOption[3]->position.x = 100.0f + sfx * 300.0f;
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
	if (imgOption[2]->position.x < 100.0f)
		imgOption[2]->position.x = 100.0f;
	else if (imgOption[2]->position.x > 400.0f)
			 imgOption[2]->position.x = 400.0f;
	prevOptionPoint = point;
}

void optionSfx(iPoint point)
{
	imgOption[3]->position.x += point.x - prevOptionPoint.x;
	if (imgOption[3]->position.x < 100.0f)
		imgOption[3]->position.x = 100.0f;
	else if (imgOption[3]->position.x > 400.0f)
			 imgOption[3]->position.x = 400.0f;
	prevOptionPoint = point;
}

void setOptionSound()
{
	float bgm = (imgOption[2]->position.x - 100.0f) / 300.0f;
	float sfx = (imgOption[3]->position.x - 100.0f) / 300.0f;

	audioVolume(bgm, sfx, SFX_NUM);
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
			setOptionSound();
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
		audioPlay(AUDIO_MenuSelected);
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

		if (popOption->selected != j && j != -1)
			audioPlay(AUDIO_MenuMouseOver);

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
