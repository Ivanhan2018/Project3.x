#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "MissionWeiXin.h"
#include "CGPGameRecord.h"

class HNHelpScence
	:public cocos2d::Node
	,public FvSingleton<HNHelpScence> 
{
public:
	HNHelpScence();
	~HNHelpScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();
public:
	void Button_BangZhu(cocos2d::Ref*,WidgetUserInfo*);
};