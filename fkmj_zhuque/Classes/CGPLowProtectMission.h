#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CCallMission.h"


///< 回调
class IGPLowProtectMissionLink
{
public:
	virtual ~IGPLowProtectMissionLink(){}
	virtual void onLowProtectQueryInfoResult(SCORE lScoreCondition, SCORE lScoreAmount, byte cbTakeTimes) {};
	virtual void onLowProtectDoneResult(bool bSuccessed, SCORE lGameScore, const char* szDescription) {};
};

class CGPLowProtectMission
	: public CCallMission
{
public:
	CGPLowProtectMission(const char * url, int port);
	// 设置回调接口
	void setMissionSink(IGPLowProtectMissionLink* pIGPLowProtectMissionSink);
	///< 请求账户低保信息
	void requickLowProtectInfo();
	void CB_requickLowProtectInfo();
	void Net_requickLowProtectInfo(void * data, int size);
	///< 请求领取低保
	void obtainLowProtect();
	void CB_obtainLowProtect();
	void Net_onObtainLowProtect(void * data, int size);

private:
	// 回调
	IGPLowProtectMissionLink* mIGPLowProtectMissionSink;
};