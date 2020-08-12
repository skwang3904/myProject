#include "RgMenu.h"

void createCharTexture();
void freeCharTexture();

void loadRgMenu()
{
	createCharTexture();

	createPopMenuBG();
	createPopMenuButton();

	showPopMenuBG(true);
	showPopMenuButton(true);
}

void freeRgMenu()
{
	freeCharTexture();

	freePopMenuBG();
	freePopMenuButton();
}

void drawRgMenu(float dt)
{
	drawPopMenuBG(dt);
	drawPopMenuButton(dt);
}

void keyRgMenu(iKeyState stat, iPoint point)
{
	if (keyPopMenuBG(stat, point) ||
		keyPopMenuButton(stat, point))
		return;
}

//-------------------------------------------------------------------------------------

iPopup* popMenu;
iImage** imgMainChar;

Texture*** texChars;

void createCharTexture()
{
	texChars = (Texture***)malloc(sizeof(Texture**) * 2);

	const char* strPath[2] = {
	"assets/char/CharGood.png",
	"assets/char/CharJump.png"
	};

	for (int i = 0; i < 2; i++)
		texChars[i] = createDivideImage(4, 1, strPath[i]);
}

void freeCharTexture()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
			freeImage(texChars[i][j]);
		free(texChars[i]);
	}
	free(texChars);
}

void createPopMenuBG()
{
	iPopup* pop = new iPopup(iPopupStyleNone);
	iImage* img;
	Texture* tex;

	imgMainChar = (iImage**)malloc(sizeof(iImage*) * 2);

	iPoint size[3] = { 
		iPointMake(devSize.width, devSize.height),
		iPointMake(devSize.width / 4.0f, devSize.height / 3.0f),
		iPointMake(devSize.width / 4.0f, devSize.height / 3.0f)
	};

	const char* strPath = {	"assets/menu/Menu_background.png" };

	for (int i = 0; i < 3; i++)
	{
		iPoint p = size[i];
		img = new iImage();
		if (i == 0)
		{
			tex = createTexture(p.x, p.y);
			fbo->bind(tex);
			fbo->setSize(p);

			Texture* texBG = createImage(strPath);
			drawImage(texBG, 0, 0,
				0, 0, texBG->width, texBG->height,
				TOP | LEFT, p.x / texBG->width, p.y / texBG->height,
				2, 0);
			freeImage(texBG);

			fbo->backSize();
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		else // 캐릭터
		{
			Texture** texChar = texChars[i-1];
			for (int j = 0; j < 4; j++)
			{
				tex = createTexture(p.x, p.y);

				fbo->bind(tex);
				fbo->setSize(p);

				Texture* t = texChar[j];
				drawImage(t, 0, 0,
					0, 0, t->width, t->height,
					TOP | LEFT, p.x / t->width, p.y / t->height,
					2, 0);

				fbo->backSize();
				fbo->unbind();

				img->addObject(tex);
				freeImage(tex);
			}

			if (i == 1)
			{ 
				imgMainChar[0] = img; 
				img->position = iPointMake(p.x, p.y * 1.3f);
			}
			else 
			{
				imgMainChar[1] = img;
				img->position = iPointMake(p.x + devSize.width / 2.0f, p.y * 1.3f);

			}
			img->animation = true;
			img->_aniDt = 1.0f;
		}

		img->reverse = REVERSE_HEIGHT;
		pop->addObject(img);
	}

	img = new iImage();
	const char* strChar = {
	"캐릭터 정보\n"
	"체력 = 100\n"
	"공격력 = 10\n"
	"공격속도 = 1.0\n"
	"이동속도 = 700\n"
	"대충 기본캐릭이라는뜻"
	};

	iPoint p = size[1];
	iGraphics* g = iGraphics::instance();
	g->init(p.x, p.y);
	
	setStringName("assets/BMJUA_ttf.ttf");
	setStringSize(40);
	setStringRGBA(1, 1, 0, 1);
	setStringBorder(0);

	int lineNum;
	char** line = iString::getStringLine(strChar, lineNum);
	for (int i = 0; i < lineNum; i++)
		g->drawString(0, 45 * i, TOP | LEFT, line[i]);
	iString::freeStringLine(line, lineNum);

	setStringName("궁서체");
	setStringSize(1);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	tex = g->getTexture();

	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(p.x * 2.0f, p.y * 1.3f);
	pop->addObject(img);

	pop->openPosition = iPointZero;
	pop->closePosition = iPointZero;
	popMenu = pop;
}

void freePopMenuBG()
{
	delete popMenu;
}

void showPopMenuBG(bool show)
{
	popMenu->show(show);
}

void drawPopMenuBG(float dt)
{
	for (int i = 0; i < 2; i++)
	{
		iImage* img = imgMainChar[i];
		img->position += iPointMake(10, 0);
		if (img->position.x > devSize.width)
			img->position.x = -img->tex->width;
	}

	popMenu->paint(dt);
}

bool keyPopMenuBG(iKeyState stat, iPoint point)
{
	return false;
}

//-------------------------------------------------------------------------------------

iPopup* popMenuBtn;
iImage** imgMenuBtn;

void drawBeforeMenuButton(iPopup* me, iPoint p, float dt);

void createPopMenuButton()
{
	int i, j;
	iPopup* pop = new iPopup(iPopupStyleNone);
	iImage* img = new iImage();
	Texture* tex, *t;

	imgMenuBtn = (iImage**)malloc(sizeof(iImage*) * 6);

	iPoint size = iPointMake(devSize.width * 0.8f, devSize.height * 0.4f);
	tex = createTexture(size.x, size.y);
	fbo->bind(tex);
	fbo->setSize(size);

	t = createImage("assets/menu/Logo.png");
	drawImage(t, 0, 0,
		0, 0, t->width, t->height,
		TOP | LEFT, size.x / t->width, size.y / t->height,
		2, 0);
	freeImage(t);

	fbo->backSize();
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	img->reverse = REVERSE_HEIGHT;
	img->position = iPointMake(devSize.width * 0.09f, 0.0f);
	pop->addObject(img);

	size = iPointMake(devSize.height / 10.0f, devSize.height / 10.0f);
	for (i = 0; i < 4; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.x, size.y);
			fbo->bind(tex);
			fbo->setSize(size);

			t = createImage("assets/menu/selectButton%d.png", j);
			drawImage(t, 0, 0,
				0, 0, t->width, t->height,
				TOP | LEFT, size.x / t->width, size.y / t->height,
				2, 0);
			freeImage(t);

			if (i < 2)
			{
				t = texChars[i][0];
				drawImage(t, size.x / 2.0f, size.y / 2.0f,
					0, 0, t->width, t->height,
					VCENTER | HCENTER, size.x / t->width * 1.5f, size.y / t->height,
					2, 0);
			}

			fbo->backSize();
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		img->reverse = REVERSE_HEIGHT;
		img->position = iPointMake(devSize.width * 0.25f + devSize.width * 0.15f * i,
			devSize.height * 0.22f);

		imgMenuBtn[i] = img;
		pop->addObject(img);
	}

	const char* strPath[2] = {
	"left",	"right"
	};

	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.x, size.y);
			fbo->bind(tex);
			fbo->setSize(size);

			const char* str = j == 0 ? strPath[i] : "check";
			t = createImage("assets/menu/%sArrow.png", str);
			drawImage(t, 0, 0,
				0, 0, t->width, t->height,
				TOP | LEFT, size.x / t->width, size.y / t->height,
				2, 0);
			freeImage(t);

			fbo->backSize();
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		imgMenuBtn[4 + i] = img;
		img->reverse = REVERSE_HEIGHT;
		img->position = iPointMake(img->tex->width + (devSize.width - img->tex->width * 3.0f) * i,
			(devSize.height - img->tex->height) * 0.5f);
		pop->addObject(img);
	}

	pop->methodDrawBefore = drawBeforeMenuButton;
	popMenuBtn = pop;
}

void freePopMenuButton()
{
	delete popMenuBtn;
	free(imgMenuBtn);
}

void showPopMenuButton(bool show)
{
	popMenuBtn->show(show);
}

void drawBeforeMenuButton(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 6; i++)
		imgMenuBtn[i]->setTexAtIndex(popMenuBtn->selected == i);
}

void drawPopMenuButton(float dt)
{
	popMenuBtn->paint(dt);
}

bool keyPopMenuButton(iKeyState stat, iPoint point)
{
	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popMenuBtn->selected;
		if (i == -1) break;

		break;
	}
	case iKeyStateMoved:
	{
		for (int i = 0; i < 6; i++)
		{
			if (containPoint(point, imgMenuBtn[i]->touchRect(popMenuBtn->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popMenuBtn->selected != j)
		{
			;
		}

		popMenuBtn->selected = j;
		break;
	}
	case iKeyStateEnded:
	{
		break;
	}
	default:
		break;
	}

	return false;
}
