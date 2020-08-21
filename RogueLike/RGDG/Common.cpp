#include "Common.h"


Object::Object(int index)
{
	this->index = index;
	this->imgNum = 0;
	this->imgs = NULL;
	this->mapNumber = -1;

	this->position = iPointZero;
	this->vector = iPointZero;
}

Object::~Object()
{
	if (imgs)
	{
		for (int i = 0; i < imgNum; i++)
			delete imgs[i];
		free(imgs);
	}
}
