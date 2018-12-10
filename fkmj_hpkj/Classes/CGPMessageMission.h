#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

class IGPMessageMissionSink
{
public:
	virtual ~IGPMessageMissionSink(){}
	virtual void onGPMessageListItem(int index, int TopNumber, int RankNumber, int score, const systemtime& datetime){}
	virtual void onGPMessageAwardResult(const std::string& szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 邮件任务
//////////////////////////////////////////////////////////////////////////
class CGPMessageMission
	: public CSocketMission
{
public:
	CGPMessageMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPMessageMissionSink* pIGPMessageMissionSink);
	// 获取邮件列表
	void getMessageList();
	// 领取排行榜奖励
	void getRankAward(int iRankNumber);
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 获取邮件列表
	bool onSubMessageList(void* data, int size);
	// 获取邮件排行榜奖励结果
	bool onSubMessageAwardResult(void* data, int size);

private:
	// 回调
	IGPMessageMissionSink* mIGPMessageMissionSink;

	// 任务类型
	uint8 mMissionType;
	// 排行版排名
	int mRankNumber;
}; // CGPMessageMission
