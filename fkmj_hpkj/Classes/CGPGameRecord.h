#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CCallMission.h"

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
class CGGameRecordSink
{
public:
	virtual ~CGGameRecordSink(){}
	virtual void onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo){}
	virtual void onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo){}
	virtual void onGPBackGameChildRecord(tagPrivateRandRecordChild* pNetInfo){}
};

//////////////////////////////////////////////////////////////////////////
// 游戏记录
//////////////////////////////////////////////////////////////////////////
class CGPGameRecordMission
	: public CCallMission
{
public:
	CGPGameRecordMission(const char* url, int port);
	// 设置回调接口
	void setMissionSink(CGGameRecordSink* pIGPKefuMissionSink);

	void GetGameRecordList(int iUserID);
	void CB_GetGameRecordList(int iUserID);
	void Net_GetGameRecordList(void* data, int dataSize);

	void GetPrivateRandTotalRecord(int iRecordID);
	void CB_PrivateRandTotalRecord(int iRecordID);
	void Net_PrivateRandTotalRecord(void* data, int dataSize);

	void GetPrivateRandChildRecord(int iChildRecordID);
	void CB_PrivateRandChildRecord(int iChildRecordID);
	void Net_PrivateRandChildRecord(void* data, int dataSize);

private:
	// 回调
	CGGameRecordSink* mIGPGameRecordSink;
}; 
