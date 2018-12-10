#include "CGPSignInMission.h"

#define BEGINNER_MISSION_NULL		0
#define BEGINNER_MISSION_QUERY	1
#define BEGINNER_MISSION_DONE		2
#define SIGNIN_MISSION_AWARD	3

CGPSignInMission::CGPSignInMission(const char* url, int port)
: CSocketMission(url, port)
{
	mIGPSignInMissionSink = 0;
	mMissionType = BEGINNER_MISSION_NULL;
}

// 设置回调接口
void CGPSignInMission::setMissionSink(IGPSignInMissionSink* pIGPSignInMissionSink)
{
	mIGPSignInMissionSink = pIGPSignInMissionSink;
}
//查询签到
void CGPSignInMission::query()
{
	mMissionType = BEGINNER_MISSION_QUERY;
	start();
}
void CGPSignInMission::done()
{
	mMissionType = BEGINNER_MISSION_DONE;
	start();
}
void CGPSignInMission::award()
{
	mMissionType = SIGNIN_MISSION_AWARD;
	start();
}

void CGPSignInMission::onEventTCPSocketLink()
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketLink\n");

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	switch (mMissionType)
	{		// 查询
		case BEGINNER_MISSION_QUERY:
			{
				CMD_GP_CheckInQueryInfo kNetInfo;
				kNetInfo.dwUserID = pGlobalUserData->dwUserID;
				strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
				send(MDM_GP_USER_SERVICE, SUB_GP_CHECKIN_QUERY,&kNetInfo,sizeof(CMD_GP_CheckInQueryInfo));
				break;
			}
		//执行签到
		case BEGINNER_MISSION_DONE:
			{
				CMD_GP_CheckInDone kNetInfo;
				kNetInfo.dwUserID = pGlobalUserData->dwUserID;
				strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
				strncpy(kNetInfo.szMachineID,DF::shared()->GetMachineID(),countarray(kNetInfo.szMachineID));
				send(MDM_GP_USER_SERVICE, SUB_GP_CHECKIN_DONE,&kNetInfo,sizeof(CMD_GP_CheckInDone));
				break;
			}
		case SIGNIN_MISSION_AWARD:
			{
				CMD_GP_CheckInDone kNetInfo;
				kNetInfo.dwUserID = pGlobalUserData->dwUserID;
				strncpy(kNetInfo.szPassword,pGlobalUserData->szPassword,countarray(kNetInfo.szPassword));
				strncpy(kNetInfo.szMachineID,DF::shared()->GetMachineID(),countarray(kNetInfo.szMachineID));
				send(MDM_GP_USER_SERVICE, SUB_GP_CHECKIN_AWARD,&kNetInfo,sizeof(CMD_GP_CheckInDone));
				break;
			}
	default:
		stop();
		break;
	}
	
}

void CGPSignInMission::onEventTCPSocketShut()
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketShut\n");
}

void CGPSignInMission::onEventTCPSocketError(int errorCode)
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketError code[%d]\n", errorCode);
}

bool CGPSignInMission::onEventTCPSocketRead(int main, int sub, void* data, int size) 
{
	PLAZZ_PRINTF("CGPSignInMission::onEventTCPSocketRead main:%d sub:%d size:%d\n", main, sub, size);
	if (main != MDM_GP_USER_SERVICE)
	{
		return false;
	}
	switch (sub)
	{
		//签到信息
		case SUB_GP_CHECKIN_INFO:
		{

			return onSubQueryInfoResult(data, size);
		}
		//签到结果
		case SUB_GP_CHECKIN_RESULT:
		{
			return onSubDoneResult(data, size);
		}
	}

	return false;
}

bool CGPSignInMission::onSubQueryInfoResult(void* data, int size )
{
	PLAZZ_PRINTF("CGPSignInMission::onSubQueryInfoResult");
	if (size != sizeof(CMD_GP_CheckInInfo))
	{
		ASSERT(false);
		return false;
	}
	CMD_GP_CheckInInfo* pNetInfo = (CMD_GP_CheckInInfo*)data;

	stop();

	if (mIGPSignInMissionSink)
		mIGPSignInMissionSink->onSignInQueryInfoResult(pNetInfo);
	return true;
}

bool CGPSignInMission::onSubDoneResult(void* data, int size)
{
	PLAZZ_PRINTF("CGPSignInMission::onSubDoneResult");
	CMD_GP_CheckInResult* pNetInfo = (CMD_GP_CheckInResult*)data;

	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();
	if (pNetInfo->bSuccessed)
	{
		pGlobalUserData->lUserScore = pNetInfo->lScore;
		pGlobalUserInfo->upPlayerInfo();
	}

	stop();

	if (mIGPSignInMissionSink)
		mIGPSignInMissionSink->onSignInDoneResult(pNetInfo);
	return true;
}