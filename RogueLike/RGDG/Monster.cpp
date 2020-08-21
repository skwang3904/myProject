#include "Monster.h"

Monster::Monster(int index) : Object(index)
{

}

Monster::~Monster()
{
}

void Monster::paint(float dt, iPoint off)
{
}

//---------------------------------------------------------------------------------------
// golemNomal

GolemNomal** _golemNomal = NULL;
GolemNomal** golemNomal = NULL;
int golemNomalNum = 0;

GolemNomal::GolemNomal(int index) : Monster(index)
{

}

GolemNomal::~GolemNomal()
{
}

void GolemNomal::paint(float dt, iPoint off)
{
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
