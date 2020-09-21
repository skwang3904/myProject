#include "Loading.h"

int8 gamestat, _gamestat;
METHOD_LOADING methodLoadingFree;
METHOD_LOADING methodLoadingLoad;

#define _loadingDt 1.0f
float loadingDt = _loadingDt;

float popGameLoadingDt, _popGameLoadingDt;

void setLoading(int8 newGameState, METHOD_LOADING free, METHOD_LOADING load)
{
	_gamestat = newGameState;
	methodLoadingFree = free;
	methodLoadingLoad = load;

	loadingDt = 0.0f;
}

void loadGameLoading()
{
	createPopGameLoading();
	popGameLoadingDt = _popGameLoadingDt = 1.0f;
}

void freeGameLoading()
{
	freePopGameLoading();
}

void drawLoading(float dt)
{
	if (loadingDt == _loadingDt)
		return;

	float alpha = 0.0f;

	float halfDt = _loadingDt / 2.0f;
	if (loadingDt < halfDt)
	{
		loadingDt += dt;
		if (loadingDt > halfDt)
		{
			loadingDt = halfDt;
			methodLoadingFree();
			methodLoadingLoad();
			gamestat = _gamestat;
		}

		alpha = loadingDt / halfDt;
	}
	else if (loadingDt == halfDt)
	{
		if (popGameLoadingDt == _popGameLoadingDt)
		{
			dt = 0.0f;
			popGameLoadingDt = 0.0f;
			showPopGameLoading(true);
		}
		else
		{
			if (popGameLoading->stat == iPopupStatProc)
			{
				popGameLoadingDt += dt;
				if (popGameLoadingDt > _popGameLoadingDt)
				{
					popGameLoadingDt = _popGameLoadingDt;

					loadingDt += 0.000001f;
					showPopGameLoading(false);
				}
			}
		}

		alpha = 1.0f;
	}
	else
	{
		if (popGameLoading->bShow == false)
		{
			loadingDt += dt;
			if (loadingDt > _loadingDt)
			{
				loadingDt = _loadingDt;
			}
		}
		alpha = 1.0f - (loadingDt - halfDt) / halfDt;
	}

	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
	drawPopGameLoading(dt);
	setRGBA(1, 1, 1, 1);
}

bool nowLoading()
{
	if (loadingDt < _loadingDt)
		return true;
	return false;
}

bool keyLoading(iKeyState stat, iPoint point)
{
	if( loadingDt < _loadingDt )
		return true;
	return false;
}

//------------------------------------------------------------------------------------------
iPopup* popGameLoading;
iImage* imgGameLoading;

void drawPopGameLoadingBefore(iPopup* me, iPoint p, float dt);

void createPopGameLoading()
{
	int i, j;
	iImage* img;
	Texture* tex, * t;
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	iSize size;

	const char* strPath[2] = {
	"assets/loading/LoadingBar_BG.png",
	"assets/loading/LoadingBar_Fill.png",
	};

	iSize sizes[2] = {
		{devSize.width / 2.0f, devSize.height / 4.0f},
		{devSize.width / 2.0f * 0.95f, devSize.height / 4.0f * 0.25f},
	};

	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		size = sizes[i];
		tex = createTexture(size.width, size.height);

		fbo->bind(tex);
		t = createImage(strPath[i]);
		DRAWIMAGE(t, size);
		freeImage(t);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);
		if (i == 1)
		{
			img->position = iPointMake(sizes[0].width * 0.025f, sizes[0].height * 0.7f);
			img->texCoordx = 0.0f;
			imgGameLoading = img;
		}

		pop->addObject(img);
	}

	pop->openPosition =
	pop->closePosition = iPointMake((devSize.width - sizes[0].width) / 2.0f, (devSize.height - sizes[0].height) / 3.0f);
	pop->methodDrawBefore = drawPopGameLoadingBefore;
	popGameLoading = pop;
}

void freePopGameLoading()
{
	delete popGameLoading;
}

void showPopGameLoading(bool show)
{
	popGameLoading->show(show);
}

void drawPopGameLoadingBefore(iPopup* me, iPoint p, float dt)
{
	float d = popGameLoadingDt / _popGameLoadingDt;
	imgGameLoading->texCoordx = d;
}

void drawPopGameLoading(float dt)
{
	popGameLoading->paint(dt);
}