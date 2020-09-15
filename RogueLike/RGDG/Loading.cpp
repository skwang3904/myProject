#include "Loading.h"

int8 gamestat, _gamestat;
METHOD_LOADING methodLoadingFree;
METHOD_LOADING methodLoadingLoad;

#define _loadingDt 1000.0f
float loadingDt = _loadingDt;

void setLoading(int8 newGameState, METHOD_LOADING free, METHOD_LOADING load)
{
	_gamestat = newGameState;
	methodLoadingFree = free;
	methodLoadingLoad = load;

	loadingDt = 0.0f;
}

void drawLoading(float dt)
{
	if (loadingDt == _loadingDt)
		return;

	float alpha = 0.0f;

	if (loadingDt < 0.5f)
	{
		loadingDt += dt;
		if (loadingDt > 0.5f)
		{
			loadingDt = 0.5f;
			methodLoadingFree();
			methodLoadingLoad();
			gamestat = _gamestat;
		}
		alpha = loadingDt / 0.5f;
	}
	else if (loadingDt == 0.5f)
	{
		loadingDt += 0.000001f;
		alpha = 1.0f;
	}
	else
	{
		loadingDt += dt;
		alpha = 1.0f - (loadingDt - 0.5f) / 0.5f;
		if (loadingDt > 1.0f)
			loadingDt = _loadingDt;
	}

	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
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
