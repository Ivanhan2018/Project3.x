#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

//////////////////////////////////////////////////////////////////////////
// 修改签名提示
//////////////////////////////////////////////////////////////////////////
class IGPUnderWriteMissionSink
{
public:
	virtual ~IGPUnderWriteMissionSink(){}
	virtual void onGPUnderWriteSuccess(const char* szDescription){}
	virtual void onGPUnderWriteFailure(const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 修改个性签名
//////////////////////////////////////////////////////////////////////////
class CGPUnderWriteMission
	: public CSocketMission
{
public:
	CGPUnderWriteMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPUnderWriteMissionSink* pIGPUnderWriteMissionSink);
	// 个性签名
	void modifyUnderWrite(char szUnderWrite[LEN_UNDER_WRITE]);
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
	IGPUnderWriteMissionSink* mIGPUnderWriteMissionSink;

	//个性签名
	char mUnderWrite[LEN_UNDER_WRITE];
}; // CGPUnderWriteMission
