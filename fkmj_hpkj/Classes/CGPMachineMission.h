#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class IGPMachineMissionSink
{
public:
	virtual ~IGPMachineMissionSink(){}
	virtual void onGPMachineSuccess(const char* szDescription){}
	virtual void onGPMachineFailure(const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改
//////////////////////////////////////////////////////////////////////////
class CGPMachineMission
	: public CSocketMission
{
public:
	CGPMachineMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPMachineMissionSink* pIGPMachineMissionSink);
	// 解绑或绑定机器
	void modifyMachine(const char* szInsurePassword, uint8 cbBind);
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	// 操作成功
	bool onSubOperateSuccess(void* data, int size);
	// 操作失败
	bool onSubOperateFailure(void* data, int size);
	
private:
	// 回调
	IGPMachineMissionSink* mIGPMachineMissionSink;

	char mInsurePassword[LEN_PASSWORD];
	uint8 mBind;
}; // CGPMachineMission
