#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "CGPShopInfoMission.h"

class HNShopScence
	:public cocos2d::Node
	,public CGShopInfoMissionSink
	,public FvSingleton<HNShopScence> 
{
public:
	HNShopScence();
	~HNShopScence();
public:
	virtual bool init();
	virtual void onGPShopInfoListResult(CMD_GP_BackShopInfo* pNetInfo);
public:
	void EnterScence();
	void hideAll();

public:
	void Button_Shop(cocos2d::Ref*,WidgetUserInfo*);
	void ShopButton_Buy(cocos2d::Ref*,WidgetUserInfo*);
private:

	CGPShopInfoMission		m_kShopInfoMission;
};