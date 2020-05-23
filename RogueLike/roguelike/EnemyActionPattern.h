#pragma once

#include "iStd.h"

struct EnemyGolem;

//----------------------------------------------------------------------------------------
// idle

void IdleEyeBlink(EnemyGolem* enm, float dt);

//----------------------------------------------------------------------------------------
// walk

void WalkToPlayer(EnemyGolem* enm, float dt);

//----------------------------------------------------------------------------------------
// attack

bool commonAttack(EnemyGolem* enm, float dt);
bool rangeAttack(EnemyGolem* enm, float dt);
