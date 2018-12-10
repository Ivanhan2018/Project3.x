#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

#define MISSION_PASSWORD_NULL		0
#define MISSION_PASSWORD_LOGIN		1
#define MISSION_PASSWORD_INSURE		2

//////////////////////////////////////////////////////////////////////////
// 修改密码提示
//////////////////////////////////////////////////////////////////////////
class IGPPasswordMissionSink
{
public:
	virtual ~IGPPasswordMissionSink(){}
	virtual void onGPPasswordSuccess(int type, const char* szDescription){}
	virtual void onGPPasswordFailure(int type, const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 修改密码任务
//////////////////////////////////////////////////////////////////////////
class CGPPasswordMission
	: public CSocketMission
{
public:
	CGPPasswordMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPPasswordMissionSink* pIGPPasswordMissionSink);
	// 修改登陆密码
	void modifyLogin(const char* szSrc, const char* szDst);
	// 修改银行柜密码
	void modifyInsure(const char* szSrc, const char* szDst);
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
	// 任务类型
	uint8 mMissionType;
    // 回调
	IGPPasswordMissionSink* mIGPPasswordMissionSink;

	//////////////////////////////////////////////////////////////////////////
	// 修改登陆密码
	//////////////////////////////////////////////////////////////////////////
	// 原始密码
	char mSrcLogin[LEN_PASSWORD];
	// 目标密码
	char mDstLogin[LEN_PASSWORD];

	//////////////////////////////////////////////////////////////////////////
	// 修改银行密码
	//////////////////////////////////////////////////////////////////////////
	// 原始密码
	char mSrcInsure[LEN_PASSWORD];
	// 目标密码
	char mDstInsure[LEN_PASSWORD];
}; // CGPPasswordMission
