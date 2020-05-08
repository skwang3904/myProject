#pragma once

struct iColor4f
{
	float r, g, b, a;
};

struct iColor4b
{
	unsigned char r, g, b, a;
};

#define iColor4fWhite iColor4fMake(1, 1, 1, 1);
#define iColor4fBlack iColor4fMake(0, 0, 0, 1);
iColor4f iColor4fMake(float r, float g, float b, float a);

#define iColor4bWhite iColor4bMake(0xFF, 0xFF, 0xFF, 0xFF);
#define iColor4bBlack iColor4bMake(0x00, 0x00, 0x00, 0xFF);
iColor4b iColor4bMake(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

