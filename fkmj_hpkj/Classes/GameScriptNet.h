#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "CGPublicNoticMission.h"

class GameScriptNet
	:public cocos2d::Ref
	,public CGPublicNoticMissionSink
	,public FvSingletonBase<GameScriptNet>
{
public:
	GameScriptNet();
	~GameScriptNet();

public:
	void LogonSucess();
	void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
	void OnGetConfig();
public:
	CGPublicNoticMission m_kPublicNoticMission;
};