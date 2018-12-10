#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"

class EffectScence
	:public FvSingleton<EffectScence> 
	,public cocos2d::Node
{
public:
	EffectScence();
	~EffectScence();
	
	 bool init();
public:
	void ShowGoldEffect();
};