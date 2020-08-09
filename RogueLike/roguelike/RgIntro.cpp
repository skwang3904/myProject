#include "RgIntro.h"

#include "RgProc.h"
#include "RgGame.h"
#include "loading.h"


void loadRgIntro()
{
	createPopIntroBG();
	createPopIntroButten();

	showPopIntroBG(true);

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

static bool click = false;

void createPopIntroBG()
{
	iPopup* popBg = new iPopup(iPopupStyleAlpha);
	popIntroBG = popBg;

	iImage* imgBg = new iImage();
	Texture* texBg = createImage("assets/intro/introBG2.jpg");
	imgBg->addObject(texBg);
	freeImage(texBg);

	imgBg->ratio = 1.5f;
	popBg->addObject(imgBg);


	iPopup* popPress = new iPopup(iPopupStyleAlpha);
	popIntroPress = popPress;
	iImage* imgPress = new iImage();
	imgPressStart = imgPress;
	Texture* texPress = createImage("assets/intro/pressAnyKey.png");
	imgPress->addObject(texPress);
	freeImage(texPress);

	imgPress->position = iPointMake(devSize.width / 2, devSize.height / 2);
	popPress->addObject(imgPress);

	popPress->_showDt = 1.0f;
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


	if (click == false)
	{
		if (popIntroPress->stat == iPopupStatProc)
			showPopIntroPress(false);
		else if (popIntroPress->bShow == false)
			showPopIntroPress(true);
		popIntroPress->paint(dt);
	}
}

bool keyPopIntroBG(iKeyState stat, iPoint point)
{
	switch (stat)
	{
	case iKeyStateBegan:
	{
		click = true;
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
void createPopIntroButten()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	popIntroBtn = pop;

	imgIntroBtn = (iImage**)malloc(sizeof(iImage*) * 3);
	Texture* tex;
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			tex = createImage("assets/intro/ButtonBrown%d.png", j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->ratio = 2.0f;
		img->position = iPointMake(50, 500 + 200 * i);
		imgIntroBtn[i] = img;

		pop->addObject(img);
	}

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

void drawPopIntroButten(float dt)
{
	popIntroBtn->paint(dt);
}

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
			setLoading(gs_proc, freeRgIntro, loadRgProc);
		}
		else if (i == 1)
		{
			// 옵션
		}
		else if (i == 2)
		{
			// 게임종료
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
iImage* imgOptionBtn;

void createPopIntroOption()
{
}

void freePopIntroOption()
{
}

void showPopIntroOption(bool show)
{
}

void drawPopIntroOption(float dt)
{
}

bool keyPopIntroOption(iKeyState stat, iPoint point)
{
	return false;
}