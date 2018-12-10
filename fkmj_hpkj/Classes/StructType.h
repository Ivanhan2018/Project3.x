#pragma once

#include "cocos2d.h"

struct RandFloat
{
	RandFloat(float fValue)
		:fMin(fValue)
		,fMax(fValue)
	{}
	RandFloat()
		:fMin(0)
		,fMax(0)
	{}
	std::string kUserKey;
	float fMin;
	float fMax;
};

struct RandPos
{
	RandPos()
	{}
	RandPos(cocos2d::Vec2 kPos)
	{
		kRandX = RandFloat(kPos.x);
		kRandY = RandFloat(kPos.y);
	}
	std::string kUserKey;
	RandFloat kRandX;
	RandFloat kRandY;
};
struct UserString
{
	UserString()
	{

	}
	std::string kUserKey;
	std::string kUserValue;
};