#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "MissionWeiXin.h"
#include "CGPGameRecord.h"

class HNSetScence
	:public cocos2d::Node
	,public FvSingleton<HNSetScence> 
{
public:
	HNSetScence();
	~HNSetScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();

public:
	void SetBarSider1(cocos2d::Ref*,WidgetUserInfo*);
	void SetBarSider2(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HideSet(cocos2d::Ref*,WidgetUserInfo*);
	void Button_HomeSet(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameSet(cocos2d::Ref* pNode,WidgetUserInfo* pUserInfo);
};