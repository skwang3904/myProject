#include "loading.h"

#include "RgGame.h"

bool loadingCheck;
int _gamestat;
Loading_Method methodFree;
Loading_Method methodLoad;

#define _loadingDt 100.0f
float loadingDt = _loadingDt;

void setLoading(int gamestat, Loading_Method free, Loading_Method load)
{
	_gamestat = gamestat;
	methodFree = free;
	methodLoad = load;
	loadingDt = 0.0f;
	loadingCheck = true;
}

void drawLoading(float dt)
{
	if (loadingDt == _loadingDt)
		return;

	float alpha;

	if (loadingDt < 0.5f)
	{
		loadingDt += dt;
		if (loadingDt > 0.5f)
			loadingDt = 0.5f;
		alpha = loadingDt / 0.5f;
	}
	else if (loadingDt == 0.5f)
	{
		methodFree();
		methodLoad();
		gamestat = _gamestat;
		loadingDt += 0.1f;
		alpha = 1.0f;
	}
	else //if( loadingDt > 0.5f)
	{
		loadingDt += dt;
		alpha = 7.0f - (loadingDt - 0.5f) / 0.5f;
		printf("%.2f\n" , alpha);
		if (loadingDt > 4.0f)
		{
			loadingDt = _loadingDt;
			loadingCheck = false;
		}
	}

	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
}

bool keyLoading(iKeyState stat, iPoint point)
{
	if (loadingDt < _loadingDt)
		return true;
	return false;
}