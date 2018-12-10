#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class CGAddRankMissionSink
{
public:
	virtual ~CGAddRankMissionSink(){}
	virtual void onGPAwardInfoResult(CMD_GP_BackAddBankAwardInfo* pNetInfo){}
	virtual void onGPBackAddBankResult(CMD_GP_BackAddBank* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPAddRankMission
	: public CCallMission
{
public:
	CGPAddRankMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGAddRankMissionSink* pIGPKefuMissionSink);

	void GetRankAwardConfig();
	void CB_GetRankAwardConfig();
	void Net_RankAwardConfig(void* data, int dataSize);

	void GetAddBank(int iRankIdex);
	void CB_GetAddBank(int iRandIdex);
	void Net_AddBank(void* data, int dataSize);

private:
	// 回调
	CGAddRankMissionSink* mIGPAddRankMissionSink;
}; 
