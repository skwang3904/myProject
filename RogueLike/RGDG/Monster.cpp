#include "Monster.h"

#include "PlayerChar.h"
#include "Map.h"

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
GolemNomal** golemNomal = NULL;
int golemNomalNum = 0;

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

	position = iPointMake(300, 300);
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
	golemNomalNum = 0;
	golemNomal = (GolemNomal**)malloc(sizeof(GolemNomal*) * GOLEM_NOMAL_NUM);

	_golemNomal = (GolemNomal**)malloc(sizeof(GolemNomal*) * GOLEM_NOMAL_NUM);
	for (int i = 0; i < GOLEM_NOMAL_NUM; i++)
	{
		_golemNomal[i] = new GolemNomal(0);

		golemNomal[golemNomalNum] = _golemNomal[i];
		golemNomalNum++;
	}

}

void freeMonster()
{
	for (int i = 0; i < GOLEM_NOMAL_NUM; i++)
		delete _golemNomal[i];
	free(_golemNomal);
	free(golemNomal);
}

void drawMonster(float dt)
{
	for (int i = 0; i < golemNomalNum; i++)
		golemNomal[i]->paint(dt, iPointZero);
}