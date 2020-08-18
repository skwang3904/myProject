#include "RgIntro.h"

#include "Common.h"

#include "RgGame.h"
#include "RgMenu.h"
#include "RgProc.h"
#include "loading.h"

void showPopIntroPress(bool show);

void loadRgIntro()
{
	createPopIntroBG();
	createPopIntroButten();

	showPopIntroBG(true);
	showPopIntroPress(true);
}

void freeRgIntro()
{
	freePopIntroBG();
	freePopIntroButten();
}

void drawRgIntro(float dt)
{
	drawPopIntroBG(dt);
	drawPopIntroButten(dt);
}

void keyRgIntro(iKeyState stat, iPoint point)
{
	if (keyPopIntroBG(stat, point) ||
		keyPopIntroButten(stat, point))
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
#if 1
			setLoading(gs_menu, freeRgIntro, loadRgMenu);
#else
			setLoading(gs_proc, freeRgIntro, loadRgProc);
#endif
		}
		else if (i == 1)
		{
			// 옵션
			showPopOption(true);
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
