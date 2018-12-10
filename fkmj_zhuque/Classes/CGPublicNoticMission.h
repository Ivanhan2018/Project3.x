#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 机器绑定修改提示
//////////////////////////////////////////////////////////////////////////
class CGPublicNoticMissionSink
{
public:
	virtual ~CGPublicNoticMissionSink(){}
	virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription){}
};

//////////////////////////////////////////////////////////////////////////
// 兑换道具
//////////////////////////////////////////////////////////////////////////
class CGPublicNoticMission
	: public CCallMission
{
public:
	CGPublicNoticMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGPublicNoticMissionSink* pIGPKefuMissionSink);

	void PublicNotice(std::string kKey);
	void CB_PublicNotice(std::string kKey);
	void Net_PublicNotice(void* data, int dataSize);
	void Net_OperateFailure(void* data, int dataSize);
private:
	// 回调
	std::string m_kKeyName;
	CGPublicNoticMissionSink* mIGPKefuMissionSink;
}; 
