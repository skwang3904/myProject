#include "Common.h"

#include "AppRogueLike.h"

//----------------------------------------------------------------------------
Object** objects = NULL;
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
#define OPTION_BUTTON_NUM 5
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

	size = iSizeMake(frameSize.width, frameSize.height);
	iGraphics* g = iGraphics::instance();
	g->init(size);
	setStringSize(frameSize.height * 0.075f);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	g->drawString(size.width / 2.0f, size.height * 0.075f, VCENTER | HCENTER, "Setting");
	g->drawString(size.width / 2.0f, size.height * 0.175f, VCENTER | HCENTER, "BGM");
	g->drawString(size.width / 2.0f, size.height * 0.375f, VCENTER | HCENTER, "SFX");
	g->drawString(size.width / 2.0f, size.height * 0.625f, VCENTER | RIGHT, "FullScreen");
	text = g->getTexture();

	size = frameSize;
	tex = createTexture(size.width, size.height);
	fbo->bind(tex);
	t = createImage("assets/setting/Setting panel.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	drawImage(text, size.width / 2.0f, size.height - text->height * 0.5f,
		0, 0, text->width, text->height,
		VCENTER | HCENTER, 1.0f, 1.0f,
		2, 0, REVERSE_HEIGHT);
	freeImage(text);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);


	const char* strBtn[OPTION_BUTTON_NUM] = {
		"assets/setting/Accept%d.png",
		"assets/setting/Close%d.png",
		"BGM img",
		"SFX img",
		"fullscreenBtn img",
	};

	iSize sizeBtn[OPTION_BUTTON_NUM] = {
		{frameSize.width * 0.197860f, frameSize.height * 0.08064f},
		{frameSize.width * 0.101604f, frameSize.height * 0.08064f},
		{devSize.width * 0.02f, devSize.width * 0.05f},
		{devSize.width * 0.02f, devSize.width * 0.05f},
		{devSize.width * 0.1f, devSize.width * 0.05f},
	};

	for (i = 0; i < OPTION_BUTTON_NUM; i++)
	{
		img = new iImage();
		size = sizeBtn[i];
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.width, size.height);
			fbo->bind(tex);
			if (i < 2)
			{
				t = createImage(strBtn[i], j);
				drawImage(t, 0, size.height,
					0, 0, t->width, t->height,
					BOTTOM | LEFT, size.width / t->width, size.height/ t->height,
					2, 0, REVERSE_HEIGHT);
				freeImage(t);
			}
			else if(i < 4)
			{
				// bgm:sfx bar
				if (j == 0) fbo->clear(1, 0, 1, 1);
				else		fbo->clear(0, 1, 0, 1);
			}
			else
			{
				if (j == 0)
				{
					setRGBA(1, 1, 1, 0.2f);
					t = createImage(strBtn[0], 1);
					drawImage(t, 0, 0,
						0, 0, t->width, t->height,
						TOP | LEFT, size.width * 0.5f / t->width, size.height / t->height,
						2, 0, REVERSE_HEIGHT);
					freeImage(t);

					setRGBA(1, 1, 1, 1);
					t = createImage(strBtn[1], 0);
					drawImage(t, size.width, 0,
						0, 0, t->width, t->height,
						TOP | RIGHT, size.width * 0.5f / t->width, size.height / t->height,
						2, 0, REVERSE_HEIGHT);
					freeImage(t);
					setRGBA(1, 1, 1, 1);
				}
				else
				{
					setRGBA(1, 1, 1, 1);
					t = createImage(strBtn[0], 1);
					drawImage(t, 0, 0,
						0, 0, t->width, t->height,
						TOP | LEFT, size.width * 0.5f / t->width, size.height / t->height,
						2, 0, REVERSE_HEIGHT);
					freeImage(t);

					setRGBA(1, 1, 1, 0.2f);
					t = createImage(strBtn[1], 0);
					drawImage(t, size.width, 0,
						0, 0, t->width, t->height,
						TOP | RIGHT, size.width * 0.5f / t->width, size.height / t->height,
						2, 0, REVERSE_HEIGHT);
					freeImage(t);
					setRGBA(1, 1, 1, 1);
				}
			}
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		iPoint p = iPointZero;
		// button
		if (i == 0)			p = iPointMake(frameSize.width * 0.406421f, frameSize.height - frameSize.height * 0.096774f);
		else if (i == 1)	p = iPointMake(frameSize.width * 0.839572f, frameSize.height * 0.033f);
		//bgm, sfx
		else if (i == 2)	p = iPointMake(50.0f, frameSize.height * 0.5f * 0.43f);
		else if (i == 3)	p = iPointMake(50.0f, frameSize.height * 0.5f * 0.83f);
		// fullscreen
		else if (i == 4)	p = iPointMake(frameSize.width * 0.6f, frameSize.height * 0.5f * 1.2f);

		img->position = p;
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
	imgOption[2]->position.x = 50.0f + bgm * (570.0f - 50.0f);
	imgOption[3]->position.x = 50.0f + sfx * (570.0f - 50.0f);
}

void drawPopOptionBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < OPTION_BUTTON_NUM; i++)
	{
		if (i == 4) continue;
		imgOption[i]->setTexAtIndex(i == popOption->selected);
	}
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
	if (imgOption[2]->position.x < 50.0f)
		imgOption[2]->position.x = 50.0f;
	else if (imgOption[2]->position.x > 570.0f)
			 imgOption[2]->position.x = 570.0f;
	prevOptionPoint = point;
}

void optionSfx(iPoint point)
{
	imgOption[3]->position.x += point.x - prevOptionPoint.x;
	if (imgOption[3]->position.x < 50.0f)
		imgOption[3]->position.x = 50.0f;
	else if (imgOption[3]->position.x > 570.0f)
			 imgOption[3]->position.x = 570.0f;
	prevOptionPoint = point;
}

void setOptionSound()
{
	float bgm = (imgOption[2]->position.x - 50.0f) / 570.0f;
	float sfx = (imgOption[3]->position.x - 50.0f) / 570.0f;

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
			//popOption->selected = -1;
			//showPopOption(false);
		}
		else if (i == 1)
		{
			// close
			showPopOption(false);
			popOption->selected = -1;
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
		else if (i == 4)
		{
			goFullscreen();
			imgOption[4]->setTexAtIndex(isFullscreen);
		}

		audioPlay(AUDIO_MenuSelected);
		break;
	}
	case iKeyStateMoved:
	{
		if (methodOption == NULL)
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
		}

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
