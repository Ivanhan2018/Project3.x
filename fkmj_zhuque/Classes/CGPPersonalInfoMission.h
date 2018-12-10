#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

#define MISSION_LEVEL		0
#define MISSION_NOTICE		1
#define MISSION_PERSON_INFO 2
/*#define MISSION_		2*/

//////////////////////////////////////////////////////////////////////////
// 修改密码提示
//////////////////////////////////////////////////////////////////////////
class IGPPersonalInfoMissionSink
{
public:
	virtual ~IGPPersonalInfoMissionSink(){}
	virtual void onQueryLevelInfoSuccess(SCORE, SCORE, wchar_t[]){};
	virtual void onQueryLevelUpTipsSuccess(int cur_lv_, int cur_exp_, dword next_exp_, dword reward_gold_, SCORE reward_ingot_){};
};

//////////////////////////////////////////////////////////////////////////
// 修改密码任务
//////////////////////////////////////////////////////////////////////////
class CGPPersonalInfoMission
	: public CSocketMission
{
public:
	CGPPersonalInfoMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(IGPPersonalInfoMissionSink* pIGPPersonalInfoMissionSink);
// 	// 修改登陆密码
// 	void modifyLogin(const char* szSrc, const char* szDst);
// 	// 修改银行柜密码
// 	void modifyInsure(const char* szSrc, const char* szDst);

	void requestLevelInfo();
	//////////////////////////////////////////////////////////////////////////
	///< 请求人物金币元宝信息
	void requesttPersionInfo();
	// ISocketEngineSink
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);
private:
	bool need_request_lvInfo;

// 	// 操作成功
// 	bool onSubOperateSuccess(void* data, int size);
// 	// 操作失败
// 	bool onSubOperateFailure(void* data, int size);

	bool onQueryLevelInfoSuccess(void *data, int size_);
	bool onQueryLevelUpTipsSuccess(void *data, int size_);
private:
	// 任务类型
	uint8 mMissionType;
	// 回调
	IGPPersonalInfoMissionSink* mIGPPersonalInfoMissionSink;
}; // CGPPasswordMission

