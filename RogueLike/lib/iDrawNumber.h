#pragma once

#include "iStd.h"

//-------------------------------------
// TDfont
//-------------------------------------


struct iFont
{
	int strNum;
	Texture** tex;
};

class numFont
{
public:
	numFont();
	virtual ~numFont();

	void drawFont(int x, int y, int anc, int dis, float ratX, float ratY, const char* szFormat, ...);
	//void drawFont(int x, int y, int anc, const char* szFormat, ...);

public:
	iFont* fontNum;
};

extern numFont* numberFont;

void loadNumberFont();
void freeNumberFont();