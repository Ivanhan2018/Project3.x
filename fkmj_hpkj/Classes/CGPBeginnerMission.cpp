#include "CGPBeginnerMission.h"

#define BEGINNER_MISSION_NULL		0
#define BEGINNER_MISSION_QUERY		1
#define BEGINNER_MISSION_DONE		2

CGPBeginnerMission::CGPBeginnerMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPSignInMissionSink = 0;
	mMissionType = BEGINNER_MISSION_NULL;
}

// 设置回调接口
void CGPBeginnerMission::setMissionSink(IGPBeginnerMissionSink* pIGPSignInMissionSink)
{
	mIGPSignInMissionSink = pIGPSignInMissionSink;
}
//查询签到
void CGPBeginnerMission::query()
{
	mMissionType = BEGINNER_MISSION_QUERY;
	start();
}
void CGPBeginnerMission::done()
{
	mMissionType = BEGINNER_MISSION_DONE;
	start();
}

void CGPBeginnerMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketLink\n");

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{		// 查询
		case BEGINNER_MISSION_QUERY:
			{
				CMD_GP_BeginnerQueryInfo kNetInfo;
				kNetInfo.dwUserID = pGlobalUserData->dwUserID;
				strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
				send(MDM_GP_USER_SERVICE, SUB_GP_BEGINNER_QUERY,&kNetInfo,sizeof(CMD_GP_CheckInQueryInfo));
				break;
			}
		//执行签到
		case BEGINNER_MISSION_DONE:
			{
				CMD_GP_BeginnerDone kNetInfo;
				kNetInfo.dwUserID = pGlobalUserData->dwUserID;
				strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
				strncpy(kNetInfo.szMachineID,DF::shared()->GetMachineID(),countarray(kNetInfo.szMachineID));
				send(MDM_GP_USER_SERVICE, SUB_GP_BEGINNER_DONE,&kNetInfo,sizeof(CMD_GP_CheckInDone));
				break;
			}
	default:
		stop();
		break;
	}
	
}

void CGPBeginnerMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPBeginnerMission::onEventTCPSocketShut\n");
}

void CGPBeginnerMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPBeginnerMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPBeginnerMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPBeginnerMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}
	switch (sub)
	{
		//签到信息
		case SUB_GP_BEGINNER_INFO:
		{

			return onSubQueryInfoResult(data, size);
		}
		//签到结果
		case SUB_GP_BEGINNER_RESULT:
		{
			return onSubDoneResult(data, size);
		}
	}

	return false;
}

bool CGPBeginnerMission::onSubQueryInfoResult(void* data, int size )
{
	PLAZZ_PRINTF("CGPBeginnerMission::onSubQueryInfoResult");
	if (size != sizeof(CMD_GP_BeginnerInfo))
	{
		ASSERT(false);
		return false;
	}
	CMD_GP_BeginnerInfo* pNetInfo = (CMD_GP_BeginnerInfo*)data;

	stop();

	if (mIGPSignInMissionSink)
		mIGPSignInMissionSink->onBeginnerQueryInfoResult(pNetInfo);
	return true;
}

bool CGPBeginnerMission::onSubDoneResult(void* data, int size)
{
	PLAZZ_PRINTF("CGPBeginnerMission::onSubDoneResult");
	CMD_GP_BeginnerResult* pNetInfo = (CMD_GP_BeginnerResult*)data;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pNetInfo->bSuccessed)
	{
		if (pNetInfo->lAwardType == CMD_GP_BeginnerInfo::Type_Gold)
		{
			pGlobalUserData->lUserScore = pNetInfo->lAwardCout;
			pGlobalUserInfo->upPlayerInfo();
		}
	}

	stop();

	if (mIGPSignInMissionSink)
		mIGPSignInMissionSink->onBeginnerDoneResult(pNetInfo);
	return true;
}