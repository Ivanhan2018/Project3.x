#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class CGExchangeMissionSink
{
public:
	virtual ~CGExchangeMissionSink(){}
	virtual void onGPExchangeHuaFeiListResult(CMD_GP_BackExchangeHuaFei* pNetInfo){}
	virtual void onGPExchangeDoneResult(bool bSucess,const char* pChar){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPExchangeMission
	: public CCallMission
{
public:
	CGPExchangeMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGExchangeMissionSink* pIGPKefuMissionSink);

	void GetExchangeList();
	void CB_ExchangeList();
	void Net_ExchangeHuaFeiList(void* data, int dataSize);

	void ExchangeItem(int iExchangeID);
	void CB_ExchangeItem(int iExchangeID);
	void Net_ExchangeItemSucess(void* data, int dataSize);
	void Net_ExchangeItemFail(void* data, int dataSize);
private:
	// 回调
	CGExchangeMissionSink* mIGPExchangeHuaFeiMissionSink;
}; 
