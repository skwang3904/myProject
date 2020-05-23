#include "iPoint.h"

#include "iStd.h"
#include <math.h>

bool iPoint::operator == (const iPoint& p)
{
	return (x == p.x && y == p.y);
}

bool iPoint::operator != (const iPoint& p)
{
	return (x != p.x || y != p.y);
}

iPoint iPoint::operator + (const iPoint& p)
{
	iPoint r;
	r.x = x + p.x;
	r.y = y + p.y;
	return r;
}

iPoint& iPoint::operator += (const iPoint& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

iPoint& iPoint::operator += (const float f)
{
	x = x + f;
	y = y + f;
	return *this;
}

iPoint iPoint::operator - (const iPoint& p)
{
	iPoint r;
	r.x = x - p.x;
	r.y = y - p.y;
	return r;
}

iPoint& iPoint::operator -= (const iPoint& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

iPoint iPoint::operator*(const iPoint& p)
{
	iPoint r;
	r.x = x * p.x;
	r.y = y * p.y;
	return r;
}

iPoint& iPoint::operator *= (const iPoint& p)
{
	x *= p.x;
	y *= p.y;
	return *this;
}

iPoint iPoint::operator * (const float f)
{
	iPoint r;
	r.x = x * f;
	r.y = y * f;
	return r;
}

iPoint& iPoint::operator *= (const float f)
{
	x *= f;
	y *= f;
	return *this;
}

iPoint& iPoint::operator /= (const iPoint& p)
{
	x /= p.x;
	y /= p.y;
	return *this;
}

iPoint iPoint::operator / (const float f)
{
	iPoint r;
	r.x = x / f;
	r.y = y / f;
	return r;
}

iPoint& iPoint::operator /= (const float f)
{
	x /= f;
	y /= f;
	return *this;
}

iPoint iPointMake(float x, float y)
{
	iPoint r;
	r.x = x;
	r.y = y;
	return r;
}

iPoint iPointVector(iPoint p)
{
	return p / sqrtf(p.x * p.x + p.y * p.y);
}

float iPointLength(iPoint p)
{
	return sqrtf(p.x * p.x + p.y * p.y);
}

iPoint iPointRotate(const iPoint& p, const iPoint& t, float degree)
{
	// 점 t를 기준점으로 p를 degree만큼 시계방향 회전
	while (degree < 0)		degree += 360;
	while (degree > 360)	degree -= 360;
	float sinr = _sin(degree);
	float cosr = _cos(degree);
#if 0// bottom|left - math
	iPoint r;
	r.x = t.x + (p.x - t.x) * cosr - (p.y - t.y) * sir;
	r.y = t.y + (p.y - t.y) * sinr + (p.y - t.y) * cosr;
#else// top|left
	iPoint r = iPointMake(p.x - t.x, t.y - p.y);
	iPoint _p;
	_p.x = r.x * cosr - r.y * sinr;
	_p.y = r.x * sinr + r.y * cosr;

	_p.x = t.x + _p.x;
	_p.y = t.y - _p.y;
	return _p;
#endif
}


float iPointAngle(iPoint& v1, iPoint& v2, iPoint& v3)
{
	iPoint v1p = v1 - v2;
	iPoint v3p = v3 - v2;

	float v1pLen = iPointLength(v1p);
	float v3pLen = iPointLength(v3p);

	float dot = v1p.x * v3p.x + v1p.y * v3p.y;
	float x = dot / (v1pLen * v3pLen);

	if (v3p.y > v1p.y)
	{
		if (dot == 0.0f)
			return 270.0f;
		return 360 - (acos(x) * 180 / M_PI);
	}

	if (dot == 0.0f)
		return 90.0f;
	return acos(x) * 180 / M_PI;
}