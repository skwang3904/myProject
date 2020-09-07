#include "Monster.h"

#include "Tile.h"
#include "Map.h"
#include "ProcData.h"
#include "PlayerChar.h"

//---------------------------------------------------------------------------------------
// monster parents class
Monster** monster;
int monsterNum;

Monster* cbMonster = NULL;
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
	alive = false;
	prevHp = 0.0f;
	hp = _hp = 0.0f;
	attackPoint =  _attackPoint = 0.0f;
	attackSpeed =  _attackSpeed = 0.0f;
	attackDt = _attackDt = 0.0f;
	attackDelay =  _attackDelay = 0.0f;
	actionDt = _actionDt = 0.0f;

	moveSpeed = 0.0f;
	lookDistance = 0.0f;
	meleeDistance = 0.0f;
	rangeDistance = 0.0f;

	distance = 0.0f;
	reverse = REVERSE_NONE;

	showHpDt = _showHpDt = 0.0f;
}

Monster::~Monster()
{
}

void Monster::drawShadow(float dt, iPoint off)
{
	iSize size = iSizeMake(img->tex->width, img->tex->height);
	iPoint p = position + iPointMake(size.width / 2.0f, size.height * 0.6f) + off;
	setRGBA(0, 0, 0, 0.3f);
	fillEllipse(p.x, p.y, size.width * 0.5f, size.height * 0.25f);
	setRGBA(1, 1, 1, 1);
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
int golemNomalNum;

GolemNomal::GolemNomal(int index, int8 mapNum, iPoint pos) : Monster(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex;

	imgNum = MONSTER_IMG_NUM;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);
	iSize size = iSizeMake(200, 150);
	for (i = 0; i < imgNum; i++)
	{
		MonsterImageInfo* gni = &golemNomalImage[i];
		img = new iImage();

		for (j = 0; j < gni->imgNum; j++)
		{
			tex = createTexture(size.width, size.height);

			fbo->bind(tex);
			Texture* t = createImage(gni->strPath, j);
			drawImage(t, size.width / 2.0f, size.height / 2.0f, 
				0,0, t->width, t->height,
				VCENTER | HCENTER, size.width / t->width, size.height / t->height,
				2, 0, REVERSE_HEIGHT);
			freeImage(t);
			fbo->unbind();

			img->addObject(tex);
			freeImage(tex);
		}

		img->_aniDt = gni->aniDt;
		img->_repeatNum = gni->repeatNum;
		img->animation = (gni->repeatNum == 0 ? true : false);
		img->lastFrame = gni->lastFrame;
		img->position = iPointMake(0, -size.height * 0.33f);
		imgs[i] = img;
	}

	this->img = imgs[0];
	mapNumber = mapNum;

	position = maps[mapNumber]->tileOff + pos;
	vector = iPointZero;

	size = iSizeMake(this->img->tex->width, this->img->tex->height);
	touchRect = iRectMake(position, size);


	alive = true;
	state = monster_idle;
	MonsterInfo* mi = &monsterInfo[GOLEM_NOMAL];
	prevHp = 0.0f;
	hp = _hp = mi->_hp;
	attackPoint = _attackPoint = mi->_attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = mi->_attackSpeed;
	attackDt = 0.0f;
	_attackDt = mi->_attackDt;
	attackDelay = 0.0f;
	_attackDelay = mi->_attackDelay;
	_actionDt = mi->_actionDt;

	moveSpeed = mi->moveSpeed;
	lookDistance = mi->lookDistance;
	meleeDistance = mi->meleeDistance;
	rangeDistance = mi->rangeDistance;

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

//---------------------------------------------------------------------------------------
// golemBoss
int golemBossNum;
// #bug
GolemBoss::GolemBoss(int index, int8 mapNum, iPoint pos) : Monster(index, mapNum, pos)
{
	int i, j;
	iImage* img;
	Texture* tex;

	// 4 dir Image
	imgNum = MONSTER_IMG_NUM * 4;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);
	iSize size = iSizeMake(378, 300);
	char c[4] = { 'l', 'r', 't', 'd' };
	setRGBA(1, 1, 1, 1);
	for (i = 0; i < imgNum; i++)
	{
		img = new iImage();

		MonsterImageInfo* gbi = &golemBossImage[i / 4];
		int num = gbi->imgNum / 4;
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

		img->_aniDt = gbi->aniDt;
		img->_repeatNum = gbi->repeatNum;
		img->animation = (gbi->repeatNum == 0 ? true : false);
		img->lastFrame = gbi->lastFrame;
		img->position = iPointMake(0, -size.height * 0.33f);
		imgs[i] = img;
	}
	this->img = imgs[0];
	mapNumber = mapNum;

	position = maps[mapNumber]->tileOff + pos;
	vector = iPointZero;

	size = iSizeMake(this->img->tex->width, this->img->tex->height);
	touchRect = iRectMake(position, size);


	alive = true;
	state = monster_idle;
	MonsterInfo* mi = &monsterInfo[GOLEM_BOSS];
	prevHp = 0.0f;
	hp = _hp = mi->_hp;
	attackPoint = _attackPoint = mi->_attackPoint;
	//attackSpeed = 0.0f;
	_attackSpeed = mi->_attackSpeed;
	//attackDt = 0.0f;
	_attackDt = mi->_attackDt;
	//attackDelay = 0.0f;
	_attackDelay = mi->_attackDelay;
	_actionDt = mi->_actionDt;

	moveSpeed = mi->moveSpeed;
	lookDistance = mi->lookDistance;
	meleeDistance = mi->meleeDistance;
	rangeDistance = mi->rangeDistance;

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
	}
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------


void loadMonster()
{
	int i, j;

	monsterNum = 0;
	monster = (Monster**)malloc(sizeof(Monster*) * 30);

	golemNomalNum = GOLEM_NOMAL_NUM;
	for (i = 0; i < golemNomalNum; i++)
	{
		monster[monsterNum] = new GolemNomal(0, 0, iPointMake(300 + 100 * i, 300 + 200 * i));
		monsterNum++;
	}

	golemBossNum = GOLEM_BOSS_NUM;
	for (i = 0; i < TILE_TOTAL_NUM; i++)
	{
		if (maps[i]->state == MapType_Boss)
		{
			monster[monsterNum] = new GolemBoss(0, i, iPointMake(300 , 300));
			monsterNum++;
			break;
		}
	}
}

void freeMonster()
{
	int num = golemNomalNum + golemBossNum;
	for (int i = 0; i < num; i++)
		delete monster[i];
	free(monster);
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
