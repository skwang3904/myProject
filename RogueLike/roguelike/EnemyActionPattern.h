#pragma once

#include "iStd.h"

struct EnemyGolem;

bool MethodFalse(EnemyGolem* enm, float dt);

//----------------------------------------------------------------------------------------
// idle

void IdleEyeBlink(EnemyGolem* enm, float dt);

//----------------------------------------------------------------------------------------
// hurt

void commonHurt(EnemyGolem* enm, float dt);

//----------------------------------------------------------------------------------------
// walk

void WalkToPlayer(EnemyGolem* enm, float dt);

//----------------------------------------------------------------------------------------
// attack

bool commonAttack(EnemyGolem* enm, float dt);
bool rangeAttack(EnemyGolem* enm, float dt);
