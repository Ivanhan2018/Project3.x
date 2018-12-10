#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

class IGPBeginnerMissionSink
{
public:
	virtual ~IGPBeginnerMissionSink(){}
	virtual void onBeginnerQueryInfoResult(CMD_GP_BeginnerInfo* pNetInfo) {};
	virtual void onBeginnerDoneResult(CMD_GP_BeginnerResult* pNetInfo) {};
};

//////////////////////////////////////////////////////////////////////////
// 签到任务
//////////////////////////////////////////////////////////////////////////
class CGPBeginnerMission
	: public CSocketMission
{
public:
	CGPBeginnerMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPBeginnerMissionSink* pIGPSignInMissionSink);
	//查询签到
	void query();
	//签到
	void done();
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
	IGPBeginnerMissionSink* mIGPSignInMissionSink;

	// 任务类型
	uint8 mMissionType;
}; // CGPMessageMission
