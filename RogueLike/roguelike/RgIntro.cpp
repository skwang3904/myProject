#include "RgIntro.h"

#include "RgGame.h"
#include "RgMenu.h"
#include "RgProc.h"
#include "loading.h"

void showPopIntroPress(bool show);

void loadRgIntro()
{
	createPopIntroBG();
	createPopIntroButten();
	createPopIntroOption();

	showPopIntroBG(true);
	showPopIntroPress(true);
}

void freeRgIntro()
{
	freePopIntroBG();
	freePopIntroButten();
	freePopIntroOption();
}

void drawRgIntro(float dt)
{
	drawPopIntroBG(dt);
	drawPopIntroButten(dt);
	drawPopIntroOption(dt);
}

void keyRgIntro(iKeyState stat, iPoint point)
{
	if (keyPopIntroBG(stat, point) ||
		keyPopIntroButten(stat, point)||
		keyPopIntroOption(stat, point))
		return;
}

//-------------------------------------------------------------------------------------

iPopup* popIntroBG;
iPopup* popIntroPress;
iImage* imgPressStart;

static float pressAlpha = 0.0f;
void methodBeforePress(iPopup* me, iPoint p, float dt)
{
	setRGBA(1, 1, 1, (min(fabsf(pressAlpha - 1.5f), 1.0f)));
}
void methodAfterPress(iPopup* me, iPoint p, float dt)
{
	setRGBA(1, 1, 1, 1);
}

void createPopIntroBG()
{
	const char* strPath[2] = {
		"assets/intro/introBG2.jpg",
		"assets/intro/pressAnyKey.png"
	};

	iPoint p[2] = {
		{devSize.width, devSize.height},
		{513, 150}
	};

	for (int i = 0; i < 2; i++)
	{
		iPopup* pop = new iPopup(iPopupStyleAlpha);

		iImage* img = new iImage();
		Texture* tex = createTexture(p[i].x, p[i].y);
		fbo->bind(tex);
		fbo->setSize(p[i].x, p[i].y);

		Texture* texBg = createImage(strPath[i]);
		drawImage(texBg, 0, 0,
			0, 0, texBg->width, texBg->height,
			TOP | LEFT, p[i].x / texBg->width, p[i].y / texBg->height,
			2, 0);
		freeImage(texBg);

		fbo->backSize();
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);
		img->reverse = REVERSE_HEIGHT;

		pop->addObject(img);

		if (i == 0)
		{
			popIntroBG = pop;
		}
		else // if ( i == 1)
		{
			iPoint pos = iPointMake((devSize.width - img->tex->width) / 2.0f , 
				(devSize.height - img->tex->height) / 2.0f);	
			img->position = pos;

			pop->_showDt = 1.0f;
			pop->methodDrawBefore = methodBeforePress;
			pop->methodDrawAfter = methodAfterPress;
			
			imgPressStart = img;
			popIntroPress = pop;
		}
	}
}

void freePopIntroBG()
{
	delete popIntroBG;
	delete popIntroPress;
}

void showPopIntroBG(bool show)
{
	popIntroBG->show(show);
}

void showPopIntroPress(bool show)
{
	popIntroPress->show(show);
}

void drawPopIntroBG(float dt)
{
	popIntroBG->paint(dt);

	pressAlpha += dt;
	if (pressAlpha > 3.0f)
		pressAlpha = 0.0f;
	popIntroPress->paint(dt);
}

bool keyPopIntroBG(iKeyState stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStateBegan:
	{
		showPopIntroPress(false);
		showPopIntroButten(true);
		break;
	}
	case iKeyStateMoved:
		break;
	case iKeyStateEnded:
		break;
	default:
		break;
	}
	return false;
}

//-------------------------------------------------------------------------------------

iPopup* popIntroBtn;
iImage** imgIntroBtn;

void drawBeforeIntroButton(iPopup* me, iPoint p, float dt);

void createPopIntroButten()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	popIntroBtn = pop;

	imgIntroBtn = (iImage**)malloc(sizeof(iImage*) * 3);
	iPoint size = iPointMake(devSize.width / 10.0f, devSize.height / 10.0f);
	iImage* img;
	Texture* tex, *texText;

	const char* strBtn[3] = {
		"게임시작", "   옵션", " 끝내기"
	};


	for (int i = 0; i < 3; i++)
	{
		img = new iImage();
		
		for (int j = 0; j < 2; j++)
		{
			tex = createTexture(size.x, size.y);
			fbo->bind(tex);
			fbo->setSize(size.x, size.y);

			Texture* texBt = createImage("assets/intro/ButtonBrown%d.png", j);
			drawImage(texBt, 0, 0,
				0, 0, texBt->width, texBt->height,
				TOP | LEFT, size.x / texBt->width, size.y / texBt->height,
				2, 0);
			freeImage(texBt);

			{
				iGraphics* g = iGraphics::instance();
				g->init(size.x, size.y);
				setStringName("assets/BMJUA_ttf.ttf");
				setStringRGBA(0, 0, 0, 1);
				setStringSize(30);
				setStringBorder(0);
				g->drawString(0, 0, TOP | LEFT, strBtn[i]);
				setStringName("궁서체");
				setStringRGBA(1, 1, 1, 1);
				setStringSize(1);
				setStringBorder(0);
				texText = g->getTexture();
				drawImage(texText, size.x / 4.0f, size.y / 3.0f, TOP | LEFT);
				freeImage(texText);
			}

			fbo->backSize();
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		};

		img->reverse = REVERSE_HEIGHT;
		img->position = iPointMake(img->tex->width / 2.0f, 
			devSize.height / 2.0f + img->tex->height * 1.3 * i);

		imgIntroBtn[i] = img;
		pop->addObject(img);
	}

	pop->methodDrawBefore = drawBeforeIntroButton;


}

void freePopIntroButten()
{
	delete popIntroBtn;
	free(imgIntroBtn);
}

void showPopIntroButten(bool show)
{
	popIntroBtn->show(show);
}

void drawBeforeIntroButton(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 3; i++)
		imgIntroBtn[i]->setTexAtIndex(popIntroBtn->selected == i);
}

void drawPopIntroButten(float dt)
{
	popIntroBtn->paint(dt);
}

extern bool runWnd;
bool keyPopIntroButten(iKeyState stat, iPoint point)
{
	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popIntroBtn->selected;
		if (i == -1) break;

		if (i == 0)
		{
			//게임시작
			setLoading(gs_menu, freeRgIntro, loadRgMenu);
		}
		else if (i == 1)
		{
			// 옵션
			showPopIntroOption(true);
		}
		else if (i == 2)
		{
			// 게임종료
			runWnd = false;
		}
		break;
	}
	case iKeyStateMoved:
	{
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(point, imgIntroBtn[i]->touchRect(popIntroBtn->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popIntroBtn->selected != j)
		{
			;
		}

		popIntroBtn->selected = j;
		break;
	}
	case iKeyStateEnded:

		break;
	default:
		break;
	}
	return false;
}

//-------------------------------------------------------------------------------------

iPopup* popIntroOption;
iImage* imgOptionBtnBar;
iImage** imgOptionBtn;

void drawBeforePopIntroOption(iPopup* me, iPoint p, float dt);

void createPopIntroOption()
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
	
	for(i=0; i<2;i++)
		g->drawString(size.x * 0.3f - (size.x * 0.05f) * i, size.y * 0.33f + size.y * 0.1f * i, TOP|LEFT, strOption[i]);

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

	const char* strPath[4] = {
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
		{size.x * 0.5f, size.y * 0.3f},
		{size.x * 0.5f, size.y * 0.4f},
		{size.x	* 0.55f, size.y * 0.55f},
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
				texOp = createImage("assets/intro/%s.png", strPath[0]);
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
		else //if (i == 1)
		{
			for (j = 0; j < 2; j++)
			{
				tex = createTexture(sizeBtn[i].width, sizeBtn[i].height);
				fbo->bind(tex);
				fbo->setSize(sizeBtn[i].width, sizeBtn[i].height);

				texOp = createImage("assets/intro/%s%d.png", strPath[i-1], j);
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

	pop->methodDrawBefore = drawBeforePopIntroOption;
	popIntroOption = pop;
}

void freePopIntroOption()
{
	delete popIntroOption;
}

void showPopIntroOption(bool show)
{
	popIntroOption->show(show);
}

void drawBeforePopIntroOption(iPopup* me, iPoint p, float dt)
{
	for (int i = 3; i < 5; i++)
	{
		imgOptionBtn[i]->setTexAtIndex(popIntroOption->selected == i);
	}
}

void drawPopIntroOption(float dt)
{
	popIntroOption->paint(dt);
}

static iPoint prevPosition;
static bool click = false;
static int clickNum = 0;

bool optionBtnCheck = false;
bool keyPopIntroOption(iKeyState stat, iPoint point)
{
	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popIntroOption->selected;
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
			showPopIntroOption(false);
		}

		if (i == 4)
		{
			showPopIntroOption(false);
		}
		break;
	}
	case iKeyStateMoved:
	{
		for (int i = 0; i < 5; i++)
		{
			if (containPoint(point, imgOptionBtn[i]->touchRect(popIntroOption->closePosition)))
			{				
				j = i;
				break;
			}
		}

		if (popIntroOption->selected != j)
		{
			;
		}

		popIntroOption->selected = j;

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