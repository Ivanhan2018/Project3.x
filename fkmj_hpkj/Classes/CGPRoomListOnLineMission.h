#pragma once
#include "cocos2d.h"
#include "PlatformHeader.h"
#include "CSocketMission.h"

//////////////////////////////////////////////////////////////////////////////////

//列表任务
class CGPRoomListOnLineMission
	: public CSocketMission
{
public:
	CGPRoomListOnLineMission(const char* url, int port);

	//变量定义
protected:
	bool							m_bUpdateOnLine;					//更新人数

	//函数定义
public:
	//构造函数
	CGPRoomListOnLineMission();
	//析构函数
	virtual ~CGPRoomListOnLineMission();

	//重载函数
public:
	virtual void onEventTCPSocketLink();
	virtual void onEventTCPSocketShut();
	virtual void onEventTCPSocketError(int errorCode);
	virtual bool onEventTCPSocketRead(int main, int sub, void* data, int dataSize);

	//功能函数
public:
	//更新人数
	void UpdateOnLineInfo();
};
