#pragma once

#include "iPoint.h"
#include "iSize.h"

struct iRect
{
	iPoint origin;
	iSize size;
};

#define iRectZero iRectMake(0, 0, 0, 0)
iRect iRectMake(float x, float y, float width, float height);

