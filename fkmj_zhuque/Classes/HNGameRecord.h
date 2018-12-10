#pragma once
#include "cocos2d.h"
#include "FvSingleton.h"
#include "WidgetDefine.h"
#include "MissionWeiXin.h"
#include "CGPGameRecord.h"

class HNGameRecord
	:public cocos2d::Node
	,public CGGameRecordSink
	,public FvSingleton<HNGameRecord> 
{
public:
	HNGameRecord();
	~HNGameRecord();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();
	std::string timeToString(systemtime kSystem);
	std::string scoreToString(int iScore);
public:
	virtual void onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo);
	virtual void onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo);

public:
	void Button_zhanji(cocos2d::Ref*,WidgetUserInfo*);
	void Button_ShowTotal(cocos2d::Ref*,WidgetUserInfo*);
	void Button_GameRecordBack(cocos2d::Ref*,WidgetUserInfo*);
	void Button_StartGameRecord(cocos2d::Ref*,WidgetUserInfo*);
private:
	CGPGameRecordMission m_kGameRecordMission;
	tagPrivateRandTotalRecord m_kPrivateRandTotalRecord;
};