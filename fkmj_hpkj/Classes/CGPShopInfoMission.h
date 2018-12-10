#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class CGShopInfoMissionSink
{
public:
	virtual ~CGShopInfoMissionSink(){}
	virtual void onGPShopInfoListResult(CMD_GP_BackShopInfo* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPShopInfoMission
	: public CCallMission
{
public:
	CGPShopInfoMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGShopInfoMissionSink* pIGPKefuMissionSink);

	void GetShopInfoList();
	void CB_GetShopInfoList();
	void Net_ShopInfoList(void* data, int dataSize);

	void UpShopOder(CMD_GP_UpShopOder kOder);
	void CB_UpShopOder();
	void Net_OperateSucess(void* data, int dataSize);
	void Net_OperateFailure(void* data, int dataSize);
private:
	// 回调
	CMD_GP_UpShopOder		m_kShopUpOder;
	CGShopInfoMissionSink* mIGPShopInfoMissionSink;
}; 
