#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

#define INSURE_MISSION_SAVE		1
#define INSURE_MISSION_TAKE		2
#define INSURE_MISSION_TRANSFER	3
#define INSURE_MISSION_QUERY	4
#define INSURE_MISSION_ENABLE	5

class IGPInsureMissionSink
{
public:
	virtual ~IGPInsureMissionSink(){}
	virtual void onInsureInfo(){}														//银行资料													
	virtual void onInsureSuccess(int type, const char* szDescription){}					//银行成功
	virtual void onInsureFailure(int type, const char* szDescription){}					//    失败
	virtual bool onInsureTransferConfirm(const char* szMessage){ return false; }			//    转账
	virtual void onInsureEnableResult(int type, const char* szMessage) {}				//    开通
};


class CGPInsureMission
	: public CSocketMission
{
public:
	CGPInsureMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPInsureMissionSink* pIGPInsureMissionSink);
	// 查询
	void query();
	// 存入
	void save(int64 score);
	// 取出
	void take(int64 score, const char* szInsurePass);
	// 转账
	void transfer(int64 score, const char* szInsurePass, const char* szNickName, uint8 cbByNickName);
	// 开通
	void enable(const char* szLogonPass, const char* szInsurePass);
	//////////////////////////////////////////////////////////////////////////
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:

	//转账查询
	bool onSubQueryInfoResult(void* data, int size);
	//银行资料
	bool onSubInsureInfo(void* data, int size);
	//银行成功
	bool onSubInsureSuccess(void* data, int size);
	//银行失败
	bool onSubInsureFailure(void* data, int size);
	// 操作成功
	bool onSubOperateSuccess(void* data, int size);
	// 操作失败
	bool onSubOperateFailure(void* data, int size);
	
private:
	// 任务类型
	uint8 mMissionType;
	// 回调接口
	IGPInsureMissionSink* mIGPInsureMissionSink;

	// 数量
	int64 mScore;

	// 银行密码
	char mInsurePass[LEN_PASSWORD];
	// ID,昵称标识
	uint8 mByNickName;
	// ID,昵称
	char mNickName[LEN_NICKNAME];
	// 登录密码
	char mLogonPass[LEN_PASSWORD];

}; // CGPInsureMission
