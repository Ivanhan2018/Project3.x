#include "CGPTimeAwardMission.h"

#define MISSION_TIME_AWARD_NULL		0
#define MISSION_TIME_AWARD_CHECK	1
#define MISSION_TIME_AWARD_GET		2

CGPTimeAwardMission::CGPTimeAwardMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPTimeAwardMissionSink = 0;
	mMissionType = MISSION_TIME_AWARD_NULL;
}

// 设置回调接口
void CGPTimeAwardMission::setMissionSink(IGPTimeAwardMissionSink* pIGPTimeAwardMissionSink)
{
	mIGPTimeAwardMissionSink = pIGPTimeAwardMissionSink;
}

void CGPTimeAwardMission::checkTimeAward()
{
	mMissionType = MISSION_TIME_AWARD_CHECK;
	start();
}

void CGPTimeAwardMission::getTimeAward()
{
	mMissionType = MISSION_TIME_AWARD_GET;
	start();
}

void CGPTimeAwardMission::onEventTCPSocketLink()
{
	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{
	case MISSION_TIME_AWARD_CHECK:
		{
			//变量定义
		}
		break;
	case MISSION_TIME_AWARD_GET:
		{
			//变量定义
		}
		break;
	default:
		stop();
		break;
	}
	
}

void CGPTimeAwardMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPTimeAwardMission::onEventTCPSocketShut\n");
}

void CGPTimeAwardMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPTimeAwardMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPTimeAwardMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPTimeAwardMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_REMOTE_SERVICE)
	{
		return false;
	}

	switch (sub)
	{
		//邮件列表
	case SUB_GP_S_TIME_AWARD_CHECK:	return onSubTimeAwardCheck(data, size);
		//获取邮件奖励结果
	case SUB_GP_S_TIME_AWARD_GET:	return onSubTimeAwardGet(data, size);
	}

	return false;
}

bool CGPTimeAwardMission::onSubTimeAwardCheck(void* data, int size)
{
	stop();

	do 
	{
		if (mIGPTimeAwardMissionSink == 0)
			break;

	
	} while (0);
	
	return true;
}


//获取邮件排行榜奖励结果
bool CGPTimeAwardMission::onSubTimeAwardGet(void* data, int size)
{
	stop();

	do 
	{
		if (mIGPTimeAwardMissionSink == 0)
			break;
	} while (0);

	return true;
}