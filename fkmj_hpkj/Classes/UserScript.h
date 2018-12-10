#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "CGPublicNoticMission.h"

class UserScript
	:public CGPublicNoticMissionSink
	,public FvSingleton<UserScript> 
{
public:
	UserScript();
	~UserScript();
public:
	void getUserDefine(std::string kKey);
	virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
private:
	CGPublicNoticMission m_kPublicNoticMission;
};