#include "Monster.h"

#include "Proc.h"
#include "Tile.h"
#include "Map.h"
#include "ProcData.h"
#include "PlayerChar.h"
#include "Item.h"

//---------------------------------------------------------------------------------------
// monster parents class
Monster** _monster = NULL;
Monster** monster = NULL;
int monsterNum;

iImage** golemNomal_Image = NULL;
iImage** golemBoss_Image = NULL;
Monster::Monster(int index, int8 mapNum, iPoint pos) : Object(index, mapNum, pos)
{
	stateMethod[0] = &Monster::actionIdle;
	stateMethod[1] = &Monster::actionMove;
	stateMethod[2] = &Monster::actionMeleeAttack;
	stateMethod[3] = &Monster::actionRangeAttack;
	stateMethod[4] = &Monster::actionHurt;
	stateMethod[5] = &Monster::actionDeath;

	cbMethod = NULL;

	state = monster_idle;

	attackDt = _attackDt = 0.0f;
	attackDelay =  _attackDelay = 0.0f;

	lookDistance = 0.0f;
	meleeDistance = 0.0f;
	rangeDistance = 0.0f;

	distance = 0.0f;
	reverse = REVERSE_NONE;

	showHpDt = _showHpDt = 0.0f;

	itemDropNum = 0;
	itemTypeKindNum = 0;
	memset(itemTypeKind, 0x00, sizeof(itemTypeKind));
}

Monster::~Monster()
{
}

void Monster::initOtherAct(int index)
{
	int num = imgNum;
	for (int i = 0; i < num; i++)
	{
		if (i == index)
		{
			switch (index)
			{
			case monster_idle: 
			case monster_move: 
			{
				attackDt = 0.0f;
				attackDelay = 0.0f;
				break;
			}
			case monster_meleeAttack: break;
			case monster_rangeAttack: break;
			case monster_hurt: 
			{
				attackDt = 0.0f;
				attackDelay = 0.0f;
				break;
			}
			case monster_death: 
			{
				attackDt = 0.0f;
				attackDelay = 0.0f;
				break;
			}
			default: 
				break;
			}
			continue;
		}
		imgs[i]->frame = 0;
	}
}

void Monster::cbMonsterSetIdle()
{
	state = monster_idle;
	initOtherAct(monster_idle);

	cbMethod = NULL;
}

void Monster::cbMonsterSetAliveFalse()
{
	alive = false;

	cbMethod = NULL;
	//item drop
	// tmp(itemIndex, itemNum);
}

void Monster::showHPbar()
{
	showHpDt = 0.0f;
}

void Monster::drawHPbar(float dt, iPoint p)
{
	if (showHpDt < _showHpDt)
	{
		showHpDt += dt;

		float h = linear(min(showHpDt / _showHpDt * 2.0f,1.0f), prevHp, hp);
		setRGBA(0, 0, 0, 1);
		fillRect(p.x, p.y + 30, img->tex->width, 20);
		setRGBA(1, 0, 0, 1);
		fillRect(p.x, p.y + 30, img->tex->width * (prevHp / _hp), 20);
		setRGBA(0.5f, 1, 0, 1);
		fillRect(p.x, p.y + 30, img->tex->width * (h / _hp), 20);
		setRGBA(1, 1, 1, 1);

		if (showHpDt >= _showHpDt)
			prevHp = hp;
	}
}

//---------------------------------------------------------------------------------------
// golemNomal

GolemNomal::GolemNomal(int index, int8 mapNum, iPoint pos) : Monster(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex;
	iSize size;
	int stage = st->stageNum;


	imgNum = MONSTER_IMG_NUM;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);
	if (golemNomal_Image == NULL)
		golemNomal_Image = (iImage**)calloc(sizeof(iImage*), imgNum);
	
	for (i = 0; i < imgNum; i++)
	{
		MonsterImageInfo* gni = &golemNomalImage[i];
		size = gni->size;
		if (golemNomal_Image[i] == NULL)
		{
			img = new iImage();
			for (j = 0; j < gni->imgNum; j++)
			{
				tex = createTexture(size.width, size.height);

				fbo->bind(tex);
				Texture* t = createImage(gni->strPath, j);
				drawImage(t, size.width / 2.0f, size.height / 2.0f,
					0, 0, t->width, t->height,
					VCENTER | HCENTER, size.width / t->width, size.height / t->height,
					2, 0, REVERSE_HEIGHT);
				freeImage(t);
				fbo->unbind();

				img->addObject(tex);
				freeImage(tex);
			}

			golemNomal_Image[i] = img;
		}

		img = golemNomal_Image[i];
		img->_aniDt = gni->aniDt + gni->_aniDt / gni->imgNum * stage;
		img->_repeatNum = gni->repeatNum;
		img->animation = (gni->repeatNum == 0 ? true : false);
		img->lastFrame = gni->lastFrame;
		img->position = iPointMake(0, -size.height * 0.33f);

		imgs[i] = golemNomal_Image[i]->copy();
	}

	this->img = imgs[0];

	position = maps[mapNumber]->tileOff + position;
	vector = iPointZero;

	size = iSizeMake(this->img->tex->width, this->img->tex->height);
	touchRect = iRectMake(position, size);


	alive = true;
	state = monster_idle;
	MonsterInfo* mi = &monsterInfo[MT_golemNomal];
	prevHp = hp = _hp = mi->setMonsterStatus(&mi->hp, stage);
	attackPoint = _attackPoint = mi->setMonsterStatus(&mi->attackPoint, stage);
	attackSpeed = 0.0f;
	_attackSpeed = mi->setMonsterStatus(&mi->attackSpeed, stage);
	attackDt = 0.0f;
	_attackDt = mi->setMonsterStatus(&mi->attackDt, stage);
	attackDelay = 0.0f;
	_attackDelay = mi->setMonsterStatus(&mi->attackDelay, stage);
	_actionDt = mi->setMonsterStatus(&mi->actionDt, stage);

	moveSpeed = mi->setMonsterStatus(&mi->moveSpeed, stage);
	lookDistance = mi->setMonsterStatus(&mi->lookDistance, stage);
	meleeDistance = mi->setMonsterStatus(&mi->meleeDistance, stage);
	rangeDistance = mi->setMonsterStatus(&mi->rangeDistance, stage);

	itemDropNum = mi->itemDropNum;
	itemTypeKindNum = mi->itemTypeKindNum;
	for (i = 0; i < itemTypeKindNum; i++)
		itemTypeKind[i] = mi->itemTypeKind[i];

	distance = 0.0f;
	reverse = REVERSE_NONE;

	showHpDt = 0.0f;
	_showHpDt = 2.0f;

}

GolemNomal::~GolemNomal()
{

}

void GolemNomal::paint(float dt, iPoint off)
{
	if (mapNumber != player->mapNumber)
		return;

	if (actionDt < _actionDt)
		actionDt += dt;

	drawShadow(dt, off);

	if (hp <= 0.0f)
	{
		state = monster_death;
		// start death ani
		// alive false = after death ani

	}
	else
	{
		// 공격 받았을때 hurt
		if (state == monster_hurt)
		{
			
		}
		else
		{
			iPoint pp = player->position + iPointMake(player->img->tex->width, player->img->tex->height) * 0.5f;
			iPoint ep = position + iPointMake(img->tex->width, img->tex->height) * 0.5f;
			vector = iPointVector(pp - ep);
			distance = iPointLength(pp - ep);
			reverse = (vector.x > 0 ? REVERSE_NONE : REVERSE_WIDTH);
			if (distance < meleeDistance)
			{
				state = monster_meleeAttack;
			}
			else
			{
				if (state == monster_meleeAttack)
				{
					;
				}
				else
				{
					if (distance < lookDistance)
						state = monster_move;
					else
						state = monster_idle;
				}
			}
		}
	}

	(this->*stateMethod[state])(dt);

	touchRect = iRectMake(position.x, position.y, img->tex->width, img->tex->height);
	img->reverse = reverse;
	iPoint p = position + off;
	img->paint(dt, p);

	drawHPbar(dt, p);

	if (cbMethod)
	{
		if (img->animation == false)
			(this->*cbMethod)();
	}
}

void GolemNomal::drawShadow(float dt, iPoint off)
{
	iSize size = iSizeMake(img->tex->width, img->tex->height);
	iPoint p = position + iPointMake(size.width / 2.0f, size.height * 0.6f) + off;
	setRGBA(0, 0, 0, 0.3f);
	fillEllipse(p.x, p.y, size.width * 0.5f, size.height * 0.25f);
	setRGBA(1, 1, 1, 1);
}

void GolemNomal::action(Object* obj)
{
	if (actionDt < _actionDt)
		return;

	actionDt = 0.0f;
	prevHp = hp;
	hp -= obj->attackPoint;
	if (hp < 0.0f)
		hp = 0.0f;

	if (state != monster_meleeAttack)
		state = monster_hurt;

	showHPbar();
}

void GolemNomal::actionIdle(float dt)
{
	img = imgs[0];
}

void GolemNomal::actionMove(float dt)
{
	img = imgs[1];

	iPoint mp = vector * (moveSpeed * dt);
	wallCheck(this, mp);
}

void GolemNomal::actionMeleeAttack(float dt)
{
	if (imgs[2]->animation)
	{
		if (attackDt < _attackDt)
		{
			attackDt += dt;
		}
		else if (attackDt > _attackDt)
		{
			// player->dmg
			if (distance < meleeDistance)
				player->action(this);

			attackDt = _attackDt;
		}

		return;
	}

	if (attackDelay < _attackDelay)
	{
		attackDelay += dt;
		img = imgs[2];

		return;
	}

	img->startAnimation();
	cbMethod = &Monster::cbMonsterSetIdle;

	attackDt = 0.0f;
	attackDelay = 0.0f;
}

void GolemNomal::actionRangeAttack(float dt)
{
	img = imgs[3];
}

void GolemNomal::actionHurt(float dt)
{
	//from getDmg

	img = imgs[4];
	if (imgs[4]->animation)
	{

	}
	else
	{
		img->startAnimation();
		cbMethod = &Monster::cbMonsterSetIdle;
	}
}


void GolemNomal::actionDeath(float dt)
{
	img = imgs[5];
	if (imgs[5]->animation)
	{

	}
	else
	{
		img->startAnimation();
		cbMethod = &Monster::cbMonsterSetAliveFalse;
		dropMonsterItem(this);
	}
}

//---------------------------------------------------------------------------------------
// golemElete
GolemElete::GolemElete(int index, int8 mapNum, iPoint pos) : Monster(index, mapNum, pos)
{
}

GolemElete::~GolemElete()
{
}

void GolemElete::paint(float dt, iPoint off)
{
}

void GolemElete::drawShadow(float dt, iPoint off)
{
}

//---------------------------------------------------------------------------------------
// golemBoss

GolemBoss::GolemBoss(int index, int8 mapNum, iPoint pos) : Monster(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex;
	iSize size;
	int stage = st->stageNum;

	// 4 dir Image
	imgNum = MONSTER_IMG_NUM * 4;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);
	if (golemBoss_Image == NULL)
		golemBoss_Image = (iImage**)calloc(sizeof(iImage*), imgNum);

	char c[4] = { 'l', 'r', 't', 'd' };
	for (i = 0; i < imgNum; i++)
	{
		MonsterImageInfo* gbi = &golemBossImage[i / 4];
		int num = gbi->imgNum / 4;
		size = gbi->size;
		if (golemBoss_Image[i] == NULL)
		{
			img = new iImage();

			for (j = 0; j < num; j++)
			{
				tex = createTexture(size.width, size.height);

				fbo->bind(tex);
				Texture* t = createImage(gbi->strPath, c[i % 4], j);
				drawImage(t, size.width / 2.0f, size.height / 2.0f,
					0, 0, t->width, t->height,
					VCENTER | HCENTER, size.width / t->width, size.height / t->height,
					2, 0, REVERSE_HEIGHT);
				freeImage(t);
				fbo->unbind();

				img->addObject(tex);
				freeImage(tex);
			}

			golemBoss_Image[i] = img;
		}

		img = golemBoss_Image[i];
		img->_aniDt = gbi->aniDt + gbi->_aniDt / num * stage;
		img->_repeatNum = gbi->repeatNum;
		img->animation = (gbi->repeatNum == 0 ? true : false);
		img->lastFrame = gbi->lastFrame;
		img->position = iPointMake(0, -size.height * 0.33f);

		imgs[i] = golemBoss_Image[i]->copy();
	}

	this->img = imgs[0];

	position = maps[mapNumber]->tileOff + position;
	vector = iPointZero;

	size = iSizeMake(this->img->tex->width, this->img->tex->height);
	touchRect = iRectMake(position, size);


	alive = true;
	state = monster_idle;
	MonsterInfo* mi = &monsterInfo[MT_golemBoss];
	prevHp = hp = _hp =			 mi->setMonsterStatus(&mi->hp, stage);
	attackPoint = _attackPoint = mi->setMonsterStatus(&mi->attackPoint, stage);
	attackSpeed = 0.0f;								  
	_attackSpeed =				 mi->setMonsterStatus(&mi->attackSpeed, stage);
	attackDt = 0.0f;								  
	_attackDt =		 			 mi->setMonsterStatus(&mi->attackDt, stage);
	attackDelay = 0.0f;								  
	_attackDelay =				 mi->setMonsterStatus(&mi->attackDelay, stage);
	_actionDt =					 mi->setMonsterStatus(&mi->actionDt, stage);
								 					  
	moveSpeed =					 mi->setMonsterStatus(&mi->moveSpeed, stage);
	lookDistance =				 mi->setMonsterStatus(&mi->lookDistance, stage);
	meleeDistance =				 mi->setMonsterStatus(&mi->meleeDistance, stage);
	rangeDistance =				 mi->setMonsterStatus(&mi->rangeDistance, stage);

	itemDropNum = mi->itemDropNum;
	itemTypeKindNum = mi->itemTypeKindNum;
	for (i = 0; i < itemTypeKindNum; i++)
		itemTypeKind[i] = mi->itemTypeKind[i];

	distance = 0.0f;
	reverse = REVERSE_NONE;

	showHpDt = 0.0f;
	_showHpDt = 2.0f;
	dir = 0;
}

GolemBoss::~GolemBoss()
{
}

void GolemBoss::paint(float dt, iPoint off)
{
	//if (mapNumber != player->mapNumber)
		//return;

	drawShadow(dt, off);

	if (state < monster_meleeAttack)
	{
		float angle = iPointAngle(vector, iPointZero, iPointMake(1, 0));
		if (angle > 315 || angle < 45)	{ dir = 1; }
		else if (angle > 45  && angle < 135)	{ dir = 2; }
		else if (angle > 135 && angle < 225)	{ dir = 0; }
		else if (angle > 225 && angle < 315)	{ dir = 3; }
	}


	if (actionDt < _actionDt)
		actionDt += dt;

	if (hp <= 0.0f)
	{
		state = monster_death;
		// start death ani
		// alive false = after death ani

	}
	else
	{
		// 공격 받았을때 hurt
		if (state == monster_hurt)
		{

		}
		else
		{
			iPoint pp = player->position + iPointMake(player->img->tex->width, player->img->tex->height) * 0.5f;
			iPoint ep = position + iPointMake(img->tex->width, img->tex->height) * 0.5f;
			vector = iPointVector(pp - ep);
			distance = iPointLength(pp - ep);
			//reverse = (vector.x > 0 ? REVERSE_NONE : REVERSE_WIDTH);
			if (distance < meleeDistance)
			{
				state = monster_meleeAttack;
			}
			else
			{
				if (state == monster_meleeAttack)
				{
					;
				}
				else
				{
					if (distance < lookDistance)
						state = monster_move;
					else
						state = monster_idle;
				}
			}
		}
	}

	(this->*stateMethod[state])(dt);

	touchRect = iRectMake(position.x, position.y, img->tex->width, img->tex->height);
	//img->reverse = reverse;
	iPoint p = position + off;
	img->paint(dt, p);

	drawHPbar(dt, p);

	if (cbMethod)
	{
		if (img->animation == false)
			(this->*cbMethod)();
	}
}

void GolemBoss::drawShadow(float dt, iPoint off)
{
	iSize size = iSizeMake(img->tex->width, img->tex->height);
	iPoint p = position + iPointMake(size.width / 2.0f, size.height * 0.6f) + off;
	setRGBA(0, 0, 0, 0.3f);
	fillEllipse(p.x, p.y, size.width * 0.5f, size.height * 0.25f);
	setRGBA(1, 1, 1, 1);
}

void GolemBoss::action(Object* obj)
{
	if (actionDt < _actionDt)
		return;

	actionDt = 0.0f;
	prevHp = hp;
	hp -= obj->attackPoint;
	if (hp < 0.0f)
		hp = 0.0f;

	if (state != monster_meleeAttack)
		state = monster_hurt;

	showHPbar();
}

void GolemBoss::actionIdle(float dt)
{
	img = imgs[0 + dir];
}

void GolemBoss::actionMove(float dt)
{
	img = imgs[4 + dir];

	iPoint mp = vector * (moveSpeed * dt);
	wallCheck(this, mp);
}

void GolemBoss::actionMeleeAttack(float dt)
{
	if (imgs[8 + dir]->animation)
	{
		if (attackDt < _attackDt)
		{
			attackDt += dt;
		}
		else if (attackDt > _attackDt)
		{
			// player->dmg
			if(distance < meleeDistance)
				player->action(this);

			attackDt = _attackDt;
		}

		return;
	}

	if (attackDelay < _attackDelay)
	{
		attackDelay += dt;
		img = imgs[8 + dir];

		return;
	}

	img->startAnimation();
	cbMethod = &Monster::cbMonsterSetIdle;

	attackDt = 0.0f;
	attackDelay = 0.0f;
}

void GolemBoss::actionRangeAttack(float dt)
{
	img = imgs[12 + dir];
}

void GolemBoss::actionHurt(float dt)
{
	img = imgs[16 + dir];
	if (imgs[16 + dir]->animation)
	{

	}
	else
	{
		img->startAnimation();
		cbMethod = &Monster::cbMonsterSetIdle;
	}
}

void GolemBoss::actionDeath(float dt)
{
	img = imgs[20 + dir];
	if (imgs[20 + dir]->animation)
	{

	}
	else
	{
		img->startAnimation();
		cbMethod = &Monster::cbMonsterSetAliveFalse;
		MapObjectNextDoor::setNextDoor(this);
		dropMonsterItem(this);
	}
}

//---------------------------------------------------------------------------------------

void loadMonster()
{
	int i, j;

	int num = 0;
	num += st->actMonsterNum[MT_golemNomal];
	num += st->actMonsterNum[MT_golemBoss];

	monsterNum = 0;
	_monster = (Monster**)malloc(sizeof(Monster*) * num);
	monster = (Monster**)malloc(sizeof(Monster*) * num);


	int index = 0;
	int8 mapNum = 0;
	iPoint pos = iPointZero;
	for (i = 0; i < num; i++)
	{
		MonsterData* md = &st->monsterData[i];
		index = md->index;
		mapNum = md->mapNum;
		pos = md->position;
		
		if (i < st->actMonsterNum[MT_golemNomal])
		{
			_monster[i] = new GolemNomal(index, mapNum, pos);
			monster[monsterNum] = _monster[i];
			monsterNum++;
		}
		else if (i < st->actMonsterNum[MT_golemNomal] + st->actMonsterNum[MT_golemBoss])
		{
			for (j = 0; j < TILE_TOTAL_NUM; j++)
			{
				if (maps[j]->state == MapType_Boss)
				{
					mapNum = j;
					md->mapNum = j;
					_monster[i] = new GolemBoss(index, mapNum, pos);
					monster[monsterNum] = _monster[i];
					monsterNum++;
					break;
				}
			}
		}
	}
}

void freeMonsterImage()
{
	int i, num;

	num = MONSTER_IMG_NUM;
	for (i = 0; i < num; i++)
		delete golemNomal_Image[i];
	free(golemNomal_Image);
	golemNomal_Image = NULL;
	
	num = MONSTER_IMG_NUM * 4;
	for (i = 0; i < num; i++)
		delete golemBoss_Image[i];
	free(golemBoss_Image);
	golemBoss_Image = NULL;
}

void freeMonster()
{
	int i, num = 0;
	for (i = 0; i < MT_max; i++)
		num += st->actMonsterNum[i];

	for (i = 0; i < num; i++)
		delete _monster[i];
	free(_monster);
	_monster = NULL;
	free(monster);
	monster = NULL;

	freeMonsterImage();
}

void drawMonster(float dt)
{
	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		m->paint(dt, DRAW_OFF);

		if (m->alive == false)
		{ // death
			monsterNum--;
			monster[i] = monster[monsterNum];
			i--;
		}
	}
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------