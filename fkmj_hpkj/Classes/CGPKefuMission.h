#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class IGPKefuMissionSink
{
public:
	virtual ~IGPKefuMissionSink(){}
	virtual void onGPKefuResult(const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPKefuMission
	: public CSocketMission
{
public:
	CGPKefuMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPKefuMissionSink* pIGPKefuMissionSink);
	// 兑换道具
	void Kefu();
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 兑换结果
	bool onSubKefuResult(void* data, int size);
	
private:
	// 回调
	IGPKefuMissionSink* mIGPKefuMissionSink;
}; // CGPKefuMission
