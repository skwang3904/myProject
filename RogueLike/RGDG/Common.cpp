#include "Common.h"

#include "Map.h"
#include "ProcData.h"
#include "PlayerChar.h"
#include "Monster.h"
#include "Weapon.h"
#include "Item.h"

//----------------------------------------------------------------------------

Object::Object(int index, int8 mapNum, iPoint pos)
{
	this->index = index;
	alive = false;

	imgNum = 0;
	imgs = NULL;
	img = NULL;
	this->mapNumber = mapNum;

	this->position = pos;
	vector = iPointZero;
	touchRect = iRectZero;

	prevHp = 0.0f;
	hp = _hp = 0.0f;
	attackPoint = _attackPoint = 0.0f;
	attackSpeed = _attackSpeed = 0.0f;
	actionDt = _actionDt = 0.0f;

	moveSpeed = 0.0f;
}

Object::~Object()
{
	for (int i = 0; i < imgNum; i++)
		delete imgs[i];
	free(imgs);
}

//----------------------------------------------------------------------------
// Stage
Stage* st = NULL;
#define FILE_PATH "save.sav"
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
		int a = -1;
		if (i == 0)
			a = 1;
		else if (i == 1)
			a = 0;
		weaponData[i].index = a;
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

	for (i = 0; i < MT_max; i++)
	{
		int n = 0;
		switch (i)
		{
		case MT_golemNomal: n = 5 + random() % 10; break;
		case MT_golemBoss: n = 1; break;
		default: break;
		}
		actMonsterNum[i] = n;
	}

	num = actMonsterNum[MT_golemNomal];
	for (i = 0; i < num; i++)
	{
		MonsterData* md = &monsterData[i];
		md->index = 0;
		md->mapNum = am[random() % cnNum];
		md->position = iPointMake(100 + 100 * (random() % 5), 150 + 50 * (random() % 10));
	}

	//num += actMonsterNum[golemBoss];
	for (i = 0; i < cnNum; i++)
	{
		if (maps[am[i]]->state == MapType_Boss)
		{
			MonsterData* md = &monsterData[actMonsterNum[MT_golemNomal]];
			md->index = 0;
			md->mapNum = am[i];
			md->position = iPointMake(450, 450);
		}
	}
}

void loadStage()
{
	int i, length;
	char* buf = loadFile(FILE_PATH, length);
	if (buf)
	{
		st = (Stage*)buf;
		createMap(true);
	}
	else
	{
		createMap(false);

		st = (Stage*)calloc(sizeof(Stage), 1);
		st->create();
		saveStage();
	}

	createMapImage();
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
	nextDt = _nextDt = STAGE_LOADING_DT;
	center = iPointMake(devSize.width, devSize.height) / 2.0f;

	passDt = _passDt = PASS_DT;
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
	case 1:					p = curr->tileOff + iPointMake(TILE_NUM_X * TILE_Width, 0);  break;
	case -1:				p = curr->tileOff + iPointMake(-TILE_NUM_X * TILE_Width, 0);		break;
	case TILE_TOTAL_SQRT:	p = curr->tileOff + iPointMake(0, TILE_NUM_Y * TILE_Height); break;
	case -TILE_TOTAL_SQRT:	p = curr->tileOff + iPointMake(0, -TILE_NUM_Y * TILE_Height);	break;
	default: printf("pass tile error\n");
	}

	passDt += dt;
	if (passDt > _passDt)
	{
		passDt = _passDt;
		prevMapNumber = mapNumber;
	}

	float d = passDt / _passDt;
	iPoint pp = linear(d, curr->tileOff, prev->tileOff) + DRAW_OFF;
	iPoint cp = linear(d, p, curr->tileOff) + DRAW_OFF;

	setRGBA(0.3f, 0.3f, 0.3f, 1);
	if (passDt < _passDt)
		prev->img->paint(dt, pp);
	curr->img->paint(dt, cp);
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
			createMap();
			st->setStageData();
			createMapImage();
			loadPlayerChar();
			loadMonster();
			loadItem();
		}

		d = 1.0f - nextDt / nd;
	}
	else if (nextDt == nd)
	{
		nextDt += 0.00001f;
	}
	else if( nextDt < _nextDt)
	{
		nextDt += dt;
		if (nextDt > _nextDt)
		{
			nextDt = _nextDt;
			init();
		}
		d = nextDt / nd - 1.0f;
	}

#if 1
	float len = iPointLength(iPointMake(devSize.width / 2.0f, devSize.height / 2.0f));
	setRGBA(0, 0, 0, 1);
	fillCircle(center, len * d, true);
	setRGBA(1, 1, 1, 1);

#else
	//fillCircle(center, (devSize.width / 2.0f) * d, false);
	setLineWidth(10);
	drawCircle(center, (devSize.width / 2.0f) * d);
	setLineWidth(1);

#endif
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
