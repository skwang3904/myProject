#include "Monster.h"

#include "PlayerChar.h"
#include "Map.h"
#include "ProcData.h"

Monster** monster;
int monsterNum;

//---------------------------------------------------------------------------------------
// monster parents class
Monster::Monster(int index) : Object(index)
{
	method[0] = &Monster::actionIdle;
	method[1] = &Monster::actionMove;
	method[2] = &Monster::actionMeleeAttack;
	method[3] = &Monster::actionRangeAttack;
	method[4] = &Monster::actionHurt;
	method[5] = &Monster::actionDeath;
}

Monster::~Monster()
{
}

//---------------------------------------------------------------------------------------
// golemNomal

GolemNomal** _golemNomal = NULL;

GolemNomal::GolemNomal(int index) : Monster(index)
{
	img = new iImage();

	Texture* t = createImage("assets/monster/golem1/idle/Golem_01_Idle_0.png");
	iSize size = iSizeMake(200, 150);
	Texture* tex = createTexture(size.width, size.height);

	fbo->bind(tex);
	drawImage(t, 0, 0,
		0, 0, t->width, t->height,
		TOP | LEFT, size.width / t->width, size.height / t->height,
		2, 0, REVERSE_HEIGHT);
	freeImage(t);
	fbo->unbind();

	img->addObject(tex);
	freeImage(tex);


	state = monster_idle;
	
	alive = true;

	MonsterInfo* mi = &monsterInfo[0];
	hp, _hp = mi->_hp;
	attackPoint = _attackPoint = mi->_attackPoint;
	attackSpeed = 0.0f;
	_attackSpeed = mi->_attackSpeed;
	moveSpeed = mi->moveSpeed;

	//common data
	//imgNum
	//imgs
	//img
	mapNumber = 0;

	position = iPointMake(300, 300);
	vector = iPointZero;
}

GolemNomal::~GolemNomal()
{
	delete img;
}

void GolemNomal::paint(float dt, iPoint off)
{
	if (getKeyDown(keyboard_j))
	{
		int n = random() % 6;
		(this->*method[n])();
	}
	
	if (hp <= 0.0f)
	{
		state = monster_death;
	}
	else
	{

	}

	(this->*method[state])();

	iPoint p = position + off + DRAW_OFF;
	img->paint(dt, p);
}

void GolemNomal::actionIdle()
{
	printf("called method idle\n");
}

void GolemNomal::actionMove()
{
	printf("called method move\n");
}

void GolemNomal::actionMeleeAttack()
{
	printf("called method meleeAttack\n");
}

void GolemNomal::actionRangeAttack()
{
	printf("called method rangeAttack\n");
}

void GolemNomal::actionHurt()
{
	printf("called method hurt\n");
}

void GolemNomal::actionDeath()
{
	printf("called method death\n");
}

//---------------------------------------------------------------------------------------
// golemElete

GolemElete::GolemElete(int index) : Monster(index)
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

GolemBoss::GolemBoss(int index) : Monster(index)
{
}

GolemBoss::~GolemBoss()
{
}

void GolemBoss::paint(float dt, iPoint off)
{
}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------


void loadMonster()
{
	monsterNum = 0;
	monster = (Monster**)malloc(sizeof(Monster*) * GOLEM_NOMAL_NUM);

	_golemNomal = (GolemNomal**)malloc(sizeof(GolemNomal*) * GOLEM_NOMAL_NUM);
	for (int i = 0; i < GOLEM_NOMAL_NUM; i++)
	{
		_golemNomal[i] = new GolemNomal(0);

		monster[monsterNum] = _golemNomal[i];
		monsterNum++;
	}

}

void freeMonster()
{
	free(monster);

	for (int i = 0; i < GOLEM_NOMAL_NUM; i++)
		delete _golemNomal[i];
	free(_golemNomal);
}

void drawMonster(float dt)
{
	for (int i = 0; i < monsterNum; i++)
	{
		Monster* m = monster[i];
		m->paint(dt, iPointZero);

		if (m->alive == false)
		{ // death
			monsterNum--;
			monster[i] = monster[monsterNum];
			i--;
		}
	}
}
