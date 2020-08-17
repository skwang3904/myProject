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
iPopup** popMenuChar;

iImage* imgMainCharExplanation;
iImage** imgMainChar;

Texture*** texChars;

void createCharTexture()
{
	texChars = (Texture***)malloc(sizeof(Texture**) * 2);

	const char* strPath[2] = {
	"assets/char/CharGood.png",
	"assets/char/CharGood2.png"
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
	popMenuChar = (iPopup**)malloc(sizeof(iPopup*) * 2);
	iPopup* pop0 = new iPopup(iPopupStyleMove);
	iPopup* pop1 = new iPopup(iPopupStyleMove);

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
				//img->position = iPointMake(p.x, p.y * 1.3f);
			}
			else 
			{
				imgMainChar[1] = img;
				//img->position = iPointMake(p.x + devSize.width / 2.0f, p.y * 1.3f);

			}
			img->animation = true;
			img->_aniDt = 1.0f;
		}

		img->reverse = REVERSE_HEIGHT;

		if (i == 0)
			pop->addObject(img);
		else if (i == 1)
		{
			iPopup* pp = pop0;
			pp->addObject(img);
			pp->openPosition = iPointMake(p.x, p.y * 1.3);
			pp->closePosition = iPointMake(p.x, p.y * 1.3);
			
		}
		else
		{
			iPopup* pp = pop1;
			pp->addObject(img);
			pp->openPosition = iPointMake(p.x - devSize.width/2.0f, p.y * 1.3);
			pp->closePosition = iPointMake(p.x - devSize.width / 2.0f, p.y * 1.3);
		}
	}

	img = new iImage();
	const char* strChar[2] = {
	"캐릭터\n체력 = 100\n공격력 = 10\n공격속도 = 1.0\n이동속도 = 700\n첫번째",
	"캐릭터\n체력 = 100\n공격력 = 10\n공격속도 = 1.0\n이동속도 = 700\n두번째",
	};

	iPoint p = size[1];
	iGraphics* g = iGraphics::instance();

	for (int i = 0; i < 2; i++)
	{
		g->init(p.x, p.y);

		setStringName("assets/BMJUA_ttf.ttf");
		setStringSize(40);
		setStringRGBA(1, 1, 0, 1);
		setStringBorder(0);

		int lineNum;
		char** line = iString::getStringLine(strChar[i], lineNum);
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
	}

	img->position = iPointMake(p.x * 2.0f, p.y * 1.3f);
	imgMainCharExplanation = img;
	pop->addObject(img);

	pop->openPosition = iPointZero;
	pop->closePosition = iPointZero;
	popMenu = pop;

	pop0->show(true);
	pop0->_showDt = 1.0f;

	pop1->show(false);
	pop1->_showDt = 1.0f;

	popMenuChar[0] = pop0;
	popMenuChar[1] = pop1;
}

void freePopMenuBG()
{
	delete popMenu;

	delete popMenuChar[0];
	delete popMenuChar[1];
	free(popMenuChar);
}

void showPopMenuBG(bool show)
{
	popMenu->show(show);
}

void drawPopMenuBG(float dt)
{
	popMenu->paint(dt);

	popMenuChar[0]->paint(dt);
	popMenuChar[1]->paint(dt);
}

bool keyPopMenuBG(iKeyState stat, iPoint point)
{
	return false;
}

//-------------------------------------------------------------------------------------

iPopup* popMenuBtn;
iImage** imgMenuBtn;
extern int playerCharNumber;

void drawBeforeMenuButton(iPopup* me, iPoint p, float dt);

void createPopMenuButton()
{
	int i, j;
	iPopup* pop = new iPopup(iPopupStyleNone);
	iImage* img = new iImage();
	Texture* tex, *t;

	imgMenuBtn = (iImage**)malloc(sizeof(iImage*) * 7);

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

	img = new iImage();
	size = iPointMake(devSize.width * 0.5f, devSize.height * 0.15f);
	iGraphics* g = iGraphics::instance();
	for (int i = 0; i < 2; i++)
	{
		g->init(size.x, size.y);
		igImage* igimg = g->createIgImage("assets/PlayerUI/combatUI.png");

		float width = igimg->GetWidth();
		float height = igimg->GetHeight();
		setRGBA(1, 1, 1, i == 0 ? 0.7f : 1.0f);
		g->drawImage(igimg, 0, 0,
			0, 0, width, height, 
			TOP | LEFT, size.x / width, size.y / height,
			2, 0, REVERSE_NONE);
		setRGBA(1, 1, 1, 1);

		setStringRGBA(0, i, 0, 1);
		setStringSize(50);
		setStringName("assets/BMJUA_ttf.ttf");
		setStringBorder(0);
		
		g->drawString((size.x - 200) / 2.0f, (size.y - 50) / 2.0f, TOP | LEFT, "게임 시작");

		setStringRGBA(1, 1, 1, 1);
		setStringSize(1);
		setStringName("궁서체");
		setStringBorder(0);
			
		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	
	img->position = iPointMake((devSize.width - img->tex->width) * 0.5f, devSize.height * 0.75f);
	imgMenuBtn[6] = img;
	pop->addObject(img);

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
	for (int i = 0; i < 7; i++)
		imgMenuBtn[i]->setTexAtIndex(popMenuBtn->selected == i);
}

void drawPopMenuButton(float dt)
{
	popMenuBtn->paint(dt);
}

bool currentChar(int i)
{
	if (playerCharNumber == i)
		return true;

	playerCharNumber = i;
	imgMainCharExplanation->setTexAtIndex(i);

	return false;
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

		if (i == 0)
		{
			if (currentChar(0))
				break;

			iPopup* p0 = popMenuChar[0];
			iPopup* p1 = popMenuChar[1];
			p0->show(true);
			p1->show(false);
			p0->openPosition = iPointMake(-imgMainChar[0]->tex->width, devSize.height / 3.0 * 1.3f);
			p0->closePosition = iPointMake(devSize.width / 4.0f, devSize.height / 3.0 * 1.3f);

			p1->openPosition = iPointMake(devSize.width, devSize.height / 3.0 * 1.3f);
			p1->closePosition = iPointMake(devSize.width / 4.0f, devSize.height / 3.0 * 1.3f);
		}
		else if (i == 1)
		{
			if (currentChar(1))
				break;

			iPopup* p0 = popMenuChar[0];
			iPopup* p1 = popMenuChar[1];
			p0->show(false);
			p1->show(true);

			p0->openPosition = iPointMake(-imgMainChar[0]->tex->width, devSize.height / 3.0 * 1.3f);
			p0->closePosition = iPointMake(devSize.width / 4.0f, devSize.height / 3.0 * 1.3f);

			p1->openPosition = iPointMake(devSize.width, devSize.height / 3.0 * 1.3f);
			p1->closePosition = iPointMake(devSize.width / 4.0f, devSize.height / 3.0 * 1.3f);
		}
		else if (i == 2);
		else if (i == 3);
		else if (i == 4);
		else if (i == 5);
		else if (i == 6);

		break;
	}
	case iKeyStateMoved:
	{
		for (int i = 0; i < 7; i++)
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
