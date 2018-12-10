#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////

class ICGUserInGameServerInfoSink
{
public:
	virtual ~ICGUserInGameServerInfoSink(){}
	virtual void onUserInGameServerID(CMD_GP_InGameSeverID* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////

class CGUserInGameServerInfo
	: public CCallMission
{
	//函数定义
public:
	//构造函数
	CGUserInGameServerInfo(const char* url, int port);
	//析构函数
	virtual ~CGUserInGameServerInfo();

	void setMissionSink(ICGUserInGameServerInfoSink* pSink);

	//网络事件
public:
	void PerformInGameServerID(int iUserID);
	void CB_InGameServerID(int iUserID);
	void Net_InGameServerID(void* data, int dataSize);

private:
	// 回调
	ICGUserInGameServerInfoSink* m_pInGameServerInfoSink;
}; 
