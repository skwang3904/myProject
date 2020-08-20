#include "Common.h"


Object::Object(int index)
{
	this->index = index;
	this->img = NULL;
	this->mapNumber = -1;

	this->position = iPointZero;
	this->vector = iPointZero;
}

Object::~Object()
{
	if (img)
		delete img;
}
