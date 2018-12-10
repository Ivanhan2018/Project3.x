#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////

class ICGPRouletteMissionSink
{
public:
	virtual ~ICGPRouletteMissionSink(){}
	virtual void onRouletteUeserInfo(CMD_GP_BackRouletteUeserInfo* pNetInfo){}
	virtual void onRouletteDone(CMD_GP_BackRouletteDone* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////

class CGPRouletteMission
	: public CCallMission
{
	//函数定义
public:
	//构造函数
	CGPRouletteMission(const char* url, int port);
	//析构函数
	virtual ~CGPRouletteMission();

	void setMissionSink(ICGPRouletteMissionSink* pSink);

	//网络事件
public:
	void PerformRouletteUeserInfo(int iUserID,int iVaildDay = -1);
	void CB_RouletteUeserInfo(int iUserID,int iVaildDay);
	void Net_RouletteUeserInfo(void* data, int dataSize);

	void PerformRouletteDone(int iUserID);
	void CB_RouletteDone(int iUserID);
	void Net_RouletteDone(void* data, int dataSize);

private:
	// 回调
	ICGPRouletteMissionSink* m_pRouletteMissionSink;
}; 
