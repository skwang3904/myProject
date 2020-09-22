#include "Proc.h"

#include "Loading.h"
#include "Intro.h"

#include "Map.h"
#include "ProcData.h"
#include "PlayerChar.h"
#include "Monster.h"
#include "Weapon.h"
#include "Item.h"
#include "Projectile.h"

iSort* procSort = NULL;
void loadProc()
{
	objects = (Object**)malloc(sizeof(Object*) * _sdNum);
	procSort = new iSort();

	loadNumberFont();

	loadMap();
	loadStage();
	loadPlayerChar();
	loadMonster();
	loadWeapon();
	loadItem();
	loadProjectile();

	loadPassMap();

	createPopState();
	createPopProcButton();
	createPopMiniMap();
	createPopInven();
	createPopStageNum();
	createPopProcMenu();
	createPopGameOver();

	createPopStageLoading();

	showPopStageNum(true);
	showPopState(true);
	showPopProcButton(true);
	showPopMiniMap(true);

	audioPlay(AUDIO_GameMusic);
}

void freeProc()
{
	delete procSort;
	freeNumberFont();

	freeMap();
	freePlayerChar();
	freeMonster();
	freeWeapon();
	freeItem();
	freeProjectile();

	freePassMap();
	freeStage();

	freePopState();
	freePopProcButton();
	freePopMiniMap();
	freePopInven();
	freePopStageNum();
	freePopProcMenu();
	freePopGameOver();

	freePopStageLoading();
}

void drawProc(float dt)
{
#if SORTING
	procSort->init();
#endif

	float pop_dt = dt;
	if (popProcMenu->bShow || 
		popGameOver->bShow)
		dt = 0.0f;

	float pass_dt = dt;
	if (passMap->passDt < passMap->_passDt ||
		passMap->nextDt < passMap->_nextDt)
		pass_dt = 0.0f;

	drawMap(pass_dt);
	drawPlayerChar(pass_dt);
	drawMonster(pass_dt);
	drawWeapon(pass_dt);
	drawItem(pass_dt);
#if SORTING
	procSort->update();
	for(int i=0; i<procSort->sdNum;i++)
		objects[procSort->get(i)]->paint(dt, DRAW_OFF);
#endif
	drawProjectile(pass_dt);

	passMap->update(dt);

	drawPopState(dt);
	drawPopProcButton(dt);
	drawPopMiniMap(dt);
	drawPopInven(dt);
	drawPopStageNum(dt);
	drawPopProcMenu(pop_dt);
	drawPopGameOver(pop_dt);

	//numberFont->drawFont()

	if (passMap->nextStage(dt))
	{
		drawPopStageLoading(dt);
		return;
	}
}

void keyProc(iKeyState stat, iPoint point)
{
	if (keyPopGameOver(stat, point) ||
		keyPopProcMenu(stat, point) ||
		keyPopInven(stat,point)	||
		keyPopMiniMap(stat, point) ||
		keyPopProcButton(stat, point) ||
		keyPopState(stat, point) ||
		keyPopStageNum(stat, point) ||
		keyPopStageLoading(stat, point))
		return;
}

//----------------------------------------------------------------------------
// Stage
Stage* st = NULL;
void Stage::create()
{
	int i, j = 0, k, num = TILE_TOTAL_NUM;

	stageNum = 0;
	//maps
	for (i = 0; i < num; i++)
	{
		st->mapData[i].state = maps[i]->state;
		st->mapData[i].tileIndex = maps[i]->tileIndex;
	}

	int actMap[TILE_CONNECT_NUM];
	for (i = 0; i < TILE_CONNECT_NUM; i++)
	{
		while (1)
		{
			if (maps[j]->tileIndex != -1)
			{
				actMap[i] = j;
				j++;
				break;
			}
			j++;
		}
	}

	//player
	setPlayerData(actMap, TILE_CONNECT_NUM);

	PlayerData* pd = &playerData;
	PlayerInfo* pi = &playerInfo[pd->index];
	pd->hp = pd->_hp = pi->_hp;
	pd->attackPoint = pd->_attackPoint = pi->_attackPoint;
	pd->attackSpeed = 0.0f;
	pd->_attackSpeed = pi->_attackSpeed;
	pd->moveSpeed = pi->moveSpeed;

	//monster
	setMonsterData(actMap, TILE_CONNECT_NUM);

	for (int i = 0; i < 10; i++)
	{
#if 0
		int a = -1;
		if (i == 0)			a = 1;
		else if (i == 1)	a = 0;
		else if( i == 2)	a = 2;
#elif 1
		if (i < 4)
			weaponData[i].index = i;
		else
			weaponData[i].index = -1;
#else
		weaponData[i].index = i;
#endif
	}
}

void Stage::setStageData()
{
	int i, j = 0, num = TILE_TOTAL_NUM;

	for (i = 0; i < num; i++)
	{
		mapData[i].state = (int)maps[i]->state;
		mapData[i].tileIndex = maps[i]->tileIndex;
	}

	int actMap[TILE_CONNECT_NUM];
	for (i = 0; i < TILE_CONNECT_NUM; i++)
	{
		while (1)
		{
			if (maps[j]->tileIndex != -1)
			{
				actMap[i] = j;
				j++;
				break;
			}
			j++;
		}
	}

	// playerInfo
	setPlayerData(actMap, TILE_CONNECT_NUM);

	PlayerData* pd = &playerData;
	PlayerChar* p = player;
	pd->hp = p->hp;
	pd->_hp = p->_hp;
	pd->attackPoint = p->attackPoint;
	pd->_attackPoint = p->_attackPoint;
	pd->attackSpeed = p->attackSpeed;
	pd->_attackSpeed = p->_attackSpeed;
	pd->moveSpeed = p->moveSpeed;

	// monsterInfo 
	setMonsterData(actMap, TILE_CONNECT_NUM);

	// weaponInfo
	num = weaponNum;
	for (i = 0; i < num; i++)
	{
		WeaponData* wd = &weaponData[i];
		wd->index = weapon[i]->index;
	}

	saveStage();
}

void Stage::setPlayerData(int* actMap, int connectNum)
{
	int* am = actMap;
	int cnNum = connectNum;

	PlayerData* pd = &playerData;
	pd->index = 0;
	pd->mapNum = am[random() % cnNum];
	pd->position = iPointMake(TILE_NUM_X * TILE_Width / 2.0f,
		TILE_NUM_Y * TILE_Height / 2.0f);
}

void Stage::setMonsterData(int* actMap, int connectNum)
{
	int i, j = 0, num = 0;
	int* am = actMap;
	int cnNum = connectNum;

	if (monster)
	{
		for (i = 0; i < MT_max; i++)
			num += actMonsterNum[i];

		for (i = 0; i < num; i++)
			delete _monster[i];
		free(_monster);
		_monster = NULL;
		free(monster);
		monster = NULL;
	}

	int n = 0;
	num = TILE_NUM_X * TILE_NUM_Y;
	for (i = 0; i < TILE_TOTAL_NUM; i++)
	{
		MapTile* m = maps[i];
		if (m->state == MapType_Boss) continue;
		for (j = 0; j < num; j++)
		{
			if (m->tile[j] == TILE_MONSTER_SPAWN)
			{
				MonsterData* md = &monsterData[n];
				md->index = 0;
				md->mapNum = i;
				md->position = iPointMake(TILE_Width * (j % TILE_NUM_X), TILE_Height * (j / TILE_NUM_X));
				n++;
			}
		}
	}

	for (i = 0; i < MT_max; i++)
	{
		switch (i)
		{
		case MT_golemNomal: /*n = 5 + random() % 10;*/ break;
		case MT_golemElete: n = 0;  break;
		case MT_golemBoss: n = 1; break;
		default: break;
		}
		actMonsterNum[i] = n;
	}

	for (i = 0; i < cnNum; i++)
	{
		if (maps[am[i]]->state == MapType_Boss)
		{
			MonsterData* md = &monsterData[actMonsterNum[MT_golemNomal]];
			md->index = 0;
			md->mapNum = am[i];
			md->position = iPointMake(TILE_Width * TILE_NUM_X, TILE_Height * TILE_NUM_Y) / 2.0f - iPointMake(100, 100);
		}
	}
}

void loadStageNew()
{
	createMap(false);
	st = (Stage*)calloc(sizeof(Stage), 1);
	st->create();
	saveStage();

	createMapImage();
	setRGBA(1, 1, 1, 1);
}

void loadStageContinue()
{
	int length;
	char* buf = loadFile(FILE_PATH, length);
	st = (Stage*)buf;
	createMap(true);

	createMapImage();
	setRGBA(1, 1, 1, 1);
}

bool isNewGame = true;
void loadStage()
{
	if (isNewGame)	loadStageNew();
	else			loadStageContinue();
}

void saveStage()
{
	saveFile(FILE_PATH, (char*)st, sizeof(Stage));
}

void freeStage()
{
	free(st);
}

//----------------------------------------------------------------------------
// passMap
PassMap* passMap;
void PassMap::init()
{
	nextDt = _nextDt = PASS_STAGE_DT;
	center = iPointMake(devSize.width, devSize.height) / 2.0f;

	popDt = _popDt = PASS_POP_DT;

	passDt = _passDt = PASS_MAP_DT;
	mapNumber = prevMapNumber = player->mapNumber;
}

void PassMap::pass(int8 mapNum)
{
	if (mapNum == prevMapNumber)
		return;

	mapNumber = mapNum;
	passDt = 0.0f;
}

void PassMap::update(float dt)
{
	if (passDt == _passDt)
		return;

	MapTile* prev = maps[prevMapNumber];
	MapTile* curr = maps[mapNumber];

	iPoint p = iPointZero;
	switch (mapNumber - prevMapNumber)
	{ //LRUD
	case 1:					p = curr->tileOff + iPointMake(TILE_NUM_X * TILE_Width, 0);		break;
	case -1:				p = curr->tileOff + iPointMake(-TILE_NUM_X * TILE_Width, 0);	break;
	case TILE_TOTAL_SQRT:	p = curr->tileOff + iPointMake(0, TILE_NUM_Y * TILE_Height);	break;
	case -TILE_TOTAL_SQRT:	p = curr->tileOff + iPointMake(0, -TILE_NUM_Y * TILE_Height);	break;
	default: printf("pass tile error\n");
	}

	passDt += dt;
	if (passDt > _passDt)
	{
		passDt = _passDt;
		prevMapNumber = mapNumber;

#if 0
		player->camera = iPointZero - maps[mapNumber]->tileOff;
		player->mapNumber = mapNumber;
#endif 
	}

	float d = passDt / _passDt;
	iPoint pp = linear(d, curr->tileOff, prev->tileOff);
	iPoint cp = linear(d, p, curr->tileOff);

	setRGBA(0.3f, 0.3f, 0.3f, 1);
	if (passDt < _passDt)
		prev->img->paint(dt, pp + DRAW_OFF);
	curr->img->paint(dt, cp + DRAW_OFF);
	setRGBA(1, 1, 1, 1);
}

extern bool runWnd;
void PassMap::startNextStage()
{
	st->stageNum++;
	if (st->stageNum < END_STAGE)
	{
		nextDt = 0.0f;
	}
	else
	{
		MessageBox(NULL, TEXT("GAME CLEAR"), TEXT("Congratulation"), MB_OK);
		runWnd = false;
		audioStop(AUDIO_GameMusic);
	}
}

bool PassMap::nextStage(float dt)
{
	if (nextDt == _nextDt)
		return false;

	float nd = _nextDt / 2.0f;
	float d = 0.0f;
	if (nextDt < nd)
	{
		nextDt += dt;
		if (nextDt > nd)
		{
			nextDt = nd;
			popDt = 0.0f;
			createMap();
			st->setStageData();
			createMapImage();
			loadPlayerChar();
			loadMonster();
			loadItem();

			showPopStageLoading(true);
		}

		d = 1.0f - nextDt / nd;
	}
	else if (nextDt == nd)
	{
		if (popDt < _popDt)
		{
			if (popStageLoading->stat == iPopupStatProc)
			{
				popDt += dt;
				if (popDt == _popDt)
					popDt += 1.0f;
			}
		}
		else if (popDt > _popDt)
		{
			popDt = _popDt;
			showPopStageLoading(false);
		}
		else if (popDt == _popDt)
		{
			if (popStageLoading->bShow == false)
				nextDt += 0.00001f;
		}
	}
	else if (nextDt < _nextDt)
	{
		nextDt += dt;
		if (nextDt > _nextDt)
		{
			nextDt = _nextDt;
			init();
			showPopStageNum(true);
		}
		d = nextDt / nd - 1.0f;
	}

	setRGBA(0, 0, 0, 1);
	float len = iPointLength(iPointMake(devSize.width / 2.0f, devSize.height / 2.0f));
	if (d > 0.0f)	fillCircle(center, len * d, true);
	else			fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);

	return true;
}

void loadPassMap()
{
	passMap = (PassMap*)calloc(sizeof(PassMap), 1);
	passMap->init();
}

void freePassMap()
{
	free(passMap);
}

//-----------------------------------------------------------
// stageLoading
iPopup* popStageLoading;
iImage* imgStageLoadingChar;
iImage** imgStageLoadingCase;

void drawPopStageLoadingAfter(iPopup* me, iPoint p, float dt);

void createPopStageLoading()
{
	int i, j;
	Texture* tex, *t;
	iImage* img;
	iPopup* pop = new iPopup(iPopupStyleAlpha);
	iSize size;

	size = devSize;
	img = new iImage();
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	fbo->clear(0.2f, 0.2f, 0.2f, 1);
	t = createImage("assets/loading/LoadingText.png");
	drawImage(t, size.width / 2.0f, size.height - 150,
		0, 0, t->width, t->height,
		VCENTER | HCENTER, 1.0f, 1.0f,
		2, 0, REVERSE_HEIGHT);
	freeImage(t);

	fbo->unbind();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	const char* strPath[3] = {
	"assets/loading/LoadingCase.png", 
	"assets/loading/LoadingChallenge.png", 
	"assets/loading/LoadingClear.png",
	};

	imgStageLoadingCase = (iImage**)malloc(sizeof(iImage*) * END_STAGE);
	size = iSizeMake(250, 250);
	float ratio = 0.7f;
	for (i = 0; i < END_STAGE; i++)
	{
		img = new iImage();
		for (j = 0; j < 3; j++)
		{
			tex = createTexture(size.width, size.height);
			fbo->bind(tex);
			t = createImage(strPath[0]);
			DRAWIMAGE(t, size);
			freeImage(t);
			if (j != 0)
			{
				t = createImage(strPath[j]);
				drawImage(t, size.width / 2.0f, size.height / 2.0f,
					0, 0, t->width, t->height,
					VCENTER | HCENTER, size.width / t->width * ratio, size.height / t->height * ratio,
					2, 0, REVERSE_HEIGHT);
				freeImage(t);
			}
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		
		float standard = (devSize.width - size.width * END_STAGE) / (END_STAGE + 1);
		img->position = iPointMake(standard + (standard + size.width) * i, devSize.height * 0.33f);
		imgStageLoadingCase[i] = img;
		pop->addObject(img);
	}

	pop->_showDt = 5.0f;

	pop->methodDrawAfter = drawPopStageLoadingAfter;
	popStageLoading = pop;

	imgStageLoadingChar = player->imgs[8];
}

void freePopStageLoading()
{
	delete popStageLoading;
	free(imgStageLoadingCase);
}

void showPopStageLoading(bool show)
{
	popStageLoading->show(show);
	if (show)
	{
		for (int i = 0; i < END_STAGE; i++)
		{
			iImage* img = imgStageLoadingCase[i];
			int stage = st->stageNum;
			int index = 0;
			if (i < stage)
				index = 2;
			else if (i == stage)
				index = 1;
			else
				index = 0;

			img->setTexAtIndex(index);
		}
		iImage* img = imgStageLoadingChar;

		float ratio = min(imgStageLoadingCase[0]->tex->width / img->tex->width, imgStageLoadingCase[0]->tex->height / img->tex->height);
		img->animation = true;
		img->_aniDt = 1.0f;
		img->ratio = ratio;
	}
}

void drawPopStageLoadingAfter(iPopup* me, iPoint p, float dt)
{
	iPoint curr = imgStageLoadingCase[st->stageNum - 1]->position;
	iPoint next = imgStageLoadingCase[st->stageNum]->position;
	
	float d = passMap->popDt / passMap->_popDt;
	iPoint pos = linear(d, curr, next);

	iImage* img = imgStageLoadingChar;
	img->position = pos + iPointMake(20, 40); // tmp

	img->paint(dt, iPointZero);
	if (me->bShow == false)
	{
		iImage* img = imgStageLoadingChar;
		img->animation = false;
		img->position = iPointZero;
		img->ratio = 1.0f;
	}
}

void drawPopStageLoading(float dt)
{
	popStageLoading->paint(dt);
}

bool keyPopStageLoading(iKeyState stat, iPoint point)
{
	return false;
}

//-----------------------------------------------------------
// stageNum
iPopup* popStageNum;
iStrTex* stStageNum;

Texture* methodStStageNum(const char* str);

void createPopStageNum()
{
	int i, j;
	Texture* tex, * t;
	iImage* img;
	iPopup* pop = new iPopup(iPopupStyleMove);
	iSize size;

	img = new iImage();
	size = iSizeMake(devSize.width * 0.7f, devSize.height * 0.2f);
	tex = createTexture(size.width, size.height);
	fbo->bind(tex);
	t = createImage("assets/PlayerUI/StageNumBanner.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);


	img = new iImage();
	iStrTex* st = new iStrTex(methodStStageNum);
	st->setString("%d", 0);
	stStageNum = st;
	img->addObject(st->tex);

	pop->addObject(img);
	iPoint p = iPointMake((devSize.width - size.width)/2.0f, (devSize.height - size.height) / 2.0f);
	pop->openPosition = iPointMake(p.x, -img->tex->height);
	pop->closePosition = iPointMake(p.x, p.y);
	
	popStageNum = pop;
}

Texture* methodStStageNum(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum);
	int stage = atoi(line[0]) + 1;
	iString::freeStringLine(line, lineNum);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(devSize.width * 0.7f, devSize.height * 0.17f);
	g->init(size);

	setStringSize(devSize.height * 0.1f);
	setStringRGBA(1, 1, 0, 1);
	setStringBorder(2);
	setStringBorderRGBA(0, 0, 0, 1);
	g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, "Stage : %d", stage);

	return g->getTexture();
}

void freePopStageNum()
{
	delete popStageNum;
	delete stStageNum;
}

void showPopStageNum(bool show)
{
	popStageNum->show(show);
	if(show)
		passMap->popDt = 0.0f;
}

void drawPopStageNum(float dt)
{
	if (popStageNum->bShow)
	{
		passMap->popDt += dt;
		if (passMap->popDt > passMap->_popDt)
		{
			passMap->popDt = passMap->_popDt;
			showPopStageNum(false);
		}
	}

	popStageNum->paint(dt);

	stStageNum->setString("%d", st->stageNum);
}

bool keyPopStageNum(iKeyState stat, iPoint point)
{
	return false;
}

//-----------------------------------------------------------
// UI
iPopup* popState;
iStrTex* stState;
iStrTex* stDisplayStageNum;

iImage** imgState;
Texture* methodStState(const char* str);
Texture* methodStDisplayStageNum(const char* str);

void createPopState()
{
	int i, j;
	iImage* img;
	Texture* tex, *t;
	iPopup* pop = new iPopup(iPopupStyleNone);
	iSize size;

	const char* strIcon[2] = {
		"assets/PlayerUI/heart.png",
		"assets/PlayerUI/tmp.png",
	};

	iPoint p = iPointMake(devSize.width * 0.025f, devSize.height * 0.025f);
	size = iSizeMake(devSize.width * 0.05f, devSize.width * 0.05f);
	for (i = 0; i < 2; i++) // icon
	{
		img = new iImage();
		tex = createTexture(size.width, size.height);

		fbo->bind(tex);
		t = createImage(strIcon[i]);
		DRAWIMAGE(t, size);
		freeImage(t);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);

		//size = iSizeMake(devSize.width * 0.2f, devSize.height * 0.07f);
		img->position = iPointMake(p.x, p.y + size.height * 1.1f * i);
		pop->addObject(img);
	}

	p = iPointMake(p.x * 3.0f, p.y * 2.0f);
	size = iSizeMake(size.width * 4, size.height * 0.5f);
	for (i = 0; i < 2; i++) // bar
	{
		img = new iImage();
		tex = createTexture(size.width, size.height);

		fbo->bind(tex);
		t = createImage("assets/PlayerUI/Bar_empty.png");
		DRAWIMAGE(t, size);
		freeImage(t);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);
		img->position = iPointMake(p.x, p.y + size.height * 2.0f * 1.1f * i);
		pop->addObject(img);
	}
	
	imgState = (iImage**)malloc(sizeof(iImage*) * 2);
	const char* strBar[2] = {
		"assets/PlayerUI/Bar_heart.png",
		"assets/PlayerUI/Bar_tmp.png",
	};

	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		tex = createTexture(size.width, size.height);

		fbo->bind(tex);
		t = createImage(strBar[i]);
		DRAWIMAGE(t, size);
		freeImage(t);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);

		img->position = iPointMake(p.x, p.y + size.height * 2.0f * 1.1f * i);
		imgState[i] = img;

		pop->addObject(img);
	}

	img = new iImage();
	iStrTex* st = new iStrTex(methodStState);
	st->setString("%d\n%d", 0, 0);
	img->addObject(st->tex);
	img->position = iPointMake(p.x * 1.2f, p.y * 0.3f);
	stState = st;
	pop->addObject(img);


	img = new iImage();
	size = iSizeMake(devSize.width * 0.2f, devSize.height * 0.04f);
	p = iPointMake(devSize.width - size.width * 1.1f, size.height * 0.2f);
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	t = createImage("assets/PlayerUI/StageNumBanner.png");
	DRAWIMAGE(t, size);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	img->position = p;
	pop->addObject(img);


	img = new iImage();
	st = new iStrTex(methodStDisplayStageNum);
	st->setString("%d", 0);
	img->addObject(st->tex);
	img->position = p;
	stDisplayStageNum = st;
	pop->addObject(img);

	popState = pop;
}

Texture* methodStState(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum);
	int hp = atoi(line[0]);
	int _hp = atoi(line[1]);
	iString::freeStringLine(line, lineNum);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(devSize.width * 0.15f, devSize.width * 0.025f);

	g->init(size);

	g->fillRect(0, 0, size.width, size.height,5);

	setStringRGBA(0, 0, 0, 1);
	setStringSize(size.height * 0.7f);
	setStringBorder(0);
	g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, "HP : %d / %d", hp, _hp);

	return g->getTexture();
}

Texture* methodStDisplayStageNum(const char* str)
{
	int stage = atoi(str);

	iGraphics* g = iGraphics::instance();
	iSize size = iSizeMake(devSize.width * 0.2f, devSize.height * 0.04f);
	g->init(size);

	setStringSize(size.height * 0.7f);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(0);
	g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, "Stage: %d", stage);

	return g->getTexture();
}

void freePopState()
{
	delete popState;
	delete stState;
	delete stDisplayStageNum;
	free(imgState);
}

void showPopState(bool show)
{
	popState->show(show);
}

void drawPopState(float dt)
{
	popState->paint(dt);

#if 1
	imgState[0]->texCoordx = player->hp / player->_hp;
	imgState[1]->texCoordx = player->hp / player->_hp;
#endif

	stState->setString("%.0f\n%.0f", player->hp, player->_hp);
	stDisplayStageNum->setString("%d", st->stageNum + 1);
}

bool keyPopState(iKeyState stat, iPoint point)
{
	return false;
}

//-----------------------------------------------------------
// ProcButton
iPopup* popProcButton;
iImage** imgProcButtonBtn;

iImage* imgInvenWeaponBtn;

void drawPopProcButtonBefore(iPopup* me, iPoint p, float dt);
#define ProcButton_NUM 1
void createPopProcButton()
{
	int i, j;
	iImage* img;
	Texture* tex, *t;
	iPopup* pop = new iPopup(iPopupStyleMove);
	iSize size;


	//menu, inven
	imgProcButtonBtn = (iImage**)malloc(sizeof(iImage*) * ProcButton_NUM);

	const char* strPath[ProcButton_NUM] = {
		"assets/PlayerUI/MenuBtn.png",

	};

	iSize sizes[ProcButton_NUM] = {
		{devSize.width * 0.2f, devSize.height * 0.067f},
	};

	iPoint imgPos[ProcButton_NUM] = {
		{sizes[0].width * 0.1f, devSize.height - sizes[0].height * 1.5f},
	};

	for (i = 0; i < ProcButton_NUM; i++)
	{
		img = new iImage();

		for (j = 0; j < 2; j++)
		{
			size = sizes[i];
			tex = createTexture(size.width, size.height);

			fbo->bind(tex);
			t = createImage(strPath[i]);
			if (j == 0) setRGBA(1, 1, 1, 1);
			else		setRGBA(0, 1, 0, 1);

			if (i == 0)
			{
				drawImage(t, 0, size.height / 2.0f,
					t->width * 0.5f, 0, t->width, t->height,
					VCENTER | LEFT, size.width / (t->width * 0.5f), size.height / t->height, 
					2, 0, REVERSE_HEIGHT);
			}
			else
				DRAWIMAGE(t, size);
			freeImage(t);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		img->position = imgPos[i];
		imgProcButtonBtn[i] = img;
		pop->addObject(img);
	}
	setRGBA(1, 1, 1, 1);

	img = new iImage();
	size = sizes[1] * 0.7f;
	tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	Weapon* w = (Weapon*)player->arrayWeapon->objectAtIndex(player->currWeaponIndex(), false);
	t = w->img->tex;
	DRAWIMAGE(t, size);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	
	img->position = imgPos[1] + iPointMake(sizes[1].width * 0.15f, sizes[1].height * 0.15f);
	imgInvenWeaponBtn = img;
	pop->addObject(img);

	pop->methodDrawBefore = drawPopProcButtonBefore;
	popProcButton = pop;
}

void freePopProcButton()
{
	delete popProcButton;
	free(imgProcButtonBtn);
}

void showPopProcButton(bool show)
{
	popProcButton->show(show);
}

void drawPopProcButtonBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < ProcButton_NUM; i++)
		imgProcButtonBtn[i]->setTexAtIndex(i == popProcButton->selected);

#if 0
	Weapon* w = (Weapon*)player->arrayWeapon->objectAtIndex(player->currWeaponIndex(), false);
	Texture* t = w->img->tex;
	iSize size = iSizeMake(100, 100);
	float r = min(size.width / t->width, size.height / t->height);

	fbo->bind(imgInvenWeaponBtn->tex);
	fbo->clear(0, 0, 0, 0);
	drawImage(t, size.width / 2.0f, size.height / 2.0f,
		0, 0, t->width, t->height,
		VCENTER | HCENTER, r, r,
		2, 0, REVERSE_HEIGHT);
	fbo->unbind();
#endif
}

void drawPopProcButton(float dt)
{
	popProcButton->paint(dt);
}

bool keyPopProcButton(iKeyState stat, iPoint point)
{
	if (popProcButton->bShow == false)
		return false;

	if (popProcButton->stat != iPopupStatProc)
		return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popProcButton->selected;
		if (i == -1)
		{
			audioPlay(AUDIO_MenuSelected);
			break;
		}

		if (i == 0)
		{
			showPopProcMenu(true);
		}

		audioPlay(AUDIO_MenuSelected);
		break;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < ProcButton_NUM; i++)
		{
			if (containPoint(point, imgProcButtonBtn[i]->touchRect(popProcButton->closePosition)))
			{
				j = i;
				break;
			}
		}
		
		if (popProcButton->selected != j && j != -1)
			audioPlay(AUDIO_MenuMouseOver);

		popProcButton->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}

	return true;
}

//-----------------------------------------------------------
// MiniMap
iPopup* popMiniMap;
iPopup* popMiniMapState;
iImage* imgMiniMap;

#define MINIMAP_SIZE 10
void refreshMiniMap(Texture* tex)
{
	fbo->bind(tex);
	fbo->clear(0, 0, 0, 0);

	setLineWidth(10);
	for (int i = 0; i < TILE_TOTAL_NUM; i++)
	{
		if (maps[i]->tileIndex == -1)
			continue;

		iPoint p = iPointMake(TILE_Width * (i % TILE_TOTAL_SQRT), TILE_Height * (i / TILE_TOTAL_SQRT));
		setRGBA(0, 0, 0, 1);
		fillRect(p.x, p.y, TILE_Width, TILE_Height);

		if (i == player->mapNumber) setRGBA(0, 1, 0, 1);
		else						setRGBA(1, 1, 1, 1);

		fillRect(p.x + 1, p.y + 1, TILE_Width - 2, TILE_Height - 2 , 5);

		if (maps[i]->state == MapType_ItemBox)
		{
			setRGBA(0, 0, 1, 0.7f);
			fillRect(p.x + 3, p.y + 3, TILE_Width - 6, TILE_Height - 6, 5);
		}
		else if (maps[i]->state == MapType_Boss)
		{
			setRGBA(1, 0, 0, 0.7f);
			fillRect(p.x + 3, p.y + 3, TILE_Width - 6, TILE_Height - 6, 5);
		}
	}
	setRGBA(1, 1, 1, 1);
	setLineWidth(1);

	fbo->unbind();
}

void createPopMiniMap()
{
	int i, j;
	iImage* img;
	Texture* tex;
	iPopup* pop = new iPopup(iPopupStyleMove);
	
	img = new iImage();
	iSize size = iSizeMake(TILE_Width * TILE_TOTAL_SQRT, TILE_Height * TILE_TOTAL_SQRT);
	tex = createTexture(size.width, size.height);
	refreshMiniMap(tex);
	img->addObject(tex);
	freeImage(tex);

	img->reverse = REVERSE_HEIGHT;
	img->ratio = 2.0f;
	imgMiniMap = img;
	pop->addObject(img);

	pop->openPosition = 
	pop->closePosition = iPointMake(devSize.width - size.width * img->ratio - 50, 50);
	popMiniMap = pop;

	const char* strState[2] = {
		"Boss", "ItemBox"
	};
	pop = new iPopup(iPopupStyleNone);
	iGraphics* g = iGraphics::instance();
	size = iSizeMake(120,100);
	setStringSize(20);
	setStringRGBA(0, 0.7f, 0, 1);
	setStringBorder(0);
	g->init(size);
	g->fillRect(0, 0, size.width, size.height, 5);
	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		if (i == 0) setRGBA(1, 0, 0, 1);
		else		setRGBA(0, 0, 1, 1);
		g->fillRect(5, 5 + 50 * i, 40, 40, 3);
		g->drawString(size.width * 0.95f, 25 + 50 * i, VCENTER | RIGHT, strState[i]);
	}
	setRGBA(1, 1, 1, 1);
	tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	pop->openPosition =
	pop->closePosition = iPointMake(devSize.width - size.width * img->ratio - 50, 50)
						+ iPointMake(0, (TILE_Height + 20) * TILE_TOTAL_SQRT);
	popMiniMapState = pop;
}

void freePopMiniMap()
{
	delete popMiniMap;
	delete popMiniMapState;
}

void showPopMiniMap(bool show)
{
	popMiniMap->show(show);
	popMiniMapState->show(show);
}

void drawPopMiniMap(float dt)
{
	iImage* img = imgMiniMap;
	refreshMiniMap(img->tex);
	if (getKeyDown(keyboard_tab))
	{
		if (img->ratio == 2.0f)
		{
			img->ratio = 4.0f;
			iPoint p = iPointMake(img->tex->width, img->tex->height) * img->ratio;
			popMiniMap->closePosition = iPointMake((devSize.width - p.x) / 2.0f, (devSize.height - p.y) / 2.0f);
		}
		else
		{
			img->ratio = 2.0f;
			popMiniMap->closePosition = iPointMake(devSize.width - img->tex->width * img->ratio - 50, 50);
		}
	}

	popMiniMap->paint(dt);
	popMiniMapState->paint(dt);
}

bool keyPopMiniMap(iKeyState stat, iPoint point)
{
	int i, j= -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{

		break;
	}
	case iKeyStateMoved:
	{


		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}

	return false;
}

//-----------------------------------------------------------
// Inven
iPopup* popInven = NULL;
iImage** imgInvenBtn;
iImage** imgInvenWeapon;
bool invenOpen;
#define POPINVEN_DT 1.0f

void drawPopInvenBefore(iPopup* me, iPoint p, float dt);

void createPopInven()
{
	int i, j;
	iImage* img;
	Texture* tex, *t, *text;
	iPopup* pop = new iPopup(iPopupStyleMove);
	iSize size, s;

	imgInvenBtn = (iImage**)malloc(sizeof(iImage*) * 8);
	imgInvenWeapon = (iImage**)malloc(sizeof(iImage*) * 8);

	// 키를 누르면 오른쪽에서 왼쪽으로 열리면서 팝업이 띄워짐
	// 현재 무기가 바뀌거나 버려졌을때 새로 이미지를 그림
	// 
	const char* strPath = "assets/PlayerUI/WeaponBtn.png";

	size = iSizeMake(devSize.width * 0.1f, devSize.width * 0.1f);
	s = size * 0.8f;
	iPoint p;
	for (i = 0; i < 8; i++)
	{
		p = iPointMake(size.width * i, 0);
		
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.width, size.height);

			fbo->bind(tex);
			t = createImage(strPath);
			if (j == 0)setRGBA(1, 1, 1, 1);
			else setRGBA(0, 1, 0, 1);
			DRAWIMAGE(t, size);
			freeImage(t);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}
		setRGBA(1, 1, 1, 1);
		img->position = p;
		imgInvenBtn[i] = img;
		pop->addObject(img);

		//
		img = new iImage();
		tex = createTexture(s.width, s.height);

		fbo->bind(tex);
		t = createImage(strPath);
		DRAWIMAGE(t, s);
		freeImage(t);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);
		img->position = p + iPointMake(size.width * 0.1f, size.height * 0.1f);
		imgInvenWeapon[i] = img;
		pop->addObject(img);
	}
	
	pop->openPosition = iPointMake(devSize.width - size.width, devSize.height - size.height);
	pop->closePosition = iPointMake(devSize.width - size.width, devSize.height - size.height);
	pop->methodDrawBefore = drawPopInvenBefore;

	pop->bShow = true;
	pop->stat = iPopupStatProc;
	pop->showDt =
	pop->_showDt = POPINVEN_DT;

	popInven = pop;

	invenOpen = true;
	drawPopInvenRefreshWeapon();
}

void freePopInven()
{
	delete popInven;
	free(imgInvenBtn);
	free(imgInvenWeapon);
}

void showPopInven(bool show)
{
	//popInven->show(show);
	iPopup* pop = popInven;
	if (pop->showDt < pop->_showDt)
		return;

	popInven->showDt = 0.0f;
	invenOpen = show;

	iSize size = iSizeMake(devSize.width * 0.1f, devSize.width * 0.1f);
	int num = player->arrayWeapon->count;
	if (invenOpen)
	{
		popInven->openPosition = iPointMake(devSize.width - size.width, devSize.height - size.height);
		popInven->closePosition = iPointMake(devSize.width - size.width * num, devSize.height - size.height);
	}
	else
	{
		popInven->openPosition = iPointMake(devSize.width - size.width * num, devSize.height - size.height);
		popInven->closePosition = iPointMake(devSize.width - size.width, devSize.height - size.height);
	}
}

void drawPopInvenRefreshWeapon()
{
	if (popInven == NULL)
		return;

	int i, n, num = player->arrayWeapon->count;
	Texture* tex, *t;
	Weapon* w;
	iSize size = iSizeZero;
	for (i = 0; i < num; i++)
	{
		n = player->arrayWeapon->currIndex - i;
		if (n < 0)
			n += num;

		w  = (Weapon*)player->arrayWeapon->objectAtIndex(n, false);
		t = w->img->tex;
		tex = imgInvenWeapon[i]->tex;
		size = iSizeMake(tex->width, tex->height);
		float min = min(size.width / t->width, size.height / t->height);
		fbo->bind(tex);
		fbo->clear(0, 0, 0, 0);
		drawImage(t, size.width / 2.0f, size.height / 2.0f,
			0, 0, t->width, t->height,
			VCENTER | HCENTER, min, min,
			2, 0, REVERSE_HEIGHT);
		fbo->unbind();
	}

	if (popInven->showDt < popInven->_showDt)
		return;

	size = iSizeMake(devSize.width * 0.1f, devSize.width * 0.1f);
	if (invenOpen)
	{
		popInven->openPosition = iPointMake(devSize.width - size.width * num, devSize.height - size.height);
	}
	else
	{
		popInven->closePosition = iPointMake(devSize.width - size.width * num, devSize.height - size.height);
	}
}

void drawPopInvenBefore(iPopup* me, iPoint p, float dt)
{
	int num = player->arrayWeapon->count;
	for (int i = 0; i < num; i++)
		imgInvenBtn[i]->setTexAtIndex(i == popInven->selected);	
}

void drawPopInven(float dt)
{
	iPopup* pop = popInven;
	iPoint p = pop->closePosition;
	if (pop->showDt < pop->_showDt)
	{
		pop->showDt += dt;
		float d = pop->showDt / pop->_showDt;
		if (d > 1.0f)
		{
			d = 1.0f;
			p = pop->closePosition;
			pop->closePosition = pop->openPosition;
			pop->openPosition = p;
		}
		else
		{
			pop->closePosition = linear(d, pop->closePosition, pop->openPosition);
		}
	}
	
	pop->paint(dt);

	if (pop->showDt < pop->_showDt)
		pop->closePosition = p;

	if (getKeyDown(keyboard_r))
	{
		if (invenOpen)	showPopInven(false);
		else			showPopInven(true);
	}
}

bool keyPopInven(iKeyState stat, iPoint point)
{
	if (popInven->bShow == false)
		return false;

	if (popInven->stat != iPopupStatProc)
		return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popInven->selected;
		if (i == -1)
		{
			audioPlay(AUDIO_MenuSelected);
			break;
		}

		int n = player->arrayWeapon->currIndex - i;
		if (n < 0)
			n += player->arrayWeapon->count;

		player->selectWeapon(n);

		popInven->selected = -1;
		audioPlay(AUDIO_MenuSelected);
		return true;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < 8; i++)
		{
			if (containPoint(point, imgInvenBtn[i]->touchRect(popInven->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popInven->selected != j && j != -1)
			audioPlay(AUDIO_MenuMouseOver);

		popInven->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}


	return true;
}

//-----------------------------------------------------------
// ProcMenu
iPopup* popProcMenu;
iImage** imgProcMenu;

void drawPopProcMenuBefore(iPopup* me, iPoint p, float dt);
#define ProcMenuBtn_NUM 4
void createPopProcMenu()
{
	int i, j;
	iImage* img;
	Texture* tex, *t;
	iPopup* pop = new iPopup(iPopupStyleZoom);
	iSize size;


	img = new iImage();
	iSize frameSize = iSizeMake(devSize.width * 0.4f, devSize.height * 0.7f);
	tex = createTexture(frameSize.width, frameSize.height);

	fbo->bind(tex);
	t = createImage("assets/menu/Menu panel.png");
	DRAWIMAGE(t, frameSize);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	imgProcMenu = (iImage**)malloc(sizeof(iImage*) * ProcMenuBtn_NUM);
	const char* strPath[ProcMenuBtn_NUM] = {
		"assets/menu/New game Button%d.png",
		"assets/menu/Continue Button%d.png",
		"assets/menu/Settings Button%d.png",
		"assets/menu/Quit Button%d.png",
	};

	size = iSizeMake(frameSize.width * 0.8f ,frameSize.height * 0.15f);
	for (i = 0; i < ProcMenuBtn_NUM; i++)
	{
		img = new iImage();
		
		for (j = 0; j < 2; j++)
		{
			tex = createTexture(size.width, size.height);
			fbo->bind(tex);
			t = createImage(strPath[i], j);
			DRAWIMAGE(t, size);
			freeImage(t);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		img->position = iPointMake(frameSize.width * 0.1f, size.height * 1.5f + size.height * 1.1f * i);
		imgProcMenu[i] = img;
		pop->addObject(img);
	}
	

	//pop->openPosition = iPointMake((devSize.width - frameSize.width) / 2.0f, -frameSize.height);
	pop->openPosition = imgProcButtonBtn[0]->position;
	pop->closePosition = iPointMake((devSize.width - frameSize.width) / 2.0f, (devSize.height - frameSize.height) / 2.0f);
	pop->methodDrawBefore = drawPopProcMenuBefore;
	popProcMenu = pop;
}

void freePopProcMenu()
{
	delete popProcMenu;
	free(imgProcMenu);
}

void showPopProcMenu(bool show)
{
	popProcMenu->show(show);
}

void drawPopProcMenuBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < ProcMenuBtn_NUM; i++)
		imgProcMenu[i]->setTexAtIndex(i == popProcMenu->selected);
}

void drawPopProcMenu(float dt)
{
	popProcMenu->paint(dt);
}

bool keyPopProcMenu(iKeyState stat, iPoint point)
{
	if (popProcMenu->bShow == false)
		return false;

	if (popProcMenu->stat != iPopupStatProc)
		return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popProcMenu->selected;
		if (i == -1)
		{
			audioPlay(AUDIO_MenuSelected);
			break;
		}

		if (i == 0)
		{
			// new Game
			isNewGame = true;
			setLoading(gamestat_proc, freeProc, loadProc);

		}
		else if (i == 1)
		{
			// 재개
			showPopProcMenu(false);
		}
		else if (i == 2)
		{
			// 옵션
			showPopOption(true);
		}
		else if (i == 3)
		{
			//나가기
			setLoading(gamestat_intro, freeProc, loadIntro);
			audioStop(AUDIO_GameMusic);
		}
		audioPlay(AUDIO_MenuSelected);
		return true;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < ProcMenuBtn_NUM; i++)
		{
			if (containPoint(point, imgProcMenu[i]->touchRect(popProcMenu->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popProcMenu->selected != j && j != -1)
			audioPlay(AUDIO_MenuMouseOver);

		popProcMenu->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}

	return true;
}

//-----------------------------------------------------------
// GameOver
iPopup* popGameOver;
iImage** imgGameOverBtn;

void drawPopGameOverBefore(iPopup* me, iPoint p, float dt);

void createPopGameOver()
{
	int i, j;
	iImage* img;
	Texture* tex;
	iPopup* pop = new iPopup(iPopupStyleMove);
	
	img = new iImage();
	iGraphics* g = iGraphics::instance();
	iSize size;
	iSize frameSize = iSizeMake(512, 512);
	g->init(frameSize);

	setRGBA(0, 1, 0, 1);
	g->fillRect(0, 0, frameSize.width, frameSize.height, 20);
	setRGBA(1, 1, 1, 1);

	setStringRGBA(0, 0, 0, 1);
	setStringSize(40);
	setStringBorder(0);
	g->drawString(frameSize.width / 2.0f, 32, VCENTER | HCENTER, "Game Over");

	tex = g->getTexture();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);


	imgGameOverBtn = (iImage**)malloc(sizeof(iImage*) * 2);
	const char* strBtn[2] = {
		"Menu", "Exit"
	};
	size = iSizeMake(312, 128);
	for (i = 0; i < 2; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
			{
				setRGBA(0, 0.5f, 0.5f, 1);
				setStringRGBA(1, 1, 1, 1);
			}
			else
			{
				setRGBA(1, 1, 1, 1);
				setStringRGBA(0, 0, 0, 1);
			}
			g->fillRect(0, 0, size.width, size.height, 5);
			setRGBA(1, 1, 1, 1);

			setStringSize(50);
			setStringBorder(0);
			g->drawString(size.width / 2.0f, size.height / 2.0f, VCENTER | HCENTER, strBtn[i]);

			tex = g->getTexture();
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake((frameSize.width- size.width) * 0.5f, 80 + size.height * 1.2 * i);
		imgGameOverBtn[i] = img;
		pop->addObject(img);
	}

	pop->openPosition = iPointMake((devSize.width - frameSize.width) / 2.0f, -128);
	pop->closePosition = iPointMake((devSize.width - frameSize.width) / 2.0f, (devSize.height - frameSize.height) / 2.0f);
	pop->methodDrawBefore = drawPopGameOverBefore;
	popGameOver = pop;
}

void freePopGameOver()
{
	delete popGameOver;
	free(imgGameOverBtn);
}

void showPopGameOver(bool show)
{
	popGameOver->show(show);
}

void drawPopGameOverBefore(iPopup* me, iPoint p, float dt)
{
	for (int i = 0; i < 2; i++)
		imgGameOverBtn[i]->setTexAtIndex(i == popGameOver->selected);
}

void drawPopGameOver(float dt)
{
	popGameOver->paint(dt);
}

bool keyPopGameOver(iKeyState stat, iPoint point)
{
	if (popGameOver->bShow == false)
		return false;

	//if(popGameOver->stat != iPopupStatProc)
	//	return false;

	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{
		i = popGameOver->selected;
		if (i == -1)
		{
			audioPlay(AUDIO_MenuSelected);
			break;
		}

		if (i == 0)
		{
			// main
			showPopGameOver(false);
			setLoading(gamestat_intro, freeProc, loadIntro);
		}
		else if (i == 1)
		{
			//exit
			showPopGameOver(false);
			runWnd = false;
		}
		audioPlay(AUDIO_MenuSelected);
		audioStop(AUDIO_GameMusic);
		break;
	}
	case iKeyStateMoved:
	{
		for (i = 0; i < 2; i++)
		{
			if (containPoint(point, imgGameOverBtn[i]->touchRect(popGameOver->closePosition)))
			{
				j = i;
				break;
			}
		}

		if (popProcMenu->selected != j && j != -1)
			audioPlay(AUDIO_MenuMouseOver);

		popGameOver->selected = j;

		break;
	}
	case iKeyStateEnded:
	{

		break;
	}
	}

	return true;
}
