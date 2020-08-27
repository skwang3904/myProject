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

	state = monster_idle;
	alive = false;
	hp = _hp = 0.0f;
	attackPoint =  _attackPoint = 0.0f;
	attackSpeed =  _attackSpeed = 0.0f;
	moveSpeed = 0.0f;

	attackDt = 0.0f;
	_attackDt = 0.0f;
	lookDistance = 0.0f;
	meleeDistance = 0.0f;
	rangeDistance = 0.0f;

	distance = 0.0f;
}

Monster::~Monster()
{
}

//---------------------------------------------------------------------------------------
// golemNomal

GolemNomal** _golemNomal = NULL;
int golemNomalNum;

GolemNomal::GolemNomal(int index) : Monster(index)
{
	iImage* img;
	Texture* tex;
	imgNum = 6;
	imgs = (iImage**)malloc(sizeof(iImage*) * imgNum);
	iSize size = iSizeMake(200, 150);
	for (int i = 0; i < imgNum; i++)
	{
		img = new iImage();
		tex = createTexture(size.width, size.height, false);
		
		fbo->bind(tex);
		fbo->clear(1.0f - 0.2f * i, 0, 0.2f * i, 1);
		fbo->unbind();

		img->addObject(tex);
		freeImage(tex);

		imgs[i] = img;
	}

	this->img = imgs[0];
	mapNumber = 0;

	position = iPointMake(300, 300);
	vector = iPointZero;

	touchRect = iRectZero;



	alive = true;
	MonsterInfo* mi = &monsterInfo[0];
	hp = _hp = mi->_hp;
	attackPoint = _attackPoint = mi->_attackPoint;
	//attackSpeed = 0.0f;
	_attackSpeed = mi->_attackSpeed;
	moveSpeed = mi->moveSpeed;

	//attackDt = 0.0f;
	_attackDt = mi->_attackDt;
	lookDistance = mi->lookDistance;
	meleeDistance = mi->meleeDistance;
	rangeDistance = mi->rangeDistance;

}

GolemNomal::~GolemNomal()
{

}

void GolemNomal::paint(float dt, iPoint off)
{
	if (mapNumber != player->mapNumber)
		return;

	if (hp <= 0.0f)
	{
		state = monster_death;
		//start death ani
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

				//else
				{
					if (distance < lookDistance)
						state = monster_move;
					else
						state = monster_idle;
				}
			}
		}
	}

	(this->*method[state])(dt);

	iPoint p = position + off;
	img->paint(dt, p);
}

void GolemNomal::getDmg(float dmg)
{
	hp -= dmg;
	
	if( state != monster_meleeAttack)
		state = monster_hurt;
}

void GolemNomal::actionIdle(float dt)
{
	//printf("called method idle\n");

	img = imgs[0];
}

void GolemNomal::actionMove(float dt)
{
	//printf("called method move\n");
	img = imgs[1];

	position += vector * (moveSpeed * dt);

}

void GolemNomal::actionMeleeAttack(float dt)
{
	if (imgs[2]->animation)
	{
		attackDt += dt;
		if (attackDt > _attackDt)
		{
			// player->dmg
			printf("hit\n");
			img->animation = false;
		}
		return;
	}

	if (attackSpeed < _attackSpeed)
	{
		attackSpeed += dt;
		return;
	}

	img = imgs[2];

#if 0
	img->startAnimation();
#endif
	img->animation = true;

	attackDt = 0.0f;
	attackSpeed -= _attackSpeed;
}

void GolemNomal::actionRangeAttack(float dt)
{
	printf("called method rangeAttack\n");

	img = imgs[3];

}

void GolemNomal::actionHurt(float dt)
{
	printf("called method hurt\n");

	img = imgs[4];

}

void GolemNomal::actionDeath(float dt)
{
	printf("called method death\n");

	img = imgs[5];

}

void GolemNomal::initOtherAct(int index)
{
	for (int i = 0; i < imgNum; i++)
	{
		if (index == i) continue;
		imgs[i]->frame = 0;
	}
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

	golemNomalNum = GOLEM_NOMAL_NUM;
	_golemNomal = (GolemNomal**)malloc(sizeof(GolemNomal*) * golemNomalNum);
	for (int i = 0; i < golemNomalNum; i++)
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
		m->paint(dt, DRAW_OFF);

		if (m->alive == false)
		{ // death
			monsterNum--;
			monster[i] = monster[monsterNum];
			i--;
		}
	}
}