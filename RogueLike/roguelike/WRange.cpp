#include "WRange.h"

#include "Weapon.h"

void rangeWeapon::init(
	const char* info, 
	iImage* iImg, 
	float iAttackDmg, 
	float iAttackSpeed, 
	float iWidthReach, 
	float iHeightReach, 
	float iHoldAngle)
{
	infoImg = infoFromMW(info);
	infomation = info;
	img = iImg;

	attackDmg = iAttackDmg;
	attackSpeed = _attackSpeed = iAttackSpeed;
	widthReach = iWidthReach;
	heightReach = iHeightReach;
	holdAngle = iHoldAngle;

	combatPosition = iPointZero;
	hitBox = iRectZero;
	attackEnemy = false;
}

