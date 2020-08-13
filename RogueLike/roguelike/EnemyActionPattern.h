#pragma once

#include "iStd.h"


class MonsterData;
//bool MethodFalse(EnemyGolem* enm, float dt);
//
////-----------------------------------------------------------------
//// idle
//
//void IdleEyeBlink(EnemyGolem* enm, float dt);
//
////-----------------------------------------------------------------
//// hurt
//
//void commonHurt(EnemyGolem* enm, float dt);
//
////-----------------------------------------------------------------
//// walk
//
//void WalkToPlayer(EnemyGolem* enm, float dt);
//
////-----------------------------------------------------------------
//// attack
//
//bool commonAttack(EnemyGolem* enm, float dt);
//bool rangeAttack(EnemyGolem* enm, float dt);

bool MethodFalse(MonsterData* enm, float dt);

//-----------------------------------------------------------------
// idle

void IdleEyeBlink(MonsterData* enm, float dt);
void IdleBoss(MonsterData* enm, float dt);


//-----------------------------------------------------------------
// hurt

void commonHurt(MonsterData* enm, float dt);
void commonHurtBoss(MonsterData* enm, float dt);

//-----------------------------------------------------------------
// walk

void WalkToPlayer(MonsterData* enm, float dt);
void WalkBossToPlayer(MonsterData* enm, float dt);


//-----------------------------------------------------------------
// attack

bool commonAttack(MonsterData* enm, float dt);
bool commonAttackBoss(MonsterData* enm, float dt);

//bool rangeAttack(MonsterData* enm, float dt);
