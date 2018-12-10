#pragma once
#include "cocos2d.h"
#include "TimeManager.h"

class TimeNode
{
public:
	TimeNode();
	~TimeNode();
public:
	void useTimeNode();
private:
	bool m_bUseTimeNode;
};

