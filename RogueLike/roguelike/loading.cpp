#include "loading.h"

int gamestat;
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
}

bool getLoading()
{
	return loadingDt < _loadingDt;
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
		alpha = 1.0f - (loadingDt - 0.5f) / 2.0f;
		if (loadingDt > 2.5f)
			loadingDt = _loadingDt;
	}

	setRGBA(0, 1, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
}

bool keyLoading(iKeyState stat, iPoint point)
{
	if (loadingDt < _loadingDt)
		return true;
	return false;
}