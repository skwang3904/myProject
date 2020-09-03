#include "iRect.h"

iRect iRectMake(float x, float y, float width, float height)
{
	iRect r;
	r.origin.x = x;
	r.origin.y = y;
	r.size.width = width;
	r.size.height = height;
	return r;
}

iRect iRectMake(iPoint point, iSize size)
{
	return iRectMake(point.x, point.y, size.width, size.height);
}
