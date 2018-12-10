#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

class IGPSignInMissionSink
{
public:
	virtual ~IGPSignInMissionSink(){}
	virtual void onSignInQueryInfoResult(CMD_GP_CheckInInfo* pNetInfo) {};
	virtual void onSignInDoneResult(CMD_GP_CheckInResult* pNetInfo) {};
};

//////////////////////////////////////////////////////////////////////////
// 签到任务
//////////////////////////////////////////////////////////////////////////
class CGPSignInMission
	: public CSocketMission
{
public:
	CGPSignInMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPSignInMissionSink* pIGPSignInMissionSink);
	//查询签到
	void query();
	//签到
	void done();
	//领取奖励
	void award();
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);

private:
	bool onSubQueryInfoResult(void* data, int size);
	bool onSubDoneResult(void* data, int size);

private:
	// 回调
	IGPSignInMissionSink* mIGPSignInMissionSink;

	// 任务类型
	uint8 mMissionType;
}; // CGPMessageMission
