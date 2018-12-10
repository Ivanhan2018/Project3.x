#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"
class IGPTimeAwardMissionSink
{
public:
	virtual ~IGPTimeAwardMissionSink(){}
	virtual void onGPTimeAwardCheckResult(int64 lLastAwardTime, int64 lCurrentTime, int iTimeForGold, int iGold, int iGoldLimit){}
	virtual void onGPTimeAwardGetResult(int32 iGold){}
};

//////////////////////////////////////////////////////////////////////////
// 邮件任务
//////////////////////////////////////////////////////////////////////////
class CGPTimeAwardMission
	: public CSocketMission
{
public:
	CGPTimeAwardMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPTimeAwardMissionSink* pIGPTimeAwardMissionSink);
	// 查询时间奖励
	void checkTimeAward();
	// 领取时间奖励
	void getTimeAward();
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 查询时间奖励
	bool onSubTimeAwardCheck(void* data, int size);
	// 获取时间奖励结果
	bool onSubTimeAwardGet(void* data, int size);

private:
	// 回调
	IGPTimeAwardMissionSink* mIGPTimeAwardMissionSink;

	// 任务类型
	uint8 mMissionType;
}; // CGPTimeAwardMission
