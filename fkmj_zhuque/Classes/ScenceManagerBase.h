#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"

#include "IStringMessageSink.h"

class ScenceManagerBase
	:public FvSingletonBase<ScenceManagerBase>
{
public:
	ScenceManagerBase();
	~ScenceManagerBase();
public:
	virtual void GameBackScence() = 0;
};