#include "Common.h"

#include "RgGame.h"





//---------------------------------------------------------------------
// Option

iPopup* popOption;
iImage* imgOptionBtnBar;
iImage** imgOptionBtn;

void drawBeforePopOption(iPopup* me, iPoint p, float dt);

void createPopOption()
{
	int i, j;

	imgOptionBtn = (iImage**)malloc(sizeof(iImage*) * 5);
	iPopup* pop = new iPopup(iPopupStyleZoom);
	iImage* img = new iImage();
	Texture* tex;

	iPoint size = iPointMake(devSize.width / 3.0f, devSize.height / 2.0f);

	tex = createTexture(size.x, size.y);
	fbo->bind(tex);
	fbo->setSize(size);

	Texture* texOp = createImage("assets/intro/OptionWindow.png");
	iPoint ratio = iPointMake(size.x / texOp->width, size.y / texOp->height);
	drawImage(texOp, 0, 0,
		0, 0, texOp->width, texOp->height,
		TOP | LEFT, ratio.x, ratio.y,
		2, 0);
	freeImage(texOp);

	Texture* texOpBar = createImage("assets/intro/Option_Controler_Bar.png");
	for (i = 0; i < 2; i++)
	{
		drawImage(texOpBar, size.x * 0.5f, size.y * 0.33f + size.y * 0.1f * i,
			0, 0, texOpBar->width, texOpBar->height,
			TOP | LEFT, 1.0f, 1.0f,
			2, 0);
	}
	freeImage(texOpBar);

	const char* strOption[2] = {
		"MUSIC","SOUNDS"
	};
	iGraphics* g = iGraphics::instance();
	g->init(iSizeMake(size.x, size.y));
	setStringName("assets/pixelfont.ttf");
	setStringRGBA(0, 0, 0, 1);
	setStringSize(20);
	setStringBorder(0);

	for (i = 0; i < 2; i++)
	{
		g->drawString(size.x * 0.3f - (size.x * 0.05f) * i,
			size.y * 0.33f + size.y * 0.1f * i,
			TOP | LEFT, strOption[i]);
	}

	setStringName("궁서체");
	setStringRGBA(1, 1, 1, 1);
	setStringSize(1);
	setStringBorder(0);

	Texture* texText = g->getTexture();
	drawImage(texText, 0, 0, TOP | LEFT);
	freeImage(texText);

	fbo->backSize();
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	img->reverse = REVERSE_HEIGHT;
	imgOptionBtnBar = img;
	pop->addObject(img);

	const char* strPath[5] = {
	"Option_Controler",
	"Option_Controler",
	"Option_Check Button",
	"Option_Check",
	"Option_X"
	};

	iSize sizeBtn[5] = {
		{size.y * 0.05f, size.x * 0.1f},
		{size.y * 0.05f, size.x * 0.1f},
		{size.y * 0.1f, size.y * 0.1f},
		{size.x * 0.2f, size.x * 0.2f},
		{size.x * 0.2f, size.x * 0.2f}
	};

	iPoint p[5] = {
		//{size.x * 0.5f, size.y * 0.3f},
		//{size.x * 0.5f, size.y * 0.4f},
		{420.0f, size.y * 0.3f},
		{420.0f, size.y * 0.4f},
		{size.x * 0.55f, size.y * 0.55f},
		{size.x * 0.25f, size.y * 0.75f},
		{size.x * 0.5f, size.y * 0.75f},
	};

	for (i = 0; i < 5; i++)
	{
		img = new iImage();

		if (i == 0 || i == 1)
		{
			tex = createTexture(sizeBtn[i].width, sizeBtn[i].height);
			fbo->bind(tex);
			fbo->setSize(sizeBtn[i].width, sizeBtn[i].height);

			for (j = 0; j < 2; j++)
			{
				texOp = createImage("assets/intro/%s.png", strPath[i]);
				drawImage(texOp, 0, 0,
					0, 0, texOp->width, texOp->height,
					TOP | LEFT, sizeBtn[i].width / texOp->width, sizeBtn[i].height / texOp->height * 0.9f,
					2, 0);
				freeImage(texOp);
			}

			fbo->backSize();
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		else
		{
			for (j = 0; j < 2; j++)
			{
				tex = createTexture(sizeBtn[i].width, sizeBtn[i].height);
				fbo->bind(tex);
				fbo->setSize(sizeBtn[i].width, sizeBtn[i].height);

				texOp = createImage("assets/intro/%s%d.png", strPath[i], j);
				drawImage(texOp, 0, 0,
					0, 0, texOp->width, texOp->height,
					TOP | LEFT, sizeBtn[i].width / texOp->width, sizeBtn[i].height / texOp->height,
					2, 0);
				freeImage(texOp);

				fbo->backSize();
				fbo->unbind();

				img->addObject(tex);
				freeImage(tex);
			}
		}

		img->position = p[i];
		img->reverse = REVERSE_HEIGHT;
		imgOptionBtn[i] = img;
		pop->addObject(img);
	}

	pop->openPosition = iPointMake(devSize.width / 20.0f,
		devSize.height * 0.6f);
	pop->closePosition = iPointMake((devSize.width - size.x) / 2.0f, (devSize.height - size.y) / 2.0f);

	pop->methodDrawBefore = drawBeforePopOption;
	popOption = pop;

}

void freePopOption()
{
	delete popOption;
	free(imgOptionBtn);
}

void showPopOption(bool show)
{
	popOption->show(show);
}

void drawBeforePopOption(iPopup* me, iPoint p, float dt)
{
	for (int i = 3; i < 5; i++)
	{
		imgOptionBtn[i]->setTexAtIndex(popOption->selected == i);
	}
}

void drawPopOption(float dt)
{
	popOption->paint(dt);
}

static iPoint prevPosition;
static bool click = false;
static int clickNum = 0;

bool optionBtnCheck = false;
bool keyPopOption(iKeyState stat, iPoint point)
{
	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popOption->selected;
		if (i == -1) break;

		if (i == 0 || i == 1)
		{
			click = true;
			prevPosition = point;
			clickNum = i;
		}

		if (i == 2)
		{
			imgOptionBtn[2]->setTexAtIndex(optionBtnCheck);
			optionBtnCheck = !optionBtnCheck;
		}

		if (i == 3)
		{
			showPopOption(false);
		}

		if (i == 4)
		{
			showPopOption(false);
		}
		break;
	}
	case iKeyStateMoved:
	{
		for (int i = 0; i < 5; i++)
		{
			if (containPoint(point, imgOptionBtn[i]->touchRect(popOption->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popOption->selected != j)
		{
			;
		}

		popOption->selected = j;

		if (click)
		{
			imgOptionBtn[clickNum]->position.x += point.x - prevPosition.x;
			prevPosition = point;

			if (imgOptionBtn[clickNum]->position.x < 310)
				imgOptionBtn[clickNum]->position.x = 310;
			else if (imgOptionBtn[clickNum]->position.x > 420)
				imgOptionBtn[clickNum]->position.x = 420;

			float bgm = (imgOptionBtn[0]->position.x - 310) / 110.0f;
			float sfx = (imgOptionBtn[1]->position.x - 310) / 110.0f;
			audioVolume(bgm, sfx, SOUND_SFX_NUM);
			PRINTF(bgm);
			PRINTF(sfx);

		}
		break;
	}
	case iKeyStateEnded:
	{
		click = false;
		break;
	}
	default:
		break;
	}
	return false;
}