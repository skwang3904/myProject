#include "iDrawNumber.h"

numFont* numberFont;
numFont::numFont()
{
	const char* str = "0123456789:?!./,-+";

	fontNum = (iFont*)malloc(sizeof(iFont) * 1);

	int num = strlen(str);
	fontNum->strNum = num;
	fontNum->tex = (Texture**)malloc(sizeof(Texture*) * num);

	for (int i = 0; i < num; i++)
	{
		if (i == num - 1)	   fontNum->tex[i] = createImage("assets/imgNumber/plus.png");		 // +
		else if (i == num - 2) fontNum->tex[i] = createImage("assets/imgNumber/minus.png");		 // -
		else if (i == num - 3) fontNum->tex[i] = createImage("assets/imgNumber/rest.png");		 // ,
		else if (i == num - 4) fontNum->tex[i] = createImage("assets/imgNumber/slash.png");		 // /
		else if (i == num - 5) fontNum->tex[i] = createImage("assets/imgNumber/period.png");		 // .
		else if (i == num - 6) fontNum->tex[i] = createImage("assets/imgNumber/exclamation.png"); // !
		else if (i == num - 7) fontNum->tex[i] = createImage("assets/imgNumber/question.png");    // ?
		else if (i == num - 8) fontNum->tex[i] = createImage("assets/imgNumber/equal.png");       // :
		else                   fontNum->tex[i] = createImage("assets/imgNumber/%d.png", i);
	}
}

numFont::~numFont()
{
	for (int i = 0; i < 18; i++)
		freeImage(fontNum->tex[i]);

	free(fontNum);

}

void numFont::drawFont(int x, int y, int anc, int dis, float ratX, float ratY, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	char szText[1024];
	_vsnprintf(szText, sizeof(szText), szFormat, args);
	va_end(args);

	int i, len = strlen(szText);
	int w = 0;
	int h = 0;
	for (i = 0; i < len; i++)
	{
		Texture* tex;
		if (szText[i] == '+')      tex = fontNum->tex[fontNum->strNum - 1];
		else if (szText[i] == '-') tex = fontNum->tex[fontNum->strNum - 2];
		else if (szText[i] == ',') tex = fontNum->tex[fontNum->strNum - 3];
		else if (szText[i] == '/') tex = fontNum->tex[fontNum->strNum - 4];
		else if (szText[i] == '.') tex = fontNum->tex[fontNum->strNum - 5];
		else if (szText[i] == '!') tex = fontNum->tex[fontNum->strNum - 6];
		else if (szText[i] == '?') tex = fontNum->tex[fontNum->strNum - 7];
		else if (szText[i] == ':') tex = fontNum->tex[fontNum->strNum - 8];
		else                	   tex = fontNum->tex[szText[i] - '0'];

		w += (dis + tex->width * ratX);
		h += (dis + tex->height * ratY);

	}

	switch (anc)
	{
	case TOP | LEFT:                                    break;
	case TOP | HCENTER:        x -= w / 2;              break;
	case TOP | RIGHT:          x -= w;                  break;
	case VCENTER | LEFT:                    y -= h / 2; break;
	case VCENTER | HCENTER:    x -= w / 2;  y -= h / 2; break;
	case VCENTER | RIGHT:      x -= w;      y -= h / 2; break;
	case BOTTOM | LEFT:                     y -= h;     break;
	case BOTTOM | HCENTER:     x -= w / 2;  y -= h;     break;
	case BOTTOM | RIGHT:       x -= w;      y -= h;     break;
	}

	for (i = 0; i < len; i++)
	{
		Texture* tex;
		if (szText[i] == '+')      tex = fontNum->tex[fontNum->strNum - 1];
		else if (szText[i] == '-') tex = fontNum->tex[fontNum->strNum - 2];
		else if (szText[i] == ',') tex = fontNum->tex[fontNum->strNum - 3];
		else if (szText[i] == '/') tex = fontNum->tex[fontNum->strNum - 4];
		else if (szText[i] == '.') tex = fontNum->tex[fontNum->strNum - 5];
		else if (szText[i] == '!') tex = fontNum->tex[fontNum->strNum - 6];
		else if (szText[i] == '?') tex = fontNum->tex[fontNum->strNum - 7];
		else if (szText[i] == ':') tex = fontNum->tex[fontNum->strNum - 8];
		else                 	   tex = fontNum->tex[szText[i] - '0'];

		drawImage(tex, x, y, 0, 0, tex->width, tex->height, TOP | LEFT, ratX, ratY, 2, 0, REVERSE_NONE);
		x += (dis + tex->width * ratX);
	}

}

//void numFont::drawFont(int x, int y, int anc, const char* szFormat, ...)
//{
//	drawFont(x, y, anc, 1, 1, 1, szFormat);
//}

