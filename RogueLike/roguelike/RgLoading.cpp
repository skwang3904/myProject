#include "RgLoading.h"

#include "Stage.h"

iPopup* popNextStage;

void loadRgLoading()
{
	createPopPlayerDead();
	createPopNextStage();

}

void freeRgLoading()
{
	freePopPlayerDead();
	freePopNextStage();
}

void showRgLoading(bool show, loadingType type)
{
	switch (type)
	{
	case PlayerDead:
	{
		showPopPlayerDead(show);
		break;

	}
	case NextStage:
	{
		showPopNextStage(show);
		break;
	}

	default:
		break;
	}
}

bool bShowRgLoading(loadingType type)
{
	switch (type)
	{
	case PlayerDead:
	{	
		break;
	}
	case NextStage:
	{
		if (popNextStage->bShow)
			return true;
		else
			return false;
	}

	default:
		break;
	}
	return false;
}

void drawRgLoading(float dt, loadingType type)
{
	switch (type)
	{
	case PlayerDead:
	{
		drawPopPlayerDead(dt);
		break;

	}
	case NextStage:
	{
		drawPopNextStage(dt);
		break;
	}

	default:
		break;
	}
}

void keyRgLoading(iKeyState stat, iPoint point)
{
	if (keyPopPlayerDead(stat, point) ||
		keyPopNextStage(stat, point))
		return;
}

//-------------------------------------------------------------------

void createPopPlayerDead()
{
}

void freePopPlayerDead()
{
}

void showPopPlayerDead(bool show)
{
}

void drawPopPlayerDead(float dt)
{
}

bool keyPopPlayerDead(iKeyState stat, iPoint point)
{
	return false;
}

//-------------------------------------------------------------------

iImage* imgLoadingBar;
float nextStageLoadingTime = 0.0f;
void createPopNextStage()
{
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	popNextStage = pop;

	iImage* imgLoadingBarBG = new iImage();
	iImage* imgLoadingBarFill = new iImage();
	imgLoadingBar = imgLoadingBarFill;

	Texture* texLoadingBarBG = createImage("assets/loading/LoadingBar_BG.png");
	Texture* texLoadingBarFill = createImage("assets/loading/LoadingBar_Fill.png");

	imgLoadingBarBG->addObject(texLoadingBarBG);
	imgLoadingBarFill->addObject(texLoadingBarFill);

	freeImage(texLoadingBarBG);
	freeImage(texLoadingBarFill);

	imgLoadingBarBG->position = iPointMake(100,100);
	imgLoadingBarFill->position = imgLoadingBarBG->position 
		+ iPointMake((imgLoadingBarBG->tex->width - imgLoadingBarFill->tex->width) / 2, 170);
	
	pop->addObject(imgLoadingBarBG);
	pop->addObject(imgLoadingBarFill);
	pop->openPosition = iPointMake(200,50);
	pop->closePosition = pop->openPosition;
	pop->_showDt = 0.5f;
}

void freePopNextStage()
{
	delete popNextStage;
}

void showPopNextStage(bool show)
{
	nextStageLoadingTime = 0.0f;
	popNextStage->show(show);
}

void drawPopNextStage(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);

	nextStageLoadingTime += dt;
	float f = nextStageLoadingTime / _nextStageloadingTime;
	if (f < 0.3f) f = 0.3f;
	else if (f < 0.5f) f = 0.6f;
	else if (f < 1.2f) f = 0.8f;
	else  f = 1.0f;

	imgLoadingBar->imgRatioX = f;

	popNextStage->paint(dt);

	if (nextStageLoadingTime > _nextStageloadingTime + 2.0f)
		popNextStage->show(false);
}

bool keyPopNextStage(iKeyState stat, iPoint point)
{
	if(popNextStage->bShow == false)
		return false;

	if (popNextStage->stat != iPopupStatProc)
		return true;
}

//-------------------------------------------------------------------
